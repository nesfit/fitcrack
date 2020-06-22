/**
 * @file AttackBench.cpp
 * @brief Source file for creation of Benchmark workunit
 * @authors see AUTHORS
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackBenchAll.h>


CAttackBenchAll::CAttackBenchAll(PtrJob &job, PtrHost &host, CSqlLoader *sqlLoader):
AttackMode(job, host, 0, sqlLoader)
{}


bool CAttackBenchAll::makeWorkunit()
{
	/** Create the workunit first */
	generateWorkunit();

	DB_WORKUNIT wu;
	char name1[Config::SQL_BUF_SIZE], path[Config::SQL_BUF_SIZE];
	const char* infiles[1];
	int retval;

	/** Make a unique name for the workunit and its config file */
	std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);

	/** Create the config file */
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

	f << "|||mode|String|1|a|||\n";
	f.close();

	/** Fill in the workunit parameters */
	wu.clear();
	wu.appid = Config::app->id;
	safe_strcpy(wu.name, name1);
	//five hours ought to be enough
	wu.delay_bound = 18000;
	infiles[0] = name1;

	setDefaultWorkunitParams(&wu);

	/** Register the workunit with BOINC */
	std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
	retval = create_work(
		wu,
		Config::inTemplatePathBench,
		path,
		config.project_path(path),
		infiles,
		1,
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


bool CAttackBenchAll::generateWorkunit()
{
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
			"Generating benchmark workunit ...\n");

	/** Create benchmark workunit */
	m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), 0, 0, 0, 0, 0, false, 0, false);
	return true;
}