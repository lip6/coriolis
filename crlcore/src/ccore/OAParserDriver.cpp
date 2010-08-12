// -*- C++ -*-

#include "hurricane/Cell.h"
using namespace Hurricane;

#include "crlcore/OADriver.h"
#include "crlcore/OAParser.h"
#include "openaccess/OpenAccess.h"

namespace CRL {
    OADriver::OADriver(Cell* cell) : _cell(cell) {}

    void OADriver::save(const std::string& filePath) {
        CRL::OpenAccess::oaDriver(filePath, _cell);
    }

    OAParser::OAParser(const std::string& libPath,const std::string& libName) 
        : _libPath(libPath), _libName(libName) {}
    
    Cell* OAParser::open(const std::string& cellName) {
        return CRL::OpenAccess::oaCellParser(_libPath,
                                             _libName, cellName);
    }
    
}// namespace CRL
