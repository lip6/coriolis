//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_SPLITTERS__
#define __NIMBUS_SPLITTERS__

#include "hurricane/Collection.h"

namespace Nimbus {

  using namespace Hurricane;

  class Splitter;



// ****************************************************************************************************
// Splitters declaration
// ****************************************************************************************************

typedef GenericCollection<Splitter*> Splitters;



// ****************************************************************************************************
// SplitterLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Splitter*> SplitterLocator;



// ****************************************************************************************************
// SplitterFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Splitter*> SplitterFilter;



// ****************************************************************************************************
// for_each_splitter declaration
// ****************************************************************************************************

#define for_each_splitter(splitter, splitters)\
/******************************************/\
{\
	SplitterLocator _locator = splitters.getLocator();\
	while (_locator.isValid()) {\
		Splitter* splitter = _locator.getElement();\
		_locator.progress();


}
#endif // __NIMBUS_SPLITTERS__
