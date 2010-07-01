
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
// |  C++ Header  :       "./Graphics.h"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_GRAPHICS__
#define  __HURRICANE_GRAPHICS__


#include  <string>
#include  <vector>

#include  "hurricane/Breakpoint.h"
#include  "hurricane/TextTranslator.h"
#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/ColorScale.h"
#include  "hurricane/viewer/BreakpointWidget.h"

#include  <QFont>

class QColor;
class QPen;
class QBrush;




namespace Hurricane {


  class Name;


  class Graphics {

    public:
    // Accessors.
      static  Graphics*                    getGraphics      ();
      static  bool                         isEnabled        ();
      static  const QFont                  getFixedFont     ( int weight=QFont::Normal, bool italic=false, bool underline=false, int scale=0 );
      static  const QFont                  getNormalFont    ( bool bold=false, bool italic=false, bool underline=false );
      static  const Name&                  getGroup         ( const Name& key );
      static  QColor                       getColor         ( const Name& key, int darkening=100 );
      static  QPen                         getPen           ( const Name& key, int darkening=100 );
      static  QBrush                       getBrush         ( const Name& key, int darkening=100 );
      static  const string&                getPattern       ( const Name& key );
      static  float                        getThreshold     ( const Name& key );
      static  int                          getDarkening     ();
      static  const ColorScale&            getColorScale    ( ColorScale::ScaleType );
      static  const TextTranslator&        getHtmlTranslator();
      static  string                       toHtml           ( const string& );
      static  bool                         breakpointStopCb ( const string& message );

    // Modifiers.
      static  void                         addStyle         ( DisplayStyle* displayStyle );
      static  void                         setStyle         ( const Name& key );
      static  void                         setStyle         ( size_t id );
      static  DisplayStyle*                getStyle         ( const Name& key );
      static  DisplayStyle*                getStyle         ();
      static  const vector<DisplayStyle*>& getStyles        ();
      static  void                         enable           ();

    // Internals - Attributes.
    protected:
      static  Graphics*                    _singleton;
              TextTranslator               _htmlTranslator;
              vector<DisplayStyle*>        _styles;
              DisplayStyle*                _active;
              FireColorScale               _fireColorScale;
              RainbowColorScale            _rainbowColorScale;
              TemperatureColorScale        _temperatureColorScale;
              bool                         _qtEnabled;

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
      inline  QColor                       _getColor          ( const Name& key, int darkening ) const;
      inline  QPen                         _getPen            ( const Name& key, int darkening ) const;
      inline  QBrush                       _getBrush          ( const Name& key, int darkening ) const;
      inline  const string&                _getPattern        ( const Name& key ) const;
      inline  float                        _getThreshold      ( const Name& key ) const;
      inline  int                          _getDarkening      () const;
      inline  const ColorScale&            _getColorScale     ( ColorScale::ScaleType ) const;
      inline  void                         _enable            ();
      inline  const TextTranslator&        _getHtmlTranslator () const;

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




} // End of Hurricane namespace.


#endif  // __HURRICANE_GRAPHICS__
