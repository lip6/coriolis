// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: Pins.h
// Authors: Christophe Alexandre.
// ****************************************************************************************************

#ifndef HURRICANE_PINS
#define HURRICANE_PINS

#include "Collection.h"

namespace Hurricane {

class Pin;



// ****************************************************************************************************
// Pins declaration
// ****************************************************************************************************

typedef GenericCollection<Pin*> Pins;



// ****************************************************************************************************
// PinLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Pin*> PinLocator;



// ****************************************************************************************************
// PinFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Pin*> PinFilter;



// ****************************************************************************************************
// for_each_pin declaration
// ****************************************************************************************************

#define for_each_pin(pin, pins)\
/******************************/\
{\
	PinLocator _locator = pins.GetLocator();\
	while (_locator.IsValid()) {\
		Pin* pin = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_PINS
