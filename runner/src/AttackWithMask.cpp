/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackWithMask.hpp"

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
  if (!has_charsets_)
  has_charsets_ = true;
}

/* Public */
AttackWithMask::AttackWithMask(const ConfigTask& config, Directory& directory) : AttackCrackingBase(config, directory), has_charsets_(false) {
  
}
