#include "InputDict.h"

InputDict::InputDict(PtrDictionary dbDict, uint64_t startIndex):
	m_dbDict(std::move(dbDict)),
	m_startIndex(startIndex),
	m_startPos(0),
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
	for(writtenCount = 0; writtenCount < passCount && std::getline(m_file, line) && out; ++writtenCount, ++m_curIndex)
	{
		out<<line<<'\n';
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
	std::ofstream out(dst, std::ios::ate);
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
	if(m_startPos > 0)
	{
		//position is known, seek to it
		if(!m_file.seekg(m_startPos))
		{
			throw Exception("Seeking failed in dictionary");
		}
		return;
	}
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
	//save position for later, may come in handy
	m_startPos = m_file.tellg();
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

void InputDict::DoOpenDict()
{
	m_file.open(Config::dictDir + m_dbDict->getDictFileName());
	if(!m_file)
	{
		throw Exception("Failed to open dict for reading");
	}
}
