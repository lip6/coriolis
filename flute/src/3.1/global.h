#ifndef  FLUTE_GLOBAL_H
#define  FLUTE_GLOBAL_H

#include <stdio.h>

namespace Flute {

#define  TRUE   1
#define  FALSE  0
#define  MAXLONG  0x7fffffffL

struct point
{
  long    x, y;
};

typedef  struct point Point;

typedef  long nn_array[8];

}  // Flute namespace.

#endif  /* FLUTE_GLOBAL_H */
