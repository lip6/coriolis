
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
#include  <QRect>
#include  <QPoint>

class QCursor;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QAction;


#include  "hurricane/Commons.h"
#include  "hurricane/Point.h"
#include  "hurricane/Box.h"
#include  "hurricane/Transformation.h"
#include  "hurricane/Query.h"

#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/CellWidgets.h"
#include  "hurricane/viewer/Selector.h"
//#include  "hurricane/viewer/SelectorCriterion.h"
#include  "hurricane/viewer/SelectorCommand.h"




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
      inline  set<Selector*>&         getSelectorSet          ();
      inline  void                    setStartLevel           ( int );
      inline  void                    setStopLevel            ( int );
      inline  void                    setQueryFilter          ( int );
    // Painter control & Hurricane objects drawing primitives.
      inline  void                    addDrawExtensionGo      ( const Name&, InitExtensionGo_t*, DrawExtensionGo_t* );
      inline  QPainter&               getPainter              ();
      inline  float                   getScale                () const;
              bool                    isDrawableLayer         ( const Name& );
              bool                    isDrawableExtension     ( const Name& );
              void                    drawBox                 ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit );
              void                    drawBox                 ( const Box& );
              void                    drawLine                ( DbU::Unit, DbU::Unit, DbU::Unit, DbU::Unit );
              void                    drawLine                ( const Point&, const Point& );
              void                    drawText                ( const Point&, const Name&, int angle=0, bool reverse=false  );
              void                    drawGrid                ();
              void                    drawSpot                ();
              void                    drawScreenRect          ( const QPoint&, const QPoint& );
              void                    drawScreenPolyline      ( const QPoint*, int, int );
    // Geometric conversions.                                 
              QRect                   dbuToDisplayRect        ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 ) const;
              QRect                   dbuToDisplayRect        ( const Box& box ) const;
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
              QSize                   minimumSizeHint         () const;
              void                    paintEvent              ( QPaintEvent* );
              void                    resizeEvent             ( QResizeEvent* );
              void                    keyPressEvent           ( QKeyEvent* );
              void                    mouseMoveEvent          ( QMouseEvent* );
              void                    mousePressEvent         ( QMouseEvent* );
              void                    mouseReleaseEvent       ( QMouseEvent* );
    signals:                                                  
              void                    cellChanged             ( Cell* );
              void                    cellPreModificated      ();
              void                    cellPostModificated     ();
              void                    updatePalette           ( Cell* );
              void                    mousePositionChanged    ( const Point& position );
              void                    selectionChanged        ( const set<Selector*>&, Cell* );
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
              void                    updatePalette           ();
              void                    cellPreModificate       ();
              void                    cellPostModificate      ();
      inline  void                    redraw                  ();
              void                    redraw                  ( QRect redrawArea );
      inline  void                    redrawSelection         ();
              void                    redrawSelection         ( QRect redrawArea );
              void                    goLeft                  ( int dx = 0 );
              void                    goRight                 ( int dx = 0 );
              void                    goUp                    ( int dy = 0 );
              void                    goDown                  ( int dy = 0 );
              void                    fitToContents           ();
              void                    setScale                ( float scale );
              void                    setShowBoundaries       ( bool state );
              void                    reframe                 ( const Box& box );
              void                    displayReframe          ();
              void                    shiftLeft               ( int dx );
              void                    shiftRight              ( int dx );
              void                    shiftUp                 ( int dy );
              void                    shiftDown               ( int dy );

    private:
      class Spot {
        public:
                             Spot        ( CellWidget* cw );
                 void        setRestore  ( bool restore );
          inline void        setShowSpot ( bool show );
                 void        restore     ();
                 void        moveTo      ( const QPoint& point );
        private:
                 CellWidget* _cellWidget;
                 QPoint      _spotPoint;
                 bool        _restore;
                 bool        _showSpot;
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
          inline QPixmap*       plane              (); 
          inline QPixmap*       plane              ( size_t i ); 
          inline QPainter&      painter            (); 
          inline QPainter&      painter            ( size_t i ); 
          inline void           painterBegin       ();
          inline void           painterBegin       ( size_t i );
          inline void           paintersBegin      ();
          inline void           painterEnd         ();
          inline void           painterEnd         ( size_t i );
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
                 void           copyToSelect       ( int sx, int sy, int h, int w );
          inline void           copyToScreen       ();
                 void           copyToScreen       ( int sx, int sy, int h, int w );
        private:
                 CellWidget*    _cellWidget;
                 QPixmap*       _planes[2];
                 QPainter       _painters[3];
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
          inline  void        setQuery               ( const Box&            area
                                                     , const Transformation& transformation
                                                     , const BasicLayer*     basicLayer
                                                     , ExtensionSlice::Mask  extensionMask
                                                     , unsigned int          filter
                                                     );
          inline  void        addDrawExtensionGo     ( const Name&
                                                     , InitExtensionGo_t*
                                                     , DrawExtensionGo_t*
                                                     );
                  void        setDrawExtensionGo     ( const Name& );
          virtual bool        hasMasterCellCallback  () const;
          virtual bool        hasGoCallback          () const;
          virtual bool        hasExtensionGoCallback () const;
          virtual void        masterCellCallback     ();
          virtual void        goCallback             ( Go* go );
          virtual void        extensionGoCallback    ( Go* go );
                  void        drawGo                 ( const Go*              go
                                                     , const BasicLayer*      basicLayer
                                                     , const Box&             area
                                                     , const Transformation&  transformation
                                                     );
    
        protected:
                  CellWidget*        _cellWidget;
                  DrawExtensionGo_t* _drawExtensionGo;
                  map<Name,pair<InitExtensionGo_t*,DrawExtensionGo_t*> >
                                     _drawExtensionGos;
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
          virtual bool        hasExtensionGoCallback () const;
          virtual void        masterCellCallback     ();
          virtual void        extensionGoCallback    ( Go* go );
          virtual void        goCallback             ( Go* go );
    
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
              DrawingPlanes              _drawingPlanes;
              DrawingQuery               _drawingQuery;
              TextDrawingQuery           _textDrawingQuery;
              int                        _queryFilter;
              QPoint                     _mousePosition;
              Spot                       _spot;
              Cell*                      _cell;
              bool                       _showBoundaries;
              bool                       _showSelection;
              bool                       _cumulativeSelection;
              bool                       _selectionHasChanged;
              int                        _delaySelectionChanged;
              bool                       _cellModificated;
              set<Selector*>             _selectors;
              SelectorCriterions         _selection;
              vector<Command*>           _commands;
              size_t                     _redrawRectCount;
              int                        _textFontHeight;
  };


  inline void  CellWidget::Spot::setShowSpot ( bool show )
  { _showSpot = show; }


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


  inline QPixmap*  CellWidget::DrawingPlanes::plane () 
  { return plane(_workingPlane); }


  inline QPixmap*  CellWidget::DrawingPlanes::plane ( size_t i ) 
  { return _planes[i]; }


  inline QPainter&  CellWidget::DrawingPlanes::painter () 
  { return painter(_workingPlane); }


  inline QPainter&  CellWidget::DrawingPlanes::painter ( size_t i ) 
  { return _painters[i]; }


  inline void  CellWidget::DrawingPlanes::painterBegin ()
  { painterBegin ( _workingPlane ); }


  inline void  CellWidget::DrawingPlanes::painterBegin ( size_t i )
  {
    if ( i<2 ) _painters[i].begin ( _planes[i] );
    else       _painters[i].begin ( _cellWidget );
  }


  inline void  CellWidget::DrawingPlanes::paintersBegin ()
  {
    painterBegin ( 0 );
    painterBegin ( 1 );
  }


  inline void  CellWidget::DrawingPlanes::painterEnd   ()
  { painterEnd ( _workingPlane ); }


  inline void  CellWidget::DrawingPlanes::painterEnd   ( size_t i )
  { _painters[i].end (); }


  inline void  CellWidget::DrawingPlanes::paintersEnd ()
  {
    painterEnd ( 0 );
    painterEnd ( 1 );
  }


  inline void  CellWidget::DrawingPlanes::copyToSelect ()
  { copyToSelect ( 0, 0, width(), height() ); }


  inline void  CellWidget::DrawingPlanes::copyToScreen ()
  { copyToScreen ( 0, 0, width(), height() ); }


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


  inline set<Selector*>& CellWidget::getSelectorSet ()
  { return _selectors; }


  inline  QPoint& CellWidget::getOffsetVA ()
  { return _offsetVA; }


  inline void  CellWidget::redraw ()
  { redraw ( QRect(QPoint(0,0),_drawingPlanes.size()) ); }


  inline void  CellWidget::redrawSelection ()
  { redrawSelection ( QRect(QPoint(0,0),_drawingPlanes.size()) ); }


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


  inline QPainter& CellWidget::getPainter ()
  { return _drawingPlanes.painter(); }


  inline float  CellWidget::getScale () const
  { return _scale; }


  inline void  CellWidget::setQueryFilter ( int filter )
  { _queryFilter = filter; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_CELL_WIDGET__
