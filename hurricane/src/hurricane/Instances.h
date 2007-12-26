// ****************************************************************************************************
// File: Instances.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INSTANCES
#define HURRICANE_INSTANCES

#include "Collection.h"

namespace Hurricane {

class Instance;



// ****************************************************************************************************
// Instances declaration
// ****************************************************************************************************

typedef GenericCollection<Instance*> Instances;



// ****************************************************************************************************
// InstanceLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Instance*> InstanceLocator;



// ****************************************************************************************************
// InstanceFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Instance*> InstanceFilter;



// ****************************************************************************************************
// for_each_instance declaration
// ****************************************************************************************************

#define for_each_instance(instance, instances)\
/*********************************************/\
{\
	InstanceLocator _locator = instances.GetLocator();\
	while (_locator.IsValid()) {\
		Instance* instance = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_INSTANCES

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
