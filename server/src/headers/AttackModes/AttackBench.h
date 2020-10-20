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
        CAttackBench(PtrJob &job, PtrHost &host, CSqlLoader *sqlLoader);

        /**
         * @brief Destructor
         */
        ~CAttackBench() override = default;

        virtual char getModeLetter() override;
        
        virtual std::string generateBasicConfig(unsigned attackMode, unsigned attackSubmode, std::string name, unsigned hashType,
                unsigned generateRandomRules = 0, unsigned hwTempAbort = 90, std::string ruleLeft="", std::string ruleRight="",
                std::string charset1="", std::string charset2="", std::string charset3="", std::string charset4="") override;

        virtual std::string makeLimitingConfigLine(const std::string &, const std::string &, const std::string &) override {return "";}

        virtual std::unique_ptr<InputDict> makeInputDict(PtrDictionary dict, uint64_t startIndex, bool isSticky) override;

        virtual std::unique_ptr<MaskSplitter> makeMaskSplitter(std::vector<std::string> customCharsets) override;

        virtual PtrDictionary FindCurrentDict(std::vector<PtrDictionary> &dicts) const override;

        virtual PtrDictionary GetWorkunitDict() const override;

        virtual PtrMask GetWorkunitMask() const override;

        virtual bool loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace);

        virtual uint64_t getMinPassCount() const {return 0;}
};

template <typename BaseAttack>
CAttackBench<BaseAttack>::CAttackBench(PtrJob &job, PtrHost &host, CSqlLoader *sqlLoader):
    BaseAttack(std::make_shared<CBenchmarkJob>(*job), host, 60, sqlLoader)
{
    job = this->m_job;
}

template <typename BaseAttack>
char CAttackBench<BaseAttack>::getModeLetter()
{
    return 'b';
}

template <typename BaseAttack>
std::string CAttackBench<BaseAttack>::generateBasicConfig(unsigned attackMode, unsigned attackSubmode, std::string name,
    unsigned hashType, unsigned generateRandomRules, unsigned hwTempAbort, std::string ruleLeft, std::string ruleRight, 
    std::string charset1, std::string charset2, std::string charset3, std::string charset4)
{
    std::ostringstream configBuilder;
    configBuilder << AttackMode::generateBasicConfig(attackMode, attackSubmode, name,
        hashType, generateRandomRules, hwTempAbort, ruleLeft, ruleRight, charset1, charset2, charset3, charset4);
    auto dicts = this->m_job->getDictionaries();
    bool hasRightDicts = false;
    if (!this->hasStickyLeftDict()) {
      std::string pwd_dist;
      for (auto &dict : dicts) {
        if (!dict->isLeft()) {
          hasRightDicts = true;
          continue;
        }

        pwd_dist += dict->getPasswordDistribution();
      }

      if (!pwd_dist.empty()) {
        configBuilder << "|||benchmark_dict1|String|" << pwd_dist.size() << '|'
                      << pwd_dist << "|||\n";
      }
    }

    if (this->hasStickyLeftDict() || hasRightDicts) {
      std::string pwd_dist;
      for (auto &dict : dicts) {
        if (dict->isLeft()) {
          continue;
        }
        hasRightDicts = true;

        pwd_dist += dict->getPasswordDistribution();
      }
      if (hasRightDicts) {
        configBuilder << "|||benchmark_dict2|String|" << pwd_dist.size() << '|'
                      << pwd_dist << "|||\n";
      }
    }

    return configBuilder.str();
}

template <typename BaseAttack>
std::unique_ptr<InputDict> CAttackBench<BaseAttack>::makeInputDict(PtrDictionary dict, uint64_t startIndex, bool isSticky)
{
    //do NOT screw up sticky files
    if(isSticky)
    {
        return BaseAttack::makeInputDict(dict, startIndex, isSticky);
    }
    return std::unique_ptr<InputDict>(new InputDictBenchmark(dict, startIndex));
}

template <typename BaseAttack>
std::unique_ptr<MaskSplitter> CAttackBench<BaseAttack>::makeMaskSplitter(std::vector<std::string> customCharsets)
{
    return std::unique_ptr<MaskSplitter>(new MaskSplitterBenchmark(std::move(customCharsets)));
}

template <typename BaseAttack>
PtrDictionary CAttackBench<BaseAttack>::FindCurrentDict(std::vector<PtrDictionary> &dicts) const
{
    Tools::printDebug("Bench attack: dict count is %zu\n", dicts.size());
    auto maxElem = std::max_element(dicts.begin(), dicts.end(), [](const PtrDictionary &lhs, const PtrDictionary &rhs){
        return lhs->getHcKeyspace() < rhs->getHcKeyspace();
    });
    if(maxElem == dicts.end()) return nullptr;
    return *maxElem;
}

template <typename BaseAttack>
PtrDictionary CAttackBench<BaseAttack>::GetWorkunitDict() const
{
    return std::make_shared<CBenchmarkDictionary>(*BaseAttack::GetWorkunitDict());
}

template <typename BaseAttack>
PtrMask CAttackBench<BaseAttack>::GetWorkunitMask() const
{
    return std::make_shared<CBenchmarkMask>(*BaseAttack::GetWorkunitMask());
}

template <typename BaseAttack>
bool CAttackBench<BaseAttack>::loadNextPreterminals(std::string & preterminals, uint64_t & realKeyspace)
{
    preterminals = "";
    realKeyspace = 0;
    return true;
}

#endif //WORKGENERATOR_ATTACKBENCH_H
