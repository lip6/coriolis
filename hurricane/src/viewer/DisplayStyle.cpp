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
// |  C++ Module  :       "./DisplayStyle.cpp"                       |
// +-----------------------------------------------------------------+


#include  <cassert>

#include  "hurricane/Error.h"
#include  "hurricane/viewer/DisplayStyle.h"
#include  "hurricane/viewer/Graphics.h"


namespace Hurricane {

  using namespace std;


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


// -------------------------------------------------------------------
// Class  :  "RawDrawingStyle".

  RawDrawingStyle::RawDrawingStyle ( const Name&   name
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
  { }


  RawDrawingStyle::~RawDrawingStyle ()
  {
    qtFree();
  }


  DrawingStyle  RawDrawingStyle::create ( const Name&   name
                                        , const string& pattern
                                        ,       int     red
                                        ,       int     green
                                        ,       int     blue
                                        ,       int     borderWidth
                                        ,       float   threshold
                                        ,       bool    goMatched
                                        )
  {
    DrawingStyle  style = DrawingStyle( new RawDrawingStyle ( name
                                                            , pattern
                                                            , red
                                                            , green
                                                            , blue
                                                            , borderWidth
                                                            , threshold
                                                            , goMatched ) );
  //if ( Graphics::isEnabled() )
  //  style->qtAllocate ();

    return style;
  }


  void  RawDrawingStyle::qtAllocate ()
  {
    if (not _color) {
      _color = new QColor ( _red, _green, _blue );

      _pen = new QPen ();
      if (_borderWidth) {
        _pen->setStyle( Qt::SolidLine );
        _pen->setWidth( _borderWidth );
      } else
        _pen->setStyle( Qt::NoPen );
      _pen->setColor( *_color );

      _brush = Hurricane::getBrush( _pattern, _red, _green, _blue );
    }
  }


  void  RawDrawingStyle::qtFree ()
  {
    if (_color) {
      delete _brush;
      delete _pen;
      delete _color;

      _brush = NULL;
      _pen   = NULL;
      _color = NULL;
    }
  }


  QColor  RawDrawingStyle::getColor ( const DisplayStyle::HSVr& darkening ) const
  {
    assert ( _color != NULL );

  //return _color->darker ( darkening );
    return DisplayStyle::darken(*_color,darkening);
  }


  QPen  RawDrawingStyle::getPen ( const DisplayStyle::HSVr& darkening ) const
  {
    assert ( _pen != NULL );

    QPen pen ( *_pen );
  //pen.setColor ( _color->darker(darkening) );
    pen.setColor ( DisplayStyle::darken(*_color,darkening) );

    return pen;
  }


  QBrush  RawDrawingStyle::getBrush ( const DisplayStyle::HSVr& darkening ) const
  {
    assert ( _brush != NULL );

    QBrush brush ( *_brush );
  //brush.setColor ( _color->darker(darkening) );
    brush.setColor ( DisplayStyle::darken(*_color,darkening) );
    return brush;
  }


  void  RawDrawingStyle::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", "DrawingStyle" );
    jsonWrite( w, "_name"       , _name        );
    jsonWrite( w, "_red"        , _red         );
    jsonWrite( w, "_green"      , _green       );
    jsonWrite( w, "_blue"       , _blue        );
    jsonWrite( w, "_borderWidth", _borderWidth );
    jsonWrite( w, "_pattern"    , _pattern     );
    jsonWrite( w, "_threshold"  , _threshold   );
    jsonWrite( w, "_goMatched"  , _goMatched   );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class :  "JsonDrawingStyle".

  Initializer<JsonDrawingStyle>  jsonDrawingStyleInit ( 0 );


  void  JsonDrawingStyle::initialize ()
  { JsonTypes::registerType( new JsonDrawingStyle (JsonWriter::RegisterMode) ); }


  JsonDrawingStyle::JsonDrawingStyle ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( ".DrawingGroup", typeid(DrawingGroup) );
    add( "_name"        , typeid(string)       );
    add( "_red"         , typeid(int64_t)      );
    add( "_green"       , typeid(int64_t)      );
    add( "_blue"        , typeid(int64_t)      );
    add( "_borderWidth" , typeid(int64_t)      );
    add( "_pattern"     , typeid(string)       );
    add( "_threshold"   , typeid(double)       );
    add( "_goMatched"   , typeid(bool)         );
  }


  JsonDrawingStyle::~JsonDrawingStyle ()
  { }


  string  JsonDrawingStyle::getTypeName () const
  { return "DrawingStyle"; }


  JsonDrawingStyle* JsonDrawingStyle::clone ( unsigned long flags ) const
  { return new JsonDrawingStyle ( flags ); }


  void JsonDrawingStyle::toData(JsonStack& stack)
  {
    cdebug_log(19,1);

    check( stack, "JsonDrawingStyle::toData" );

    DrawingGroup* dg = get<DrawingGroup*>( stack, ".DrawingGroup" );
    DrawingStyle  ds;

    if (not dg) return;

    string name        = get<string> ( stack, "_name"        );
    int    red         = get<int64_t>( stack, "_red"         );
    int    green       = get<int64_t>( stack, "_green"       );
    int    blue        = get<int64_t>( stack, "_blue"        );
    int    borderWidth = get<int64_t>( stack, "_borderWidth" );
    string pattern     = get<string> ( stack, "_pattern"     );
    float  threshold   = get<double> ( stack, "_threshold"   );
    bool   goMatched   = get<bool>   ( stack, "_goMatched"   );

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
    // Actual creation.
      ds = RawDrawingStyle::create( name
                                  , pattern
                                  , red
                                  , green
                                  , blue
                                  , borderWidth
                                  , threshold
                                  , goMatched
                                  );
      dg->addDrawingStyle( ds );

      if (Graphics::isEnabled()) dg->qtAllocate();
    } else {
    // Check coherency with existing DrawingStyle.
      ds = dg->find( name );
      if (ds == nullptr) {
        cerr << Error( "JsonDrawingStyle::toData(): No DrawingStyle \"%s\" in the existing DisplayStyle."
                     , name.c_str()
                     ) << endl;
      }
    }
    
    update( stack, NULL );

    cdebug_tabw(19,-1);
  }


// -------------------------------------------------------------------
// Class  :  "DrawingGroup".

  DrawingGroup::DrawingGroup ( const Name& name )
    : _name         (name)
    , _drawingStyles()
  { }


  void  DrawingGroup::qtAllocate ()
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      _drawingStyles[i]->qtAllocate ();
  }


  void  DrawingGroup::qtFree ()
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      _drawingStyles[i]->qtFree ();
  }


  DrawingGroup* DrawingGroup::getClone ()
  {
    DrawingGroup* clone = new DrawingGroup ( getName() );

    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ )
      clone->_drawingStyles.push_back ( _drawingStyles[i] );

    return clone;
  }


  DrawingGroup::~DrawingGroup ()
  { }


  size_t  DrawingGroup::findIndex ( const Name& key ) const
  {
    for ( size_t i=0 ; i < _drawingStyles.size() ; i++ ) {
      if ( _drawingStyles[i]->getName() == key )
        return i;
    }

    return InvalidIndex;
  }


  DrawingStyle  DrawingGroup::find ( const Name& key ) const
  {
    size_t i = findIndex ( key );
    if ( i != InvalidIndex )
      return _drawingStyles[i];

    return DrawingStyle();
  }


  void  DrawingGroup::addDrawingStyle ( DrawingStyle ds )
  {
    size_t i = findIndex ( ds->getName() );
    if ( i != InvalidIndex ) {
      _drawingStyles[i] = ds;
    } else {
      _drawingStyles.push_back ( ds );
    }
  }


  DrawingStyle  DrawingGroup::addDrawingStyle ( const Name&   key
                                              , const string& pattern
                                              ,       int     red
                                              ,       int     green
                                              ,       int     blue
                                              ,       int     borderWidth
                                              ,       float   threshold
                                              ,       bool    goMatched
                                              )
  {
    DrawingStyle ds = RawDrawingStyle::create ( key, pattern, red, green, blue, borderWidth, threshold, goMatched );

    size_t i = findIndex ( key );
    if ( i != InvalidIndex ) {
      _drawingStyles[i] = ds;
    } else {
      _drawingStyles.push_back ( ds );
    }
    return ds;
  }


  void  DrawingGroup::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", "DrawingGroup" );
    jsonWrite( w, "_name", _name );

    w->key( "+drawingStyles" );
    w->startArray();
    for ( DrawingStyle ds : _drawingStyles ) ds->toJson( w );
    w->endArray();

    w->endObject();
  }


// -------------------------------------------------------------------
// Class :  "JsonDrawingGroup".

  Initializer<JsonDrawingGroup>  jsonDrawingGroupInit ( 0 );


  void  JsonDrawingGroup::initialize ()
  { JsonTypes::registerType( new JsonDrawingGroup (JsonWriter::RegisterMode) ); }


  JsonDrawingGroup::JsonDrawingGroup ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( ".DisplayStyle" , typeid(DisplayStyle*) );
    add( "_name"         , typeid(string)        );
    add( "+drawingStyles", typeid(JsonArray)     );
  }


  JsonDrawingGroup::~JsonDrawingGroup ()
  { }


  string  JsonDrawingGroup::getTypeName () const
  { return "DrawingGroup"; }


  JsonDrawingGroup* JsonDrawingGroup::clone ( unsigned long flags ) const
  { return new JsonDrawingGroup ( flags ); }


  void JsonDrawingGroup::toData(JsonStack& stack)
  {
    cdebug_log(19,1);

    check( stack, "JsonDrawingGroup::toData" );

    DisplayStyle* ds = get<DisplayStyle*>( stack, ".DisplayStyle" );
    DrawingGroup* dg = NULL;

    if (not ds) return;

    string name = get<string>( stack, "_name" );

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
    // Actual creation.
      size_t gi = 0;
      ds->findOrCreate( name, gi );
      dg = ds->findGroup( name );
    } else {
    // Check coherency with existing DrawingGroup.
      dg = ds->findGroup( name );
      if (dg == NULL) {
        cerr << Error( "JsonDrawingGroup::toData(): No DrawingGroup \"%s\" in the existing DisplayStyle."
                     , name.c_str()
                     ) << endl;
      }
    }
    
    update( stack, dg );

    cdebug_tabw(19,-1);
  }


// -------------------------------------------------------------------
// Class  :  "DisplayStyle".

  QColor  DisplayStyle::darken ( const QColor& color, const DisplayStyle::HSVr& darkening )
  {
    QColor hsvColor = color.toHsv();
    if ( not darkening.isId() ) {
      qreal darkHue   = color.hueF();
      qreal darkSat   = color.saturationF();
      qreal darkValue = color.valueF();

    //hsvColor.setHsvF ( darkHue/darkening.getHue(), darkSat/3.0, darkValue/2.5 );
      hsvColor.setHsvF ( darkHue  /darkening.getHue()
                       , darkSat  /darkening.getSaturation()
                       , darkValue/darkening.getValue()
                       );
    }
    return hsvColor;
  }


   DisplayStyle::DisplayStyle ( const Name& name )
     : _name(name)
     , _description("<No Description>")
     , _groups()
     , _darkening(1.0,3.0,2.5)
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


  void  DisplayStyle::qtFree ()
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ )
      _groups[gi]->qtFree ();
  }


  const Name& DisplayStyle::getGroup ( const Name& key ) const
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ ) {
      DrawingStyle style = _groups[gi]->find ( key );
      if ( style )
        return _groups[gi]->getName();
    }

    return UnmatchedGroup;
  }


  const string& DisplayStyle::getPattern ( const Name& key ) const
  {
    return find(key)->getPattern();
  }


  QColor  DisplayStyle::getColor ( const Name& key, const DisplayStyle::HSVr& darkening ) const
  {
    return find(key)->getColor(darkening);
  }


  QPen  DisplayStyle::getPen ( const Name& key, const DisplayStyle::HSVr& darkening ) const
  {
    return find(key)->getPen(darkening);
  }


  QBrush  DisplayStyle::getBrush ( const Name& key, const DisplayStyle::HSVr& darkening ) const
  {
    return find(key)->getBrush(darkening);
  }


  float  DisplayStyle::getThreshold ( const Name& key ) const
  {
    return find(key)->getThreshold();
  }


  DrawingGroup* DisplayStyle::findGroup ( const Name& groupKey )
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ ) {
      if ( _groups[gi]->getName() == groupKey )
        return _groups[gi];
    }
    return NULL;
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


  DrawingStyle  DisplayStyle::find ( const Name& key ) const
  {
    for ( size_t gi=0 ; gi < _groups.size() ; gi++ ) {
      DrawingStyle style = _groups[gi]->find ( key );
      if ( style )
        return style;
    }

    if ( key == "fallback" )
      throw Error ("Unable to find builtin \"fallback\" DrawingStyle in %s."
                  ,getString(getName()).c_str());

    return find ( "fallback" );
  }


  void  DisplayStyle::setDarkening ( const DisplayStyle::HSVr& darkening )
  {
    if ( darkening.getHue() < 0.1 ) {
      cerr << "[ERROR] Invalid hue darkening factor: " << darkening.getHue() << "." << endl;
      return;
    }
    if ( darkening.getSaturation() < 0.1 ) {
      cerr << "[ERROR] Invalid saturation darkening factor: " << darkening.getSaturation() << "." << endl;
      return;
    }
    if ( darkening.getValue() < 0.1 ) {
      cerr << "[ERROR] Invalid value darkening factor: " << darkening.getValue() << "." << endl;
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


  void  DisplayStyle::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", "DisplayStyle" );
    jsonWrite( w, "_name"       ,  _name        );
    jsonWrite( w, "_description",  _description );
    jsonWrite( w, "_darkening"  , &_darkening   );

    w->key( "+groups" );
    w->startArray();
    for ( DrawingGroup* dg : _groups ) dg->toJson( w );
    w->endArray();

    w->endObject();
  }


// -------------------------------------------------------------------
// Class :  "JsonDisplayStyle".

  Initializer<JsonDisplayStyle>  jsonDisplayStyleInit ( 0 );


  void  JsonDisplayStyle::initialize ()
  { JsonTypes::registerType( new JsonDisplayStyle (JsonWriter::RegisterMode) ); }


  JsonDisplayStyle::JsonDisplayStyle ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( "_name"       , typeid(string)             );
    add( "_description", typeid(string)             );
    add( "_darkening"  , typeid(DisplayStyle::HSVr) );
    add( "+groups"     , typeid(JsonArray)          );
  }


  JsonDisplayStyle::~JsonDisplayStyle ()
  { }


  string  JsonDisplayStyle::getTypeName () const
  { return "DisplayStyle"; }


  JsonDisplayStyle* JsonDisplayStyle::clone ( unsigned long flags ) const
  { return new JsonDisplayStyle ( flags ); }


  void JsonDisplayStyle::toData(JsonStack& stack)
  {
    cdebug_log(19,1);

    check( stack, "JsonDisplayStyle::toData" );

    Graphics*     graphics = get<Graphics*>( stack, "_graphics" );
    DisplayStyle* ds       = NULL;

    if (not graphics) return;

    string              name        = get<string>             ( stack, "_name"        );
    string              description = get<string>             ( stack, "_description" );
    DisplayStyle::HSVr* darkening   = get<DisplayStyle::HSVr*>( stack, "_darkening"   );

    if (stack.issetFlags(JsonWriter::TechnoMode)) {
    // Actual creation.
      ds = new DisplayStyle ( name );
      ds->setDescription(  description );
      ds->setDarkening  ( *darkening   );
      graphics->addStyle( ds );
    } else {
    // Check coherency with existing DisplayStyle.
      ds = Graphics::getStyle( name );
      if (ds == NULL) {
        cerr << Error( "JsonDisplayStyle::toData(): No DisplayStyle \"%s\" in the existing DisplayStyle."
                     , name.c_str()
                     ) << endl;
      }
    }
    
    update( stack, ds );

    cdebug_tabw(19,-1);
  }


// -------------------------------------------------------------------
// Class :  "JsonHSVr".

  Initializer<JsonHSVr>  jsonHSVrInit ( 0 );


  void  JsonHSVr::initialize ()
  { JsonTypes::registerType( new JsonHSVr (JsonWriter::RegisterMode) ); }


  JsonHSVr::JsonHSVr ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( "_hue"       , typeid(double) );
    add( "_saturation", typeid(double) );
    add( "_value"     , typeid(double) );
  }


  JsonHSVr::~JsonHSVr ()
  { }


  string  JsonHSVr::getTypeName () const
  { return "HSVr"; }


  JsonHSVr* JsonHSVr::clone ( unsigned long flags ) const
  { return new JsonHSVr ( flags ); }


  void JsonHSVr::toData(JsonStack& stack)
  {
    cdebug_log(19,1);

    check( stack, "JsonHSVr::toData" );

    double hue        = get<double>( stack, "_hue"        );
    double saturation = get<double>( stack, "_saturation" );
    double value      = get<double>( stack, "_value"      );

    DisplayStyle::HSVr* hsvr = new DisplayStyle::HSVr(hue,saturation,value);
    
    update( stack, hsvr );

    cdebug_tabw(19,-1);
  }


}  // Hurricane namespace.
