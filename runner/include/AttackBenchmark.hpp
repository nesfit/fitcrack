/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef ATTACKBENCHMARK_HPP 
#define ATTACKBENCHMARK_HPP

#include "ConfigTask.hpp"
#include "Directory.hpp"

#include <algorithm>
#include <string>
#include <sstream>

static const int DICT_GEN_TIME_LIMIT = 5;

/** Class representing hashcat's attack benchmark */
template <typename BaseAttack>
class AttackBenchmark: public BaseAttack {

protected:

	/**
		* @brief   Function adding all atttack specific arguments
		*/
	void addSpecificArguments();

	virtual std::string addRequiredFile(const std::string &file_name);
	//no limits for benchmark
	virtual void addGeneratorLimitingArgument(const std::string&) {}
	virtual void addGeneratorLimitingRequiredFile(const std::string&) {}

public: 

	/**
		* @brief   Basic constructor
		* @param   config [in] Representation of config file
		*/
	AttackBenchmark(const ConfigTask& config, Directory &directory);

};

template <typename BaseAttack>
AttackBenchmark<BaseAttack>::AttackBenchmark(const ConfigTask& config, Directory &directory) : BaseAttack(config, directory) {
  
}

namespace
{
	struct StatusTimerSearcher
	{
		bool operator()(const std::string &arg) const
		{
			static const std::string searched = "--status-timer=";
			return arg.substr(0, searched.length()) == searched;
		}
	};
}

template <typename BaseAttack>
void AttackBenchmark<BaseAttack>::addSpecificArguments() {
	BaseAttack::addSpecificArguments();
  this->addArgument("--runtime");
	if(this->getExternalGeneratorName().empty())
	{
		this->addArgument("3");
	}
	else
	{
		//hashcat 5 for some reason updates the status for fast hashes only once every blue moon for some reason
		Logging::debugPrint(Logging::Detail::GeneralInfo, "benchmarking process fed by pipe, longer observation required");
		this->addArgument("30");
		std::vector<std::string>::iterator pos = std::find_if(this->arguments_.begin(), this->arguments_.end(), StatusTimerSearcher());
		if(pos == this->arguments_.end())
		{
			RunnerUtils::runtimeException("Benchmark attack failed to find the status timer argument");
		}
		*pos = "--status-timer=2";
	}
}

struct PasswordLenCount
{
	size_t len;
	size_t count;
};

inline std::istream &operator>>(std::istream &in, PasswordLenCount &data)
{
	in>>data.len;
	Logging::debugPrint(Logging::Detail::DevelDebug, "data len "+RunnerUtils::toString(data.len));
	if(in.get() != ':')
	{
		in.unget();
		in.setstate(std::ios::failbit);
	}
	in>>data.count;
	Logging::debugPrint(Logging::Detail::DevelDebug, "data count "+RunnerUtils::toString(data.count));
	if(in.get() != ';')
	{
		in.unget();
		in.setstate(std::ios::failbit);
	}
	return in;
}

static const unsigned bufSize = 16384;

template <typename BaseAttack>
std::string AttackBenchmark<BaseAttack>::addRequiredFile(const std::string &file_name)
{
	std::string shape;
	if((file_name == "dict1" || file_name == "dict2") && this->config_.find("benchmark_"+file_name, shape))
	{
		Logging::debugPrint(Logging::Detail::CustomOutput, "Making benchmark dictionary named "+file_name);
		File resolvedFile;
		if(!this->directory_.find(file_name, resolvedFile))
		{
			RunnerUtils::runtimeException("Missing hashcat required file " + file_name);
		}
		std::ofstream dict(resolvedFile.getRelativePath().c_str(), std::ios::binary);
		std::istringstream convertor(shape);
		PasswordLenCount pwdLenCount;
		time_t startTime = time(NULL);
		bool running = true;
		while(running && convertor>>pwdLenCount)
		{
			//we don't really expect passwords that long, but better safe than sorry
			//-1 for the newline
			if(pwdLenCount.len > bufSize-1)
			{
				RunnerUtils::runtimeException("Password too long");
			}
			char buf[bufSize];
			//create the "password"
			std::memset(buf, 'a', pwdLenCount.len);
			buf[pwdLenCount.len] = '\n';
			//copy it into the whole buffer to speed up writing
			size_t repetitions = std::min(bufSize/(pwdLenCount.len+1), pwdLenCount.count);
			size_t lineLen = pwdLenCount.len+1;
			for(size_t i = 1; i < repetitions; ++i)
			{
				std::memcpy(buf+i*lineLen, buf, lineLen);
			}
			//do the writes
			while(running && pwdLenCount.count)
			{
				size_t linesToWrite = std::min(repetitions, pwdLenCount.count);
				dict.write(buf, lineLen*linesToWrite);
				pwdLenCount.count -= linesToWrite;
				running = time(NULL)-startTime < DICT_GEN_TIME_LIMIT;
			}
		}
		if(!running)
		{
			Logging::debugPrint(Logging::Detail::CustomOutput, "Generating interrupted due to passed time limit");
		}
	}
	return BaseAttack::addRequiredFile(file_name);
}

#endif // ATTACKBENCHMARK_HPP
