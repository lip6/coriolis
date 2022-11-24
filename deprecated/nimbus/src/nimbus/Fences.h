
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_FENCES_H__
#define __NIMBUS_FENCES_H__

#include <set>
#include "hurricane/Collection.h"

namespace Nimbus {

  using namespace std;
  using namespace Hurricane;

  class Fence;

  typedef set<Fence*> FenceSet;

// ****************************************************************************************************
// Fencesdeclaration
// ****************************************************************************************************

typedef GenericCollection<Fence*> Fences;



// ****************************************************************************************************
// FenceLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Fence*> FenceLocator;



// ****************************************************************************************************
// FenceFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Fence*> FenceFilter;



// ****************************************************************************************************
// for_each_fences declaration
// ****************************************************************************************************

#define for_each_fence(fence, fences)\
/****************************************************/\
{\
    FenceLocator _locator = fences.getLocator();\
    while (_locator.isValid()) {\
        Fence* fence = _locator.getElement();\
        _locator.progress();


}
#endif // __FENCES_H
