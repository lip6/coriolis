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
        std::string _libPath;
        std::string _libName;
    public:
        OAParser(const std::string& libPath,const std::string& libName);
        Hurricane::Cell* open(const  std::string& cellName);
    };
} // End of CRL namespace.

#endif//__OA_PARSER_H__
