
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

  class DrawingStyle;


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
                                                           ,       bool    goMatched
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
      class HSVr {
        public:
          inline       HSVr          ( float hue=1.0, float saturation=1.0, float value=1.0 );
          inline bool  isId          () const;
          inline float getHue        () const;
          inline float getSaturation () const;
          inline float getValue      () const;
          inline void  setHue        ( float );
          inline void  setSaturation ( float );
          inline void  setValue      ( float );
        private:
          float  _hue;
          float  _saturation;
          float  _value;
      };

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
                                       
      static QColor                       darken           ( const QColor& color, const DisplayStyle::HSVr& darkening );
    // Constructor & Destructor.       
                                          DisplayStyle     ( const Name& name );
                                         ~DisplayStyle     ();
                                       
    // Accessors.                      
             const Name&                  getName          () const;
      inline const string&                getDescription   () const;
      inline const HSVr&                  getDarkening     () const;
             const Name&                  getGroup         ( const Name& key ) const;
             const string&                getPattern       ( const Name& key ) const;
             QColor                       getColor         ( const Name& key, const HSVr& ) const;
             QPen                         getPen           ( const Name& key, const HSVr& ) const;
             QBrush                       getBrush         ( const Name& key, const HSVr& ) const;
             float                        getThreshold     ( const Name& key ) const;
      inline const vector<DrawingGroup*>& getDrawingGroups () const;
             DrawingStyle*                find             ( const Name& key ) const;
                                       
    // Modifiers.                      
             void                         qtAllocate       ();
      inline void                         setDescription   ( const string& description );
      inline void                         setDescription   ( const char* description );
             void                         inheritFrom      ( const DisplayStyle* base );
             void                         setDarkening     ( const HSVr& );
             void                         addDrawingStyle  ( const Name&   groupKey
                                                           , const Name&   key
                                                           , const string& pattern
                                                           ,       int     red
                                                           ,       int     green
                                                           ,       int     blue
                                                           ,       int     borderWidth
                                                           ,       float   threshold
                                                           ,       bool    goMatched  =true
                                                           );

    protected:
    // Internals - Attributes.
             const Name                   _name;
             string                       _description;
             vector<DrawingGroup*>        _groups;
             HSVr                         _darkening;
                                        
    // Internals - Methods.             
             void                         findOrCreate     ( const Name& groupKey
                                                           , size_t&     gi );
             void                         find             ( const Name& groupKey
                                                           , const Name& key
                                                           , size_t&     gi
                                                           , size_t&     si ) const;

  };


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
                                        ,       bool    goMatched  =true
                                        );
             DrawingStyle* link         ();
             size_t        unlink       ();
    public:
    // Accessors.
             void          qtAllocate   ();
      inline bool          isGoMatched  () const;
      inline const Name&   getName      () const;
      inline const string& getPattern   () const;
             QColor        getColor     ( const DisplayStyle::HSVr& ) const;
             QPen          getPen       ( const DisplayStyle::HSVr& ) const;
             QBrush        getBrush     ( const DisplayStyle::HSVr& ) const;
      inline float         getThreshold () const;
    private:
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
                   bool    _goMatched;
                   size_t  _refcount;
    private:
    // Internal - Constructors & Destructors.
                           DrawingStyle ( const Name&   name
                                        , const string& pattern
                                        ,       int     red
                                        ,       int     green
                                        ,       int     blue
                                        ,       int     borderWidth
                                        ,       float   threshold
                                        ,       bool    goMatched
                                        );
                           DrawingStyle ( const DrawingStyle& );
                          ~DrawingStyle ();
             DrawingStyle& operator=    ( const DrawingStyle& );
  };






  // Functions.
  inline bool                         DrawingStyle::isGoMatched      () const { return _goMatched; }
  inline const Name&                  DrawingStyle::getName          () const { return _name; }
  inline const string&                DrawingStyle::getPattern       () const { return _pattern; }
  inline float                        DrawingStyle::getThreshold     () const { return _threshold; }
                                   
  inline const Name&                  DrawingGroup::getName          () const { return _name; }
  inline const vector<DrawingStyle*>& DrawingGroup::getDrawingStyles () const { return _drawingStyles; }

  inline const Name&                  DisplayStyle::getName          () const { return _name; }
  inline const vector<DrawingGroup*>& DisplayStyle::getDrawingGroups () const { return _groups; }
  inline const DisplayStyle::HSVr&    DisplayStyle::getDarkening     () const { return _darkening; }
  inline const string&                DisplayStyle::getDescription   () const { return _description; }
  inline void                         DisplayStyle::setDescription   ( const string& description ) { _description = description; }
  inline void                         DisplayStyle::setDescription   ( const char* description ) { _description = description; }

  inline DisplayStyle::HSVr::HSVr ( float hue, float saturation, float value )
    : _hue(hue), _saturation(saturation), _value(value) { }

  inline bool  DisplayStyle::HSVr::isId          () const { return (_hue==1.0) and (_saturation==1.0) and (_value==1.0); }
  inline float DisplayStyle::HSVr::getHue        () const { return _hue; }
  inline float DisplayStyle::HSVr::getSaturation () const { return _saturation; }
  inline float DisplayStyle::HSVr::getValue      () const { return _value; }
  inline void  DisplayStyle::HSVr::setHue        ( float hue ) { _hue=hue; }
  inline void  DisplayStyle::HSVr::setSaturation ( float saturation ) { _saturation=saturation; }
  inline void  DisplayStyle::HSVr::setValue      ( float value ) { _value=value; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_DISPLAYSTYLE__
