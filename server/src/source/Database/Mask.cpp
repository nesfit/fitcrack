/**
 * @file Mask.cpp
 * @brief Source file for fc_mask entry
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Mask.h>
#include <SqlLoader.h>


CMask::CMask(DbMap & maskMap, CSqlLoader * sqlLoader)
    :   m_sqlLoader(sqlLoader)
{
    try
    {
        this->m_id = std::stoull(maskMap["id"]);
        this->m_jobId = std::stoull(maskMap["job_id"]);
        this->m_mask = maskMap["mask"];
        this->m_currentIndex = std::stoull(maskMap["current_index"]);
        this->m_hcKeyspace = std::stoull(maskMap["hc_keyspace"]);
        this->m_keyspace = std::stoull(maskMap["keyspace"]);
        this->m_incrementMin = std::stoull(maskMap["increment_min"]);
        this->m_incrementMax = std::stoull(maskMap["increment_max"]);
    }
    catch(std::logic_error & error)
    {
        Tools::printDebugTimestamp("Error converting maskMap to PtrMask: %s", error.what());
        exit(1);
    }
}


PtrMask CMask::create(DbMap & maskMap, CSqlLoader * sqlLoader)
{
    return PtrMask(new CMask(maskMap, sqlLoader));
}


std::string CMask::getTableName()
{
    return Config::tableNameMask;
}


void CMask::updateIndex(uint64_t newIndex)
{
    /** Local update */
    this->m_currentIndex = newIndex;

    /** Database update */
    this->m_sqlLoader->updateMaskIndex(this->m_id, newIndex);
}


void CMask::updateMask(std::string newMask, uint64_t newKeyspace, uint64_t newHcKeyspace, uint64_t incrementMin, uint64_t incrementMax)
{
    /** Local update */
    this->m_mask = newMask;
    this->m_keyspace = newKeyspace;
    this->m_hcKeyspace = newHcKeyspace;
    this->m_incrementMin = incrementMin;
    this->m_incrementMax = incrementMax;

    /** Database update */
    this->m_sqlLoader->updateMask(this->m_id, newMask, newKeyspace, newHcKeyspace, incrementMin, incrementMax);
}


/**
 * @section Table attributes getters
 */

uint64_t CMask::getId() const
{
    return m_id;
}


uint64_t CMask::getJobId() const
{
    return m_jobId;
}


const std::string & CMask::getMask() const
{
    return m_mask;
}


uint64_t CMask::getCurrentIndex() const
{
    return m_currentIndex;
}


uint64_t CMask::getHcKeyspace() const
{
    return m_hcKeyspace;
}

uint64_t CMask::getKeyspace() const
{
    return m_keyspace;
}

uint64_t CMask::getIncrementMin() const
{
    return m_incrementMin;
}

uint64_t CMask::getIncrementMax() const
{
    return m_incrementMax;
}

uint64_t CMask::getLength() const
{
    uint64_t maskLen = 0;

    for(size_t i = 0; i < m_mask.size(); ++i)
    {
        // Fixed character
        if(m_mask[i] != '?')
        {
            maskLen += 1;
            continue;
        }

        // Character set or escaped question mark
        i++;
        maskLen += 1;
    }

    return maskLen;
}

bool CMask::compare(PtrMask otherMask, uint64_t length) const
{
    std::string m1 = m_mask;
    uint64_t m1Len = this->getLength();

    std::string m2 = otherMask->getMask();
    uint64_t m2Len = otherMask->getLength();

    int index = 0;
    int maskIndex = 0;
    while(maskIndex < length)
    {
        if(maskIndex >= m1Len && maskIndex < m2Len)
            return false;
        else if(maskIndex >= m2Len && maskIndex < m1Len)
            return false;
        else if(maskIndex >= m2Len && maskIndex >= m1Len)
            return true;

        if(m1[index] != m2[index])
            return false;
        
        if(m1[index] == '?')
        {  
            index++;
            if(m1[index] != m2[index])
                return false;
        }

        index++;
        maskIndex++;
    }
    return true;
}
