// -*- C++ -*-

#ifndef __OA_DRIVER_H__
#define __OA_DRIVER_H__

#include <string>

namespace Hurricane {
    class Cell;
}

namespace CRL {
    class OADriver {
    private:
        Hurricane::Cell* _cell;
    public:
        OADriver(Hurricane::Cell*);
        void save(const std::string& technoFilePath,const std::string& designFilePath);
    };
} // End of CRL namespace.

#endif//__OA_DRIVER_H__

