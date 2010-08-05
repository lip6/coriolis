// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-08-05 20:13:03" - OpenAccess.h
// x-----------------------------------------------------------------x
// |  This file is part of the hurricaneAMS Software.                |
// |  Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved         |
// | =============================================================== |
// |  Author      :                 Chistophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// x-----------------------------------------------------------------x
// |  Author      :                    Jean-Manuel Caba              |
// |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
// x-----------------------------------------------------------------x

#ifndef __OPENACCESS_H__
#define __OPENACCESS_H__

#include <string>

namespace Hurricane {
    class Library;
    class Cell;
}

namespace CRL {
    class OpenAccess {
    public:
        static void oaDriver(const std::string& libPath, Hurricane::Cell* cell);
        static Hurricane::Cell* oaCellParser(const std::string& libPath,
                                             const std::string& libName, const std::string& cellName);
    };
}//namespace CRL

#endif /*__OPENACCESS_H__ */
