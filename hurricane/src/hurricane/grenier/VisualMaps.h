#ifndef HURRICANE_VISUAL_MAPS
#define HURRICANE_VISUAL_MAPS

#include "Collection.h"

namespace Hurricane {

class VisualMap;



// ****************************************************************************************************
// VisualMaps declaration
// ****************************************************************************************************

typedef GenericCollection<VisualMap*> VisualMaps;



// ****************************************************************************************************
// VisualMapLocator declaration
// ****************************************************************************************************

typedef GenericLocator<VisualMap*> VisualMapLocator;



// ****************************************************************************************************
// VisualMapFilter declaration
// ****************************************************************************************************

typedef GenericFilter<VisualMap*> VisualMapFilter;



// ****************************************************************************************************
// for_each_view declaration
// ****************************************************************************************************

#define for_each_visual_map(visualMap, visualMaps)\
/*********************************/\
{\
	VisualMapLocator _locator = visualMaps.GetLocator();\
	while (_locator.IsValid()) {\
		VisualMap* visualMap = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_VISUAL_MAPS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
