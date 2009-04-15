
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
#include  <functional>
#include  <tr1/memory>

#include  <QWidget>
#include  <QPixmap>
#include  <QPainter>
#include  <QPrinter>
#include  <QImage>
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
#include  "hurricane/viewer/Ruler.h"




namespace Hurricane {

  using std::vector;
  using std::unary_function;
  using std::tr1::shared_ptr;

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

  enum UpdateState { ExternalEmit    = 0
                   , InternalEmit
                   , InternalReceive
                   };


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget".


  class CellWidget : public QWidget {
      Q_OBJECT;

    private:
      class   DrawingPlanes;
    public:
      class   State;
      typedef void ( DrawExtensionGo_t )( CellWidget*
                                        , const Go*
                                        , const BasicLayer*
                                        , const Box&
                                        , const Transformation&
                                        );
      typedef void ( InitExtensionGo_t )( CellWidget* );
      enum    RubberShape { Centric     = 1
                          , Barycentric
                          , Steiner
                          };
    public:
    // Constructor & Destructor.
                                      CellWidget                 ( QWidget* parent=NULL );
      virtual                        ~CellWidget                 ();
    // Accessors.                                                
    //        MapView*                getMapView                 () { return _mapView; };
              void                    setCell                    ( Cell* );
      inline  Cell*                   getCell                    () const;
      inline  shared_ptr<State>&      getState                   ();
      inline  PaletteWidget*          getPalette                 ();
      inline  Occurrences             getOccurrencesUnder        ( const QRect& ) const;
              Occurrences             getOccurrencesUnder        ( const Box& ) const;
      inline  SelectorSet&            getSelectorSet             ();
      inline  RulerSet&               getRulerSet                ();
      inline  RubberShape             getRubberShape             () const;
      inline  int                     getStartLevel              () const;
      inline  int                     getStopLevel               () const;
      inline  Query::Mask             getQueryFilter             () const ;
              void                    bindToPalette              ( PaletteWidget* );
              void                    detachFromPalette          ();
              void                    bindCommand                ( Command* );
              void                    unbindCommand              ( Command* );
      inline  bool                    realMode                   () const;
      inline  bool                    symbolicMode               () const;
      inline  bool                    showBoundaries             () const;
      inline  bool                    showSelection              () const;
      inline  bool                    cumulativeSelection        () const;
      inline  void                    setRubberShape             ( RubberShape );
      inline  void                    setStartLevel              ( int );
      inline  void                    setStopLevel               ( int );
      inline  void                    setQueryFilter             ( Query::Mask );
      inline  bool                    timeout                    ( const char*, const Timer&, double timeout, bool& timedout ) const;
    // Painter control & Hurricane objects drawing primitives.   
      inline  void                    setEnableRedrawInterrupt   ( bool );
      inline  void                    addDrawExtensionGo         ( const Name&, InitExtensionGo_t*, DrawExtensionGo_t* );
      inline  QPainter&               getPainter                 ( size_t plane=PlaneId::Working );
      inline  int                     getDarkening               () const;
      inline  void                    copyToPrinter              ( QPrinter*, bool imageOnly = false );
      inline  void                    copyToImage                ( QImage* );
      inline  const float&            getScale                   () const;
      inline  const QPoint&           getMousePosition           () const;
      inline  void                    updateMousePosition        ();
              void                    setLayerVisible            ( const Name& layer, bool visible );
              bool                    isDrawable                 ( const Name& );
              bool                    isDrawableLayer            ( const Name& );
              bool                    isDrawableExtension        ( const Name& );
              bool                    isSelectable               ( const Name& ) const;
              bool                    isSelectable               ( const Layer* ) const;
      inline  void                    setDarkening               ( int );
      inline  void                    setPen                     ( const QPen& , size_t plane=PlaneId::Working );
              void                    drawBox                    ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit );
              void                    drawBox                    ( const Box& );
              void                    drawLine                   ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit, bool mode=true );
              void                    drawLine                   ( const Point&, const Point&, bool mode=true );
              void                    drawText                   ( const Point&, const char*, bool bold=false, int angle=0, bool reverse=false );
              void                    drawGrid                   ( QRect );
              void                    drawSpot                   ();
              void                    drawRuler                  ( shared_ptr<Ruler> );
              void                    drawRulers                 ( QRect );
              void                    drawDisplayText            ( const QPoint&, const char*, bool bold=false, int angle=0, bool reverse=false );
              void                    drawScreenLine             ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working, bool mode=true );
              void                    drawScreenRect             ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working );
              void                    drawScreenRect             ( const QRect& ,                size_t plane=PlaneId::Working );
              void                    drawScreenPolyline         ( const QPoint*, int, int,      size_t plane=PlaneId::Working );
    // Geometric conversions.                                    
              QRect                   dbuToDisplayRect           ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2, bool usePoint=true ) const;
              QRect                   dbuToDisplayRect           ( const Box& box , bool usePoint=true ) const;
              QPoint                  dbuToDisplayPoint          ( DbU::Unit x, DbU::Unit y ) const;
              QPoint                  dbuToDisplayPoint          ( const Point& point ) const;
      inline  int                     dbuToDisplayX              ( DbU::Unit x ) const;
      inline  int                     dbuToDisplayY              ( DbU::Unit y ) const;
      inline  int                     dbuToDisplayLength         ( DbU::Unit length ) const;
      inline  int                     dbuToScreenX               ( DbU::Unit x ) const;
      inline  int                     dbuToScreenY               ( DbU::Unit y ) const;
              QPoint                  dbuToScreenPoint           ( DbU::Unit x, DbU::Unit y ) const;
      inline  QPoint                  dbuToScreenPoint           ( const Point& point ) const;
      inline  DbU::Unit               displayToDbuX              ( int  x ) const;
      inline  DbU::Unit               displayToDbuY              ( int  y ) const;
      inline  DbU::Unit               displayToDbuLength         ( int  length ) const;
      inline  Box                     displayToDbuBox            ( const QRect& rect ) const;
      inline  DbU::Unit               screenToDbuX               ( int  x ) const;
      inline  DbU::Unit               screenToDbuY               ( int  y ) const;
      inline  Point                   screenToDbuPoint           ( const QPoint& point ) const;
      inline  Box                     screenToDbuBox             ( const QRect& rect ) const;
      inline  Box&                    pixelInflate               ( Box&, int pixels ) const;
      inline  Point                   getTopLeft                 () const;
              Box                     computeVisibleArea         ( float scale ) const;
              Box                     computeVisibleArea         ( float scale, const Point& topLeft ) const;
              Box                     computeVisibleArea         ( const Box&, float& scale ) const;
      inline  bool                    _underDetailedGridThreshold() const;
      inline  DbU::Unit               _snapGridStep              () const;
      inline  DbU::Unit               _onSnapGrid                ( DbU::Unit ) const;
      inline  Point                   _onSnapGrid                ( const Point& ) const;
    // Qt QWidget Functions Overloads.                           
              void                    pushCursor                 ( Qt::CursorShape cursor );
              void                    popCursor                  ();
      virtual QSize                   minimumSizeHint            () const;
      virtual void                    showEvent                  ( QShowEvent* );
      virtual void                    paintEvent                 ( QPaintEvent* );
      virtual void                    resizeEvent                ( QResizeEvent* );
      virtual void                    wheelEvent                 ( QWheelEvent* );
      virtual void                    keyPressEvent              ( QKeyEvent* );
      virtual void                    keyReleaseEvent            ( QKeyEvent* );
      virtual void                    mouseMoveEvent             ( QMouseEvent* );
      virtual void                    mousePressEvent            ( QMouseEvent* );
      virtual void                    mouseReleaseEvent          ( QMouseEvent* );
    signals:                                                  
              void                    cellChanged                ( Cell* );
              void                    cellPreModificated         ();
              void                    cellPostModificated        ();
              void                    stateChanged               ( shared_ptr<CellWidget::State>& );
              void                    styleChanged               ();
              void                    queryFilterChanged         ();
              void                    layoutModeChanged          ();
              void                    updatePalette              ( Cell* );
              void                    mousePositionChanged       ( const Point& position );
              void                    selectionModeChanged       ();
              void                    selectionChanged           ( const SelectorSet& );
              void                    selectionToggled           ( Occurrence );
              void                    showBoundariesToggled      ( bool );
    public slots:                                                
    // Qt QWidget Slots Overload & CellWidget Specifics.         
              void                    setState                   ( shared_ptr<CellWidget::State>& );
      inline  void                    openRefreshSession         ();
      inline  void                    closeRefreshSession        ();
      inline  DrawingPlanes&          getDrawingPlanes           ();
      inline  QPoint&                 getOffsetVA                ();
              void                    select                     ( const Net* );
              void                    select                     ( Occurrence );
              bool                    isSelected                 ( Occurrence );
              void                    selectOccurrencesUnder     ( Box selectArea );
              void                    unselect                   ( const Net* );
              void                    unselect                   ( Occurrence );
              void                    unselectAll                ();
              void                    toggleSelection            ( Occurrence );
              void                    setShowSelection           ( bool state );
              void                    setCumulativeSelection     ( bool state );
              void                    _select                    ( const Net* );
              void                    _unselect                  ( const Net* );
              void                    _selectOccurrencesUnder    ( Box selectArea );
              void                    _unselectAll               ();
      inline  void                    addRuler                   ( const Point&, const Point& );
      inline  void                    addRuler                   ( shared_ptr<Ruler> );
      inline  void                    clearRulers                ();
              void                    changeQueryFilter          ();
              void                    rubberChange               ();
              void                    changeLayoutMode           ();
              void                    setStyle                   ( int id );
              void                    updatePalette              ();
              void                    cellPreModificate          ();
              void                    cellPostModificate         ();
      inline  void                    refresh                    ();
              void                    _redraw                    ( QRect redrawArea );
      inline  void                    redrawSelection            ();
              void                    redrawSelection            ( QRect redrawArea );
              void                    goLeft                     ( int dx = 0 );
              void                    goRight                    ( int dx = 0 );
              void                    goUp                       ( int dy = 0 );
              void                    goDown                     ( int dy = 0 );
              void                    fitToContents              ( bool historyEnable=true );
              void                    fitToNet                   ( const Net*, bool historyEnable=true );
              void                    setScale                   ( float );
              void                    scaleHistoryUp             ();
              void                    scaleHistoryDown           ();
              void                    setRealMode                ();
              void                    setSymbolicMode            ();
              void                    setShowBoundaries          ( bool state );
              void                    reframe                    ();
              void                    reframe                    ( const Box& box, bool historyEnable=true );
              void                    displayReframe             ();
              void                    _goLeft                    ( int dx );
              void                    _goRight                   ( int dx );
              void                    _goUp                      ( int dy );
              void                    _goDown                    ( int dy );
              void                    _refresh                   ();

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
          inline              RedrawManager       ( CellWidget* );
                             ~RedrawManager       ();
                 void         goLeft              ( int );
                 void         goRight             ( int );
                 void         goUp                ( int );
                 void         goDown              ( int );
                 void         refresh             ();
                 void         process             ();
          inline void         stopProcessing      ();
          inline bool         isProcessing        () const;
          inline bool         interrupted         () const;
          inline size_t       getPendings         () const;
          inline void         openRefreshSession  ();
          inline void         closeRefreshSession ();
        private:
          CellWidget*         _widget;
          list<RedrawEvent*>  _events;
          int                 _refreshSession;
          bool                _processing;
          bool                _interrupted;
      };

    public:
      class PlaneId {
        public:
          enum Ids { Normal    = 0  // _planes[0]
                   , Selection = 1  // _planes[1]
                   , Widget    = 2
                   , Printer   = 3
                   , Image     = 4
                   , Working   = 5
                   };
      };

    private:
      class DrawingPlanes {
        public:
                                DrawingPlanes       ( const QSize& size, CellWidget* cw );
                               ~DrawingPlanes       ();
          inline bool           getLineMode         () const;
          inline size_t         getWorkingPlane     () const;
          inline void           pushWorkingPlane    ();
          inline void           popWorkingPlane     ();
          inline int            width               () const;
          inline int            height              () const;
          inline QSize          size                () const;
          inline void           select              ( size_t i );
          inline QPainter&      painter             ( size_t i=PlaneId::Working ); 
          inline void           begin               ( size_t i=PlaneId::Working );
          inline void           end                 ( size_t i=PlaneId::Working );
          inline void           buffersBegin        ();
          inline void           buffersEnd          ();
                 void           setLineMode         ( bool mode );
                 void           setPen              ( const QPen& pen );
                 void           setBrush            ( const QBrush& brush );
                 void           setBackground       ( const QBrush& brush );
                 void           setBackgroundMode   ( Qt::BGMode mode );
                 void           resize              ( const QSize& size );
                 void           shiftLeft           ( int dx );
                 void           shiftRight          ( int dx );
                 void           shiftUp             ( int dy );
                 void           shiftDown           ( int dy );
          inline void           copyToSelect        ();
          inline void           copyToSelect        ( const QRect& );
                 void           copyToSelect        ( int sx, int sy, int h, int w );
          inline void           copyToScreen        ();
                 void           copyToScreen        ( int sx, int sy, int h, int w );
          inline void           copyToPrinter       ( QPrinter*, bool imageOnly );
                 void           copyToPrinter       ( int sx, int sy, int h, int w, QPrinter*, bool imageOnly );
          inline void           copyToImage         ( QImage* );
                 void           copyToImage         ( int sx, int sy, int h, int w, QImage* );
        private:
                 CellWidget*    _cellWidget;
                 QPrinter*      _printer;
                 QImage*        _image;
                 QPixmap*       _planes[2];
                 QPainter       _painters[PlaneId::Working];
                 QPen           _normalPen;
                 QPen           _linePen;
                 size_t         _workingPlane;
                 size_t         _pushWorkingPlane;
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
                         SelectorCriterions ();
                        ~SelectorCriterions ();
          inline void    setCellWidget      ( CellWidget* );
                 bool    add                ( const Net* net );
                 bool    add                ( Box area );
                 bool    remove             (  const Net* net );
                 void    clear              ();
                 void    revalidate         ();
          inline size_t  size               () const;
        private:
          CellWidget*                 _cellWidget;
          vector<SelectorCriterion*>  _criterions;
      };

    public:
      class State {
        public:
          inline                     State                  ( Cell* cell=NULL );
          inline void                setCell                ( Cell* );
          inline void                setCellWidget          ( CellWidget* );
          inline void                setRealMode            ();
          inline void                setSymbolicMode        ();
          inline void                setShowBoundaries      ( bool );
          inline void                setShowSelection       ( bool );
          inline void                setCumulativeSelection ( bool );
                 void                setScale               ( float );
          inline void                setTopLeft             ( DbU::Unit, DbU::Unit );
          inline void                setTopLeft             ( const Point& );
          inline void                setQueryFilter         ( Query::Mask );
          inline void                setStartLevel          ( int );
          inline void                setStopLevel           ( int );
          inline void                setRubberShape         ( RubberShape );
          inline void                setHistoryEnable       ( bool );
                 bool                scaleHistoryUp         ();
                 bool                scaleHistoryDown       ();
          inline Cell*               getCell                () const;
                 const Name&         getName                () const;
          inline SelectorCriterions& getSelection           ();
          inline RulerSet&           getRulers              ();
          inline bool                realMode               () const;
          inline bool                symbolicMode           () const;
          inline bool                showBoundaries         () const;
          inline bool                showSelection          () const;
          inline bool                cumulativeSelection    () const;
          inline bool                getHistoryEnable       () const;
          inline size_t              getHistorySize         () const;
          inline const float&        getScale               () const;
          inline const Point&        getTopLeft             () const;
          inline Query::Mask         getQueryFilter         () const;
          inline int                 getStartLevel          () const;
          inline int                 getStopLevel           () const;
          inline RubberShape         getRubberShape         () const;

        private:
          class ScaleEntry {
            public:
              inline ScaleEntry ( float, const Point& );
            public:
              float  _scale;
              Point  _topLeft;
          };

        private:
          Cell*               _cell;
          CellWidget*         _cellWidget;
          SelectorCriterions  _selection;
          RulerSet            _rulers;
          bool                _symbolicMode;
          bool                _showBoundaries;
          bool                _showSelection;
          Query::Mask         _queryFilter;
          int                 _startLevel;
          int                 _stopLevel;
          RubberShape         _rubberShape;
          bool                _cumulativeSelection;
          vector<ScaleEntry>  _scaleHistory;
          size_t              _ihistory;
          bool                _historyEnable;
      };
    public:
      class FindStateName : public unary_function< const shared_ptr<State>&, bool > {
        public:
          inline      FindStateName ( const Name& );
          inline bool operator()    ( const shared_ptr<State>& );
        private:
          const Name  _cellName;
      };

    protected:
    // Internal: Attributes.
      static  const int                  _stripWidth;
      static  const int                  _initialSide;
              vector<Qt::CursorShape>    _cursors;
    //        MapView*                   _mapView;
              Technology*                _technology;
              PaletteWidget*             _palette;
              Box                        _displayArea;
              Box                        _visibleArea;
              QPoint                     _offsetVA;
              RedrawManager              _redrawManager;
              DrawingPlanes              _drawingPlanes;
              DrawingQuery               _drawingQuery;
              TextDrawingQuery           _textDrawingQuery;
              int                        _darkening;
              QPoint                     _mousePosition;
              Spot                       _spot;
              shared_ptr<State>          _state;
              bool                       _cellChanged;
              bool                       _selectionHasChanged;
              int                        _delaySelectionChanged;
              bool                       _cellModificated;
              bool                       _enableRedrawInterrupt;
              SelectorSet                _selectors;
              vector<Command*>           _commands;
              size_t                     _redrawRectCount;
              int                        _textFontHeight;

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


  inline void  CellWidget::setEnableRedrawInterrupt ( bool state )
  { _enableRedrawInterrupt = state; }


  inline void  CellWidget::openRefreshSession ()
  { _redrawManager.openRefreshSession (); }


  inline void  CellWidget::closeRefreshSession ()
  { _redrawManager.closeRefreshSession (); }


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


  inline bool  CellWidget::RedrawManager::isProcessing () const
  { return _processing; }


  inline void  CellWidget::RedrawManager::stopProcessing ()
  { _processing = false; }


  inline size_t  CellWidget::RedrawManager::getPendings () const
  { return _events.size(); }


  inline void  CellWidget::RedrawManager::openRefreshSession ()
  { _refreshSession++; }


  inline void  CellWidget::RedrawManager::closeRefreshSession ()
  {
    _refreshSession--;
    if ( !_processing && (_refreshSession == 0) ) process ();
  }


  inline bool  CellWidget::RedrawManager::interrupted () const
  {
#ifdef ALLOW_REQUEST_INTERRUPT
    return ( _events.size() > 5 ) || _interrupted;
#else
    return _interrupted;
#endif
  }


  inline bool  CellWidget::DrawingPlanes::getLineMode () const
  { return _lineMode; }


  inline size_t  CellWidget::DrawingPlanes::getWorkingPlane () const
  { return _workingPlane; }


  inline void  CellWidget::DrawingPlanes::pushWorkingPlane ()
  { _pushWorkingPlane = _workingPlane; }


  inline void  CellWidget::DrawingPlanes::popWorkingPlane ()
  { _workingPlane = _pushWorkingPlane; }


  inline int  CellWidget::DrawingPlanes::width () const
  { return _planes[PlaneId::Normal]->width(); }


  inline int  CellWidget::DrawingPlanes::height () const
  { return _planes[PlaneId::Normal]->height(); }


  inline QSize  CellWidget::DrawingPlanes::size () const
  { return _planes[PlaneId::Normal]->size(); }


  inline void  CellWidget::DrawingPlanes::select ( size_t i )
  { _workingPlane = i; }


  inline QPainter&  CellWidget::DrawingPlanes::painter ( size_t i ) 
  { return _painters[ (i>=PlaneId::Working) ? _workingPlane : i ]; }


  inline void  CellWidget::DrawingPlanes::begin ( size_t i )
  {
    size_t wp = (i>=PlaneId::Working) ? _workingPlane : i;
    switch ( wp ) {
      case PlaneId::Normal:
      case PlaneId::Selection: _painters[wp].begin ( _planes[wp]  ); break;
      case PlaneId::Widget:    _painters[ 2].begin ( _cellWidget ); break;
      case PlaneId::Printer:   _painters[ 3].begin ( _printer    ); break;
      case PlaneId::Image:     _painters[ 4].begin ( _image      ); break;
      default:
        cerr << "[BUG] Bad plane selection." << endl;
    }
  }


  inline void  CellWidget::DrawingPlanes::end   ( size_t i )
  { _painters[(i>=PlaneId::Working)?_workingPlane:i].end (); }


  inline void  CellWidget::DrawingPlanes::buffersBegin ()
  {
    begin ( PlaneId::Normal );
    begin ( PlaneId::Selection );
  }


  inline void  CellWidget::DrawingPlanes::buffersEnd ()
  {
    end ( PlaneId::Normal );
    end ( PlaneId::Selection  );
  }


  inline void  CellWidget::DrawingPlanes::copyToSelect ()
  { copyToSelect ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToSelect ( const QRect& r )
  { copyToSelect ( r.x(), r.y(), r.width(), r.height() ); }


  inline void  CellWidget::DrawingPlanes::copyToScreen ()
  { copyToScreen ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToPrinter ( QPrinter* printer, bool imageOnly )
  {
    copyToPrinter ( 0
                  , 0
                  , _cellWidget->geometry().width()
                  , _cellWidget->geometry().height()
                  , printer
                  , imageOnly
                  );
  }


  inline void  CellWidget::DrawingPlanes::copyToImage ( QImage* image )
  {
    copyToImage ( 0
                , 0
                , _cellWidget->geometry().width()
                , _cellWidget->geometry().height()
                , image
                );
  }


  inline void  CellWidget::SelectorCriterions::setCellWidget ( CellWidget* cw )
  { _cellWidget = cw; }


  inline size_t  CellWidget::SelectorCriterions::size () const
  { return _criterions.size(); }


  inline CellWidget::State::ScaleEntry::ScaleEntry ( float scale, const Point& topLeft )
    : _scale(scale), _topLeft(topLeft)
  { }


  inline CellWidget::State::State ( Cell* cell )
    : _cell               (cell)
    , _cellWidget         (NULL)
    , _selection          ()
    , _rulers             ()
    , _symbolicMode       (true)
    , _showBoundaries     (true)
    , _showSelection      (false)
    , _queryFilter        (~Query::DoTerminalCells)
    , _startLevel         (0)
    , _stopLevel          (99)
    , _rubberShape        (CellWidget::Steiner)
    , _cumulativeSelection(false)
    , _scaleHistory       ()
    , _ihistory           (0)
    , _historyEnable      (false)
  {
    _scaleHistory.push_back ( ScaleEntry(1.0,Point(0,0)) );
  }


  inline bool  CellWidget::State::symbolicMode () const
  { return _symbolicMode; }


  inline void  CellWidget::State::setCell ( Cell* cell )
  { _cell = cell; }


  inline void  CellWidget::State::setCellWidget ( CellWidget* cw )
  {
    _cellWidget = cw;
    _selection.setCellWidget ( cw );
  }


  inline void  CellWidget::State::setRealMode ()
  { _symbolicMode = false; }


  inline void  CellWidget::State::setSymbolicMode ()
  { _symbolicMode = true; }


  inline void  CellWidget::State::setShowBoundaries ( bool state )
  { _showBoundaries = state; }


  inline void  CellWidget::State::setShowSelection ( bool state )
  { _showSelection = state; }


  inline void  CellWidget::State::setCumulativeSelection ( bool state )
  { _cumulativeSelection = state; }


  inline void  CellWidget::State::setTopLeft ( DbU::Unit x, DbU::Unit y )
  {
    _scaleHistory[_ihistory]._topLeft.setX(x);
    _scaleHistory[_ihistory]._topLeft.setY(y);
  }


  inline void  CellWidget::State::setTopLeft ( const Point& topLeft )
  { _scaleHistory[_ihistory]._topLeft = topLeft; }


  inline void  CellWidget::State::setQueryFilter ( Query::Mask mask )
  { _queryFilter = mask; }


  inline void  CellWidget::State::setStartLevel ( int level )
  { _startLevel = level; }


  inline void  CellWidget::State::setStopLevel ( int level )
  { _stopLevel = level; }


  inline void  CellWidget::State::setRubberShape ( RubberShape shape )
  { _rubberShape = shape; }


  inline void  CellWidget::State::setHistoryEnable ( bool enable )
  { _historyEnable = enable; }


  inline Cell* CellWidget::State::getCell () const
  { return _cell; }


  inline CellWidget::SelectorCriterions& CellWidget::State::getSelection ()
  { return _selection; }


  inline RulerSet& CellWidget::State::getRulers ()
  { return _rulers; }


  inline bool  CellWidget::State::showBoundaries () const
  { return _showBoundaries; }


  inline bool  CellWidget::State::showSelection () const
  { return _showSelection; }


  inline bool  CellWidget::State::cumulativeSelection () const
  { return _cumulativeSelection; }


  inline bool  CellWidget::State::getHistoryEnable () const
  { return _historyEnable; }


  inline size_t  CellWidget::State::getHistorySize () const
  { return _scaleHistory.size(); }


  inline const Point& CellWidget::State::getTopLeft () const
  { return _scaleHistory[_ihistory]._topLeft; }


  inline Query::Mask  CellWidget::State::getQueryFilter () const
  { return _queryFilter; }


  inline int  CellWidget::State::getStartLevel () const
  { return _startLevel; }


  inline int  CellWidget::State::getStopLevel () const
  { return _stopLevel; }


  inline CellWidget::RubberShape  CellWidget::State::getRubberShape () const
  { return _rubberShape; }


  inline const float& CellWidget::State::getScale () const
  { return _scaleHistory[_ihistory]._scale; }


  CellWidget::FindStateName::FindStateName ( const Name& cellName )
    : unary_function< const shared_ptr<State>&, bool >()
    , _cellName(cellName)
  { }


  bool  CellWidget::FindStateName::operator () ( const shared_ptr<State>& state )
  { return state->getName() == _cellName; }


  inline shared_ptr<CellWidget::State>& CellWidget::getState ()
  {
    _state->setTopLeft ( getTopLeft() );
    return _state;
  }
  

  inline void  CellWidget::addDrawExtensionGo ( const Name&        name
                                              , InitExtensionGo_t* initExtensionGo
                                              , DrawExtensionGo_t* drawExtensionGo
                                              )
  { _drawingQuery.addDrawExtensionGo ( name, initExtensionGo, drawExtensionGo ); }


  inline void  CellWidget::setStartLevel ( int level )
  {
    _drawingQuery.setStartLevel ( level );
    _state->setStartLevel ( level );
    emit queryFilterChanged ();
  }


  inline void  CellWidget::setStopLevel ( int level )
  {
    _drawingQuery.setStopLevel ( level );
    _state->setStopLevel ( level );
    emit queryFilterChanged ();
  }


  inline int  CellWidget::getStartLevel () const
  { return _drawingQuery.getStartLevel (); }


  inline int  CellWidget::getStopLevel () const
  { return _drawingQuery.getStopLevel (); }


  inline CellWidget::DrawingPlanes& CellWidget::getDrawingPlanes ()
  { return _drawingPlanes; }


  inline SelectorSet& CellWidget::getSelectorSet ()
  { return _selectors; }


  Occurrences  CellWidget::getOccurrencesUnder ( const QRect& area ) const
  { return getOccurrencesUnder(screenToDbuBox(area)); }


  inline  QPoint& CellWidget::getOffsetVA ()
  { return _offsetVA; }


  inline void  CellWidget::addRuler ( const Point& origin, const Point& extremity )
  {
    _state->getRulers().insert ( shared_ptr<Ruler>( new Ruler(origin,extremity) ) );
    refresh ();
  }


  inline void  CellWidget::addRuler ( shared_ptr<Ruler> ruler )
  {
    _state->getRulers().insert ( ruler );
    refresh ();
  }


  inline void  CellWidget::clearRulers ()
  { _state->getRulers().clear (); refresh(); }


  inline  void  CellWidget::refresh ()
  { _redrawManager.refresh(); }


  inline void  CellWidget::redrawSelection ()
  { redrawSelection ( QRect(QPoint(0,0),_drawingPlanes.size()) ); }


  inline void  CellWidget::copyToPrinter ( QPrinter* printer, bool imageOnly )
  { _drawingPlanes.copyToPrinter ( printer, imageOnly ); }


  inline void  CellWidget::copyToImage ( QImage* image )
  { _drawingPlanes.copyToImage ( image ); }


  inline int  CellWidget::dbuToDisplayX ( DbU::Unit x ) const
  { return (int)rint ( (float)( x - _displayArea.getXMin() ) * getScale() ); }


  inline int  CellWidget::dbuToDisplayY ( DbU::Unit y ) const
  { return (int)rint ( (float)( _displayArea.getYMax() - y ) * getScale() ); }


  inline int  CellWidget::dbuToDisplayLength ( DbU::Unit length ) const
  { return (int)rint ( (float)length * getScale() ); }


  inline int  CellWidget::dbuToScreenX ( DbU::Unit x ) const
  { return (int)rint ( (float)( x - _displayArea.getXMin() ) * getScale() ) - _offsetVA.x(); }


  inline int  CellWidget::dbuToScreenY ( DbU::Unit y ) const
  { return (int)rint ( (float)( _displayArea.getYMax() - y ) * getScale() ) - _offsetVA.y(); }


  inline QPoint  CellWidget::dbuToScreenPoint ( const Point& point ) const
  { return QPoint ( dbuToScreenX(point.getX()), dbuToScreenY(point.getY()) ); }


  inline DbU::Unit  CellWidget::displayToDbuX ( int x ) const
  { return (DbU::Unit)(x/getScale()) + _displayArea.getXMin(); }


  inline DbU::Unit  CellWidget::displayToDbuY ( int y ) const
  { return _displayArea.getYMax() - (DbU::Unit)(y/getScale()); }


  inline DbU::Unit  CellWidget::displayToDbuLength ( int length ) const
  { return (int)( (float)length / getScale() ); }


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


  inline Box& CellWidget::pixelInflate ( Box& box, int pixels ) const
  { return box.inflate(displayToDbuLength(pixels)); }


  inline Point  CellWidget::getTopLeft () const
  { return Point(_visibleArea.getXMin(),_visibleArea.getYMax()); }


  inline Cell* CellWidget::getCell () const
  { return _state->getCell(); }


  inline PaletteWidget* CellWidget::getPalette ()
  { return _palette; }


  inline bool  CellWidget::realMode () const
  { return !_state->symbolicMode(); }


  inline bool  CellWidget::symbolicMode () const
  { return _state->symbolicMode(); }


  inline bool  CellWidget::showBoundaries () const
  { return _state->showBoundaries(); }


  inline bool  CellWidget::showSelection () const
  { return _state->showSelection(); }


  inline bool  CellWidget::cumulativeSelection () const
  { return _state->cumulativeSelection(); }


  inline QPainter& CellWidget::getPainter ( size_t plane )
  { return _drawingPlanes.painter(plane); }


  inline int  CellWidget::getDarkening () const
  { return _darkening; }


  inline const float& CellWidget::getScale () const
  { return _state->getScale(); }


  inline const QPoint& CellWidget::getMousePosition () const
  { return _mousePosition; }


  inline void  CellWidget::updateMousePosition ()
  {
    Point mousePoint = screenToDbuPoint ( _mousePosition );
    emit mousePositionChanged ( _onSnapGrid(mousePoint) );
  }


  inline void  CellWidget::setQueryFilter ( Query::Mask filter )
  {
    _state->setQueryFilter ( filter );
    emit queryFilterChanged ();
  }


  inline Query::Mask  CellWidget::getQueryFilter () const
  { return _state->getQueryFilter(); }


  inline void  CellWidget::setRubberShape ( RubberShape shape )
  {
    _state->setRubberShape ( shape );
    emit queryFilterChanged ();
  }


  inline CellWidget::RubberShape  CellWidget::getRubberShape () const
  { return _state->getRubberShape(); }


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


  inline DbU::Unit  CellWidget::_snapGridStep () const
  { return  symbolicMode() ? DbU::getSymbolicSnapGridStep() : DbU::getRealSnapGridStep(); }


  inline DbU::Unit  CellWidget::_onSnapGrid ( DbU::Unit u ) const
  { return symbolicMode() ? DbU::getOnSymbolicSnapGrid(u) : DbU::getOnRealSnapGrid(u); }


  inline Point  CellWidget::_onSnapGrid ( const Point& p ) const
  { return Point(_onSnapGrid(p.getX()),_onSnapGrid(p.getY())); }


} // End of Hurricane namespace.


#endif  // __HURRICANE_CELL_WIDGET__
