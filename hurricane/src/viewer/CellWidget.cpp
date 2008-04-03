
// -*- C++ -*-


# include <QMouseEvent>
# include <QKeyEvent>
# include <QAction>
# include <QPainter>
# include <QStylePainter>
# include <QBitmap>

# include "DataBase.h"
# include "Technology.h"
# include "BasicLayer.h"
# include "Cell.h"
# include "Instance.h"
# include "Slice.h"
# include "Segment.h"
# include "Contact.h"

# include "ScreenLayer.h"
# include "LayersList.h"
// # include "MapView.h"
# include "CellWidget.h"




namespace Hurricane {


const int  CellWidget::_stripWidth = 100;




CellWidget::CellWidget ( Cell* cell ) : QWidget()
                                      , _styles()
                                      , _activeStyle(NULL)
                                      , _screenLayers()
                                      , _layersList(NULL)
                                      , _displayArea(0,0,6*_stripWidth,6*_stripWidth)
                                      , _visibleArea(_stripWidth,_stripWidth,4*_stripWidth,4*_stripWidth)
                                      , _scale(1.0)
                                      , _offsetVA(_stripWidth,_stripWidth)
                                      , _drawingBuffer(6*_stripWidth,6*_stripWidth)
                                      , _painter()
                                      , _lastMousePosition(0,0)
                                      , _cell(cell)
                                      , _mouseGo(false)
                                      , _openCell(true)
                                      , _showBoundaries(true)
{
  setAttribute   ( Qt::WA_OpaquePaintEvent );
  setAttribute   ( Qt::WA_NoSystemBackground );
  setAttribute   ( Qt::WA_PaintOnScreen );
  setAttribute   ( Qt::WA_StaticContents );
  setSizePolicy  ( QSizePolicy::Expanding, QSizePolicy::Expanding );
  setFocusPolicy ( Qt::StrongFocus );
//setAutoFillBackground ( false );

  // Build ScreenLayer vector here from BasicLayers.
  DataBase* database = getDataBase();
  if (database) {
    Technology* technology = database->getTechnology();
    if (technology) {
      for_each_basic_layer ( layer, technology->getBasicLayers() ) {
        _screenLayers.push_back ( new ScreenLayer(layer,_screenLayers.size(),true) );
        end_for;
      }
    }
  }

//_mapView    = new MapView    ( this );
  _layersList = new LayersList ( this );

  _styles [ "Alliance" ] = DisplayStyle ();
  _activeStyle = &_styles [ "Alliance" ];

  fitToContents ();
  _openCell = false;
}


CellWidget::~CellWidget ()
{
  cerr << "CellWidget::~CellWidget()" << endl;

  for ( size_t i=0 ; i < _screenLayers.size() ; i++ )
    delete _screenLayers[i];
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
//cerr << "CellWidget::redraw ()" << endl;

  pushCursor ( Qt::BusyCursor );

  _painter.begin ( &_drawingBuffer );

  _painter.setPen        ( Qt::NoPen );
  _painter.setBackground ( _activeStyle->getBrush("background") );
  _painter.setClipRect   ( redrawArea );
  _painter.eraseRect     ( redrawArea );

  Box  redrawBox = getBox ( redrawArea );

//   cerr << "  Box:  " << redrawBox << endl;
//   cerr << "  Rect: " << redrawArea.x() << ","
//                      << redrawArea.y() << " "
//                      << redrawArea.width() << "x"
//                      << redrawArea.height() << endl;

  for ( size_t i=0 ; i<_screenLayers.size() ; i++ ) {
    if ( isDrawable(_screenLayers[i]) ) {
      _painter.setBrush ( _screenLayers[i]->getBrush() );
      drawCell ( _cell, _screenLayers[i]->getBasicLayer(), redrawBox, Transformation() );
    }
  }

  if ( _showBoundaries ) {
    _painter.setPen   ( getPen  ("boundaries") );
    _painter.setBrush ( getBrush("boundaries") );
    drawBoundaries ( _cell, redrawBox, Transformation() );
  }

  _painter.end ();

  update ();
  popCursor ();

//   cerr << "*******************************************************" << endl;
}


void  CellWidget::addStyle ( const string& key, DisplayStyle& displayStyle )
{
  if ( _styles.find(key) != _styles.end() )
    cerr << "[WARNING] CellWidget::addStyle(): overriding style \"" << key << "\"." << endl;

  _styles [ key ] = displayStyle;
  _activeStyle = & _styles [ key ];
}


void  CellWidget::setStyle ( const string& key )
{
  if ( _styles.find(key) == _styles.end() ) {
    cerr << "[WARNING] CellWidget::setStyle(): no style named \"" << key << "\"." << endl;
    return;
  }

  _activeStyle = & _styles [ key ];
}


void  CellWidget::drawBoundaries ( const Cell*           cell
                                 , const Box&            redrawArea
                                 , const Transformation& transformation
                                 )
{
//   cerr << "CellWidget::drawBoudaries ()" << endl;

//   QRect rect = getScreenRect(transformation.getBox(cell->getAbutmentBox()));

//   cerr << "  Box:  " << transformation.getBox(cell->getAbutmentBox()) << endl;
//   cerr << "  Rect: " << rect.x() << "+" << rect.y()
//        << "," << rect.width() << "x" << rect.height() << endl;

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


bool  CellWidget::isDrawable ( const ScreenLayer* screenLayer )
{
  return      screenLayer->isVisible()
         && ( screenLayer->getBasicLayer()->getDisplayThreshold() < _scale*100 );
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
}


void CellWidget::drawSegment ( const Segment*        segment
                             , const BasicLayer*     basicLayer
                             , const Box&            redrawArea
                             , const Transformation& transformation
                             )
{
  if ( 1 < getScreenLength(segment->getWidth()) ) {
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


void  CellWidget::drawBox ( const Box& box )
{
  _painter.drawRect ( getScreenRect(box) );
}


void  CellWidget::drawLine ( const Point& p1, const Point& p2 )
{
  _painter.drawLine ( getScreenPoint(p1), getScreenPoint(p2) );
}


void  CellWidget::goLeft ( int dx )
{
  if ( !dx ) dx = geometry().size().width() / 4;
  if (  _offsetVA.rx() - dx >= 0 ) {
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

  if (  _offsetVA.rx() + dx < 2*_stripWidth ) {
    _offsetVA.rx() += dx;
    update ();
  } else {
    shiftRight ( dx );
  }
}


void  CellWidget::goUp ( int dy )
{
  if ( !dy ) dy = geometry().size().height() / 4;
  if (  _offsetVA.ry() - dy >= 0 ) {
    _offsetVA.ry() -= dy;
    update ();
  } else {
    shiftUp ( dy );
  }
}


void  CellWidget::goDown ( int dy )
{
  if ( !dy ) dy = geometry().size().height() / 4;
  if (  _offsetVA.ry() + dy < 2*_stripWidth ) {
    _offsetVA.ry() += dy;
    update ();
  } else {
    shiftDown ( dy );
  }
}


void  CellWidget::screenReframe ()
{
  _offsetVA.rx() = _stripWidth;
  _offsetVA.ry() = _stripWidth;

  Unit xmin = (Unit)( _visibleArea.getXMin() - ((float)_offsetVA.x()/_scale) );
  Unit xmax = (Unit)( xmin + ((float)_drawingBuffer.width()/_scale) ) ;
  Unit ymax = (Unit)( _visibleArea.getYMax() + ((float)_offsetVA.y()/_scale) );
  Unit ymin = (Unit)( ymax - ((float)_drawingBuffer.height()/_scale) ) ;
                   
  _displayArea = Box ( xmin, ymin, xmax, ymax );

  redraw ();
}


void  CellWidget::setScale ( float scale )
{
  _scale = scale;

  Point center = _visibleArea.getCenter();

  _visibleArea.makeEmpty ();
  _visibleArea.merge ( (Unit)( center.getX() - width () / (_scale*2) ) 
                     , (Unit)( center.getY() - height() / (_scale*2) ) 
                     , (Unit)( center.getX() + width () / (_scale*2) )
                     , (Unit)( center.getY() + height() / (_scale*2) )
                     );

//cerr << "_visibleArea: " << _visibleArea << " (offset: " << _offsetVA.x() << ")" << endl;
//cerr << "              " << center << endl;

  screenReframe ();
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

//cerr << "  _scale := " << _scale << endl;

  Point center = box.getCenter();

  width  /= 2;
  height /= 2;

  _visibleArea = Box ( (Unit)( center.getX() - width  / _scale ) 
                     , (Unit)( center.getY() - height / _scale ) 
                     , (Unit)( center.getX() + width  / _scale )
                     , (Unit)( center.getY() + height / _scale )
                     );
  screenReframe ();

//cerr << "  _displayArea: " << _displayArea << " (offset: " << _offsetVA.x() << ")" << endl;
}


void  CellWidget::fitToContents ()
{
  reframe ( _cell->getBoundingBox() );
}


void  CellWidget::shiftLeft ( int dx )
{
//cerr << "CellWidget::shiftLeft() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

  int  leftShift  = ( 1 + ( dx - _offsetVA.rx() ) / _stripWidth ) * _stripWidth;

  _displayArea.translate ( - (Unit)( leftShift / _scale ) , 0 );
  _visibleArea.translate ( - (Unit)( leftShift / _scale ) , 0 );
  _offsetVA.rx() -= dx - leftShift;

  if ( leftShift >= _drawingBuffer.width() ) {
    redraw ();
  } else {
  //cerr << "Left Shift " << leftShift << " (offset: " << _offsetVA.rx() << ")" << endl;
    _painter.begin ( &_drawingBuffer );
    _painter.drawPixmap ( leftShift, 0
                        , _drawingBuffer
                        , 0, 0
                        , _drawingBuffer.width()-leftShift, _drawingBuffer.height()
                        );
    _painter.end ();

    redraw ( QRect ( QPoint ( 0, 0 )
                   , QSize  ( leftShift, _drawingBuffer.height() )) );
  }

  assert ( _offsetVA.rx() >= 0 );
}


void  CellWidget::shiftRight ( int dx )
{
//cerr << "CellWidget::shiftRight() - " << dx << " (offset: " << _offsetVA.rx() << ")" << endl;

  int  rightShift  = ( ( _offsetVA.rx() + dx ) / _stripWidth ) * _stripWidth;

  _displayArea.translate ( (Unit)( rightShift / _scale ) , 0 );
  _visibleArea.translate ( (Unit)( rightShift / _scale ) , 0 );
  _offsetVA.rx() += dx - rightShift;

//cerr << "  _displayArea: " << _displayArea << endl;

  if ( rightShift >= _drawingBuffer.width() ) {
    redraw ();
  } else {
  //cerr << "  Right Shift " << rightShift << " (offset: " << _offsetVA.rx() << ")" << endl;
    _painter.begin ( &_drawingBuffer );
    _painter.drawPixmap ( 0, 0
                        , _drawingBuffer
                        , rightShift, 0
                        , _drawingBuffer.width()-rightShift, _drawingBuffer.height()
                        );
    _painter.end ();

    redraw ( QRect ( QPoint ( _drawingBuffer.width()-rightShift, 0 )
                   , QSize  ( rightShift, _drawingBuffer.height() )) );
  }

  assert ( _offsetVA.rx() >= 0 );
}


void  CellWidget::shiftUp ( int dy )
{
//cerr << "CellWidget::shiftUp() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

  int  upShift  = ( 1 + ( dy - _offsetVA.ry() ) / _stripWidth ) * _stripWidth;

  _displayArea.translate ( 0, (Unit)( upShift / _scale ) );
  _visibleArea.translate ( 0, (Unit)( upShift / _scale ) );
  _offsetVA.ry() -= dy - upShift;

  if ( upShift >= _drawingBuffer.height() ) {
    redraw ();
  } else {
  //cerr << "Left Shift " << upShift << " (offset: " << _offsetVA.ry() << ")" << endl;
    _painter.begin ( &_drawingBuffer );
    _painter.drawPixmap ( 0, upShift
                        , _drawingBuffer
                        , 0, 0
                        , _drawingBuffer.width(), _drawingBuffer.height()-upShift
                        );
    _painter.end ();

    redraw ( QRect ( QPoint ( 0, 0 )
                   , QSize  ( _drawingBuffer.width(), upShift )) );
  }

  assert ( _offsetVA.ry() >= 0 );
}


void  CellWidget::shiftDown ( int dy )
{
//cerr << "CellWidget::shiftDown() - " << dy << " (offset: " << _offsetVA.ry() << ")" << endl;

  int  downShift  = (  ( _offsetVA.ry() + dy ) / _stripWidth ) * _stripWidth;

  _displayArea.translate ( 0, - (Unit)( downShift / _scale ) );
  _visibleArea.translate ( 0, - (Unit)( downShift / _scale ) );
  _offsetVA.ry() += dy - downShift;

  if ( downShift >= _drawingBuffer.height() ) {
    redraw ();
  } else {
  //cerr << "Right Shift " << downShift << " (offset: " << _offsetVA.ry() << ")" << endl;
    _painter.begin ( &_drawingBuffer );
    _painter.drawPixmap ( 0, 0
                        , _drawingBuffer
                        , 0, downShift
                        , _drawingBuffer.width(), _drawingBuffer.height()-downShift
                        );
    _painter.end ();

    redraw ( QRect ( QPoint ( 0, _drawingBuffer.height()-downShift )
                   , QSize  ( _drawingBuffer.width(), downShift )) );
  }

  assert ( _offsetVA.ry() >= 0 );
}


void  CellWidget::paintEvent ( QPaintEvent* )
{
//cerr << "CellWidget::paintEvent()" << endl;

  QPainter painter ( this );
  painter.drawPixmap ( 0, 0, _drawingBuffer, _offsetVA.rx(), _offsetVA.ry(), width(), height() );
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
    _displayArea.inflate ( 0, 0, (Unit)(uaDelta.width()/_scale), (Unit)(uaDelta.height()/_scale) );
    _visibleArea.inflate ( 0, 0, (Unit)(uaDelta.width()/_scale), (Unit)(uaDelta.height()/_scale) );
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
//cerr << "CellWidget::mouseMoveEvent()" << endl;

  if ( _mouseGo ) {
    int  dx = event->x() - _lastMousePosition.x();
  //cerr << "dX (px): " << dx << " _offsetVA.rx() " << _offsetVA.rx() << endl;
    if ( dx > 0 ) goLeft  (  dx );
    if ( dx < 0 ) goRight ( -dx );

    int dy = event->y() - _lastMousePosition.y();
  //cerr << "dY (px): " << dy << " _offsetVA.ry() " << _offsetVA.ry() << endl;
    if ( dy > 0 ) goUp   (  dy );
    if ( dy < 0 ) goDown ( -dy );

    _lastMousePosition = event->pos();
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


QPoint  CellWidget::getScreenPoint ( Unit x, Unit y ) const
{
  return QPoint ( getScreenX(x), getScreenY(y) );
}


QPoint  CellWidget::getScreenPoint ( const Point& point ) const
{
  return getScreenPoint ( point.getX(), point.getY() );
}


QRect  CellWidget::getScreenRect ( Unit x1, Unit y1, Unit x2, Unit y2 ) const
{
  return QRect ( getScreenX(x1)
               , getScreenY(y2)
               , getScreenX(x2) - getScreenX(x1)
               , getScreenY(y1) - getScreenY(y2)
               );
}


QRect  CellWidget::getScreenRect ( const Box& box ) const
{
  return getScreenRect ( box.getXMin()
                       , box.getYMin()
                       , box.getXMax()
                       , box.getYMax()
                       );
}


void  CellWidget::setShowBoundaries ( bool state )
{
  if ( _showBoundaries != state ) {
    _showBoundaries = state;
    redraw ();
  }
}


} // End of Hurricane namespace.
