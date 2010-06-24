// -*- C++ -*-

#ifndef __OA_DRIVER_H__
#define __OA_DRIVER_H__

#include <string>

namespace Hurricane {
    class Cell;
}

namespace CRL {
    class OADriver {
    public:
        OADriver(Hurricane::Cell*);
        void save(const std::string& path);
        
    private:
        Hurricane::Cell* _cell;
    };
} // End of CRL namespace.

#endif//__OA_DRIVER_H__

