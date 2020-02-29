// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/Graphics.h"                 |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_GRAPHICS_H
#define  HURRICANE_GRAPHICS_H


#include <string>
#include <vector>
#include <QFont>
#include "hurricane/Observer.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/TextTranslator.h"
#include "hurricane/viewer/DisplayStyle.h"
#include "hurricane/viewer/ColorScale.h"
#include "hurricane/viewer/BreakpointWidget.h"

class QColor;
class QPen;
class QBrush;


namespace Hurricane {

  class Name;


// -------------------------------------------------------------------
// Class  :  "Graphics".

  class Graphics {
    public:
      enum NotifyFlags { ChangedDisplayStyles = (1<<0) };
    public:
    // Accessors.
      static  Graphics*                    getGraphics      ();
      static  void                         addObserver      ( BaseObserver* );
      static  void                         removeObserver   ( BaseObserver* );
      static  void                         notify           ( unsigned int flags );
      static  bool                         isEnabled        ();
      static  bool                         isHighDpi        ();
      static  const QFont                  getFixedFont     ( int weight=QFont::Normal, bool italic=false, bool underline=false, int scale=0 );
      static  const QFont                  getNormalFont    ( bool bold=false, bool italic=false, bool underline=false );
      static  const Name&                  getGroup         ( const Name& key );
      static  QColor                       getColor         ( const Name& key, const DisplayStyle::HSVr& darkening=DisplayStyle::HSVr() );
      static  QPen                         getPen           ( const Name& key, const DisplayStyle::HSVr& darkening=DisplayStyle::HSVr() );
      static  QBrush                       getBrush         ( const Name& key, const DisplayStyle::HSVr& darkening=DisplayStyle::HSVr() );
      static  const string&                getPattern       ( const Name& key );
      static  float                        getThreshold     ( const Name& key );
      static  const DisplayStyle::HSVr&    getDarkening     ();
      static  const ColorScale&            getColorScale    ( ColorScale::ScaleType );
      static  QColor                       darken           ( const QColor& );
      static  const TextTranslator&        getHtmlTranslator();
      static  string                       toHtml           ( const string& );
      static  int                          toHighDpi        ( int );
      static  bool                         breakpointStopCb ( const string& message );
      static  void                         clear            ();
    // Modifiers.
      static  void                         addStyle         ( DisplayStyle* displayStyle );
      static  void                         setStyle         ( const Name& key );
      static  void                         setStyle         ( size_t id );
      static  DisplayStyle*                getStyle         ( const Name& key );
      static  DisplayStyle*                getStyle         ();
      static  const vector<DisplayStyle*>& getStyles        ();
      static  void                         enable           ();
      static  void                         disable          ();
              void                         toJson           ( JsonWriter* ) const;
    // Internals - Attributes.
    protected:
      static  Graphics*                    _singleton;
              Observable                   _observers;
              TextTranslator               _htmlTranslator;
              vector<DisplayStyle*>        _styles;
              DisplayStyle*                _active;
              FireColorScale               _fireColorScale;
              RainbowColorScale            _rainbowColorScale;
              TemperatureColorScale        _temperatureColorScale;
              bool                         _qtEnabled;
              bool                         _highDpi;

    // Internals - Constructors & Destructors.
                                           Graphics           ();
                                           Graphics           ( const Graphics& );
              Graphics&                    operator=          ( const Graphics& );
                                          ~Graphics           ();
    // Internals - Methods.                                   
              size_t                       _findStyle         ( const Name& key ) const;
              void                         _addStyle          ( DisplayStyle* displayStyle );
              void                         _setStyle          ( const Name& key );
              void                         _setStyle          ( size_t id );
              DisplayStyle*                _getStyle          ( const Name& key );
              DisplayStyle*                _getStyle          () const;
      inline  const vector<DisplayStyle*>& _getStyles         () const;
      inline  const Name&                  _getGroup          ( const Name& key ) const;
      inline  QColor                       _getColor          ( const Name& key, const DisplayStyle::HSVr& darkening ) const;
      inline  QPen                         _getPen            ( const Name& key, const DisplayStyle::HSVr& darkening ) const;
      inline  QBrush                       _getBrush          ( const Name& key, const DisplayStyle::HSVr& darkening ) const;
      inline  const string&                _getPattern        ( const Name& key ) const;
      inline  float                        _getThreshold      ( const Name& key ) const;
      inline  const DisplayStyle::HSVr&    _getDarkening      () const;
      inline  const ColorScale&            _getColorScale     ( ColorScale::ScaleType ) const;
              void                         _enable            ();
              void                         _disable           ();
      inline  const TextTranslator&        _getHtmlTranslator () const;
              void                         _clear             ();
  };


  inline const Name& Graphics::_getGroup ( const Name& name ) const
  { return _active->getGroup(name); }

  inline QColor  Graphics::_getColor ( const Name& name, const DisplayStyle::HSVr& darkening ) const
  { return _active->getColor(name,darkening); }

  inline QPen  Graphics::_getPen ( const Name& name, const DisplayStyle::HSVr& darkening ) const
  { return _active->getPen(name,darkening); }

  inline QBrush  Graphics::_getBrush ( const Name& name, const DisplayStyle::HSVr& darkening ) const
  { return _active->getBrush(name,darkening); }

  inline const string& Graphics::_getPattern ( const Name& name ) const
  { return _active->getPattern(name); }

  inline float  Graphics::_getThreshold ( const Name& name ) const
  { return _active->getThreshold(name); }

  inline const DisplayStyle::HSVr& Graphics::_getDarkening () const
  { return _active->getDarkening(); }

  inline const ColorScale& Graphics::_getColorScale ( ColorScale::ScaleType id ) const
  {
    switch ( id ) {
      case ColorScale::Grey:
      case ColorScale::Fire:
        return _fireColorScale;
        break;
      case ColorScale::Rainbow:
        return _rainbowColorScale;
        break;
      case ColorScale::Temperature:
        return _temperatureColorScale;
        break;
    }
    return _fireColorScale;
  }

  inline DisplayStyle* Graphics::_getStyle () const
  { return _active; }

  inline const vector<DisplayStyle*>& Graphics::_getStyles () const
  { return _styles; }

  inline const TextTranslator& Graphics::_getHtmlTranslator () const
  { return _htmlTranslator; }


// -------------------------------------------------------------------
// Class  :  "JsonGraphics".

  class JsonGraphics : public JsonObject {
    public:
      static  void          initialize   ();
                            JsonGraphics ( unsigned long flags );
                           ~JsonGraphics ();
      virtual string        getTypeName  () const;
      virtual JsonGraphics* clone        ( unsigned long ) const;
      virtual void          toData       ( JsonStack& ); 
    private:
      std::string  _active;
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_GRAPHICS__
