/*
* Author : see AUTHORS
* Licence: MIT, see LICENSE
*/

#include "AttackHybridDictMask.hpp"

AttackHybridDictMask::AttackHybridDictMask(const ConfigTask& config, Directory& directory) : AttackWithMask(config, directory) {
  
}

void AttackHybridDictMask::addSpecificArguments() {

  AttackCrackingBase::addSpecificArguments();


  createMaskFile();
  addRequiredFile(HashcatConstant::MaskFileName);

  if (has_charsets_) {
    addArgument("--hex-charset");
  }
  addRequiredFile("dict");
}
