
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
// |  C++ Module  :       "./DisplayStyle.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <cassert>

# include  "hurricane/viewer/DisplayStyle.h"




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
    if ( borderWidth ) {
      _pen.setStyle ( Qt::SolidLine );
      _pen.setWidth ( borderWidth );
    }
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


  QColor  DrawingStyle::getColor ( int darkening ) const
  {
    return _color.darker ( darkening );
  }


  QPen  DrawingStyle::getPen ( int darkening ) const
  {
    QPen pen ( _pen );
    pen.setColor ( _color.darker(darkening) );
    return pen;
  }


  QBrush  DrawingStyle::getBrush ( int darkening ) const
  {
    QBrush brush ( _brush );
    brush.setColor ( _color.darker(darkening) );
    return brush;
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
                                       )
  {
    size_t gi;
    findOrCreate ( groupKey, gi );

    _groups[gi]->addDrawingStyle ( key, pattern, red, green, blue, borderWidth, threshold );
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
