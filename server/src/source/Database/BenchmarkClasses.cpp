#include <BenchmarkClasses.h>

std::vector<Config::Ptr<CMask>> CBenchmarkJob::getMasks() const
{
	std::vector<Config::Ptr<CMask>> result;
	for(auto &mask: m_masks)
	{
		result.push_back(std::make_shared<CBenchmarkMask>(*mask));
	}
	return result;
}

std::vector<Config::Ptr<CDictionary>> CBenchmarkJob::getDictionaries() const
{
	std::vector<Config::Ptr<CDictionary>> result;
	for(auto &dict: m_dictionaries)
	{
		result.push_back(std::make_shared<CBenchmarkDictionary>(*dict));
	}
	return result;
}

std::vector<Config::Ptr<CDictionary>> CBenchmarkJob::getRightDictionaries() const
{
	std::vector<Config::Ptr<CDictionary>> result;
	for(auto &dict: m_dictionaries)
	{
		if(dict->isLeft()) continue;
		result.push_back(std::make_shared<CBenchmarkDictionary>(*dict));
	}
	return result;
}

