// ****************************************************************************************************
// File: MapView.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

#include "MapView.h"
#include "MainView.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// MapView implementation
// ****************************************************************************************************

MapView::MapView(MainView* mainView)
// *********************************
:	Inherit(),
	_mainView(mainView)
{
	if (!_mainView)
		throw Error("Can't create " + _TName("MapView") + " : null main view");

	if (_mainView->GetMapView())
		throw Error("Can't create " + _TName("MapView") + " : already exists");
}

void MapView::Delete()
// *******************
{
	throw Error("Abnormal deletion of " + _TName("MapView"));
}

Cell* MapView::GetCell() const
// ***************************
{
	return _mainView->GetCell();
}

Transformation MapView::GetTransformation() const
// **********************************************
{
	return _mainView->GetTransformation();
}

Box MapView::GetClientArea() const
// *******************************
{
	return Inherit::GetClientArea().Merge(_mainView->GetVisibleArea());
}

MapView* MapView::_Create(MainView* mainView)
// ******************************************
{
	MapView* mapView = new MapView(mainView);

	mapView->_PostCreate();

	return mapView;
}

void MapView::_PostCreate()
// ************************
{
	Inherit::_PostCreate();

	HideRubbers();
	HideMarkers();
	HideCutPoints();
	HideAllLayers();
	HideGrid();
	HideSelection();
	HideDisplaySlots();
	DisableAutoScroll();
	EnableDoubleBuffering();
}

void MapView::_Delete()
// ********************
{
	_PreDelete();

	delete this;
}

string MapView::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	return s;
}

Record* MapView::_GetRecord() const
// **************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("MainView", _mainView));
	}
	return record;
}

void MapView::_SetCell(Cell* newCell)
// **********************************
{
	assert(newCell == NULL);
}

void MapView::_SetTransformation(const Transformation& transformation)
// *******************************************************************
{
	//assert(false);
}

void MapView::_PostRepaint()
// *************************
{
	Box clientArea = _mainView->GetClientArea();
	DrawRectangle(clientArea);

	Box visibleArea = _mainView->GetVisibleArea();
	DrawRectangle(visibleArea);
	DrawLine(GetX(0),
				visibleArea.GetYCenter(),
				visibleArea.GetXMin(),
				visibleArea.GetYCenter());
	DrawLine(visibleArea.GetXMax(),
				visibleArea.GetYCenter(),
				GetX(GetScreenWidth()),
				visibleArea.GetYCenter());
	DrawLine(visibleArea.GetXCenter(),
				GetY(0),
				visibleArea.GetXCenter(),
				visibleArea.GetYMax());
	DrawLine(visibleArea.GetXCenter(),
				GetY(GetScreenHeight()),
				visibleArea.GetXCenter(),
				visibleArea.GetYMin());
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
