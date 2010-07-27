// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/OADriver.h"
#include "crlcore/OAParser.h"
#include "openaccess/OpenAccess.h"

namespace CRL {
    OADriver::OADriver(Cell* cell) : _cell(cell) {}

    void OADriver::save(const string& filePath) {
        CRL::OpenAccess::oaDriver(filePath, _cell);
    }

    OAParser::OAParser(const string& libPath,const string& cellName) 
        : _libPath(libPath), _cellName(cellName) {}
    
    Cell* OAParser::open() {
        //dummy for now
        return NULL;
    }
    
}// namespace CRL
