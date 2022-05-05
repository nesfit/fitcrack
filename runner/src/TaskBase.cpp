/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#include "TaskBase.hpp"
#include "Attack.hpp"
#include "Trickle.hpp"

/* Protected */

void TaskBase::actualizeComputedHashes(
    const unsigned long long newly_computed_hashes) {
  computed_hashes_ += newly_computed_hashes;
}

/* Public */

TaskBase::TaskBase(Directory &directory, ConfigTask &task_config,
                   const std::string &host_config,
                   const std::string &output_file,
                   const std::string &workunit_name)
    : task_config_(task_config), host_config_(host_config),
      directory_(directory), computed_hashes_(0), total_hashes_(0),
      output_file_(output_file), workunit_name_(workunit_name) {}

TaskBase::TaskBase(Directory &directory, ConfigTask &task_config,
                   ConfigHost &host_config, const std::string &output_file,
                   const std::string &workunit_name)
    : task_config_(task_config), host_config_(host_config),
      directory_(directory), computed_hashes_(0), total_hashes_(0),
      output_file_(output_file), workunit_name_(workunit_name) {}

double TaskBase::fractionDone() {
  if (computed_hashes_ > total_hashes_)
    return 0.0;
  // RunnerUtils::runtimeException("portion_done is bigger than total");
  double fraction_done = computed_hashes_ / (double)total_hashes_;
  return (fraction_done != fraction_done ? 0.0 : fraction_done); // NaN test
}

void TaskBase::writeOutputFile(std::string &output_message) {
  std::fstream output_stream;
  File result_file;

  PRINT_POSITION_IN_CODE();

  if (!directory_.find(BoincConstants::ResultFileName, result_file)) {
    result_file.changeTo(BoincConstants::ResultFileName);
  }

  Logging::debugPrint(Logging::Detail::GeneralInfo,
                      "Result_file : " + result_file.getRelativePath());

  File::openWriteStream(output_stream, result_file.getRelativePath(),
                        std::fstream::trunc);
  output_stream.write(output_message.c_str(), output_message.length());

  PRINT_POSITION_IN_CODE();

  output_stream.close();
}

void TaskBase::reportProgress() {
  double fraction_done, percent_done;
  fraction_done = fractionDone();
  percent_done = fraction_done * 100.0;

  Logging::debugPrint(
      Logging::Detail::DevelDebug,
      "Progress : " + RunnerUtils::toString(fraction_done) +
          " percent done : " + RunnerUtils::toString(percent_done) +
          " rinted : " + RunnerUtils::toString(::rint(percent_done)));

  // Add call of the trickler progress message
  if (!boinc_is_standalone()) {
    Trickle trickle_xml;

    trickle_xml.addElement("workunit_name", workunit_name_);
    trickle_xml.addElement("progress", percent_done);

    if (!status_info_.empty()) {
      trickle_xml.addElement("total_speed", getTotalSpeed());

      for (const auto &device : status_info_.at("devices")) {
        std::string id = std::to_string((int)device.at("device_id"));
        std::string name = device.at("device_name");
        std::string type = device.at("device_type");
        int64_t speed = device.at("speed");
        int64_t temp = device.value("temp", -1); // -1 when hwmon is disabled
        int64_t util = device.at("util");

        trickle_xml.addElement("device_" + id + "_name", name);
        trickle_xml.addElement("device_" + id + "_type", type);
        trickle_xml.addElement("device_" + id + "_speed", speed);
        trickle_xml.addElement("device_" + id + "_temp", temp);
        trickle_xml.addElement("device_" + id + "_util", util);
      }
    }

    const std::string &trickle_message = trickle_xml.getXml();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored                                                 \
    "-Wwrite-strings" // disable G++ warning about deprecated cast
    boinc_send_trickle_up(
        const_cast<char *>(BoincConstants::TrickleDeamonName.c_str()),
        const_cast<char *>(
            trickle_message.c_str())); // progress to boinc project server
#pragma GCC diagnostic pop
    Logging::debugPrint(Logging::Detail::DevelDebug,
                        " trickle daemon name :" +
                            BoincConstants::TrickleDeamonName);
    Logging::debugPrint(Logging::Detail::ObjectContentRevision,
                        " generated trickle message :" + trickle_message);
    boinc_fraction_done(fraction_done); // progress to BOINC manager
  }
  std::cout << "Progress : " << RunnerUtils::toString(computed_hashes_) << "/"
            << RunnerUtils::toString(total_hashes_) << " : "
            << RunnerUtils::toString(percent_done) << "%" << std::endl;
}

int TaskBase::saveAndFinish() {
  PRINT_POSITION_IN_CODE();
  finish();
  PRINT_POSITION_IN_CODE();
  saveResult();
  PRINT_POSITION_IN_CODE();
  return exit_code_;
}

void TaskBase::saveResult() {
  std::string output_message;

  PRINT_POSITION_IN_CODE();
  output_message = generateOutputMessage();
  PRINT_POSITION_IN_CODE();

  Logging::debugPrint(Logging::Detail::GeneralInfo,
                      "Result: \n" + output_message);

  writeOutputFile(output_message);
}

uint64_t TaskBase::getTotalSpeed() {
  if (!status_info_.contains("devices"))
    return 0;

  uint64_t speed_sum = 0;
  for (const auto &device : status_info_.at("devices")) {
    uint64_t device_speed = device.at("speed");
    speed_sum += device_speed;
  }

  return speed_sum;
}
