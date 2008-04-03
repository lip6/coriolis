// *************************************************************************************************
// *************************************************************************************************
//   File:	CellWidget.h
// *************************************************************************************************
// *************************************************************************************************

#ifndef H_CellWidget
#define H_CellWidget

#include <QWidget>

#include "Box.h"
//#include "Commands.h"
#include "Slice.h"
#include "Segment.h"
#include "Contact.h"
#include "Layers.h"
#include "BasicLayer.h"
#include "Occurrences.h"
#include "Points.h"
using namespace Hurricane;
//#include "Selectors.h"

//OPEN_MY_NAMESPACE

//class View;
//class TrackBox;


// *************************************************************************************************
// CellWidget
// *************************************************************************************************

class /* Q_EXPORT */ CellWidget : public QWidget {

  Q_OBJECT

  public:
    				~CellWidget();

            Cell*                       getCell() const;
     //View*			getView() const ;
	    const Point&		getCenter() const;
	    double			getScale() const;
	    Unit			getX(int screenX) const;
	    Unit			getY(int screenY) const;
	    Unit			getSize(int screenSize) const;
	    Point			getPoint(const QPoint& screenPoint) const;
	    Box				getBox(const QRect& screenRect) const;
	    int				getScreenX(const Unit& x) const;
	    int				getScreenY(const Unit& y) const;
	    int				getScreenSize(const Unit& size) const;
	    QPoint			getScreenPoint(const Point& point) const;
	    QRect			getScreenRect(const Box& box) const;
	    Point			getSnapPoint(const Point& point) const;
	    Point			getSnapPoint(int screenX, int screenY) const;
	    const QColor&		getBackgroundColor() const;
	    const QColor&		getForegroundColor() const;
	    const Layer::Mask&		getVisibleLayerMask() const;
	    BasicLayers			getVisibleLayers() const;
	    //Command*			getCommand(int button) const;
	    //Command*			getStartedCommand() const;
	    //Commands			getCommands() const;
	    const Point&		getGridOrigin() const;
	    const Unit&			getGridXStep() const;
	    const Unit&			getGridYStep() const;
	    unsigned			getGridDisplayThreshold() const;
	    const QColor&		getGridColor() const;
	    Unit			getGridX(const Unit& x,
						 int         sign = 0) const;
	    Unit			getGridY(const Unit& y,
						 int         sign = 0) const;
	    Point			getGridPoint(const Point& point,
						     int          xSign = 0,
						     int          ySign = 0) const;
	    unsigned			getDisplayDepth() const;
	    unsigned			getDisplaySize() const;
	    Occurrences			getSelection() const;
	    Box				getSelectionBox() const;
	    unsigned			getSelectionLimit() const;
	    const QColor&		getSelectionColor() const;
	    Occurrences			getHighlightion() const;
	    Box				getHighlightionBox() const;
	    unsigned			getHighlightionLimit() const;
	    const QFont&		getFont();
	    const QPen&			getPen();
            const QPen&                 getPen(const BasicLayer* layer);
	    const QBrush&		getBrush();
            const QBrush&               getBrush(BasicLayer* layer);
	    const QPoint		getBrushOrigin();
	    //TrackBox*			getTrackBox() const;

	    bool			doubleBuffering() const;
	    bool			automaticScrolling() const;
	    bool			isVisible(BasicLayer* layer) const;
	    bool			isDrawable(BasicLayer* layer) const;
	    bool			gridIsVisible() const;
	    bool			gridAxesAreVisible() const;
	    bool			gridIsDrawable() const;
	    bool			isOnGridX(const Unit& x,
						  unsigned    n = 1) const;
	    bool			isOnGridY(const Unit& y,
						  unsigned    n = 1) const;
	    bool			isOnGridPoint(const Point& point,
						      unsigned     xN = 1,
						      unsigned     yN = 1) const;
	    bool			snapGrid() const;
	    bool			selectionIsVisible() const;
	    bool			isSelected(const Occurrence& occurrence) const;
	    bool			isSelectable(const Occurrence& occurrence) const;
	    bool			highlightionIsVisible() const;
	    bool			isHighlighted(const Occurrence& occurrence) const;
	    bool			isHighlightable(const Occurrence& occurrence) const;

	    void			setBackgroundColor(const QColor& color);
	    void			setForegroundColor(const QColor& color);
	    void			setDoubleBuffering(bool enable);
	    void			setAutomaticScrolling(bool enable);
	    void			setVisible(BasicLayer* layer,
						   bool   visible);
	    void			setGridOrigin(const Point& origin);
	    void			setGridXStep(const Unit& step);
	    void			setGridYStep(const Unit& step);
	    void			setGridStep(const Unit& step);
	    void			setGridSteps(const Unit& xStep,
						     const Unit& yStep);
	    void			setGridDisplayThreshold(unsigned threshold);
	    void			setGridColor(const QColor& color);
	    void			setGridVisible(bool visible);
	    void			setGridAxesVisible(bool visible);
	    void			setSnapGrid(bool enable);
	    void			setDisplayDepth(unsigned displayDepth);
	    void			setDisplaySize(unsigned displaySize);
	    void			setSelectionLimit(unsigned limit);
	    void			setSelectionColor(const QColor& color);
	    void			setSelectionVisible(bool visible);
	    void			setHighlightionLimit(unsigned limit);
	    void			setHighlightionVisible(bool visible);
	    void			setFont(const QFont& font);
	    void			setPen(const QPen& pen,
					       double      brightness = 1.0);
	    void			setBrush(const QBrush& brush,
						 double        brightness = 1.0);
	    void			setBrushOrigin(const QPoint& origin);
	    //void			setTrackBox(TrackBox* trackBox);

	    void			showAllLayers();
	    void			hideAllLayers();
	    bool			select(const Occurrence& occurrence);
	    void			unselect(const Occurrence& occurrence);
	    //void			clearSelection();
	    //bool			highlight(const Occurrence& occurrence);
	    //void			unhighlight(const Occurrence& occurrence);
	    //void			clearHighlightion();
	    void			clearPeeks();
	    void			addPeek(const Box& box);

	    void			invalidate();
	    void			invalidate(const QRect& screenRect);
	    void			invalidate(const Box& box);
            void			fitToContent(unsigned screenMargin = 5) ;
	    void			reframe(double scale);
	    void			reframe(const Point& center);
	    void			reframe(const Point& center,
						double       scale);
	    void			reframe(const Box& box);
	    void			scroll(const Unit& dx,
					       const Unit& dy);
	    void			redraw();
	    void			abortStartedCommand();


            void drawContent(const Cell* cell, const BasicLayer* basicLayer, const Hurricane::Box& updateArea, const Transformation& transformation) const;
            void drawContent(const Instance* instance, const BasicLayer* basicLayer, const Hurricane::Box& updateArea, const Transformation& transformation) const;
            void drawSlice(const Slice* slice, const BasicLayer* basicLayer, const Hurricane::Box& updateArea, const Transformation& transformation) const;
            void drawPhantoms(const Cell* cell, const Box& updateArea, const Transformation& transformation) const;
            void drawPhantoms(const Instance* instance, const Box& updateArea, const Transformation& transformation) const;
            void drawBoundaries(const Cell* cell, const Box& updateArea, const Transformation& transformation) const;
            void drawBoundaries(const Instance* instance, const Box& updateArea, const Transformation& transformation) const;
            void drawGo(const Go* go, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;
            void drawSegment(const Segment* segment, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;
            void drawContact(const Contact* contact, const BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation) const;

	    void			drawLine(const Unit& xo,
						 const Unit& yo,
						 const Unit& xe,
						 const Unit& ye) const;
	    void			drawLine(const Point& origin,
						 const Point& extremity) const;
	    void			drawPolyline(const Points& points);

	    void			drawRectangle(const Unit& xMin,
						      const Unit& yMin,
						      const Unit& xMax,
						      const Unit& yMax) const;
	    void			drawRectangle(const Box& box) const;
	    void			drawPolygon(const Points& points);
	    //void			drawText(const Box&       frame,
	    //    				 const string&    text,
	    //    				 const Direction& direction);
	    //void			drawText(const Box&       frame,
//						 const Name&      name,
//						 const Direction& direction);
#ifndef DOXYGEN
  public:
    typedef QWidget			Inherit;

     //Label			getLabel() const;
     Record*			getRecord() const;
     Slot*			getSlot(const string& name) const;
	    Box				getWorld() const;
	    //Selectors			getSelectors() const;
	    //Highlightors		getHighlightors() const;

     bool			hasRecord() const;
     bool			allowAutomaticScrolling() const;

	    void			reframe();

     void			preRedraw(const Box& area);
     void			postRedraw(const Box& area);

	    void			drawSnapPoint();

					CellWidget(Cell* cell,
                                                QWidget*    parent = NULL,
						 const char* name = NULL);

  protected:

     void			resizeEvent(QResizeEvent* event);
     //void			enterEvent(QEvent* event);
     //void			leaveEvent(QEvent* event);
     //void			mousePressEvent(QMouseEvent* event);
     //void			mouseMoveEvent(QMouseEvent* event);
     //void			mouseReleaseEvent(QMouseEvent* event);
     //void			focusInEvent(QFocusEvent* event);
     //void			focusOutEvent(QFocusEvent* event);
     void			paintEvent(QPaintEvent* event);

     void			onSetVisible(Layer* layer,
						     bool   visible);

  private:
					CellWidget(const CellWidget& porthole);
					// not implemented to forbid copy

	    CellWidget&			operator=(const CellWidget& porthole);
					// not implemented to forbid assignment

	    int				getClipCode(double x,
						    double y) const;
	    bool			clipLine(double& xo,
						 double& yo,
						 int     co,
						 double& xe,
						 double& ye,
						 int     ce,
						 int     u = 0) const;
//	    bool			clipLines(const Points& points,
//						  QPointArray&  sPoints,
//						  unsigned&     sCount,
//						  bool          closed = false) const;
//
//    typedef map<int, Command*>		CommandMap;
//    typedef set<Selector*>		SelectorSet;
//    typedef set<Highlightor*>		HighlightorSet;
    typedef list<Box>			PeekList;

            Cell*                       _cell;
	    Point			_center;
	    double			_scale;
	    int				_screenDx;
	    int				_screenDy;
	    int				_brushDx;
	    int				_brushDy;
	    bool			_alreadyExposed;
	    QRegion			_invalidRegion;
	    Box				_clipBox;
	    double			_clipX[4];
	    double			_clipY[4];
	    QPainter*			_painter;
	    QColor			_backgroundColor;
	    QColor			_foregroundColor;
	    bool			_doubleBuffering;
	    bool			_automaticScrolling;
            Layer::Mask			_visibleLayerMask;
	    //CommandMap			_commandMap;
	    //Command*			_startedCommand;
	    Point			_gridOrigin;
	    Unit			_gridXStep;
	    Unit			_gridYStep;
	    bool			_gridIsVisible;
	    bool			_gridAxesAreVisible;
	    unsigned 			_gridDisplayThreshold;
	    QColor			_gridColor;
	    bool			_snapGrid;
	    bool			_drawSnapPoint;
	    Point			_snapPoint;
	    unsigned			_displayDepth;
	    unsigned			_displaySize;
	    //SelectorSet			_selectorSet;
	    unsigned			_selectionLimit;
	    QColor			_selectionColor;
	    bool			_selectionIsVisible;
	    //HighlightorSet		_highlightorSet;
	    unsigned			_highlightionLimit;
	    bool			_highlightionIsVisible;
    //static  QPointArray			_sPoints;
	    //TrackBox*			_trackBox;
	    PeekList			_peekList;
            map<const BasicLayer*, QBrush> _basicLayersBrush;
            map<const BasicLayer*, QPen> _basicLayersPen;

  protected:
  //restricted:
     //QPixmap*			_getPixmap() const ;

	    //void			_setStartedCommand(Command* command);
	    //void			_setTrackBox(TrackBox* trackBox);
	    void			_setClipBox(const Box& area);

	    void			_redraw();
	    //void			_callDrawOf(Command* command);

	   // void			_insertCommand(Command* command);
	   // void			_removeCommand(Command* command);
	   // void			_insertSelector(Selector* selector);
	   // void			_removeSelector(Selector* selector);
	   // void			_insertHighlightor(Highlightor* highlightor);
	   // void			_removeHighlightor(Highlightor* highlightor);
#endif // DOXYGEN

};



// *************************************************************************************************
// getLabel()
// getRecord()
// getSlot()
// *************************************************************************************************

#ifndef DOXYGEN
//inline
//Label
//getLabel(const CellWidget& porthole)
//{
//    return porthole.getLabel();
//}
//
//inline
//Label
//getLabel(const CellWidget* porthole)
//{
//    return (!porthole) ? Label("nil") : porthole->getLabel();
//}
//
//inline
//Record*
//getRecord(const CellWidget& porthole)
//{
//    return porthole.getRecord();
//}
//
//inline
//Record*
//getRecord(const CellWidget* porthole)
//{
//    return (!porthole) ? NULL : porthole->getRecord();
//}
//
//inline
//Slot*
//getSlot(const string&   name,
//	const CellWidget& porthole)
//{
//    return porthole.getSlot(name);
//}
//
//inline
//Slot*
//getSlot(const string&   name,
//	const CellWidget* porthole)
//{
//    return (!porthole) ? getSlot(name, getLabel("nil")) : porthole->getSlot(name);
//}
#endif // DOXYGEN



// *************************************************************************************************
// hasRecord()
// *************************************************************************************************

#ifndef DOXYGEN
//inline
//bool
//hasRecord(const CellWidget& porthole)
//{
//    return porthole.hasRecord();
//}
//
//inline
//bool
//hasRecord(const CellWidget* porthole)
//{
//    return (!porthole) ? false : porthole->hasRecord();
//}
#endif // DOXYGEN



// *************************************************************************************************
// operator<<()
// *************************************************************************************************

#ifndef DOXYGEN
//inline
//ostream&
//operator<<(ostream&        stream,
//	   const CellWidget& porthole)
//{
//    return stream << porthole.getLabel();
//}
//
//inline
//ostream&
//operator<<(ostream&        stream,
//	   const CellWidget* porthole)
//{
//    return (!porthole) ? (stream << "nil") : (stream << "&" << porthole->getLabel());
//}
#endif // DOXYGEN



// *************************************************************************************************
// CellWidget::getBackgroundColor()
// CellWidget::getForegroundColor()
// CellWidget::getStartedCommand()
// CellWidget::getGridOrigin()
// CellWidget::getGridXStep()
// CellWidget::getGridYStep()
// CellWidget::getGridDisplayThreshold()
// CellWidget::getGridColor()
// CellWidget::getDisplayDepth()
// CellWidget::getDisplaySize()
// CellWidget::getSelectionColor()
// CellWidget::getSelectionLimit()
// CellWidget::getHighlightionLimit()
// CellWidget::getTrackBox()
// *************************************************************************************************

inline
const QColor&
CellWidget::getBackgroundColor() const
{
    return _backgroundColor;
}

inline
const QColor&
CellWidget::getForegroundColor() const
{
    return _foregroundColor;
}

inline
const Layer::Mask&
CellWidget::getVisibleLayerMask() const
{
    return _visibleLayerMask;
}

//
//inline
//Command*
//CellWidget::getStartedCommand() const
//{
//    return _startedCommand;
//}

inline
const Point&
CellWidget::getGridOrigin() const
{
    return _gridOrigin;
}

inline
const Unit&
CellWidget::getGridXStep() const
{
    return _gridXStep;
}

inline
const Unit&
CellWidget::getGridYStep() const
{
    return _gridYStep;
}

inline
unsigned
CellWidget::getGridDisplayThreshold() const
{
    return _gridDisplayThreshold;
}

inline
const QColor&
CellWidget::getGridColor() const
{
    return _gridColor;
}

inline
unsigned
CellWidget::getDisplayDepth() const
{
    return _displayDepth;
}

inline
unsigned
CellWidget::getDisplaySize() const
{
    return _displaySize;
}

inline
const QColor&
CellWidget::getSelectionColor() const
{
    return _selectionColor;
}

inline
unsigned
CellWidget::getSelectionLimit() const
{
    return _selectionLimit;
}

inline
unsigned
CellWidget::getHighlightionLimit() const
{
    return _highlightionLimit;
}

//inline
//TrackBox*
//CellWidget::getTrackBox() const
//{
//    return _trackBox;
//}



// *************************************************************************************************
// CellWidget::doubleBuffering()
// CellWidget::automaticScrolling()
// CellWidget::gridIsVisible()
// CellWidget::gridAxesAreVisible()
// CellWidget::gridIsDrawable()
// CellWidget::snapGrid()
// CellWidget::selectionIsVisible()
// CellWidget::highlightionIsVisible()
// *************************************************************************************************

inline
bool
CellWidget::doubleBuffering() const
{
    return _doubleBuffering;
}

inline
bool
CellWidget::automaticScrolling() const
{
    return _automaticScrolling;
}

inline
bool
CellWidget::gridIsVisible() const
{
    return _gridIsVisible;
}

inline
bool
CellWidget::gridAxesAreVisible() const
{
    return _gridAxesAreVisible;
}

inline
bool
CellWidget::gridIsDrawable() const
{
    return ((int)_gridDisplayThreshold < getScreenSize(min(_gridXStep, _gridYStep)));
}

inline
bool
CellWidget::snapGrid() const
{
    return _snapGrid;
}

inline
bool
CellWidget::selectionIsVisible() const
{
    return _selectionIsVisible;
}

inline
bool
CellWidget::highlightionIsVisible() const
{
    return _highlightionIsVisible;
}



// *************************************************************************************************
// CellWidget::setDoubleBuffering()
// CellWidget::setSelectionLimit()
// CellWidget::setHighlightionLimit()
// *************************************************************************************************

inline
void
CellWidget::setDoubleBuffering(bool enable)
{
    _doubleBuffering = enable;
}

inline
void
CellWidget::setSelectionLimit(unsigned limit)
{
    _selectionLimit = limit;
}

inline
void
CellWidget::setHighlightionLimit(unsigned limit)
{
    _highlightionLimit = limit;
}



// *************************************************************************************************
// CellWidget::drawLine()
// CellWidget::drawRectangle()
// CellWidget::drawText()
// *************************************************************************************************

inline
void
CellWidget::drawLine(const Point& origin,
		   const Point& extremity) const
{
    drawLine(origin.getX(), origin.getY(), extremity.getX(), extremity.getY());
}

inline
void
CellWidget::drawRectangle(const Unit& xMin,
			const Unit& yMin,
			const Unit& xMax,
			const Unit& yMax) const
{
    drawRectangle(Box(xMin, yMin, xMax, yMax));
}

//inline
//void
//CellWidget::drawText(const Box&       frame,
//		   const Name&      name,
//		   const Direction& direction)
//{
//    drawText(frame, name.getString(), direction);
//}



// *************************************************************************************************
// CellWidget::_setTrackBox()
// *************************************************************************************************

//#ifndef DOXYGEN
//inline
//void
//CellWidget::_setTrackBox(TrackBox* trackBox)
//{
//    _trackBox = trackBox;
//}
//#endif // DOXYGEN
//


// *************************************************************************************************
// File documentation
// *************************************************************************************************

/*!\file
   \brief	This file contains the definition of the CellWidget class.
   \paragraph	Concept of CellWidget:
	\nn	A porthole is a Qt widget used to display a cell view.
	\nn	When a porthole is bound to a given cell view the content of this cell view is
		displayed according to some porthole characteristics (zoom factor, grid, visible
		layers, selection, highlightion, ...).
	\nn	The display reflects exactly the current state of the cell view (there isn't a
		parallel display list).
	\nn	A porthole can display one and only one cell view, but a cell view can be displayed
		simultaneously in multiple portholes. The fact that each porthole has its own
		characteristics provides the capability to show different aspects of the same cell
		view within separated portholes.
   \paragraph	Concept of MainCellWidget:
	\nn	The MainCellWidget class implements the concept of main porthole.
	\nn	The porthole class being an abstract class, to display a cell view you should create
		a main porthole (or a specialization) and insert this widget in your graphic
		interface (as all other widgets).
	\nn	Furthermore, you can attach to each main porthole a map porthole used to visualize
		which part of the cell view is currently drawn in the main porthole and, can attach
		a palette used to control the visibility of each layer and you can also two main
		scroll bars.
   \paragraph	Concept of MapCellWidget:
	\nn	The MapCellWidget class implements the concept of map porthole.
	\nn	A map porthole is used to visualize which part of the cell view is currently
		displayed by a main porthole.
   \see		View, MainCellWidget, MapCellWidget, Palette or Command.
   \author	\remye
   */



// *************************************************************************************************
// CellWidget documentation
// *************************************************************************************************

/*!\class	CellWidget
   \brief	This abstract class introduces the concept of porthole
   \concept	A porthole is a Qt widget used to display a cell view.
	\nn	When a porthole is bound to a given cell view the content of this cell view is
		displayed according to some porthole characteristics (zoom factor, grid, visible
		layers, selection, highlightion, ...) as we will discover in the following.
	\nn	The display reflects exactly the current state of the cell view (there isn't a
		parallel display list).
	\nn	A porthole can display one and only one cell view, but a cell view can be displayed
		simultaneously in multiple portholes. The fact that each porthole has its own
		characteristics provides the capability to show different aspects of the same cell
		view within separated portholes.
   \paragraph	Specialization:
	\nn	The database provides two kind of portholes: the MainCellWidget which is used to
		display the totality or some part of a cell view; and the MapCellWidget which is
		associated to a main porthole to show which part of the cell view is currently
		displayed in this main porthole.
	\nn	Note that, if you need to create a new kind of porthole you should inherit of one
		of those types and not directly of the base class.
   \paragraph	Selection:
	\nn	Each porthole manages a collection of selected occurrences. The selection being
		managed by portholes and not by cell views it's possible to have differents
		selections for differents portholes bound to the same cell view.
	\nn	The selection is managed with the help of occurrence properties which is a very
		secure mechanism. So, when an occurrence is no longer valid, this occurrence is
		automaticaly unselected from each porthole it was selected in.
   \paragraph	Highlightion:
	\nn	Each porthole manages a collection of highlighted occurrences. The highlightion
		being managed by portholes and not by cell views it's possible to have differents
		highlightions for differents portholes bound to the same cell view.
	\nn	The highlightion is managed with the help of occurrence properties which is a very
		secure mechanism. So, when an occurrence is no longer valid, this occurrence is
		automaticaly unhighlighted from each porthole it was highlighted in.
   \paragraph	Commands:
	\nn	The database provides some command facilities. A command is an object which models
		the behaviour to be obtained in reaction to mouse events within a porthole.
	\nn	Furthermore, coordinates which are manipulated by commands are cell view
		coordinates and not screen coordinates (the translation being done by the porthole).
	\nn	%Commands are very powerfull objects which make the task more easier by managing
		automaticaly important features of portholes like double buffering, automatic
		scrolling, synchronization of the xor more when drawing ghost shapes...
   \paragraph	Palette:
	\nn	A palette can be used to control the visibility of the layers in a given main
		porthole.
   \paragraph	Main scroll bars:
	\nn	You can also attach to each main porthole an horizontal and/or a vertical scroll
		bar.
   \paragraph	Updates:
	\nn	A lot of functions provided by this class change the characteristics of the display.
	\nn	So, to avoid useless intermediate drawings or screen flickerings those functions
		don't update the display immediately but post some invalidation regions which are
		cumulated until a real request of redraw is asked.
	\nn	Furthermore, when a redraw is asked, something is done only when the invalidated
		region isn't empty.
   \paragraph	Drawing:
	\nn	The CellWidget class provides some convenience functions to draw shapes which are
		defined in cell view coordinates (and not in screen ones) according to the center,
		scale and clipping region.
	\nn	So, no need to manage screen transformations, clipping or something like that. No
		need to manage colors, pens, brushes or something else because all is controled by
		the porthole which know when it is drawing a given layer or when it is drawing a
		command ghost (in xor mode).
   \paragraph	Usage:
	\nn	The following piece of code illustrate the manner to build a very simple graphical
		interface using a main porthole, a map porthole, two main scroll bars and a palette.
		Furthermore, some commands are installed in the different portholes. The returned
		editor being a widget it can be embedded in another parent widget which can add, for
		example, a menu bar, a status bar or some additionnal decorations...
	\code
		* QWidget* getEditor(View* view)
		* {
		*     QHBox* editor = new QHBox();
		*
		*     QFrame* mainFrame = new QFrame(editor);
		*     mainFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		*     mainFrame->setLineWidth(2);
		*     mainFrame->setMinimumSize(120, 120);
		*     editor->setStretchFactor(mainFrame, 1);
		*
		*     QGridLayout* mainLayout = new QGridLayout(mainFrame, 2, 2);
		*     mainLayout->setMargin(2);
		*
		*     MainCellWidget* mainCellWidget = new MainCellWidget(mainFrame);
		*     mainLayout->addWidget(mainCellWidget, 0, 0);
		*
		*     MainScrollBar* hScrollBar = new MainScrollBar(mainFrame);
		*     hScrollBar->setOrientation(Qt::Horizontal);
		*     mainLayout->addWidget(hScrollBar, 1, 0);
		*
		*     MainScrollBar* vScrollBar = new MainScrollBar(mainFrame);
		*     vScrollBar->setOrientation(Qt::Vertical);
		*     mainLayout->addWidget(vScrollBar, 0, 1);
		*
		*     QVBox* vBox = new QVBox(editor);
		*     editor->setStretchFactor(vBox, 0);
		*     vBox->setMinimumWidth(120);
		*     vBox->setMaximumWidth(120);
		*     vBox->setSpacing(5);
		*
		*     QFrame* mapFrame = new QFrame(vBox);
		*     mapFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		*     mapFrame->setLineWidth(2);
		*     mapFrame->setMinimumSize(120, 120);
		*     mapFrame->setMaximumSize(120, 120);
		*
		*     QGridLayout* mapLayout = new QGridLayout(mapFrame, 1, 1);
		*     mapLayout->setMargin(2);
		*
		*     MapCellWidget* mapCellWidget = new MapCellWidget(mapFrame);
		*     mapLayout->addWidget(mapCellWidget, 0, 0);
		*
		*     Palette* palette = new Palette(vBox);
		*
		*     mainCellWidget->setView(view);
		*     mainCellWidget->setMapCellWidget(mapCellWidget);
		*     mainCellWidget->setPalette(palette);
		*     mainCellWidget->setHScrollBar(hScrollBar);
		*     mainCellWidget->setVScrollBar(vScrollBar);
		*
		*     mapCellWidget->hideAllLayers();
		*     Layer* phantom = getDatabase()->getTechnology()->getLayer("PHANTOM");
		*     if (phantom) {
		*         mapCellWidget->setVisible(phantom, true);
		*     }
		*
		*     getZoomCommand().installOn(mainCellWidget);
		*     getSelectCommand().installOn(mainCellWidget);
		*     getPanCommand().installOn(mainCellWidget);
		*
		*     getZoomCommand().installOn(mapCellWidget);
		*     getPanCommand().installOn(mapCellWidget);
		*
		*     editor->resize(700, 500);
		*     editor->setMargin(5);
		*     editor->setSpacing(5);
		*     editor->show();
		*
		*     return editor;
		* }
		\endcode
	\nn
   \see		View, MainCellWidget, MapCellWidget, MainScrollBar, Palette or Command.
   \author	\remye
   */

/*!\function	CellWidget::~CellWidget()
   \behaviour	This is the destructor.
   */

//@{
/*!\function	View* CellWidget::getView() const
   \behaviour	This function returns the view displayed in this porthole.
   */

/*!\function	const Point& CellWidget::getCenter() const
   \behaviour	This function returns the center of this porthole.
	\nn	This center is in fact the point of the cell view which is located on the center of
		this porthole.
   \note	Returns the point \c [0,0] if the porthole isn't bound to a cell view.
   */

/*!\function	double CellWidget::getScale() const
   \behaviour	This function returns the scale used by this porthole to display its contents.
   \note	Returns \c 1.0 if the porthole isn't bound to a cell view.
   */

/*!\function	Unit CellWidget::getX(int screenX) const
   \behaviour	This function returns the cell view abscissa corresponding to a screen abscissa.
   \parameter	screenX	The screen abscissa for which the request is asked.
   */

/*!\function	Unit CellWidget::getY(int screenY) const
   \behaviour	This function returns the cell view ordinate corresponding to a screen ordinate.
   \parameter	screenY	The screen ordinate for which the request is asked.
   */

/*!\function	Unit CellWidget::getSize(int screenSize) const
   \behaviour	This function returns the cell view dimension corresponding to a screen dimension.
   \parameter	screenSize	The screen dimension for which the request is asked.
   */

/*!\function	Point CellWidget::getPoint(const QPoint& screenPoint) const
   \behaviour	This function returns the cell view location corresponding to a screen location.
   \parameter	screenPoint	The screen location for which the request is asked.
   */

/*!\function	Box CellWidget::getBox(const QRect& screenRect) const
   \behaviour	This function returns the cell view box corresponding to a screen rectangle.
   \parameter	screenRect	The screen rectangle for which the request is asked.
   */

/*!\function	int CellWidget::getScreenX(const Unit& x) const
   \behaviour	This function returns the screeen abscissa corresponding to a cell view abscissa.
   \parameter	x	The cell view abscissa for which the request is asked.
   */

/*!\function	int CellWidget::getScreenY(const Unit& y) const
   \behaviour	This function returns the screeen ordinate corresponding to a cell view ordinate.
   \parameter	y	The cell view ordinate for which the request is asked.
   */

/*!\function	int CellWidget::getScreenSize(const Unit& size) const
   \behaviour	This function returns the screeen dimension corresponding to a cell view dimension.
   \parameter	size	The cell view dimension for which the request is asked.
   */

/*!\function	QPoint CellWidget::getScreenPoint(const Point& point) const
   \behaviour	This function returns the screeen location corresponding to a cell view location.
   \parameter	point	The cell view location for which the request is asked.
   */

/*!\function	QRect CellWidget::getScreenRect(const Box& box) const
   \behaviour	This function returns the screeen rectangle corresponding to a cell view box.
   \parameter	box	The cell view box for which the request is asked.
   */

/*!\function	Point CellWidget::getSnapPoint(const Point& point) const
   \behaviour	This convenience function returns the cell view location associated to a given cell
   		view location in respect to the snap grid status.
	\nn	So when snap grid is enabled the returned location correspond to the nearest grid
		point of the given location else the returned location is the given location itself.
   \parameter	point	The cell view location for which the request is asked.
   */

/*!\function	Point CellWidget::getSnapPoint(int screenX, int screenY) const
   \behaviour	This convenience function returns the cell view location associated to the specified
		screen coordinates in respect to the snap grid status.
	\nn	So when snap grid is enabled the returned location correspond to the nearest grid
		point of the location obtained from the specified screen coordinates else the
		returned location is directly the location obtained from the specified screen
		coordinates.
   \parameter	screenX	The screen abscissa for which the request is asked.
   \parameter	screenY	The screen ordinate for which the request is asked.
   */

/*!\function	const QColor& CellWidget::getBackgroundColor() const
   \behaviour	This function returns the background color of this porthole.
   */

/*!\function	const QColor& CellWidget::getForegroundColor() const
   \behaviour	This function returns the foreground color of this porthole.
   */

/*!\function	const LayerMask& CellWidget::getVisibleLayerMask() const
   \behaviour	This function returns the mask representing the visible layers of this porthole.
   */

/*!\function	Layers CellWidget::getVisibleLayers() const
   \behaviour	This function returns the collection of visible layers (can be empty).
   \note	The layers are sorted from the deepest one to the higher one.
   */

/*!\function	Command* CellWidget::getCommand(int button) const
   \behaviour	This function returns the command which is installed on a given button.
   \parameter	button	The button for which we want to get the associated command.
   */

/*!\function	Command* CellWidget::getStartedCommand() const
   \behaviour	This function returns the command of this porthole which has started.
   \note	Returns \NULL when no command has started.
   */

/*!\function	Commands CellWidget::getCommands() const
   \behaviour	This function returns the collection of commands installed on this porthole.
   */

/*!\function	const Point& CellWidget::getGridOrigin() const
   \behaviour	This function returns the origin of the grid.
   */

/*!\function	const Unit& CellWidget::getGridXStep() const
   \behaviour	This function returns the horizontal grid step.
   */

/*!\function	const Unit& CellWidget::getGridYStep() const
   \behaviour	This function returns the vertical grid step.
   */

/*!\function	unsigned CellWidget::getGridDisplayThreshold() const
   \behaviour	This function returns the display threshold used to control the drawability of the
		grid.
	\nn	The threshold is a minimal number of pixels allowed between two consecutives grid
		points on screen. When the number of pixels is less than the threshold the grid
		isn't drawn (even it is visible).
   */

/*!\function	const QColor& CellWidget::getGridColor() const
   \behaviour	This function returns the grid color of this porthole.
   */

/*!\function	Unit CellWidget::getGridX(const Unit& x, int sign) const
   \behaviour	This function returns the grid abscissa corresponding to a given cell view abscissa.
   \parameter	x		The cell view abscissa for which the request is asked.
   \parameter	sign		Indicates which grid position should be used. When the sign is -1 we
				consider the lower grid position, when the sign is 0 we consider the
				nearest position and when the sign is 1 we consider the upper grid
				position (all other values are forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when the \c sign doesn't correspond to a
				legal value.
   */

/*!\function	Unit CellWidget::getGridY(const Unit& y, int sign) const
   \behaviour	This function returns the grid ordinate corresponding to a given cell view ordinate.
   \parameter	y		The cell view ordinate for which the request is asked.
   \parameter	sign		Indicates which grid position should be used. When the sign is -1 we
				consider the lower grid position, when the sign is 0 we consider the
				nearest position and when the sign is 1 we consider the upper grid
				position (all other values are forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when the \c sign doesn't correspond to a
				legal value.
   */

/*!\function	Point CellWidget::getGridPoint(const Point& point, int xSign, int ySign) const
   \behaviour	This function returns the grid location corresponding to a given cell view location.
   \parameter	point		The cell view location for which the request is asked.
   \parameter	xSign		and
   \parameter	ySign		Indicate which grid position should be used in the associated
				direction. When the sign is -1 we consider the lower grid position,
				when the sign is 0 we consider the nearest position and when the
				sign is 1 we consider the upper grid position (all other values are
				forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when the \c xSign or \c ySign doesn't
				correspond to a legal value.
   */

/*!\function	unsigned CellWidget::getDisplayDepth() const
   \behaviour	This function returns the display depth of this porthole.
   \note	The display depth represent the maximal number of hierarchy levels that should be
		drawn in this porthole.
   */

/*!\function	unsigned CellWidget::getDisplaySize() const
   \behaviour	This function returns the display size of this porthole.
   \note	To be drawn in a given porthole the maximal size of the bounding box of a figure
		(on the screen) should be greather than the display size (defined in pixels).
   */

/*!\function	Occurrences CellWidget::getSelection() const
   \behaviour	This function returns the collection of occurrences selected in this porthole.
   */

/*!\function	Box CellWidget::getSelectionBox() const
   \behaviour	This function returns the box enclosing the selection of this porthole.
   */

/*!\function	unsigned CellWidget::getSelectionLimit() const
   \behaviour	This function returns the maximal number of occurrences that could be selected in
		this porthole.
   */

/*!\function	const QColor& CellWidget::getSelectionColor() const
   \behaviour	This function returns the selection color of this porthole.
   */

/*!\function	Occurrences CellWidget::getHighlightion() const
   \behaviour	This function returns the collection of occurrences highlighted in this porthole.
   */

/*!\function	Box CellWidget::getHighlightionBox() const
   \behaviour	This function returns the box enclosing the highlightion of this porthole.
   */

/*!\function	unsigned CellWidget::getHighlightionLimit() const
   \behaviour	This function returns the maximal number of occurrences that could be highlighted in
		this porthole.
   */

/*!\function	const QFont& CellWidget::getFont() const
   \behaviour	This function returns the current font of this porthole.
   \note	This is this font which is used by the drawing and filling functions.
   */

/*!\function	const QPen& CellWidget::getPen() const
   \behaviour	This function returns the current pen of this porthole.
   \note	This is this pen which is used by the drawing and filling functions.
   */

/*!\function	const QBrush& CellWidget::getBrush() const
   \behaviour	This function returns the current brush of this porthole.
   \note	This is this brush which is used by the drawing and filling functions.
   */

/*!\function	const QPoint& CellWidget::getBrushOrigin() const
   \behaviour	This function returns the current brush origin of this porthole.
   \note	This is this brush origin which is used by the drawing and filling functions.
   */

/*!\function	TrackBox* CellWidget::getTrackBox() const
   \behaviour	This function returns the track box associated to this porthole.
   */
//@}

//@{
/*!\function	bool CellWidget::doubleBuffering() const
   \behaviour	This function returns \TRUE when the double buffering is enabled.
   */

/*!\function	bool CellWidget::automaticScrolling() const
   \behaviour	This function returns \TRUE when the automatic scrolling is enabled.
   */

/*!\function	bool CellWidget::isVisible(Layer* layer) const
   \behaviour	This function returns \TRUE when the specified layer is visible.
   \parameter	layer	The layer for which the request is asked.
   \note	A visible layer is not systematicaly drawn. The display depends also of the layer
		display threshold and the current scale.
   */

/*!\function	bool CellWidget::isDrawable(Layer* layer) const
   \behaviour	This function returns \TRUE when the layer can be drawn according to the layer
		display threshold and the current scale.
   \note	A non visible layer can be drawable and a visible layer can be not drawable. So to
		known if the layer should really be drawn you need to check its visibility and its
		drawability.
   */

/*!\function	bool CellWidget::gridIsVisible() const
   \behaviour	This function returns \TRUE when the grid is visible.
   \note	A visible grid is not systematicaly drawn. The display depends also of the grid
		display threshold and the current scale.
   */

/*!\function	bool CellWidget::gridAxesAreVisible() const
   \behaviour	This function returns \TRUE when the grid axes are visible.
   \note	Those grid axes are drawn only when this function returns \TRUE and the grid is
		drawn.
   */

/*!\function	bool CellWidget::gridIsDrawable() const
   \behaviour	This function returns \TRUE when the grid can be drawn according to the grid
		display threshold and the current scale.
   \note	A non visible grid can be drawable and a visible grid can be not drawable. So to
		known if the grid should really be drawn you need to check its visibility and its
		drawability.
   */

/*!\function	bool CellWidget::isOnGridX(const Unit& x, unsigned n) const
   \behaviour	This function returns \TRUE when the specified cell view abscissa is on a grid
		position.
   \parameter	x		The cell view abscissa for which the request is asked.
   \parameter	n		This argument allows to do not take into account each grid position
				but only those which are multiples of n (a null value is forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when \c n is null.
   */

/*!\function	bool CellWidget::isOnGridY(const Unit& y, unsigned n) const
   \behaviour	This function returns \TRUE when the specified cell view ordinate is on a grid
		position.
   \parameter	y		The cell view ordinate for which the request is asked.
   \parameter	n		This argument allows to do not take into account each grid position
				but only those which are multiples of n (a null value is forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when \c n is null.
   */

/*!\function	bool CellWidget::isOnGridPoint(const Point& point, unsigned xN, unsigned yN) const
   \behaviour	This function returns \TRUE when the specified cell view location is on a grid
		location.
   \parameter	point		The cell view location for which the request is asked.
   \parameter	xN		and
   \parameter	yN		Allow to do not take into account each grid position but only those
				which are multiples of xN horizontaly and yN verticaly (null values
				are forbidden).
   \exception	INVALID_REQUEST	This exception is thrown when \c xN or \c yN is null.
   */

/*!\function	bool CellWidget::snapGrid() const
   \behaviour	This function returns \TRUE when the snap grid is enabled.
   */

/*!\function	bool CellWidget::selectionIsVisible() const
   \behaviour	This function returns \TRUE when the selection is visible.
   */

/*!\function	bool CellWidget::isSelected(const Occurrence& occurrence) const
   \behaviour	This function returns \TRUE when the specified occurrence is selected in this
		porthole.
   */

/*!\function	bool CellWidget::isSelectable(const Occurrence& occurrence) const
   \behaviour	This function returns \TRUE when the specified occurrence can be selected in this
		porthole (meaning that the occurrence is selectable and the view owning the
		occurrence is the view bound to this porthole).
   */

/*!\function	bool CellWidget::highlightionIsVisible() const
   \behaviour	This function returns \TRUE when the highlightion is visible.
   */

/*!\function	bool CellWidget::isHighlighted(const Occurrence& occurrence) const
   \behaviour	This function returns \TRUE when the specified occurrence is highlighted in this
		porthole.
   */

/*!\function	bool CellWidget::isHighlightable(const Occurrence& occurrence) const
   \behaviour	This function returns \TRUE when the specified occurrence can be highlighted in this
		porthole (meaning that the occurrence is highlightable and the view owning the
		occurrence is the view bound to this porthole).
   */
//@}

//@{
/*!\function	void CellWidget::setBackgroundColor(const QColor& color)
   \behaviour	This function sets the background color of this porthole to the specified value.
   \parameter	color	The new background color.
   */

/*!\function	void CellWidget::setForegroundColor(const QColor& color)
   \behaviour	This function sets the foreground color of this porthole to the specified value.
   \parameter	color	The new foreground color.
   */

/*!\function	void CellWidget::setDoubleBuffering(bool enable)
   \behaviour	This function enables or disables the double buffering.
   \parameter	enable	The new status.
   */

/*!\function	void CellWidget::setAutomaticScrolling(bool enable)
   \behaviour	This function enables or disables the automatic scrolling.
   \parameter	enable			The new status.
   \exception	INVALID_REQUEST		This exception is thrown when this porthole is a map
					porthole.
   */

/*!\function	void CellWidget::setVisible(Layer* layer, bool visible)
   \behaviour	This function sets the visibility status of the specified layer in this porthole to
		the specified value.
   \parameter	layer	The layer for which the request is asked.
   \parameter	visible	The new status.
   */

/*!\function	void CellWidget::setGridOrigin(const Point& origin)
   \behaviour	This function sets the origin of the grid of this porthole to the specified value.
   \parameter	origin	The new grid origin.
   */

/*!\function	void CellWidget::setGridXStep(const Unit& step)
   \behaviour	This function sets the horizontal grid step of this porthole to the specified value.
   \parameter	step		The new horizontal grid step.
   \exception	INVALID_REQUEST This exception is thrown when the \c step is null.
   */

/*!\function	void CellWidget::setGridYStep(const Unit& step)
   \behaviour	This function sets the vertical grid step of this porthole to the specified value.
   \parameter	step		The new vertical grid step.
   \exception	INVALID_REQUEST This exception is thrown when the \c step is null.
   */

/*!\function	void CellWidget::setGridStep(const Unit& step)
   \behaviour	This function sets the horizontal and vertical grid steps of this porthole to the
		specified value.
   \parameter	step		The new horizontal and vertical grid step.
   \exception	INVALID_REQUEST This exception is thrown when the \c step is null.
   */

/*!\function	void CellWidget::setGridSteps(const Unit& xStep, const Unit& yStep)
   \behaviour	This function sets the horizontal and vertical grid step of this porthole to the
		specified values.
   \parameter	xStep		The new horizontal grid step.
   \parameter	yStep		The new vertical grid step.
   \exception	INVALID_REQUEST	This exception is thrown when \c xStep or \c yStep is null.
				is null.
   */

/*!\function	void CellWidget::setGridDisplayThreshold(unsigned threshold)
   \behaviour	This function sets the grid display threshold of this porthole to the specified
		value.
   \parameter	threshold	The new display threshold.
   \exception	INVALID_REQUEST	This exception is thrown when the new display threshold is too
				small (less than 3 pixels).
   */

/*!\function	void CellWidget::setGridColor(const QColor& color)
   \behaviour	This function sets the grid color of this porthole to the specified value.
   \parameter	color	The new grid color.
   */

/*!\function	void CellWidget::setGridVisible(bool visible)
   \behaviour	This function sets the visibility status of the grid in this porthole to the
		specified value.
   \parameter	visible	The new status.
   */

/*!\function	void CellWidget::setGridAxesVisible(bool visible)
   \behaviour	This function sets the visibility status of the grid axes in this porthole to the
		specified value.
   \parameter	visible	The new status.
   */

/*!\function	void CellWidget::setSnapGrid(bool enable)
   \behaviour	This function enables or disables the snap grid.
   \parameter	enable	The new status.
   */

/*!\function	void CellWidget::setDisplayDepth(unsigned displayDepth)
   \behaviour	This function sets the display depth of this porthole to the specified value.
   \parameter	displayDepth	The new display depth.
   */

/*!\function	void CellWidget::setDisplaySize(unsigned displaySize)
   \behaviour	This function sets the display size of this porthole to the specified value.
   \parameter	displaySize	The new display size.
   */

/*!\function	void CellWidget::setSelectionLimit(unsigned limit)
   \behaviour	This function sets the selection limit of this porthole to the specified value.
   \parameter	limit	The new selection limit (a null value indicating no limit).
   */

/*!\function	void CellWidget::setSelectionColor(const QColor& color)
   \behaviour	This function sets the selection color of this porthole to the specified value.
   \parameter	color	The new selection color.
   */

/*!\function	void CellWidget::setSelectionVisible(bool visible)
   \behaviour	This function sets the visibility status of the selection in this porthole to the
		specified value.
   \parameter	visible	The new status.
   */

/*!\function	void CellWidget::setHighlightionLimit(unsigned limit)
   \behaviour	This function sets the highlightion limit of this porthole to the specified value.
   \parameter	limit	The new highlightion limit (a null value indicating no limit).
   */

/*!\function	void CellWidget::setHighlightionVisible(bool visible)
   \behaviour	This function sets the visibility status of the highlightion in this porthole to the
		specified value.
   \parameter	visible	The new status.
   */

/*!\function	void CellWidget::setFont(const QFont& font)
   \behaviour	This function sets the font of this porthole to the specified value.
   \parameter	font	The new font.
   \note	When a layer (or a ghost) is displayed, the graphical characteristics of this layer
		(or this ghost) have been positionned. So, in principle, unless very specific needs,
		there is no need to call this function. If you do so, dont forget to restore the
		previous font after use.
   */

/*!\function	void CellWidget::setPen(const QPen& pen, double brightness)
   \behaviour	This function sets the pen of this porthole to the specified value.
   \parameter	pen		The new pen.
   \parameter	brightness	The brightness of the new pen.
   \exception	INVALID_REQUEST	This exception is thrown if the brightness isn't in the range
				[0.1 1.0].
   \notes	When a layer (or a ghost) is displayed, the graphical characteristics of this layer
		(or this ghost) have been positionned. So, in principle, unless very specific needs,
		there is no need to call this function. If you do so, dont forget to restore the
		previous pen after use.
   */

/*!\function	void CellWidget::setBrush(const QBrush& brush, double brightness)
   \behaviour	This function sets the brush of this porthole to the specified value.
   \parameter	brush		The new brush.
   \parameter	brightness	The brightness of the new brush.
   \exception	INVALID_REQUEST	This exception is thrown if the brightness isn't in the range
				[0.1 1.0].
   \notes	When a layer (or a ghost) is displayed, the graphical characteristics of this layer
		(or this ghost) have been positionned. So, in principle, unless very specific needs,
		there is no need to call this function. If you do so, dont forget to restore the
		previous brush after use.
   */

/*!\function	void CellWidget::setBrushOrigin(const QPoint& origin)
   \behaviour	This function sets the brush origin of this porthole to the specified value.
   \parameter	origin	The new brush origin.
   \note	When a layer (or a ghost) is displayed, the graphical characteristics of this layer
		(or this ghost) have been positionned. So, in principle, unless very specific needs,
		there is no need to call this function. If you do so, dont forget to restore the
		previous brush origin after use.
   */

/*!\function	void CellWidget::setTrackBox(TrackBox* track)
   \behaviour	This function bounds this porthole with the specified track box.
   \parameter	track	The track box to bound with this porthole (can be \NULL).
   */
//@}

//@{
/*!\function	void CellWidget::showAllLayers()
   \behaviour	This function sets the visibility status of all layers to \TRUE for this porthole.
   */

/*!\function	void CellWidget::hideAllLayers()
   \behaviour	This function sets the visibility status of all layers to \FALSE for this porthole.
   */

/*!\function	bool CellWidget::select(const Occurrence& occurrence)
   \behaviour	This function selects the specified occurrence in this porthole.
   \parameter	occurrence	The occurrence to select.
   */

/*!\function	void CellWidget::unselect(const Occurrence& occurrence)
   \behaviour	This function unselects the specified occurrence from this porthole.
   \parameter	occurrence	The occurrence to unselect.
   */

/*!\function	void CellWidget::clearSelection()
   \behaviour	This function clears the selection.
   */

/*!\function	bool CellWidget::highlight(const Occurrence& occurrence)
   \behaviour	This function highlights the specified occurrence in this porthole.
   \parameter	occurrence	The occurrence to highlight.
   */

/*!\function	void CellWidget::unhighlight(const Occurrence& occurrence)
   \behaviour	This function unhighlights the specified occurrence from this porthole.
   \parameter	occurrence	The occurrence to unhighlight.
   */

/*!\function	void CellWidget::clearHighlightion()
   \behaviour	This function clears the highlightion.
   */

/*!\function	void CellWidget::clearPeeks()
   */

/*!\function	void CellWidget::addPeek(const Box& box)
   */

/*!\function	void CellWidget::invalidate()
   \behaviour	This function post an invalidation region representing the visible part of the cell
		view bound to this porthole.
   \example
	\code
		* void MyCellWidget::refresh()
		* {
		*     invalidate();
		*     redraw();
		* }
		\endcode
   */

/*!\function	void CellWidget::invalidate(const QRect& screenRect)
   \behaviour	This convenience function post an invalidation region representing the cell view
		box corresponding to a given screen rect.
   \parameter	screenRect	The screen rectangle to invalidate.
   */

/*!\function	void CellWidget::invalidate(const Box& box)
   \behaviour	This function post an invalidation region representing a given cell view box.
   \parameter	box	The cell view box to invalidate.
   */

/*!\function	void CellWidget::fitToContent(unsigned screenMargin)
   \behaviour	This function makes the entire cell view visible in this porthole.
   \parameter	screenMargin	Represent the minimal number of pixel we want around the bounding
				box of the cell view.
   */

/*!\function	void CellWidget::reframe(double scale)
   \behaviour	This function sets the scale used to display the content of this porthole to the
		specified value.
   \parameter	scale	The new scale.
   \examples
	\code
		* void MyCellWidget::zoomIn()
		* {
		*     reframe(getScale() * 1.2));
		*     redraw();
		* }
		\endcode
	\code
		* void MyCellWidget::zoomOut()
		* {
		*     reframe(getScale() / 1.2));
		*     redraw();
		* }
		\endcode
   \note	This function does nothing if the new scale is null.
   */

/*!\function	void CellWidget::reframe(const Point& center)
   \behaviour	This function sets the center of this porthole to the specified value.
   \parameter	center	The new center.
   \example
	\code
		* void MyCellWidget::pan(const Point& point)
		* {
		*     reframe(point);
		*     redraw();
		* }
		\endcode
   */

/*!\function	void CellWidget::reframe(const Point& center, double scale)
   \behaviour	This function sets the center and the scale of this porthole to the specified
		values.
   \parameter	center	The new center.
   \parameter	scale	The new scale.
   \note	This function does nothing if the new scale is null.
   */

/*!\function	void CellWidget::reframe(const Box& box)
   \behaviour	This function allows you to map a cell view region into this porthole.
   \remark	The aspect ratios of the cell view region and the porthole can be differents
		but the function ensures that the entire region will becomes visible.
   \parameter	box	The region for which the request is asked.
   \note	This function does nothing if the region is inverted.
   */

/*!\function	void CellWidget::scroll(const Unit& dx, const Unit& dy)
   \behaviour	This function scrolls the content of this porthole of the specified quantities.
   \parameter	dx	The horizontal quantity.
   \parameter	dy	The verticaly quantity.
   \note	The display is updated immediately (it is not differed as it will be with the
		reframe or fitToContent functions). So, it is not needed to call the redraw
		function to see the effects.
   */

/*!\function	void CellWidget::redraw()
   \behaviour	This function is called or should be called to redraw the invalidated region.
   \important	Nothing is done if the invalidated region is empty.
   */

/*!\function	void CellWidget::abortStartedCommand()
   \behaviour	This function aborts the started command.
   \important	Nothing is done if no command has started.
   */

/*!\function	void CellWidget::drawLine(const Unit& xo,											    const Unit& yo,											const Unit& xe,											    const Unit& ye)
   \behaviour	This function draws the line joining the points defined by the specified
		coordinates with the current pen.
   \parameter	xo		The abscissa of the line origin.
   \parameter	yo		The ordinate of the line origin.
   \parameter	xe		The abscissa of the line extremity.
   \parameter	ye		The ordinate of the line extremity.
   */

/*!\function	void CellWidget::drawLine(const Point& origin, const Point& extremity)
   \behaviour	This function draws the line joining the specified points with the current pen.
   \parameter	origin		The origin of the line.
   \parameter	extremity	The extremity of the line.
   */

/*!\function	void CellWidget::drawPolyline(const Points& points)
   \behaviour	This function draws the polyline joining the specified points with the current pen.
   \parameter	points		The collection of points defining the polyline.
   */

/*!\function	void CellWidget::drawRectangle(const Unit& xMin,											 const Unit& yMin,										     const Unit& xMax,											 const Unit& yMax)
   \behaviour	This function draws the rectangle defined by the specified values with the current
		pen and brush.
   \parameter	xMin		The left bound of the rectangle.
   \parameter	yMin		The bottom bound of the rectangle.
   \parameter	xMax		The right bound of the rectangle.
   \parameter	yMax		The top bound of the rectangle.
   */

/*!\function	void CellWidget::drawRectangle(const Box& box)
   \behaviour	This convenience function behaves essentially like the above function.
   \parameter	box		The box which defines the limits of the rectangle.
   */

/*!\function	void CellWidget::drawPolygon(const Points& points)
   \behaviour	This function draws the polygon defined by the specified points with the current
		pen and brush.
   \parameter	points		The collection of points which defining the polygon.
   */

/*!\function	void CellWidget::drawText(const Box& frame,										    const string& text,											const Direction& direction)
   \behaviour	This function draws the string defined by the specified text, in the specified
		direction, inside the specified frame, with the specified area style and using the
		current font and pen.
	\nn	Texts are displayed using a fixed size font. The appearence of each text depend to
		the scale factor of this porthole. The frame is used to control the display of a
		text. So, to be really drawn, the screen dimensions of a text should be smaller than
		the dimensions of the frame.
   \parameter	frame		Defines the maximal dimensions of the text.
   \parameter	text		Defines the string that should be drawn.
   \parameter	direction	Defines the direction of the text.
   */

/*!\function	void CellWidget::drawText(const Box& frame,										    const Name& name,											const Direction& direction)
   \behaviour	This convenience function behaves essentially like the above function.
   \parameter	frame		Defines the maximal dimensions of the name.
   \parameter	name		Defines the string that should be drawn.
   \parameter	direction	Defines the direction of the text.
   */
//@}



//CLOSE_MY_NAMESPACE
//
//USING_MY_NAMESPACE

#endif // H_CellWidget

// *************************************************************************************************
