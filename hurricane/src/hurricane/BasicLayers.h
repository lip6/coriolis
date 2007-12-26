// ****************************************************************************************************
// File: BasicLayers.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_BASIC_LAYERS
#define HURRICANE_BASIC_LAYERS

#include "Collection.h"

namespace Hurricane {

class BasicLayer;



// ****************************************************************************************************
// BasicLayers declaration
// ****************************************************************************************************

typedef GenericCollection<BasicLayer*> BasicLayers;



// ****************************************************************************************************
// BasicLayerLocator declaration
// ****************************************************************************************************

typedef GenericLocator<BasicLayer*> BasicLayerLocator;



// ****************************************************************************************************
// BasicLayerFilter declaration
// ****************************************************************************************************

typedef GenericFilter<BasicLayer*> BasicLayerFilter;



// ****************************************************************************************************
// for_each_basic_layer declaration
// ****************************************************************************************************

#define for_each_basic_layer(basicLayer, basicLayers)\
/****************************************************/\
{\
	BasicLayerLocator _locator = basicLayers.GetLocator();\
	while (_locator.IsValid()) {\
		BasicLayer* basicLayer = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_BASIC_LAYERS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
