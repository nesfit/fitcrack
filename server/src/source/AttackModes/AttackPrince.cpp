/**
 * @file AttackPrince.cpp
 * @brief Source file for creation of Prince Attack
 * @authors David Bolvansky
 * @date 22. 2. 2020
 * @license MIT, see LICENSE
 */
 
#include <AttackPrince.h>


CAttackPrince::CAttackPrince(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
    :   AttackMode(std::move(job), host, seconds, sqlLoader)
{
}


bool CAttackPrince::makeWorkunit()
{
    /** Create the workunit instance first */
    std::string princeDictPath =
        Config::dictDir + ".prince_" + std::to_string(m_job->getId()) + ".txt";
    if (!generateWorkunit()) {
      remove(princeDictPath.c_str());
      return false;
    }

    bool with_rules = m_job->getAttackSubmode() == 1;
    DB_WORKUNIT wu;
    char name1[Config::SQL_BUF_SIZE], name2[Config::SQL_BUF_SIZE],
        name3[Config::SQL_BUF_SIZE], name4[Config::SQL_BUF_SIZE],
        path[Config::SQL_BUF_SIZE];
    const char *infiles[with_rules ? 4 : 3];
    int retval;

    /** Make a unique name for the workunit and its input file */
    std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
    /** Same name of dict file - for sticky flag to work */
    std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_dict_%" PRIu64 "", Config::appName, m_job->getId());
    if (with_rules) {
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

    Tools::printDebug("CONFIG for new workunit:\n");

    /** Output original config from DB */
    configFile << generateBasicConfig(
        m_job->getAttackMode(), m_job->getAttackSubmode(), m_job->getDistributionMode(), m_job->getName(),
        m_job->getHashType(), m_job->getRandomRulesCount(),
        m_job->getHWTempAbort(), m_job->getOptimizedFlag());

    /** Output mode */
    uint64_t startIndex = m_workunit->getStartIndex();
    configFile << "|||mode|String|1|n|||\n";
    std::string skipFromStart = std::to_string(startIndex);
    configFile << "|||skip_from_start|BigUInt|" << skipFromStart.size() << "|" << skipFromStart << "|||\n";
    std::string dictHcKeyspace = std::to_string(m_workunit->getHcKeyspace());
    configFile << "|||dict_hc_keyspace|BigUInt|" << dictHcKeyspace.size() << "|" << dictHcKeyspace << "|||\n";
    std::string casePermute = std::to_string(m_job->getCasePermuteFlag());
    configFile << "|||case_permute|UInt|1|" << casePermute << "|||\n";
    std::string checkDuplicates = std::to_string(m_job->getCheckDuplicatesFlag());
    configFile << "|||check_duplicates|UInt|1|" << checkDuplicates << "|||\n";
    std::string minPasswordLen = std::to_string(m_job->getMinPasswordLen());
    configFile << "|||min_password_len|UInt|" << minPasswordLen.size() << "|" << minPasswordLen << "|||\n";
    std::string maxPasswordLen = std::to_string(m_job->getMaxPasswordLen());
    configFile << "|||max_password_len|UInt|" << maxPasswordLen.size() << "|" << maxPasswordLen << "|||\n";
    std::string minElemInChain = std::to_string(m_job->getMinElemInChain());
    configFile << "|||min_elem_in_chain|UInt|" << minElemInChain.size() << "|" << minElemInChain << "|||\n";
    std::string maxElemInChain = std::to_string(m_job->getMaxElemInChain());
    configFile << "|||max_elem_in_chain|UInt|" << maxElemInChain.size() << "|" << maxElemInChain << "|||\n";

    configFile.close();

    /** Create data file with hashes */
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

    /** Merge dictionaries to one. */
    if (startIndex == 0) {
      Tools::printDebug("Creating PRINCE dictionary.\n");
      std::ofstream princeDictFile;
      princeDictFile.open(princeDictPath);
      if (!princeDictFile.is_open()) {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                              m_host->getBoincHostId(),
                              "Failed to open PRINCE attack dictionary! "
                              "Setting job to malformed.\n");
      }

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
            m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                                Config::JobState::JobMalformed);
            return false;
          }

          princeDictFile << dictFile.rdbuf();
      }

      princeDictFile.close();
    }

    /** Create dictionary file. */
    retval = config.download_path(name3, path);
    if (retval) {
      Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                            m_host->getBoincHostId(),
                            "Failed to receive BOINC filename - dict1. Setting "
                            "job to malformed.\n");
      m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                          Config::JobState::JobMalformed);
      return false;
    }

    if(!std::ifstream(path))
    {
      std::ofstream dictFile;
      dictFile.open(path);
      if (!dictFile.is_open()) {
        Tools::printDebugHost(
            Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
            "Failed to open dict1 BOINC input file! Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                            Config::JobState::JobMalformed);
        return false;
      }

      std::ifstream princeDictFile;
      princeDictFile.open(princeDictPath);
      if (!princeDictFile.is_open()) {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                              m_host->getBoincHostId(),
                              "Failed to open PRINCE attack dictionary! "
                              "Setting job to malformed.\n");
      }

      dictFile << princeDictFile.rdbuf();
      dictFile.close();
      princeDictFile.close();
      Tools::printDebug("Copied PRINCE dictionary to data file.\n");
    }

    if (with_rules) {
      std::ofstream rulesFile;
      /** Create rules file */
      retval = config.download_path(name4, path);
      if (retval) {
        Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                              m_host->getBoincHostId(),
                              "Failed to receive BOINC filename - rules. "
                              "Setting job to malformed.\n");
        m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                            Config::JobState::JobMalformed);
        return false;
      }

      if(!std::ifstream(path))
      {

        rulesFile.open(path);
        if (!rulesFile.is_open()) {
          Tools::printDebugHost(Config::DebugType::Error, m_job->getId(),
                                m_host->getBoincHostId(),
                                "Failed to open rules BOINC input file! Setting "
                                "job to malformed.\n");
          m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                              Config::JobState::JobMalformed);
          return false;
        }

        if (m_job->getRules().empty()) {
          Tools::printDebugHost(
              Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
              "Rules is not set in database! Setting job to malformed.\n");
          m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                              Config::JobState::JobMalformed);
          return false;
        }

        std::ifstream rules;
        rules.open((Config::rulesDir + m_job->getRules()).c_str());
        if (!rules) {
          Tools::printDebugHost(
              Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
              "Failed to open rules file! Setting job to malformed.\n");
          m_sqlLoader->updateRunningJobStatus(m_job->getId(),
                                              Config::JobState::JobMalformed);
          return false;
        }

        rulesFile << rules.rdbuf();
        rulesFile.close();
        rules.close();
      }
    }

    /** Fill in the workunit parameters */
    wu.clear();
    wu.appid = Config::app->id;
    safe_strcpy(wu.name, name1);
    wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
    infiles[0] = name1;
    infiles[1] = name2;
    infiles[2] = name3;
    if (with_rules)
      infiles[3] = name4;

    setDefaultWorkunitParams(&wu);

    /** Register the workunit with BOINC */
    std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
    retval = create_work(
            wu,
            with_rules ? Config::inTemplatePathPrinceRules : Config::inTemplatePathPrince,
            path,
            config.project_path(path),
            infiles,
            with_rules ? 4 : 3,
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

bool CAttackPrince::generateWorkunit() {
  Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                        m_host->getBoincHostId(),
                        "Generating prince workunit ...\n");
  uint64_t currentIndex = m_job->getCurrentIndex();
  uint64_t jobHcKeyspace = m_job->getHcKeyspace();
  /** Check if the job isn't finished */
  if (currentIndex >= jobHcKeyspace)
    return false;

  /** Compute password count */
  uint64_t passwordsRange = getPasswordCountToProcess();
  if (passwordsRange < getMinPassCount()) {
    Tools::printDebugHost(
        Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
        "Passwords range is too small! Falling back to minimum passwords\n");
    passwordsRange = getMinPassCount();
  }

  Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                        m_host->getBoincHostId(),
                        "Current index: %" PRIu64 "\n", currentIndex);
  Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                        m_host->getBoincHostId(),
                        "Job HC Keyspace: %" PRIu64 "\n", jobHcKeyspace);
  Tools::printDebugHost(Config::DebugType::Log, m_job->getId(),
                        m_host->getBoincHostId(),
                        "Passwords range: %" PRIu64 "\n", passwordsRange);

  /** Adjust password range */
  if (currentIndex + passwordsRange > jobHcKeyspace)
    passwordsRange = jobHcKeyspace - currentIndex;

  /** Create the workunit */
  m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(),
                                 m_host->getBoincHostId(), currentIndex, 0,
                                 passwordsRange, 0, 0, false, 0, false);
  if (!m_workunit)
    return false;
  /** Update the job index */
  m_job->updateIndex(currentIndex + passwordsRange);

  return true;
}
