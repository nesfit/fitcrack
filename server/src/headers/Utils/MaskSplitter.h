#ifndef FITCRACK_MASK_SPLITTER_H_
#define FITCRACK_MASK_SPLITTER_H_

#include <unordered_map>
#include <string>
#include <vector>

class MaskSplitter
{
public:
	explicit MaskSplitter(std::vector<std::string> customCharsets = {});
	struct MaskSettings
	{
		std::string mask;
		std::vector<std::string> customCharsets;
		uint64_t keyspace = 1;
	};
	MaskSettings GetMaskSlice(const std::string &mask, uint64_t startIndex, uint64_t desiredKeyspace);
private:
	void NormalizeCustomCharsets();
	const std::string &GetCharset(char placeholder);
	static const std::unordered_map<char, std::string> s_builtinCharsets;
	std::vector<std::string> m_customCharsets;
};

#endif //FITCRACK_MASK_SPLITTER_H_