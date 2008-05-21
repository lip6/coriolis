// ****************************************************************************************************
// File: Libraries.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_LIBRARIES
#define HURRICANE_LIBRARIES

#include "hurricane/Collection.h"

namespace Hurricane {

class Library;



// ****************************************************************************************************
// Libraries declaration
// ****************************************************************************************************

typedef GenericCollection<Library*> Libraries;



// ****************************************************************************************************
// LibraryLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Library*> LibraryLocator;



// ****************************************************************************************************
// LibraryFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Library*> LibraryFilter;



// ****************************************************************************************************
// for_each_library declaration
// ****************************************************************************************************

#define for_each_library(library, libraries)\
/*******************************************/\
{\
    LibraryLocator _locator = libraries.getLocator();\
    while (_locator.isValid()) {\
        Library* library = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_LIBRARIES

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
