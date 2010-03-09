
// -*- C++ -*-


# include  <cassert>
# include  <string>

# include  <QStringList>
# include  <QXmlStreamReader>

# include  "hurricane/Error.h"
# include  "hurricane/Warning.h"
# include  "hurricane/DbU.h"
# include  "hurricane/DataBase.h"
# include  "hurricane/Technology.h"
# include  "hurricane/BasicLayer.h"

# include  "crlcore/Utilities.h"
# include  "crlcore/RealTechnologyParser.h"




namespace CRL {


  using namespace std;

  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DbU;


  const QString  RealTechnologyParser::MissingSymbolicNameError        = "Missing symbolic layer name attribute";
  const QString  RealTechnologyParser::MissingRealNameError            = "Missing real layer name attribute";
  const QString  RealTechnologyParser::InvalidSymbolicNameError        = "Reference to unknown symbolic layer \"%1\"";
  const QString  RealTechnologyParser::MissingGridValueError           = "Missing grid value attribute";
  const QString  RealTechnologyParser::MissingGridsPerLambdaValueError = "Missing gridsperlambda value attribute";
  const QString  RealTechnologyParser::MissingGridUnitError            = "Missing grid unit attribute";
  const QString  RealTechnologyParser::UnknownGridUnitError            = "Unknown grid unit kind \"%1\"";


  void  RealTechnologyParser::parseTechnology ()
  {
    parseTags ( TagsTechnology );
  }


  void  RealTechnologyParser::parseReal ()
  {
    parseTags ( TagsReal );
  }


  void  RealTechnologyParser::parseName ()
  {
  // May do something of the name here...
    readTextAsString();
  }


  void  RealTechnologyParser::parseGrid ()
  {
    QString           value;
    double            gridValue;
    DbU::UnitPower    gridUnit;

    value = _reader->attributes().value("value").toString();
    if ( !value.isEmpty() )
      gridValue = value.toDouble ();
    else
      printError ( MissingGridValueError );

    value = _reader->attributes().value("unit").toString();
    if ( !value.isEmpty() ) {
      if      ( value == "pico"   ) gridUnit = DbU::Pico;
      else if ( value == "nano"   ) gridUnit = DbU::Nano;
      else if ( value == "micron" ) gridUnit = DbU::Micro;
      else if ( value == "milli"  ) gridUnit = DbU::Milli;
      else if ( value == "unity"  ) gridUnit = DbU::Unity;
      else if ( value == "kilo"   ) gridUnit = DbU::Kilo;
      else
        printError ( UnknownGridUnitError.arg(value) );
    }
    else
      printError ( MissingGridUnitError );

    DbU::setPhysicalsPerGrid ( gridValue, gridUnit );

    parseNoChilds ();
  }


  void  RealTechnologyParser::parseGridsPerLambda ()
  {
    QString  value;
    double   gridsPerLambda;

    value = _reader->attributes().value("value").toString();
    if ( !value.isEmpty() )
      gridsPerLambda = value.toInt ();
    else
      printError ( MissingGridsPerLambdaValueError );

    DbU::setGridsPerLambda ( gridsPerLambda );

    parseNoChilds ();
  }


  void  RealTechnologyParser::parseLayers ()
  {
    parseTags ( TagsLayers );
  }


  void  RealTechnologyParser::parseProcessLayer ()
  {
    QString           value;
    string            symbolicName;
    string            realName;
    unsigned int      gdsIInumber = (unsigned int)-1;

    value = _reader->attributes().value("symbolic").toString();
    if ( !value.isEmpty() )
      symbolicName = value.toStdString ();
    else
      printError ( MissingSymbolicNameError );

    value = _reader->attributes().value("real").toString();
    if ( !value.isEmpty() )
      realName = value.toStdString ();
    else
      printError ( MissingRealNameError );

    value = _reader->attributes().value("gdsII").toString();
    if ( !value.isEmpty() )
      gdsIInumber = value.toUInt ();

    if ( !symbolicName.empty() ) {
      BasicLayer* basicLayer = _technology->getBasicLayer ( symbolicName );
      if ( basicLayer ) {
        basicLayer->setRealName      ( realName.c_str() );
        basicLayer->setExtractNumber ( gdsIInumber );
      } else
        printError ( InvalidSymbolicNameError.arg(symbolicName.c_str()) );
    }

    parseNoChilds ();
  }


  RealTechnologyParser::RealTechnologyParser ( DataBase* db, QXmlStreamReader* reader )
    : XmlParser(reader,TagSetSize)
    , _dataBase(db)
    , _technology(NULL)
  {
    assert ( _dataBase != NULL );

    _technology = db->getTechnology ();
    assert ( _technology != NULL );

    addTagEntry ( TagsStandAlone, "technology"    , (tagParser_t)&RealTechnologyParser::parseTechnology     );
    addTagEntry ( TagsTechnology, "real"          , (tagParser_t)&RealTechnologyParser::parseReal           );
    addTagEntry ( TagsReal      , "name"          , (tagParser_t)&RealTechnologyParser::parseName           );
    addTagEntry ( TagsReal      , "grid"          , (tagParser_t)&RealTechnologyParser::parseGrid           );
    addTagEntry ( TagsReal      , "gridsperlambda", (tagParser_t)&RealTechnologyParser::parseGridsPerLambda );
    addTagEntry ( TagsReal      , "layers"        , (tagParser_t)&RealTechnologyParser::parseLayers         );
    addTagEntry ( TagsLayers    , "processlayer"  , (tagParser_t)&RealTechnologyParser::parseProcessLayer   );
  }


  RealTechnologyParser* RealTechnologyParser::create ( DataBase* db, QXmlStreamReader* reader )
  {
    return new RealTechnologyParser ( db, reader );
  }


  bool  RealTechnologyParser::load ( DataBase* db, const string& path )
  {
    bool  aborted = false;

    try {
      RealTechnologyParser  rtp ( db );
      return rtp._load ( path );
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
      cerr << "[ERROR] Aborting & unloading S2R Technology." << endl;

    //Technology* technology = db->getTechnology ();
    //if ( technology ) technology->destroy ();

      exit ( 1 );
    }

    return false;
  }


  void  RealTechnologyParser::_postLoad ()
  {
  }


  const char* RealTechnologyParser::_getMessage ( MessageId id )
  {
    const char* message = "<unknwown message id>";
    switch ( id ) {
      case OpenFile: message = "S2R Technology configuration"; break;
    }

    return message;
  }


} // End of CRL namespace.
