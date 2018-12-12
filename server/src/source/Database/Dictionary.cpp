/**
 * @file Dictionary.cpp
 * @brief Source file for fc_[package_]dictionary entries
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Dictionary.h>
#include <SqlLoader.h>


CDictionary::CDictionary(DbMap &dictMap, CSqlLoader *sqlLoader)
    :   m_sqlLoader(sqlLoader)
{
    try
    {
        this->m_id = std::stoull(dictMap["id"]);
        this->m_packageId = std::stoull(dictMap["job_id"]);
        this->m_dictionaryId = std::stoull(dictMap["dictionary_id"]);
        this->m_currentIndex = std::stoull(dictMap["current_index"]);
        this->m_hcKeyspace = std::stoull(dictMap["keyspace"]);
        this->m_dictFileName = dictMap["path"];
        this->m_isLeft = (bool)(std::stoi(dictMap["is_left"]));
    }
    catch(std::logic_error & error)
    {
        Tools::printDebugTimestamp("Error converting dictMap to PtrDictionary: %s", error.what());
        exit(1);
    }
}


PtrDictionary CDictionary::create(DbMap &dictMap, CSqlLoader *sqlLoader)
{
    return PtrDictionary(new CDictionary(dictMap, sqlLoader));
}


std::string CDictionary::getTableName()
{
    return Config::tableNamePckgDictionary;
}


void CDictionary::updateIndex(uint64_t newIndex)
{
    /** Local update */
    this->m_currentIndex = newIndex;

    /** Database update */
    this->m_sqlLoader->updateDictionaryIndex(this->m_id, newIndex);
}


/**
 * @section Table attributes getters
 */

uint64_t CDictionary::getId() const
{
    return m_id;
}


uint64_t CDictionary::getPackageId() const
{
    return m_packageId;
}


uint64_t CDictionary::getDictionaryId() const
{
    return m_dictionaryId;
}


uint64_t CDictionary::getCurrentIndex() const
{
    return m_currentIndex;
}


uint64_t CDictionary::getHcKeyspace() const
{
    return m_hcKeyspace;
}


const std::string &CDictionary::getDictFileName() const
{
    return m_dictFileName;
}


bool CDictionary::isLeft() const
{
    return m_isLeft;
}
