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
        std::string _cellName;
    public:
        OAParser(const string& libPath,const string& cellName);
        Hurricane::Cell* open();
    };
} // End of CRL namespace.

#endif//__OA_PARSER_H__
