
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
// |  C++ Module  :       "./Graphics.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# include  <assert.h>

# include  <Qt>
# include  <QBrush>
# include  <QPen>
# include  <QApplication>

# include  "hurricane/Name.h"

# include  "hurricane/viewer/DisplayStyle.h"
# include  "hurricane/viewer/Graphics.h"




namespace Hurricane {


  Graphics* Graphics::_singleton = NULL;


  Graphics::Graphics ()
    : _styles()
    , _active(NULL)
    , _fireColorScale()
    , _qtEnabled(false)
  {
  }


  Graphics::~Graphics ()
  {
    delete _singleton;
  }


  Graphics* Graphics::getGraphics ()
  {
    if ( !_singleton ) {
      _singleton = new Graphics ();
      DisplayStyle* fallback = new DisplayStyle("Fallback");
      fallback->setDescription ( "Builtin fallback style" );
      _singleton->_addStyle ( fallback );
    }

    return _singleton;
  }


  const QFont  Graphics::getFixedFont ( int weight, bool italic, bool underline )
  {
    const QFont defaultFont = QApplication::font ();

    QFont fixedFont ( "Bitstream Vera Sans Mono", defaultFont.pointSize() );
    fixedFont.setWeight    ( weight );
    fixedFont.setItalic    ( italic );
    fixedFont.setUnderline ( underline );

    return fixedFont;
  }


  const QFont  Graphics::getNormalFont ( bool bold, bool italic, bool underline )
  {
    QFont defaultFont = QApplication::font ();

    defaultFont.setBold      ( bold );
    defaultFont.setItalic    ( italic );
    defaultFont.setUnderline ( underline );

    return defaultFont;
  }


  void  Graphics::_enable ()
  {
    _qtEnabled = true;
    for ( size_t si=0 ; si < _styles.size() ; si++ )
      _styles[si]->qtAllocate ();

    _fireColorScale.qtAllocate ();
  }


  size_t  Graphics::_findStyle ( const Name& name ) const
  {
    size_t si = 0;
    for ( ; si < _styles.size() ; si++ )
      if ( _styles[si]->getName() == name )
        break;

    return si;
  }



  void  Graphics::_addStyle ( DisplayStyle* displayStyle )
  {
    size_t si = _findStyle(displayStyle->getName());
    if ( si != _styles.size() ) {
      cerr << "[WARNING] Graphics::addStyle(): overriding style \""
           << displayStyle->getName() << "\"." << endl;

      delete _styles [ si ];
    } else {
      _styles.push_back ( NULL );
    }

    _styles [ si ] = displayStyle;
    _active = _styles [ si ];

    if ( _qtEnabled )
      _active->qtAllocate ();
  }


  void  Graphics::_setStyle ( const Name& key )
  {
    size_t si = _findStyle(key);
    if ( si == _styles.size() ) {
      cerr << "[WARNING] Graphics::setStyle(): no style named \"" << key << "\"." << endl;
      return;
    }

    _active = _styles [ si ];
  }


  void  Graphics::_setStyle ( size_t id )
  {
    if ( id >= _styles.size() ) {
      cerr << "[WARNING] Graphics::setStyle(): no style id \"" << id << "\"." << endl;
      return;
    }

    _active = _styles [ id ];
  }


  DisplayStyle* Graphics::_getStyle ( const Name& key )
  {
//  if ( !_qtEnabled )
//    cerr << "[ERROR] Graphics has not been enabled, you are likely to crash..." << endl;

    size_t si = _findStyle(key);
    if ( si == _styles.size() ) 
      return NULL;

    return _styles [ si ];
  }


  bool  Graphics::isEnabled ()
  {
    return getGraphics()->_qtEnabled;
  }


  void  Graphics::enable ()
  {
    return getGraphics()->_enable();
  }


  void  Graphics::addStyle ( DisplayStyle* displayStyle )
  {
    getGraphics()->_addStyle ( displayStyle );
  }


  void  Graphics::setStyle ( const Name& key )
  {
    getGraphics()->_setStyle ( key );
  }


  void  Graphics::setStyle ( size_t id )
  {
    getGraphics()->_setStyle ( id );
  }


  DisplayStyle* Graphics::getStyle ( const Name& key )
  {
    return getGraphics()->_getStyle ( key );
  }


  DisplayStyle* Graphics::getStyle ()
  {
    return getGraphics()->_getStyle ();
  }


  const vector<DisplayStyle*>& Graphics::getStyles ()
  {
    return getGraphics()->_getStyles ();
  }


  const Name& Graphics::getGroup ( const Name& key )
  {
    return getGraphics()->_getGroup ( key );
  }


  QColor  Graphics::getColor ( const Name& key, int darkening )
  {
    return getGraphics()->_getColor ( key, darkening );
  }


  QPen  Graphics::getPen ( const Name& key, int darkening )
  {
    return getGraphics()->_getPen ( key, darkening );
  }


  QBrush  Graphics::getBrush ( const Name& key, int darkening )
  {
    return getGraphics()->_getBrush ( key, darkening );
  }


  const string& Graphics::getPattern ( const Name& key )
  {
    return getGraphics()->_getPattern ( key );
  }


  float  Graphics::getThreshold ( const Name& key )
  {
    return getGraphics()->_getThreshold ( key );
  }


  int  Graphics::getDarkening ()
  {
    return getGraphics()->_getDarkening ();
  }


  const ColorScale& Graphics::getColorScale ( ColorScale::ScaleType id )
  {
    return getGraphics()->_getColorScale ( id );
  }




} // End of Hurricane namespace.
