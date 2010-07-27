// -*-compile-command:"cd ../../../../.. && make"-*-
// Time-stamp: "2010-07-26 16:16:41" - OpenAccess.h
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

#ifdef HAVE_OPENACCESS
#include "oa/oaDesignDB.h"
using namespace oa;
#else
namespace oa {
    class oaCell;
    class oaLib;
}
#endif

namespace CRL {
    class OpenAccess {
    public:
        static Hurricane::Cell* oaCellParser(oa::oaCell* cell);
        static Hurricane::Library* oaLibParser(oa::oaLib* lib);
        static oa::oaCell* oaDriver(const std::string& libPath, Hurricane::Cell* cell);
    };
}//namespace CRL

#endif /*__OPENACCESS_H__ */
