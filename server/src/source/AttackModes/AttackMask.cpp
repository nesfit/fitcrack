/**
 * @file AttackMask.cpp
 * @brief Source file for creation of Mask Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackMask.h>
#include <algorithm>


CAttackMask::CAttackMask(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
{

}


bool CAttackMask::makeWorkunit()
{
    /** Create the workunit instance first */
    if (!m_workunit && !generateWorkunit())
        return false;

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE],name2[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];
    const char* infiles[2];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);

    /** Load the workunit mask to object */
    PtrMask workunitMask = m_sqlLoader->loadMask(m_workunit->getMaskId());

    /** Append mode, mask, start_index, stop_index to config */
    retval = config.download_path(name1, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - config. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream f;
    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open config BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << generateBasicConfig(m_job->getAttackMode(), m_job->getAttackSubmode(), m_job->getDistributionMode(),
                             m_job->getName(), m_job->getHashType(), 0,
                             m_job->getHWTempAbort(), m_job->getOptimizedFlag(),
                             "", "", m_job->getCharset1(), m_job->getCharset2(),
                             m_job->getCharset3(), m_job->getCharset4());

    /** Output mask */
    f << "|||mask|String|" << workunitMask->getMask().length() << "|" << workunitMask->getMask() << "|||\n";
    Tools::printDebug("|||mask|String|%d|%s|||\n", workunitMask->getMask().length(), workunitMask->getMask().c_str());

    // Increment mode enabled
    if(workunitMask->getIncrementMin() != 0)
    {
        std::string min = std::to_string(workunitMask->getIncrementMin());
        f << "|||mask_increment_min|UInt|" << min.size() << "|" << min << "|||\n";
        Tools::printDebug("|||mask_increment_min|UInt|%d|%s|||\n", min.size(), min.c_str());

        std::string max = std::to_string(workunitMask->getIncrementMax());
        f << "|||mask_increment_max|UInt|" << max.size() << "|" << max << "|||\n";
        Tools::printDebug("|||mask_increment_max|UInt|%d|%s|||\n", max.size(), max.c_str());
    
        uint64_t totalKeyspace = workunitMask->getKeyspace();
        std::string totalKeyspaceStr = std::to_string(totalKeyspace);
        f << "|||mask_hc_keyspace|BigUInt|" << totalKeyspaceStr.size() << "|" << totalKeyspaceStr << "|||\n";
        Tools::printDebug("|||mask_hc_keyspace|BigUInt|%d|%s|||\n", totalKeyspaceStr.size(), totalKeyspaceStr.c_str());
    }
    else
    {
        /** Output start_index */
        auto skipLine = makeLimitingConfigLine("start_index", "BigUInt", std::to_string(m_workunit->getStartIndex()));
        f << skipLine;
        Tools::printDebug(skipLine.c_str());

        uint64_t maskHcKeyspace = workunitMask->getHcKeyspace();
        uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();

        /** Output stop_index - only if it is not the last workunit in the current mask */
        if (m_workunit->getStartIndex() + workunitHcKeyspace < maskHcKeyspace)
        {
            auto limitLine = makeLimitingConfigLine("hc_keyspace", "BigUInt", std::to_string(workunitHcKeyspace));
            f << limitLine;
            Tools::printDebug(limitLine.c_str());
        }
        else
        {
            /** Otherwise, send whole mask_hc_keyspace for correct progress calculation, --limit is omitted */
            std::string maskHcKeyspaceStr = std::to_string(maskHcKeyspace);
            f << "|||mask_hc_keyspace|BigUInt|" << maskHcKeyspaceStr.size() << "|"
            << maskHcKeyspaceStr << "|||\n";
            Tools::printDebug("|||mask_hc_keyspace|BigUInt|%d|%s|||\n",
                            maskHcKeyspaceStr.size(), maskHcKeyspaceStr.c_str());
        }
    }

    f.close();

    /** Create data file */
    retval = config.download_path(name2, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - data. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f.open(path);
    if (!f.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open data BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    f << m_job->getHashes();
    f.close();


    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
    infiles[0] = name1;
    infiles[1] = name2;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            Config::inTemplatePathMask,
            path,
            config.project_path(path),
            infiles,
            2,
            config
    );

    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to create BOINC workunit. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    restrict_wu_to_host(wu, m_workunit->getBoincHostId());

    m_workunit->setWorkunitId(uint64_t(wu.id));
    m_sqlLoader->addNewWorkunit(m_workunit);

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                          "Workunit succesfully created\n");
    return true;
}


bool CAttackMask::generateWorkunit()
{
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating mask workunit ...\n");

    /** Compute password count */
    uint64_t passCount = getPasswordCountToProcess();
    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Number of real passwords host could compute: %" PRIu64 "\n", passCount);

    if (passCount < getMinPassCount())
    {
        Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
                "Passcount is too small! Falling back to minimum passwords\n");
        passCount = getMinPassCount();
    }

    bool noChainsFound = false;
    PtrMask currentMask;
    uint64_t hcKeyspace = 0;
    uint64_t maskIndex = 0;

    uint64_t mergeMasksEnabled = m_sqlLoader->getEnableMergeMasks();
    // If enabled, try to find the longest mask chain with keyspace sum lower than host power
    if(mergeMasksEnabled && passCount != 0)
    {
        // Filter new masks
        std::vector<PtrMask> allMasks = m_job->getMasks();
        std::vector<PtrMask> newMasks;
        for (PtrMask & mask : allMasks)
        {
            if(mask->getCurrentIndex() == 0 && mask->getIncrementMin() == 0)
                newMasks.push_back(mask);
        }

        // Sort them by length in descending order
        std::sort(newMasks.begin(), newMasks.end(), [](const PtrMask& a, const PtrMask& b){
                return a->getLength() > b->getLength();
             } );

        // Find all mask chains
        std::vector<std::vector<PtrMask>> chains;
        for (PtrMask & mask : newMasks)
        {
            // Current mask keyspaces
            uint64_t maskHcKeyspace = mask->getHcKeyspace();
            uint64_t maskKeyspace = mask->getKeyspace();

            // Current chain
            std::vector<PtrMask> mergedMasks;
            mergedMasks.push_back(mask);

            // Current chain keyspace sums
            uint64_t sumMaskHcKeyspace = maskHcKeyspace;
            uint64_t sumMaskKeyspace = maskKeyspace;

            PtrMask current = mask;
            int currentLen = 0;

            bool found = true;
            // Iterate through other masks, if any of them is merged into current chain, repeat
            while(found)
            {
                found = false;
                for (PtrMask & innerMask : newMasks)
                {
                    currentLen = current->getLength();
                    int len = innerMask->getLength();

                    // Inner mask must be one mask character shorter than current mask
                    if(len != currentLen - 1)
                    {
                        continue;
                    }

                    // Masks must be identical, except for the last mask character
                    if(!current->compare(innerMask, currentLen - 1))
                    {
                        continue;
                    }

                    // Host power (passCount) must be equal or greater than the keyspace of all merged masks combined
                    if(sumMaskKeyspace + innerMask->getKeyspace() > passCount)
                    {
                        continue;
                    }
                        
                    found = true;
                    mergedMasks.push_back(innerMask);
                    sumMaskHcKeyspace += innerMask->getHcKeyspace();
                    sumMaskKeyspace += innerMask->getKeyspace();
                    // Inner mask is now merged into the end of the chain, thus becoming the current mask
                    current = innerMask;
                    break;
                }
            }
            // Current mask has changed, merge must have occured - create new chain
            if(current != mask)
            {
                chains.push_back(mergedMasks);
            }
        }

        // Sort chains by length in descending order
        std::sort(chains.begin(), chains.end(), [](const std::vector<PtrMask>& a, const std::vector<PtrMask>& b){
                return a.size() > b.size();
             } );

        // At least one chain exists, increment mode can be used
        if(chains.size() > 0)
        {
            // Use the first (longest) chain
            std::vector<PtrMask> chain = chains.front();
            currentMask = chain.front();

            uint64_t sumMaskHcKeyspace = 0;
            uint64_t sumMaskKeyspace = 0;

            // Calculate keyspace sums
            // HcKeyspace sum will be inaccurate due to different mask division factors!
            for (PtrMask & mask : chain)
            {
                sumMaskHcKeyspace += mask->getHcKeyspace();
                sumMaskKeyspace += mask->getKeyspace();
            }

            hcKeyspace = sumMaskHcKeyspace;

            // Get increment min and max from the first and the last mask in the chain
            uint64_t min = (chain.back())->getLength();
            uint64_t max = (chain.front())->getLength();

            // Update current mask
            currentMask->updateMask(currentMask->getMask(), sumMaskKeyspace, sumMaskHcKeyspace, min, max);
            
            // Remove merged masks
            for (PtrMask & mask : chain)
            {
                if(mask->getId() == currentMask->getId())
                    continue;

                m_job->removeMask(mask->getId());
            }

            //Reload masks in job
            m_job->loadMasks(false);
        }
        // No chains found, increment wont be used
        else
        {
            noChainsFound = true;
        }
    }

    if(!mergeMasksEnabled || noChainsFound || passCount == 0)
    {
        std::vector<PtrMask> maskVec = m_job->getMasks();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Masks left for this job: %" PRIu64 "\n", maskVec.size());

        /** Find the following mask */
        // Benchmark should use the longest one for most accurate results
        bool findLongest = (passCount == 0) ? true : false;
        currentMask = FindCurrentMask(maskVec, false, findLongest);

        if (!currentMask)
        {
            /** No masks found, no workunit could be generated */
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "No masks found for this job\n");
            return false;
        }
        uint64_t maskHcKeyspace = currentMask->getHcKeyspace();
        uint64_t maskKeyspace = currentMask->getKeyspace();

        uint64_t hcDivisionFactor = maskKeyspace/maskHcKeyspace;
        //round up
        hcKeyspace = (passCount+hcDivisionFactor-1)/hcDivisionFactor;

        maskIndex = currentMask->getCurrentIndex();

        if (maskIndex + hcKeyspace > maskHcKeyspace)
        {
            /** Host is too powerful for this mask, it will finish it */
            hcKeyspace = maskHcKeyspace - maskIndex;
            Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                    "Adjusting #passwords, mask too small, new #: %" PRIu64 "\n", hcKeyspace);
        }
    }

    /** Create new mask workunit */
    m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), maskIndex, 0, hcKeyspace,
                         currentMask->getId(), 0, false, 0, false);

    /** Update indexes for job and mask*/
    m_job->updateIndex(m_job->getCurrentIndex() + hcKeyspace);
    currentMask->updateIndex(maskIndex + hcKeyspace);

    return true;
}
