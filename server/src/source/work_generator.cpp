/**
 * @file WorkGenerator.cpp
 * @brief This is the entry place of Work Generator
 * It loads arguments and runs selected generator
 *
 * @authors Lukas Zobal (zobal.lukas(at)gmail.com)
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */

#include <Config.h>
#include <SimpleGenerator.h>

/** BOINC includes */
#include <util.h>
#include <sched_util_basic.h>


void usage(char *name)
{
    Tools::printDebug("This is an Fitcrack BOINC work generator.\n"
        "This work generator has the following properties\n"
        "  It attempts to maintain a \"cushion\" of 2 unsent job instances for each host.\n"
        "- Creates work for the application \"example_app\".\n"
        "- Creates a new input file for each job;\n"
        "  the file (and the workunit names) contain a timestamp\n"
        "  and sequence number, so that they're unique.\n\n"
        "Usage: %s [OPTION]...\n\n"
        "Options:\n"
        "  [ --app X                Application name (default: fitcrack)\n"
        "  [ -h | --help ]          Shows this help text.\n", name);
}


void parseArguments(int argc, char * argv[])
{
    int i, retval;
    char buf[Config::SQL_BUF_SIZE];
    Config::app = new DB_APP;

    /**     Argument parsing */
    for (i = 1; i < argc; ++i)
    {
        /** Set debug level */
        if (is_arg(argv[i], "d"))
        {
            if (!argv[++i])
            {
                /** Missing debug level argument */
                log_messages.printf(MSG_CRITICAL, "%s requires an argument\n\n", argv[--i]);
                usage(argv[0]);
                exit(1);
            }

            int dl = atoi(argv[i]);
            log_messages.set_debug_level(dl);
            if (dl == 4)
            {
                g_print_queries = true;
            }
        }

        /** Set application name */
        else if (!strcmp(argv[i], "--app"))
        {
            if (!argv[++i])
            {
                /** Missing application name argument */
                log_messages.printf(MSG_CRITICAL, "%s requires an argument\n\n", argv[--i]);
                usage(argv[0]);
                exit(1);
            }

            Config::appName = argv[i];
        }

        /** Show help message */
        else if (is_arg(argv[i], "h") || is_arg(argv[i], "help"))
        {
            usage(argv[0]);
            exit(0);
        }

        /** Unknown argument */
        else
        {
            log_messages.printf(MSG_CRITICAL, "unknown command line argument: %s\n\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    /** Parse config file */
    retval = config.parse_file();
    if (retval)
    {
        log_messages.printf(MSG_CRITICAL, "Can't parse config.xml: %s\n", boincerror(retval));
        exit(1);
    }

    /** Open database */
    retval = boinc_db.open(config.db_name, config.db_host, config.db_user, config.db_passwd);
    if (retval)
    {
        log_messages.printf(MSG_CRITICAL, "can't open db\n");
        exit(1);
    }

    /** Find application */
    std::snprintf(buf, sizeof(buf), "where name='%s'", Config::appName);
    if (Config::app->lookup(buf))
    {
        log_messages.printf(MSG_CRITICAL, "can't find app %s\n", Config::appName);
        exit(1);
    }

    /** Find input templates */
    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileBench.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathBench))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileMask.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathMask))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileMarkov.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathMarkov))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileDict.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathDict))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileDictAlt.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathDictAlt))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFilePrince.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathPrince)) {
      log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
      exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s",
                  Config::inTemplateFilePrinceRules.c_str());
    if (read_file_malloc(config.project_path(buf),
                         Config::inTemplatePathPrinceRules)) {
      log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
      exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFilePcfg.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathPcfg))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFilePcfgRules.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathPcfgRules))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileCombinator.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathCombinator))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileHybridDictMask.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathHybridDictMask))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileHybridMaskDict.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathHybridMaskDict))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileRule.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathRule))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    std::snprintf(buf, sizeof(buf), "templates/%s", Config::inTemplateFileRuleAlt.c_str());
    if (read_file_malloc(config.project_path(buf), Config::inTemplatePathRuleAlt))
    {
        log_messages.printf(MSG_CRITICAL, "can't read input template %s\n", buf);
        exit(1);
    }

    /** Set starting values */
    Config::startTime = time(0);
    Config::seqNo = 0;
    Config::projectDir = std::string("/root/project/log_") + Config::appName + "/";

    Tools::printDebugTimestamp("BOINC arguments parsed successfully\n");
}


int main(int argc, char * argv[])
{
    Tools::printDebugTimestamp("Starting Fitcrack WorkGenerator ...\n");

    /** BOINC argument parsing */
    parseArguments(argc, argv);

    /** Run Generator */
    CSimpleGenerator simpleGenerator;
    simpleGenerator.run();

    /** Delete DB_APP */
    delete Config::app;

    return 0;
}