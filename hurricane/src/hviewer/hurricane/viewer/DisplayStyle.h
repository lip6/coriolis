
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
// |  C++ Header  :       "./DisplayStyle.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __DISPLAYSTYLE_H__
#   define  __DISPLAYSTYLE_H__


# include  <string>
# include  <map>

# include  <QColor>
# include  <QPen>
# include  <QBrush>

# include  "hurricane/Commons.h"
# include  "hurricane/Name.h"

# include  "hurricane/viewer/ScreenUtilities.h"


namespace Hurricane {




  class DrawingStyle {

    public:
    // Constructors & Destructors.
      static DrawingStyle* create       ( const Name&   name
                                        , const string& pattern    ="FFFFFFFFFFFFFFFF"
                                        ,       int     red        =0
                                        ,       int     green      =0
                                        ,       int     blue       =0
                                        ,       int     borderWidth=0
                                        ,       float   threshold  =1.0
                                        );
             DrawingStyle* link         ();
             size_t        unlink       ();

    // Accessors.
      inline const Name&   getName      () const;
      inline const string& getPattern   () const;
             QColor        getColor     ( int darkening ) const;
             QPen          getPen       ( int darkening ) const;
             QBrush        getBrush     ( int darkening ) const;
      inline float         getThreshold () const;

    protected:
    // Internal - Attributes.
             const Name    _name;
                   string  _pattern;
                   QColor  _color;
                   QPen    _pen;
                   QBrush  _brush;
                   float   _threshold;
                   size_t  _refcount;

    // Internal - Constructors & Destructors.
                           DrawingStyle ( const Name&   name
                                        , const string& pattern    ="FFFFFFFFFFFFFFFF"
                                        ,       int     red        =0
                                        ,       int     green      =0
                                        ,       int     blue       =0
                                        ,       int     borderWidth=0
                                        ,       float   threshold  =1.0
                                        );
                           DrawingStyle ( const DrawingStyle& );
                          ~DrawingStyle ();
             DrawingStyle& operator=    ( const DrawingStyle& );

  };




  class DrawingGroup {

    public:
    // Constructors & Destructors.
                                    DrawingGroup     ( const Name&   name );
                                   ~DrawingGroup     ();
              DrawingGroup*         getClone         ();

    // Methods.
      inline const Name&            getName          () const;
      inline vector<DrawingStyle*>& getDrawingStyles ();
             size_t                 findIndex        ( const Name& key ) const;
             DrawingStyle*          find             ( const Name& key );
             DrawingStyle*          addDrawingStyle  ( const Name&   key
                                                     , const string& pattern
                                                     ,       int     red
                                                     ,       int     green
                                                     ,       int     blue
                                                     ,       int     borderWidth
                                                     ,       float   threshold
                                                     );
      
    protected:
    // Internal - Attributes.
             const Name             _name;
             vector<DrawingStyle*>  _drawingStyles;

    // Internal - Constructors & Destructors.
                                    DrawingGroup     ( const DrawingGroup& );
             DrawingGroup&          operator=        ( const DrawingGroup& );

  };




  class DisplayStyle {

    public:
    // Static Members.
      static const Name             Viewer;
      static const Name             Fallback;
      static const Name             Background;
      static const Name             Foreground;
      static const Name             Rubber;
      static const Name             Phantom;
      static const Name             Boundaries;
      static const Name             Marker;
      static const Name             SelectionDraw;
      static const Name             SelectionFill;
      static const Name             Grid;
      static const Name             Spot;
      static const Name             Ghost;
      static const Name             Text;
      static const Name             Undef;
      static const Name             UnmatchedGroup;

    // Constructor & Destructor.
                                    DisplayStyle     ( const Name& name );
                                   ~DisplayStyle     ();

    // Accessors.
             const Name&            getName          () const;
      inline const string&          getDescription   () const;
      inline int                    getDarkening     () const;
             const Name&            getGroup         ( const Name& key ) const;
             const string&          getPattern       ( const Name& key ) const;
             QColor                 getColor         ( const Name& key, int darkening ) const;
             QPen                   getPen           ( const Name& key, int darkening ) const;
             QBrush                 getBrush         ( const Name& key, int darkening ) const;
             float                  getThreshold     ( const Name& key ) const;
      inline vector<DrawingGroup*>& getDrawingGroups ();
             DrawingStyle*          find             ( const Name& key ) const;

    // Modifiers.
      inline void                   setDescription   ( const string& description );
      inline void                   setDescription   ( const char* description );
             void                   inheritFrom      ( const DisplayStyle* base );
             void                   setDarkening     ( int darkening );
             void                   addDrawingStyle  ( const Name&   groupKey
                                                     , const Name&   key
                                                     , const string& pattern
                                                     ,       int     red
                                                     ,       int     green
                                                     ,       int     blue
                                                     ,       int     borderWidth
                                                     ,       float   threshold
                                                     );

    protected:
    // Internals - Attributes.
             const Name             _name;
             string                 _description;
             vector<DrawingGroup*>  _groups;
             int                    _darkening;

    // Internals - Methods.
             void                   findOrCreate     ( const Name& groupKey
                                                     , size_t&     gi );
             void                   find             ( const Name& groupKey
                                                     , const Name& key
                                                     , size_t&     gi
                                                     , size_t&     si );

  };




  // Functions.
  inline const Name&            DrawingStyle::getName          () const { return _name; }
  inline const string&          DrawingStyle::getPattern       () const { return _pattern; }
  inline float                  DrawingStyle::getThreshold     () const { return _threshold; }

  inline const Name&            DrawingGroup::getName          () const { return _name; }
  inline vector<DrawingStyle*>& DrawingGroup::getDrawingStyles () { return _drawingStyles; }

  inline const Name&            DisplayStyle::getName          () const { return _name; }
  inline vector<DrawingGroup*>& DisplayStyle::getDrawingGroups () { return _groups; }
  inline int                    DisplayStyle::getDarkening     () const { return _darkening; }
  inline const string&          DisplayStyle::getDescription   () const { return _description; }
  inline void                   DisplayStyle::setDescription   ( const string& description ) { _description = description; }
  inline void                   DisplayStyle::setDescription   ( const char* description ) { _description = description; }


} // End of Hurricane namespace.


# endif
