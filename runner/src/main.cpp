/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include <main.hpp>

#include <memory>

int main(int argc, char **argv) {

  BOINC_OPTIONS options;
  RunnerUtils::initializeBoinc(options);

  int error_value = 0;

  std::string exception_message;

  try {
    Logging::debugPrint(Logging::Detail::Important, "Runner " RUNNER_VERSION);

    Directory directory(".");
    File file;

    directory.scanForEntities();

    if (!directory.findVersionedFile("hashcat_files", "zip", file)) {
      RunnerUtils::runtimeException("Couldn't find hashcat_files archive.");
      return -1;
    }

    unsigned long long extraction_start = RunnerUtils::timeInMs();
    //FileZip hashcatkernels(file);
    //hashcatkernels.extract();
    unsigned long long extraction_end = RunnerUtils::timeInMs();

    Logging::debugPrint(Logging::Detail::DevelDebug, " whole EXTRACTION took: " + RunnerUtils::toString(extraction_end - extraction_start) + "ms");

    directory.scanForEntities();

    const char *config = argc > 1 ?argv[1] : "config";
    directory.find(config, file);

    std::unique_ptr<TaskBase> task(Task::create(file, directory));

    task->initialize();
    task->startComputation();
    task->progress();
    error_value = task->saveAndFinish();
    error_value = (error_value == 1 ? 0 : error_value);
  }
  catch (const std::exception& e) {
    exception_message = e.what();
    Logging::debugPrint(Logging::Detail::Important, "Runner failed with std::runtime_error: \n what() : " + exception_message);
  }
  catch(...)
  {
    Logging::debugPrint(Logging::Detail::Important, "Runner failed with an unknown exception");
  }

  if (RunnerUtils::isStandalone()) {
    Logging::debugPrint(Logging::Detail::Important, "Results and run outputs are stored in 'out' and 'stderr.txt' files.");
  }

  if (!exception_message.empty()) {
    boinc_finish_message(error_value, exception_message.c_str(), false); //exit(0);
  } else {
    boinc_finish(error_value); // exit(0);
  }

  return 0;
}
