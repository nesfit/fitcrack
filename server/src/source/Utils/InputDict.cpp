#include "InputDict.h"

InputDict::InputDict(const std::string path) {
  m_file.open(path);
  if (!m_file) {
    throw Exception("Failed to open dict for reading");
  }
}

uint64_t InputDict::WritePasswordsTo(uint64_t passCount, const std::string &dst)
{
	std::ofstream out(dst, std::ios::ate);
	if(!out)
	{
		throw Exception("Failed to open output file for writing passwords");
	}
	std::string line;
	uint64_t writtenCount;
	for(writtenCount = 0; writtenCount < passCount && std::getline(m_file, line); ++writtenCount)
	{
		line += '\n';
		out.write(line.c_str(), line.length());
	}
	if(!m_file && !m_file.eof())
	{
		throw Exception("Reading from source dictionary failed while getting passwords");
	}
	if(!out)
	{
		throw Exception("Writing to destination dictionary failed while getting passwords");
	}
	return writtenCount;
}

void InputDict::CopyTo(const std::string &dst)
{
	if(!m_file.seekg(0))
	{
		throw Exception("Rewinding to beginning of dictionary failed");
	}
	std::ofstream out(dst, std::ios::ate|std::ios::app);
	if(!out)
	{
		throw Exception("Failed to open output file for copying dictionary");
	}
	if(!(out<<m_file.rdbuf()))
	{
		throw Exception("Failed to copy dictionary data");
	}
}

std::ifstream::pos_type InputDict::GetCurrentDictPos() {
    return m_file.tellg();
}

void InputDict::SetCurrentDictPos(std::ifstream::pos_type pos) {
    m_file.seekg(pos);
}
