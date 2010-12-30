
// -*- C++ -*-


# include  <cassert>
# include  <string>
# include  <QXmlStreamReader>
# include  "hurricane/viewer/DisplayStyle.h"
# include  "hurricane/viewer/Graphics.h"
# include  "crlcore/GraphicsParser.h"


namespace CRL {

  using Hurricane::Graphics;


  void  GraphicsParser::parseDefault ()
  {
    QString state = readTextAsString ();
    if ( state == "true" )
      _defaultDisplayStyle = _displayStyle;
  }


  void  GraphicsParser::parseInherit ()
  {
    Name name = readTextAsString().toStdString();

    DisplayStyle* baseStyle = Graphics::getStyle ( name );
    if ( baseStyle )
      _displayStyle->inheritFrom ( baseStyle );
    else
      cerr << "[ERROR] Base style \"" << getString(name)
           << "\" of \"" << getString(_displayStyle->getName())
           << "\" doesn't exist (yet?)." << endl; 
  }


  void  GraphicsParser::parseDescription ()
  {
    string description = readTextAsString().toStdString();

    _displayStyle->setDescription ( description );
  }


  void  GraphicsParser::parseDarkening ()
  {
    QString attribute;
    float   hue        = 1.0;
    float   saturation = 1.0;
    float   value      = 1.0;

    attribute = _reader->attributes().value("hue").toString();
    if ( not attribute.isEmpty() )
      hue = attribute.toFloat ();

    attribute = _reader->attributes().value("saturation").toString();
    if ( not attribute.isEmpty() )
      saturation = attribute.toFloat ();

    attribute = _reader->attributes().value("value").toString();
    if ( not attribute.isEmpty() )
      value = attribute.toFloat ();

    _displayStyle->setDarkening ( DisplayStyle::HSVr(hue,saturation,value) );

    parseNoChilds ();
  }


  void  GraphicsParser::parseDrawingStyle ()
  {
    string  pattern          = "FFFFFFFFFFFFFFFF";
    int     red              = 255;
    int     green            = 255;
    int     blue             = 255;
    int     border           = 0;
    float   threshold        = 1.0;
    Name    drawingStyleName = "unnamed";
    bool    goMatched        = true;
    QString value;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      drawingStyleName = value.toStdString ();
    else
      cerr << "[ERROR] Unnamed drawing style." << endl;

    value = _reader->attributes().value("color").toString();
    if ( !value.isEmpty() ) {
      QStringList components = value.split ( "," );
      if ( components.size() == 3 ) {
        red   = components[0].toInt ();
        green = components[1].toInt ();
        blue  = components[2].toInt ();
      } else
      cerr << "[ERROR] Malformed color attribute." << endl;
    } else
      cerr << "[ERROR] Missing madatory color attribute." << endl;

    value = _reader->attributes().value("pattern").toString();
    if ( !value.isEmpty() )
      pattern = value.toStdString ();

    value = _reader->attributes().value("border").toString();
    if ( !value.isEmpty() )
      border = value.toInt ();

    value = _reader->attributes().value("threshold").toString();
    if ( !value.isEmpty() )
      threshold = value.toFloat ();

    value = _reader->attributes().value("goMatched").toString().toLower();
    if      ( value == "true"  ) goMatched = true;
    else if ( value == "false" ) goMatched = false;

    _displayStyle->addDrawingStyle ( _drawingGroupName
                                   , drawingStyleName
                                   , pattern
                                   , red
                                   , green
                                   , blue
                                   , border
                                   , threshold
                                   , goMatched
                                   );

    parseNoChilds ();
  }


  void  GraphicsParser::parseDrawingGroup ()
  {
    _drawingGroupName = _reader->attributes().value("name").toString().toStdString();
    if ( _drawingGroupName.isEmpty() )
      cerr << "[ERROR] Empty group name." << endl;

    parseTags ( TagsDrawingGroup );
  }


  void  GraphicsParser::parseDisplayStyle ()
  {
    Name name = _reader->attributes().value("name").toString().toStdString();
    _displayStyle = new DisplayStyle ( name ); 
    Graphics::addStyle ( _displayStyle );

    if ( !_defaultDisplayStyle )
      _defaultDisplayStyle = _displayStyle;

    parseTags ( TagsDisplayStyle );
  }


  void  GraphicsParser::parseDisplayStyles ()
  {
    parseTags ( TagsDisplayStyles );
  }


  void  GraphicsParser::parseGraphics ()
  {
    parseTags ( TagsGraphics );

    if ( _defaultDisplayStyle )
      Graphics::setStyle ( _defaultDisplayStyle->getName() );
  }


  GraphicsParser::GraphicsParser ( QXmlStreamReader* reader )
    : XmlParser(reader,6)
    , _displayStyle(NULL)
    , _defaultDisplayStyle(NULL)
    , _drawingGroupName()
  {
    addTagEntry ( TagsStandAlone   , "graphics"     , (tagParser_t)&GraphicsParser::parseGraphics      );
    addTagEntry ( TagsGraphics     , "displaystyles", (tagParser_t)&GraphicsParser::parseDisplayStyles );
    addTagEntry ( TagsDisplayStyles, "displaystyle" , (tagParser_t)&GraphicsParser::parseDisplayStyle  );
    addTagEntry ( TagsDisplayStyle , "group"        , (tagParser_t)&GraphicsParser::parseDrawingGroup  );
    addTagEntry ( TagsDisplayStyle , "default"      , (tagParser_t)&GraphicsParser::parseDefault       );
    addTagEntry ( TagsDisplayStyle , "inherit"      , (tagParser_t)&GraphicsParser::parseInherit       );
    addTagEntry ( TagsDisplayStyle , "description"  , (tagParser_t)&GraphicsParser::parseDescription   );
    addTagEntry ( TagsDisplayStyle , "darkening"    , (tagParser_t)&GraphicsParser::parseDarkening     );
    addTagEntry ( TagsDrawingGroup , "drawingstyle" , (tagParser_t)&GraphicsParser::parseDrawingStyle  );
  }


  GraphicsParser* GraphicsParser::create ( QXmlStreamReader* reader )
  {
    return new GraphicsParser ( reader );
  }


  bool  GraphicsParser::load ( const string& path )
  {
    GraphicsParser  gp;
    return gp._load ( path );
  }


  void  GraphicsParser::_postLoad ()
  {
    if ( getDefaultDisplayStyle() )
      Graphics::setStyle ( getDefaultDisplayStyle()->getName() );
  }


  const char* GraphicsParser::_getMessage ( MessageId id )
  {
    const char* message = "<unknwown message id>";
    switch ( id ) {
      case OpenFile: message = "graphics configuration"; break;
    }

    return message;
  }


} // End of CRL namespace.
