#ifndef HURRICANE_NAMES
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
#define HURRICANE_NAMES

#include "Collection.h"

namespace Hurricane {

class Name;



// ****************************************************************************************************
// Names declaration
// ****************************************************************************************************

typedef GenericCollection<Name*> Names;



// ****************************************************************************************************
// NameLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Name*> NameLocator;



// ****************************************************************************************************
// NameFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Name*> NameFilter;



// ****************************************************************************************************
// for_each_name declaration
// ****************************************************************************************************

#define for_each_name(name, names)\
/****************************************************/\
{\
    NameLocator _locator = names.getLocator();\
    while (_locator.isValid()) {\
        Name* name = _locator.getElement();\
        _locator.progress();

} // End of Hurricane namespace.

#endif // HURRICANE_NAMES 
