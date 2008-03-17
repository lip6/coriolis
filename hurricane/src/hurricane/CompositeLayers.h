// ****************************************************************************************************
// File: CompositeLayers.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_COMPOSITE_LAYERS
#define HURRICANE_COMPOSITE_LAYERS

#include "Collection.h"

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
    while (_locator.IsValid()) {\
        CompositeLayer* compositeLayer = _locator.getElement();\
        _locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_COMPOSITE_LAYERS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
