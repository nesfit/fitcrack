#include "InputDict.h"

InputDict::InputDict(PtrDictionary dbDict, uint64_t startIndex):
	m_dbDict(std::move(dbDict)),
	m_startIndex(startIndex),
	m_curIndex(0)
{}

uint64_t InputDict::WritePasswordsTo(uint64_t passCount, const std::string &dst)
{
	EnsureDictPosition();
	std::ofstream out(dst, std::ios::ate);
	if(!out)
	{
		throw Exception("Failed to open output file for writing passwords");
	}
	std::string line;
	uint64_t writtenCount;
	for(writtenCount = 0; writtenCount < passCount && std::getline(m_file, line); ++writtenCount, ++m_curIndex)
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
	EnsureDictIsOpen();
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

void InputDict::EnsureDictPosition()
{
	//open and move to correct position
	EnsureDictIsOpen();

	//check if set manually using SetCurrentDictPos
	if (GetCurrentDictPos() > 0)
		return;

	if(m_curIndex > m_startIndex)
	{
		//reset position
		if(!m_file.seekg(0))
		{
			throw Exception("Rewinding to beginning of dictionary failed");
		}
	}
	std::string line;
	//ignore startIndex passwords
	for(m_curIndex = 0; m_curIndex < m_startIndex && std::getline(m_file, line); ++m_curIndex)
		;

	if(!m_file && !m_file.eof())
	{
		throw Exception("Getting to the desired position in a dictionary failed");
	}
}

void InputDict::EnsureDictIsOpen()
{
	if(!m_file.is_open())
	{
		return DoOpenDict();
	}
}

std::ifstream::pos_type InputDict::GetCurrentDictPos() {
    return m_file.tellg();
}

void InputDict::SetCurrentDictPos(std::ifstream::pos_type pos) {
    m_file.seekg(pos);
}

void InputDict::DoOpenDict()
{
	m_file.open(Config::dictDir + m_dbDict->getDictFileName());
	if(!m_file)
	{
		throw Exception("Failed to open dict for reading");
	}
}
