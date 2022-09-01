/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackWithMask.hpp"

#include "Logging.hpp"

/* Private */

void AttackWithMask::addMask(const std::vector<std::string> &customCharsets) {
  std::string mask;

  if (config_.find("mask", mask)) {
    //convert to hex
    {
      std::ostringstream logStr;
      logStr<<"Attack with mask uses charsets. Converting mask to hex. Before: "<<mask.c_str();
      Logging::debugPrint(Logging::Detail::Important, logStr.str().c_str());
    }
    std::ostringstream hexConvertor;
    hexConvertor<<std::hex<<std::setfill('0');
    for(size_t i = 0; i < mask.size(); ++i)
    {
      char c = mask[i];
      if(c == '?')
      {
        ++i;
        if(mask[i] != '?')
        {
          //is charset, output as is
          hexConvertor<<'?'<<mask[i];
          continue;
        }
        //is question mark
        c = '?';
      }
      hexConvertor<<std::setw(2)<<int(c);
    }
    mask = hexConvertor.str();
    {
      std::ostringstream logStr;
      logStr<<"After conversion: "<<mask.c_str();
      Logging::debugPrint(Logging::Detail::Important, logStr.str().c_str());
    }

    std::fstream mask_file;
    File::openWriteStream(mask_file, HashcatConstant::MaskFileName, std::ios_base::trunc);

    for (std::string customCharset : customCharsets)
      mask_file << customCharset << ",";

    mask_file << mask;
    mask_file.close();
    addArgument(HashcatConstant::MaskFileName);
  }
  else
  {
    RunnerUtils::runtimeException("Missing hashcat required argument \"mask\"");
  }
}

void AttackWithMask::createMaskAndCharsets() {

  std::vector<std::string> customCharsets;
  for (size_t i = 0; i < 4; ++i) {
    std::ostringstream charsetNameStr;
    charsetNameStr << "charset" << i;
    std::string charsetName = charsetNameStr.str();

    std::string customCharset;

    if (config_.find(charsetName, customCharset))
      customCharsets.push_back(customCharset);
  }

  addMask(customCharsets);
  addArgument("--hex-charset");
}

/* Public */
AttackWithMask::AttackWithMask(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory) {
  
}
