#ifndef FITCRACK_MASK_SPLITTER_H_
#define FITCRACK_MASK_SPLITTER_H_

#include <unordered_map>
#include <string>
#include <vector>

class MaskSplitter
{
public:
	explicit MaskSplitter(std::vector<std::string> customCharsets = {});
	virtual ~MaskSplitter() = default;
	struct MaskSettings
	{
		std::string mask;
		std::vector<std::string> customCharsets;
		uint64_t keyspace;
		MaskSettings(std::string maskP = "", std::vector<std::string> customCharsetsP = {}, uint64_t keyspaceP = 1):
			mask(std::move(maskP)), customCharsets(std::move(customCharsetsP)), keyspace(keyspaceP)
		{}
	};
	virtual MaskSettings GetMaskSlice(const std::string &mask, uint64_t startIndex, uint64_t desiredKeyspace);
protected:
	void NormalizeCustomCharsets();
	const std::string &GetCharset(char placeholder);
	static const std::unordered_map<char, std::string> s_builtinCharsets;
	std::vector<std::string> m_customCharsets;
};

class MaskSplitterBenchmark: public MaskSplitter
{
	using MaskSplitter::MaskSplitter;
	virtual MaskSettings GetMaskSlice(const std::string &mask, uint64_t, uint64_t) override {return {mask, m_customCharsets, 0};}
};

#endif //FITCRACK_MASK_SPLITTER_H_