
// -*- C++ -*-


# include  "DisplayStyle.h"




namespace Hurricane {



DisplayStyle::DrawingStyle::DrawingStyle ( const string& pattern
                                         ,       int     red
                                         ,       int     green
                                         ,       int     blue
                                         ,       int     borderWidth
                                         ) : _pattern(pattern)
                                           , _color(QColor(red,green,blue))
                                           , _pen(_color)
                                           , _brush(Hurricane::getBrush(_pattern,red,green,blue))
{
  if ( borderWidth ) {
    _pen.setWidth ( borderWidth );
  } else {
    _pen.setStyle ( Qt::NoPen );
  }
}


DisplayStyle::DisplayStyle () : _drawingStyles()
{
  addDrawingStyle ( "fallback"     , "FFFFFFFFFFFFFFFF",   0,   0,   0, 1 );
  addDrawingStyle ( "background"   , "FFFFFFFFFFFFFFFF",  50,  50,  50, 1 );
  addDrawingStyle ( "foreground"   , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
  addDrawingStyle ( "rubber"       , "FFFFFFFFFFFFFFFF", 192,   0, 192, 1 );
  addDrawingStyle ( "phantom"      , "FFFFFFFFFFFFFFFF", 139, 134, 130, 1 );
  addDrawingStyle ( "boundaries"   , "0000000000000000", 208, 199, 192, 1 );
  addDrawingStyle ( "marker"       , "FFFFFFFFFFFFFFFF",  80, 250,  80, 1 );
  addDrawingStyle ( "selectionDraw", "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
  addDrawingStyle ( "selectionFill", "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
  addDrawingStyle ( "grid"         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
  addDrawingStyle ( "spot"         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
  addDrawingStyle ( "ghost"        , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1 );
}


const string& DisplayStyle::getPattern ( const string& key ) const
{
  return findDrawingStyle(key).getPattern();
}


const QColor& DisplayStyle::getColor ( const string& key ) const
{
  return findDrawingStyle(key).getColor();
}


const QPen& DisplayStyle::getPen ( const string& key ) const
{
  return findDrawingStyle(key).getPen();
}


const QBrush& DisplayStyle::getBrush ( const string& key ) const
{
  return findDrawingStyle(key).getBrush();
}


const DisplayStyle::DrawingStyle& DisplayStyle::findDrawingStyle ( const string& key ) const
{
  StyleIterator  it = _drawingStyles.find ( key );
  if ( it != _drawingStyles.end() ) return it->second;

  return _drawingStyles.find("fallback")->second;
}


void  DisplayStyle::addDrawingStyle  ( const string& key
                                     , const string& pattern
                                     ,       int     red
                                     ,       int     green
                                     ,       int     blue
                                     ,       int     borderWidth
                                     )
{
  _drawingStyles [ key ] = DrawingStyle ( pattern, red, green, blue, borderWidth );
}


}
