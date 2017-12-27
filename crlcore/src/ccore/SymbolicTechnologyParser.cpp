
// -*- C++ -*-


#include  <cassert>
#include  <string>
#include  <algorithm>

#include  <QStringList>
#include  <QXmlStreamReader>

#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DbU.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/RegularLayer.h"
#include  "hurricane/DiffusionLayer.h"
#include  "hurricane/TransistorLayer.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/ContactLayer.h"

#include  "crlcore/SymbolicTechnologyParser.h"
#include  "crlcore/Utilities.h"




namespace CRL {


  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DbU;
  using Hurricane::RegularLayer;
  using Hurricane::DiffusionLayer;
  using Hurricane::TransistorLayer;
  using Hurricane::TransistorLayer;
  using Hurricane::ContactLayer;
  using Hurricane::ViaLayer;


  const QString  SymbolicTechnologyParser::UnnamedRuleError           = "Rule has an empty name.";
  const QString  SymbolicTechnologyParser::UnnamedBasicLayerError     = "Missing name attribute in <basiclayer>.";
  const QString  SymbolicTechnologyParser::UnnamedSymbolicLayerError  = "Missing name attribute in <%1>.";
  const QString  SymbolicTechnologyParser::UnnamedLayerError          = "Missing name attribute in <layer>.";
  const QString  SymbolicTechnologyParser::InvalidRulePathError       = "Invalid rule path \"%1\".";
  const QString  SymbolicTechnologyParser::MissingValueError          = "Rule \"%1\" has no <value> attribute.";
  const QString  SymbolicTechnologyParser::UnknownRuleError           = "Misspelled or unknown rule \"%1\".";
  const QString  SymbolicTechnologyParser::UndefinedLayerError        = "Reference to yet undefined layer \"%1\".";
  const QString  SymbolicTechnologyParser::NotACompositeLayerError    = "Rule layer \"%1\" is not a composite layer.";
  const QString  SymbolicTechnologyParser::NotABasicLayerError        = "Rule sub-layer \"%1\" is not a basic layer.";
  const QString  SymbolicTechnologyParser::LayerOutnumber             = "More than %1 sub-layers in <%2>.";
  const QString  SymbolicTechnologyParser::LayerMissingLayer          = "Less than %1 sub-layers in <%2>.";


  void  SymbolicTechnologyParser::parseTechnology ()
  {
    parseTags ( TagsTechnology );
  }


  void  SymbolicTechnologyParser::parseName ()
  {
    _technology->setName ( readTextAsString().toStdString().c_str() );
  }


  void  SymbolicTechnologyParser::parseBasicLayer ()
  {
    QString               value;
    string                layerName;
    BasicLayer::Material  layerMaterial = BasicLayer::Material::other;
    unsigned              extractNumber = 0;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedBasicLayerError );

    value = _reader->attributes().value("material").toString();
    if ( !value.isEmpty() ) {
      if      ( value == "nWell"    ) layerMaterial = BasicLayer::Material::nWell;
      else if ( value == "pWell"    ) layerMaterial = BasicLayer::Material::pWell;
      else if ( value == "nImplant" ) layerMaterial = BasicLayer::Material::nImplant;
      else if ( value == "pImplant" ) layerMaterial = BasicLayer::Material::pImplant;
      else if ( value == "active"   ) layerMaterial = BasicLayer::Material::active;
      else if ( value == "poly"     ) layerMaterial = BasicLayer::Material::poly;
      else if ( value == "cut"      ) layerMaterial = BasicLayer::Material::cut;
      else if ( value == "metal"    ) layerMaterial = BasicLayer::Material::metal;
      else if ( value == "blockage" ) layerMaterial = BasicLayer::Material::blockage;
      else if ( value == "other"    ) layerMaterial = BasicLayer::Material::other;
    }

    if ( !layerName.empty() ) {
      _basicLayer = BasicLayer::create ( _technology, layerName, layerMaterial, extractNumber );
      _layers [ _basicLayer->getName() ] = _basicLayer;
    }

    value = _reader->attributes().value("basiclayer").toString();
    if ( not value.isEmpty() and (layerMaterial == BasicLayer::Material::blockage) ) {
      layerName = value.toStdString ();
      BasicLayer* routingLayer = _technology->getBasicLayer ( layerName.c_str() );
      if ( routingLayer and _basicLayer ) {
        routingLayer->setBlockageLayer ( _basicLayer );
      }
    }

    parseNoChilds ();

    _basicLayer = NULL;
  }


  void  SymbolicTechnologyParser::parseRegularLayer ()
  {
    QString       value;
    string        layerName;
    RegularLayer* regularLayer = NULL;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedSymbolicLayerError.arg("regularlayer") );

    _layerComponents.clear ();

    parseTags ( TagsBasicLayers );

    if ( !layerName.empty() ) {
      if ( !_layerComponents.empty() ) {
        if ( _layerComponents.size() > 1 )
          printError ( LayerOutnumber.arg("1","regularlayer") );

        regularLayer = RegularLayer::create ( _technology, layerName );
        regularLayer->setBasicLayer ( _layerComponents[0] );
        _layers [ regularLayer->getName() ] = regularLayer;
        _layerComponents.clear ();
      }
    }
  }


  void  SymbolicTechnologyParser::parseDiffusionLayer ()
  {
    QString         value;
    string          layerName;
    DiffusionLayer* layer     = NULL;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedSymbolicLayerError.arg("diffusionlayer") );

    _layerComponents.clear ();

    parseTags ( TagsBasicLayers );

    if ( !layerName.empty() ) {
      reverse ( _layerComponents.begin(), _layerComponents.end() );
      switch ( _layerComponents.size() ) {
        case 0:
        case 1:
          printError ( LayerMissingLayer.arg("2","diffusionlayer") );
          break;
        case 2:
          _layerComponents.push_back ( NULL );
        default:
          layer = DiffusionLayer::create ( _technology
                                         , layerName
                                         , _layerComponents[0]
                                         , _layerComponents[1]
                                         , _layerComponents[2]
                                         );
          _layers [ layer->getName() ] = layer;
          if ( _layerComponents.size() > 3 )
            printError ( LayerOutnumber.arg("3","diffusionlayer") );
      }
    }

    _layerComponents.clear ();
  }


  void  SymbolicTechnologyParser::parseTransistorLayer ()
  {
    QString          value;
    string           layerName;
    TransistorLayer* layer     = NULL;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedSymbolicLayerError.arg("transistorlayer") );

    _layerComponents.clear ();

    parseTags ( TagsBasicLayers );

    if ( !layerName.empty() ) {
      reverse ( _layerComponents.begin(), _layerComponents.end() );
      switch ( _layerComponents.size() ) {
        case 0:
        case 1:
        case 2:
          printError ( LayerMissingLayer.arg("3","transistorlayer") );
          break;
        case 3:
          _layerComponents.push_back ( NULL );
        default:
          layer = TransistorLayer::create ( _technology
                                          , layerName
                                          , _layerComponents[0]
                                          , _layerComponents[1]
                                          , _layerComponents[2]
                                          , _layerComponents[3]
                                          );
          _layers [ layer->getName() ] = layer;
          if ( _layerComponents.size() > 4 )
            printError ( LayerOutnumber.arg("4","transistorlayer") );
      }
    }

    _layerComponents.clear ();
  }


  void  SymbolicTechnologyParser::parseContactLayer ()
  {
    QString       value;
    string        layerName;
    ContactLayer* layer     = NULL;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedSymbolicLayerError.arg("contactlayer") );

    _layerComponents.clear ();

    parseTags ( TagsBasicLayers );

    if ( !layerName.empty() ) {
      reverse ( _layerComponents.begin(), _layerComponents.end() );
      switch ( _layerComponents.size() ) {
        case 0:
        case 1:
        case 2:
        case 3:
          printError ( LayerMissingLayer.arg("3","contactlayer") );
          break;
        case 4:
          _layerComponents.push_back ( NULL );
        default:
          layer = ContactLayer::create ( _technology
                                       , layerName
                                       , _layerComponents[0]
                                       , _layerComponents[1]
                                       , _layerComponents[2]
                                       , _layerComponents[3]
                                       , _layerComponents[4]
                                       );
          _layers [ layer->getName() ] = layer;
          if ( _layerComponents.size() > 5 )
            printError ( LayerOutnumber.arg("5","contactlayer") );
      }
    }

    _layerComponents.clear ();
  }


  void  SymbolicTechnologyParser::parseViaLayer ()
  {
    QString   value;
    string    layerName;
    ViaLayer* layer     = NULL;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedSymbolicLayerError.arg("vialayer") );

    _layerComponents.clear ();

    parseTags ( TagsBasicLayers );

    if ( !layerName.empty() ) {
      reverse ( _layerComponents.begin(), _layerComponents.end() );
      switch ( _layerComponents.size() ) {
        case 0:
        case 1:
          printError ( LayerMissingLayer.arg("3","vialayer") );
          break;
        default:
          layer = ViaLayer::create ( _technology
                                   , layerName
                                   , _layerComponents[0]
                                   , _layerComponents[1]
                                   , _layerComponents[2]
                                   );
          _layers [ layer->getName() ] = layer;
          if ( _layerComponents.size() > 3 )
            printError ( LayerOutnumber.arg("3","vialayer") );
      }
    }

    _layerComponents.clear ();
  }


  void  SymbolicTechnologyParser::parseLayer ()
  {
    QString  value;
    string   layerName;

    value = _reader->attributes().value("name").toString();
    if ( !value.isEmpty() )
      layerName = value.toStdString ();
    else
      printError ( UnnamedLayerError );

    if ( !layerName.empty() ) {
      BasicLayer* basicLayer = getBasicLayer ( QString(layerName.c_str()), false );
      if ( basicLayer )
        _layerComponents.push_back ( basicLayer );
    }

    parseNoChilds ();
  }


  void  SymbolicTechnologyParser::parseSymbolic ()
  {
    parseTags ( TagsSymbolic );
  }


  void  SymbolicTechnologyParser::parsePrecision ()
  {
    DbU::setPrecision ( readTextAsUInt() );
  }


  void  SymbolicTechnologyParser::parseGridStep ()
  {
  //DbU::setGridStep ( DbU::symbolic( readTextAsDouble()) );
    parseNoChilds ();
  }


  void  SymbolicTechnologyParser::parseRules ()
  {
    parseTags ( TagsRules );
  }


  void  SymbolicTechnologyParser::parseRule ()
  {
    double        doubleValue = 0.0;
    Layer*        ruleLayer   = NULL;
    BasicLayer*   basicLayer  = NULL;
    QString       ruleName;
    unsigned int  ruleType;

    ruleName = _reader->attributes().value("name").toString();
    if ( !ruleName.isEmpty() ) {
    //cerr << qPrintable(ruleName) << endl;
      ruleType = splitRulePath ( ruleName, ruleLayer, basicLayer );
      if ( ( ruleType != InvalidRule ) && ( ruleLayer ) ) {
        QString ruleValue = _reader->attributes().value("value").toString();
        if ( !ruleValue.isEmpty() ) {
          switch ( ruleType & ValueMask ) {
            case DoubleValue: doubleValue = ruleValue.toDouble(); break;
          }
          switch ( ruleType ) {
            case ExtentionCap:   ruleLayer->setExtentionCap  (basicLayer,DbU::lambda(doubleValue)); break;
            case ExtentionWidth: ruleLayer->setExtentionWidth(basicLayer,DbU::lambda(doubleValue)); break;
            case Enclosure:      ruleLayer->setEnclosure     (basicLayer,DbU::lambda(doubleValue)); break;
            case MinimumWidth:   ruleLayer->setMinimalSize   (DbU::lambda(doubleValue)); break;
            case MinimumSide:    ruleLayer->setMinimalSize   (DbU::lambda(doubleValue)); break;
          }
        } else
          printError ( MissingValueError.arg(ruleName) );
      } else
        if ( ruleLayer )
          printError ( UnknownRuleError.arg(ruleName) );
    } else
      printError ( UnnamedRuleError );

    parseNoChilds ();
  }


  unsigned int  SymbolicTechnologyParser::splitRulePath ( const QString& path
                                                        , Layer*&        ruleLayer
                                                        , BasicLayer*&   basicLayer
                                                        )
  {
    ruleLayer  = NULL;
    basicLayer = NULL;

    unsigned int  ruleType     = InvalidRule;
    QStringList   pathElements = path.split ( "." );

    if ( pathElements.isEmpty() ) {
      printError ( InvalidRulePathError.arg(path) );
      return ruleType;
    }

    ruleLayer = getLayer ( pathElements[0], false  );
    pathElements.pop_front ();

    basicLayer = getBasicLayer ( pathElements[0], true );
    if ( basicLayer ) pathElements.pop_front ();

    QString  tailPath = pathElements.join ( "." );

    if      ( tailPath == "extention.cap"      ) ruleType = ExtentionCap;
    else if ( tailPath == "extention.width"    ) ruleType = ExtentionWidth;
    else if ( tailPath == "enclosure"          ) ruleType = Enclosure;
    else if ( tailPath == "minimum.width"      ) ruleType = MinimumWidth;
    else if ( tailPath == "minimum.side"       ) ruleType = MinimumSide;

    return ruleType;
  }


  Layer* SymbolicTechnologyParser::getLayer ( const QString& name, bool ignoreError )
  {
    Name                              layerName = name.toStdString();
    map<const Name,Layer*>::iterator  it        = _layers.find ( layerName );

    if ( it == _layers.end() ) {
      if ( !ignoreError )
        printError ( UndefinedLayerError.arg(name) );
      return NULL;
    }

    return it->second;
  }


  BasicLayer* SymbolicTechnologyParser::getBasicLayer ( const QString& name, bool ignoreError )
  {
    Name                              layerName = name.toStdString();
    map<const Name,Layer*>::iterator  it        = _layers.find ( layerName );

    if ( it == _layers.end() ) {
      if ( !ignoreError )
        printError ( UndefinedLayerError.arg(name) );
      return NULL;
    }

    BasicLayer* basicLayer = dynamic_cast<BasicLayer*> ( it->second );
    if ( !basicLayer )
      printError ( NotABasicLayerError.arg(name) );

    return basicLayer;
  }


  SymbolicTechnologyParser::SymbolicTechnologyParser ( DataBase* db, QXmlStreamReader* reader )
    : XmlParser(reader,TagSetSize)
    , _dataBase(db)
    , _technology(NULL)
    , _basicLayer(NULL)
    , _layers()
    , _layerComponents()
  {
    assert ( _dataBase != NULL );

    _technology = db->getTechnology ();
    if ( !_technology )
      _technology = Technology::create ( db, "<SymbolicTechnologyParser>" );

    addTagEntry ( TagsStandAlone    , "technology"     , (tagParser_t)&SymbolicTechnologyParser::parseTechnology      );
    addTagEntry ( TagsTechnology    , "name"           , (tagParser_t)&SymbolicTechnologyParser::parseName            );
    addTagEntry ( TagsTechnology    , "basiclayer"     , (tagParser_t)&SymbolicTechnologyParser::parseBasicLayer      );
    addTagEntry ( TagsTechnology    , "regularlayer"   , (tagParser_t)&SymbolicTechnologyParser::parseRegularLayer    );
    addTagEntry ( TagsTechnology    , "diffusionlayer" , (tagParser_t)&SymbolicTechnologyParser::parseDiffusionLayer  );
    addTagEntry ( TagsTechnology    , "transistorlayer", (tagParser_t)&SymbolicTechnologyParser::parseTransistorLayer );
    addTagEntry ( TagsTechnology    , "vialayer"       , (tagParser_t)&SymbolicTechnologyParser::parseViaLayer        );
    addTagEntry ( TagsTechnology    , "contactlayer"   , (tagParser_t)&SymbolicTechnologyParser::parseContactLayer    );
    addTagEntry ( TagsBasicLayers   , "layer"          , (tagParser_t)&SymbolicTechnologyParser::parseLayer           );
    addTagEntry ( TagsTechnology    , "symbolic"       , (tagParser_t)&SymbolicTechnologyParser::parseSymbolic        );
    addTagEntry ( TagsSymbolic      , "precision"      , (tagParser_t)&SymbolicTechnologyParser::parsePrecision       );
    addTagEntry ( TagsSymbolic      , "gridstep"       , (tagParser_t)&SymbolicTechnologyParser::parseGridStep        );
    addTagEntry ( TagsSymbolic      , "rules"          , (tagParser_t)&SymbolicTechnologyParser::parseRules           );
    addTagEntry ( TagsRules         , "rule"           , (tagParser_t)&SymbolicTechnologyParser::parseRule            );
  }


  SymbolicTechnologyParser* SymbolicTechnologyParser::create ( DataBase* db, QXmlStreamReader* reader )
  {
    return new SymbolicTechnologyParser ( db, reader );
  }


  bool  SymbolicTechnologyParser::load ( DataBase* db, const string& path )
  {
    bool  aborted = false;

    try {
      SymbolicTechnologyParser  tp ( db );
      return tp._load ( path );
    }
    catch ( Error& e ) {
      cerr << e.what() << endl;
      aborted = true;
    }
    catch ( ... ) {
      cout << "[ERROR] Abnormal termination: unknown exception.\n" << endl;
      exit ( 2 );
    }

    if ( aborted ) {
      cerr << "[ERROR] Aborting & unloading Technology." << endl;

    //Technology* technology = db->getTechnology ();
    //if ( technology ) technology->destroy ();

      exit ( 1 );
    }

    return false;
  }


  void  SymbolicTechnologyParser::_postLoad ()
  {
  // Fixme.
    _technology->setSymbolicLayer ( "POLY" );
    _technology->setSymbolicLayer ( "METAL1" );
    _technology->setSymbolicLayer ( "METAL2" );
    _technology->setSymbolicLayer ( "METAL3" );
    _technology->setSymbolicLayer ( "METAL4" );
    _technology->setSymbolicLayer ( "METAL5" );
    _technology->setSymbolicLayer ( "METAL6" );
    _technology->setSymbolicLayer ( "BLOCKAGE1" );
    _technology->setSymbolicLayer ( "BLOCKAGE2" );
    _technology->setSymbolicLayer ( "BLOCKAGE3" );
    _technology->setSymbolicLayer ( "BLOCKAGE4" );
    _technology->setSymbolicLayer ( "BLOCKAGE5" );
    _technology->setSymbolicLayer ( "BLOCKAGE6" );
    _technology->setSymbolicLayer ( "gmetalh" );
    _technology->setSymbolicLayer ( "gmetalv" );
    _technology->setSymbolicLayer ( "VIA12" );
    _technology->setSymbolicLayer ( "VIA23" );
    _technology->setSymbolicLayer ( "VIA34" );
    _technology->setSymbolicLayer ( "VIA45" );
    _technology->setSymbolicLayer ( "VIA56" );
    _technology->setSymbolicLayer ( "gcontact" );
  }


  const char* SymbolicTechnologyParser::_getMessage ( MessageId id )
  {
    const char* message = "<unknwown message id>";
    switch ( id ) {
      case OpenFile: message = "Technology configuration"; break;
    }

    return message;
  }


} // End of CRL namespace.
