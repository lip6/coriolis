
// -*- C++ -*-


# include  <QPainter>

# include  "BasicLayer.h"

# include  "ScreenUtilities.h"
# include  "ScreenLayer.h"




namespace Hurricane {


LayerSample::LayerSample ( ScreenLayer* layer )
  : QWidget()
  , _sample(QSize(20,20))
  , _layer(layer)
{
  setAttribute  ( Qt::WA_StaticContents );
  setSizePolicy ( QSizePolicy::Fixed, QSizePolicy::Fixed );
  setFixedSize  ( 20, 20 );

  redraw ();
}


void  LayerSample::redraw ()
{
  QPainter  painter ( &_sample );

  painter.setPen        ( Qt::NoPen );
  painter.setBackground ( QBrush(Qt::black,Qt::SolidPattern) );
  painter.eraseRect     ( 0, 0, 20, 20 );
  painter.setBrush      ( _layer->getBrush() );
  painter.drawRect      ( 2, 2, 16, 16 );
}


void  LayerSample::paintEvent ( QPaintEvent* )
{
  QPainter painter ( this );
  painter.drawPixmap ( 0, 0, _sample );
}


ScreenLayer::ScreenLayer ( BasicLayer* basicLayer
                         , size_t      index
                         , bool        isVisible
                         ) : _layer(basicLayer)
                           , _index(index)
                           , _isVisible(isVisible)
                           , _brush(H::getBrush(_layer))
{}


const Name& ScreenLayer::getName () const
{
  return _layer->getName ();
}


LayerSample*  ScreenLayer::getSample ()
{
  return new LayerSample ( this );
}


} // End of Hurricane namespace.
