
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
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


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>


# ifndef  __CELL_WIDGET_H__
#   define  __CELL_WIDGET_H__

# include  <math.h>

# include  <vector>

# include  <QWidget>
# include  <QPixmap>
# include  <QPainter>
# include  <QRect>
# include  <QPoint>

class QCursor;
class QResizeEvent;
class QMouseEvent;
class QKeyEvent;
class QAction;


# include  "hurricane/Commons.h"
# include  "hurricane/Point.h"
# include  "hurricane/Box.h"
# include  "hurricane/Transformation.h"

# include  "hurricane/viewer/DisplayStyle.h"
# include  "hurricane/viewer/CellWidgets.h"
# include  "hurricane/viewer/Selector.h"




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
  class HPaletteEntry;
  class HPalette;
//class MapView;


  class CellWidget : public QWidget {
      Q_OBJECT;

    private:
      class DrawingPlanes;

    public:
    // Constructor & Destructor.
                                      CellWidget           ( QWidget* parent=NULL );
      virtual                        ~CellWidget           ();
    // Accessors.
    //        MapView*                getMapView           () { return _mapView; };
              void                    setCell              ( Cell* cell );
      inline  Cell*                   getCell              () const;
      inline  HPalette*               getPalette           ();
              void                    bindToPalette        ( HPalette* palette );
              void                    detachFromPalette    ();
      inline  bool                    showBoundaries       () const;
      inline  set<Selector*>&         getSelectorSet       ();
      inline  bool                    showSelection        () const;
              void                    select               ( const Net* net, bool delayRedraw=false );
              void                    select               ( Occurrence& occurence );
              void                    unselect             ( Occurrence& occurence );
              void                    unselectAll          ( bool delayRedraw=true );
    // Painter control & Hurricane objects drawing primitives.
              void                    drawBoundaries       ( const Cell*    , const Box&, const Transformation& );
              void                    drawBoundaries       ( const Instance*, const Box&, const Transformation& );
              bool                    isDrawable           ( const Name& entryName );
              void                    drawCell             ( const Cell*    , const BasicLayer*, const Box&, const Transformation& );
              void                    drawInstance         ( const Instance*, const BasicLayer*, const Box&, const Transformation& );
              void                    drawSlice            ( const Slice*   , const BasicLayer*, const Box&, const Transformation& );
              void                    drawGo               ( const Go*      , const BasicLayer*, const Box&, const Transformation& );
              void                    drawSegment          ( const Segment* , const BasicLayer*, const Box&, const Transformation& );
              void                    drawContact          ( const Contact* , const BasicLayer*, const Box&, const Transformation& );
              void                    drawPad              ( const Pad*     , const BasicLayer*, const Box&, const Transformation& );
              void                    drawBox              ( const Box& );
              void                    drawLine             ( const Point&, const Point& );
              void                    drawGrid             ();
              void                    drawSpot             ();
    // Geometric conversions.
              QRect                   dbuToDisplayRect     ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 ) const;
              QRect                   dbuToDisplayRect     ( const Box& box ) const;
              QPoint                  dbuToDisplayPoint    ( DbU::Unit x, DbU::Unit y ) const;
              QPoint                  dbuToDisplayPoint    ( const Point& point ) const;
      inline  int                     dbuToDisplayX        ( DbU::Unit x ) const;
      inline  int                     dbuToDisplayY        ( DbU::Unit y ) const;
      inline  int                     dbuToDisplayLength   ( DbU::Unit length ) const;
      inline  int                     dbuToScreenX         ( DbU::Unit x ) const;
      inline  int                     dbuToScreenY         ( DbU::Unit y ) const;
              QPoint                  dbuToScreenPoint     ( DbU::Unit x, DbU::Unit y ) const;
      inline  QPoint                  dbuToScreenPoint     ( const Point& point ) const;
      inline  DbU::Unit               displayToDbuX        ( int  x ) const;
      inline  DbU::Unit               displayToDbuY        ( int  y ) const;
      inline  DbU::Unit               displayToDbuLength   ( int  length ) const;
      inline  Box                     displayToDbuBox      ( const QRect& rect ) const;
      inline  DbU::Unit               screenToDbuX         ( int  x ) const;
      inline  DbU::Unit               screenToDbuY         ( int  y ) const;
      inline  Point                   screenToDbuPoint     ( const QPoint& point ) const;
    // Qt QWidget Functions Overloads.
              void                    pushCursor           ( Qt::CursorShape cursor );
              void                    popCursor            ();
              QSize                   minimumSizeHint      () const;
              void                    paintEvent           ( QPaintEvent* );
              void                    resizeEvent          ( QResizeEvent* );
              void                    keyPressEvent        ( QKeyEvent* );
              void                    mouseMoveEvent       ( QMouseEvent* );
              void                    mousePressEvent      ( QMouseEvent* );
              void                    mouseReleaseEvent    ( QMouseEvent* );
    signals:
              void                    mousePositionChanged ( const Point& position );
    public slots:
    // Qt QWidget Slots Overload & CellWidget Specifics.
      inline  DrawingPlanes&          getDrawingPlanes     ();
      inline  QPoint&                 getOffsetVA          ();
              void                    setShowSelection     ( bool state );
      inline  void                    redraw               ();
              void                    redraw               ( QRect redrawArea );
      inline  void                    redrawSelection      ();
              void                    redrawSelection      ( QRect redrawArea );
              void                    goLeft               ( int dx = 0 );
              void                    goRight              ( int dx = 0 );
              void                    goUp                 ( int dy = 0 );
              void                    goDown               ( int dy = 0 );
              void                    fitToContents        ();
              void                    setScale             ( float scale );
              void                    setShowBoundaries    ( bool state );
              void                    reframe              ( const Box& box );
              void                    displayReframe       ();
              void                    shiftLeft            ( int dx );
              void                    shiftRight           ( int dx );
              void                    shiftUp              ( int dy );
              void                    shiftDown            ( int dy );

    private:
      class Spot {
        public:
                      Spot       ( CellWidget* cw );
          void        setRestore ( bool restore );
          void        restore    ();
          void        moveTo     ( const QPoint& point );
        private:
          CellWidget* _cellWidget;
          QPoint      _spotPoint;
          bool        _restore;
      };

    private:
      class DrawingPlanes {
        public:
                                DrawingPlanes ( const QSize& size, CellWidget* cw );
                               ~DrawingPlanes ();
          inline int            width         () const;
          inline int            height        () const;
          inline QSize          size          () const;
          inline void           select        ( size_t i );
          inline QPixmap*       plane         (); 
          inline QPixmap*       plane         ( size_t i ); 
          inline QPainter&      painter       (); 
          inline QPainter&      painter       ( size_t i ); 
          inline void           painterBegin  ();
          inline void           painterBegin  ( size_t i );
          inline void           paintersBegin ();
          inline void           painterEnd    ();
          inline void           painterEnd    ( size_t i );
          inline void           paintersEnd   ();
                 void           resize        ( const QSize& size );
                 void           shiftLeft     ( int dx );
                 void           shiftRight    ( int dx );
                 void           shiftUp       ( int dy );
                 void           shiftDown     ( int dy );
          inline void           copyToSelect  ();
                 void           copyToSelect  ( int sx, int sy, int h, int w );
          inline void           copyToScreen  ();
                 void           copyToScreen  ( int sx, int sy, int h, int w );
        private:
                 CellWidget*    _cellWidget;
                 QPixmap*       _planes[2];
                 QPainter       _painters[3];
                 size_t         _workingPlane;
        private:
                                DrawingPlanes ( const DrawingPlanes& );
                 DrawingPlanes& operator=     ( const DrawingPlanes& );
      };

    protected:
    // Internal: Attributes.
      static  const int               _stripWidth;
              vector<Qt::CursorShape> _cursors;
    //        MapView*                _mapView;
              Technology*             _technology;
              HPalette*               _palette;
              Box                     _displayArea;
              Box                     _visibleArea;
              float                   _scale;
              QPoint                  _offsetVA;
              DrawingPlanes           _drawingPlanes;
              QPoint                  _lastMousePosition;
              Spot                    _spot;
              Cell*                   _cell;
              bool                    _mouseGo;
              bool                    _showBoundaries;
              bool                    _showSelection;
              bool                    _selectionHasChanged;
              set<Selector*>          _selectors;
              size_t                  _redrawRectCount;
  };




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
    else  _painters[i].begin ( _cellWidget );
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


  inline  CellWidget::DrawingPlanes& CellWidget::getDrawingPlanes ()
  { return _drawingPlanes; }


  inline  set<Selector*>& CellWidget::getSelectorSet ()
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


  inline Cell* CellWidget::getCell () const
  { return _cell; }


  inline HPalette* CellWidget::getPalette ()
  { return _palette; }


  inline bool  CellWidget::showBoundaries () const
  { return _showBoundaries; }


  inline  bool  CellWidget::showSelection  () const
  { return _showSelection; }


} // End of Hurricane namespace.


# endif
