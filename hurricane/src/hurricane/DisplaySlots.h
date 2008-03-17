// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: DisplaySlots.h
// Author: C. Alexandre
// ****************************************************************************************************

#ifndef HURRICANE_DISPLAY_SLOTS
#define HURRICANE_DISPLAY_SLOTS

#include "Collection.h"

namespace Hurricane {

class DisplaySlot;



// ****************************************************************************************************
// DisplaySlots declaration
// ****************************************************************************************************

typedef GenericCollection<DisplaySlot*> DisplaySlots;



// ****************************************************************************************************
// DisplaySlotLocator declaration
// ****************************************************************************************************

typedef GenericLocator<DisplaySlot*> DisplaySlotLocator;



// ****************************************************************************************************
// DisplaySlotFilter declaration
// ****************************************************************************************************

typedef GenericFilter<DisplaySlot*> DisplaySlotFilter;



// ****************************************************************************************************
// for_each_view declaration
// ****************************************************************************************************

#define for_each_display_slot(displaySlot, displaySlots)\
/******************************************************/\
{\
    DisplaySlotLocator _locator = displaySlots.getLocator();\
    while (_locator.IsValid()) {\
        DisplaySlot* displaySlot = _locator.getElement();\
        _locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_DISPLAY_SLOTS
