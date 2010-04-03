
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Header  :       "./DisplayStyle.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_DISPLAYSTYLE_H__
#define  __HURRICANE_DISPLAYSTYLE_H__


#include  <string>
#include  <map>

#include  <QColor>
#include  <QPen>
#include  <QBrush>

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"

#include  "hurricane/viewer/ScreenUtilities.h"


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
             void          qtAllocate   ();
      inline const Name&   getName      () const;
      inline const string& getPattern   () const;
             QColor        getColor     ( int darkening ) const;
             QPen          getPen       ( int darkening ) const;
             QBrush        getBrush     ( int darkening ) const;
      inline float         getThreshold () const;

    protected:
    // Internal - Attributes.
             const Name    _name;
                   int     _red;
                   int     _green;
                   int     _blue;
                   int     _borderWidth;
                   string  _pattern;
                   QColor *_color;
                   QPen   *_pen;
                   QBrush *_brush;
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
              DrawingGroup*               getClone         ();
              void                        qtAllocate       ();

    // Methods.
      inline const Name&                  getName          () const;
      inline const vector<DrawingStyle*>& getDrawingStyles () const;
             size_t                       findIndex        ( const Name& key ) const;
             DrawingStyle*                find             ( const Name& key ) const;
             DrawingStyle*                addDrawingStyle  ( const Name&   key
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
      static const Name                   Viewer;
      static const Name                   Fallback;
      static const Name                   Background;
      static const Name                   Foreground;
      static const Name                   Rubber;
      static const Name                   Phantom;
      static const Name                   Boundaries;
      static const Name                   Marker;
      static const Name                   SelectionDraw;
      static const Name                   SelectionFill;
      static const Name                   Grid;
      static const Name                   Spot;
      static const Name                   Ghost;
      static const Name                   TextRuler;
      static const Name                   TextCell;
      static const Name                   TextInstance;
      static const Name                   TextComponent;
      static const Name                   TextReference;
      static const Name                   Undef;
      static const Name                   UnmatchedGroup;
                                       
    // Constructor & Destructor.       
                                          DisplayStyle     ( const Name& name );
                                         ~DisplayStyle     ();
                                       
    // Accessors.                      
             const Name&                  getName          () const;
      inline const string&                getDescription   () const;
      inline int                          getDarkening     () const;
             const Name&                  getGroup         ( const Name& key ) const;
             const string&                getPattern       ( const Name& key ) const;
             QColor                       getColor         ( const Name& key, int darkening ) const;
             QPen                         getPen           ( const Name& key, int darkening ) const;
             QBrush                       getBrush         ( const Name& key, int darkening ) const;
             float                        getThreshold     ( const Name& key ) const;
      inline const vector<DrawingGroup*>& getDrawingGroups () const;
             DrawingStyle*                find             ( const Name& key ) const;
                                       
    // Modifiers.                      
             void                         qtAllocate       ();
      inline void                         setDescription   ( const string& description );
      inline void                         setDescription   ( const char* description );
             void                         inheritFrom      ( const DisplayStyle* base );
             void                         setDarkening     ( int darkening );
             void                         addDrawingStyle  ( const Name&   groupKey
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
             const Name                   _name;
             string                       _description;
             vector<DrawingGroup*>        _groups;
             int                          _darkening;
                                        
    // Internals - Methods.             
             void                         findOrCreate     ( const Name& groupKey
                                                           , size_t&     gi );
             void                         find             ( const Name& groupKey
                                                           , const Name& key
                                                           , size_t&     gi
                                                           , size_t&     si ) const;

  };




  // Functions.
  inline const Name&                  DrawingStyle::getName          () const { return _name; }
  inline const string&                DrawingStyle::getPattern       () const { return _pattern; }
  inline float                        DrawingStyle::getThreshold     () const { return _threshold; }
                                   
  inline const Name&                  DrawingGroup::getName          () const { return _name; }
  inline const vector<DrawingStyle*>& DrawingGroup::getDrawingStyles () const { return _drawingStyles; }

  inline const Name&                  DisplayStyle::getName          () const { return _name; }
  inline const vector<DrawingGroup*>& DisplayStyle::getDrawingGroups () const { return _groups; }
  inline int                          DisplayStyle::getDarkening     () const { return _darkening; }
  inline const string&                DisplayStyle::getDescription   () const { return _description; }
  inline void                         DisplayStyle::setDescription   ( const string& description ) { _description = description; }
  inline void                         DisplayStyle::setDescription   ( const char* description ) { _description = description; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_DISPLAYSTYLE__
