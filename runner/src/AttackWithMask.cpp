/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackWithMask.hpp"

#include "Logging.hpp"

/* Private */

void AttackWithMask::addCharset(std::fstream& mask_file, const std::string& charset_name) {
  std::string content;

  if (config_.find(charset_name, content)) {
    content += ",";
    mask_file.write(content.data(), content.length());
    setHasCharsets();
  }
}

void AttackWithMask::addMask(std::fstream& mask_file) {
  std::string mask;

  if (config_.find("mask", mask)) {
    //convert to hex
    if(has_charsets_)
    {
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
    }
    mask_file.write(mask.data(), mask.length());
  }
}

void AttackWithMask::createMaskFile() {
  std::fstream mask_file;

  File::openWriteStream(mask_file, HashcatConstant::MaskFileName, std::ios_base::trunc);

  addCharset(mask_file, "charset1");
  addCharset(mask_file, "charset2");
  addCharset(mask_file, "charset3");
  addCharset(mask_file, "charset4");

  addMask(mask_file);

  mask_file.close();

  directory_.scanForEntities();
}

void AttackWithMask::setHasCharsets() {
  has_charsets_ = true;
}

/* Public */
AttackWithMask::AttackWithMask(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory), has_charsets_(false) {
  
}
