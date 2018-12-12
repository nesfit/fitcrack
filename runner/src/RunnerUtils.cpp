/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "RunnerUtils.hpp"

void RunnerUtils::initializeBoinc(BOINC_OPTIONS& options) {
    boinc_options_defaults(options);
    options.multi_process = true;   // if your app uses multiple processes
    options.multi_thread = true;    // if your app's main process uses multiple threads
    options.normal_thread_priority = true;

    boinc_init_options(&options);
}

bool RunnerUtils::isStandalone() {
    return boinc_is_standalone();
}

unsigned long RunnerUtils::stoul(const std::string& number, size_t *idx, int base) {
    char *end_pointer;
    unsigned long value = strtoul(number.c_str(), &end_pointer, base);

    if (end_pointer == number.c_str()) {

        throw std::invalid_argument("RunnerUtils::stoul : invalid argument : end_ptr == number.c_str() for string " + number);
    }

    if (value == ULONG_MAX && errno == ERANGE) {
        throw std::out_of_range("RunnerUtils::stoul");
    }

    if (idx) {
        *idx = end_pointer - number.c_str();
    }
    return value;
}

unsigned long long RunnerUtils::stoull(const std::string& number, size_t *idx, int base) {
    char *end_pointer;
    unsigned long long value = strtoull(number.c_str(), &end_pointer, base);

    if (end_pointer == number.c_str()) {
        throw std::invalid_argument("RunnerUtils::stoull : invalid argument : end_ptr == number.c_str() for string " + number);
    }

    if (value == ULLONG_MAX && errno == ERANGE) {
        throw std::out_of_range("RunnerUtils::stoull");
    }

    if (idx) {
        *idx = end_pointer - number.c_str();
    }
    return value;
}

unsigned long long RunnerUtils::timeInMs() {
    unsigned long long current_time = 0;
    struct timespec *t;

    t = new struct timespec;

    clock_gettime(CLOCK_REALTIME, t);
    current_time = (unsigned long long)((t->tv_sec * 1E3) + (t->tv_nsec / 1E6));

    delete t;
    return current_time;
}

