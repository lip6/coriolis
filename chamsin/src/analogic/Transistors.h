
// ****************************************************************************************************
// File: Transistors.h
// Authors: YIFEI WU 
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_TRANSISTORS
#define HURRICANE_TRANSISTORS

#include "Collection.h"

BEGIN_NAMESPACE_HURRICANE

class Transistor;


// ****************************************************************************************************
// Transistors declaration
// ****************************************************************************************************

typedef GenericCollection<Transistor*> Transistors;



// ****************************************************************************************************
// TransistorLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Transistor*> TransistorLocator;



// ****************************************************************************************************
// TransistorFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Transistor*> TransistorFilter;



// ****************************************************************************************************
// for_each_transistor declaration
// ****************************************************************************************************

#define for_each_transistor(transistor, transistors)\
/******************************/\
{\
	TransistorLocator _locator = transistors.GetLocator();\
	while (_locator.IsValid()) {\
		Transistor* transistor = _locator.GetElement();\
		_locator.Progress();



END_NAMESPACE_HURRICANE

#endif // HURRICANE_TRANSISTORS

