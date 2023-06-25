// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/CellWidget.h"               |
// +-----------------------------------------------------------------+


#pragma  once
#include <math.h>
#include <vector>
#include <functional>
#include <memory>
#include <boost/function.hpp>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPrinter>
#include <QImage>
#include <QRect>
#include <QPoint>
class QCursor;
class QShowEvent;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QAction;

#include "hurricane/Timer.h"
#include "hurricane/Commons.h"
#include "hurricane/Warning.h"
#include "hurricane/Point.h"
#include "hurricane/Box.h"
#include "hurricane/Transformation.h"
#include "hurricane/Query.h"
#include "hurricane/viewer/DisplayStyle.h"
#include "hurricane/viewer/CellWidgets.h"
#include "hurricane/viewer/Selector.h"
#include "hurricane/viewer/SelectorCriterion.h"
#include "hurricane/viewer/Ruler.h"


namespace Hurricane {

  using std::vector;
  using std::unary_function;
  using std::shared_ptr;

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
      typedef boost::function< void(QPainter&) >  PainterCb_t;
      enum    RubberShape    { Centric=1, Barycentric, Steiner };
      enum    TextFlag       { Bold    =0x0001
                             , BigFont =0x0002
                             , Reverse =0x0004
                             , Frame   =0x0008
                             , Rounded =0x0010
                             , Center  =0x0020
                             , Left    =0x0040
                             , Right   =0x0080
                             , Top     =0x0100
                             };
      enum    Flag           { NoFlags        =0x0000
                             , NoResetCommands=0x0001
                             };
    public:
      enum    ResolutionMode { Res_CellMode=1, Res_DesignMode=2 };
    public:
    // Constructor & Destructor.
                                        CellWidget                 ( QWidget* parent=NULL );
      virtual                          ~CellWidget                 ();
    // Accessors.                                                  
    //        MapView*                  getMapView                 () { return _mapView; };
              void                      setCell                    ( Cell*, Path  topPath=Path(), unsigned int flags=NoFlags );
      inline  Cell*                     getCell                    () const;
      inline  Cell*                     getTopCell                 () const;
      inline  Path                      getTopPath                 () const;
      inline  shared_ptr<State>&        getState                   ();
      inline  shared_ptr<State>         getStateClone              ();
      inline  PaletteWidget*            getPalette                 ();
      inline  Occurrences               getOccurrencesUnder        ( const QRect& ) const;
              Occurrences               getOccurrencesUnder        ( const Box& ) const;
      inline  SelectorSet&              getSelectorSet             ();
      inline  RulerSet&                 getRulerSet                ();
      inline  RubberShape               getRubberShape             () const;
      inline  int                       getStartLevel              () const;
      inline  int                       getStopLevel               () const;
      inline  Query::Mask               getQueryFilter             () const ;
              void                      bindToPalette              ( PaletteWidget* );
              void                      detachFromPalette          ();
              void                      detach                     ( Selector*);
              void                      bindCommand                ( Command* );
              void                      unbindCommand              ( Command* );
              void                      resetCommands              ();
      inline  void                      setActiveCommand           ( Command* );
      inline  Command*                  getActiveCommand           () const;
              Command*                  getCommand                 ( const std::string& ) const;
      inline  void                      resetActiveCommand         ();
      inline  void                      setCursorStep              ( DbU::Unit );
      inline  void                      setRealSnapGridStep        ( DbU::Unit step );
      inline  unsigned int              getDbuMode                 () const;
      inline  bool                      gridMode                   () const;
      inline  bool                      symbolicMode               () const;
      inline  bool                      physicalMode               () const;
      inline  DbU::UnitPower            getUnitPower               () const;
      inline  bool                      showBoundaries             () const;
      inline  bool                      showSelection              () const;
      inline  bool                      cumulativeSelection        () const;
      inline  void                      setPixelThreshold          ( int );
      inline  void                      setDbuMode                 ( int );
      inline  void                      setUnitPower               ( DbU::UnitPower );
      inline  void                      setRubberShape             ( RubberShape );
      inline  void                      setStartLevel              ( int );
      inline  void                      setStopLevel               ( int );
      inline  void                      setQueryFilter             ( Query::Mask );
      inline  bool                      timeout                    ( const char*, const Timer&, double timeout, bool& timedout ) const;
    // Painter control & Hurricane objects drawing primitives.   
      inline  void                      setEnableRedrawInterrupt   ( bool );
      inline  void                      addDrawExtensionGo         ( const Name&, InitExtensionGo_t*, DrawExtensionGo_t* );
      inline  void                      copyDrawExtensionGos       ( const CellWidget* );
      inline  QPainter&                 getPainter                 ( size_t plane=PlaneId::Working );
      inline  const DisplayStyle::HSVr& getDarkening               () const;
      inline  void                      copyToPrinter              ( int xpaper, int ypaper, QPrinter*, PainterCb_t& );
      inline  void                      copyToImage                ( QImage*, PainterCb_t& );
      inline  int                       getPixelThreshold          () const;
      inline  const float&              getScale                   () const;
      inline  const QPoint&             getMousePosition           () const;
      inline  void                      updateMousePosition        ();
              void                      setLayerVisible            ( const Name& layer, bool visible );
              bool                      isLayerVisible             ( const Name& );
              bool                      isDrawable                 ( const Name& );
              bool                      isDrawableLayer            ( const Name& );
              bool                      isDrawableExtension        ( const Name& );
              bool                      isSelectable               ( const Name& ) const;
              bool                      isSelectable               ( const Layer* ) const;
              bool                      isPrinter                  () const;
              void                      setPrinter                 ( bool );
      inline  void                      setDarkening               ( const DisplayStyle::HSVr& );
      inline  void                      setPen                     ( const QPen& , size_t plane=PlaneId::Working );
              void                      drawBox                    ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit );
              void                      drawBox                    ( const Box& );
              void                      drawBoxBorder              ( const Box& );
              void                      drawLine                   ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit, bool mode=true );
              void                      drawLine                   ( const Point&, const Point&, bool mode=true );
              void                      drawText                   ( const Point&, const char*, unsigned int flags=0, int angle=0 );
              void                      drawGrid                   ( QRect );
              void                      drawSpot                   ();
              void                      drawRuler                  ( shared_ptr<Ruler> );
              void                      drawRulers                 ( QRect );
              void                      drawDisplayText            ( const QRect& , const char*, unsigned int flags=0 );
              void                      drawDisplayText            ( const QPoint&, const char*, unsigned int flags=0, int angle=0 );
              void                      drawScreenPolygon          ( const QPoint*, int count,     size_t plane=PlaneId::Working );
              void                      drawScreenPolygon          ( const QPolygon&,              size_t plane=PlaneId::Working );
              void                      drawScreenLine             ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working, bool mode=true );
              void                      drawScreenRect             ( const QPoint&, const QPoint&, size_t plane=PlaneId::Working );
              void                      drawScreenRect             ( const QRect& ,                size_t plane=PlaneId::Working );
              void                      drawScreenPolyline         ( const QPoint*, int, int,      size_t plane=PlaneId::Working );
    // Geometric conversions.                                      
      inline  DbU::Unit                 toDbu                      ( float ) const;
              QRect                     dbuToScreenRect            ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2, bool usePoint=true ) const;
              QRect                     dbuToScreenRect            ( const Box& box , bool usePoint=true ) const;
      inline  int                       dbuToScreenX               ( DbU::Unit x ) const;
      inline  int                       dbuToScreenY               ( DbU::Unit y ) const;
      inline  int                       dbuToScreenLength          ( DbU::Unit length ) const;
      inline  QPoint                    dbuToScreenPoint           ( DbU::Unit x, DbU::Unit y ) const;
      inline  QPoint                    dbuToScreenPoint           ( const Point& point ) const;
      inline  DbU::Unit                 screenToDbuLength          ( int  length ) const;
      inline  DbU::Unit                 screenToDbuX               ( int  x ) const;
      inline  DbU::Unit                 screenToDbuY               ( int  y ) const;
      inline  Point                     screenToDbuPoint           ( const QPoint& point ) const;
      inline  Box                       screenToDbuBox             ( const QRect& rect ) const;
      inline  Box&                      pixelInflate               ( Box&, int pixels ) const;
      inline  Point                     getTopLeft                 () const;
      inline  Box                       getVisibleArea             () const;
              Box                       computeVisibleArea         ( float scale ) const;
              Box                       computeVisibleArea         ( float scale, const Point& topLeft ) const;
              Box                       computeVisibleArea         ( const Box&, float& scale ) const;
      inline  DbU::Unit                 cursorStep                 () const;
      inline  bool                      _underDetailedGridThreshold() const;
      inline  DbU::Unit                 _snapGridStep              () const;
      inline  DbU::Unit                 _onSnapGrid                ( DbU::Unit ) const;
      inline  Point                     _onSnapGrid                ( const Point& ) const;
      inline  DbU::Unit                 _onCursorGrid              ( DbU::Unit ) const;
      inline  Point                     _onCursorGrid              ( const Point& ) const;
    // Qt QWidget Functions Overloads.                             
              void                      pushCursor                 ( Qt::CursorShape cursor );
              void                      popCursor                  ();
      virtual QSize                     minimumSizeHint            () const;
      virtual void                      showEvent                  ( QShowEvent* );
      virtual void                      resizeEvent                ( QResizeEvent* );
      virtual void                      wheelEvent                 ( QWheelEvent* );
      virtual void                      keyPressEvent              ( QKeyEvent* );
      virtual void                      keyReleaseEvent            ( QKeyEvent* );
      virtual void                      mouseMoveEvent             ( QMouseEvent* );
      virtual void                      mousePressEvent            ( QMouseEvent* );
      virtual void                      mouseReleaseEvent          ( QMouseEvent* );
    signals:                                                    
              void                      cellChanged                ( Cell* );
              void                      cellPreModificated         ();
              void                      cellPostModificated        ();
              void                      stateChanged               ( shared_ptr<CellWidget::State>& );
              void                      styleChanged               ();
              void                      queryFilterChanged         ();
              void                      dbuModeChanged             ( unsigned int mode, DbU::UnitPower );
              void                      updatePalette              ( Cell* );
              void                      mousePositionChanged       ( const Point& position );
              void                      selectionModeChanged       ();
              void                      selectionChanged           ( const SelectorSet& );
              void                      selectionToggled           ( Selector* );
              void                      unlinkSelector             ( Selector* );
              void                      showBoundariesToggled      ( bool );
    protected:
      virtual void                      paintEvent                 ( QPaintEvent* );
    public slots:                                                
    // Qt QWidget Slots Overload & CellWidget Specifics.         
              void                      setState                   ( shared_ptr<CellWidget::State>&
                                                                   , unsigned int flags=NoFlags );
      inline  void                      openRefreshSession         ();
      inline  void                      closeRefreshSession        ();
      inline  DrawingPlanes&            getDrawingPlanes           ();
           // void                      select                     ( const Net* );
              void                      select                     ( Occurrence );
              void                      select                     ( Occurrences );
              void                      selectSet                  ( const OccurrenceSet& );
              void                      selectSet                  ( const ComponentSet& );
              bool                      isSelected                 ( Occurrence );
              void                      selectOccurrencesUnder     ( Box selectArea );
           // void                      unselect                   ( const Net* );
              void                      unselect                   ( Occurrence );
              void                      unselect                   ( Occurrences );
              void                      unselectSet                ( const ComponentSet& );
              void                      unselectSet                ( const OccurrenceSet& );
              void                      unselectAll                ();
              void                      toggleSelection            ( Occurrence );
              void                      setShowSelection           ( bool state );
              void                      setCumulativeSelection     ( bool state );
           // void                      _select                    ( const Net* );
           // void                      _unselect                  ( const Net* );
           // void                      _selectOccurrencesUnder    ( Box selectArea );
              void                      _unselectAll               ();
      inline  void                      addRuler                   ( const Point&, const Point& );
      inline  void                      addRuler                   ( shared_ptr<Ruler> );
      inline  void                      clearRulers                ();
              void                      changeQueryFilter          ();
              void                      rubberChange               ();
              void                      changeDbuMode              ( unsigned int mode, DbU::UnitPower );
              void                      setStyle                   ( int id );
              void                      updatePalette              ();
              void                      cellPreModificate          ();
              void                      cellPostModificate         ();
      inline  void                      refresh                    ();
              void                      _redraw                    ( QRect redrawArea );
      inline  void                      redrawSelection            ();
              void                      redrawSelection            ( QRect redrawArea );
              void                      goLeft                     ( int dx = 0 );
              void                      goRight                    ( int dx = 0 );
              void                      goUp                       ( int dy = 0 );
              void                      goDown                     ( int dy = 0 );
              void                      fitToContents              ( bool historyEnable=true );
              void                      fitToNet                   ( const Net*, bool historyEnable=true );
              void                      setScale                   ( float );
              void                      scaleHistoryUp             ();
              void                      scaleHistoryDown           ();
    //        void                      setGridMode                ();
    //        void                      setSymbolicMode            ();
    //        void                      setPhysicalMode            ( DbU::UnitPower );
              void                      setShowBoundaries          ( bool state );
              void                      reframe                    ();
              void                      reframe                    ( const Box& box, bool historyEnable=true );
              void                      displayReframe             ();
              void                      _goLeft                    ( int dx );
              void                      _goRight                   ( int dx );
              void                      _goUp                      ( int dy );
              void                      _goDown                    ( int dy );
              void                      _refresh                   ();
              std::string               _getString                 () const;

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
                   , AutoCopy  = 2  // _planes[2]
                   , Widget    = 3
                   , Printer   = 4
                   , Image     = 5
                   , Working   = 6
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
          inline void           copyToPrinter       ( int xpaper, int ypaper, QPrinter*, CellWidget::PainterCb_t& );
                 void           copyToPrinter       ( int xpaper, int ypaper, int sx, int sy, int h, int w, QPrinter*, CellWidget::PainterCb_t& );
          inline void           copyToImage         ( QImage*, CellWidget::PainterCb_t& );
                 void           copyToImage         ( int sx, int sy, int h, int w, QImage*, CellWidget::PainterCb_t& );
        private:
          static const int      _cartoucheWidth;
          static const int      _cartoucheHeight;
          static const int      _titleHeight;
                 CellWidget*    _cellWidget;
                 QPrinter*      _printer;
                 QImage*        _image;
                 QPixmap*       _planes[3];
                 QPainter       _painters[PlaneId::Working];
                 QPen           _normalPen;
                 QPen           _linePen;
                 QPoint         _brushOrigin;
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
          inline  void          copyDrawExtensionGos   ( const DrawingQuery& );
                  void          setDrawExtensionGo     ( const Name& );
          virtual bool          hasMasterCellCallback  () const;
          virtual bool          hasGoCallback          () const;
          virtual bool          hasMarkerCallback      () const;
          virtual bool          hasRubberCallback      () const;
          virtual bool          hasExtensionGoCallback () const;
          virtual void          masterCellCallback     ();
          virtual void          goCallback             ( Go*     );
          virtual void          rubberCallback         ( Rubber* );
          virtual void          markerCallback         ( Marker* );
          virtual void          extensionGoCallback    ( Go*     );
                  void          drawMasterCell         ( const Cell*            cell
                                                       , const Transformation&  transformation
                                                       );
                  void          drawGo                 ( const Go*              go
                                                       , const BasicLayer*      basicLayer
                                                       , const Box&             area
                                                       , const Transformation&  transformation
                                                       );
                  void          drawRubber             ( const Rubber*          rubber
                                                       , const Box&             area
                                                       , const Transformation&  transformation
                                                       );
                  void          drawMarker             ( const Marker*          marker
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
          inline void                              setCellWidget      ( CellWidget* );
          inline const vector<SelectorCriterion*>& getCriterions      () const;
                 SelectorCriterion*                add                ( Occurrence netOccurrence );
                 SelectorCriterion*                add                ( Box area );
          inline SelectorCriterion*                add                ( SelectorCriterion* );
                 bool                              remove             ( Occurrence netOccurrence );
                 void                              clear              ();
                 void                              invalidate         ();
                 void                              revalidate         ();
          inline size_t                            size               () const;
        private:
          CellWidget*                 _cellWidget;
          vector<SelectorCriterion*>  _criterions;
      };

    public:
      class State {
        public:
          inline                     State                  ( Cell* cell=NULL, Path topPath=Path() );
                 State*              clone                  () const;
          inline void                setCell                ( Cell* );
          inline void                setTopPath             ( Path );
          inline void                setCellWidget          ( CellWidget* );
          inline void                setCursorStep          ( DbU::Unit );
          inline DbU::Unit           getCursorStep          () const;
          inline DbU::UnitPower      getUnitPower           () const;
          inline void                setDbuMode             ( int );
          inline void                setUnitPower           ( DbU::UnitPower );
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
          inline Cell*               getTopCell             () const;
          inline Path                getTopPath             () const;
                 const Name&         getName                () const;
          inline SelectorCriterions& getSelection           ();
          inline RulerSet&           getRulers              ();
          inline DbU::Unit           cursorStep             () const;
          inline unsigned int        getDbuMode             () const;
          inline bool                gridMode               () const;
          inline bool                symbolicMode           () const;
          inline bool                physicalMode           () const;
          inline bool                nanoMode               () const;
          inline bool                microMode              () const;
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
          Path                _topPath;
          Name                _hierarchicalName;
          CellWidget*         _cellWidget;
          SelectorCriterions  _selection;
          RulerSet            _rulers;
          DbU::Unit           _cursorStep;
          unsigned int        _dbuMode;
          DbU::UnitPower      _unitPower;
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
          const Name  _cellHierName;
      };

    protected:
    // Internal: Attributes.
              vector<Qt::CursorShape>    _cursors;
    //        MapView*                   _mapView;
              Technology*                _technology;
              PaletteWidget*             _palette;
              Box                        _screenArea;
              RedrawManager              _redrawManager;
              DrawingPlanes              _drawingPlanes;
              DrawingQuery               _drawingQuery;
              TextDrawingQuery           _textDrawingQuery;
              DisplayStyle::HSVr         _darkening;
              QPoint                     _mousePosition;
              Spot                       _spot;
              shared_ptr<State>          _state;
              bool                       _isPrinter;
              bool                       _cellChanged;
              bool                       _selectionHasChanged;
              int                        _delaySelectionChanged;
              bool                       _cellModificated;
              bool                       _enableRedrawInterrupt;
              SelectorSet                _selectors;
              Command*                   _activeCommand;
              vector<Command*>           _commands;
              size_t                     _redrawRectCount;
              int                        _textFontHeight;
              int                        _pixelThreshold;

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


  inline void  CellWidget::DrawingQuery::copyDrawExtensionGos ( const CellWidget::DrawingQuery& other )
  { _drawExtensionGos = other._drawExtensionGos; }


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
      case PlaneId::Selection:
      case PlaneId::AutoCopy:  _painters[wp              ].begin( _planes[wp] ); break;
      case PlaneId::Widget:    _painters[PlaneId::Widget ].begin( _cellWidget ); break;
      case PlaneId::Printer:   _painters[PlaneId::Printer].begin( _printer    ); break;
      case PlaneId::Image:     _painters[PlaneId::Image  ].begin( _image      ); break;
      default:
        std::cerr << "[BUG] Bad plane selection." << std::endl;
    }
  }


  inline void  CellWidget::DrawingPlanes::end ( size_t i )
  { _painters[(i>=PlaneId::Working)?_workingPlane:i].end (); }


  inline void  CellWidget::DrawingPlanes::buffersBegin ()
  {
    begin( PlaneId::Normal    );
    begin( PlaneId::Selection );
    begin( PlaneId::AutoCopy  );
  }


  inline void  CellWidget::DrawingPlanes::buffersEnd ()
  {
    end( PlaneId::Normal    );
    end( PlaneId::Selection );
    end( PlaneId::AutoCopy  );
  }


  inline void  CellWidget::DrawingPlanes::copyToSelect ()
  { copyToSelect ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToSelect ( const QRect& r )
  { copyToSelect ( r.x(), r.y(), r.width(), r.height() ); }


  inline void  CellWidget::DrawingPlanes::copyToScreen ()
  { copyToScreen ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToPrinter ( int xpaper, int ypaper, QPrinter* printer, CellWidget::PainterCb_t& cb )
  {
    copyToPrinter ( xpaper
                  , ypaper
                  , 0
                  , 0
                  , _cellWidget->geometry().width()
                  , _cellWidget->geometry().height()
                  , printer
                  , cb
                  );
  }


  inline void  CellWidget::DrawingPlanes::copyToImage ( QImage* image, CellWidget::PainterCb_t& cb )
  {
    copyToImage ( 0
                , 0
                , _cellWidget->geometry().width()
                , _cellWidget->geometry().height()
                , image
                , cb
                );
  }


  inline void  CellWidget::SelectorCriterions::setCellWidget ( CellWidget* cw )
  { _cellWidget = cw; }


  inline size_t  CellWidget::SelectorCriterions::size () const
  { return _criterions.size(); }


  inline const vector<SelectorCriterion*>& CellWidget::SelectorCriterions::getCriterions () const
  { return _criterions; }


  inline SelectorCriterion*  CellWidget::SelectorCriterions::add ( SelectorCriterion* criterion )
  {
    _criterions.push_back( criterion );
    return _criterions.back();
  }


  inline CellWidget::State::ScaleEntry::ScaleEntry ( float scale, const Point& topLeft )
    : _scale(scale), _topLeft(topLeft)
  { }


  inline CellWidget::State::State ( Cell* cell, Path topPath )
    : _cell               (cell)
    , _topPath            (topPath)
    , _hierarchicalName   ()
    , _cellWidget         (NULL)
    , _selection          ()
    , _rulers             ()
    , _cursorStep         (DbU::lambda(0.5))
    , _dbuMode            (DbU::Symbolic)
    , _unitPower          (DbU::Nano)
    , _showBoundaries     (true)
    , _showSelection      (false)
    , _queryFilter        (~Query::DoTerminalCells)
    , _startLevel         (0)
    , _stopLevel          (99)
    , _rubberShape        (CellWidget::Barycentric)
    , _cumulativeSelection(false)
    , _scaleHistory       ()
    , _ihistory           (0)
    , _historyEnable      (false)
  {
    _scaleHistory.push_back ( ScaleEntry(1.0,Point(0,0)) );
    if (_cell) _hierarchicalName = Name( _cell->getHierarchicalName() );
  }


  inline unsigned int  CellWidget::State::getDbuMode () const
  { return _dbuMode; }


  inline bool  CellWidget::State::symbolicMode () const
  { return (_dbuMode == DbU::Symbolic); }


  inline bool  CellWidget::State::gridMode () const
  { return (_dbuMode == DbU::Grid); }


  inline bool  CellWidget::State::physicalMode () const
  { return (_dbuMode == DbU::Physical); }


  inline void  CellWidget::State::setCell ( Cell* cell )
  {
    _cell = cell;
    if (_cell) _hierarchicalName = Name( _cell->getHierarchicalName() );
  }


  inline void  CellWidget::State::setTopPath ( Path topPath )
  { _topPath = topPath; }


  inline void  CellWidget::State::setCellWidget ( CellWidget* cw )
  {
    _cellWidget = cw;
    _selection.setCellWidget ( cw );
  }


  inline void  CellWidget::State::setCursorStep ( DbU::Unit step )
  { _cursorStep = step; }

  inline DbU::Unit CellWidget::State::getCursorStep () const
  { return _cursorStep; }


  inline DbU::UnitPower CellWidget::State::getUnitPower () const
  { return _unitPower; }


  inline void  CellWidget::State::setDbuMode ( int mode )
  {
    _dbuMode = mode;
    switch ( _dbuMode ) {
      case DbU::Symbolic: _cursorStep = DbU::fromLambda(0.5); break;
      case DbU::Grid:     _cursorStep = DbU::fromGrid  (1.0); break;
      case DbU::Physical: _cursorStep = DbU::fromGrid  (1.0); break;
    }
  }


  inline void  CellWidget::State::setUnitPower ( DbU::UnitPower p )
  { _unitPower = p; }


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


  inline Path  CellWidget::State::getTopPath () const
  { return _topPath; }


  inline Cell* CellWidget::State::getTopCell () const
  { return (_topPath.isEmpty()) ? _cell : _topPath.getOwnerCell(); }


  inline DbU::Unit  CellWidget::State::cursorStep () const
  { return _cursorStep; }


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


  inline  void  CellWidget::setPixelThreshold ( int pixelThreshold )
  { _pixelThreshold = pixelThreshold; }


  inline  int  CellWidget::getPixelThreshold () const
  { return _pixelThreshold; }


  inline CellWidget::FindStateName::FindStateName ( const Name& cellHierName )
    : unary_function< const shared_ptr<State>&, bool >()
    , _cellHierName(cellHierName)
  { }


  inline bool  CellWidget::FindStateName::operator () ( const shared_ptr<State>& state )
  { return state->getName() == _cellHierName; }


  inline void  CellWidget::setActiveCommand ( Command* command )
  { _activeCommand = command; }


  inline Command* CellWidget::getActiveCommand () const
  { return _activeCommand; }


  inline void  CellWidget::resetActiveCommand ()
  { _activeCommand = NULL; }


  inline void  CellWidget::setCursorStep ( DbU::Unit step )
  { _state->setCursorStep(step); }


  inline void  CellWidget::setRealSnapGridStep ( DbU::Unit step )
  { DbU::setRealSnapGridStep(step); }


  inline shared_ptr<CellWidget::State>& CellWidget::getState ()
  {
    _state->setTopLeft ( getTopLeft() );
    return _state;
  }


  inline shared_ptr<CellWidget::State>  CellWidget::getStateClone ()
  {
    _state->setTopLeft ( getTopLeft() );
    return shared_ptr<State>( _state->clone() );
  }
  

  inline void  CellWidget::addDrawExtensionGo ( const Name&        name
                                              , InitExtensionGo_t* initExtensionGo
                                              , DrawExtensionGo_t* drawExtensionGo
                                              )
  { _drawingQuery.addDrawExtensionGo( name, initExtensionGo, drawExtensionGo ); }
  

  inline void  CellWidget::copyDrawExtensionGos ( const CellWidget* other )
  { _drawingQuery.copyDrawExtensionGos( other->_drawingQuery ); }


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


  inline void  CellWidget::copyToPrinter ( int xpaper, int ypaper, QPrinter* printer, CellWidget::PainterCb_t& cb )
  { _drawingPlanes.copyToPrinter( xpaper, ypaper, printer, cb ); }


  inline void  CellWidget::copyToImage ( QImage* image, PainterCb_t& cb )
  { _drawingPlanes.copyToImage ( image, cb ); }


  inline DbU::Unit  CellWidget::toDbu ( float d ) const
  {
    DbU::Unit unit;
    switch ( getDbuMode() ) {
      case DbU::Physical: unit = DbU::grid(DbU::physicalToGrid(d,DbU::Micro)); break;
      case DbU::Grid:     unit = DbU::grid(d); break;
      case DbU::Db:       unit = DbU::db((long)d); break;
      default:
      case DbU::Symbolic: unit = DbU::lambda(d); break;
    }
    return unit;
  }


  inline int  CellWidget::dbuToScreenX ( DbU::Unit x ) const
  { return (int)rint ( (float)( x - _screenArea.getXMin() ) * getScale() ); }


  inline int  CellWidget::dbuToScreenY ( DbU::Unit y ) const
  { return (int)rint ( (float)( _screenArea.getYMax() - y ) * getScale() ); }


  inline int  CellWidget::dbuToScreenLength ( DbU::Unit length ) const
  { return (int)rint ( (float)length * getScale() ); }


  inline QPoint  CellWidget::dbuToScreenPoint ( DbU::Unit x, DbU::Unit y ) const
  { return QPoint ( dbuToScreenX(x), dbuToScreenY(y) ); }


  inline QPoint  CellWidget::dbuToScreenPoint ( const Point& point ) const
  { return QPoint ( dbuToScreenX(point.getX()), dbuToScreenY(point.getY()) ); }


  inline DbU::Unit  CellWidget::screenToDbuX ( int x ) const
  { return (DbU::Unit)(x/getScale()) + _screenArea.getXMin(); }


  inline DbU::Unit  CellWidget::screenToDbuY ( int y ) const
  { return _screenArea.getYMax() - (DbU::Unit)(y/getScale()); }


  inline DbU::Unit  CellWidget::screenToDbuLength ( int length ) const
  { return (int)( (float)length / getScale() ); }


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
  { return box.inflate(screenToDbuLength(pixels)); }


  inline Point  CellWidget::getTopLeft () const
  { return Point(_screenArea.getXMin(),_screenArea.getYMax()); }


  inline Box  CellWidget::getVisibleArea () const
  { return computeVisibleArea(getScale()); }


  inline Cell* CellWidget::getCell () const
  { return _state->getCell(); }


  inline Cell* CellWidget::getTopCell () const
  { return _state->getTopCell(); }


  inline Path  CellWidget::getTopPath () const
  { return _state->getTopPath(); }


  inline PaletteWidget* CellWidget::getPalette ()
  { return _palette; }


  inline DbU::Unit  CellWidget::cursorStep () const
  { return _state->cursorStep(); }


  inline unsigned int  CellWidget::getDbuMode () const
  { return _state->getDbuMode(); }


  inline bool  CellWidget::gridMode () const
  { return _state->gridMode(); }


  inline bool  CellWidget::symbolicMode () const
  { return _state->symbolicMode(); }


  inline bool  CellWidget::physicalMode () const
  { return _state->physicalMode(); }


  inline DbU::UnitPower CellWidget::getUnitPower () const
  { return _state->getUnitPower(); }


  inline bool  CellWidget::showBoundaries () const
  { return _state->showBoundaries(); }


  inline bool  CellWidget::showSelection () const
  { return _state->showSelection(); }


  inline bool  CellWidget::cumulativeSelection () const
  { return _state->cumulativeSelection(); }


  inline QPainter& CellWidget::getPainter ( size_t plane )
  { return _drawingPlanes.painter(plane); }


  inline const DisplayStyle::HSVr& CellWidget::getDarkening () const
  { return _darkening; }


  inline const float& CellWidget::getScale () const
  { return _state->getScale(); }


  inline const QPoint& CellWidget::getMousePosition () const
  { return _mousePosition; }


  inline void  CellWidget::updateMousePosition ()
  {
    Point mousePoint = screenToDbuPoint ( _mousePosition );
    emit mousePositionChanged ( _onCursorGrid(mousePoint) );
  }


  inline void  CellWidget::setQueryFilter ( Query::Mask filter )
  {
    _state->setQueryFilter ( filter );
    emit queryFilterChanged ();
  }


  inline Query::Mask  CellWidget::getQueryFilter () const
  { return _state->getQueryFilter(); }


  inline void  CellWidget::setDbuMode ( int mode )
  { _state->setDbuMode(mode); }


  inline void  CellWidget::setUnitPower ( DbU::UnitPower p )
  { _state->setUnitPower(p); }


  inline void  CellWidget::setRubberShape ( RubberShape shape )
  {
    _state->setRubberShape ( shape );
    _redrawManager.refresh ();
    emit queryFilterChanged ();
  }


  inline CellWidget::RubberShape  CellWidget::getRubberShape () const
  { return _state->getRubberShape(); }


  inline void  CellWidget::setPen ( const QPen& pen, size_t plane )
  { _drawingPlanes.painter(plane).setPen(pen); }


  inline void  CellWidget::setDarkening ( const DisplayStyle::HSVr& darkening )
  { _darkening = darkening; }


  inline bool  CellWidget::isPrinter () const
  { return _isPrinter; }


  inline void  CellWidget::setPrinter ( bool state )
  { _isPrinter = state; }


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


  inline DbU::Unit  CellWidget::_onCursorGrid ( DbU::Unit u ) const
  { return DbU::getOnCustomGrid(u,cursorStep()); }


  inline Point  CellWidget::_onCursorGrid ( const Point& p ) const
  { return Point(_onCursorGrid(p.getX()),_onCursorGrid(p.getY())); }


} // End of Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::CellWidget);
IOSTREAM_POINTER_SUPPORT(Hurricane::CellWidget);
