/**
 * @file AttackRules.cpp
 * @brief Source file for creation of Rules Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackRules.h>

#include <algorithm>
#include <cmath>    /**< std::round */


CAttackRules::CAttackRules(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
{

}


bool CAttackRules::makeWorkunit() {
    /** Create the workunit instance first */
    std::string mergedDictsPath =
        Config::dictDir + ".dict_" + std::to_string(m_job->getId()) + ".txt";
    if (!generateWorkunit()) {
      remove(mergedDictsPath.c_str());
      return false;
    }

    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE], name2[Config::SQL_BUF_SIZE], name3[Config::SQL_BUF_SIZE],
            name4[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];

    const char *infiles[4];
    int retval;

    // Check if workunit contains split rules
    bool split = (m_workunit->getRuleCount() != 0);

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);

    // Dictionary name is unique if fragmented on server or if workunit contains split rules
    if (split || m_job->getDistributionMode() == 0)
      std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_%d_%d.dict",
                    Config::appName, Config::startTime, Config::seqNo++);
    else if (m_job->getDistributionMode() == 1)
      std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_dict_%" PRIu64 "",
                    Config::appName, m_job->getId());

    // Rule filename must be unique for split rules
    if(split)
    {
      std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_%d_%d.rule",
                    Config::appName, Config::startTime, Config::seqNo++);
    }
    else
    {
      /** Same name of rules file - for sticky flag to work */
      std::snprintf(name4, Config::SQL_BUF_SIZE, "%s_rules_%" PRIu64 "", Config::appName, m_job->getId());
    }

    /** Append mode to config */
    retval = config.download_path(name1, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - config. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream configFile;
    configFile.open(path);
    if (!configFile.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open config BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    configFile << generateBasicConfig(m_job->getAttackMode(), m_job->getAttackSubmode(),
                             m_job->getDistributionMode(), m_job->getName(),
                             m_job->getHashType(), 0, m_job->getHWTempAbort(), m_job->getOptimizedFlag());

    // Single password with large number of rules must use slow candidates mode for optimal speed
    if(split || m_job->getSlowCandidatesFlag())
    {
      configFile << "|||slow_candidates|BigUInt|1|1|||\n";
    }

    if (split || m_job->getDistributionMode() == 0) {
      // Number of passwords in the sent dictionary (the dictionary fragment).
      std::string dict1Keyspace = std::to_string(m_workunit->getHcKeyspace());
      configFile << "|||dict1_keyspace|BigUInt|" << dict1Keyspace.size()
                 << "|" << dict1Keyspace << "|||\n";
    } else if (m_job->getDistributionMode() == 1) {
      uint64_t startIndex = m_workunit->getStartIndex();
      std::string skipFromStart = std::to_string(startIndex);
      configFile << "|||start_index|BigUInt|" << skipFromStart.size() << "|"
                 << skipFromStart << "|||\n";
      std::string limit = std::to_string(m_workunit->getHcKeyspace());
      configFile << "|||hc_keyspace|BigUInt|" << limit.size() << "|" << limit
                 << "|||\n";
      // Number of passwords in the sent dictionary (the whole dictionary).
      std::string dict1Keyspace = std::to_string(m_job->getHcKeyspace());
      configFile << "|||dict1_keyspace|BigUInt|" << dict1Keyspace.size()
                 << "|" << dict1Keyspace << "|||\n";
    }

    /** Create data file */
    retval = config.download_path(name2, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - data. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream hashesFile;
    hashesFile.open(path);
    if (!hashesFile.is_open())
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to open data BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    hashesFile << m_job->getHashes();
    hashesFile.close();

    /** Create dict1 file */
    retval = config.download_path(name3, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - dict1. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    try
    {
      if(split)
      {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Creating dictionary fragment with 1 password.\n");

        // Open dictionary containing the password
        uint64_t dictId = m_workunit->getDictionaryId();
        PtrDictionary ptrDict = m_sqlLoader->loadDictionary(dictId);
        std::string dictPath = Config::dictDir + ptrDict->getDictFileName();
        std::ifstream file_descriptor(dictPath);

        std::string password;

        // Get the password
        if(m_workunit->getStartIndex2() == 0)
        {
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "New rule split.\n");
          file_descriptor.seekg(m_workunit->getSplitPos());
          std::getline(file_descriptor, password);
          if (!m_workunit->isDuplicated()) 
          {
            ptrDict->updatePos(file_descriptor.tellg());
          }
        }
        else
        {
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Continuing with existing rule split.\n");
          file_descriptor.seekg(m_workunit->getSplitPos());
          std::getline(file_descriptor, password);
        }
        file_descriptor.close();

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                  "RuleIndex: %" PRIu64 ", RuleCount: %" PRIu64 ".\n", m_workunit->getStartIndex2(), m_workunit->getRuleCount());

        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Password: '%s'\n", password.c_str());
        
        // Create dictionary fragment containing the password
        std::ofstream dictFile;
        dictFile.open(path);
        if (!dictFile.is_open()) {
          Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                                m_host->getBoincHostId(),
                                "Failed to open dict1 BOINC input file!"
                                "Setting job to malformed.\n");
          m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                              Config::JobState::JobMalformed);
          return false;
        }
        dictFile << password << '\n';
        dictFile.close();
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                              m_host->getBoincHostId(),
                              "Workunit dictionary prepared.\n");
      }
      else if (m_job->getDistributionMode() == 0) {
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                              m_host->getBoincHostId(),
                              "Creating dictionary fragment\n");

        /** Load current workunit dictionary */
        PtrDictionary workunitDict = GetWorkunitDict();

        configFile << "|||hex_dict|UInt|1|" << std::to_string(workunitDict->isHexDict()) << "|||\n";

        auto inputDict = makeInputDict(workunitDict, false);

        /** Seek to the right position (faster then skipping line by line) */
        auto filePosInDict = workunitDict->getCurrentPos();
        inputDict->SetCurrentDictPos(filePosInDict);

        /** Add 'keyspace' passwords to dict file */

        uint64_t workunitKeyspace = m_workunit->getHcKeyspace();

        Tools::printDebugHost(
            Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Adding %" PRIu64 " passwords to host dictionary file\n",
            workunitKeyspace);
        auto writtenPasswords =
            inputDict->WritePasswordsTo(workunitKeyspace, path);
        if (writtenPasswords == 0 && workunitKeyspace > 0) {
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                                m_host->getBoincHostId(),
                                "'start_index' parameter is too far away\n");
          if (!m_workunit->isDuplicated()) {
            workunitDict->updateIndex(workunitDict->getHcKeyspace());
            m_job->updateIndex(m_job->getCurrentIndex() -
                               m_workunit->getHcKeyspace());
          }
          return true;
        } else if (writtenPasswords < workunitKeyspace) {
          Tools::printDebugHost(
              Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Ate all passwords, new workunit keyspace: %" PRIu64
              "\nSetting job to Finishing\n",
              writtenPasswords);

          if (!m_workunit->isDuplicated()) {
            m_job->updateIndex(m_job->getCurrentIndex() -
                               m_workunit->getHcKeyspace() + writtenPasswords);
            workunitDict->updateIndex(workunitDict->getHcKeyspace());
          }

          m_workunit->setHcKeyspace(writtenPasswords);
        }

        workunitDict->updatePos(inputDict->GetCurrentDictPos());
      } else if (m_job->getDistributionMode() == 1) {
        uint64_t startIndex = m_workunit->getStartIndex();

        /** Merge dictionaries to one. */
        if (startIndex == 0) {
          std::ofstream mergedDictsFile;
          mergedDictsFile.open(mergedDictsPath);
          if (!mergedDictsFile.is_open()) {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                                  m_host->getBoincHostId(),
                                  "Failed to open merged dictionary! "
                                  "Setting job to malformed.\n");
          }
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                                m_host->getBoincHostId(),
                                "Merging dictionaries\n");
          std::vector<PtrDictionary> dictVec = m_job->getDictionaries();
          for (PtrDictionary &dict : dictVec) {

            std::ifstream dictFile;
            std::string dictPath = Config::dictDir + dict->getDictFileName();
            dictFile.open(dictPath);
            if (!dictFile.is_open()) {
              Tools::printDebugHost(
                  Config::DebugType::Error, m_job->getId(),
                  m_host->getBoincHostId(),
                  "Cannot find dictionary file! Setting job to malformed.\n");
              m_sqlLoader->updateRunningJobStatus(
                  m_job->getId(), Config::JobState::JobMalformed);
              return false;
            }

            mergedDictsFile << dictFile.rdbuf();
          }

          mergedDictsFile.close();
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                                m_host->getBoincHostId(),
                                "Dictionaries merged.\n");
        }

        if (!std::ifstream(path)) {
          std::ofstream dictFile;
          dictFile.open(path);
          if (!dictFile.is_open()) {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                                  m_host->getBoincHostId(),
                                  "Failed to open dict1 BOINC input file! "
                                  "Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                                Config::JobState::JobMalformed);
            return false;
          }

          std::ifstream mergedDictsFile;
          mergedDictsFile.open(mergedDictsPath);
          if (!mergedDictsFile.is_open()) {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                                  m_host->getBoincHostId(),
                                  "Failed to open merged dictionary! "
                                  "Setting job to malformed.\n");
          }

          dictFile << mergedDictsFile.rdbuf();
          dictFile.close();
          mergedDictsFile.close();
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                                m_host->getBoincHostId(),
                                "Workunit dictionary prepared.\n");
        }
      }
    }
    catch(const std::exception &e)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "A dictionary operation failed. Setting job to malformed. Message: %s\n", e.what());
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    configFile.close();

    Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(), "Done.\n");

    /** Create rules file */
    retval = config.download_path(name4, path);
    if (retval)
    {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                "Failed to receive BOINC filename - rules. Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
        return false;
    }

    std::ofstream rulesFile;    
    if(!std::ifstream(path))
    {
        rulesFile.open(path);
        if (!rulesFile.is_open())
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                    "Failed to open rules BOINC input file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        if(m_job->getRules().empty())
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                    "Rules is not set in database! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        std::ifstream rules;
        rules.open((Config::rulesDir + m_job->getRules()).c_str());
        if (!rules.is_open())
        {
            Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
                    "Failed to open rules file! Setting job to malformed.\n");
            m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
            return false;
        }

        // Copy part of the rules
        if(split)
        {
          uint64_t splitStart = m_workunit->getStartIndex2();
          uint64_t splitEnd = splitStart + m_workunit->getRuleCount();

          std::string line;
          uint64_t lineCount = 0;
          while (std::getline(rules, line)) 
          {
              if(lineCount < splitStart)
              {
                lineCount++;
                continue;
              }

              if(lineCount < splitEnd)
              {
                rulesFile << line << '\n';
                lineCount++;
              }
              else
              {
                break;
              }
          }
        }
        // Copy all the rules
        else
        {
          rulesFile << rules.rdbuf();
        }

        rulesFile.close();
    }


    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
    infiles[0] = name1;
    infiles[1] = name2;
    infiles[2] = name3;
    infiles[3] = name4;

    setDefaultWorkunitParams(&wu);

    char * templatePath;
    if(split)
      templatePath = Config::inTemplatePathRuleSplit;
    else
      templatePath = (m_job->getDistributionMode() == 0) ? Config::inTemplatePathRule : Config::inTemplatePathRuleAlt;

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            templatePath,
            path,
            config.project_path(path),
            infiles,
            4,
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


bool CAttackRules::generateWorkunit()
{
        Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Generating dictionary workunit ...\n");
    uint64_t currentIndex = m_job->getCurrentIndex();
    uint64_t jobHcKeyspace = m_job->getHcKeyspace();
    bool split = false;
    /** Check if the job isn't finished */
    if (currentIndex >= jobHcKeyspace)
    {
      if(m_job->getSplitRuleIndex() != 0)
        split = true;
      else
        return false;
    }

    /** Compute password count */
    uint64_t passCount = getPasswordCountToProcess();

    // Real host power, taken from the last benchmark
    uint64_t realPower;
    // Total number of rules in this job
    uint64_t totalRuleCount;

    // Password count may be inaccurate
    if(passCount == m_seconds)
    {
        realPower = m_sqlLoader->getLatestBenchmarkPower(m_host->getBoincHostId(), m_job->getAttackMode(), m_job->getHashType());
        totalRuleCount = m_sqlLoader->getRuleCount(m_job->getRulesId());

        // Split rules if the host is not able to complete a single password in m_seconds
        if(realPower * m_seconds < totalRuleCount)
        {
          split = true;
          passCount = 1;
        }
        // Recalculate password count
        else
        {
          passCount = (realPower * m_seconds) / totalRuleCount;
        }
    }

    if (split || m_job->getDistributionMode() == 0) 
    {
      uint64_t dictIndex = 0;
      uint64_t dictId = 0;
      uint64_t splitRuleIndex = 0;
      uint64_t splitRuleCount = 0;
      uint64_t splitPos = 0;

      PtrDictionary currentDict;

      // Job already started splitting rules and contains unfinished password
      if(split && m_job->getSplitRuleIndex() != 0)
      {
        // Get starting rule index and number of rules remaining in rule file
        splitRuleIndex = m_job->getSplitRuleIndex();
        uint64_t remainingRules = totalRuleCount - splitRuleIndex;

        // Cap the number of rules assigned to this split
        splitRuleCount = (realPower * m_seconds > remainingRules) ? remainingRules : realPower * m_seconds; 

        // Load dictionary id, index and split password dict position
        dictId = m_job->getSplitDictId();
        dictIndex = m_job->getSplitDictIndex();
        splitPos = m_job->getSplitDictPos();

        // Workunit finishes all rules for this passsword, reset job split_* values 
        if(splitRuleCount == remainingRules)
        {
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Rule split complete.\n");
          m_job->removeRuleSplit();
        }
        // Workunit continues with rule splitting, update rule index
        else
        {
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
              "Rule split fragment, new ruleIndex value: %" PRIu64 "\n", splitRuleIndex + splitRuleCount);
          m_job->updateRuleIndex(splitRuleIndex + splitRuleCount);
        }
      }
      else
      {
        /** Load job dictionaries */
        std::vector<PtrDictionary> dictVec = m_job->getDictionaries();
        Tools::printDebugHost(
            Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
            "Dictionaries left for this job: %" PRIu64 "\n", dictVec.size());

        /** Find the following dictionary */
        currentDict = FindCurrentDict(dictVec);

        if (!currentDict) {
          /** No dictionaries found, no workunit could be generated */
          Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                                m_host->getBoincHostId(),
                                "No dictionaries found for this job\n");
          return false;
        }

        dictIndex = currentDict->getCurrentIndex();
        dictId = currentDict->getId();

        // Create new rule split
        if(split)
        {
          splitRuleIndex = 0;

          splitPos = currentDict->getCurrentPos();
          
          // Cap the number of rules assigned to this split
          splitRuleCount = (realPower * m_seconds > totalRuleCount) ? totalRuleCount : realPower * m_seconds; 

          // Create new rule split
          m_job->createRuleSplit(currentDict->getId(), dictIndex, splitPos, splitRuleCount);
        }
        else
        {
          // No split - standard distribution 0 workunit
          uint64_t dictHcKeyspace = currentDict->getHcKeyspace();
          if (dictIndex + passCount > dictHcKeyspace) {
            /** Host is too powerful for this mask, it will finish it */
            passCount = dictHcKeyspace - dictIndex;
            Tools::printDebugHost(
                Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
                "Adjusting #passwords, dictionary too small, new #: %" PRIu64 "\n",
                passCount);
          }
        }
      }

      /** Create the workunit */
      m_workunit = CWorkunit::create(
          m_job->getId(), m_host->getId(), m_host->getBoincHostId(), dictIndex,
          splitRuleIndex, passCount, 0, dictId, false, 0, false, splitRuleCount, splitPos);
      if (!m_workunit)
        return false;

      /** Update the job/dictionary index if current workunit does not continue with rule splitting */
      if(splitRuleIndex == 0)
      {
        m_job->updateIndex(currentIndex + passCount);
        currentDict->updateIndex(dictIndex + passCount);
      }
    } else if (m_job->getDistributionMode() == 1) {
      /** Adjust password count */
      if (currentIndex + passCount > jobHcKeyspace)
        passCount = jobHcKeyspace - currentIndex;

      /** Create the workunit */
      m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(),
                                     m_host->getBoincHostId(), currentIndex, 0,
                                     passCount, 0, 0, false, 0, false);
      if (!m_workunit)
        return false;
      /** Update the job index */
      m_job->updateIndex(currentIndex + passCount);
    }

    return true;
}