// ****************************************************************************************************
// File: MainViews.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_MAIN_VIEWS
#define HURRICANE_MAIN_VIEWS

#include "Collection.h"

namespace Hurricane {

class MainView;



// ****************************************************************************************************
// MainViews declaration
// ****************************************************************************************************

typedef GenericCollection<MainView*> MainViews;



// ****************************************************************************************************
// MainViewLocator declaration
// ****************************************************************************************************

typedef GenericLocator<MainView*> MainViewLocator;



// ****************************************************************************************************
// MainViewFilter declaration
// ****************************************************************************************************

typedef GenericFilter<MainView*> MainViewFilter;



// ****************************************************************************************************
// for_each_main_view declaration
// ****************************************************************************************************

#define for_each_main_view(mainView, mainViews)\
/**********************************************/\
{\
	MainViewLocator _locator = mainViews.GetLocator();\
	while (_locator.IsValid()) {\
		MainView* mainView = _locator.GetElement();\
		_locator.Progress();



} // End of Hurricane namespace.

#endif // HURRICANE_MAIN_VIEWS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
