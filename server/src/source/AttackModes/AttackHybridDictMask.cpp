/**
 * @file AttackCombinator.cpp
 * @brief Source file for creation of Combinator Attack
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <AttackHybridDictMask.h>

#include <cmath>    /**< std::round */
#include <sstream>
#include <iomanip>

#include "InputDict.h"


CAttackHybridDictMask::CAttackHybridDictMask(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader)
	:   AttackMode(std::move(job), host, seconds, sqlLoader)
{

}


bool CAttackHybridDictMask::makeWorkunit()
{
	/** Create the workunit first */
	if (!m_workunit && !generateWorkunit())
		return false;

	DB_WORKUNIT wu;
	char name1[Config::SQL_BUF_SIZE], name2[Config::SQL_BUF_SIZE], name3[Config::SQL_BUF_SIZE],
		path[Config::SQL_BUF_SIZE];

	const char* infiles[4];
	int retval;

	/** Make a unique name for the workunit and its input file */
	std::snprintf(name1, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
	std::snprintf(name2, Config::SQL_BUF_SIZE, "%s_%d_%d", Config::appName, Config::startTime, Config::seqNo++);
	/** Same name of the dictionary - for sticky flag to work */
	std::snprintf(name3, Config::SQL_BUF_SIZE, "%s_hybrid_%" PRIu64 ".dict", Config::appName, m_job->getId());

	/** Append mode to config */
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
		m_job->getName(), m_job->getHashType(), 0, m_job->getHWTempAbort(), m_job->getOptimizedFlag(), m_job->getRuleLeft());

	/** Load the workunit mask to object */
	PtrMask workunitMask = GetWorkunitMask();

	// Debug
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
		"In the dictionary, there are %" PRIu64 " passwords\n", m_job->getHcKeyspace());
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
		"The mask can generate %" PRIu64 " passwords\n", workunitMask->getKeyspace());
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
		"Workunit can compute %" PRIu64 " passwords\n", m_workunit->getHcKeyspace());

	auto MakeConfigLinesFromSplitMask = [this](const MaskSplitter::MaskSettings &settings) {
		std::ostringstream cfg;
		cfg<<makeConfigLine("mask", "String", settings.mask);
		for(size_t i = 0; i < settings.customCharsets.size(); ++i)
		{
			std::ostringstream hexCharset;
			hexCharset<<std::hex<<std::setfill('0');
			for(char c: settings.customCharsets[i])
			{
				hexCharset<<std::setw(2)<<+c;
			}
			cfg<<makeConfigLine("charset"+std::to_string(i+1), "String", hexCharset.str());
		}
		return cfg.str();
	};

	try
	{

		/** Check combinator workunit type */
		/** Dictionary is already fragmented, continue fragmenting */
		if (m_workunit->getStartIndex2() != 0)
		{
			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Dictionary is already fragmented, we need to continue\n");

			/** Will ignore 'start_index' passwords */
			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Skipping %" PRIu64 " passwords\n", m_workunit->getStartIndex());

			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Mask for this WU is going to be just a single password");

			/** Append skip and limit to config */
			Tools::printDebug("Adding additional info to CONFIG:\n");

			{
				auto maskSplitter = makeMaskSplitter();
				auto mask = maskSplitter->GetMaskSlice(workunitMask->getMask(), m_workunit->getStartIndex(), 1);
				auto maskConfig = MakeConfigLinesFromSplitMask(mask);
				f<<maskConfig;
				Tools::printDebug(maskConfig.c_str());
			}

			uint64_t workunitStartIndex2 = m_workunit->getStartIndex2();
			auto skipLine = makeLimitingConfigLine("start_index", "BigUInt", std::to_string(workunitStartIndex2));
			f << skipLine;
			Tools::printDebug(skipLine.c_str());

			/** Check if we reach the end of password keyspace in 1st dict */
			if (m_workunit->getHcKeyspace() + workunitStartIndex2 >= m_job->getHcKeyspace())
			{
				m_workunit->setHcKeyspace(m_job->getHcKeyspace() - workunitStartIndex2);

				if (!m_workunit->isDuplicated())
				{
					workunitMask->updateIndex(workunitMask->getCurrentIndex() + 1);
					m_job->updateIndex(m_job->getCurrentIndex() + 1);
					m_job->updateIndex2(0);
				}
			}
			else
			{
				uint64_t workunitHcKeyspace = m_workunit->getHcKeyspace();
				if (!m_workunit->isDuplicated())
					m_job->updateIndex2(workunitStartIndex2 + workunitHcKeyspace);

				auto limitLine = makeLimitingConfigLine("hc_keyspace", "BigUInt", std::to_string(workunitHcKeyspace));
				f << limitLine;
				Tools::printDebug(limitLine.c_str());
			}
		}
		/** Host has enough power, no fragments */
		else if (m_workunit->getHcKeyspace() > (0.5f * m_job->getHcKeyspace()))
		{
			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Host has enough power, no fragments\n");

			/** # of real passwords / # of passwords in first dictionaries = # of passwords to check from dict2*/
			uint64_t maskKeyspace = (uint64_t)(std::round(m_workunit->getHcKeyspace() / (float)(m_job->getHcKeyspace())));

			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Rounded desired # of passwords from mask: %" PRIu64 " passwords\n", maskKeyspace);

			Tools::printDebug("Adding additional info to CONFIG: \n");

			{
				auto maskSplitter = makeMaskSplitter();
				auto mask = maskSplitter->GetMaskSlice(workunitMask->getMask(), m_workunit->getStartIndex(), maskKeyspace);
				auto maskConfig = MakeConfigLinesFromSplitMask(mask);
				f<<maskConfig;
				Tools::printDebug(maskConfig.c_str());
				maskKeyspace = mask.keyspace;
			}

			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"actual # of passwords we can get from mask: %" PRIu64 " passwords\n", maskKeyspace);

			uint64_t newCurrentIndex = m_workunit->getStartIndex() + maskKeyspace;
			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"New current index of current mask: %" PRIu64 "\n", newCurrentIndex);

			if (!m_workunit->isDuplicated())
			{
				workunitMask->updateIndex(newCurrentIndex);
				m_job->updateIndex(m_job->getCurrentIndex() + maskKeyspace);
			}

			/** Exact number of passwords in workunit
			* @warning Combinator sets real keyspace to hc_keyspace, as there is no such thing as hc_keyspace here
			**/
			m_workunit->setHcKeyspace(maskKeyspace * m_job->getHcKeyspace());
		}
		/** Host doesn't have enough power, start fragmenting */
		else
		{
			Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
				"Host doesn't have the power, start fragmenting\n");

			/** Append skip and limit to config */
			Tools::printDebug("Adding additional info to CONFIG:\n");

			{
				auto maskSplitter = makeMaskSplitter();
				auto mask = maskSplitter->GetMaskSlice(workunitMask->getMask(), m_workunit->getStartIndex(), 1);
				auto maskConfig = MakeConfigLinesFromSplitMask(mask);
				f<<maskConfig;
				Tools::printDebug(maskConfig.c_str());
			}

			auto skipLine = makeLimitingConfigLine("start_index", "BigUInt", std::to_string(0));
			f << skipLine;
			Tools::printDebug(skipLine.c_str());

			auto workunitHcKeyspace = m_workunit->getHcKeyspace();
			auto limitLine = makeLimitingConfigLine("hc_keyspace", "BigUInt", std::to_string(workunitHcKeyspace));
			f << limitLine;
			Tools::printDebug(limitLine.c_str());

			/** Update current_index_2 of the first fragmented dict */
			if (!m_workunit->isDuplicated())
				m_job->updateIndex2(workunitHcKeyspace);
		}

		Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
			"Done. Closing files\n");

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
		if (!f)
		{
			Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
				"Failed to open data BOINC input file! Setting job to malformed.\n");
			m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
			return false;
		}

		f << m_job->getHashes();
		f.close();

		/** Create dict file */
		retval = config.download_path(name3, path);
		if (retval)
		{
			Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
				"Failed to receive BOINC filename - dict. Setting job to malformed.\n");
			m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
			return false;
		}

		if(!std::ifstream(path))
		{
			auto dictVec = m_job->getDictionaries();
			for (auto & dict : dictVec)
			{
				if (!dict->isLeft())
					continue;

				auto dictFile = makeInputDict(dict, 0, true);
				dictFile->CopyTo(path);
			}
		}
	}
	catch(const InputDict::Exception &e)
	{
		Tools::printDebugHost(Config::DebugType::Error, m_job->getId(), m_host->getBoincHostId(),
			"A dictionary operation failed. Setting job to malformed. Message: %s\n", e.what());
		m_sqlLoader->updateRunningJobStatus(m_job->getId(), Config::JobState::JobMalformed);
		return false;
	}

	/** Fill in the workunit parameters */
	wu.clear();
	wu.appid = Config::app->id;
	safe_strcpy(wu.name, name1);
	wu.delay_bound = m_job->getTimeoutFactor() * (uint32_t)(m_job->getSecondsPerWorkunit());
	infiles[0] = name1;
	infiles[1] = name2;
	infiles[2] = name3;

	setDefaultWorkunitParams(&wu);

	/** Register the workunit with BOINC */
	std::snprintf(path, Config::SQL_BUF_SIZE, "templates/%s", Config::outTemplateFile.c_str());
	retval = create_work(
			wu,
			Config::inTemplatePathHybridDictMask,
			path,
			config.project_path(path),
			infiles,
			3,
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


bool CAttackHybridDictMask::generateWorkunit()
{
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
		"Generating hybrid dict+mask workunit ...\n");

	/** Compute password count */
	uint64_t passCount = getPasswordCountToProcess();

	if (passCount < getMinPassCount())
	{
		Tools::printDebugHost(Config::DebugType::Warn, m_job->getId(), m_host->getBoincHostId(),
			"Passcount is too small! Falling back to minimum passwords\n");
		passCount = getMinPassCount();
	}

	std::vector<PtrMask> maskVec = m_job->getMasks();
	Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
		"Masks left for this job: %" PRIu64 "\n", maskVec.size());

	/** Find the following mask */
	PtrMask currentMask = FindCurrentMask(maskVec, true);

	if (!currentMask)
	{
		/** No masks found, no workunit could be generated */
		Tools::printDebugHost(Config::DebugType::Log, m_job->getId(), m_host->getBoincHostId(),
			"No masks found for this job\n");
		return false;
	}

	/**
	 * Create the workunit
	 * @warning We save number of real passwords to hc_keyspace as we don't work with hc_indexes in combinator attack
	 */
	m_workunit = CWorkunit::create(m_job->getId(), m_host->getId(), m_host->getBoincHostId(), currentMask->getCurrentIndex(),
		m_job->getCurrentIndex2(), passCount, currentMask->getId(), 0, false, 0, false);
	/**
	 * @warning Index updating (current_index(_2)) must be done later
	 */
	return true;
}
