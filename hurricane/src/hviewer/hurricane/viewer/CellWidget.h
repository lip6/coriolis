
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./CellWidget.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_CELL_WIDGET_H__
#define  __HURRICANE_CELL_WIDGET_H__

#include  <math.h>

#include  <vector>

#include  <QWidget>
#include  <QPixmap>
#include  <QPainter>
#include  <QPrinter>
#include  <QRect>
#include  <QPoint>

class QCursor;
class QShowEvent;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QAction;


#include  "hurricane/Timer.h"
#include  "hurricane/Commons.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Point.h"
#include  "hurricane/Box.h"
#include  "hurricane/Transformation.h"
#include  "hurricane/Query.h"

#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/CellWidgets.h"
#include  "hurricane/viewer/Selector.h"
#include  "hurricane/viewer/SelectorCriterion.h"




namespace Hurricane {

  class Technology;
  class BasicLayer;
  class Go;
  class Net;
  class Cell;
  class Instance;
  class Slice;
  class Segment;
  class Contact;
  class Pad;

  class Selector;
  class PaletteWidget;
  class Command;
//class MapView;


  class CellWidget : public QWidget {
      Q_OBJECT;

    private:
      class DrawingPlanes;
    public:
      typedef void ( DrawExtensionGo_t )( CellWidget*
                                        , const Go*
                                        , const BasicLayer*
                                        , const Box&
                                        , const Transformation&
                                        );
      typedef void ( InitExtensionGo_t )( CellWidget* );
    public:
      enum RubberShape { Centric     = 1
                       , Barycentric
                       , Steiner
                       };
    public:
    // Constructor & Destructor.
                                      CellWidget              ( QWidget* parent=NULL );
      virtual                        ~CellWidget              ();
    // Accessors.                                             
    //        MapView*                getMapView              () { return _mapView; };
              void                    setCell                 ( Cell* );
      inline  Cell*                   getCell                 () const;
      inline  PaletteWidget*          getPalette              ();
              void                    bindToPalette           ( PaletteWidget* );
              void                    detachFromPalette       ();
              void                    bindCommand             ( Command* );
              void                    unbindCommand           ( Command* );
      inline  bool                    showBoundaries          () const;
      inline  bool                    showSelection           () const;
      inline  bool                    cumulativeSelection     () const;
      inline  Occurrences             getOccurrencesUnder     ( const QRect& ) const;
              Occurrences             getOccurrencesUnder     ( const Box& ) const;
      inline  SelectorSet&            getSelectorSet          ();
      inline  void                    setRubberShape          ( RubberShape );
      inline  RubberShape             getRubberShape          () const;
      inline  void                    setStartLevel           ( int );
      inline  void                    setStopLevel            ( int );
      inline  void                    setQueryFilter          ( int );
      inline  int                     getQueryFilter          () const ;
      inline  bool                    timeout                 ( const char*, const Timer&, double timeout, bool& timedout ) const;
    // Painter control & Hurricane objects drawing primitives.
      inline  void                    addDrawExtensionGo      ( const Name&, InitExtensionGo_t*, DrawExtensionGo_t* );
      inline  QPainter&               getPainter              ( size_t plane=PlaneId::Working );
      inline  int                     getDarkening            () const;
      inline  void                    copyToPrinter           ( QPrinter* printer );
      inline  float                   getScale                () const;
      inline  const QPoint&           getMousePosition        () const;
              bool                    isDrawable              ( const Name& );
              bool                    isDrawableLayer         ( const Name& );
              bool                    isDrawableExtension     ( const Name& );
              bool                    isSelectable            ( const Name& ) const;
              bool                    isSelectable            ( const Layer* ) const;
      inline  void                    setDarkening            ( int );
      inline  void                    setPen                  ( const QPen& , size_t plane=PlaneId::Working );
              void                    drawBox                 ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit );
              void                    drawBox                 ( const Box& );
              void                    drawLine                ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit, bool mode=true );
              void                    drawLine                ( const Point&, const Point&, bool mode=true );
              void                    drawText                ( const Point&, const Name&, int angle=0, bool reverse=false );
              void                    drawGrid                ( QRect );
              void                    drawSpot                ();
              void                    drawScreenLine          ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working, bool mode=true );
              void                    drawScreenRect          ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working );
              void                    drawScreenRect          ( const QRect& ,                size_t plane=PlaneId::Working );
              void                    drawScreenPolyline      ( const QPoint*, int, int,      size_t plane=PlaneId::Working );
    // Geometric conversions.                                 
              QRect                   dbuToDisplayRect        ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2, bool usePoint=true ) const;
              QRect                   dbuToDisplayRect        ( const Box& box , bool usePoint=true ) const;
              QPoint                  dbuToDisplayPoint       ( DbU::Unit x, DbU::Unit y ) const;
              QPoint                  dbuToDisplayPoint       ( const Point& point ) const;
      inline  int                     dbuToDisplayX           ( DbU::Unit x ) const;
      inline  int                     dbuToDisplayY           ( DbU::Unit y ) const;
      inline  int                     dbuToDisplayLength      ( DbU::Unit length ) const;
      inline  int                     dbuToScreenX            ( DbU::Unit x ) const;
      inline  int                     dbuToScreenY            ( DbU::Unit y ) const;
              QPoint                  dbuToScreenPoint        ( DbU::Unit x, DbU::Unit y ) const;
      inline  QPoint                  dbuToScreenPoint        ( const Point& point ) const;
      inline  DbU::Unit               displayToDbuX           ( int  x ) const;
      inline  DbU::Unit               displayToDbuY           ( int  y ) const;
      inline  DbU::Unit               displayToDbuLength      ( int  length ) const;
      inline  Box                     displayToDbuBox         ( const QRect& rect ) const;
      inline  DbU::Unit               screenToDbuX            ( int  x ) const;
      inline  DbU::Unit               screenToDbuY            ( int  y ) const;
      inline  Point                   screenToDbuPoint        ( const QPoint& point ) const;
      inline  Box                     screenToDbuBox          ( const QRect& rect ) const;
    // Qt QWidget Functions Overloads.                        
              void                    pushCursor              ( Qt::CursorShape cursor );
              void                    popCursor               ();
      virtual QSize                   minimumSizeHint         () const;
      virtual void                    showEvent               ( QShowEvent* );
      virtual void                    paintEvent              ( QPaintEvent* );
      virtual void                    resizeEvent             ( QResizeEvent* );
      virtual void                    wheelEvent              ( QWheelEvent* );
      virtual void                    keyPressEvent           ( QKeyEvent* );
      virtual void                    keyReleaseEvent         ( QKeyEvent* );
      virtual void                    mouseMoveEvent          ( QMouseEvent* );
      virtual void                    mousePressEvent         ( QMouseEvent* );
      virtual void                    mouseReleaseEvent       ( QMouseEvent* );
    signals:                                                  
              void                    cellChanged             ( Cell* );
              void                    cellPreModificated      ();
              void                    cellPostModificated     ();
              void                    settingsChanged         ();
              void                    styleChanged            ( void* emitter );
              void                    updatePalette           ( Cell* );
              void                    mousePositionChanged    ( const Point& position );
              void                    selectionChanged        ( const SelectorSet&, Cell* );
              void                    occurrenceToggled       ( Occurrence );
              void                    showSelectionToggled    ( bool );
    public slots:                                             
    // Qt QWidget Slots Overload & CellWidget Specifics.      
      inline  DrawingPlanes&          getDrawingPlanes        ();
      inline  QPoint&                 getOffsetVA             ();
              void                    select                  ( const Net* net, bool delayRedraw=false );
              void                    select                  ( Occurrence occurence );
              bool                    isSelected              ( Occurrence occurence );
              void                    selectOccurrencesUnder  ( Box selectArea );
              void                    unselect                ( const Net* net, bool delayRedraw=false );
              void                    unselect                ( Occurrence occurence );
              void                    unselectAll             ( bool delayRedraw=false );
              void                    toggleSelect            ( Occurrence occurence, bool fromPopup );
              void                    setShowSelection        ( bool state );
              void                    setCumulativeSelection  ( bool state );
              void                    _select                 ( const Net* net, bool delayRedraw=false );
              void                    _unselect               ( const Net* net, bool delayRedraw=false );
              void                    _selectOccurrencesUnder ( Box selectArea );
              void                    _unselectAll            ( bool delayRedraw );
              void                    rubberChange            ();
              void                    styleChange             ( void* emitter );
              void                    updatePalette           ();
              void                    cellPreModificate       ();
              void                    cellPostModificate      ();
      inline  void                    refresh                 ();
              void                    _redraw                 ( QRect redrawArea );
      inline  void                    redrawSelection         ();
              void                    redrawSelection         ( QRect redrawArea );
              void                    goLeft                  ( int dx = 0 );
              void                    goRight                 ( int dx = 0 );
              void                    goUp                    ( int dy = 0 );
              void                    goDown                  ( int dy = 0 );
              void                    fitToContents           ( bool delayed=false );
              void                    setScale                ( float );
              void                    setShowBoundaries       ( bool state );
              void                    reframe                 ( const Box& box, bool delayed=false  );
              void                    displayReframe          ( bool delayed=false );
              void                    _goLeft                 ( int dx );
              void                    _goRight                ( int dx );
              void                    _goUp                   ( int dy );
              void                    _goDown                 ( int dy );
              void                    _refresh                ();

    private:
      class Spot {
        public:
                               Spot             ( CellWidget* );
                 void          setRestore       ( bool );
          inline void          setShowSpot      ( bool );
          inline const QPoint& getSpotPoint     () const;
                 void          restore          ();
                 QPoint        computeSpotPoint ( const QPoint& );
                 void          moveTo           ( const QPoint& );
        private:
                 CellWidget* _cellWidget;
                 QPoint      _spotPoint;
                 bool        _restore;
                 bool        _showSpot;
      };

    private:
      class RedrawEvent {
        public:
          enum EventType { GoLeft  = 1
                         , GoRight = 2
                         , GoUp    = 3
                         , GoDown  = 4
                         , Refresh = 5
                         };
        public:
                              RedrawEvent ( EventType, int shift, CellWidget* );
          inline EventType    getType     () const;
          inline int          getShift    () const;
        private:
          EventType  _type;
          int        _shift;
      };

    private:
      class RedrawManager {
        public:
          inline              RedrawManager ( CellWidget* );
                             ~RedrawManager ();
                 void         goLeft        ( int );
                 void         goRight       ( int );
                 void         goUp          ( int );
                 void         goDown        ( int );
                 void         refresh       ();
                 void         process       ();
          inline void         clearOverrun  ();
          inline bool         isOverrun     () const;
          inline bool         interrupted   () const;
          inline size_t       getPendings   () const;
        private:
          CellWidget*         _widget;
          list<RedrawEvent*>  _events;
          bool                _overrun;
          bool                _refreshInterrupt;
      };

    public:
      class PlaneId {
        public:
          enum Ids { Normal    = 0
                   , Selection = 1
                   , Widget    = 2
                   , Printer   = 3
                   , Working   = 4
                   };
      };

    private:
      class DrawingPlanes {
        public:
                                DrawingPlanes      ( const QSize& size, CellWidget* cw );
                               ~DrawingPlanes      ();
          inline bool           getLineMode        () const;
          inline int            width              () const;
          inline int            height             () const;
          inline QSize          size               () const;
          inline void           select             ( size_t i );
          inline QPainter&      painter            ( size_t i=PlaneId::Working ); 
          inline void           painterBegin       ( size_t i=PlaneId::Working );
          inline void           paintersBegin      ();
          inline void           painterEnd         ( size_t i=PlaneId::Working );
          inline void           paintersEnd        ();
                 void           setLineMode        ( bool mode );
                 void           setPen             ( const QPen& pen );
                 void           setBrush           ( const QBrush& brush );
                 void           setBackground      ( const QBrush& brush );
                 void           setBackgroundMode  ( Qt::BGMode mode );
                 void           resize             ( const QSize& size );
                 void           shiftLeft          ( int dx );
                 void           shiftRight         ( int dx );
                 void           shiftUp            ( int dy );
                 void           shiftDown          ( int dy );
          inline void           copyToSelect       ();
          inline void           copyToSelect       ( const QRect& );
                 void           copyToSelect       ( int sx, int sy, int h, int w );
          inline void           copyToScreen       ();
                 void           copyToScreen       ( int sx, int sy, int h, int w );
          inline void           copyToPrinter      ( QPrinter* );
                 void           copyToPrinter      ( int sx, int sy, int h, int w, QPrinter* );
        private:
                 CellWidget*    _cellWidget;
                 QPrinter*      _printer;
                 QPixmap*       _planes[2];
                 QPainter       _painters[4];
                 QPen           _normalPen;
                 QPen           _linePen;
                 size_t         _workingPlane;
                 bool           _lineMode;
        private:
                                DrawingPlanes ( const DrawingPlanes& );
                 DrawingPlanes& operator=     ( const DrawingPlanes& );
      };

    private:
      class DrawingQuery : public Query {
        public:
                                DrawingQuery           ( CellWidget* widget );
          inline  void          setQuery               ( const Box&            area
                                                       , const Transformation& transformation
                                                       , const BasicLayer*     basicLayer
                                                       , ExtensionSlice::Mask  extensionMask
                                                       , unsigned int          filter
                                                       );
          inline  void          addDrawExtensionGo     ( const Name&
                                                       , InitExtensionGo_t*
                                                       , DrawExtensionGo_t*
                                                       );
                  void          setDrawExtensionGo     ( const Name& );
          virtual bool          hasMasterCellCallback  () const;
          virtual bool          hasGoCallback          () const;
          virtual bool          hasRubberCallback      () const;
          virtual bool          hasExtensionGoCallback () const;
          virtual void          masterCellCallback     ();
          virtual void          goCallback             ( Go*     );
          virtual void          rubberCallback         ( Rubber* );
          virtual void          extensionGoCallback    ( Go*     );
                  void          drawGo                 ( const Go*              go
                                                       , const BasicLayer*      basicLayer
                                                       , const Box&             area
                                                       , const Transformation&  transformation
                                                       );
                  void          drawRubber             ( const Rubber*          rubber
                                                       , const Box&             area
                                                       , const Transformation&  transformation
                                                       );
                  void          drawExtensionGo        ( CellWidget*            widget
                                                       , const Go*              go
                                                       , const BasicLayer*      basicLayer
                                                       , const Box&             area
                                                       , const Transformation&  transformation
                                                       );
          inline  unsigned int  getGoCount             () const;
          inline  unsigned int  getExtensionGoCount    () const;
          inline  unsigned int  getInstanceCount       () const;
          inline  void          resetGoCount           ();
          inline  void          resetExtensionGoCount  ();
          inline  void          resetInstanceCount     ();
    
        protected:
                  CellWidget*        _cellWidget;
                  DrawExtensionGo_t* _drawExtensionGo;
                  map<Name,pair<InitExtensionGo_t*,DrawExtensionGo_t*> >
                                     _drawExtensionGos;
                  unsigned int       _goCount;
                  unsigned int       _extensionGoCount;
                  unsigned int       _instanceCount;
      };

    private:
      class TextDrawingQuery : public Query {
        public:
                              TextDrawingQuery       ( CellWidget* widget );
          inline  void        setQuery               ( const Box&            area
                                                     , const Transformation& transformation
                                                     );
          virtual bool        hasMasterCellCallback  () const;
          virtual bool        hasGoCallback          () const;
          virtual bool        hasRubberCallback      () const;
          virtual bool        hasExtensionGoCallback () const;
          virtual void        masterCellCallback     ();
          virtual void        goCallback             ( Go* go );
          virtual void        extensionGoCallback    ( Go* go );
          virtual void        rubberCallback         ( Rubber* );
    
        protected:
                  CellWidget* _cellWidget;
      };

    private:
      class SelectorCriterions {
        public:
                SelectorCriterions ( CellWidget* );
               ~SelectorCriterions ();
          bool  add                ( const Net* net, bool delayRedraw );
          bool  add                ( Box area );
          bool  remove             (  const Net* net, bool delayRedraw );
          void  clear              ();
          void  revalidate         ();
        private:
          CellWidget*                 _cellWidget;
          vector<SelectorCriterion*>  _criterions;
      };

    protected:
    // Internal: Attributes.
      static  const int                  _stripWidth;
              vector<Qt::CursorShape>    _cursors;
    //        MapView*                   _mapView;
              Technology*                _technology;
              PaletteWidget*             _palette;
              Box                        _displayArea;
              Box                        _visibleArea;
              float                      _scale;
              QPoint                     _offsetVA;
              RedrawManager              _redrawManager;
              DrawingPlanes              _drawingPlanes;
              DrawingQuery               _drawingQuery;
              TextDrawingQuery           _textDrawingQuery;
              int                        _queryFilter;
              int                        _darkening;
              QPoint                     _mousePosition;
              Spot                       _spot;
              Cell*                      _cell;
              bool                       _cellChanged;
              bool                       _showBoundaries;
              bool                       _showSelection;
              bool                       _cumulativeSelection;
              bool                       _selectionHasChanged;
              int                        _delaySelectionChanged;
              bool                       _cellModificated;
              SelectorSet                _selectors;
              SelectorCriterions         _selection;
              vector<Command*>           _commands;
              size_t                     _redrawRectCount;
              int                        _textFontHeight;
              RubberShape                _rubberShape;

      friend class RedrawManager;
  };


  inline void  CellWidget::Spot::setShowSpot ( bool show )
  { _showSpot = show; }


  inline const QPoint& CellWidget::Spot::getSpotPoint () const
  { return _spotPoint; }


  inline void  CellWidget::DrawingQuery::setQuery ( const Box&            area
                                                  , const Transformation& transformation
                                                  , const BasicLayer*     basicLayer
                                                  , ExtensionSlice::Mask  extensionMask
                                                  , unsigned int          filter
                                                  )
  {
    Query::setQuery ( _cellWidget->getCell()
                    , area
                    , transformation
                    , basicLayer
                    , extensionMask
                    , filter
                    );
  }


  inline void  CellWidget::DrawingQuery::addDrawExtensionGo ( const Name&        name
                                                            , InitExtensionGo_t* initExtensionGo
                                                            , DrawExtensionGo_t* drawExtensionGo
                                                            )
  { _drawExtensionGos[name] = make_pair(initExtensionGo,drawExtensionGo); }


  inline void  CellWidget::DrawingQuery::resetGoCount ()
  { _goCount = 0; }


  inline void  CellWidget::DrawingQuery::resetExtensionGoCount ()
  { _extensionGoCount = 0; }


  inline void  CellWidget::DrawingQuery::resetInstanceCount ()
  { _instanceCount = 0; }


  inline unsigned int  CellWidget::DrawingQuery::getGoCount () const
  { return _goCount; }


  inline unsigned int  CellWidget::DrawingQuery::getExtensionGoCount () const
  { return _extensionGoCount; }


  inline unsigned int  CellWidget::DrawingQuery::getInstanceCount () const
  { return _instanceCount; }


  inline CellWidget::RedrawEvent::EventType  CellWidget::RedrawEvent::getType () const
  { return _type; }


  inline int  CellWidget::RedrawEvent::getShift () const
  { return _shift; }


  inline bool  CellWidget::RedrawManager::isOverrun () const
  { return _overrun; }


  inline void  CellWidget::RedrawManager::clearOverrun ()
  { _overrun = false; }


  inline size_t  CellWidget::RedrawManager::getPendings () const
  { return _events.size(); }


  inline bool  CellWidget::RedrawManager::interrupted () const
  { return ( _events.size() > 5 ) || _refreshInterrupt; }


  inline bool  CellWidget::DrawingPlanes::getLineMode () const
  { return _lineMode; }


  inline int  CellWidget::DrawingPlanes::width () const
  { return _planes[0]->width(); }


  inline int  CellWidget::DrawingPlanes::height () const
  { return _planes[0]->height(); }


  inline QSize  CellWidget::DrawingPlanes::size () const
  { return _planes[0]->size(); }


  inline void  CellWidget::DrawingPlanes::select ( size_t i )
  { _workingPlane = i; }


  inline QPainter&  CellWidget::DrawingPlanes::painter ( size_t i ) 
  { return _painters[(i>3)?_workingPlane:i]; }


  inline void  CellWidget::DrawingPlanes::painterBegin ( size_t i )
  {
    switch ( i ) {
      case 4: i = _workingPlane;
      case 0:
      case 1: _painters[i].begin ( _planes[i]  ); break;
      case 2: _painters[2].begin ( _cellWidget ); break;
      case 3: _painters[3].begin ( _printer    ); break;
    }
  }


  inline void  CellWidget::DrawingPlanes::paintersBegin ()
  {
    painterBegin ( 0 );
    painterBegin ( 1 );
  }


  inline void  CellWidget::DrawingPlanes::painterEnd   ( size_t i )
  { _painters[(i>3)?_workingPlane:i].end (); }


  inline void  CellWidget::DrawingPlanes::paintersEnd ()
  {
    painterEnd ( 0 );
    painterEnd ( 1 );
  }


  inline void  CellWidget::DrawingPlanes::copyToSelect ()
  { copyToSelect ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToSelect ( const QRect& r )
  { copyToSelect ( r.x(), r.y(), r.width(), r.height() ); }


  inline void  CellWidget::DrawingPlanes::copyToScreen ()
  { copyToScreen ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToPrinter ( QPrinter* printer )
  {
    copyToPrinter ( 0
                  , 0
                  , _cellWidget->geometry().width()
                  , _cellWidget->geometry().height()
                  , printer
                  );
  }


  inline void  CellWidget::addDrawExtensionGo ( const Name&        name
                                              , InitExtensionGo_t* initExtensionGo
                                              , DrawExtensionGo_t* drawExtensionGo
                                              )
  { _drawingQuery.addDrawExtensionGo ( name, initExtensionGo, drawExtensionGo ); }


  inline void  CellWidget::setStartLevel ( int level )
  { _drawingQuery.setStartLevel ( level ); }


  inline void  CellWidget::setStopLevel ( int level )
  { _drawingQuery.setStopLevel ( level ); }


  inline CellWidget::DrawingPlanes& CellWidget::getDrawingPlanes ()
  { return _drawingPlanes; }


  inline SelectorSet& CellWidget::getSelectorSet ()
  { return _selectors; }


  Occurrences  CellWidget::getOccurrencesUnder ( const QRect& area ) const
  { return getOccurrencesUnder(screenToDbuBox(area)); }


  inline  QPoint& CellWidget::getOffsetVA ()
  { return _offsetVA; }


  inline  void  CellWidget::refresh ()
  { _redrawManager.refresh(); }


  inline void  CellWidget::redrawSelection ()
  { redrawSelection ( QRect(QPoint(0,0),_drawingPlanes.size()) ); }


  inline void  CellWidget::copyToPrinter ( QPrinter* printer )
  { _drawingPlanes.copyToPrinter ( printer ); }


  inline int  CellWidget::dbuToDisplayX ( DbU::Unit x ) const
  { return (int)rint ( (float)( x - _displayArea.getXMin() ) * _scale ); }


  inline int  CellWidget::dbuToDisplayY ( DbU::Unit y ) const
  { return (int)rint ( (float)( _displayArea.getYMax() - y ) * _scale ); }


  inline int  CellWidget::dbuToDisplayLength ( DbU::Unit length ) const
  { return (int)rint ( (float)length * _scale ); }


  inline int  CellWidget::dbuToScreenX ( DbU::Unit x ) const
  { return (int)rint ( (float)( x - _displayArea.getXMin() ) * _scale ) - _offsetVA.x(); }


  inline int  CellWidget::dbuToScreenY ( DbU::Unit y ) const
  { return (int)rint ( (float)( _displayArea.getYMax() - y ) * _scale ) - _offsetVA.y(); }


  inline QPoint  CellWidget::dbuToScreenPoint ( const Point& point ) const
  { return QPoint ( dbuToScreenX(point.getX()), dbuToScreenY(point.getY()) ); }


  inline DbU::Unit  CellWidget::displayToDbuX ( int x ) const
  { return (DbU::Unit)(x/_scale) + _displayArea.getXMin(); }


  inline DbU::Unit  CellWidget::displayToDbuY ( int y ) const
  { return _displayArea.getYMax() - (DbU::Unit)(y/_scale); }


  inline DbU::Unit  CellWidget::displayToDbuLength ( int length ) const
  { return (int)( (float)length / _scale ); }


  inline Box  CellWidget::displayToDbuBox ( const QRect& rect ) const
  {
    return Box ( displayToDbuX(rect.x())
               , displayToDbuY(rect.y())
               , displayToDbuX(rect.x()+rect.width ())
               , displayToDbuY(rect.y()+rect.height())
               );
  }


  inline DbU::Unit  CellWidget::screenToDbuX ( int x ) const
  { return displayToDbuX(x+_offsetVA.x()); }


  inline DbU::Unit  CellWidget::screenToDbuY ( int y ) const
  { return displayToDbuY(y+_offsetVA.y()); }


  inline Point  CellWidget::screenToDbuPoint ( const QPoint& point ) const
  { return Point ( screenToDbuX(point.x()), screenToDbuY(point.y()) ); }


  inline Box  CellWidget::screenToDbuBox ( const QRect& rect ) const
  {
    return Box ( screenToDbuX(rect.x())
               , screenToDbuY(rect.y())
               , screenToDbuX(rect.x()+rect.width ())
               , screenToDbuY(rect.y()+rect.height())
               );
  }


  inline Cell* CellWidget::getCell () const
  { return _cell; }


  inline PaletteWidget* CellWidget::getPalette ()
  { return _palette; }


  inline bool  CellWidget::showBoundaries () const
  { return _showBoundaries; }


  inline bool  CellWidget::showSelection  () const
  { return _showSelection; }


  inline bool  CellWidget::cumulativeSelection () const
  { return _cumulativeSelection; }


  inline QPainter& CellWidget::getPainter ( size_t plane )
  { return _drawingPlanes.painter(plane); }


  inline int  CellWidget::getDarkening () const
  { return _darkening; }


  inline float  CellWidget::getScale () const
  { return _scale; }


  inline const QPoint& CellWidget::getMousePosition () const
  { return _mousePosition; }


  inline void  CellWidget::setQueryFilter ( int filter )
  { _queryFilter = filter; }


  inline int  CellWidget::getQueryFilter () const
  { return _queryFilter; }


  inline void  CellWidget::setRubberShape ( RubberShape shape )
  {
    _rubberShape = shape;
    refresh ();
  }


  inline CellWidget::RubberShape  CellWidget::getRubberShape () const
  { return _rubberShape; }


  inline void  CellWidget::setPen ( const QPen& pen, size_t plane )
  { _drawingPlanes.painter(plane).setPen(pen); }


  inline void  CellWidget::setDarkening ( int darkening )
  { _darkening = darkening; }


  inline  bool  CellWidget::timeout ( const char* fname, const Timer& timer, double timeout, bool& timedout ) const
  {
    if ( timedout ) return true;
    if ( timer.getCombTimeOnTheFly() < timeout ) return false;

    timedout = true;
    cerr << Warning("CellWidget::%s(): timeout %.3f (limit:%.1f)."
                   ,fname,timer.getCombTimeOnTheFly(),timeout) << endl;
    return true;
  }


} // End of Hurricane namespace.


#endif  // __HURRICANE_CELL_WIDGET__
