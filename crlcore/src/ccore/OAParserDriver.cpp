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

    OAParser::OAParser(const std::string& cellLibPath,const std::string& cellLibName,
                       const std::string& techLibPath,const std::string& techLibName) 
        : _cellLibPath(cellLibPath), _cellLibName(cellLibName), 
          _techLibPath(techLibPath), _techLibName(techLibName) {
    }
    
    Cell* OAParser::open(const std::string& cellName) {
        return CRL::OpenAccess::oaCellParser(_cellLibPath, _cellLibName, cellName,
                                             _techLibPath,_techLibName);
    }
    
}// namespace CRL
