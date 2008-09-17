
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
// |  C++ Header  :       "./Graphics.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __GRAPHICS_H__
#   define  __GRAPHICS_H__


# include  <string>
# include  <vector>

# include  "hurricane/viewer/DisplayStyle.h"

# include  <QFont>

class QColor;
class QPen;
class QBrush;




namespace Hurricane {


  class Name;


  class Graphics {

    public:
    // Accessors.
      static  Graphics*                    getGraphics   ();
      static  bool                         isEnabled     ();
      static  const QFont                  getFixedFont  ( int weight=QFont::Normal, bool italic=false, bool underline=false );
      static  const QFont                  getNormalFont ( bool bold=false, bool italic=false, bool underline=false );
      static  const Name&                  getGroup      ( const Name& key );
      static  QColor                       getColor      ( const Name& key, int darkening=100 );
      static  QPen                         getPen        ( const Name& key, int darkening=100 );
      static  QBrush                       getBrush      ( const Name& key, int darkening=100 );
      static  const string&                getPattern    ( const Name& key );
      static  float                        getThreshold  ( const Name& key );
      static  int                          getDarkening  ();

    // Modifiers.
      static  void                         addStyle      ( DisplayStyle* displayStyle );
      static  void                         setStyle      ( const Name& key );
      static  void                         setStyle      ( size_t id );
      static  DisplayStyle*                getStyle      ( const Name& key );
      static  DisplayStyle*                getStyle      ();
      static  const vector<DisplayStyle*>& getStyles     ();
      static  void                         enable        ();

    // Internals - Attributes.
    protected:
      static  Graphics*                    _singleton;
              vector<DisplayStyle*>        _styles;
              DisplayStyle*                _active;
              bool                         _qtEnabled;

    // Internals - Constructors & Destructors.
                                           Graphics      ();
                                           Graphics      ( const Graphics& );
              Graphics&                    operator=     ( const Graphics& );
                                          ~Graphics      ();

    // Internals - Methods.
              size_t                       _findStyle    ( const Name& key ) const;
              void                         _addStyle     ( DisplayStyle* displayStyle );
              void                         _setStyle     ( const Name& key );
              void                         _setStyle     ( size_t id );
              DisplayStyle*                _getStyle     ( const Name& key );
              DisplayStyle*                _getStyle     () const;
      inline  const vector<DisplayStyle*>& _getStyles    () const;
      inline  const Name&                  _getGroup     ( const Name& key ) const;
      inline  QColor                       _getColor     ( const Name& key, int darkening ) const;
      inline  QPen                         _getPen       ( const Name& key, int darkening ) const;
      inline  QBrush                       _getBrush     ( const Name& key, int darkening ) const;
      inline  const string&                _getPattern   ( const Name& key ) const;
      inline  float                        _getThreshold ( const Name& key ) const;
      inline  int                          _getDarkening () const;
      inline  void                         _enable       ();

  };


  inline const Name& Graphics::_getGroup ( const Name& name ) const
  { return _active->getGroup(name); }

  inline QColor  Graphics::_getColor ( const Name& name, int darkening ) const
  { return _active->getColor(name,darkening); }

  inline QPen  Graphics::_getPen ( const Name& name, int darkening ) const
  { return _active->getPen(name,darkening); }

  inline QBrush  Graphics::_getBrush ( const Name& name, int darkening ) const
  { return _active->getBrush(name,darkening); }

  inline const string& Graphics::_getPattern ( const Name& name ) const
  { return _active->getPattern(name); }

  inline float  Graphics::_getThreshold ( const Name& name ) const
  { return _active->getThreshold(name); }

  inline int  Graphics::_getDarkening () const
  { return _active->getDarkening(); }

  inline DisplayStyle* Graphics::_getStyle () const
  { return _active; }

  inline const vector<DisplayStyle*>& Graphics::_getStyles () const
  { return _styles; }




} // End of Hurricane namespace.


# endif
