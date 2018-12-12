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
        this->m_packageId = std::stoull(maskMap["job_id"]);
        this->m_mask = maskMap["mask"];
        this->m_currentIndex = std::stoull(maskMap["current_index"]);
        this->m_hcKeyspace = std::stoull(maskMap["hc_keyspace"]);
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


/**
 * @section Table attributes getters
 */

uint64_t CMask::getId() const
{
    return m_id;
}


uint64_t CMask::getPackageId() const
{
    return m_packageId;
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
