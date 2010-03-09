// author : Damien Dupuis
// date   : 08.12.2009
// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/GdsDriver.h"
#include "agds/Agds.h"

namespace CRL {
    GdsDriver::GdsDriver(Cell* cell) : _cell(cell), _name(""), _lib(""), _uUnits(0), _pUnits(0) {}

    bool GdsDriver::save(const string& filePath) {
        CRL::agdsDriver(filePath, _cell, _name, _lib, _uUnits, _pUnits);
        return true;
    }
}// namespace CRL
