// *************************************************************************************************
// *************************************************************************************************
//   File:    CellWidget.cpp
// *************************************************************************************************
// *************************************************************************************************

#include "CellWidget.h"

#include "Cell.h"
#include "Boxes.h"
//#include "Command.h"
#include "DataBase.h"
#include "Exception.h"
#include "Layer.h"
#include "Record.h"
//#include "Selector.h"
#include "Slice.h"
#include "Slot.h"
#include "Technology.h"
#include "Transformation.h"
using namespace H;

#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include <math.h>
#include <functional>

//OPEN_MY_NAMESPACE

namespace {

QBrush getBrush(const string &pattern, int redValue, int greenValue, int blueValue) {
    if (pattern == "FFFFFFFFFFFFFFFF") {
        return QBrush(QColor(redValue, greenValue, blueValue));
    } else {
        uchar bits[8];
        for (int i = 0; i < 8; i++) {
            int high = pattern[i * 2];
            if (('0' <= high) && (high <= '9')) {
                high = high - '0';
            } else {  
                if (('a' <= high) && (high <= 'f')) {
                    high = 10 + high - 'a';
                } else {  
                    if (('A' <= high) && (high <= 'F')) {
                        high = 10 + high - 'A';
                    } else {
                        high = '0';
                    }
                }
            }
            int low = pattern[(i * 2) + 1];
            if (('0' <= low) && (low <= '9')) {
                low = low - '0';
            } else {  
                if (('a' <= low) && (low <= 'f')) {
                    low = 10 + low - 'a';
                } else {  
                    if (('A' <= low) && (low <= 'F')) {
                        low = 10 + low - 'A';
                    } else {
                        low = '0';
                    }
                }
            }
            bits[i] = (uchar)((high * 16) + low); 
        }

        return QBrush(QColor(redValue, greenValue, blueValue), QBitmap::fromData(QSize(8,8), bits, QImage::Format_Mono));
    }
}

static QColor  backgroundColor    = QColor(  50,  50,  50 );
static QColor  foregroundColor    = QColor( 255, 255, 255 );
static QColor  rubberColor        = QColor( 192,   0, 192 );
static QColor  phantomColor       = QColor( 139, 134, 130 );
static QColor  boundaryColor      = QColor( 208, 199, 192 );
static QColor  markerColor        = QColor(  80, 250,  80 );
static QColor  selectionDrawColor = QColor( 255, 255, 255 );
static QColor  selectionFillColor = QColor( 255, 255, 255 );
static QColor  gridColor          = QColor( 255, 255, 255 );
static QColor  spotColor          = QColor( 255, 255, 255 );
static QColor  ghostColor         = QColor( 255, 255, 255 );

static QPen boundariesPen       = QPen(boundaryColor);
static QBrush boundariesBrush   = QBrush(boundaryColor);
static QPen phantomsPen         = QPen(phantomColor);
static QBrush phantomsBrush     = QBrush(phantomColor);
static QBrush defaultBrush      = QBrush();
static QPen defaultPen          = QPen();

}


Cell*
CellWidget::getCell() const {
    return _cell;
}

// *************************************************************************************************
// getLabel()
// *************************************************************************************************

///static
///Label
///getLabel(const QColor& color)
///{
///    return getLabel(color.red()) + ":" + getLabel(color.green()) + ":" + getLabel(color.blue());
///}
///


// *************************************************************************************************
// CellWidget::_sPoints
// *************************************************************************************************

//QPointArray CellWidget::_sPoints;



// *************************************************************************************************
// CellWidget::CellWidget()
// *************************************************************************************************

CellWidget::CellWidget(Cell* cell,
        QWidget* parent,
        const char* name)
:   Inherit(parent),
    _cell(cell),
    _center(0, 0),
    _scale(1),
    _screenDx(0),
    _screenDy(0),
    _brushDx(0),
    _brushDy(0),
    _alreadyExposed(false),
    _invalidRegion(),
    _clipBox(),
    _clipX(),
    _clipY(),
    _painter(NULL),
    _backgroundColor(20, 20, 20),
    _foregroundColor(255, 255, 255),
    _doubleBuffering(false),
    _automaticScrolling(false),
    _visibleLayerMask(~0),
    //_commandMap(),
    //_startedCommand(NULL),
    _gridOrigin(),
    _gridXStep(getUnit(1)),
    _gridYStep(getUnit(1)),
    _gridIsVisible(true),
    _gridAxesAreVisible(false),
    _gridDisplayThreshold(8),
    _gridColor(QColor(155, 155, 155)),
    _snapGrid(true),
    _drawSnapPoint(false),
    _snapPoint(),
    _displayDepth(1),
    _displaySize(2),
    //_selectorSet(),
    _selectionLimit(100000),
    _selectionColor(255, 255, 255),
    _selectionIsVisible(true),
    //_highlightorSet(),
    _highlightionLimit(10000),
    _highlightionIsVisible(true),
    //_trackBox(NULL),
    _peekList(),
    _basicLayersBrush(),
    _basicLayersPen()
{
    setFocusPolicy(Qt::StrongFocus); // to accepts focus by tabbing and clicking
    setMouseTracking(true); // to have move events even a mouse button isn't pressed

    DataBase* database = getDataBase();
    if (database) {
        Technology* technology = database->getTechnology();
        if (technology) {
            for_each_basic_layer(basiclayer, technology->getBasicLayers()) {
                _basicLayersBrush[basiclayer] = 
                    ::getBrush(basiclayer->getFillPattern(),
                            basiclayer->getRedValue(),
                            basiclayer->getGreenValue(),
                            basiclayer->getBlueValue());
                _basicLayersPen[basiclayer] = 
                        QPen(QColor(basiclayer->getRedValue(), basiclayer->getGreenValue(), basiclayer->getBlueValue()));
                end_for;
            }
        }
    }
}



// *************************************************************************************************
// CellWidget::~CellWidget()
// *************************************************************************************************

CellWidget::~CellWidget()
{
    clearPeeks();
    //clearHighlightion();
    //clearSelection();

    //forEachCommand(command, getCommands()) {
    //    command->uninstallFrom(this);
    //    endFor;
    //}

    //if (_trackBox) {
    //    _trackBox->_setCellWidget(NULL);
    //}
}



// *************************************************************************************************
// CellWidget::getLabel()
// CellWidget::getRecord()
// CellWidget::getSlot()
// *************************************************************************************************

//Label
//CellWidget::getLabel() const
//{
//    Label label = Label::create(getTypeName(*this));
//
//    View* view = getView();
//    if (view) {
//    label.add(" ", view);
//    }
//
//    return label;
//}
//
//Record*
//CellWidget::getRecord() const
//{
//    Record* record = new Record(this);
//
//    if (record) {
//    record->addHeader("Widget");
//
//    record->addHeader("CellWidget");
//
//    record->addSlot("View",                  getView());
//    record->addSlot("Center",                getCenter());
//    record->addSlot("Scale",                 getScale());
//    record->addSlot("BackgroundColor",       My::getLabel(getBackgroundColor()));
//    record->addSlot("ForegroundColor",       My::getLabel(getForegroundColor()));
//    record->addSlot("DoubleBuffering",       doubleBuffering());
//    record->addSlot("AutomaticScrolling",    automaticScrolling());
//    record->addSlot("VisibleLayers",         getVisibleLayers());
//    record->addSlot("Commands",              getCommands());
//    record->addSlot("GridOrigin",            getGridOrigin());
//    record->addSlot("GridXStep",             getValueString(getGridXStep()));
//    record->addSlot("GridYStep",             getValueString(getGridYStep()));
//    record->addSlot("GridDisplayThreshold",  getGridDisplayThreshold());
//    record->addSlot("GridIsVisible",         gridIsVisible());
//    record->addSlot("GridAxesAreVisible",    gridAxesAreVisible());
//    record->addSlot("GridIsDrawable",        gridIsDrawable());
//    record->addSlot("GridColor",             My::getLabel(getGridColor()));
//    record->addSlot("SnapGrid",              snapGrid());
//    record->addSlot("DisplayDepth",          getDisplayDepth());
//    record->addSlot("DisplaySize",           getDisplaySize());
//    record->addSlot("SelectionIsVisible",    selectionIsVisible());
//    record->addSlot("Selection",             getSelection());
//    record->addSlot("SelectionBox",          getSelectionBox());
//    unsigned limit = getSelectionLimit();
//    if (limit) {
//        record->addSlot("SelectionLimit", limit);
//    }
//    else {
//        record->addSlot("SelectionLimit", "NONE");
//    }
//    record->addSlot("SelectionColor",        My::getLabel(getSelectionColor()));
//    record->addSlot("HighlightionIsVisible", highlightionIsVisible());
//    record->addSlot("Highlightion",          getHighlightion());
//    record->addSlot("HighlightionBox",       getHighlightionBox());
//    record->addSlot("TrackBox",              getTrackBox());
//    }
//
//    return record;
//}
//
//Slot*
//CellWidget::getSlot(const string& name) const
//{
//    return new StandardSlot<const CellWidget*>(name, this);
//}
//


// *************************************************************************************************
// CellWidget::getCenter()
// CellWidget::getScale()
// *************************************************************************************************

const Point&
CellWidget::getCenter() const
{
    return _center;
}

double
CellWidget::getScale() const
{
    return _scale;
}



// *************************************************************************************************
// CellWidget::getX()
// CellWidget::getY()
// CellWidget::getSize()
// CellWidget::getPoint()
// CellWidget::getBox()
// *************************************************************************************************

Unit
CellWidget::getX(int screenX) const
{
    return getUnit((screenX - _screenDx) / _scale);
}

Unit
CellWidget::getY(int screenY) const
{
    return getUnit(((height() - screenY) - _screenDy) / _scale);
}

Unit
CellWidget::getSize(int screenSize) const
{
    return getUnit(screenSize / _scale);
}

Point
CellWidget::getPoint(const QPoint& screenPoint) const
{
    return Point(getX(screenPoint.x()), getY(screenPoint.y()));
}

Box
CellWidget::getBox(const QRect& screenRect) const
{
    if (screenRect.isEmpty()) {
    return Box();
    }

    return Box(getX(screenRect.left()),
           getY(screenRect.bottom()),
           getX(screenRect.right()),
           getY(screenRect.top()));
}



// *************************************************************************************************
// CellWidget::getScreenX()
// CellWidget::getScreenY()
// CellWidget::getScreenSize()
// CellWidget::getScreenPoint()
// CellWidget::getScreenBox()
// *************************************************************************************************

int
CellWidget::getScreenX(const Unit& x) const
{
    return _screenDx + (int)rint(getValue(x) * _scale);
}

int
CellWidget::getScreenY(const Unit& y) const
{
    return height() - (int)rint(_screenDy + (getValue(y) * _scale));
}

int
CellWidget::getScreenSize(const Unit& size) const
{
    return (int)rint(getValue(size) * _scale);
}

QPoint
CellWidget::getScreenPoint(const Point& point) const
{
    return QPoint(getScreenX(point.getX()), getScreenY(point.getY()));
}

QRect
CellWidget::getScreenRect(const Box& box) const
{
    if (box.isEmpty()) {
    return QRect();
    }

    int screenX1 = getScreenX(box.getXMin());
    int screenY1 = getScreenY(box.getYMin());
    int screenX2 = getScreenX(box.getXMax());
    int screenY2 = getScreenY(box.getYMax());

    return QRect(QPoint(min(screenX1, screenX2), min(screenY1, screenY2)),
         QPoint(max(screenX1, screenX2), max(screenY1, screenY2)));
}



// *************************************************************************************************
// CellWidget::getSnapPoint()
// *************************************************************************************************

Point
CellWidget::getSnapPoint(const Point& point) const
{
    return (_snapGrid) ? getGridPoint(point) : point;
}

Point
CellWidget::getSnapPoint(int screenX,
               int screenY) const
{
    return getSnapPoint(Point(getX(screenX), getY(screenY)));
}



// *************************************************************************************************
// CellWidget::getVisibleLayers()
// *************************************************************************************************

BasicLayers
CellWidget::getVisibleLayers() const
{
    DataBase* database = getDataBase();
    if (database) {
    Technology* technology = database->getTechnology();
    if (technology) {
            return technology->getBasicLayers(_visibleLayerMask);
    }
    }
    return NULL;
}



// *************************************************************************************************
// CellWidget::getCommand()
// CellWidget::getCommands()
// *************************************************************************************************

//Command*
//CellWidget::getCommand(int button) const
//{
//    CommandMap::const_iterator i = _commandMap.find(button);
//
//    return (i != _commandMap.end()) ? (*i).second : NULL;
//}
//
//Commands
//CellWidget::getCommands() const
//{
//    return _commandMap;
//}
//


// *************************************************************************************************
// CellWidget::getGridX()
// CellWidget::getGridY()
// CellWidget::getGridPoint()
// *************************************************************************************************

Unit
CellWidget::getGridX(const Unit& x,
           int         sign) const
{
    if (!((-1 <= sign) && (sign <= 1))) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    Unit cx = x;
    Unit xo = _gridOrigin.getX();

    if (cx != xo) {
    cx -= xo;
    switch (sign) {
        case -1 : {
        if (0 < cx) {
            return ((cx / _gridXStep) * _gridXStep) + xo;
        }
        else {
            if (cx < 0) {
            return (((cx / _gridXStep) - 1) * _gridXStep) + xo;
            }
        }
        return cx + xo;
        }
        case 0 : {
        Unit x1 = (cx / _gridXStep) * _gridXStep;
        Unit x2 = ((x1 < cx) ? (x1 + _gridXStep) : (x1 - _gridXStep));
        return ((labs(x1 - cx) <= labs(x2 - cx)) ? x1 : x2) + xo;
        }
        case 1 : {
        if (0 < cx) {
            return (((cx / _gridXStep) + 1) * _gridXStep) + xo;
        }
        else {
            if (cx < 0) {
            return ((cx / _gridXStep) * _gridXStep) + xo;
            }
        }
        return cx + xo;
        }
    }
    }

    return cx;
}

Unit
CellWidget::getGridY(const Unit& y,
           int         sign) const
{
    if (!((-1 <= sign) && (sign <= 1))) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    Unit cy = y;
    Unit yo = _gridOrigin.getY();

    if (cy != yo) {
    cy -= yo;
    switch (sign) {
        case -1 : {
        if (0 < cy) {
            return ((cy / _gridYStep) * _gridYStep) + yo;
        }
        else {
            if (cy < 0) {
            return (((cy / _gridYStep) - 1) * _gridYStep) + yo;
            }
        }
        return cy + yo;
        }
        case 0 : {
        Unit y1 = (cy / _gridYStep) * _gridYStep;
        Unit y2 = ((y1 < cy) ? (y1 + _gridYStep) : (y1 - _gridYStep));
        return ((labs(y1 - cy) <= labs(y2 - cy)) ? y1 : y2) + yo;
        }
        case 1 : {
        if (0 < cy) {
            return (((cy / _gridYStep) + 1) * _gridYStep) + yo;
        }
        else {
            if (cy < 0) {
            return ((cy / _gridYStep) * _gridYStep) + yo;
            }
        }
        return cy + yo;
        }
    }
    }

    return cy;
}

Point
CellWidget::getGridPoint(const Point& point,
               int          xSign,
               int          ySign) const
{
    return Point(getGridX(point.getX(), xSign), getGridY(point.getY(), ySign));
}



// *************************************************************************************************
// CellWidget::getWorld()
// *************************************************************************************************

//Box
//CellWidget::getWorld() const
//{
//    Box world = Box(getX(0), getY(0), getX(width()), getY(height()));
//
//    View* view = getView();
//
//    if (view) {
//    world.merge(view->getBoundingBox());
//    }
//
//    return world;
//}



// *************************************************************************************************
// CellWidget::getSelectors()
// CellWidget::getSelection()
// CellWidget::getSelectionBox()
// CellWidget::getHighlightors()
// CellWidget::getHighlightion()
// CellWidget::getHighlightionBox()
// *************************************************************************************************

//Selectors
//CellWidget::getSelectors() const
//{
//    return _selectorSet;
//}

//Occurrences
//CellWidget::getSelection() const
//{
//    return getSelectors().getAssociates(getOccurrence<Selector>());
//}

//Box
//CellWidget::getSelectionBox() const
//{
//    Box selectionBox;
//
//    forEachOccurrence(occurrence, getSelection()) {
//    selectionBox.merge(occurrence.getBoundingBox());
//    endFor;
//    }
//
//    return selectionBox;
//}
//
//Highlightors
//CellWidget::getHighlightors() const
//{
//    return _highlightorSet;
//}
//
//Occurrences
//CellWidget::getHighlightion() const
//{
//    return getHighlightors().getAssociates(getOccurrence<Highlightor>());
//}
//
//Box
//CellWidget::getHighlightionBox() const
//{
//    Box highlightionBox;
//
//    forEachOccurrence(occurrence, getHighlightion()) {
//    highlightionBox.merge(occurrence.getBoundingBox());
//    endFor;
//    }
//
//    return highlightionBox;
//}
//


// *************************************************************************************************
// CellWidget::getFont()
// CellWidget::getPen()
// CellWidget::getBrush()
// CellWidget::getBrushOrigin()
// *************************************************************************************************

const QFont&
CellWidget::getFont()
{
    return (_painter) ? _painter->font() : QPainter(this).font();
}

const QPen&
CellWidget::getPen()
{
    return (_painter) ? _painter->pen() : QPainter(this).pen();
}


const QPen&
CellWidget::getPen(const BasicLayer* basiclayer) {
    map<const BasicLayer*, QPen>::const_iterator pmit = _basicLayersPen.find(basiclayer);
    if (pmit != _basicLayersPen.end()) {
        return pmit->second;
    }
    return defaultPen; 
}

const QBrush&
CellWidget::getBrush()
{
    return (_painter) ? _painter->brush() : QPainter(this).brush();
}

const QBrush&
CellWidget::getBrush(BasicLayer* basiclayer) {
    map<const BasicLayer*, QBrush>::const_iterator bmit = _basicLayersBrush.find(basiclayer);
    if (bmit != _basicLayersBrush.end()) {
        return bmit->second;
    }
    return defaultBrush;
}

const QPoint
CellWidget::getBrushOrigin()
{
    return (_painter) ? _painter->brushOrigin() : QPainter(this).brushOrigin();
}



// *************************************************************************************************
// CellWidget::hasRecord()
// *************************************************************************************************

bool
CellWidget::hasRecord() const
{
    return true;
}



// *************************************************************************************************
// CellWidget::setTrackBox()
// *************************************************************************************************

//void
//CellWidget::setTrackBox(TrackBox* trackBox)
//{
//    if (trackBox != _trackBox) {
//    if (_trackBox) {
//        _trackBox->_setCellWidget(NULL);
//    }
//    _trackBox = trackBox;
//    if (_trackBox) {
//        _trackBox->_setCellWidget(this);
//    }
//    }
//}
//


// *************************************************************************************************
// CellWidget::allowAutomaticScrolling()
// *************************************************************************************************

bool
CellWidget::allowAutomaticScrolling() const
{
    return true;
}



// *************************************************************************************************
// CellWidget::isVisible()
// CellWidget::isDrawable()
// *************************************************************************************************

bool
CellWidget::isVisible(BasicLayer* layer) const
{
    if (!layer) {
    //throw Error(NULL_LAYER, __FILE__, __LINE__);
    }

    return (_visibleLayerMask & layer->getMask());
}

bool
CellWidget::isDrawable(BasicLayer* layer) const
{
    if (!layer) {
    //throw Error(NULL_LAYER, __FILE__, __LINE__);
    }

    return (layer->getDisplayThreshold() <= _scale);
}



// *************************************************************************************************
// CellWidget::isOnGridX()
// CellWidget::isOnGridY()
// CellWidget::isOnGridPoint()
// *************************************************************************************************

bool
CellWidget::isOnGridX(const Unit& x,
            unsigned    n) const
{
    if (!n) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    Unit delta = labs(x - _gridOrigin.getX());

    if (delta < 0) {
    delta = -delta;
    }

    Unit step = _gridXStep * n;

    return (delta == ((delta / step) * step));
}

bool
CellWidget::isOnGridY(const Unit& y,
            unsigned    n) const
{
    if (!n) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    Unit delta = labs(y - _gridOrigin.getY());

    if (delta < 0) {
    delta = -delta;
    }

    Unit step = _gridYStep * n;

    return (delta == ((delta / step) * step));
}

bool
CellWidget::isOnGridPoint(const Point& point,
            unsigned     xN,
            unsigned     yN) const
{
    return isOnGridX(point.getX(), xN) && isOnGridY(point.getX(), yN);
}



// *************************************************************************************************
// CellWidget::isSelected()
// CellWidget::isSelectable()
// CellWidget::isHighlighted()
// CellWidget::isHighlightable()
// *************************************************************************************************

//bool
//CellWidget::isSelected(const Occurrence& occurrence) const
//{
//    Selector* selector =
//    dynamic_cast<Selector*>(occurrence.getProperty(Selector::getPropertyName()));
//
//    return (selector) ? selector->_contains((CellWidget*)this) : false;
//}
//
//bool
//CellWidget::isSelectable(const Occurrence& occurrence) const
//{
//    return occurrence.isSelectable() && (occurrence.getView() == getView());
//}
//
//bool
//CellWidget::isHighlighted(const Occurrence& occurrence) const
//{
//    Highlightor* highlightor =
//    dynamic_cast<Highlightor*>(occurrence.getProperty(Highlightor::getPropertyName()));
//
//    return (highlightor) ? highlightor->_contains((CellWidget*)this) : false;
//}
//
//bool
//CellWidget::isHighlightable(const Occurrence& occurrence) const
//{
//    return occurrence.isHighlightable() && (occurrence.getView() == getView());
//}



// *************************************************************************************************
// CellWidget::setBackgroundColor()
// CellWidget::setForegroundColor()
// *************************************************************************************************

void
CellWidget::setBackgroundColor(const QColor& color)
{
    if (color != _backgroundColor) {
    _backgroundColor = color;
    invalidate();
    }
}

void
CellWidget::setForegroundColor(const QColor& color)
{
    if (color != _foregroundColor) {
    _foregroundColor = color;
    invalidate();
    }
}



// *************************************************************************************************
// CellWidget::setVisible()
// *************************************************************************************************

void
CellWidget::setVisible(BasicLayer* layer,
             bool   visible)
{
    if (isVisible(layer) != visible) {
    if (visible) {
        _visibleLayerMask |= layer->getMask();
    }
    else {
        _visibleLayerMask &= ~layer->getMask();
    }

    onSetVisible(layer, visible);

    Cell* cell = getCell();

    //if (view && !view->getSlices(layer).isEmpty() && isDrawable(layer)) {
    //    invalidate();
    //}

    if (cell && !cell->getSlices(layer->getMask()).IsEmpty() && isDrawable(layer)) {
        invalidate();
    }
    }
}



// *************************************************************************************************
// CellWidget::setGridOrigin()
// CellWidget::setGridXStep()
// CellWidget::setGridYStep()
// CellWidget::setGridStep()
// CellWidget::setGridSteps()
// CellWidget::setGridDisplayThreshold()
// CellWidget::setGridColor()
// *************************************************************************************************

void
CellWidget::setGridOrigin(const Point& origin)
{
    if (origin != _gridOrigin) {
    bool gridIsDrawn = gridIsVisible() && gridIsDrawable();

    _gridOrigin = origin;

    if (gridIsDrawn || (gridIsVisible() && gridIsDrawable())) {
        invalidate();
    }
    }
}

void
CellWidget::setGridXStep(const Unit& step)
{
    if (!step) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if (step != _gridXStep) {
    bool gridIsDrawn = gridIsVisible() && gridIsDrawable();

    _gridXStep = step;

    if (gridIsDrawn || (gridIsVisible() && gridIsDrawable())) {
        invalidate();
    }
    }
}

void
CellWidget::setGridYStep(const Unit& step)
{
    if (!step) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if (step != _gridYStep) {
    bool gridIsDrawn = gridIsVisible() && gridIsDrawable();

    _gridYStep = step;

    if (gridIsDrawn || (gridIsVisible() && gridIsDrawable())) {
        invalidate();
    }
    }
}

void
CellWidget::setGridStep(const Unit& step)
{
    if (!step) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if ((step != _gridXStep) || (step != _gridYStep)) {
    bool gridIsDrawn = gridIsVisible() && gridIsDrawable();

    _gridXStep = step;
    _gridYStep = step;

    if (gridIsDrawn || (gridIsVisible() && gridIsDrawable())) {
        invalidate();
    }
    }
}

void
CellWidget::setGridSteps(const Unit& xStep,
               const Unit& yStep)
{
    if (!xStep || !yStep) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if ((xStep != _gridXStep) || (yStep != _gridYStep)) {
    bool gridIsDrawn = gridIsVisible() && gridIsDrawable();

    _gridXStep = xStep;
    _gridYStep = yStep;

    if (gridIsDrawn || (gridIsVisible() && gridIsDrawable())) {
        invalidate();
    }
    }
}

void
CellWidget::setGridDisplayThreshold(unsigned threshold)
{
    if (threshold < 3) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    _gridDisplayThreshold = threshold;

    invalidate();
}

void
CellWidget::setGridColor(const QColor& color)
{
    if (_gridColor != color) {
    _gridColor = color;
    if (gridIsVisible() && gridIsDrawable()) {
        invalidate();
    }
    }
}



// *************************************************************************************************
// CellWidget::setGridVisible()
// CellWidget::setGridAxesVisible()
// CellWidget::setSnapGrid()
// *************************************************************************************************

void
CellWidget::setGridVisible(bool visible)
{
    if (_gridIsVisible != visible) {
    _gridIsVisible = visible;
    if (gridIsDrawable()) {
        invalidate();
    }
    }
}

void
CellWidget::setGridAxesVisible(bool visible)
{
    if (_gridAxesAreVisible != visible) {
    _gridAxesAreVisible = visible;
    if (gridIsDrawable()) {
        invalidate();
    }
    }
}

void
CellWidget::setSnapGrid(bool enable)
{
    if (_snapGrid != enable) {
    if (_snapGrid && _drawSnapPoint) {
        drawSnapPoint();
        _drawSnapPoint = false;
    }
    _snapGrid = enable;
    if (_snapGrid && !_drawSnapPoint) {
        _drawSnapPoint = true;
        drawSnapPoint();
    }
    }
}



// *************************************************************************************************
// CellWidget::setDisplayDepth()
// CellWidget::setDisplayDepth()
// *************************************************************************************************

void
CellWidget::setDisplayDepth(unsigned displayDepth)
{
    if (displayDepth != _displayDepth) {
    _displayDepth = displayDepth;
    invalidate();
    }
}

void
CellWidget::setDisplaySize(unsigned displaySize)
{
    if (displaySize != _displaySize) {
    _displaySize = displaySize;
    }
}



// *************************************************************************************************
// CellWidget::setFont()
// CellWidget::setPen()
// CellWidget::setBrush()
// CellWidget::setBrushOrigin()
// *************************************************************************************************

void
CellWidget::setFont(const QFont& font)
{
    if (_painter) {
    _painter->setFont(font);
    }
}

void
CellWidget::setPen(const QPen& pen,
         double      brightness)
{
    if (!((0.1 <= brightness) && (brightness <= 1.0))) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if (_painter) {
    if (pen == Qt::NoPen) {
        _painter->setPen(pen);
    }
    else {
        QPen correctedPen = pen;

        if (brightness < 1) {
        QColor bgColor = getBackgroundColor();
        int r = bgColor.red();
        int g = bgColor.green();
        int b = bgColor.blue();

        QColor color = pen.color();
        r = r + (int)((color.red() - r) * brightness);
        g = g + (int)((color.green() - g) * brightness);
        b = b + (int)((color.blue() - b) * brightness);

        correctedPen = QPen(QColor(r, g, b));
        }

        _painter->setPen(correctedPen);
    }
    }
}

void
CellWidget::setBrush(const QBrush& brush,
           double        brightness)
{
    if (!((0.1 <= brightness) && (brightness <= 1.0))) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    if (_painter) {
    if (brush == Qt::NoBrush) {
        _painter->setBrush(brush);
    }
    else {
        QBrush correctedBrush = brush;

        if (brightness < 1) {
        QColor bgColor = getBackgroundColor();
        int r = bgColor.red();
        int g = bgColor.green();
        int b = bgColor.blue();

        QColor color = brush.color();
        r = r + (int)((color.red() - r) * brightness);
        g = g + (int)((color.green() - g) * brightness);
        b = b + (int)((color.blue() - b) * brightness);

        correctedBrush = QBrush(QColor(r, g, b), brush.style());
        }

        _painter->setBrush(correctedBrush);
    }
    }
}

void
CellWidget::setBrushOrigin(const QPoint& origin)
{
    if (_painter) {
    _painter->setBrushOrigin(_brushDx + origin.x(), _brushDy + origin.y());
    }
}



// *************************************************************************************************
// CellWidget::setSelectionColor()
// CellWidget::setSelectionVisible()
// CellWidget::setHighlightionVisible()
// *************************************************************************************************

//void
//CellWidget::setSelectionColor(const QColor& color)
//{
//    if (color != _selectionColor) {
//    _selectionColor = color;
//    if (selectionIsVisible()) {
//        invalidate(getSelectionBox());
//    }
//    }
//}
//
//void
//CellWidget::setSelectionVisible(bool visible)
//{
//    if (_selectionIsVisible != visible) {
//    _selectionIsVisible = visible;
//    invalidate(getSelectionBox());
//    }
//}
//
//void
//CellWidget::setHighlightionVisible(bool visible)
//{
//    if (_highlightionIsVisible != visible) {
//    _highlightionIsVisible = visible;
//    invalidate(getHighlightionBox());
//    }
//}
//


// *************************************************************************************************
// CellWidget::setAutomaticScrolling()
// *************************************************************************************************

void
CellWidget::setAutomaticScrolling(bool enable)
{
    if (!allowAutomaticScrolling()) {
    //throw Error(INVALID_REQUEST, __FILE__, __LINE__);
    }

    _automaticScrolling = enable;
}



// *************************************************************************************************
// CellWidget::showAllLayers()
// CellWidget::hideAllLayers()
// *************************************************************************************************

void
CellWidget::showAllLayers()
{
    DataBase* database = getDataBase();
    if (database) {
    Technology* technology = database->getTechnology();
    if (technology) {
            for_each_basic_layer(layer, technology->getBasicLayers()) {
        setVisible(layer, true);
        end_for;
        }
    }
    }
}

void
CellWidget::hideAllLayers()
{
    DataBase* database = getDataBase();
    if (database) {
    Technology* technology = database->getTechnology();
    if (technology) {
            for_each_basic_layer(layer, technology->getBasicLayers()) {
        setVisible(layer, false);
        end_for;
        }
    }
    }
}



// *************************************************************************************************
// CellWidget::select()
// CellWidget::unselect()
// CellWidget::clearSelection()
// *************************************************************************************************

//bool
//CellWidget::select(const Occurrence& occurrence)
//{
//    if (!isSelectable(occurrence)) {
//    throw Error(INVALID_REQUEST, __FILE__, __LINE__);
//    }
//
//    if (_selectionLimit && (_selectionLimit <= getSelectors().getCount())) {
//    return false;
//    }
//
//    Property* property = occurrence.getProperty(Selector::getPropertyName());
//
//    Selector* selector = NULL;
//
//    if (!property) {
//    selector = new Selector(occurrence);
//    }
//    else {
//    if (!dynamic_cast<Selector*>(property)) {
//        throw Error(BAD_PROPERTY_TYPE, __FILE__, __LINE__);
//    }
//    selector = (Selector*)property;
//    }
//
//    selector->_attachTo(this);
//
//    invalidate(occurrence.getBoundingBox());
//
//    return true;
//}

//void
//CellWidget::unselect(const Occurrence& occurrence)
//{
//    if (!isSelectable(occurrence)) {
//    throw Error(INVALID_REQUEST, __FILE__, __LINE__);
//    }
//
//    Property* property = occurrence.getProperty(Selector::getPropertyName());
//
//    if (property) {
//    if (!dynamic_cast<Selector*>(property)) {
//        throw Error(BAD_PROPERTY_TYPE, __FILE__, __LINE__);
//    }
//
//    ((Selector*)property)->_detachFrom(this);
//
//    invalidate(occurrence.getBoundingBox());
//    }
//}
//
//void
//CellWidget::clearSelection()
//{
//    Occurrences selection = getSelection();
//
//    bool _cumulativeMode = false; // HOOPS !!!
//    if (_cumulativeMode) {
//    invalidate(getSelectionBox());
//    }
//
//    while (!selection.isEmpty()) {
//    unselect(selection.getFirst());
//    }
//}
//
//
//
//// *************************************************************************************************
//// CellWidget::highlight()
//// CellWidget::unhighlight()
//// CellWidget::clearHighlightion()
//// *************************************************************************************************
//
//bool
//CellWidget::highlight(const Occurrence& occurrence)
//{
//    if (!isHighlightable(occurrence)) {
//    throw Error(INVALID_REQUEST, __FILE__, __LINE__);
//    }
//
//    unsigned count = getHighlightors().getCount();
//
//    if (_highlightionLimit && (_highlightionLimit <= count)) {
//    return false;
//    }
//
//    Property* property = occurrence.getProperty(Highlightor::getPropertyName());
//
//    Highlightor* highlightor = NULL;
//
//    if (!property) {
//    highlightor = new Highlightor(occurrence);
//    }
//    else {
//    if (!dynamic_cast<Highlightor*>(property)) {
//        throw Error(BAD_PROPERTY_TYPE, __FILE__, __LINE__);
//    }
//    highlightor = (Highlightor*)property;
//    }
//
//    highlightor->_attachTo(this);
//
//    if (!count) {
//    invalidate();
//    }
//    else {
//    invalidate(occurrence.getBoundingBox());
//    }
//
//    return true;
//}
//
//void
//CellWidget::unhighlight(const Occurrence& occurrence)
//{
//    if (!isHighlightable(occurrence)) {
//    throw Error(INVALID_REQUEST, __FILE__, __LINE__);
//    }
//
//    Property* property = occurrence.getProperty(Highlightor::getPropertyName());
//
//    if (property) {
//    if (!dynamic_cast<Highlightor*>(property)) {
//        throw Error(BAD_PROPERTY_TYPE, __FILE__, __LINE__);
//    }
//
//    ((Highlightor*)property)->_detachFrom(this);
//
//    if (getHighlightors().isEmpty()) {
//        invalidate();
//    }
//    else {
//        invalidate(occurrence.getBoundingBox());
//    }
//    }
//}
//
//void
//CellWidget::clearHighlightion()
//{
//    Occurrences highlightion = getHighlightion();
//
//    bool _cumulativeMode = false; // HOOPS !!!
//    if (_cumulativeMode) {
//    invalidate(getHighlightionBox());
//    }
//
//    while (!highlightion.isEmpty()) {
//    unhighlight(highlightion.getFirst());
//    }
//}
//
//

// *************************************************************************************************
// CellWidget::clearPeeks()
// CellWidget::addPeek()
// *************************************************************************************************

void
CellWidget::clearPeeks()
{
    for (list<Box>::iterator plit = _peekList.begin();
            plit != _peekList.end();
            ++plit) {
        invalidate(*plit);
    }
//    for_each_box(peek, getCollection(_peekList)) {
//    invalidate(peek);
//    end_for;
//    }
//
    _peekList.clear();
}

void
CellWidget::addPeek(const Box& peek)
{
    if (!peek.isEmpty()) {
    invalidate(peek);
    _peekList.push_back(peek);
    }
}



// *************************************************************************************************
// CellWidget::invalidate()
// *************************************************************************************************

void
CellWidget::invalidate()
{
    _invalidRegion = QRegion(rect());
}

void
CellWidget::invalidate(const QRect& screenRect)
{
    QRect visibleScreenRect = screenRect.intersect(rect());

    if (!visibleScreenRect.isEmpty()) {
    bool _cumulativeMode = false; // HOOPS !!!
    if (!_cumulativeMode) {
        _invalidRegion = _invalidRegion.unite(QRegion(visibleScreenRect));
    }
    else {
        _invalidRegion = QRegion(_invalidRegion.boundingRect().unite(visibleScreenRect));
    }
    }
}

void
CellWidget::invalidate(const Box& box)
{
    if (!box.isEmpty()) {
    int safetyMargin = 2; // 2 pixel
    invalidate(getScreenRect(box.getInflated(getSize(safetyMargin))));
    }
}

void
CellWidget::fitToContent(unsigned screenMargin) {
    Box area(getX(0), getY(0), getX(width()), getY(height()));
    if (_cell) {
    area = _cell->getBoundingBox();
    }

    reframe(area);
    area.inflate(screenMargin);
    reframe(area);
}
    



// *************************************************************************************************
// CellWidget::reframe()
// *************************************************************************************************

void
CellWidget::reframe()
{
    reframe(_center, _scale);
}

void
CellWidget::reframe(double scale)
{
    reframe(_center, scale);
}

void
CellWidget::reframe(const Point& center)
{
    reframe(center, _scale);
}

void
CellWidget::reframe(const Point& center,
          double       scale)
{
    if (0 < scale) {
    _center = center;
    _scale = scale;
    _screenDx = -(int)rint((getValue(_center.getX()) - (width() / (_scale*2))) * _scale);
    _screenDy = -(int)rint((getValue(_center.getY()) - (height() / (_scale*2))) * _scale);
    _brushDx = 0;
    _brushDy = 0;
    invalidate();
    }
}

void
CellWidget::reframe(const Box& box)
{
    if (!box.isEmpty()) {
    Point center = box.getCenter();

    double scale = min((double)width() / getValue(box.getWidth()),
               (double)height() / getValue(box.getHeight()));

    reframe(center, scale);
    }
}



// *************************************************************************************************
// CellWidget::scroll()
// *************************************************************************************************

void
CellWidget::scroll(const Unit& dx,
         const Unit& dy)
{
    if ((getSize(width()) < labs(dx)) || (getSize(height()) < labs(dy))) {
    reframe(getCenter().getTranslated(-dx, -dy));
    redraw();
    } else {
    int sdx = getScreenSize(dx);
    int sdy = getScreenSize(dy);

    if (sdx || sdy) {
        QCursor oldCursor = cursor();
        setCursor(Qt::WaitCursor);

        if (_snapGrid) {
        drawSnapPoint();
        }

        //forEachCommand(command, getCommands()) {
        //    _callDrawOf(command);
        //    endFor;
        //}

        _screenDx += sdx;
        _screenDy += sdy;

        _brushDx += sdx;
        _brushDy -= sdy;

        _center = getPoint(QPoint(width() / 2, height() / 2));

        int w = width();
        int h = height();

        if (0 < sdx) {
        if (0 < sdy) {
                    //bitBlt(this, sdx, 0, this, 0, sdy, w - sdx, h - sdy);
            invalidate(QRect(-1, -1, sdx + 1, h + 1));
            //_redraw();
            invalidate(QRect(sdx - 1, h - sdy - 1, w + 1, h + 1));
            //_redraw();
                    update();
        } else {
            //bitBlt(this, sdx, -sdy, this, 0, 0, w - sdx, h + sdy, CopyROP);
            invalidate(QRect(-1, -1, sdx + 1, h + 1));
            //_redraw();
            invalidate(QRect(sdx - 1, -1, w + 1, -sdy + 1));
            //_redraw();
                    update();
        }
        }
        else {
        if (0 < sdy) {
            //bitBlt(this, 0, 0, this, -sdx, sdy, w + sdx, h - sdy, CopyROP);
            invalidate(QRect(w + sdx - 1, -1, w + 1, h + 1));
           // _redraw();
            invalidate(QRect(-1, h - sdy - 1, w + sdx + 1, h + 1));
            //_redraw();
                    update();
        }
        else {
            //bitBlt(this, 0, -sdy, this, -sdx, 0, w + sdx, h + sdy);
            invalidate(QRect(w + sdx - 1, -1, w + 1, h + 1));
            //_redraw();
            invalidate(QRect(-1, -1, w + sdx + 1, -sdy + 1));
            //_redraw();
                    update();
        }
        }

        //forEachCommand(command, reverse(getCommands())) {
        //    _callDrawOf(command);
        //    endFor;
        //}

        if (_snapGrid) {
        drawSnapPoint();
        }

        setCursor(oldCursor);
    }
    }
}



// *************************************************************************************************
// CellWidget::redraw()
// *************************************************************************************************

void
CellWidget::redraw()
{
    if (!_invalidRegion.isEmpty()) {
    QCursor oldCursor = cursor();
    setCursor(Qt::WaitCursor);

    if (_snapGrid) {
        drawSnapPoint();
    }

    //forEachCommand(command, getCommands()) {
    //    _callDrawOf(command);
    //    endFor;
    //}

    if (!_alreadyExposed) {
        fitToContent();
        _alreadyExposed = true;
    }

    _redraw();

    //forEachCommand(command, reverse(getCommands())) {
    //    _callDrawOf(command);
    //    endFor;
    //}

    if (_snapGrid) {
        drawSnapPoint();
    }

    setCursor(oldCursor);

    _invalidRegion = QRegion();
    }
}



// *************************************************************************************************
// CellWidget::abortStartedCommand()
// *************************************************************************************************

//void
//CellWidget::abortStartedCommand()
//{
//    if (_startedCommand) {
//    _startedCommand->abortOn(this);
//    }
//
//    if (_trackBox) {
//    _trackBox->stop(_snapPoint);
//    }
//}



// *************************************************************************************************
// CellWidget::preRedraw()
// CellWidget::postRedraw()
// *************************************************************************************************

void
CellWidget::preRedraw(const Box& area)
{
}

void
CellWidget::postRedraw(const Box& area)
{
}


void CellWidget::drawContent(const Cell* cell, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const {
    for_each_instance(instance, cell->getInstancesUnder(updateArea)) {
        drawContent(instance, basicLayer, updateArea, transformation);
        end_for;
    }

    for_each_slice(slice, cell->getSlices()) {
        drawSlice(slice, basicLayer, updateArea, transformation);
        end_for;
    }
}


void CellWidget::drawContent(const Instance* instance, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const {
    Box masterArea = updateArea;
    Transformation masterTransformation = instance->getTransformation();
    instance->getTransformation().getInvert().applyOn(masterArea);
    transformation.applyOn(masterTransformation);
    drawContent(instance->getMasterCell(), basicLayer, masterArea, masterTransformation);
}


void CellWidget::drawSlice(const Slice* slice, const BasicLayer* basicLayer, const H::Box& updateArea, const Transformation& transformation) const {
    if (slice->getLayer()->contains(basicLayer)) {
        if (slice->getBoundingBox().intersect(updateArea)) {
            //if ((basicLayer == _layer->_getSymbolicBasicLayer()) || (3 < view->getScale())) 
            for_each_go(go, slice->getGosUnder(updateArea)) {
                drawGo(go, basicLayer, updateArea, transformation);
                end_for;
            }
        }
    }
}

void CellWidget::drawGo(const Go* go, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const {
    const Segment* segment = dynamic_cast<const Segment*>(go);
    if (segment) {
        drawSegment(segment, basicLayer, updateArea, transformation);
        return;
    }
    const Contact* contact = dynamic_cast<const Contact*>(go);
    if (contact) {
        drawContact(contact, basicLayer, updateArea, transformation);
        return;
    }
}

void CellWidget::drawSegment(const Segment* segment, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const {
    if (1 < getScreenSize(segment->getWidth())) {
        drawRectangle(transformation.getBox(segment->getBoundingBox(basicLayer)));
    } else {
        drawLine(transformation.getPoint(segment->getSourcePosition()),
                transformation.getPoint(segment->getTargetPosition()));
    }
}

void CellWidget::drawContact(const Contact* contact, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const {
    drawRectangle(transformation.getBox(contact->getBoundingBox(basicLayer)));
}

void CellWidget::drawPhantoms(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const {
    for_each_instance(instance, cell->getInstancesUnder(updateArea)) {
        drawPhantoms(instance, updateArea, transformation);
        end_for;
    }
}

void CellWidget::drawPhantoms(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const {
    H::Box masterArea = updateArea;
    Transformation masterTransformation = instance->getTransformation();
    instance->getTransformation().getInvert().applyOn(masterArea);
    transformation.applyOn(masterTransformation);
    drawPhantoms(instance->getMasterCell(), masterArea, masterTransformation);
}

void CellWidget::drawBoundaries(const Cell* cell, const H::Box& updateArea, const Transformation& transformation) const {
    drawRectangle(transformation.getBox(cell->getAbutmentBox()));
    for_each_instance(instance, cell->getInstances()) {
        drawBoundaries(instance, updateArea, transformation);
        end_for;
    }
}

void CellWidget::drawBoundaries(const Instance* instance, const H::Box& updateArea, const Transformation& transformation) const {
    H::Box masterArea = updateArea;
    Transformation masterTransformation = instance->getTransformation();
    instance->getTransformation().getInvert().applyOn(masterArea);
    transformation.applyOn(masterTransformation);
    drawBoundaries(instance->getMasterCell(), masterArea, masterTransformation);
}



// *************************************************************************************************
// CellWidget::drawLine()
// CellWidget::drawPolyline()
// CellWidget::drawRectangle()
// CellWidget::drawPolygon()
// CellWidget::drawText()
// *************************************************************************************************

void
CellWidget::drawLine(const Unit& xo,
           const Unit& yo,
           const Unit& xe,
           const Unit& ye) const
{
    if (_painter) {
    double dxo = getValue(xo);
    double dyo = getValue(yo);
    int co = getClipCode(dxo, dyo);
    double dxe = getValue(xe);
    double dye = getValue(ye);
    int ce = getClipCode(dxe, dye);
    if (clipLine(dxo, dyo, co, dxe, dye, ce)) {
        int ixo = getScreenX(getUnit(dxo));
        int iyo = getScreenY(getUnit(dyo));
        int ixe = getScreenX(getUnit(dxe));
        int iye = getScreenY(getUnit(dye));
        _painter->save();
        if (_painter->pen() == Qt::NoPen) {
        _painter->setPen(_painter->brush().color());
        }
            _painter->drawLine(ixo, iyo, ixe, iye);
        //_painter->moveTo(ixo, iyo);
        //_painter->lineTo(ixe, iye);
        _painter->restore();
    }
    }
}

//void
//CellWidget::drawPolyline(const Points& points)
//{
//    if (_painter) {
//    unsigned sCount;
//
//    if (clipLines(points, _sPoints, sCount, false)) {
//        _painter->save();
//        if (_painter->pen() == NoPen) {
//        _painter->setPen(_painter->brush().color());
//        }
//        _painter->drawPolyline(_sPoints, 0, sCount);
//        _painter->restore();
//    }
//    }
//}

void
CellWidget::drawRectangle(const Box& box) const
{
    if (_painter) {
    Box ibox = box.getIntersection(_clipBox);

    if (!ibox.isEmpty()) {
        int x = getScreenX(ibox.getXMin());
        int y = getScreenY(ibox.getYMax());
        int w = getScreenX(ibox.getXMax()) - x + 1;
        int h = getScreenY(ibox.getYMin()) - y + 1;
        _painter->drawRect(x, y, w, h);
    }
    }
}

//void
//CellWidget::drawPolygon(const Points& points)
//{
//    if (_painter) {
//    unsigned sCount;
//
//    if (clipLines(points, _sPoints, sCount, true)) {
//        _painter->drawPolygon(_sPoints, true, 0, sCount);
//    }
//    }
//}

//void
//CellWidget::drawText(const Box&       frame,
//           const string&    text,
//           const Direction& direction)
//{
//    if (_painter && !frame.isEmpty()) {
//    int x = getScreenX(frame.getXMin());
//    int y = getScreenY(frame.getYMax());
//    int w = getScreenX(frame.getXMax()) - x + 1;
//    int h = getScreenY(frame.getYMin()) - y + 1;
//    switch (direction) {
//        case NORTH: {
//        int ow = w;
//        w = h;
//        h = ow;
//        int ox = x;
//        x = - (y + w);
//        y = ox;
//        break;
//        }
//        /*
//        case WEST: {
//        x = - (x + w);
//        y = - (y + h);
//        break;
//        }
//        */
//        case SOUTH: {
//        int ow = w;
//        w = h;
//        h = ow;
//        int ox = x;
//        x = y;
//        y = - (ox + h);
//        break;
//        }
//    }
//    QFontMetrics metrics(_painter->font());
//    if ((metrics.width(text) <= w) && (metrics.height() <= h)) {
//        _painter->save();
//        switch (direction) {
//        case NORTH: _painter->rotate(-90); break;
//        /*
//        case WEST: _painter->rotate(180); break;
//        */
//        case SOUTH: _painter->rotate(90); break;
//        }
//        if (_painter->pen() == NoPen) {
//        _painter->setPen(_painter->brush().color());
//        }
//        _painter->drawText(x, y, w, h, Qt::AlignCenter, text);
//        _painter->restore();
//    }
//    }
//}
//


// *************************************************************************************************
// CellWidget::drawSnapPoint()
// *************************************************************************************************

void
CellWidget::drawSnapPoint()
{
    if (_drawSnapPoint) {
    int sx = getScreenX(_snapPoint.getX());
    int sy = getScreenY(_snapPoint.getY());

    if (((-3 <= sx) && (sx <= (width() + 3))) &&
        ((-3 <= sy) && (sy <= (height() + 3)))) {
        QPainter painter(this);
        painter.setPen(getForegroundColor());
        painter.setBrush(Qt::NoBrush);
        //painter.setRasterOp(Qt::XorROP);
        painter.drawRect(sx - 3, sy - 3, 7, 7);
    }
    }
}



// *************************************************************************************************
// CellWidget::resizeEvent()
// CellWidget::enterEvent()
// CellWidget::leaveEvent()
// CellWidget::mousePressEvent()
// CellWidget::mouseMoveEvent()
// CellWidget::mouseReleaseEvent()
// CellWidget::focusInEvent()
// CellWidget::focusOutEvent()
// CellWidget::paintEvent()
// *************************************************************************************************

void
CellWidget::resizeEvent(QResizeEvent* event)
{
    reframe();
}

//void
//CellWidget::enterEvent(QEvent* event)
//{
//    if (getView()) {
//    forEachCommand(command, getCommands()) {
//        command->onArm(this);
//        endFor;
//    }
//    }
//
//    if (_snapGrid) {
//    _drawSnapPoint = true;
//    drawSnapPoint();
//    }
//}
//
//void
//CellWidget::leaveEvent(QEvent* event)
//{
//    if (_snapGrid) {
//    drawSnapPoint();
//    _drawSnapPoint = false;
//    }
//
//    if (getView()) {
//    forEachCommand(command, getCommands()) {
//        command->onDisarm(this);
//        endFor;
//    }
//    }
//}

//void
//CellWidget::mousePressEvent(QMouseEvent* event)
//{
//    if (!_startedCommand) {
//    Point position = getSnapPoint(event->x(), event->y());
//    if (getView()) {
//        _startedCommand = getCommand(event->button());
//
//        if (_startedCommand) {
//        _startedCommand->onStart(this, position, event->state());
//        }
//    }
//    if (_trackBox) {
//        _trackBox->start(position);
//    }
//    }
//}
//
//void
//CellWidget::mouseMoveEvent(QMouseEvent* event)
//{
//    if (_startedCommand && automaticScrolling() && _startedCommand->automaticScrolling()) {
//    int sx = event->x();
//    int sy = event->y();
//    Unit dx = 0;
//    if (sx < 0) {
//        dx = getUnit(width() / 40);
//    }
//    else {
//        if (width() < sx) {
//        dx = - getUnit(width() / 40);
//        }
//    }
//    Unit dy = 0;
//    if (sy < 0) {
//        dy = - getUnit(height() / 40);
//    }
//    else {
//        if (height() < sy) {
//        dy = getUnit(height() / 40);
//        }
//    }
//    if (dx || dy) {
//        scroll(dx, dy);
//    }
//    }
//
//    Point position = getSnapPoint(event->x(), event->y());
//
//    if (position != _snapPoint) {
//    if (_snapGrid) {
//        drawSnapPoint();
//    }
//    _snapPoint = position;
//    if (_snapGrid) {
//        drawSnapPoint();
//    }
//    if (getView()) {
//        forEachCommand(command, getCommands()) {
//        command->onTrack(this, position);
//        endFor;
//        }
//
//        if (_startedCommand) {
//        _startedCommand->onUpdate(this, position, event->state());
//        }
//    }
//    if (_trackBox) {
//        _trackBox->update(position);
//    }
//    }
//}
//
//void
//CellWidget::mouseReleaseEvent(QMouseEvent* event)
//{
//    if (_startedCommand && (_startedCommand->getButton() == event->button())) {
//    Point position = getSnapPoint(event->x(), event->y());
//    if (getView()) {
//        _startedCommand->onStop(this, position);
//        _startedCommand = NULL;
//    }
//    if (_trackBox) {
//        _trackBox->stop(position);
//    }
//    }
//}
//
//void
//CellWidget::focusInEvent(QFocusEvent* event)
//{
//    // overriden to avoid repaint when the widget gains the focus
//}
//
//void
//CellWidget::focusOutEvent(QFocusEvent* event)
//{
//    // overriden to avoid repaint when the widget loose the focus
//}

void
CellWidget::paintEvent(QPaintEvent* event)
{
    invalidate(event->rect());

    redraw();
}



// *************************************************************************************************
// CellWidget::onSetVisible()
// *************************************************************************************************

void
CellWidget::onSetVisible(Layer* layer,
               bool   visible)
{
}



// *************************************************************************************************
// CellWidget:getClipCode()
// *************************************************************************************************

int
CellWidget::getClipCode(double x,
              double y) const
{
    if (x < _clipX[0]) {
    if (y < _clipY[0]) {
        return 28;
    }
    if (_clipY[3] < y) {
        return 22;
    }
    return 4;
    }

    if (_clipX[3] < x) {
    if (y < _clipY[0]) {
        return 25;
    }
    if (_clipY[3] < y) {
        return 19;
    }
    return 1;
    }

    if (y < _clipY[0]) {
    return 8;
    }

    if (_clipY[3] < y) {
    return 2;
    }

    return 0;
}



// *************************************************************************************************
// CellWidget::clipLine()
// CellWidget::clipLines()
// *************************************************************************************************

bool
CellWidget::clipLine(double& xo,
           double& yo,
           int     co,
           double& xe,
           double& ye,
           int     ce,
           int     u) const
{
    if (co & ce & -17) {
    return false;
    }

    if (!co & !ce) {
    return true;
    }

    if (co & 1) {
    yo += ((ye - yo) / (xe - xo)) * (_clipX[3] - xo);
    xo = _clipX[3];
    co = getClipCode(xo, yo);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (co & 2) {
    xo += ((xe - xo) / (ye - yo)) * (_clipY[3] - yo);
    yo = _clipY[3];
    co = getClipCode(xo, yo);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (co & 4) {
    yo += ((ye - yo) / (xe - xo)) * (_clipX[0] - xo);
    xo = _clipX[0];
    co = getClipCode(xo, yo);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (co & 8) {
    xo += ((xe - xo) / (ye - yo)) * (_clipY[0] - yo);
    yo = _clipY[0];
    co = getClipCode(xo, yo);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (ce & 1) {
    ye -= ((ye - yo) / (xe - xo)) * (xe - _clipX[3]);
    xe = _clipX[3];
    ce = getClipCode(xe, ye);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (ce & 2) {
    xe -= ((xe - xo) / (ye - yo)) * (ye - _clipY[3]);
    ye = _clipY[3];
    ce = getClipCode(xe, ye);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (ce & 4) {
    ye -= ((ye - yo) / (xe - xo)) * (xe - _clipX[0]);
    xe = _clipX[0];
    ce = getClipCode(xe, ye);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    if (ce & 8) {
    xe -= ((xe - xo) / (ye - yo)) * (ye - _clipY[0]);
    ye = _clipY[0];
    ce = getClipCode(xe, ye);
    return clipLine(xo, yo, co, xe, ye, ce, u + 1);
    }

    return true;
}

//bool
//CellWidget::clipLines(const Points& points,
//            QPointArray&  sPoints,
//            unsigned&     sCount,
//            bool          closed) const
//{
//    sCount = 0;
//
//    if (points.isEmpty()) {
//    return false;
//    }
//
//    int co, ce;
//    double xo, yo, xe, ye;
//    bool firstPoint = true;
//
//    forEachPoint(point, points) {
//    xe = getValue(point.getX());
//    ye = getValue(point.getY());
//    ce = getClipCode(xe, ye);
//    if (firstPoint) {
//        if (!ce) {
//        int sx = getScreenX(getUnit(xe));
//        int sy = getScreenY(getUnit(ye));
//        sPoints.putPoints(sCount++, 1, sx, sy);
//        }
//        firstPoint = false;
//    }
//    else {
//        static int tcc[] = {0,-3,-6,1,3,0,1,0,6,1,0,0,1,0,0,0};
//        static int cra[] = {-1,-1,-1,3,-1,-1,2,-1,-1,1,-1,-1,0,-1,-1,-1};
//
//        double xco = xo;
//        double yco = yo;
//        double xce = xe;
//        double yce = ye;
//        if (clipLine(xco, yco, co, xce, yce, ce)) {
//        if (co) {
//            int sx = getScreenX(getUnit(xco));
//            int sy = getScreenY(getUnit(yco));
//            sPoints.putPoints(sCount++, 1, sx, sy);
//        }
//        int sx = getScreenX(getUnit(xce));
//        int sy = getScreenY(getUnit(yce));
//        sPoints.putPoints(sCount++, 1, sx, sy);
//        }
//        else {
//        if (ce & 16) {
//            if (!(co & ce & ~16)) {
//            int c;
//            if (!(co & 16)) {
//                c = ce + tcc[co];
//            }
//            else {
//                bool done = false;
//                double xto = xo;
//                double yto = yo;
//                double xte = xe;
//                double yte = ye;
//                while (!done) {
//                double x = (xto + xte) / 2;
//                double y = (yto + yte) / 2;
//                c = getClipCode(x, y);
//                if (!(c & 16)) {
//                    if (c & ce) {
//                    c = ce + tcc[co & ~16];
//                    }
//                    else {
//                    c = co + tcc[ce & ~16];
//                    }
//                    done = true;
//                }
//                else {
//                    if (c == ce) {
//                    xte = x;
//                    yte = y;
//                    }
//                    else {
//                    if (c != co) {
//                        done = true;
//                    }
//                    else {
//                        xto = x;
//                        yto = y;
//                    }
//                    }
//                }
//                }
//            }
//            int sx = getScreenX(getUnit(_clipX[cra[c&~16]]));
//            int sy = getScreenY(getUnit(_clipY[cra[c&~16]]));
//            sPoints.putPoints(sCount++, 1, sx, sy);
//            }
//        }
//        else {
//            if (co & 16) {
//            if (!(co & ce)) ce = co + tcc[ce];
//            }
//            else {
//            ce |= co;
//            if (tcc[ce] == 1) ce |= 16;
//            }
//        }
//        }
//        if (ce & 16) {
//        int sx = getScreenX(getUnit(_clipX[cra[ce&~16]]));
//        int sy = getScreenY(getUnit(_clipY[cra[ce&~16]]));
//        sPoints.putPoints(sCount++, 1, sx, sy);
//        }
//    }
//    xo = xe;
//    yo = ye;
//    co = ce;
//    endFor;
//    }
//
//    if (closed && sCount) {
//    sPoints.putPoints(sCount++, 1, sPoints[0].x(), sPoints[0].y());
//    }
//
//    return sCount;
//}
//


// *************************************************************************************************
// CellWidget::_setStartedCommand()
// CellWidget::_setClipBox()
// *************************************************************************************************

//void
//CellWidget::_setStartedCommand(Command* command)
//{
//    _startedCommand = command;
//}

void
CellWidget::_setClipBox(const Box& area)
{
    _clipBox = Box(area).inflate(getSize(2));

    _clipX[0] = getValue(_clipBox.getXMin());
    _clipX[1] = getValue(_clipBox.getXMax());
    _clipX[2] = getValue(_clipBox.getXMin());
    _clipX[3] = getValue(_clipBox.getXMax());
    _clipY[0] = getValue(_clipBox.getYMin());
    _clipY[1] = getValue(_clipBox.getYMin());
    _clipY[2] = getValue(_clipBox.getYMax());
    _clipY[3] = getValue(_clipBox.getYMax());
}



// *************************************************************************************************
// CellWidget::_redraw()
// *************************************************************************************************

void
CellWidget::_redraw()
{
    if (!_invalidRegion.isEmpty()) {
    QRect invalidRect = _invalidRegion.boundingRect();

    if (_doubleBuffering) {
        _invalidRegion = invalidRect;
    }

    Box area = getBox(invalidRect).inflate(getSize(1));

    _setClipBox(area);

    QPaintDevice* device = this;

    if (_doubleBuffering) {
        //device = _getPixmap();
    }

    QPainter painter(device);
    QPainter* oldPainter = _painter;
    _painter = &painter;

    QRegion deepRegion;
        for (list<Box>::iterator plit = _peekList.begin();
                plit != _peekList.end();
                ++plit) {
        deepRegion = deepRegion.unite(QRegion(getScreenRect(*plit)));
        }
//    forEachBox(peek, _peekList) {
//        deepRegion = deepRegion.unite(QRegion(getScreenRect(peek)));
//        endFor;
//    }
    deepRegion = deepRegion.intersect(_invalidRegion);
    Box deepArea = getBox(deepRegion.boundingRect());

    _painter->setClipRegion(_invalidRegion);

    _painter->fillRect(invalidRect, QBrush(getBackgroundColor()));

    preRedraw(area);

    QRegion region = _invalidRegion.subtract(deepRegion);

    Cell* cell = getCell();
    if (cell) {


            _painter->save();


        double brightness = 1.0;
        //if (highlightionIsVisible() && !getHighlightion().isEmpty()) {
        //    brightness = 0.4;
        //}

            setPen(boundariesPen, brightness);
            setBrush(boundariesBrush, brightness);
            //drawBoundaries(cell, area, Transformation());


            for_each_basic_layer(layer, getVisibleLayers()) {
        if (isDrawable(layer)) {
            //setFont(getFont(layer));
            //setPen(layer->getPen(), brightness);
                    setPen(getPen(layer), brightness);
            //setBrush(layer->getBrush(), brightness);
                    setBrush(getBrush(layer), brightness);
            //setBrushOrigin(layer->getBrushOrigin());
            if (deepRegion.isEmpty()) {
                        drawContent(cell, layer, area, Transformation());
            //    view->_draw(this, layer, area, Transformation());
                    } else {
            _painter->setClipRegion(region);
                        drawContent(cell, layer, area, Transformation());
            //view->_draw(this, layer, area, Transformation());
            _painter->setClipRegion(deepRegion);
            unsigned displayDepth = _displayDepth;
            _displayDepth = (unsigned)-1;
            //view->_draw(this, layer, deepArea, Transformation());
                        drawContent(cell, layer, deepArea, Transformation());
            _displayDepth = displayDepth;
            _painter->setClipRegion(_invalidRegion);
            }
            //_painter->flush();
        }
        end_for;
        }
//        if (highlightionIsVisible() && !getHighlightion().isEmpty()) {
//        Technology* technology = getDatabase()->getTechnology();
//        if (technology) {
//            forEachLayer(layer, technology->getLayers()) {
//            if (isDrawable(layer)) {
//                setFont(layer->getFont());
//                setPen(layer->getPen());
//                setBrush(layer->getBrush());
//                setBrushOrigin(layer->getBrushOrigin());
//                forEachOccurrence(occurrence, getHighlightion()) {
//                Figure* figure = dynamic_cast<Figure*>(occurrence.getEntity());
//                if (figure &&
//                    figure->isMaterialized() &&
//                    (figure->getLayerMask() & layer->getMask())) {
//                    Box boundingBox = occurrence.getBoundingBox();
//                    if (boundingBox.overlaps(area)) {
//                    Transformation transformation =
//                        occurrence.getPath().getTransformation();
//                    Box correctedArea =
//                        transformation.getInverted().getBox(area);
//                    figure->_draw(this, layer, correctedArea, transformation);
//                    }
//                }
//                endFor;
//                }
//                _painter->flush();
//            }
//            endFor;
//            }
//        }
//        }
//        if (selectionIsVisible()) {
//        QColor selectionColor = getSelectionColor();
//        setPen(selectionColor);
//        setBrush(QBrush(selectionColor, Dense4Pattern));
//        setBrushOrigin(QPoint(0, 0));
//        forEachOccurrence(occurrence, getSelection()) {
//            Figure* figure = dynamic_cast<Figure*>(occurrence.getEntity());
//            if (figure) {
//            Box boundingBox = occurrence.getBoundingBox();
//            if (boundingBox.overlaps(area)) {
//                figure->_highlight(this, occurrence.getPath().getTransformation());
//            }
//            }
//            endFor;
//        }
//        }
        _painter->restore();
        if (gridIsVisible() && gridIsDrawable()) {
        Unit xMin = getGridX(getX(0));
        Unit yMin = getGridY(getY(height()));
        Unit xMax = getGridX(getX(width()));
        Unit yMax = getGridY(getY(0));
        setPen(getGridColor());
        for (Unit x = xMin; x <= xMax; x += _gridXStep) {
            int sx = getScreenX(x);
            if (_gridAxesAreVisible && (x == getGridOrigin().getX())) {
            //_painter->moveTo(sx, 0);
            //_painter->lineTo(sx, height());
                        _painter->drawLine(sx, 0, sx, height());
            }
            for (Unit y = yMin; y <= yMax; y += _gridYStep) {
            int sy = getScreenY(y);
            if (_gridAxesAreVisible && (y == getGridOrigin().getY())) {
                //_painter->moveTo(0, sy);
                //_painter->lineTo(width(), sy);
                            _painter->drawLine(0, sy, width(), sy);
            }
            if (!(isOnGridX(x, 10) && isOnGridY(y, 10))) {
                _painter->drawPoint(sx, sy);
            }
            else {
                //_painter->moveTo(sx - 2, sy);
                //_painter->lineTo(sx + 2, sy);
                            _painter->drawLine(sx - 2, sy, sx + 2, sy);
                //_painter->moveTo(sx, sy - 2);
                //_painter->lineTo(sx, sy + 2);
                            _painter->drawLine(sx, sy - 2, sx, sy + 2);
            }
            }
        }
        }
    }

    postRedraw(area);

    if (device != this) {
        int x = invalidRect.x();
        int y = invalidRect.y();
        int w = invalidRect.width();
        int h = invalidRect.height();
        //bitBlt(this, x, y, device, x, y, w, h, CopyROP, true);
    }

    _setClipBox(getBox(rect()));

    _painter = oldPainter;

    _invalidRegion = QRegion();
    }
}



// *************************************************************************************************
// CellWidget::_callDrawOf()
// *************************************************************************************************

//void
//CellWidget::_callDrawOf(Command* command)
//{
//    assert(command);
//    assert(command->isInstalledOn(this));
//
//    if (getView()) {
//    QPainter painter(this);
//
//    _painter = &painter;
//
//    _painter->setRasterOp(XorROP);
//    _painter->setPen(getForegroundColor());
//    _painter->setBrush(NoBrush);
//
//    command->onDraw(this);
//    }
//}
//


// *************************************************************************************************
// CellWidget::_insertCommand()
// CellWidget::_removeCommand()
// *************************************************************************************************

//void
//CellWidget::_insertCommand(Command* command)
//{
//    _commandMap[command->getButton()] = command;
//}
//
//void
//CellWidget::_removeCommand(Command* command)
//{
//    _commandMap.erase(command->getButton());
//}
//
//
//
//// *************************************************************************************************
//// CellWidget::_insertSelector()
//// CellWidget::_removeSelector()
//// *************************************************************************************************
//
//void
//CellWidget::_insertSelector(Selector* selector)
//{
//    _selectorSet.insert(selector);
//}
//
//void
//CellWidget::_removeSelector(Selector* selector)
//{
//    _selectorSet.erase(selector);
//}
//
//
//
//// *************************************************************************************************
//// CellWidget::_insertHighlightor()
//// CellWidget::_removeHighlightor()
//// *************************************************************************************************
//
//void
//CellWidget::_insertHighlightor(Highlightor* highlightor)
//{
//    _highlightorSet.insert(highlightor);
//}
//
//void
//CellWidget::_removeHighlightor(Highlightor* highlightor)
//{
//    _highlightorSet.erase(highlightor);
//}
//


//CLOSE_MY_NAMESPACE

// *************************************************************************************************
// *************************************************************************************************
