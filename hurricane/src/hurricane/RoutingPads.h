// ****************************************************************************************************
// 
// This file is part of the Coriolis Project.
// Copyright (c) 2001-2006  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: RoutingPads.h
// Authors: H.Clement M.Sroka
// ****************************************************************************************************

#ifndef HURRICANE_ROUTINGPADS
#define HURRICANE_ROUTINGPADS

#include "Collection.h"

namespace Hurricane {

class RoutingPad;



// ****************************************************************************************************
// RoutingPads declaration
// ****************************************************************************************************

typedef GenericCollection<RoutingPad*> RoutingPads;



// ****************************************************************************************************
// RoutingPadLocator declaration
// ****************************************************************************************************

typedef GenericLocator<RoutingPad*> RoutingPadLocator;



// ****************************************************************************************************
// RoutingPadFilter declaration
// ****************************************************************************************************

typedef GenericFilter<RoutingPad*> RoutingPadFilter;



// ****************************************************************************************************
// for_each_routing_pad declaration
// ****************************************************************************************************

#define for_each_routing_pad(routingpad, routingpads)\
/*********************************/\
{\
	RoutingPadLocator _locator = routingpads.getLocator();\
	while (_locator.isValid()) {\
		RoutingPad* routingpad = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_ROUTINGPADS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
