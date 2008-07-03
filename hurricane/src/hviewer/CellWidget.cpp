
// -*- C++ -*-


# include <QMouseEvent>
# include <QKeyEvent>
# include <QAction>
# include <QPainter>
# include <QStylePainter>
# include <QBitmap>
# include <QLabel>
# include <QStatusBar>

# include "hurricane/DataBase.h"
# include "hurricane/Technology.h"
# include "hurricane/BasicLayer.h"
# include "hurricane/Cell.h"
# include "hurricane/Instance.h"
# include "hurricane/Slice.h"
# include "hurricane/Segment.h"
# include "hurricane/Contact.h"
# include "hurricane/Pad.h"

# include "hurricane/viewer/Graphics.h"
# include "hurricane/viewer/PaletteEntry.h"
# include "hurricane/viewer/Palette.h"
# include "hurricane/viewer/DynamicLabel.h"
// # include "MapView.h"
# include "hurricane/viewer/CellWidget.h"




namespace Hurricane {


// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget::Spot".


  CellWidget::Spot::Spot ( CellWidget* cw )
    : _cellWidget(cw)
    , _spotPoint()
    , _restore(false)
  { }


  void  CellWidget::Spot::restore ()
  {
    if ( _restore ) {
      _cellWidget->copyToScreen ( _spotPoint.x()-5, _spotPoint.y()-5, 10, 10 );
      _restore = false;
    }
  }


  void  CellWidget::Spot::setRestore ( bool state )
  {
    _restore = state;
  }


  void  CellWidget::Spot::moveTo ( const QPoint& screenPoint )
  {
    QPainter& screenPainter = _cellWidget->getScreenPainter();

    Point mousePoint = _cellWidget->screenToDbuPoint ( screenPoint );
    Point  spotPoint = Point ( DbU::getOnSnapGrid(mousePoint.getX())
                             , DbU::getOnSnapGrid(mousePoint.getY())
                             );
    QPoint center = _cellWidget->dbuToScreenPoint(spotPoint);

    restore ();
    _restore = true;

    screenPainter.setPen ( Graphics::getPen("spot") );
    screenPainter.drawRect ( center.x()-3, center.y()-3, 6, 6 );
  }



// -------------------------------------------------------------------
// Class :  "Hurricane::CellWidget".


  const int  CellWidget::_stripWidth = 100;


  CellWidget::CellWidget ( QWidget* parent ) : QWidget(parent)
                                             , _statusBar(NULL)
                                             , _palette(NULL)
                                             , _xPosition(NULL)
                                             , _yPosition(NULL)
                                             , _displayArea(0,0,6*_stripWidth,6*_stripWidth)
                                             , _visibleArea(_stripWidth,_stripWidth,4*_stripWidth,4*_stripWidth)
                                             , _scale(1.0)
                                             , _offsetVA(_stripWidth,_stripWidth)
                                             , _drawingBuffer(6*_stripWidth,6*_stripWidth)
                                             , _drawingPainter()
                                             , _screenPainter()
                                             , _lastMousePosition(0,0)
                                             , _spot(this)
                                             , _cell(NULL)
                                             , _mouseGo(false)
                                             , _showBoundaries(true)
                                             , _redrawRectCount(0)
  {
  //setBackgroundRole ( QPalette::Dark );
  //setAutoFillBackground ( false );
    setAttribute     ( Qt::WA_OpaquePaintEvent );
  //setAttribute     ( Qt::WA_NoSystemBackground );
  //setAttribute     ( Qt::WA_PaintOnScreen );
  //setAttribute     ( Qt::WA_StaticContents );
    setSizePolicy    ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy   ( Qt::StrongFocus );
    setMouseTracking ( true );

    _statusBar = new QStatusBar ( this );

    _xPosition = new DynamicLabel ();
    _xPosition->setStaticText  ( "X:" );
    _xPosition->setDynamicText ( "0l" );

    _yPosition = new DynamicLabel ();
    _yPosition->setStaticText  ( "Y:" );
    _yPosition->setDynamicText ( "0l" );

    _statusBar->addPermanentWidget ( _xPosition );
    _statusBar->addPermanentWidget ( _yPosition );

  //_mapView = new MapView ( this );
    _palette = new Palette ( this );

    fitToContents ();
  }


  CellWidget::~CellWidget ()
  {
    cerr << "CellWidget::~CellWidget()" << endl;
  }


  void  CellWidget::pushCursor ( Qt::CursorShape cursor )
  {
    setCursor ( cursor );
    _cursors.push_back ( cursor );
  }


  void  CellWidget::popCursor ()
  {
    _cursors.pop_back ();
    if ( !_cursors.empty() )
      setCursor ( _cursors.back() );
    else
      unsetCursor ();
  }


  QSize  CellWidget::minimumSizeHint () const
  {
    return QSize(_stripWidth*4,_stripWidth*4);
  }


  void  CellWidget::redraw ( QRect redrawArea )
  {
    _redrawRectCount = 0;

    pushCursor ( Qt::BusyCursor );

    _spot.setRestore ( false );
    _drawingPainter.begin ( &_drawingBuffer );

    _drawingPainter.setPen        ( Qt::NoPen );
    _drawingPainter.setBackground ( Graphics::getBrush("background") );
    _drawingPainter.setClipRect   ( redrawArea );
    _drawingPainter.eraseRect     ( redrawArea );

    if ( _cell ) {
      Box  redrawBox = displayToDbuBox ( redrawArea );

      vector<PaletteEntry*>& paletteEntries = _palette->getEntries ();
      for ( size_t i=0 ; i<paletteEntries.size() ; i++ ) {
        _drawingPainter.setPen   ( Graphics::getPen  (paletteEntries[i]->getName()) );
        _drawingPainter.setBrush ( Graphics::getBrush(paletteEntries[i]->getName()) );

        if ( paletteEntries[i]->isBasicLayer() && isDrawable(paletteEntries[i]) ) {
          drawCell ( _cell, paletteEntries[i]->getBasicLayer(), redrawBox, Transformation() );
        } else if ( (paletteEntries[i]->getName() == DisplayStyle::Boundaries)
                  && paletteEntries[i]->isChecked() ) {
          drawBoundaries ( _cell, redrawBox, Transformation() );
        }
      }
    }

    _drawingPainter.end ();

    update ();
    popCursor ();

    //cerr << "Redrawed rectangles: " << _redrawRectCount << endl;
  }


  void  CellWidget::drawBoundaries ( const Cell*           cell
                                   , const Box&            redrawArea
                                   , const Transformation& transformation
                                   )
  {
    drawBox ( transformation.getBox(cell->getAbutmentBox()) );
    for_each_instance ( instance, cell->getInstances() ) {
      drawBoundaries ( instance, redrawArea, transformation );
      end_for;
    }
  }


  void  CellWidget::drawBoundaries ( const Instance*       instance
                                   , const Box&            redrawArea
                                   , const Transformation& transformation
                                   )
  {
    Box            masterArea           = redrawArea;
    Transformation masterTransformation = instance->getTransformation();

    instance->getTransformation().getInvert().applyOn ( masterArea );
    transformation.applyOn ( masterTransformation );

    drawBoundaries ( instance->getMasterCell(), masterArea, masterTransformation );
  }


  bool  CellWidget::isDrawable ( PaletteEntry* entry )
  {
    return entry->isChecked()
      && ( Graphics::getThreshold(entry->getName())/DbU::lambda(1.0) < _scale );
  }


  void  CellWidget::drawCell ( const Cell*           cell
                             , const BasicLayer*     basicLayer
                             , const Box&            redrawArea
                             , const Transformation& transformation
                             ) 
  {
    for_each_instance ( instance, cell->getInstancesUnder(redrawArea) ) {
      drawInstance ( instance, basicLayer, redrawArea, transformation );
      end_for;
    }

    for_each_slice ( slice, cell->getSlices() ) {
      drawSlice ( slice, basicLayer, redrawArea, transformation );
      end_for;
    }
  }


  void CellWidget::drawInstance ( const Instance*       instance
                                , const BasicLayer*     basicLayer
                                , const Box&            redrawArea
                                , const Transformation& transformation
                                )
  {
    Box            masterArea           = redrawArea;
    Transformation masterTransformation = instance->getTransformation();

    instance->getTransformation().getInvert().applyOn ( masterArea );
    transformation.applyOn ( masterTransformation );

    drawCell ( instance->getMasterCell(), basicLayer, masterArea, masterTransformation );
  }


  void CellWidget::drawSlice ( const Slice*          slice
                             , const BasicLayer*     basicLayer
                             , const Box&            redrawArea
                             , const Transformation& transformation
                             )
  {
    if ( slice->getLayer()->contains(basicLayer) ) {
      if ( slice->getBoundingBox().intersect(redrawArea) ) {
        for_each_go ( go, slice->getGosUnder(redrawArea) ) {
          drawGo ( go, basicLayer, redrawArea, transformation );
          end_for;
        }
      }
    }
  }


  void CellWidget::drawGo ( const Go*             go
                          , const BasicLayer*     basicLayer
                          , const Box&            redrawArea
                          , const Transformation& transformation
                          )
  {
    const Segment* segment = dynamic_cast<const Segment*>(go);
    if (segment) {
      drawSegment ( segment, basicLayer, redrawArea, transformation );
      return;
    }

    const Contact* contact = dynamic_cast<const Contact*>(go);
    if (contact) {
      drawContact ( contact, basicLayer, redrawArea, transformation );
      return;
    }

    const Pad* pad = dynamic_cast<const Pad*>(go);
    if (pad) {
      drawPad ( pad, basicLayer, redrawArea, transformation );
      return;
    }
  }


  void CellWidget::drawSegment ( const Segment*        segment
                               , const BasicLayer*     basicLayer
                               , const Box&            redrawArea
                               , const Transformation& transformation
                               )
  {
    if ( 1 < dbuToDisplayLength(segment->getWidth()) ) {
      drawBox ( transformation.getBox(segment->getBoundingBox(basicLayer)) );
    } else {
      drawLine ( transformation.getPoint(segment->getSourcePosition()),
                 transformation.getPoint(segment->getTargetPosition()) );
    }
  }


  void CellWidget::drawContact ( const Contact*        contact
                               , const BasicLayer*     basicLayer
                               , const Box&            redrawArea
                               , const Transformation& transformation
                               )
  {
    drawBox ( transformation.getBox(contact->getBoundingBox(basicLayer)) );
  }

  void CellWidget::drawPad ( const Pad*            pad
                           , const BasicLayer*     basicLayer
                           , const Box&            redrawArea
                           , const Transformation& transformation
                           )
  {
    drawBox ( transformation.getBox(pad->getBoundingBox(basicLayer)) );
  }


  void  CellWidget::drawBox ( const Box& box )
  {
    _redrawRectCount++;
    _drawingPainter.drawRect ( dbuToDisplayRect(box) );
  }


  void  CellWidget::drawLine ( const Point& p1, const Point& p2 )
  {
    _drawingPainter.drawLine ( dbuToDisplayPoint(p1), dbuToDisplayPoint(p2) );
  }


  void  CellWidget::drawGrid ()
  {
    _screenPainter.setPen ( Graphics::getPen("grid") );

    DbU::Unit  gridStep      = DbU::getSnapGridStep();
    DbU::Unit  superGridStep = gridStep*5;
    DbU::Unit  xGrid;
    DbU::Unit  yGrid;
    QPoint     center;

    for ( yGrid = DbU::getOnSnapGrid(_visibleArea.getYMin())
        ; yGrid < _visibleArea.getYMax()
        ; yGrid += gridStep
        ) {
      for ( xGrid = DbU::getOnSnapGrid(_visibleArea.getXMin())
          ; xGrid < _visibleArea.getXMax()
          ; xGrid += gridStep
          ) {
        center = dbuToScreenPoint(xGrid,yGrid);
        if ( (xGrid % superGridStep) || (yGrid % superGridStep) )
          _screenPainter.drawPoint ( center );
        else {
          _screenPainter.drawLine ( center.x()-3, center.y()  , center.x()+3, center.y()   );
          _screenPainter.drawLine ( center.x()  , center.y()-3, center.x()  , center.y()+3 );
        }
      }
    }
  }


  void  CellWidget::goLeft ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

    _visibleArea.translate ( - (DbU::Unit)( dx / _scale ) , 0 );

    if ( _offsetVA.rx() - dx >= 0 ) {
      _offsetVA.rx() -= dx;
      update ();
    } else {
      shiftLeft ( dx );
    }
  }


  void  CellWidget::goRight ( int dx )
  {
    if ( !dx ) dx = geometry().size().width() / 4;

  //cerr << "CellWidget::goRight() - dx: " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;
    _visibleArea.translate ( (DbU::Unit)( dx / _scale ) , 0 );

    if ( _offsetVA.rx() + dx < 2*_stripWidth ) {
      _offsetVA.rx() += dx;
      update ();
    } else {
      shiftRight ( dx );
    }
  }


  void  CellWidget::goUp ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _visibleArea.translate ( 0, (DbU::Unit)( dy / _scale ) );

    if ( _offsetVA.ry() - dy >= 0 ) {
      _offsetVA.ry() -= dy;
      update ();
    } else {
      shiftUp ( dy );
    }
  }


  void  CellWidget::goDown ( int dy )
  {
    if ( !dy ) dy = geometry().size().height() / 4;

    _visibleArea.translate ( 0, - (DbU::Unit)( dy / _scale ) );

    if ( _offsetVA.ry() + dy < 2*_stripWidth ) {
      _offsetVA.ry() += dy;
      update ();
    } else {
      shiftDown ( dy );
    }
  }


  void  CellWidget::displayReframe ()
  {
    _offsetVA.rx() = _stripWidth;
    _offsetVA.ry() = _stripWidth;

    DbU::Unit xmin = (DbU::Unit)( _visibleArea.getXMin() - ((float)_offsetVA.x()/_scale) );
    DbU::Unit xmax = (DbU::Unit)( xmin + ((float)_drawingBuffer.width()/_scale) ) ;
    DbU::Unit ymax = (DbU::Unit)( _visibleArea.getYMax() + ((float)_offsetVA.y()/_scale) );
    DbU::Unit ymin = (DbU::Unit)( ymax - ((float)_drawingBuffer.height()/_scale) ) ;

    _displayArea = Box ( xmin, ymin, xmax, ymax );

    redraw ();
  }


  void  CellWidget::setScale ( float scale )
  {
    _scale = scale;

    Point center = _visibleArea.getCenter();

    _visibleArea.makeEmpty ();
    _visibleArea.merge ( (DbU::Unit)( center.getX() - width () / (_scale*2) ) 
                       , (DbU::Unit)( center.getY() - height() / (_scale*2) ) 
                       , (DbU::Unit)( center.getX() + width () / (_scale*2) )
                       , (DbU::Unit)( center.getY() + height() / (_scale*2) )
                       );

  //cerr << "_visibleArea: " << _visibleArea << " (offset: " << _offsetVA.x() << ")" << endl;
  //cerr << "              " << center << endl;

    displayReframe ();
  }


  void  CellWidget::reframe ( const Box& box )
  {
  //cerr << "CellWidget::reframe() - " << box << endl;
  //cerr << "  widget size := " << _drawingBuffer.width() << "x" << _drawingBuffer.height() << endl;

    int  width  = this->width  ();
    int  height = this->height ();

    float scaleX = width  / (float)box.getWidth ();
    float scaleY = height / (float)box.getHeight();
    _scale = min ( scaleX, scaleY );

    Point center = box.getCenter();

    width  /= 2;
    height /= 2;

    _visibleArea = Box ( (DbU::Unit)( center.getX() - width  / _scale ) 
                       , (DbU::Unit)( center.getY() - height / _scale ) 
                       , (DbU::Unit)( center.getX() + width  / _scale )
                       , (DbU::Unit)( center.getY() + height / _scale )
                       );
    displayReframe ();

  //cerr << "  _displayArea: " << _displayArea << " (offset: " << _offsetVA.x() << ")" << endl;
  }


  void  CellWidget::fitToContents ()
  {
    if ( _cell ) reframe ( _cell->getBoundingBox() );
  }


  void  CellWidget::shiftLeft ( int dx )
  {
  //cerr << "CellWidget::shiftLeft() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

    int  leftShift  = ( 1 + ( dx - _offsetVA.rx() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( - (DbU::Unit)( leftShift / _scale ) , 0 );
    _offsetVA.rx() -= dx - leftShift;

    if ( leftShift >= _drawingBuffer.width() ) {
      redraw ();
    } else {
    //cerr << "Left Shift " << leftShift << " (offset: " << _offsetVA.rx() << ")" << endl;
      _drawingPainter.begin ( &_drawingBuffer );
      _drawingPainter.drawPixmap ( leftShift, 0
                          , _drawingBuffer
                          , 0, 0
                          , _drawingBuffer.width()-leftShift, _drawingBuffer.height()
                          );
      _drawingPainter.end ();

      redraw ( QRect ( QPoint ( 0, 0 )
                     , QSize  ( leftShift, _drawingBuffer.height() )) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::shiftRight ( int dx )
  {
  //cerr << "CellWidget::shiftRight() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

    int  rightShift  = ( ( _offsetVA.rx() + dx ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( (DbU::Unit)( rightShift / _scale ) , 0 );
    _offsetVA.rx() += dx - rightShift;

  //cerr << "  _displayArea: " << _displayArea << endl;

    if ( rightShift >= _drawingBuffer.width() ) {
      redraw ();
    } else {
    //cerr << "  Right Shift " << rightShift << " (offset: " << _offsetVA.rx() << ")" << endl;
      _drawingPainter.begin ( &_drawingBuffer );
      _drawingPainter.drawPixmap ( 0, 0
                          , _drawingBuffer
                          , rightShift, 0
                          , _drawingBuffer.width()-rightShift, _drawingBuffer.height()
                          );
      _drawingPainter.end ();

      redraw ( QRect ( QPoint ( _drawingBuffer.width()-rightShift, 0 )
                     , QSize  ( rightShift, _drawingBuffer.height() )) );
    }

    assert ( _offsetVA.rx() >= 0 );
  }


  void  CellWidget::shiftUp ( int dy )
  {
  //cerr << "CellWidget::shiftUp() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    int  upShift  = ( 1 + ( dy - _offsetVA.ry() ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, (DbU::Unit)( upShift / _scale ) );
    _offsetVA.ry() -= dy - upShift;

    if ( upShift >= _drawingBuffer.height() ) {
      redraw ();
    } else {
    //cerr << "Left Shift " << upShift << " (offset: " << _offsetVA.ry() << ")" << endl;
      _drawingPainter.begin ( &_drawingBuffer );
      _drawingPainter.drawPixmap ( 0, upShift
                          , _drawingBuffer
                          , 0, 0
                          , _drawingBuffer.width(), _drawingBuffer.height()-upShift
                          );
      _drawingPainter.end ();

      redraw ( QRect ( QPoint ( 0, 0 )
                     , QSize  ( _drawingBuffer.width(), upShift )) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::shiftDown ( int dy )
  {
  //cerr << "CellWidget::shiftDown() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

    int  downShift  = (  ( _offsetVA.ry() + dy ) / _stripWidth ) * _stripWidth;

    _displayArea.translate ( 0, - (DbU::Unit)( downShift / _scale ) );
    _offsetVA.ry() += dy - downShift;

    if ( downShift >= _drawingBuffer.height() ) {
      redraw ();
    } else {
    //cerr << "Right Shift " << downShift << " (offset: " << _offsetVA.ry() << ")" << endl;
      _drawingPainter.begin ( &_drawingBuffer );
      _drawingPainter.drawPixmap ( 0, 0
                          , _drawingBuffer
                          , 0, downShift
                          , _drawingBuffer.width(), _drawingBuffer.height()-downShift
                          );
      _drawingPainter.end ();

      redraw ( QRect ( QPoint ( 0, _drawingBuffer.height()-downShift )
                     , QSize  ( _drawingBuffer.width(), downShift )) );
    }

    assert ( _offsetVA.ry() >= 0 );
  }


  void  CellWidget::paintEvent ( QPaintEvent* )
  {
    _screenPainter.begin ( this );

    copyToScreen ();

    if ( isDrawable(_palette->find("grid")) ) drawGrid ();
    if ( isDrawable(_palette->find("spot")) ) _spot.moveTo ( _lastMousePosition );

    _screenPainter.end ();
  }


  void  CellWidget::resizeEvent ( QResizeEvent* )
  {
    QSize uaDelta ( 0, 0 );
    QSize uaSize  = geometry().size();

    uaSize.rwidth () += 2*_stripWidth;
    uaSize.rheight() += 2*_stripWidth;

    if ( uaSize.width () > _drawingBuffer.width () )
      uaDelta.rwidth () = uaSize.width () - _drawingBuffer.width ();

    if ( uaSize.height() > _drawingBuffer.height() )
      uaDelta.rheight() = uaSize.height() - _drawingBuffer.height();

  //cerr << "New UA widget size: " << uaSize.width() << "x" << uaSize.height() << endl;

    if ( uaDelta.width() || uaDelta.height() ) {
      _displayArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/_scale), (DbU::Unit)(uaDelta.height()/_scale) );
      _visibleArea.inflate ( 0, 0, (DbU::Unit)(uaDelta.width()/_scale), (DbU::Unit)(uaDelta.height()/_scale) );
    //cerr << "new      " << _displayArea << endl;

    //cerr << "Previous buffer size: " << _drawingBuffer.width () << "x"
    //                                 << _drawingBuffer.height() << endl;

      QSize bufferSize ( ( ( uaSize.width () / _stripWidth ) + 1 ) * _stripWidth
                       , ( ( uaSize.height() / _stripWidth ) + 1 ) * _stripWidth );
      _drawingBuffer = QPixmap ( bufferSize );

    //cerr << "Effective buffer resize to: " << bufferSize.width() << "x"
    //                                       << bufferSize.height() << endl;
    }
    redraw ();
  }


  void  CellWidget::keyPressEvent ( QKeyEvent* event )
  {
    switch ( event->key() ) {
      case Qt::Key_Up:    goUp     (); break;
      case Qt::Key_Down:  goDown   (); break;
      case Qt::Key_Left:  goLeft   (); break;
      case Qt::Key_Right: goRight  (); break;
      case Qt::Key_Z:     setScale ( _scale*2.0 ); break;
      case Qt::Key_M:     setScale ( _scale/2.0 ); break;
    }
  }


  void  CellWidget::mouseMoveEvent ( QMouseEvent* event )
  {
    if ( _mouseGo ) {
      int  dx = event->x() - _lastMousePosition.x();
      dx <<= 1;
    //cerr << "dX (px): " << dx << " _offsetVA.rx() " << _offsetVA.rx() << endl;
      if ( dx > 0 ) goLeft  (  dx );
      if ( dx < 0 ) goRight ( -dx );

      int dy = event->y() - _lastMousePosition.y();
      dy <<= 1;
    //cerr << "dY (px): " << dy << " _offsetVA.ry() " << _offsetVA.ry() << endl;
      if ( dy > 0 ) goUp   (  dy );
      if ( dy < 0 ) goDown ( -dy );

      _lastMousePosition = event->pos();
    } else {
    //cerr << "x:" << event->x() << " y:" << event->y() << endl;
      _xPosition->setDynamicText ( screenToDbuX(event->x()) );
      _yPosition->setDynamicText ( screenToDbuY(event->y()) );

      _lastMousePosition = event->pos();

      update ();
    }
  }


  void  CellWidget::mousePressEvent ( QMouseEvent* event )
  {
    if ( ( event->button() == Qt::LeftButton ) && !_mouseGo ) {
      _mouseGo           = true;
      _lastMousePosition = event->pos();
      pushCursor ( Qt::ClosedHandCursor );
    }
  }


  void  CellWidget::mouseReleaseEvent ( QMouseEvent* event )
  {
    if ( ( event->button() == Qt::LeftButton ) && _mouseGo ) {
      _mouseGo = false;
      popCursor ();
    }
  }


  QPoint  CellWidget::dbuToDisplayPoint ( DbU::Unit x, DbU::Unit y ) const
  {
    return QPoint ( dbuToDisplayX(x), dbuToDisplayY(y) );
  }


  QPoint  CellWidget::dbuToDisplayPoint ( const Point& point ) const
  {
    return dbuToDisplayPoint ( point.getX(), point.getY() );
  }


  QRect  CellWidget::dbuToDisplayRect ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 ) const
  {
    return QRect ( dbuToDisplayX(x1)
                 , dbuToDisplayY(y2)
                 , dbuToDisplayX(x2) - dbuToDisplayX(x1)
                 , dbuToDisplayY(y1) - dbuToDisplayY(y2)
                 );
  }


  QRect  CellWidget::dbuToDisplayRect ( const Box& box ) const
  {
    return dbuToDisplayRect ( box.getXMin()
                            , box.getYMin()
                            , box.getXMax()
                            , box.getYMax()
                            );
  }


  QPoint  CellWidget::dbuToScreenPoint ( DbU::Unit x, DbU::Unit y ) const
  {
    return QPoint ( dbuToScreenX(x), dbuToScreenY(y) );
  }


  void  CellWidget::setShowBoundaries ( bool state )
  {
    if ( _showBoundaries != state ) {
      _showBoundaries = state;
      redraw ();
    }
  }


  void  CellWidget::setCell ( Cell* cell )
  {
    _cell = cell;
    fitToContents ();
  }


} // End of Hurricane namespace.
