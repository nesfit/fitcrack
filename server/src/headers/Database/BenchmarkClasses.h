#pragma once

#include "Dictionary.h"
#include "Job.h"
#include "Mask.h"

class CBenchmarkDictionary: public CDictionary
{
public:
	CBenchmarkDictionary(const CDictionary &dict): CDictionary(dict) {}
	//noop
	virtual void updateIndex(uint64_t) override {}
};

class CBenchmarkJob: public CJob
{
public:
	CBenchmarkJob(const CJob &job): CJob(job) {}
	//noops
	virtual void updateIndex(uint64_t) override {}
	virtual void updateIndex2(uint64_t) override {}
	virtual std::vector<Config::Ptr<CMask>> getMasks() const override;
	virtual std::vector<Config::Ptr<CDictionary>> getDictionaries() const override;
	virtual std::vector<Config::Ptr<CDictionary>> getRightDictionaries() const override;
};

class CBenchmarkMask: public CMask
{
public:
	CBenchmarkMask(const CMask &mask): CMask(mask) {}
	//noop
	virtual void updateIndex(uint64_t) override {}
};