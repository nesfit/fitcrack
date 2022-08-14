/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackWithMask.hpp"

#include "Logging.hpp"

/* Private */

void AttackWithMask::addMask() {
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
    mask_file.write(mask.data(), mask.length());
    mask_file.close();
    addArgument(HashcatConstant::MaskFileName);
  }
  else
  {
    RunnerUtils::runtimeException("Missing hashcat required argument \"mask\"");
  }
}

void AttackWithMask::createMaskAndCharsets() {

  for(size_t i = 0; i < 4; ++i)
  {
    std::ostringstream charsetNameStr;
    charsetNameStr<<"charset"<<i;
    std::string charsetName = charsetNameStr.str();
    //TODO: maybe store to file than arg?
    findAndAddOptional(charsetName, "--custom-"+charsetName);
  }

  addMask();
  addArgument("--hex-charset");
}

/* Public */
AttackWithMask::AttackWithMask(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory) {
  
}
