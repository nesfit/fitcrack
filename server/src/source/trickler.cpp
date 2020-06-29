/**
 * @file trickler.cpp
 * @brief Source file for parsing trickle messages from hosts
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

// trickle message format:
//
// <workunit_name>X</workunit_name> (X = string, from `workunit`.`name` BOINC table column)
// <progress>Y</progress> (Y = float, from 0.0 to 100.0)
//
// mandatory arguments:
//   --variety fitcrack

#include <iostream>
#include <cmath>
#include "util.h"
#include "sched_msgs.h"
#include "trickle_handler.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

const unsigned int SQL_BUF_SIZE = 1024;


int handle_trickle_init(int argc, char** argv)
{
    /** No extra initialization needed, but the definition must be here */
    UNUSED(argc);
    UNUSED(argv);

    return 0;
}


int handle_trickle(MSG_FROM_HOST& mfh)
{
    /** Define local variables */
    bool got_progress = false;
    bool got_wu_name = false;

    double progress;
    char buf[SQL_BUF_SIZE];
    char wu_name[SQL_BUF_SIZE];

    char platformName[SQL_BUF_SIZE];
    char platformVersion[SQL_BUF_SIZE];
    char platformDeviceType[SQL_BUF_SIZE];
    char platformDeviceName[SQL_BUF_SIZE];

    int time, cpuUtil, memUtil, deviceUtil, deviceTemp, deviceIndex;
    int hw_stats_id, hw_platform_id;
    std::string fc_workunit_id;

    bool got_cpuUtil = false;
    bool got_memUtil = false;
    bool got_deviceUtil = false;
    bool got_deviceTemp = false;
    bool got_deviceIndex = false;

    bool got_platformName = false;
    bool got_platformVersion = false;
    bool got_platformDeviceType= false;
    bool got_platformDeviceName = false;

    std::cerr << std::endl;
    log_messages.printf(MSG_NORMAL,
                        "Got trickle-up message from hostId #%lu:\n%s\n\n", mfh.hostid, mfh.xml
    );

    MIOFILE mf;

    mf.init_buf_read(mfh.xml);
    XML_PARSER xp(&mf);

    /** Parse XML message */
    while (!xp.get_tag())
    {
        if (xp.parse_int("time", time))
        {
            if (std::isnan(time))
            {
                std::cerr << "ERROR: time cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed time: " << time << std::endl;
            continue;
        }

        if (xp.parse_str("workunit_name", wu_name, SQL_BUF_SIZE))
        {
            std::cerr << "Succesfully parsed workunit_name: " << wu_name << std::endl;
            got_wu_name = true;

            // Obtain fc_workunit.id, ...need workunit.id first
            snprintf(buf, SQL_BUF_SIZE, "SELECT id FROM workunit WHERE name = '%s' LIMIT 1 ;", wu_name);
            int retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }

            MYSQL_RES* sqlResult;
            sqlResult = mysql_store_result(boinc_db.mysql);
            if (!sqlResult)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }

            MYSQL_ROW row;
            row = mysql_fetch_row(sqlResult);
            fc_workunit_id = row[0];
            mysql_free_result(sqlResult);
            // workunit.id obtained

            snprintf(buf, SQL_BUF_SIZE, "SELECT id FROM fc_workunit WHERE workunit_id = '%s' LIMIT 1 ;", fc_workunit_id.c_str());
            retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }

            sqlResult = mysql_store_result(boinc_db.mysql);
            if (!sqlResult)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }

            row = mysql_fetch_row(sqlResult);
            fc_workunit_id = row[0];
            mysql_free_result(sqlResult);
            // fc_workunit ID obtained
            continue;
        }

        if (xp.parse_double("progress", progress))
        {
            if (std::isnan(progress))
            {
                std::cerr << "ERROR: Progress cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed progress: " << progress << std::endl;
            got_progress = true;
            continue;
        }

        if (xp.parse_str("platformName", platformName, SQL_BUF_SIZE))
        {
            std::cerr << "Succesfully parsed platformName: " << platformName << std::endl;
            got_platformName = true;
            continue;
        }

        if (xp.parse_str("platformVersion", platformVersion, SQL_BUF_SIZE))
        {
            std::cerr << "Succesfully parsed platformVersion: " << platformVersion << std::endl;
            got_platformVersion = true;
        }

        if (got_platformName && got_platformVersion)
        {
            // SAVE PLATFORM TO DB, KEEP INSERTED ID
            // Insert row into fc_hw_platform
            snprintf(buf, SQL_BUF_SIZE, "INSERT INTO `fc_hw_platform`(`host_id`, `workunit_id`, `name`, `version`) VALUES(%lu, '%s', '%s', '%s') ;",
            mfh.hostid, fc_workunit_id.c_str(), platformName, platformVersion);

            int retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }
            // get ID of inserted row
            hw_platform_id = boinc_db.insert_id();
            got_platformName = got_platformVersion = false;
        }

        if (xp.parse_str("deviceType", platformDeviceType, SQL_BUF_SIZE))
        {
            std::cerr << "Succesfully parsed deviceType: " << platformDeviceType << std::endl;
            got_platformDeviceType = true;
            continue;
        }

        if (xp.parse_str("deviceName", platformDeviceName, SQL_BUF_SIZE))
        {
            std::cerr << "Succesfully parsed deviceName: " << platformDeviceName << std::endl;
            got_platformDeviceName = true;
        }

        if (got_platformDeviceType && got_platformDeviceName)
        {
            // SAVE PLATFORM DEVICE TO DB
            // Insert row into fc_hw_platform_device
            snprintf(buf, SQL_BUF_SIZE, "INSERT INTO `fc_hw_platform_device`(`hw_platform_id`, `deviceType`, `name`) VALUES(%d, '%s', '%s') ;",
            hw_platform_id, platformDeviceType, platformDeviceName);

            int retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }
            got_platformDeviceType = got_platformDeviceName = false;
        }
        
        if (xp.parse_int("cpuUtil", cpuUtil))
        {
            if (std::isnan(cpuUtil))
            {
                std::cerr << "ERROR: cpuUtil cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed cpuUtil: " << cpuUtil << std::endl;
            got_cpuUtil = true;
        }

        if (xp.parse_int("memUtil", memUtil))
        {
            if (std::isnan(memUtil))
            {
                std::cerr << "ERROR: memUtil cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed memUtil: " << memUtil << std::endl;
            got_memUtil = true;
        }

        if (got_cpuUtil && got_memUtil)
        {
            // SAVE SYSTEM STATS TO DB, KEEP INSERTED ID
            // Insert row into fc_hw_stats
            snprintf(buf, SQL_BUF_SIZE, "INSERT INTO `fc_hw_stats`(`workunit_id`, `time`, `cpu_utilization`, `memory_utilization`) VALUES(%s, FROM_UNIXTIME(%d), %d, %d) ;",
            fc_workunit_id.c_str(), time, cpuUtil, memUtil);

            int retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }
            // get ID of inserted row
            hw_stats_id = boinc_db.insert_id();

            got_cpuUtil = got_memUtil = false;
        }

        if (xp.parse_int("util", deviceUtil))
        {
            if (std::isnan(deviceUtil))
            {
                std::cerr << "ERROR: util cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed util: " << deviceUtil << std::endl;
            got_deviceUtil = true;
        }

        if (xp.parse_int("temp", deviceTemp))
        {
            if (std::isnan(deviceTemp))
            {
                std::cerr << "ERROR: temp cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed temp: " << deviceTemp << std::endl;
            got_deviceTemp = true;
        }

        if (xp.parse_int("index", deviceIndex))
        {
            if (std::isnan(deviceIndex))
            {
                std::cerr << "ERROR: index cannot be nan." << std::endl;
                return 1;
            }

            std::cerr << "Succesfully parsed index: " << deviceIndex << std::endl;
            got_deviceIndex = true;
        }

        if (got_deviceUtil && got_deviceTemp && got_deviceIndex)
        {
            // SAVE DEVICE STATS TO DB

            snprintf(buf, SQL_BUF_SIZE, "INSERT INTO `fc_hw_stats_device`(`hw_stats_id`, `index`, `utilization`, `temperature`) VALUES(%d, %d, %d, %d) ;",
             hw_stats_id, deviceIndex, deviceUtil, deviceTemp);

            int retval = boinc_db.do_query(buf);
            if (retval)
            {
                std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
                boinc_db.close();
                exit(1);
            }

            got_deviceUtil = got_deviceTemp = got_deviceIndex = false;
        }
        
        //std::cerr << "WARNING: Unexpected tag: " << xp.parsed_tag << std::endl;
    }

    /** Check values */
    if (!got_progress || !got_wu_name)
    {
        std::cerr << "ERROR: Could not parse all mandatory data." << std::endl;
        return 1;
    }

    if (progress < 0.0 || progress > 100.0)
    {
        std::cerr << "ERROR: Invalid progress value: " << progress << std::endl;
        return 1;
    }

    /** Update workunit progress in DB */
    snprintf(buf, SQL_BUF_SIZE, "UPDATE `fc_workunit` SET `progress` = %f WHERE `progress` < %f AND `workunit_id` IN (SELECT id FROM workunit WHERE name = '%s') LIMIT 1 ;",
             progress, progress, wu_name);

    int retval = boinc_db.do_query(buf);
    if (retval)
    {
        std::cerr << "Problem with DB query: " << buf << "\nShutting down now." << std::endl;
        boinc_db.close();
        exit(1);
    }

    std::cerr << "Database updated successfully." << std::endl;
    return 0;
}
