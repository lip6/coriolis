// author : Damien Dupuis
// date   : 24.02.2010
// -*- C++ -*-

#ifndef __CIF_H__
#define __CIF_H__

#include <string>

namespace Hurricane {
  class Cell;
}

namespace CRL {
  void cifDriver(const std::string file, Hurricane::Cell*, std::string& name, std::string& units, double& scale );
}
# endif
