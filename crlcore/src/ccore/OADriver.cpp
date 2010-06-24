// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/OADriver.h"
#include "openaccess/OpenAccessWrapper.h"

namespace CRL {
    OADriver::OADriver(Cell* cell) : _cell(cell) {}

    void OADriver::save(const string& filePath) {
        unsigned int saveState;
        CRL::OpenAccessWrapper::oaDriver(filePath, _cell, saveState);
    }
}// namespace CRL
