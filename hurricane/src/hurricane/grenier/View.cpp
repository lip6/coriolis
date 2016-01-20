// ****************************************************************************************************
// File: View.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6  (Lip6 addded DisplaySlots) (added VisualMaps)

#include "View.h"
#include "DataBase.h"
#include "Cell.h"
#include "Technology.h"
#include "CompositeLayer.h"
#include "Selector.h"
#include "VisualMap.h"
#include "DisplaySlot.h"
#include "Go.h"
#include "Command.h"
#include "Error.h"
#include "Interruption.h"

namespace Hurricane {



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

static View*    DRAG_VIEW      = NULL;
static unsigned DRAG_BUTTON    = 0;
static bool     IN_AUTO_SCROLL = false;

static unsigned SIZE = 0;
static GdkPoint* POINTS = NULL;

GdkColor  View::_backgroundColor    = { 0,  50*255,  50*255,  50*255 };
GdkColor  View::_foregroundColor    = { 0, 255*255, 255*255, 255*255 };
GdkColor  View::_rubberColor        = { 0, 192*255,   0*255, 192*255 };
GdkColor  View::_phantomColor       = { 0, 139*255, 134*255, 130*255 };
GdkColor  View::_boundaryColor      = { 0, 208*255, 199*255, 192*255 };
GdkColor  View::_markerColor        = { 0,  80*255, 250*255,  80*255 };
GdkColor  View::_selectionDrawColor = { 0, 255*255, 255*255, 255*255 };
GdkColor  View::_selectionFillColor = { 0, 255*255, 255*255, 255*255 };
GdkColor  View::_gridColor          = { 0, 255*255, 255*255, 255*255 };
GdkColor  View::_spotColor          = { 0, 255*255, 255*255, 255*255 };
GdkColor  View::_ghostColor         = { 0, 255*255, 255*255, 255*255 };


// ****************************************************************************************************
// Utilitarians
// ****************************************************************************************************

static GdkPixmap* GetPixmap(const ScreenUnit& width, const ScreenUnit& height)
// ***************************************************************************
{
	static int WIDTH = 0;
	static int HEIGHT = 0;
	static GdkPixmap* PIXMAP = NULL;
	if (!PIXMAP || (WIDTH < width) || (HEIGHT < height)) {
		if (PIXMAP) gdk_pixmap_unref(PIXMAP);
		WIDTH = width;
		HEIGHT = height;
		GdkWindow* window = gtk_get_window();
		PIXMAP = gdk_pixmap_new(window, WIDTH, HEIGHT, gdk_window_get_visual(window)->depth);
	}
	return PIXMAP;
}



// ****************************************************************************************************
// Callbacks
// ****************************************************************************************************

static void OnExpose(GtkWidget* drawingArea, GdkEventExpose* event, View* view)
// ****************************************************************************
{
	GdkRectangle area = event->area;
	view->_OnExpose(area.x, area.y, area.width, area.height, (event->count == 0));
}

static void OnConfigure(GtkWidget* drawingArea, GdkEventConfigure* event, View* view)
// **********************************************************************************
{
	view->_OnResize(event->width, event->height);
}

static void OnEnterNotify(GtkWidget* drawingArea, GdkEventCrossing* event, View* view)
// ***********************************************************************************
{
	if (!DRAG_VIEW || (DRAG_VIEW == view)) {
		ScreenUnit screenX = (ScreenUnit)event->x;
		ScreenUnit screenY = (ScreenUnit)event->y;
		Point position(GetOnGridUnit(view->GetX(screenX)), GetOnGridUnit(view->GetY(screenY)));
		view->_OnMouseEnter(position);
	}
}

static void OnMotionNotify(GtkWidget* drawingArea, GdkEventMotion* event, View* view)
// **********************************************************************************
{
	if (!DRAG_VIEW || (DRAG_VIEW == view)) {
		ScreenUnit screenX = (ScreenUnit)event->x;
		ScreenUnit screenY = (ScreenUnit)event->y;

		if ((DRAG_VIEW == view) && view->AutoScrollIsEnabled()) {
			ScreenUnit screenDx = 0;
			if (screenX < 0)
				screenDx = screenX;
			else
				if (view->GetScreenWidth() < screenX)
					screenDx = screenX - view->GetScreenWidth();
			ScreenUnit screenDy = 0;
			if (screenY < 0)
				screenDy = screenY;
			else
				if (view->GetScreenHeight() < screenY)
					screenDy = screenY - view->GetScreenHeight();
			if (screenDx || screenDy) {
				IN_AUTO_SCROLL = true;
				view->Scroll(- view->GetSize(screenDx), view->GetSize(screenDy));
				view->Update();
				IN_AUTO_SCROLL = false;
			}
			screenX = (ScreenUnit)event->x;
			screenY = (ScreenUnit)event->y;
		}

		Point position(GetOnGridUnit(view->GetX(screenX)), GetOnGridUnit(view->GetY(screenY)));
		view->_OnMouseMove(position, event->state);
	}

	// to enable next pointer motion notify event
	gint x, y;
	GdkModifierType mt;
	gdk_window_get_pointer(drawingArea->window, &x, &y, &mt);
	// see GDK_POINTER_MOTION_HINT_MASK to understand previous lines
}

static void OnLeaveNotify(GtkWidget* drawingArea, GdkEventCrossing* event, View* view)
// ***********************************************************************************
{
	if (!DRAG_VIEW || (DRAG_VIEW == view)) {
		ScreenUnit screenX = (ScreenUnit)event->x;
		ScreenUnit screenY = (ScreenUnit)event->y;
		Point position(GetOnGridUnit(view->GetX(screenX)), GetOnGridUnit(view->GetY(screenY)));
		view->_OnMouseLeave(position);
	}
}

static void OnButtonPress(GtkWidget* drawingArea, GdkEventButton* event, View* view)
// *********************************************************************************
{
    unsigned button = event->button;
	ScreenUnit screenX = (ScreenUnit)event->x;
	ScreenUnit screenY = (ScreenUnit)event->y;
	Point position(GetOnGridUnit(view->GetX(screenX)), GetOnGridUnit(view->GetY(screenY)));
    if ( ( button == 4 ) || ( button == 5 ) )
    {
		view->_OnButtonPress(button, position, event->state);
    }
    else if (!DRAG_VIEW) {
        DRAG_BUTTON = button;
		view->_OnButtonPress(button, position, event->state);
		DRAG_VIEW = view;
	}
}

static void OnButtonRelease(GtkWidget* drawingArea, GdkEventButton* event, View* view)
// ***********************************************************************************
{
	unsigned button = event->button;
	ScreenUnit screenX = (ScreenUnit)event->x;
	ScreenUnit screenY = (ScreenUnit)event->y;
	Point position(GetOnGridUnit(view->GetX(screenX)), GetOnGridUnit(view->GetY(screenY)));
    if ( ( button == 4 ) || ( button == 5 ) )
    {
		view->_OnButtonRelease(button, position, event->state);
    }
    else if (DRAG_VIEW && ( button == DRAG_BUTTON ) ) {
		view->_OnButtonRelease(button, position, event->state);
		DRAG_VIEW = NULL;
	}
}



// ****************************************************************************************************
// View implementation
// ****************************************************************************************************

View::View()
// *********
:	Inherit(),
	_frame(NULL),
	_drawingArea(NULL),
	_center(0, 0),
	_scale(1),
	_backCenter(0, 0),
	_backScale(1),
	_screenDx(0),
	_screenDy(0),
	_screenWidth(1),
	_screenHeight(1),
	_hasBeenExposed(false),
	_screenUpdateArea(),
	_clipBox(),
	_drawable(NULL),
	_backgroundGC(NULL),
	_foregroundGC(NULL),
	_phantomGC(NULL),
	_boundaryGC(NULL),
	_rubberGC(NULL),
	_markerGC(NULL),
	_selectionDrawGC(NULL),
	_selectionFillGC(NULL),
	_gridGC(NULL),
	_spotGC(NULL),
	_ghostGC(NULL),
	_drawGC(NULL),
	_fillGC(NULL),
	_commandMap(),
	_gridIsVisible(true),
	_gridDisplayThreshold(6),
	_autoScrollIsEnabled(true),
	_doubleBufferingIsEnabled(false),
	_phantomsAreVisible(true),
	_boundariesAreVisible(true),
	_rubbersAreVisible(false),
	_markersAreVisible(true),
	_cutPointsAreVisible(false),
	_selectionIsVisible(true),
	_displaySlotsAreVisible(true),
	_visualMapsAreVisible(false),
	_spotIsVisible(false),
	_spotIsDrawable(false),
	_spotPosition(),
	_visibleBasicLayersMask(~0),
	_selectorSet(),
    _rubberDisplayType(),
    _isTextVisible(false),
	_nextOfCellViewSet(NULL)
{
}

Unit View::GetX(const ScreenUnit& screenX) const
// *********************************************
{
	return GetUnit((screenX - _screenDx) / _scale);
}

Unit View::GetY(const ScreenUnit& screenY) const
// *********************************************
{
	return GetUnit(((_screenHeight - screenY) - _screenDy) / _scale);
}

Unit View::GetSize(const ScreenUnit& screenSize) const
// ***************************************************
{
	return GetUnit(screenSize / _scale);
}

Point View::GetPoint(const ScreenPoint& screenPoint) const
// *******************************************************
{
	return Point(GetX(screenPoint.GetX()), GetY(screenPoint.GetY()));
}

Box View::GetBox(const ScreenBox& screenBox) const
// ***********************************************
{
	return Box(GetX(screenBox.GetXMin()), 
				  GetY(screenBox.GetYMin()), 
				  GetX(screenBox.GetXMax()), 
				  GetY(screenBox.GetYMax()));
}

ScreenUnit View::GetScreenX(const Unit& x) const
// *********************************************
{
	return _screenDx + (ScreenUnit)(GetValue(x) * _scale);
}

ScreenUnit View::GetScreenY(const Unit& y) const
// *********************************************
{
	return _screenHeight - (_screenDy + (ScreenUnit)(GetValue(y) * _scale));
}

ScreenUnit View::GetScreenSize(const Unit& size) const
// ***************************************************
{
	return (ScreenUnit)(GetValue(size) * _scale);
}

ScreenPoint View::GetScreenPoint(const Point& point) const
// *******************************************************
{
	return Point(GetScreenX(point.GetX()), GetScreenY(point.GetY()));
}

ScreenBox View::GetScreenBox(const Box& box) const
// ***********************************************
{
	return Box(GetScreenX(box.GetXMin()),
				  GetScreenY(box.GetYMin()),
				  GetScreenX(box.GetXMax()),
				  GetScreenY(box.GetYMax()));
}

Box View::GetClientArea() const
// ****************************
{
	Box area(-1, -1, 1, 1);
	Cell* cell = GetCell();
	if (cell) {
		Box boundingBox = cell->GetBoundingBox();
		if (!boundingBox.IsEmpty()) area = boundingBox;
	}
	GetTransformation().ApplyOn(area);
	return area;
}

Box View::GetVisibleArea() const
// *****************************
{
	return Box(GetX(0), GetY(0), GetX(_screenWidth), GetY(_screenHeight));
}

Command* View::GetCommand(unsigned button) const
// *********************************************
{
	CommandMap::const_iterator it = _commandMap.find(button);
	return (it != _commandMap.end()) ? (*it).second : NULL;
}

BasicLayers View::GetVisibleBasicLayers() const
// ********************************************
{
	Technology* technology = GetDataBase()->GetTechnology();
	return (technology) ? technology->GetBasicLayers(_visibleBasicLayersMask) : BasicLayers();
}

bool View::IsVisible(BasicLayer* basicLayer) const
// ***********************************************
{
	if (!basicLayer)
		throw Error("Can't evaluate if is visible : null basic layer");

	return (basicLayer->GetMask() & _visibleBasicLayersMask);
}

bool View::IsSelected(Go* go) const
// ********************************
{
	if (!go)
		throw Error("Can't evaluate if is selected : null go");

	Occurrence occurrence = go;
	return IsSelected(occurrence);
}

bool View::IsSelected(const Occurrence& occurrence) const
// ****************************************************
{
	for_each_selector(selector, GetSelectors()) {
		if (selector->GetOccurrence() == occurrence) return true;
		end_for;
	}
	return false;
}

void View::SetCell(Cell* newCell)
// ******************************
{
	Cell* oldCell = GetCell();
	if (newCell != oldCell) {

		UnselectAll();

		for_each_command(command, GetCommands()) command->UninstallFrom(this); end_for;

		_SetCell(newCell);
		_SetTransformation(Transformation());

		if (_hasBeenExposed) FitToContent();

		_backCenter = _center;
		_backScale = _scale;
	}
}

void View::SetTransformation(const Transformation& newTransformation)
// ******************************************************************
{
	Transformation oldTransformation = GetTransformation();
	if (newTransformation != oldTransformation) {
		_SetTransformation(newTransformation);
		if (GetCell()) FitToContent();
		_backCenter = _center;
		_backScale = _scale;
	}
}

void View::ShowGrid()
// ******************
{
	if (!_gridIsVisible) {
		_gridIsVisible = true;
		if (GridIsDrawable()) Invalidate();
	}
}

void View::HideGrid()
// ******************
{
	if (_gridIsVisible) {
		_gridIsVisible = false;
		if (GridIsDrawable()) Invalidate();
	}
}

void View::SetGridDisplayThreshold(const ScreenUnit& threshold)
// ************************************************************
{
	if (threshold < 3)
		throw Error("Can't set grid display threshold : too small value");

	if (threshold != _gridDisplayThreshold) {
		_gridDisplayThreshold = threshold;
		Invalidate();
	}
}

void View::SetVisibleBasicLayersMask(const Layer::Mask& visibleBasicLayersMask)
// ****************************************************************************
{
	if (_visibleBasicLayersMask != visibleBasicLayersMask) {
		_visibleBasicLayersMask = visibleBasicLayersMask;
		Invalidate();
	}
}

void View::ShowPhantoms()
// **********************
{
	if (!_phantomsAreVisible) {
		_phantomsAreVisible = true;
		Invalidate();
	}
}

void View::HidePhantoms()
// **********************
{
	if (_phantomsAreVisible) {
		_phantomsAreVisible = false;
		Invalidate();
	}
}

void View::ShowBoundaries()
// ************************
{
	if (!_boundariesAreVisible) {
		_boundariesAreVisible = true;
		Invalidate();
	}
}

void View::HideBoundaries()
// ************************
{
	if (_boundariesAreVisible) {
		_boundariesAreVisible = false;
		Invalidate();
	}
}

void View::ShowCutPoints()
// ***********************
{
	if (!_cutPointsAreVisible) {
		_cutPointsAreVisible = true;
		Invalidate();
	}
}

void View::HideCutPoints()
// ***********************
{
	if (_cutPointsAreVisible) {
		_cutPointsAreVisible = false;
		Invalidate();
	}
}

void View::ShowRubbers()
// *********************
{
	if (!_rubbersAreVisible) {
		_rubbersAreVisible = true;
		Invalidate();
	}
}

void View::HideRubbers()
// *********************
{
	if (_rubbersAreVisible) {
		_rubbersAreVisible = false;
		Invalidate();
	}
}

void View::ShowMarkers()
// *********************
{
	if (!_markersAreVisible) {
		_markersAreVisible = true;
		Invalidate();
	}
}

void View::HideMarkers()
// *********************
{
	if (_markersAreVisible) {
		_markersAreVisible = false;
		Invalidate();
	}
}

void View::ShowSelection()
// ***********************
{
	if (!_selectionIsVisible) {
		_selectionIsVisible = true;
		if (HasSomethingSelected()) Invalidate();
	}
}

void View::HideSelection()
// ***********************
{
	if (_selectionIsVisible) {
		_selectionIsVisible = false;
		if (HasSomethingSelected()) Invalidate();
	}
}

void View::ShowDisplaySlots()
// **************************
{
	if (!_displaySlotsAreVisible) {
		_displaySlotsAreVisible = true;
		Invalidate();
	}
}

void View::HideDisplaySlots()
// **************************
{
	if (_displaySlotsAreVisible) {
		_displaySlotsAreVisible = false;
		Invalidate();
	}
}

void View::ShowVisualMaps()
// **************************
{
	if (!_visualMapsAreVisible) {
		_visualMapsAreVisible = true;
		Invalidate();
	}
}

void View::HideVisualMaps()
// **************************
{
	if (_visualMapsAreVisible) {
		_visualMapsAreVisible = false;
		Invalidate();
	}
}

void View::Show(BasicLayer* basicLayer)
// ************************************
{
	if (!basicLayer)
		throw Error("Can't show basic layer : null basic layer");

	if (!IsVisible(basicLayer)) {
		_visibleBasicLayersMask |= basicLayer->GetMask();
		Invalidate();
	}
}

void View::Hide(BasicLayer* basicLayer)
// ************************************
{
	if (!basicLayer)
		throw Error("Can't hide basic layer : null basic layer");

	if (IsVisible(basicLayer)) {
		_visibleBasicLayersMask &= ~basicLayer->GetMask();
		Invalidate();
	}
}

void View::ShowAllLayers()
// ***********************
{
	SetVisibleBasicLayersMask(~0);
}

void View::HideAllLayers()
// ***********************
{
	SetVisibleBasicLayersMask(0);
}

void View::Select(Go* go)
// **********************
{
	if (!go)
		throw Error("Can't select go : null go");

	Occurrence occurrence = go;
	Select(occurrence);
}

void View::Unselect(Go* go)
// ************************
{
	if (!go)
		throw Error("Can't unselect go : null go");

	Occurrence occurrence = go;
	Unselect(occurrence);
}

void View::Select(Occurrence& occurrence)
// ************************************
{
	if (!occurrence.IsValid())
		throw Error("Can't select occurrence : invalid occurrence");

	if (occurrence.GetOwnerCell() != GetCell())
		throw Error("Can't select occurrence : incompatible occurrence");

	Selector* selector = (Selector*)occurrence.GetProperty(Selector::GetPropertyName());
	if (!selector)
		selector = Selector::_Create(occurrence);
	else
		if (!is_a<Selector*>(selector))
			throw Error("Abnormal property named " + GetString(Selector::GetPropertyName()));

	selector->_AttachTo(this);
	Invalidate(GetTransformation().GetBox(occurrence.GetBoundingBox()));
}

void View::Unselect(Occurrence& occurrence)
// **************************************
{
	if (!occurrence.IsValid())
		throw Error("Can't unselect occurrence : invalid occurrence");

	if (occurrence.GetOwnerCell() != GetCell())
		throw Error("Can't unselect occurrence : incompatible occurrence");

	Selector* selector = (Selector*)occurrence.GetProperty(Selector::GetPropertyName());
	if (selector) {
		if (!is_a<Selector*>(selector))
			throw Error("Abnormal property named " + GetString(Selector::GetPropertyName()));

		selector->_DetachFrom(this);
		Invalidate(GetTransformation().GetBox(occurrence.GetBoundingBox()));
	}
}

void View::UnselectAll()
// *********************
{
	Box boundingBox;

	for_each_selector(selector, GetSelectors()) {
		boundingBox.Merge(selector->GetOccurrence().GetBoundingBox());
		selector->_DetachFrom(this);
		end_for;
	}

	if (!boundingBox.IsEmpty())
		Invalidate(GetTransformation().GetBox(boundingBox));
}

void View::Invalidate(const ScreenUnit& screenXo, const ScreenUnit& screenYo, const ScreenUnit& screenXe, const ScreenUnit& screenYe)
// ****************************************************************************************************
{
	if (_drawingArea && _drawingArea->window) {
		ScreenBox screenBox = Box(0, 0, _screenWidth, _screenHeight).Inflate(1);
		ScreenBox updateBox = Box(screenXo, screenYo, screenXe, screenYe).Inflate(1);
		_screenUpdateArea.Merge(screenBox.GetIntersection(updateBox));
	}
}

void View::Invalidate()
// ********************
{
	if (_drawingArea && _drawingArea->window)
		_screenUpdateArea = ScreenBox(0, 0, _screenWidth, _screenHeight);
}

void View::Invalidate(const Box& area)
// ***********************************
{
	if (!area.IsEmpty())
		Invalidate(GetScreenX(area.GetXMin()),
					  GetScreenY(area.GetYMin()),
					  GetScreenX(area.GetXMax()),
					  GetScreenY(area.GetYMax()));
}

bool View::Update(bool useDoubleBuffering)
// ***************************************
{
	if (_screenUpdateArea.IsEmpty()) return false;

	if (!_drawingArea) return false;

	GdkWindow* window = _drawingArea->window;

	if (!window) return false;

	_HideSpot();

	for_each_command(command, GetCommands()) {
		if (!DRAG_VIEW || (DRAG_VIEW == this))
			DrawGhost(command);
		end_for;
	}

	_drawable = window;
	_drawGC = _foregroundGC;
	_fillGC = _foregroundGC;

	_clipBox = Box(GetX(-2), GetY(_screenHeight + 2), GetX(_screenWidth + 2), GetY(-2));

	if (IN_AUTO_SCROLL || useDoubleBuffering || _doubleBufferingIsEnabled)
		_drawable = GetPixmap(_screenWidth, _screenHeight);

	_screenUpdateArea.Inflate(5);

	static GdkCursor* sprayCanCursor = gdk_cursor_new(GDK_SPRAYCAN);
	gdk_window_set_cursor(window, sprayCanCursor);

	_Repaint(GetBox(_screenUpdateArea));

	static GdkCursor* leftPtrCursor = gdk_cursor_new(GDK_LEFT_PTR);
	gdk_window_set_cursor(window, leftPtrCursor);

	if (_drawable != window) {
		if (IN_AUTO_SCROLL) _screenUpdateArea = Box(0, 0, _screenWidth, _screenHeight);
		gdk_window_copy_area
			(window,
			 _foregroundGC,
			 _screenUpdateArea.GetXMin(), _screenUpdateArea.GetYMin(),
			 _drawable,
			 _screenUpdateArea.GetXMin(), _screenUpdateArea.GetYMin(),
			 _screenUpdateArea.GetWidth(), _screenUpdateArea.GetHeight());
	}

	_fillGC = _foregroundGC;
	_drawGC = _foregroundGC;
	_drawable = window;

	_screenUpdateArea = ScreenBox();

	for_each_command(command, GetCommands()) {
		if (!DRAG_VIEW || (DRAG_VIEW == this))
			DrawGhost(command);
		end_for;
	}

	_ShowSpot();

	return true;
}

void View::Refresh(bool useDoubleBuffering)
// ****************************************
{
	Invalidate();
	Update(useDoubleBuffering);
}

void View::CheckForDisplayInterruption()
// *************************************
{
	if (_DisplayHasBeenAborted()) throw Interruption("Display");
}

void View::VerticalScroll(const Unit& dy)
// **************************************
{
	Reframe(Point(_center).Translate(0, -dy));
}

void View::HorizontalScroll(const Unit& dx)
// ****************************************
{
	Reframe(Point(_center).Translate(-dx, 0));
}

void View::Scroll(const Unit& dx, const Unit& dy)
// **********************************************
{
	Reframe(Point(_center).Translate(-dx, -dy));
}

void View::Reframe(const Point& center)
// ************************************
{
	Reframe(center, _scale);
}

void View::Reframe(const Point& center, double scale)
// **************************************************
{
	if ((0 < scale) && ((center != _center) || (scale != _scale))) {
		_backCenter = _center;
		_backScale = _scale;
		_center = center;
		_scale = scale;
		_screenDx = -(int)((GetValue(_center.GetX()) - (_screenWidth / (_scale * 2))) * _scale);
		_screenDy = -(int)((GetValue(_center.GetY()) - (_screenHeight / (_scale * 2))) * _scale);
		Invalidate();
	}
}

void View::Reframe(const Box& area)
// ********************************
{
	if (!area.IsEmpty() && !area.IsFlat())
		Reframe(
			area.GetCenter(),
			min(_screenWidth / GetValue(area.GetWidth()), _screenHeight / GetValue(area.GetHeight())));
}

void View::ReframeBack()
// *********************
{
	// _backCenter is mofified inside the Reframe function
	// so we should give a copy to have a good behaviour
	Reframe(Point(_backCenter), _backScale);
}

void View::FitToContent()
// **********************
{
	Box area = GetClientArea();

	Point center = _center;
	double scale = _scale;

	Reframe(area);
	area.Inflate(GetSize(10));
	Reframe(area);

	_backCenter = center;
	_backScale = scale;

	_hasBeenExposed = true;
}

void View::DrawGhost(Command* command)
// ***********************************
{
	if (command) {
		GdkGC* oldDrawGC = _drawGC;
		GdkGC* oldFillGC = _fillGC;
		_drawGC = _ghostGC;
		_fillGC = _ghostGC;
		command->_OnDrawGhost(this);
		_drawGC = oldDrawGC;
		_fillGC = oldFillGC;
	}
}

void View::DrawPoint(const Unit& x, const Unit& y, const ScreenUnit& screenSize)
// *****************************************************************************
{
	if (!_drawable) return;

	if (x < _clipBox.GetXMin()) return;
	else if (_clipBox.GetXMax() < x) return;
	if (y < _clipBox.GetYMin()) return;
	else if (_clipBox.GetYMax() < y) return;

	ScreenUnit screenX = GetScreenX(x);
	ScreenUnit screenY = GetScreenY(y);

	static gint ANGLE = 360 * 64;
	gdk_draw_arc(_drawable,
					 _drawGC,
					 true,
					 screenX - screenSize,
					 screenY - screenSize,
					 (screenSize * 2),
					 (screenSize * 2),
					 0,
					 ANGLE);
}

void View::DrawPoint(const Point& position, const ScreenUnit& screenSize)
// **********************************************************************
{
	DrawPoint(position.GetX(), position.GetY(), screenSize);
}

void View::DrawLine(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye)
// ********************************************************************************
{
	if (!_drawable) return;

	Unit cXo = xo;
	Unit cYo = yo;
	Unit cXe = xe;
	Unit cYe = ye;

	if (_ClipLine(cXo, cYo, cXe, cYe)) {
		gdk_draw_line(_drawable,
						  _drawGC,
						  GetScreenX(cXo),
						  GetScreenY(cYo),
						  GetScreenX(cXe),
						  GetScreenY(cYe));
	}
}

void View::DrawLine(const Point& origin, const Point& extremity)
// *************************************************************
{
	DrawLine(origin.GetX(), origin.GetY(), extremity.GetX(), extremity.GetY());
}

void View::DrawLines(const vector<Point>& points, unsigned size)
// *************************************************************
{
	// PROVISOIREMENT : doit faire le clipping

	if (!_drawable || !size) return;

	if (SIZE < size) {
		if (POINTS) delete[] POINTS;
		SIZE = size;
		POINTS = new GdkPoint[SIZE];
	}

	for (unsigned i = 0; i < size; i++) {
		POINTS[i].x = GetScreenX(points[i].GetX());
		POINTS[i].y = GetScreenY(points[i].GetY());
	}

	gdk_draw_lines(_drawable, _drawGC, POINTS, size);
}

void View::DrawPolygon(const vector<Point>& points, unsigned size)
// ***************************************************************
{
	// PROVISOIREMENT : doit faire le clipping

	if (!_drawable || !size) return;

	if (SIZE < size) {
		if (POINTS) delete[] POINTS;
		SIZE = size;
		POINTS = new GdkPoint[SIZE];
	}

	for (unsigned i = 0; i < size; i++) {
		POINTS[i].x = GetScreenX(points[i].GetX());
		POINTS[i].y = GetScreenY(points[i].GetY());
	}

	gdk_draw_polygon(_drawable, _drawGC, false, POINTS, size);
}

void View::DrawRectangle(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye)
// *************************************************************************************
{
	DrawRectangle(Box(xo, yo, xe, ye));
}

void View::DrawRectangle(const Point& origin, const Point& extremity)
// ******************************************************************
{
	DrawRectangle(Box(origin, extremity));
}

void View::DrawRectangle(const Box& box)
// *************************************
{
	if (!_drawable) return;

	Box cBox = box;

	if (_ClipBox(cBox)) {
		Box screenBox = GetScreenBox(cBox);
		gdk_draw_rectangle(_drawable,
							 	 _drawGC,
								 false,
								 screenBox.GetXMin(),
								 screenBox.GetYMin(),
								 screenBox.GetWidth(),
								 screenBox.GetHeight());
	}
}

void View::DrawCircle(const Unit& x, const Unit& y, const Unit& radius)
// ********************************************************************
{
	if (!_drawable) return;

	ScreenUnit screenX = GetScreenX(x);
	ScreenUnit screenY = GetScreenY(y);
	ScreenUnit screenRadius = GetScreenSize(radius);

	static gint ANGLE = 360 * 64;
	gdk_draw_arc(_drawable,
					 _drawGC,
					 false,
					 screenX - screenRadius,
					 screenY - screenRadius,
					 (screenRadius * 2),
					 (screenRadius * 2),
					 0,
					 ANGLE);
}

void View::DrawCircle(const Point& center, const Unit& radius)
// ***********************************************************
{
	DrawCircle(center.GetX(), center.GetY(), radius);
}

void View::DrawString(const string& text, const Unit& x, const Unit& y)
// ********************************************************************
{
	if (!_drawable) return;

	Unit X = x;
	Unit Y = y;

    GdkFont * police = gdk_font_load ( "-*-lucidatypewriter-bold-*-*-*-*-120-*-*-*-*-iso8859-1" );

    gint textWidth  = gdk_string_width  ( police, text.c_str() );
    gint textHeight = gdk_string_height ( police, text.c_str() );
    
	gdk_draw_string ( _drawable,
                      police,
				      _drawGC,
				      GetScreenX( X ) + 2,
				      GetScreenY( Y ) + 2,
                      text.c_str()
				    );
    gdk_draw_rectangle ( _drawable,
                         _drawGC,
                         false,
                         GetScreenX( X ),
                         GetScreenY( Y ) - textHeight,
                         textWidth  + 4,
                         textHeight + 4
                       );
}

void View::FillPolygon(const vector<Point>& points, unsigned size, bool solid)
// ***************************************************************************
{
	// PROVISOIREMENT : doit faire le clipping

	if (!_drawable || !size) return;

	if (SIZE < size) {
		if (POINTS) delete[] POINTS;
		SIZE = size;
		POINTS = new GdkPoint[SIZE];
	}

	for (unsigned i = 0; i < size; i++) {
		POINTS[i].x = GetScreenX(points[i].GetX());
		POINTS[i].y = GetScreenY(points[i].GetY());
	}

	gdk_draw_polygon(_drawable, ((solid) ? _drawGC : _fillGC), true, POINTS, size);
}

void View::FillRectangle(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye, bool solid)
// *************************************************************************************************
{
	FillRectangle(Box(xo, yo, xe, ye), solid);
}

void View::FillRectangle(const Point& origin, const Point& extremity, bool solid)
// *****************************************************************************
{
	FillRectangle(Box(origin, extremity), solid);
}

void View::FillRectangle(const Box& box, bool solid)
// *************************************************
{
	if (!_drawable) return;

	Box cBox = box;

	if (_ClipBox(cBox)) {
		Box screenBox = GetScreenBox(cBox);
		gdk_draw_rectangle(_drawable,
							 	 (solid) ? _drawGC : _fillGC,
								 true,
								 screenBox.GetXMin(),
								 screenBox.GetYMin(),
								 screenBox.GetWidth() + 1,
								 screenBox.GetHeight() + 1);
	}
}

void View::FillCircle(const Unit& x, const Unit& y, const Unit& radius, bool solid)
// ********************************************************************************
{
	if (!_drawable) return;

	ScreenUnit screenX = GetScreenX(x);
	ScreenUnit screenY = GetScreenY(y);
	ScreenUnit screenRadius = GetScreenSize(radius);

	static gint ANGLE = 360 * 64;
	gdk_draw_arc(_drawable,
					 (solid) ? _drawGC : _fillGC,
					 true,
					 screenX - screenRadius,
					 screenY - screenRadius,
					 (screenRadius * 2),
					 (screenRadius * 2),
					 0,
					 ANGLE);
}

void View::FillCircle(const Point& center, const Unit& radius, bool solid)
// ***********************************************************************
{
	FillCircle(center.GetX(), center.GetY(), radius, solid);
}

void View::_PostCreate()
// *********************
{
	_frame = gtk_table_new(1, 1, false);
	gtk_container_set_border_width(GTK_CONTAINER(_frame), 2);

	gtk_signal_connect(GTK_OBJECT(_frame), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &_frame);

	_drawingArea = gtk_drawing_area_new();
	gtk_table_attach
		(GTK_TABLE(_frame), _drawingArea, 0, 1, 0, 1, GTK_FILL_EXPAND, GTK_FILL_EXPAND, 0, 0);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &_drawingArea);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "expose_event", GTK_SIGNAL_FUNC(OnExpose), this);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "configure_event", GTK_SIGNAL_FUNC(OnConfigure), this);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "enter_notify_event", GTK_SIGNAL_FUNC(OnEnterNotify), this);
	gtk_widget_add_events(GTK_WIDGET(_drawingArea), GDK_ENTER_NOTIFY_MASK);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "motion_notify_event", GTK_SIGNAL_FUNC(OnMotionNotify), this);
	gtk_widget_add_events
		(GTK_WIDGET(_drawingArea), GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "leave_notify_event", GTK_SIGNAL_FUNC(OnLeaveNotify), this);
	gtk_widget_add_events(GTK_WIDGET(_drawingArea), GDK_LEAVE_NOTIFY_MASK);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "button_press_event", GTK_SIGNAL_FUNC(OnButtonPress), this);
	gtk_widget_add_events(GTK_WIDGET(_drawingArea), GDK_BUTTON_PRESS_MASK);

	gtk_signal_connect
		(GTK_OBJECT(_drawingArea), "button_release_event", GTK_SIGNAL_FUNC(OnButtonRelease), this);
	gtk_widget_add_events(GTK_WIDGET(_drawingArea), GDK_BUTTON_RELEASE_MASK);

	_backgroundGC = gtk_gc_new(&_backgroundColor);
	_foregroundGC = gtk_gc_new(&_foregroundColor);
	_phantomGC = gtk_gc_new(&_phantomColor);
	_boundaryGC = gtk_gc_new(&_boundaryColor);
	_rubberGC = gtk_gc_new(&_rubberColor);
	_markerGC = gtk_gc_new(&_markerColor);
	gdk_gc_set_line_attributes (_markerGC, 1, GDK_LINE_SOLID, GDK_CAP_PROJECTING, GDK_JOIN_MITER);
	_selectionDrawGC = gtk_gc_new(&_selectionDrawColor);
	_selectionFillGC = gtk_gc_new(&_selectionFillColor, "AA55AA55AA55AA55");
	_gridGC = gtk_gc_new(&_gridColor);
	_spotGC = gtk_gc_new(&_spotColor);
	gdk_gc_set_function(_spotGC, GDK_XOR);
	_ghostGC = gtk_gc_new(&_ghostColor);
	gdk_gc_set_function(_ghostGC, GDK_XOR);

	_drawGC = _foregroundGC;
	_fillGC = _foregroundGC;

	Inherit::_PostCreate();
}

void View::_PreDelete()
// ********************
{
	Inherit::_PreDelete();

	for_each_selector(selector, GetSelectors()) selector->_DetachFrom(this); end_for;

	for_each_command(command, GetCommands()) command->UninstallFrom(this); end_for;

	gdk_gc_destroy(_backgroundGC);
	gdk_gc_destroy(_foregroundGC);
	gdk_gc_destroy(_phantomGC);
	gdk_gc_destroy(_boundaryGC);
	gdk_gc_destroy(_rubberGC);
	gdk_gc_destroy(_markerGC);
	gdk_gc_destroy(_selectionDrawGC);
	gdk_gc_destroy(_selectionFillGC);
	gdk_gc_destroy(_gridGC);
	gdk_gc_destroy(_spotGC);
	gdk_gc_destroy(_ghostGC);

	if (_frame) gtk_widget_destroy(_frame);

	Cell* cell = GetCell();
	if (cell) cell->_GetViewSet()._Remove(this);
}

string View::_GetString() const
// ****************************
{
	string s = Inherit::_GetString();
	Cell* cell = GetCell();
	if (cell) s.insert(s.length() - 1, " " + GetString(cell->GetName()));
	return s;
}

Record* View::_GetRecord() const
// ***********************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Cell", GetCell()));
		record->Add(GetSlot("Transformation",GetTransformation()));
		record->Add(GetSlot("Center", &_center));
		record->Add(GetSlot("Scale", &_scale));
		record->Add(GetSlot("ScreenWidth", &_screenWidth));
		record->Add(GetSlot("ScreenHeight", &_screenHeight));
		record->Add(GetSlot("Commands", &_commandMap));
		record->Add(GetSlot("GridIsVisible", &_gridIsVisible));
		record->Add(GetSlot("GridDisplayThreshold", &_gridDisplayThreshold));
		record->Add(GetSlot("AutoScrollIsEnabled", &_autoScrollIsEnabled));
		record->Add(GetSlot("DoubleBufferingIsEnabled", &_doubleBufferingIsEnabled));
		record->Add(GetSlot("PhantomsAreVisible", &_phantomsAreVisible));
		record->Add(GetSlot("BoundariesAreVisible", &_boundariesAreVisible));
		record->Add(GetSlot("CutPointsAreVisible", &_cutPointsAreVisible));
		record->Add(GetSlot("RubbersAreVisible", &_rubbersAreVisible));
		record->Add(GetSlot("MarkersAreVisible", &_markersAreVisible));
		record->Add(GetSlot("SelectionIsVisible", &_selectionIsVisible));
		record->Add(GetSlot("DisplaySlotsAreVisible", &_displaySlotsAreVisible));
		record->Add(GetSlot("VisualMapsAreVisible", &_visualMapsAreVisible));
		record->Add(GetSlot("VisibleBasicLayersMask", &_visibleBasicLayersMask));
		record->Add(GetSlot("Selection", &_selectorSet));
		record->Add(GetSlot("RubberDisplayType", &_rubberDisplayType));
	}
	return record;
}

bool View::_DisplayHasBeenAborted()
// ********************************
{
	bool aborted = false;
	if (gdk_events_pending()) {
		GdkEvent* event = gdk_event_get();
		if (event) {
			if (event->any.window == _drawingArea->window) {
				if ((event->type == GDK_CONFIGURE) ||
					 (event->type == GDK_BUTTON_PRESS) ||
					 (event->type == GDK_2BUTTON_PRESS) ||
					 (event->type == GDK_3BUTTON_PRESS) ||
					 (event->type == GDK_KEY_PRESS))
					aborted = true;
			}
			if (!aborted) aborted = _DisplayHasBeenAborted();
			gdk_event_put(event);
			gdk_event_free(event);
		}
	}
	return aborted;
}

void View::_PreRepaint()
// *********************
{
}

void View::_PostRepaint()
// **********************
{
}

void View::_DrawDisplaySlot(DisplaySlot* displaySlot, const Box& area, const Box& updateArea, const Transformation& transformation)
// ********************************************************************************************************************************
{
	GdkRectangle clipRectangle;
	clipRectangle.x = GetScreenX(area.GetXMin());
	clipRectangle.y = GetScreenY(area.GetYMax());
	clipRectangle.width = GetScreenX(area.GetXMax()) - clipRectangle.x;
	clipRectangle.height = GetScreenY(area.GetYMin()) - clipRectangle.y;

    if (displaySlot->IsVisible()) {
        _drawGC = displaySlot->_GetDrawGC();
        _fillGC = displaySlot->_GetFillGC();
        gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
        gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
        displaySlot->_Draw(this, updateArea, transformation);
        gdk_flush();
    }
}

void View::_Repaint(const Box& area)
// *********************************
{
	GdkRectangle clipRectangle;
	clipRectangle.x = GetScreenX(area.GetXMin());
	clipRectangle.y = GetScreenY(area.GetYMax());
	clipRectangle.width = GetScreenX(area.GetXMax()) - clipRectangle.x;
	clipRectangle.height = GetScreenY(area.GetYMin()) - clipRectangle.y;

	Box updateArea = GetTransformation().GetInvert().GetBox(area);

	_drawGC = _backgroundGC;
	_fillGC = _backgroundGC;
	gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
	gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
	FillRectangle(area, true);

	_PreRepaint();

	Cell* cell = GetCell();
	if (cell) {

		if (_phantomsAreVisible) {
			_drawGC = _phantomGC;
			_fillGC = _phantomGC;
			gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
			gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
			cell->_DrawPhantoms(this, updateArea, GetTransformation());
			gdk_flush();
		}

		if (_boundariesAreVisible) {
			_drawGC = _boundaryGC;
			_fillGC = _boundaryGC;
			gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
			gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
			cell->_DrawBoundaries(this, updateArea, GetTransformation());
			gdk_flush();
		}

		_drawGC = _foregroundGC;
		_fillGC = _foregroundGC;
		gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
		gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);

		DataBase* dataBase = GetDataBase();
		Technology* technology = dataBase->GetTechnology();
		for_each_composite_layer(compositeLayer, technology->GetCompositeLayers()) {
			compositeLayer->_UpdateSymbolicBasicLayer(_visibleBasicLayersMask);
			end_for;
		}

		try {
			for_each_basic_layer(basicLayer, GetVisibleBasicLayers()) {
				CheckForDisplayInterruption();
				if (basicLayer->GetDisplayThreshold() < _scale) {
					_drawGC = basicLayer->_GetDrawGC();
					_fillGC = basicLayer->_GetFillGC();
					gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
					gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
					cell->_DrawContent(this, basicLayer, updateArea, GetTransformation());
					gdk_flush();
					_drawGC = _foregroundGC;
					_fillGC = _foregroundGC;
				}
				end_for;
			}
		}
		catch (Interruption& interruption) {
		}

		for_each_composite_layer(compositeLayer, technology->GetCompositeLayers()) {
			compositeLayer->_UpdateSymbolicBasicLayer(~0);
			end_for;
		}

		if (_rubbersAreVisible) {
			_drawGC = _rubberGC;
			_fillGC = _rubberGC;
			gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
			gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
			cell->_DrawRubbers(this, updateArea, GetTransformation());
			gdk_flush();
		}

		if (_markersAreVisible) {
			_drawGC = _markerGC;
			_fillGC = _markerGC;
			gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
			gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
			cell->_DrawMarkers(this, updateArea, GetTransformation());
			gdk_flush();
		}

		if (_displaySlotsAreVisible) {
			cell->_DrawDisplaySlots(this, area, updateArea, GetTransformation());
		}

		if (_visualMapsAreVisible) {
			for_each_visual_map(visualMap, GetVisualMaps(cell)) {
				if (visualMap->IsVisible()) {
#if 0
					_drawGC = visualMap->_GetGC();
					_fillGC = visualMap->_GetGC();
					gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
					gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
#endif
					visualMap->_Draw(this, updateArea, GetTransformation());
					gdk_flush();
				}
				end_for;
			}
		}

		bool cutPointsAreVisible = _cutPointsAreVisible;
		_cutPointsAreVisible = true;
		if (_selectionIsVisible) {
			_drawGC = _selectionDrawGC;
			_fillGC = _selectionFillGC;
			gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
			gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
			for_each_selector(selector, GetSelectors()) {
				Occurrence occurrence = selector->GetOccurrence();
				Entity* entity = occurrence.GetEntity();
				Transformation transformation = occurrence.GetPath().GetTransformation();
				GetTransformation().ApplyOn(transformation);
				if (is_a<Go*>(entity))
					((Go*)entity)->_Highlight(this, updateArea, transformation);
				end_for;
			}
			gdk_flush();
		}
		_cutPointsAreVisible = cutPointsAreVisible;
	}

	if (GridIsVisible() && GridIsDrawable()) {
		Unit xmin = GetOnGridUnit(GetX(0));
		Unit ymin = GetOnGridUnit(GetY(_screenHeight));
		Unit xmax = GetOnGridUnit(GetX(_screenWidth));
		Unit ymax = GetOnGridUnit(GetY(0));
		for (Unit xi = xmin; xi <= xmax; xi += GetGridStep()) {
			bool bx = IsOnGrid(xi, 10);
			for (Unit yi = ymin; yi <= ymax; yi += GetGridStep()) {
				bool by = IsOnGrid(yi, 10);
				ScreenUnit screenX = GetScreenX(xi);
				ScreenUnit screenY = GetScreenY(yi);
				if (!bx || !by)
					gdk_draw_point(_drawable, _gridGC, screenX, screenY);
				else {
					gdk_draw_line(_drawable, _gridGC, screenX - 2, screenY, screenX + 2, screenY);
					gdk_draw_line(_drawable, _gridGC, screenX, screenY - 2, screenX, screenY + 2);
				}
			}
		}
	}

	_PostRepaint();

	clipRectangle.x = 0;
	clipRectangle.y = 0;
	clipRectangle.width = _screenWidth;
	clipRectangle.height = _screenHeight;
	gdk_gc_set_clip_rectangle(_drawGC, &clipRectangle);
	gdk_gc_set_clip_rectangle(_fillGC, &clipRectangle);
}

void View::_DrawSpot()
// *******************
{
	if (!_drawingArea || !_drawingArea->window) return;

	if (_spotIsDrawable) {
		ScreenUnit screenX = GetScreenX(_spotPosition.GetX());
		ScreenUnit screenY = GetScreenY(_spotPosition.GetY());
		if (((-3 <= screenX) && (screenX <= (_screenWidth + 3))) &&
		 	 ((-3 <= screenY) && (screenY <= (_screenHeight + 3))))
			gdk_draw_rectangle(_drawable, _spotGC, false, screenX - 3, screenY - 3, 6, 6);
		_spotIsVisible = !_spotIsVisible;
	}
}

void View::_ShowSpot()
// *******************
{
	if (!_spotIsVisible) _DrawSpot();
}

void View::_HideSpot()
// *******************
{
	if (_spotIsVisible) _DrawSpot();
}

int View::_GetClipCode(const Unit& x, const Unit& y) const
// *******************************************************
{
	int code = 0;
	if (x < _clipBox.GetXMin()) code = 1;
	else if (_clipBox.GetXMax() < x) code = 2;
	if (y < _clipBox.GetYMin()) code |= 4;
	else if (_clipBox.GetYMax() < y) code |= 8;
	return code;
}

bool View::_ClipLine(Unit& xo, Unit& yo, Unit& xe, Unit& ye) const
// ***************************************************************
{
	int cco = _GetClipCode(xo, yo);
	int cce = _GetClipCode(xe, ye);

	if (!(cco | cce)) return true;

	int c = cco & cce;
	if ((c & 1) || (c & 2) || (c & 4) || (c & 8)) return false;

	if (cco & 1) {
		yo += GetUnit((GetValue(ye - yo) * GetValue(_clipBox.GetXMin() - xo)) / GetValue(xe - xo));
		xo = _clipBox.GetXMin();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cco & 2) {
		yo += GetUnit((GetValue(ye - yo) * GetValue(_clipBox.GetXMax() - xo)) / GetValue(xe - xo));
		xo = _clipBox.GetXMax();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cce & 1) {
		ye += GetUnit((GetValue(yo - ye) * GetValue(_clipBox.GetXMin() - xe)) / GetValue(xo - xe));
		xe = _clipBox.GetXMin();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cce & 2) {
		ye += GetUnit((GetValue(yo - ye) * GetValue(_clipBox.GetXMax() - xe)) / GetValue(xo - xe));
		xe = _clipBox.GetXMax();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cco & 4) {
		xo += GetUnit((GetValue(xe - xo) * GetValue(_clipBox.GetYMin() - yo)) / GetValue(ye - yo));
		yo = _clipBox.GetYMin();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cco & 8) {
		xo += GetUnit((GetValue(xe - xo) * GetValue(_clipBox.GetYMax() - yo)) / GetValue(ye - yo));
		yo = _clipBox.GetYMax();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cce & 4) {
		xe += GetUnit((GetValue(xo - xe) * GetValue(_clipBox.GetYMin() - ye)) / GetValue(yo - ye));
		ye = _clipBox.GetYMin();
		return _ClipLine(xo, yo, xe, ye);
	}
	if (cce & 8) {
		xe += GetUnit((GetValue(xo - xe) * GetValue(_clipBox.GetYMax() - ye)) / GetValue(yo - ye));
		ye = _clipBox.GetYMax();
		return _ClipLine(xo, yo, xe, ye);
	}
	return true;
}

bool View::_ClipBox(Box& box) const
// ********************************
{
	box = box.GetIntersection(_clipBox);
	return !box.IsEmpty();
}

void View::_OnExpose(const ScreenUnit& screenX, const ScreenUnit& screenY, const ScreenUnit& screenWidth, const ScreenUnit& screenHeight, bool update)
// ****************************************************************************************************
{
	if (!_drawingArea) return;

	if (!_hasBeenExposed) {
		GdkWindow* window = _drawingArea->window;
		GdkColormap* colormap = gdk_window_get_colormap(window);
		GdkColor color;
		color.red = 50 * 255;
		color.green = 50 * 255;
		color.blue = 50 * 255;
		gdk_color_alloc(colormap, &color);
		gdk_window_set_background(window, &color);
		FitToContent();
		_backCenter = _center;
		_backScale = _scale;
	}

	Invalidate(screenX, screenY, screenX + screenWidth, screenY + screenHeight);

	if (update) Update();
}

void View::_OnResize(const ScreenUnit& screenWidth, const ScreenUnit& screenHeight)
// ********************************************************************************
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	if (_drawingArea && _drawingArea->window) {
		Point backCenter = _backCenter;
		double backScale = _backScale;
		double scale = _scale;
		_scale = 0; // to force reframe
		Reframe(_center, scale);
		// to restore good values altered by the hack
		_backCenter = backCenter;
		_backScale = backScale;
	}
}

void View::_OnMouseEnter(const Point& position)
// ********************************************
{
	for_each_command(command, GetCommands()) {
		command->_OnMouseEnter(this, position);
		end_for;
	}
	_spotIsDrawable = true;
	_spotPosition = position;
	_ShowSpot();
}

void View::_OnMouseMove(const Point& position, unsigned state)
// ***********************************************************
{
	_HideSpot();
	for_each_command(command, GetCommands()) {
		command->_OnMouseMove(this, position, state);
		end_for;
	}
	_spotPosition = position;
	_ShowSpot();
}

void View::_OnMouseLeave(const Point& position)
// ********************************************
{
	_HideSpot();
	_spotIsDrawable = false;
	for_each_command(command, GetCommands()) {
		command->_OnMouseLeave(this, position);
		end_for;
	}
}

void View::_OnButtonPress(unsigned button, const Point& position, unsigned state)
// ******************************************************************************
{
	Command* command = GetCommand(button);
	if (command) {
		_HideSpot();
		command->_OnButtonPress(this, position, state);
		_ShowSpot();
	}
}

void View::_OnButtonRelease(unsigned button, const Point& position, unsigned state)
// ********************************************************************************
{
	Command* command = GetCommand(button);
	if (command) {
		_HideSpot();
		command->_OnButtonRelease(this, position, state);
		_ShowSpot();
	}
}

void View::SetDefaultColor (const string& name, unsigned short red, unsigned short green, unsigned short blue)
// ***********************************************************************************************************
{
  GdkColor* color = NULL;
  if      (name == "background"   ) { color = &_backgroundColor; }
  else if (name == "foreground"   ) { color = &_foregroundColor; }
  else if (name == "rubber"       ) { color = &_rubberColor; }
  else if (name == "phantom"      ) { color = &_phantomColor; }
  else if (name == "boundary"     ) { color = &_boundaryColor; }
  else if (name == "marker"       ) { color = &_markerColor; }
  else if (name == "selectionDraw") { color = &_selectionDrawColor; }
  else if (name == "selectionFill") { color = &_selectionFillColor; }
  else if (name == "grid"         ) { color = &_gridColor; }
  else if (name == "spot"         ) { color = &_spotColor; }
  else if (name == "ghost"        ) { color = &_ghostColor; }
  else {
    throw Error ("Invalid View color name : " + name);
  }

  color->red   = red   * 255;
  color->green = green * 255;
  color->blue  = blue  * 255;
}

void View::SetColor (const string& name, unsigned short red, unsigned short green, unsigned short blue)
// ****************************************************************************************************
{
  GdkColor* color = gdk_color_new(red, green, blue);
  GdkGC*    gc;

  if      (name == "background"   ) { gc = _backgroundGC; }
  else if (name == "foreground"   ) { gc = _foregroundGC; }
  else if (name == "rubber"       ) { gc = _rubberGC; }
  else if (name == "phantom"      ) { gc = _phantomGC; }
  else if (name == "boundary"     ) { gc = _boundaryGC; }
  else if (name == "marker"       ) { gc = _markerGC; }
  else if (name == "selectionDraw") { gc = _selectionDrawGC; }
  else if (name == "selectionFill") { gc = _selectionFillGC; }
  else if (name == "grid"         ) { gc = _gridGC; }
  else if (name == "spot"         ) { gc = _spotGC; }
  else if (name == "ghost"        ) { gc = _ghostGC; }
  else {
    throw Error ("Invalid View color name : " + name);
  }

  gdk_gc_set_foreground(gc, color);
}

void View::SetRubberDisplayType(const RubberDisplayType& rubberdisplaytype)
// ************************************************************************
{
    _rubberDisplayType = rubberdisplaytype;
}

void View::SetTextDisplayState(const bool& isTextVisible)
// ************************************************************************
{
    _isTextVisible = isTextVisible;
}

// ****************************************************************************************************
// View::RubberDisplayType implementation
// ****************************************************************************************************

View::RubberDisplayType::RubberDisplayType(const Type& type)
// *********************************************************
:	_type(type)
{
}

View::RubberDisplayType::RubberDisplayType(const RubberDisplayType& rubberdisplaytype)
// ***********************************************************************************
:	_type(rubberdisplaytype._type)
{
}

View::RubberDisplayType& View::RubberDisplayType::operator=(const RubberDisplayType& rubberdisplaytype)
// ****************************************************************************************************
{
	_type = rubberdisplaytype._type;
	return *this;
}

string View::RubberDisplayType::_GetString() const
// ***********************************************
{
	switch (_type) {
		case GEOMETRIC : return "GEOMETRIC";
		case BARYCENTRIC : return "BARYCENTRIC";
		case STEINER: return "STEINER";
	}
	return "ABNORMAL";
}

Record* View::RubberDisplayType::_GetRecord() const
// ******************************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Type", ((unsigned int*)((void*)&_type))));
	return record;
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
