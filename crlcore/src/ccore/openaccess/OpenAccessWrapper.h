// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
#ifndef __OPENACCESSWRAPPER_H
#define __OPENACCESSWRAPPER_H

#include <string>
using namespace std;

namespace Hurricane {
    class Library;
    class Cell;
}

namespace CRL {

class Catalog;

class OpenAccessWrapper {
    public:
        static void oaLibLoader(const string& libPath, Hurricane::Library* lib, Catalog& catalog);
        static void oaDesignLoader(const string& path, Hurricane::Cell* cell);
        static void oaDriver(const string& cellPath, Cell* cell, unsigned int& saveState);
};

}

#endif /*__OPENACCESSWRAPPER_H */
