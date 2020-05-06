#include "MaskSplitter.h"

#include <algorithm>
#include <cmath>
#include <sstream>

//taken from https://hashcat.net/wiki/doku.php?id=mask_attack
const std::unordered_map<char, std::string> MaskSplitter::s_builtinCharsets = {
	{'l', "abcdefghijklmnopqrstuvwxyz"},
	{'u', "ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
	{'d', "0123456789"},
	{'h', "0123456789abcdef"},
	{'H', "0123456789ABCDEF"},
	{'s', R"nope( !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)nope"},
	{'a', R"nope(abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)nope"},
	{'b', 
		"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
		"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
		"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F"
		"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
		"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
		"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
		"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F"
		"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
		"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
		"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
		"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF"
		"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
		"\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF"
		"\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF"
		"\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF"
		"\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF"
	}
};

MaskSplitter::MaskSplitter(std::vector<std::string> customCharsets):
	m_customCharsets(std::move(customCharsets))
{
	NormalizeCustomCharsets();
}

void AddCharsetSlice(
	size_t curCharsetBestSize,
	const std::string &possibleCharset,
	MaskSplitter::MaskSettings &result,
	const char *charsetIdentifier = nullptr //!<If @p possible charset is already defined, pointer to it's single character identifier, else nullptr
)
{
	//0.75 to avoid forcing createion of itty bitty workunits later
	//for example by taking 90 of 91 remaining chars, forcing the next WU to use just one
	if(curCharsetBestSize > possibleCharset.size()*0.75)
	{
		//if it's more than three quarters, take it all
		curCharsetBestSize = possibleCharset.size();
	}
	else if(curCharsetBestSize > possibleCharset.size()*0.5)
	{
		//if less than 0.75 and more than half, just take half
		curCharsetBestSize = possibleCharset.size()*0.5;
	}
	//new mask starts with the characters passed over so far
	if(curCharsetBestSize <= 1)
	{
		//no need to use another charset if there is just one character in it
		//just use the first character from the possible chars charset
		result.mask.back() = possibleCharset[0];
		return;
	}
	result.keyspace *= curCharsetBestSize;
	if(curCharsetBestSize == possibleCharset.size() && charsetIdentifier)
	{
		result.mask += *charsetIdentifier;
	}
	else
	{
		result.mask += std::to_string(result.customCharsets.size()+1);
		result.customCharsets.push_back(possibleCharset.substr(0, curCharsetBestSize));
	}
}

MaskSplitter::MaskSettings MaskSplitter::GetMaskSlice(const std::string &mask, uint64_t startIndex, uint64_t desiredKeyspace)
{
	//index at which we replace a charset with a newly created one
	size_t forcedSplitIndex = std::string::npos;
	size_t adjustedStartIndex = startIndex;
	//The characters we can choose from to create the new charset
	std::string possibleNewCharset;
	//find where we *have* to split
	for(size_t i = 0; i < mask.size(); ++i)
	{
		if(mask[i] != '?')
		{
			continue;
		}
		//got a charset, analyze
		++i;
		if(mask[i] == '?')
		{
			//just a literal question mark
			continue;
		}
		const std::string &charset = GetCharset(mask[i]);
		if(adjustedStartIndex%(charset.size()) != 0)
		{
			//charset was split before, figure out where exactly
			forcedSplitIndex = i;
			size_t startingCharIndex = adjustedStartIndex%charset.size();
			adjustedStartIndex /= charset.size();
			possibleNewCharset = charset.substr(startingCharIndex);
			break;
		}
		adjustedStartIndex /= charset.size();
	}
	//now find where we *want* to split
	MaskSettings result;
	result.customCharsets = m_customCharsets;
	bool noLongerAddingPlaceholders = false;
	for(size_t i = 0; i < mask.size(); ++i)
	{
		if(mask[i] != '?')
		{
			result.mask += mask[i];
			continue;
		}
		++i;
		if(mask[i] == '?')
		{
			//just an escaped question mark, write out and continue
			result.mask += "??";
			continue;
		}
		//got a charset, analyze
		if(noLongerAddingPlaceholders)
		{
			//We already have the desired keyspace, now just add simple characters
			if(i == forcedSplitIndex)
			{
				//we already have the necessary charset, just use that
				result.mask += possibleNewCharset[0];
				continue;
			}
			const std::string &charset = GetCharset(mask[i]);
			if(i < forcedSplitIndex)
			{
				//We're before the forced split point, we can just take the first char
				result.mask += charset[0];
				continue;
			}
			//charset after the forced split point. Figure out which char we have to take
			size_t charIndex = adjustedStartIndex%charset.size();
			//add that char
			result.mask += charset[charIndex];
			//and adjust the start index for the new iteration
			adjustedStartIndex /= charset.size();
			continue;
		}
		//we are still possibly adding placeholder elements
		result.mask += '?';
		if(i == forcedSplitIndex)
		{
			//we have to split here
			size_t bestSize = std::round(double(desiredKeyspace)/result.keyspace);
			//0.75 to avoid fircing creation of itty bitty workunits later
			//for example by taking 90 of 91 remaining chars, forcing the next WU to use just one
			AddCharsetSlice(bestSize, possibleNewCharset, result);
			noLongerAddingPlaceholders = true;
			continue;
		}
		const std::string &charset = GetCharset(mask[i]);
		size_t remainingKeyspace = std::round(double(desiredKeyspace)/result.keyspace);
		if(remainingKeyspace >= charset.size())
		{
			//simplest case, just add the identifier and continue analyzing
			result.mask += mask[i];
			result.keyspace *= charset.size();
			continue;
		}
		//less than charset size, this is the end. Just figure out how much to send
		AddCharsetSlice(remainingKeyspace, charset, result, &mask[i]);
		noLongerAddingPlaceholders = true;
		continue;
	}
	return result;
}

void MaskSplitter::NormalizeCustomCharsets()
{
	for(size_t i = 0; i < m_customCharsets.size(); ++i)
	{
		auto &charset = m_customCharsets.at(i);
		//change placeholders for the actual chars
		for(size_t qmarkPos = charset.find('?', 0); qmarkPos < std::string::npos; qmarkPos = charset.find('?', qmarkPos))
		{
			const auto &referencedCharset = GetCharset(charset[qmarkPos+1]);
			charset.replace(qmarkPos, 2, referencedCharset);
			qmarkPos += referencedCharset.size();
		}
		//sort and remove duplicates
		std::sort(charset.begin(), charset.end());
		charset.erase(std::unique(charset.begin(), charset.end()), charset.end());
	}
}

const std::string &MaskSplitter::GetCharset(char placeholder)
{
	if(std::isdigit(placeholder))
	{
		//reference to another custom charset
		//-'1' since ?1 is actually the first one (with index 0)
		unsigned charsetIndex = placeholder-'1';
		return m_customCharsets.at(charsetIndex);
	}
	else
	{
		return s_builtinCharsets.at(placeholder);
	}
}