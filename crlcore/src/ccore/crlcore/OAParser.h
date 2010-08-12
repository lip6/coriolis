// -*- C++ -*-

#ifndef __OA_PARSER_H__
#define __OA_PARSER_H__

#include <string>

namespace Hurricane {
    class Cell;
}

namespace CRL {
    class OAParser {
    private:
        std::string _cellLibPath;
        std::string _cellLibName;
        std::string _techLibPath;
        std::string _techLibName;
    public:
        OAParser(const std::string& cellLibPath, const std::string& cellLibName, 
                 const std::string& techLibPath,const std::string& techLibName);
        Hurricane::Cell* open(const  std::string& cellName);
    };
} // End of CRL namespace.

#endif//__OA_PARSER_H__
