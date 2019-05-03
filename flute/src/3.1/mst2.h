#ifndef FLUTE_MST2_H
#define FLUTE_MST2_H

#include "global.h"

namespace Flute {

void  mst2_package_init( long  n );
void  mst2_package_done();
void  mst2( long n, Point* pt, long* parent );

}  // Flute namespace.

#endif 

