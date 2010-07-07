// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-07 13:37:14" - OpenAccessWrapper.h
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

#ifndef __OPENACCESSWRAPPER_H__
#define __OPENACCESSWRAPPER_H__

#include <string>

namespace Hurricane {
    class Library;
    class Cell;
}

namespace CRL {
    class OpenAccessWrapper {
    public:
        static void oaDriver(const std::string& cellPath, Cell* cell);
    };
}

#endif /*__OPENACCESSWRAPPER_H__ */
