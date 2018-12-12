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
        if (xp.parse_str("workunit_name", wu_name, SQL_BUF_SIZE))
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
    snprintf(buf, SQL_BUF_SIZE, "UPDATE `fc_job` SET `progress` = %f WHERE `progress` < %f AND `workunit_id` IN (SELECT id FROM workunit WHERE name = '%s') LIMIT 1 ;",
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
