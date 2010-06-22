
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
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
// |  C++ Module  :       "./DisplayStyle.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cassert>

# include  "hurricane/Error.h"
# include  "hurricane/viewer/DisplayStyle.h"
# include  "hurricane/viewer/Graphics.h"




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
  const Name  DisplayStyle::TextRuler     = "text.ruler";
  const Name  DisplayStyle::TextCell      = "text.cell";
  const Name  DisplayStyle::TextInstance  = "text.instance";
  const Name  DisplayStyle::TextComponent = "text.component";
  const Name  DisplayStyle::TextReference = "text.reference";
  const Name  DisplayStyle::Undef         = "undef";



  DrawingStyle::DrawingStyle ( const Name&   name
                             , const string& pattern
                             ,       int     red
                             ,       int     green
                             ,       int     blue
                             ,       int     borderWidth
                             ,       float   threshold
                             ,       bool    goMatched
                             )
    : _name       (name)
    , _red        (red)
    , _green      (green)
    , _blue       (blue)
    , _borderWidth(borderWidth)
    , _pattern    (pattern)
    , _color      (NULL)
    , _pen        (NULL)
    , _brush      (NULL)
    , _threshold  (threshold)
    , _goMatched  (goMatched)
    , _refcount   (1)
  {
  }



  DrawingStyle* DrawingStyle::create ( const Name&   name
                                     , const string& pattern
                                     ,       int     red
                                     ,       int     green
                                     ,       int     blue
                                     ,       int     borderWidth
                                     ,       float   threshold
                                     ,       bool    goMatched
                                     )
  {
    DrawingStyle* style = new DrawingStyle ( name, pattern, red, green, blue, borderWidth, threshold, goMatched );
  //if ( Graphics::isEnabled() )
  //  style->qtAllocate ();

    return style;
  }


  void  DrawingStyle::qtAllocate ()
  {
    if ( !_color ) {
      _color = new QColor ( _red, _green, _blue );

      _pen = new QPen ();
      if ( _borderWidth ) {
        _pen->setStyle ( Qt::SolidLine );
        _pen->setWidth ( _borderWidth );
      } else
        _pen->setStyle ( Qt::NoPen );
      _pen->setColor ( *_color );

      _brush = Hurricane::getBrush ( _pattern, _red, _green, _blue );
    }
  }


  QColor  DrawingStyle::getColor ( int darkening ) const
  {
    assert ( _color != NULL );

    return _color->darker ( darkening );
  }


  QPen  DrawingStyle::getPen ( int darkening ) const
  {
    assert ( _pen != NULL );

    QPen pen ( *_pen );
    pen.setColor ( _color->darker(darkening) );
    return pen;
  }


  QBrush  DrawingStyle::getBrush ( int darkening ) const
  {
    assert ( _brush != NULL );

    QBrush brush ( *_brush );
    brush.setColor ( _color->darker(darkening) );
    return brush;
  }


  DrawingStyle::~DrawingStyle ()
  {
    assert ( _refcount == 0 );
    if ( _color ) {
      delete _color;
      delete _pen;
      delete _brush;
    }
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
  { }


  void  DrawingGroup::qtAllocate ()
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      _drawingStyles[i]->qtAllocate ();
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


  DrawingStyle* DrawingGroup::find ( const Name& key ) const
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
                                              ,       bool    goMatched
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
      DrawingStyle::create ( key, pattern, red, green, blue, borderWidth, threshold, goMatched );
  }


   DisplayStyle::DisplayStyle ( const Name& name )
     : _name(name)
     , _description("<No Description>")
     , _groups()
     , _darkening(200)
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
    addDrawingStyle ( Viewer, Grid         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 8.0 );
    addDrawingStyle ( Viewer, Spot         , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 8.0 );
    addDrawingStyle ( Viewer, Ghost        , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 1.0 );
    addDrawingStyle ( Viewer, TextRuler    , "FFFFFFFFFFFFFFFF", 255, 255, 255, 1, 0.0 );
    addDrawingStyle ( Viewer, TextCell     , "8822441188224411", 255, 255, 255, 0, 1.0 );
    addDrawingStyle ( Viewer, TextInstance , "8822441188224411", 255, 255, 255, 0, 1.0 );
    addDrawingStyle ( Viewer, TextComponent, "FFFFFFFFFFFFFFFF", 255, 255, 255, 0, 1.0 );
    addDrawingStyle ( Viewer, TextReference, "FFFFFFFFFFFFFFFF", 255, 255, 255, 0, 1.0 );
    addDrawingStyle ( Viewer, Undef        , "2244118822441188", 238, 130, 238, 0, 1.0 );
  }


  DisplayStyle::~DisplayStyle ()
  {
    for ( size_t i=0 ; i < _groups.size() ; i++ ) {
      delete _groups[i];
    }
  }


  void  DisplayStyle::qtAllocate ()
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ )
      _groups[gi]->qtAllocate ();
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


  QColor  DisplayStyle::getColor ( const Name& key, int darkening ) const
  {
    return find(key)->getColor(darkening);
  }


  QPen  DisplayStyle::getPen ( const Name& key, int darkening ) const
  {
    return find(key)->getPen(darkening);
  }


  QBrush  DisplayStyle::getBrush ( const Name& key, int darkening ) const
  {
    return find(key)->getBrush(darkening);
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
                           ) const
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

    if ( key == "fallback" )
      throw Error ("Unable to find builtin \"fallback\" DrawingStyle in %s."
                  ,getString(getName()).c_str());

    return find ( "fallback" );
  }


  void  DisplayStyle::setDarkening ( int darkening )
  {
    if ( darkening <= 0 ) {
      cerr << "[ERROR] Invalid darkening factor: " << darkening << "." << endl;
      return;
    }

    _darkening = darkening;
  }


  void  DisplayStyle::addDrawingStyle  ( const Name&   groupKey
                                       , const Name&   key
                                       , const string& pattern
                                       ,       int     red
                                       ,       int     green
                                       ,       int     blue
                                       ,       int     borderWidth
                                       ,       float   threshold
                                       ,       bool    goMatched
                                       )
  {
    size_t gi;
    findOrCreate ( groupKey, gi );

    _groups[gi]->addDrawingStyle ( key, pattern, red, green, blue, borderWidth, threshold, goMatched );
  }


  void  DisplayStyle::inheritFrom ( const DisplayStyle* base )
  {
    assert ( base != NULL );

    for ( size_t i=0 ; i<_groups.size() ; i++ )
      delete _groups[i];
    _groups.clear ();

    for ( size_t gi=0 ; gi < base->_groups.size() ; gi++ )
      _groups.push_back ( base->_groups[gi]->getClone());
  }


}
