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
#include <cinttypes>
#include <map>
#include "util.h"
#include "sched_msgs.h"
#include "trickle_handler.h"

#define UNUSED(expr) do { (void)(expr); } while (0)

const unsigned int SQL_BUF_SIZE = 1024;

struct Device {
  std::string name;
  std::string type;
  std::string speed;
  std::string temp;
  std::string util;
};

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
    bool got_total_speed = false;

    double progress;
    char buf[SQL_BUF_SIZE];
    std::string wu_name;
    std::string total_speed;
    std::map<uint32_t, Device> devices;

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
        if (xp.parse_string("workunit_name", wu_name))
        {
            std::cerr << "Succesfully parsed workunit_name: " << wu_name << std::endl;
            got_wu_name = true;
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

        if (xp.parse_string("total_speed", total_speed))
        {
            std::cerr << "Succesfully parsed total speed: " << total_speed << std::endl;
            got_total_speed = true;
            continue;
        }

        char s1[SQL_BUF_SIZE];
        char s2[SQL_BUF_SIZE];
        char s3[SQL_BUF_SIZE];
        if (sscanf(xp.parsed_tag, "%[^_]_%[^_]_%s", s1, s2, s3) == 3) {
          if (strcmp(s1, "device") != 0)
            continue;
          uint32_t device_id = strtoul(s2, NULL, 10);
          std::string tag = std::string(s3);
          if (tag == "name") {
            std::string device_name;
            if (xp.parse_string(xp.parsed_tag, device_name)) {
              devices[device_id].name = device_name;
              continue;
            }
          } else if (tag == "type") {
            std::string device_type;
            if (xp.parse_string(xp.parsed_tag, device_type)) {
              devices[device_id].type = device_type;
              continue;
            }
          } else if (tag == "speed") {
            std::string device_speed;
            if (xp.parse_string(xp.parsed_tag, device_speed)) {
              devices[device_id].speed = device_speed;
              continue;
            }
          } else if (tag == "temp") {
            std::string device_temp;
            if (xp.parse_string(xp.parsed_tag, device_temp)) {
              devices[device_id].temp = device_temp;
              continue;
            }
          } else if (tag == "util") {
            std::string device_util;
            if (xp.parse_string(xp.parsed_tag, device_util)) {
              devices[device_id].util = device_util;
              continue;
            }
          }
        }

        //std::cerr << "WARNING: Unexpected tag: " << xp.parsed_tag << std::endl;
    }

    std::cerr << " DEV ---- \n";
    for (auto &d : devices)
      std::cerr << d.first << " " << d.second.name << " " << d.second.type
                << " " << d.second.speed << " " << d.second.temp << " "
                << d.second.util << "\n";
    std::cerr << " DEV ---- \n";
    /** Check values */
    if (!got_progress || !got_wu_name || !got_total_speed)
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
            progress, progress, wu_name.c_str());

    int retval = boinc_db.do_query(buf);
    if (retval)
    {
        std::cerr << "Problem with progress DB query: " << buf << "\nShutting down now." << std::endl;
        boinc_db.close();
        exit(1);
    }

    char total_speed_buf[1024];
    snprintf(total_speed_buf, SQL_BUF_SIZE, "UPDATE `fc_workunit` SET `speed` = %" PRIu64 " WHERE `workunit_id` IN (SELECT id FROM workunit WHERE name = '%s') LIMIT 1 ;",
            std::stoull(total_speed), wu_name.c_str());

    retval = boinc_db.do_query(total_speed_buf);
    if (retval)
    {
        std::cerr << "Problem with total speed DB query: " << buf << "\nShutting down now." << std::endl;
        boinc_db.close();
        exit(1);
    }

    std::cerr << "Database updated successfully." << std::endl;
    return 0;
}
