//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_GCELLS_H__
#define __NIMBUS_GCELLS_H__

#include <set>
#include "hurricane/Collection.h"

namespace Nimbus {

  using namespace std;
  using namespace Hurricane;

  class GCell;

  typedef set<GCell*> GCellSet;

// ****************************************************************************************************
// GCellsdeclaration
// ****************************************************************************************************

typedef GenericCollection<GCell*> GCells;



// ****************************************************************************************************
// GCelleLocator declaration
// ****************************************************************************************************

typedef GenericLocator<GCell*> GCellLocator;



// ****************************************************************************************************
// GCelleFilter declaration
// ****************************************************************************************************

typedef GenericFilter<GCell*> GCellFilter;



// ****************************************************************************************************
// for_each_routboxes declaration
// ****************************************************************************************************

#define for_each_gcell(gcell, gcells)\
/************************************/\
{\
    GCellLocator _locator = gcells.getLocator();\
    while (_locator.isValid()) {\
        GCell* gcell = _locator.getElement();\
        _locator.progress();


}
#endif // __GCELLS_H
