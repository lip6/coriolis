
// -*- C++ -*-


# include  <cassert>

# include  "DisplayStyle.h"




namespace Hurricane {


  const Name  DisplayStyle::UnmatchedGroup;
  const Name  DisplayStyle::Viewer        = "Viewer";
  const Name  DisplayStyle::Fallback      = "fallback";
  const Name  DisplayStyle::Background    = "background";
  const Name  DisplayStyle::Foreground    = "foreground";
  const Name  DisplayStyle::Rubber        = "rubber";
  const Name  DisplayStyle::Phantom       = "phantom";
  const Name  DisplayStyle::Boundaries    = "boundaries";
  const Name  DisplayStyle::Marker        = "marker";
  const Name  DisplayStyle::SelectionDraw = "selectionDraw";
  const Name  DisplayStyle::SelectionFill = "selectionFill";
  const Name  DisplayStyle::Grid          = "grid";
  const Name  DisplayStyle::Spot          = "spot";
  const Name  DisplayStyle::Ghost         = "ghost";
  const Name  DisplayStyle::Text          = "text";
  const Name  DisplayStyle::Undef         = "undef";



  DrawingStyle::DrawingStyle ( const Name&   name
                             , const string& pattern
                             ,       int     red
                             ,       int     green
                             ,       int     blue
                             ,       int     borderWidth
                             ,       float   threshold
                             ) : _name(name)
                               , _pattern(pattern)
                               , _color(QColor(red,green,blue))
                               , _pen(_color)
                               , _brush(Hurricane::getBrush(_pattern,red,green,blue))
                               , _threshold(threshold)
                               , _refcount(1)
  {
    if ( borderWidth )
      _pen.setWidth ( borderWidth );
    else
      _pen.setStyle ( Qt::NoPen );
  }



  DrawingStyle* DrawingStyle::create ( const Name&   name
                                     , const string& pattern
                                     ,       int     red
                                     ,       int     green
                                     ,       int     blue
                                     ,       int     borderWidth
                                     ,       float   threshold
                                     )
  {
    return new DrawingStyle ( name, pattern, red, green, blue, borderWidth, threshold );
  }


  DrawingStyle::~DrawingStyle ()
  {
    assert ( _refcount == 0 );
  }


  size_t  DrawingStyle::unlink ()
  {
    _refcount--;

    if ( !_refcount ) {
      delete this;
      return 0;
    }

    return _refcount;
  }


  DrawingStyle*  DrawingStyle::link ()
  {
    _refcount++;

    return this;
  }


  DrawingGroup::DrawingGroup ( const Name& name )
    : _name(name), _drawingStyles()
  {
  }


  DrawingGroup* DrawingGroup::getClone ()
  {
    DrawingGroup* clone = new DrawingGroup ( getName() );

    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      clone->_drawingStyles.push_back ( _drawingStyles[i]->link() );

    return clone;
  }


  DrawingGroup::~DrawingGroup ()
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      _drawingStyles[i]->unlink ();
  }


  size_t  DrawingGroup::findIndex ( const Name& key ) const
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ ) {
      if ( _drawingStyles[i]->getName() == key )
        return i;
    }

    return InvalidIndex;
  }


  DrawingStyle* DrawingGroup::find ( const Name& key )
  {
    size_t i = findIndex ( key );
    if ( i != InvalidIndex )
      return _drawingStyles[i];

    return NULL;
  }


  DrawingStyle* DrawingGroup::addDrawingStyle ( const Name&   key
                                              , const string& pattern
                                              ,       int     red
                                              ,       int     green
                                              ,       int     blue
                                              ,       int     borderWidth
                                              ,       float   threshold
                                              )
  {
    size_t i = findIndex ( key );
    if ( i != InvalidIndex ) {
      _drawingStyles[i]->unlink ();
    } else {
      i = _drawingStyles.size ();
      _drawingStyles.push_back ( NULL );
    }

    return _drawingStyles[i] =
      DrawingStyle::create ( key, pattern, red, green, blue, borderWidth, threshold );
  }


   DisplayStyle::DisplayStyle ( const Name& name )
     : _name(name), _groups()
  {
    addDrawingStyle ( Viewer, Fallback     , "FFFFFFFFFFFFFFFF",   0,   0,   0, 1, 1.0 );
    addDrawingStyle ( Viewer, Background   , "FFFFFFFFFFFFFFFF",  50,  50,  50, 1, 1.0 );
    addDrawingStyle ( Viewer, Foreground   , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, Rubber       , "FFFFFFFFFFFFFFFF", 192,   0, 192, 1, 1.0 );
    addDrawingStyle ( Viewer, Phantom      , "FFFFFFFFFFFFFFFF", 139, 134, 130, 1, 1.0 );
    addDrawingStyle ( Viewer, Boundaries   , "0000000000000000", 208, 199, 192, 1, 1.0 );
    addDrawingStyle ( Viewer, Marker       , "FFFFFFFFFFFFFFFF",  80, 250,  80, 1, 1.0 );
    addDrawingStyle ( Viewer, SelectionDraw, "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, SelectionFill, "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, Grid         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, Spot         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, Ghost        , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, Text         , "8822441188224411", 255, 255, 255, 0, 1.0 );
    addDrawingStyle ( Viewer, Undef        , "2244118822441188", 238, 130, 238, 0, 1.0 );
  }


  DisplayStyle::~DisplayStyle ()
  {
    for ( size_t i=0 ; i < _groups.size() ; i++ ) {
      delete _groups[i];
    }
  }


  const Name& DisplayStyle::getGroup ( const Name& key ) const
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ ) {
      DrawingStyle* style = _groups[gi]->find ( key );
      if ( style )
        return _groups[gi]->getName();
    }

    return UnmatchedGroup;
  }


  const string& DisplayStyle::getPattern ( const Name& key ) const
  {
    return find(key)->getPattern();
  }


  const QColor& DisplayStyle::getColor ( const Name& key ) const
  {
    return find(key)->getColor();
  }


  const QPen& DisplayStyle::getPen ( const Name& key ) const
  {
    return find(key)->getPen();
  }


  const QBrush& DisplayStyle::getBrush ( const Name& key ) const
  {
    return find(key)->getBrush();
  }


  float  DisplayStyle::getThreshold ( const Name& key ) const
  {
    return find(key)->getThreshold();
  }


  void  DisplayStyle::findOrCreate ( const Name& groupKey, size_t& gi )
  {
    for ( gi=0 ; gi < _groups.size() ; gi++ ) {
      if ( _groups[gi]->getName() == groupKey )
        break;
    }
    if ( gi == _groups.size() )
      _groups.push_back ( new DrawingGroup(groupKey) );
  }


  void  DisplayStyle::find ( const Name& groupKey
                           , const Name& key
                           , size_t&     gi
                           , size_t&     si
                           )
  {
    for ( gi=0 ; gi < _groups.size() ; gi++ ) {
      if ( _groups[gi]->getName() == groupKey ) {
        si = _groups[gi]->findIndex ( key );
        if ( si != InvalidIndex )
          return;
      }
    }

    gi = InvalidIndex;
    si = InvalidIndex;
  }


  DrawingStyle* DisplayStyle::find ( const Name& key ) const
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ ) {
      DrawingStyle* style = _groups[gi]->find ( key );
      if ( style )
        return style;
    }

    return find ( "fallback" );
  }


  void  DisplayStyle::addDrawingStyle  ( const Name&   groupKey
                                       , const Name&   key
                                       , const string& pattern
                                       ,       int     red
                                       ,       int     green
                                       ,       int     blue
                                       ,       int     borderWidth
                                       ,       float   threshold
                                       )
  {
    size_t gi;
    findOrCreate ( groupKey, gi );

    _groups[gi]->addDrawingStyle ( key, pattern, red, green, blue, borderWidth, threshold );
  }


  void  DisplayStyle::inheritFrom ( const DisplayStyle* base )
  {
    assert ( base != NULL );

    for ( size_t gi=0 ; gi < base->_groups.size() ; gi++ ) {
      _groups.push_back ( base->_groups[gi]->getClone() );
    }
  }


}
