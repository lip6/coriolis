// author : Damien Dupuis
// date   : 08.12.2009
// -*- C++ -*-

#ifndef __AGDS_H__
#define __AGDS_H__

#include <string>

namespace Hurricane {
  class Cell;
}

namespace CRL {
  void agdsDriver(const std::string& filePath, Hurricane::Cell* cell, std::string& name, std::string& lib, double& uUnits, double& pUnits);
}
# endif
