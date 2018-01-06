// ****************************************************************************************************
// File: ./hurricane/Layers.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#ifndef HURRICANE_LAYERS
#define HURRICANE_LAYERS

#include "hurricane/Collection.h"

namespace Hurricane {

class Layer;



// ****************************************************************************************************
// Layers declaration
// ****************************************************************************************************

typedef GenericCollection<Layer*> Layers;



// ****************************************************************************************************
// LayerLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Layer*> LayerLocator;



// ****************************************************************************************************
// LayerFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Layer*> LayerFilter;



// ****************************************************************************************************
// for_each_layer declaration
// ****************************************************************************************************

#define for_each_layer(layer, layers)\
/************************************/\
{\
    LayerLocator _locator = layers.getLocator();\
    while (_locator.isValid()) {\
        Layer* layer = _locator.getElement();\
        _locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_LAYERS


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
