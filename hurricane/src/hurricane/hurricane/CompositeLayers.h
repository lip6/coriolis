// ****************************************************************************************************
// File: ./hurricane/CompositeLayers.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_COMPOSITE_LAYERS
#define HURRICANE_COMPOSITE_LAYERS

#include "hurricane/Collection.h"

namespace Hurricane {

class CompositeLayer;



// ****************************************************************************************************
// CompositeLayers declaration
// ****************************************************************************************************

typedef GenericCollection<CompositeLayer*> CompositeLayers;



// ****************************************************************************************************
// CompositeLayerLocator declaration
// ****************************************************************************************************

typedef GenericLocator<CompositeLayer*> CompositeLayerLocator;



// ****************************************************************************************************
// CompositeLayerFilter declaration
// ****************************************************************************************************

typedef GenericFilter<CompositeLayer*> CompositeLayerFilter;



// ****************************************************************************************************
// for_each_composite_layer declaration
// ****************************************************************************************************

#define for_each_composite_layer(compositeLayer, compositeLayers)\
/****************************************************************/\
{\
    CompositeLayerLocator _locator = compositeLayers.getLocator();\
    while (_locator.isValid()) {\
        CompositeLayer* compositeLayer = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_COMPOSITE_LAYERS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
