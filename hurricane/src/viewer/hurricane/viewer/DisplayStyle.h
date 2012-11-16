
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/DisplayStyle.h"             |
// +-----------------------------------------------------------------+


#ifndef  __HURRICANE_DISPLAYSTYLE_H__
#define  __HURRICANE_DISPLAYSTYLE_H__


#include  <memory>
#include  <string>
#include  <QColor>
#include  <QPen>
#include  <QBrush>
#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/viewer/ScreenUtilities.h"


namespace Hurricane {

  class RawDrawingStyle;
  typedef  std::tr1::shared_ptr<RawDrawingStyle>  DrawingStyle;
  typedef  std::vector         <DrawingStyle>     DrawingStyles;


  class DrawingGroup {
    public:
    // Constructors & Destructors.
                                  DrawingGroup     ( const Name&   name );
                                 ~DrawingGroup     ();
              DrawingGroup*       getClone         ();
              void                qtAllocate       ();
    // Methods.                  
      inline const Name&          getName          () const;
      inline const DrawingStyles& getDrawingStyles () const;
             size_t               findIndex        ( const Name& key ) const;
             DrawingStyle         find             ( const Name& key ) const;
             DrawingStyle         addDrawingStyle  ( const Name&        key
                                                   , const std::string& pattern
                                                   ,       int          red
                                                   ,       int          green
                                                   ,       int          blue
                                                   ,       int          borderWidth
                                                   ,       float        threshold
                                                   ,       bool         goMatched
                                                   );
    protected:
      const Name     _name;
      DrawingStyles  _drawingStyles;
    protected:
                    DrawingGroup ( const DrawingGroup& );
      DrawingGroup& operator=    ( const DrawingGroup& );

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
      static const Name                        Viewer;
      static const Name                        Fallback;
      static const Name                        Background;
      static const Name                        Foreground;
      static const Name                        Rubber;
      static const Name                        Phantom;
      static const Name                        Boundaries;
      static const Name                        Marker;
      static const Name                        SelectionDraw;
      static const Name                        SelectionFill;
      static const Name                        Grid;
      static const Name                        Spot;
      static const Name                        Ghost;
      static const Name                        TextRuler;
      static const Name                        TextCell;
      static const Name                        TextInstance;
      static const Name                        TextComponent;
      static const Name                        TextReference;
      static const Name                        Undef;
      static const Name                        UnmatchedGroup;
                                            
      static QColor                            darken           ( const QColor& color, const DisplayStyle::HSVr& darkening );
    // Constructor & Destructor.            
                                               DisplayStyle     ( const Name& name );
                                              ~DisplayStyle     ();
                                            
    // Accessors.                           
             const Name&                       getName          () const;
      inline const std::string&                getDescription   () const;
      inline const HSVr&                       getDarkening     () const;
             const Name&                       getGroup         ( const Name& key ) const;
             const std::string&                getPattern       ( const Name& key ) const;
             QColor                            getColor         ( const Name& key, const HSVr& ) const;
             QPen                              getPen           ( const Name& key, const HSVr& ) const;
             QBrush                            getBrush         ( const Name& key, const HSVr& ) const;
             float                             getThreshold     ( const Name& key ) const;
      inline const std::vector<DrawingGroup*>& getDrawingGroups () const;
             DrawingStyle                      find             ( const Name& key ) const;
                                            
    // Modifiers.                           
             void                              qtAllocate       ();
      inline void                              setDescription   ( const std::string& description );
      inline void                              setDescription   ( const char* description );
             void                              inheritFrom      ( const DisplayStyle* base );
             void                              setDarkening     ( const HSVr& );
             void                              addDrawingStyle  ( const Name&        groupKey
                                                                , const Name&        key
                                                                , const std::string& pattern
                                                                ,       int          red
                                                                ,       int          green
                                                                ,       int          blue
                                                                ,       int          borderWidth
                                                                ,       float        threshold
                                                                ,       bool         goMatched  =true
                                                                );
                                            
    protected:                              
    // Internals - Attributes.              
             const Name                        _name;
             std::string                       _description;
             std::vector<DrawingGroup*>        _groups;
             HSVr                              _darkening;
                                        
    // Internals - Methods.             
             void                         findOrCreate     ( const Name& groupKey
                                                           , size_t&     gi );
             void                         find             ( const Name& groupKey
                                                           , const Name& key
                                                           , size_t&     gi
                                                           , size_t&     si ) const;

  };


  class RawDrawingStyle {
    public:
    // Constructors & Destructors.
      static DrawingStyle  create          ( const Name&        name
                                           , const std::string& pattern    ="FFFFFFFFFFFFFFFF"
                                           ,       int          red        =0
                                           ,       int          green      =0
                                           ,       int          blue       =0
                                           ,       int          borderWidth=0
                                           ,       float        threshold  =1.0
                                           ,       bool         goMatched  =true
                                           );
    public:
    // Accessors.
             void               qtAllocate   ();
      inline bool               isGoMatched  () const;
      inline const Name&        getName      () const;
      inline const std::string& getPattern   () const;
             QColor             getColor     ( const DisplayStyle::HSVr& ) const;
             QPen               getPen       ( const DisplayStyle::HSVr& ) const;
             QBrush             getBrush     ( const DisplayStyle::HSVr& ) const;
      inline float              getThreshold () const;
    private:
    // Internal - Attributes.
      const Name         _name;
            int          _red;
            int          _green;
            int          _blue;
            int          _borderWidth;
            std::string  _pattern;
            QColor*      _color;
            QPen*        _pen;
            QBrush*      _brush;
            float        _threshold;
            bool         _goMatched;
    public:
    // Internal - Constructors & Destructors.
                       RawDrawingStyle ( const Name&        name
                                       , const std::string& pattern
                                       ,       int          red
                                       ,       int          green
                                       ,       int          blue
                                       ,       int          borderWidth
                                       ,       float        threshold
                                       ,       bool         goMatched
                                       );
                       RawDrawingStyle ( const RawDrawingStyle& );
                      ~RawDrawingStyle ();
    private:
      RawDrawingStyle& operator=       ( const RawDrawingStyle& );
  };


  // Functions.
  inline bool                              RawDrawingStyle::isGoMatched      () const { return _goMatched; }
  inline const Name&                       RawDrawingStyle::getName          () const { return _name; }
  inline const std::string&                RawDrawingStyle::getPattern       () const { return _pattern; }
  inline float                             RawDrawingStyle::getThreshold     () const { return _threshold; }
                                           
  inline const Name&                       DisplayStyle::getName          () const { return _name; }
  inline const std::vector<DrawingGroup*>& DisplayStyle::getDrawingGroups () const { return _groups; }
  inline const DisplayStyle::HSVr&         DisplayStyle::getDarkening     () const { return _darkening; }
  inline const std::string&                DisplayStyle::getDescription   () const { return _description; }
  inline void                              DisplayStyle::setDescription   ( const std::string& description ) { _description = description; }
  inline void                              DisplayStyle::setDescription   ( const char* description ) { _description = description; }

  inline DisplayStyle::HSVr::HSVr ( float hue, float saturation, float value )
    : _hue(hue), _saturation(saturation), _value(value) { }
                                   
  inline const Name&          DrawingGroup::getName          () const { return _name; }
  inline const DrawingStyles& DrawingGroup::getDrawingStyles () const { return _drawingStyles; }

  inline bool  DisplayStyle::HSVr::isId          () const { return (_hue==1.0) and (_saturation==1.0) and (_value==1.0); }
  inline float DisplayStyle::HSVr::getHue        () const { return _hue; }
  inline float DisplayStyle::HSVr::getSaturation () const { return _saturation; }
  inline float DisplayStyle::HSVr::getValue      () const { return _value; }
  inline void  DisplayStyle::HSVr::setHue        ( float hue ) { _hue=hue; }
  inline void  DisplayStyle::HSVr::setSaturation ( float saturation ) { _saturation=saturation; }
  inline void  DisplayStyle::HSVr::setValue      ( float value ) { _value=value; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_DISPLAYSTYLE__
