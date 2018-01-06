// ****************************************************************************************************
// File: MainView.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************

#include "MainView.h"
#include "MapView.h"
#include "Cell.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// MainView implementation
// ****************************************************************************************************

MainView::MainView()
// *****************
:	Inherit(),
	_cell(NULL),
	_mapView(NULL)
{
}

MainView* MainView::Create()
// *************************
{
	MainView* mainView = new MainView();

	mainView->_PostCreate();

	return mainView;
}

bool MainView::Update(bool useDoubleBuffering)
// *******************************************
{
	if (Inherit::Update(useDoubleBuffering)) {
		if (_mapView) {
			_mapView->FitToContent();
			_mapView->Update(useDoubleBuffering);
		}
		return true;
	}
	return false;
}

void MainView::_PostCreate()
// *************************
{
	Inherit::_PostCreate();

	_mapView = MapView::_Create(this);
}

void MainView::_PreDelete()
// ************************
{
	Inherit::_PreDelete();

	if (_mapView) _mapView->_Delete();
}

string MainView::_GetString() const
// ********************************
{
	return Inherit::_GetString();
}

Record* MainView::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("MapView", _mapView));
	}
	return record;
}

void MainView::_SetCell(Cell* cell)
// ********************************
{
	if (cell != _cell) {
		UnselectAll();
		if (_cell) {
			if (_mapView) _cell->_GetViewSet()._Remove(_mapView);
			_cell->_GetViewSet()._Remove(this);
		}
		_cell = cell;
		if (_cell) {
			_cell->_GetViewSet()._Insert(this);
			if (_mapView) _cell->_GetViewSet()._Insert(_mapView);
		}
	}
}

void MainView::_SetTransformation(const Transformation& transformation)
// ********************************************************************
{
	_transformation = transformation;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
