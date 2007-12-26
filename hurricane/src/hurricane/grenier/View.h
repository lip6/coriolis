// ****************************************************************************************************
// File: View.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_VIEW
#define HURRICANE_VIEW

#include "DBo.h"
#include "Views.h"
#include "Transformation.h"
#include "BasicLayer.h"
#include "BasicLayers.h"
#include "Selector.h"
#include "Command.h"
#include "Occurrence.h"

namespace Hurricane {

class Cell;
class Go;
class DisplaySlot;



// ****************************************************************************************************
// Special declaration
// ****************************************************************************************************

typedef Unit ScreenUnit;
typedef Point ScreenPoint;
typedef Box ScreenBox;



// ****************************************************************************************************
// View declaration
// ****************************************************************************************************

class View : public DBo {
// ********************

// Types
// *****

        public: typedef DBo Inherit;

        public: typedef map<unsigned, Command*> CommandMap;
        public: typedef set<Selector*> SelectorSet;

        public: class RubberDisplayType {
        // ****************************

                public: enum Type {GEOMETRIC=0, BARYCENTRIC=1, STEINER=2};

                private: Type _type;

                public: RubberDisplayType(const Type& type = GEOMETRIC);
                public: RubberDisplayType(const RubberDisplayType& rubberdisplaytype);

                public: RubberDisplayType& operator=(const RubberDisplayType& rubberdisplaytype);

                public: operator const Type&() const {return _type;};

                public: const Type& GetType() const {return _type;};

                public: string _GetTypeName() const { return _TName("View::RubberDisplayType"); };
                public: string _GetString() const;
                public: Record* _GetRecord() const;

        };

// Attributes
// **********

        //private: GtkWidget* _frame;
        //private: GtkWidget* _drawingArea;

        private: Point _center;
        private: double _scale;
        private: Point _backCenter;
        private: double _backScale;

        private: ScreenUnit _screenDx;
        private: ScreenUnit _screenDy;
        private: ScreenUnit _screenWidth;
        private: ScreenUnit _screenHeight;

        private: bool _hasBeenExposed;

        private: Box _screenUpdateArea;

        private: Box _clipBox;

        //private: GdkDrawable* _drawable;

        // Default color settings.
        private: static GdkColor  _backgroundColor;
        private: static GdkColor  _foregroundColor;
        private: static GdkColor  _rubberColor;
        private: static GdkColor  _phantomColor;
        private: static GdkColor  _boundaryColor;
        private: static GdkColor  _markerColor;
        private: static GdkColor  _selectionDrawColor;
        private: static GdkColor  _selectionFillColor;
        private: static GdkColor  _gridColor;
        private: static GdkColor  _spotColor;
        private: static GdkColor  _ghostColor;

        private: GdkGC* _backgroundGC;
        private: GdkGC* _foregroundGC;
        private: GdkGC* _phantomGC;
        private: GdkGC* _boundaryGC;
        private: GdkGC* _rubberGC;
        private: GdkGC* _markerGC;
        private: GdkGC* _selectionDrawGC;
        private: GdkGC* _selectionFillGC;
        private: GdkGC* _gridGC;
        private: GdkGC* _spotGC;
        private: GdkGC* _ghostGC;
        private: GdkGC* _drawGC;
        private: GdkGC* _fillGC;

        private: CommandMap _commandMap;

        private: bool _gridIsVisible;
        private: ScreenUnit _gridDisplayThreshold;

        private: bool _autoScrollIsEnabled;
        private: bool _doubleBufferingIsEnabled;
        private: bool _phantomsAreVisible;
        private: bool _boundariesAreVisible;
        private: bool _rubbersAreVisible;
        private: bool _markersAreVisible;
        private: bool _cutPointsAreVisible;
        private: bool _selectionIsVisible;
        private: bool _displaySlotsAreVisible;
        private: bool _visualMapsAreVisible;
        private: bool _spotIsVisible;
        private: bool _spotIsDrawable;
        private: Point _spotPosition;

        private: Layer::Mask _visibleBasicLayersMask;

        private: SelectorSet _selectorSet;
                 
        private: RubberDisplayType _rubberDisplayType;
        private: bool _isTextVisible;

        private: View* _nextOfCellViewSet;

// Constructors
// ************

        protected: View();

// Accessors
// *********

        public: GtkWidget* GetFrame() const {return _frame;};
        public: GtkWidget* GetDrawingArea() const {return _drawingArea;};
        public: virtual Cell* GetCell() const = 0;
        public: virtual Transformation GetTransformation() const = 0;

        public: const Point& GetCenter() const {return _center;};
        public: double GetScale() const {return _scale;};
        public: const Point& GetBackCenter() const {return _backCenter;};
        public: double GetBackScale() const {return _backScale;};

        public: const ScreenUnit& GetScreenWidth() const {return _screenWidth;};
        public: const ScreenUnit& GetScreenHeight() const {return _screenHeight;};

        public: ScreenUnit GetHalfScreenWidth() const {return _screenWidth / 2;};
        public: ScreenUnit GetHalfScreenHeight() const {return _screenHeight / 2;};

        public: Unit GetX(const ScreenUnit& screenX) const;
        public: Unit GetY(const ScreenUnit& screenY) const;
        public: Unit GetSize(const ScreenUnit& screenSize) const;
        public: Point GetPoint(const ScreenPoint& screenPoint) const;
        public: Box GetBox(const ScreenBox& screenBox) const;

        public: ScreenUnit GetScreenX(const Unit& x) const;
        public: ScreenUnit GetScreenY(const Unit& y) const;
        public: ScreenUnit GetScreenSize(const Unit& size) const;
        public: ScreenPoint GetScreenPoint(const Point& point) const;
        public: ScreenBox GetScreenBox(const Box& box) const;

        public: virtual Box GetClientArea() const;
        public: Box GetVisibleArea() const;

        public: Command* GetCommand(unsigned button) const;
        public: Commands GetCommands() const {return GetCollection(_commandMap);};

        public: const ScreenUnit& GetGridDisplayThreshold() const {return _gridDisplayThreshold;};

        public: const Layer::Mask& GetVisibleBasicLayersMask() const {return _visibleBasicLayersMask;};
        public: BasicLayers GetVisibleBasicLayers() const;

        public: Selectors GetSelectors() const {return GetCollection(_selectorSet);};
                
		public: const RubberDisplayType& GetRubberDisplayType() const {return _rubberDisplayType;};

// Predicates
// **********

        public: bool GridIsVisible() const {return _gridIsVisible;};
        public: bool GridIsDrawable() const {return (_gridDisplayThreshold < GetScreenSize(GetGridStep()));};
        public: bool AutoScrollIsEnabled() const {return _autoScrollIsEnabled;};
        public: bool DoubleBufferingIsEnabled() const {return _doubleBufferingIsEnabled;};
        public: bool PhantomsAreVisible() const {return _phantomsAreVisible;};
        public: bool BoundariesAreVisible() const {return _boundariesAreVisible;};
        public: bool CutPointsAreVisible() const {return _cutPointsAreVisible;};
        public: bool RubbersAreVisible() const {return _rubbersAreVisible;};
        public: bool MarkersAreVisible() const {return _markersAreVisible;};
        public: bool SelectionIsVisible() const {return _selectionIsVisible;};
        public: bool DisplaySlotsAreVisible() const {return _displaySlotsAreVisible;};
        public: bool IsVisible(BasicLayer* basicLayer) const;
        public: bool IsTextVisible() const {return _isTextVisible;};

        public: bool IsSelected(Go* go) const;
        public: bool IsSelected(const Occurrence& occurrence) const;
        public: bool HasSomethingSelected() const {return !_selectorSet.empty();};

// Updators
// ********

        public: void SetCell(Cell* cell);
        public: void SetTransformation(const Transformation& transformation);
        public: void ShowGrid();
        public: void HideGrid();
        public: void SetGridDisplayThreshold(const ScreenUnit& threshold);
        public: void EnableAutoScroll() {_autoScrollIsEnabled = true;};
        public: void DisableAutoScroll() {_autoScrollIsEnabled = false;};
        public: void EnableDoubleBuffering() {_doubleBufferingIsEnabled = true;};
        public: void DisableDoubleBuffering() {_doubleBufferingIsEnabled = false;};
        public: void SetVisibleBasicLayersMask(const Layer::Mask& visibleBasicLayersMask);
        public: void ShowPhantoms();
        public: void HidePhantoms();
        public: void ShowBoundaries();
        public: void HideBoundaries();
        public: void ShowCutPoints();
        public: void HideCutPoints();
        public: void ShowRubbers();
        public: void HideRubbers();
        public: void ShowMarkers();
        public: void HideMarkers();
        public: void ShowSelection();
        public: void HideSelection();
        public: void ShowDisplaySlots();
        public: void HideDisplaySlots();
        public: void ShowVisualMaps();
        public: void HideVisualMaps();
        public: void Show(BasicLayer* basicLayer);
        public: void Hide(BasicLayer* basicLayer);
        public: void ShowAllLayers();
        public: void HideAllLayers();
        public: void SetRubberDisplayType(const RubberDisplayType& rubberdisplaytype);
        public: void SetTextDisplayState(const bool& isTextVisible);

        public: void Select(Go* go);
        public: void Unselect(Go* go);
        public: void Select(Occurrence& occurrence);
        public: void Unselect(Occurrence& occurrence);
        public: void UnselectAll();

// Managers
// ********

        public: void Invalidate(const ScreenUnit& screenXo, const ScreenUnit& screenYo, const ScreenUnit& screenXe, const ScreenUnit& screenYe);

        public: void Invalidate();
        public: void Invalidate(const Box& area);

        public: virtual bool Update(bool useDoubleBuffering = false);
        public: void Refresh(bool useDoubleBuffering = false);
        public: void CheckForDisplayInterruption();

        public: void VerticalScroll(const Unit& dy);
        public: void HorizontalScroll(const Unit& dx);
        public: void Scroll(const Unit& dx, const Unit& dy);

        public: void Reframe(const Point& center);
        public: void Reframe(const Point& center, double scale);
        public: void Reframe(const Box& area);
        public: void ReframeBack();
        public: void FitToContent();

        public: void DrawGhost(Command* command);

// Display primitives
// ******************

        public: void SetGCs(GdkGC* drawGC, GdkGC* fillGC);

        public: static void SetDefaultColor(const string& name, unsigned short red, unsigned short green, unsigned short blue);
        public: void SetColor(const string& name, unsigned short red, unsigned short green, unsigned short blue);

        public: void DrawPoint(const Unit& x, const Unit& y, const ScreenUnit& screenSize);
        public: void DrawPoint(const Point& position, const ScreenUnit& screenSize);

        public: void DrawLine(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye);
        public: void DrawLine(const Point& origin, const Point& extremity);

        public: void DrawLines(const vector<Point>& points, unsigned size);
        public: void DrawPolygon(const vector<Point>& points, unsigned size);

        public: void DrawRectangle(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye);
        public: void DrawRectangle(const Point& origin, const Point& extremity);
        public: void DrawRectangle(const Box& box);

        public: void DrawCircle(const Unit& x, const Unit& y, const Unit& radius);
        public: void DrawCircle(const Point& center, const Unit& radius);
            
        public: void DrawString(const string& text, const Unit& x, const Unit& y);

        public: void _DrawDisplaySlot(DisplaySlot* displaySlot, const Box& ares, const Box& updateArea, const Transformation& transformation);

        public: void FillPolygon(const vector<Point>& points, unsigned size, bool solid = false);

        public: void FillRectangle(const Unit& xo, const Unit& yo, const Unit& xe, const Unit& ye, bool solid = false);
        public: void FillRectangle(const Point& origin, const Point& extremity, bool solid = false);
        public: void FillRectangle(const Box& box, bool solid = false);

        public: void FillCircle(const Unit& x, const Unit& y, const Unit& radius, bool solid = false);
        public: void FillCircle(const Point& center, const Unit& radius, bool solid = false);

// Others
// ******

        protected: virtual void _PostCreate();

        protected: virtual void _PreDelete();

        public: virtual string _GetString() const;
        public: virtual Record* _GetRecord() const;
        public: GdkGC* _GetBackgroundGC() const {return _backgroundGC;};
        public: GdkGC* _GetForegroundGC() const {return _foregroundGC;};
        public: GdkGC* _GetPhantomGC() const {return _phantomGC;};
        public: GdkGC* _GetBoundaryGC() const {return _boundaryGC;};
        public: GdkGC* _GetRubberGC() const {return _rubberGC;};
        public: GdkGC* _GetMarkerGC() const {return _markerGC;};
        public: GdkGC* _GetSelectionDrawGC() const {return _selectionDrawGC;};
        public: GdkGC* _GetSelectionFillGC() const {return _selectionFillGC;};
        public: GdkGC* _GetGridGC() const {return _gridGC;};
        public: GdkGC* _GetGhostGC() const {return _ghostGC;};
        public: CommandMap& _GetCommandMap() {return _commandMap;};
        public: SelectorSet& _GetSelectorSet() {return _selectorSet;};
        public: View* _GetNextOfCellViewSet() const {return _nextOfCellViewSet;};

        public: bool _DisplayHasBeenAborted();

        public: virtual void _SetCell(Cell* cell) = 0;
        public: virtual void _SetTransformation(const Transformation& transformation) = 0;
        public: void _SetNextOfCellViewSet(View* view) {_nextOfCellViewSet = view;};

        public: virtual void _PreRepaint();
        public: virtual void _PostRepaint();
        public: void _Repaint(const Box& area);

        public: void _DrawSpot();
        public: void _ShowSpot();
        public: void _HideSpot();

        private: int _GetClipCode(const Unit& x, const Unit& y) const;
        private: bool _ClipLine(Unit& xo, Unit& yo, Unit& xe, Unit& ye) const;
        private: bool _ClipBox(Box& box) const;

        public: void _OnExpose(const ScreenUnit& screenX, const ScreenUnit& screenY, const ScreenUnit& screenWidth, const ScreenUnit& screenHeight, bool update);
        public: void _OnResize(const ScreenUnit& screenWidth, const ScreenUnit& screenHeight);
        public: void _OnMouseEnter(const Point& position);
        public: void _OnMouseMove(const Point& position, unsigned state);
        public: void _OnMouseLeave(const Point& position);
        public: void _OnButtonPress(unsigned button, const Point& position, unsigned state);
        public: void _OnButtonRelease(unsigned button, const Point& position, unsigned state);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::View)

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************



#endif // HURRICANE_VIEW

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
