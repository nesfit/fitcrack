/**
 * @file AttackBench.h
 * @brief Header file for creation of Benchmark workunit
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#ifndef WORKGENERATOR_ATTACKBENCH_H
#define WORKGENERATOR_ATTACKBENCH_H

#include "BenchmarkClasses.h"
#include <AttackMode.h>

#include <algorithm>
#include <sstream>

template <typename BaseAttack>
class CAttackBench : public BaseAttack {
    static_assert(std::is_base_of<AttackMode, BaseAttack>::value, "Base attack must be derived from AttackMode");
    public:
        /**
         * @brief Constructor for Benchmark Workunit
         * @param job [in] Instance of CJob which is parent of this workunit instance
         * @param host [in] Instance of CHost which this workunit belongs to
         */
        CAttackBench(PtrJob job, PtrHost &host, CSqlLoader *sqlLoader);

        /**
         * @brief Destructor
         */
        ~CAttackBench() override = default;

        virtual char getModeLetter() override;
        
        virtual std::string generateBasicConfig(unsigned attackMode, unsigned attackSubmode, std::string name,
                unsigned hashType, std::string ruleLeft="", std::string ruleRight="", std::string charset1="",
                std::string charset2="", std::string charset3="", std::string charset4="") override;

        virtual std::string makeSkipConfigLine(uint64_t toSkip) override;

        virtual std::string makeLimitConfigLine(uint64_t limit) override;

        virtual std::unique_ptr<InputDict> makeInputDict(PtrDictionary dict, uint64_t startIndex) override;

        virtual PtrDictionary FindCurrentDict(std::vector<PtrDictionary> &dicts) const override;

        virtual bool loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace);
};

template <typename BaseAttack>
CAttackBench<BaseAttack>::CAttackBench(PtrJob job, PtrHost &host, CSqlLoader *sqlLoader):
    BaseAttack(std::make_shared<CBenchmarkJob>(*job), host, 60, sqlLoader)
{
    
}

template <typename BaseAttack>
char CAttackBench<BaseAttack>::getModeLetter()
{
    return 'b';
}

template <typename BaseAttack>
std::string CAttackBench<BaseAttack>::generateBasicConfig(unsigned attackMode, unsigned attackSubmode, std::string name,
    unsigned hashType, std::string ruleLeft, std::string ruleRight, std::string charset1,
    std::string charset2, std::string charset3, std::string charset4)
{
    std::ostringstream configBuilder;
    configBuilder<<AttackMode::generateBasicConfig(attackMode, attackSubmode, name,
        hashType, ruleLeft, ruleRight, charset1, charset2, charset3, charset4);
    size_t curLen = 0;
    size_t curCount = 0;
    auto dicts = this->m_job->getDictionaries();
    bool hasRightDicts = false;
    //count numbers of passwords of different lengths
    //do left dicts first
    std::ostringstream pwdCountBuilder;
    auto CountPasswords = [&](PtrDictionary &dict) {
        std::string passwd;
        std::ifstream input(Config::dictDir + dict->getDictFileName());
        if(!input)
        {
            Tools::printDebugHost(Config::DebugType::Error, this->m_job->getId(), this->m_host->getBoincHostId(),
                    "Failed to open an input dictionary. Setting job to malformed.\n");
            this->m_sqlLoader->updateRunningJobStatus(this->m_job->getId(), Config::JobState::JobMalformed);
            throw std::runtime_error("Failed to open an input dictionary");
        }
        while(std::getline(input, passwd))
        {
            ++curCount;
            if(passwd.size() != curLen)
            {
                if(curCount != 0)
                {
                    pwdCountBuilder<<curLen<<':'<<curCount<<';';
                    curCount = 0;
                }
                curLen = passwd.size();
            }
        }
    };
    for(auto &dict: dicts)
    {
        if(!dict->isLeft())
        {
            hasRightDicts = true;
            continue;
        }
        CountPasswords(dict);
    }
    {
        auto tmp = pwdCountBuilder.str();
        configBuilder<<"|||benchmark_dict1|String|"<<tmp.size()<<'|'<<tmp<<"|||\n";
    }
    if(hasRightDicts)
    {
        pwdCountBuilder.str("");
        for(auto &dict: dicts)
        {
            if(dict->isLeft())
            {
                continue;
            }
            CountPasswords(dict);
        }
        {
            auto tmp = pwdCountBuilder.str();
            configBuilder<<"|||benchmark_dict2|String|"<<tmp.size()<<'|'<<tmp<<"|||\n";
        }
    }
    return configBuilder.str();
}

template <typename BaseAttack>
std::string CAttackBench<BaseAttack>::makeSkipConfigLine(uint64_t)
{
    return "";
}

template <typename BaseAttack>
std::string CAttackBench<BaseAttack>::makeLimitConfigLine(uint64_t)
{
    return "";
}

template <typename BaseAttack>
std::unique_ptr<InputDict> CAttackBench<BaseAttack>::makeInputDict(PtrDictionary dict, uint64_t startIndex)
{
    return std::make_unique<InputDictBenchmark>(dict, startIndex);
}

template <typename BaseAttack>
PtrDictionary CAttackBench<BaseAttack>::FindCurrentDict(std::vector<PtrDictionary> &dicts) const
{
    Tools::printDebug("Bench attack: dict count is %zu\n", dicts.size());
    auto maxElem = std::max_element(dicts.begin(), dicts.end(), [](const auto &lhs, const auto &rhs){
        return lhs->getHcKeyspace() < rhs->getHcKeyspace();
    });
    if(maxElem == dicts.end()) return nullptr;
    return *maxElem;
}

template <typename BaseAttack>
bool CAttackBench<BaseAttack>::loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace)
{
    preterminals = "";
    realKeyspace = 1;
    return true;
}

#endif //WORKGENERATOR_ATTACKBENCH_H
