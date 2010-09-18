// -*-compile-command:"cd ../../../../.. && make"-*-

#ifndef __MYASSERT_H__
#define __MYASSERT_H__

#include "hurricane/Error.h"

#undef assert
#define assert(cond) if (! (cond) ) throw Error("assertion failed : " + string( #cond ) )

#endif
