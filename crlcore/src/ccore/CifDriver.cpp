// author : Damien Dupuis
// date   : 24.02.2010
// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/CifDriver.h"
#include "cif/Cif.h"

namespace CRL {
    CifDriver::CifDriver(Cell* cell) : _cell(cell), _name(""), _units(""), _scale(0) {}

    bool CifDriver::save(const string& filePath) {
        CRL::cifDriver(filePath, _cell, _name, _units, _scale);
        return true;
    }
}// namespace CRL
