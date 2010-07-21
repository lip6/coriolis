// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/OADriver.h"
#include "openaccess/OpenAccessDriver.h"

namespace CRL {
    OADriver::OADriver(Cell* cell) : _cell(cell) {}

    void OADriver::save(const string& filePath) {
        CRL::OpenAccessWrapper::oaDriver(filePath, _cell);
    }
}// namespace CRL
