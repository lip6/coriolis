
// ****************************************************************************************************
// File: MetaTransistors.h
// Authors: YIFEI WU 
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_METATRANSISTORS
#define HURRICANE_METATRANSISTORS

#include "Collection.h"

class MetaTransistor;


// ****************************************************************************************************
// MetaTransistors declaration
// ****************************************************************************************************

typedef GenericCollection<MetaTransistor*> MetaTransistors;



// ****************************************************************************************************
// MetaTransistorLocator declaration
// ****************************************************************************************************

typedef GenericLocator<MetaTransistor*> MetaTransistorLocator;



// ****************************************************************************************************
// MetaTransistorFilter declaration
// ****************************************************************************************************

typedef GenericFilter<MetaTransistor*> MetaTransistorFilter;



// ****************************************************************************************************
// for_each_metatransistor declaration
// ****************************************************************************************************

#define for_each_metatransistor(metatransistor, metatransistors)\
/******************************/\
{\
	MetaTransistorLocator _locator = metatransistors.GetLocator();\
	while (_locator.IsValid()) {\
		MetaTransistor* metatransistor = _locator.GetElement();\
		_locator.Progress();



#endif // HURRICANE_METATRANSISTORS

