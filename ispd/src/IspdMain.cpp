
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s p d   G l o b a l   r o u t i n g  -  M a i n   G U I    |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./IspdMain.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <boost/program_options.hpp>
namespace poptions = boost::program_options;

#include  <QtGui>

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Cell.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HApplication.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
using namespace CRL;

#include "knik/GraphicKnikEngine.h"
#include "knik/KnikEngine.h"
#include "knik/NetExtension.h"
using namespace Knik;

#include "katabatic/GraphicKatabaticEngine.h"
using namespace Katabatic;

#include "IspdGui.h"
using namespace Ispd;


namespace {
// -------------------------------------------------------------------
// Global variables

# define    LINE_SIZE    4096

    enum ParserState { StateGrid
                     , StateVerti
                     , StateHoriz
                     , StateWidth
                     , StateSpacing
                     , StateVia
                     , StateDim
                     , StateNet
                     , StateObs
                     , StateEOF
                     };

    unsigned __congestion__    = 1;
    unsigned __precongestion__ = 2;
    float    __edge_cost__     = 3.0;


    Cell*               _cell = NULL;
    DataBase::DataBase* _db   = NULL;
    KnikEngine*         _knik = NULL;
    AllianceFramework*  _af   = AllianceFramework::create ();
    string              _filePath;
    int                 _parserState = StateGrid;
    size_t              _lineNumber = 0;
    char                _rawLine[LINE_SIZE];
    unsigned            _nbGCellsX;
    unsigned            _nbGCellsY;
    unsigned            _nbLayers;
    DbU::Unit           _lowerLeftX;
    DbU::Unit           _lowerLeftY;
    DbU::Unit           _tileWidth;
    DbU::Unit           _tileHeight;
    vector<unsigned>    _vertiCap;
    vector<unsigned>    _horizCap;
    vector<unsigned>    _minWidth;
    vector<unsigned>    _minSpacing;
    vector<unsigned>    _viaSpacing;
    unsigned            _nbNets;
    unsigned            _nbObs;
    Layer*              _layers[2];
    bool                _createRings = true;

// -------------------------------------------------------------------
// Function  :  "printHelp()".

  void  printHelp ()
  {
    cout << endl;
    cout << "Usage: unicorn [-v|--verbose] [-V|--very-verbose] [-D|--core-dump]    \\\n"
         << "               [-l|--trace-level <traceLevel>] [-c|--cell <cellName>] \\\n"
         << endl;
    cout << "Options:\n"
         << "    o  [-v|--verbose]      : First level of verbosity.\n"
         << "    o  [-V|--very-verbose] : Second level of verbosity (very talkative).\n"
         << "    o  [-D|--core-dump]    : Enable core dumping.\n"
         << "    o  [-l|--trace-level <traceLevel>] :\n"
         << "           Sets the level of trace, trace messages with a level superior to\n"
         << "           <traceLevel> will be printed on <stderr>.\n"
         << "    o  [-c|--cell <cellName>] :\n"
         << "           The name of the Cell to load, without extention.\n"
         << "    o  [-k|--knik] : Run the global router : Knik.\n"
         << "    o  [-K|--KNIK] : Run the global router : Knik, then analyse routing and unroute overflowed segments.\n"
         << endl;
  }

  void  _printWarning ( const char* format, ... )
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    cerr << "[WARNING] GrParser(): " << formatted << "\n"
         << "          (file: " << _filePath << ", line: " << _lineNumber << ")" << endl;
  }

  void  _printError ( bool interrupt, const char* format, ... )
  {
    static char     formatted [ 8192 ];
           va_list  args;

    va_start ( args, format );
    vsnprintf ( formatted, 8191, format, args );
    va_end ( args );

    cerr << "[ERROR] GrParser(): " << formatted << "\n"
         << "        (file: " << _filePath << ", line: " << _lineNumber << ")" << endl;

    if ( interrupt )
      throw Error ( "GrParser processed" );
  }

  long _getLong ( const char* s )
  {
      char* end;
      long value = strtol ( s, &end, 10 );
      if ( *end != '\0' )
          _printError ( false, "Incomplete string to integer conversion for \"%s\" (%ld).", s, value );
      return value;
  }

  vector<char*>  _splitString ( char* s )
  {
    vector<char*>  fields;

    fields.push_back ( s );
    while ( *s != '\0' ) {
      unsigned i = 0;
      if ( *s == ' ' || *s == '\t' ) {
        i++;
        *s = '\0';
        while ( *(s+i) == ' ' || *(s+i) == '\t' )
          i++;
        fields.push_back ( s+i );
        s += i;
      }
      else 
        s++;
    }

    return fields;
  }

  vector<char*>  _splitSegmentString ( char* s )
  {
    vector<char*>  fields;

    //fields.push_back ( s );
    while ( *s != '\0' ) {
      unsigned i = 0;
      if ( *s == '(' || *s == ')' || *s == ',' || *s == '-' ) {
        i++;
        *s = '\0';
        while ( *(s+i) == '(' || *(s+i) == ')' || *(s+i) == ',' || *(s+i) == '-' )
          i++;
        if ( *(s+i+1) != '\0' )
            fields.push_back ( s+i );
        s += i;
      }
      else 
        s++;
    }

    return fields;
  }

  void  _parseGrid ()
  {
    if ( strncmp(_rawLine,"grid", 4) )
      _printError ( true, "Missing Grid Declaration." );

    vector<char*> fields =_splitString ( _rawLine+5 );
    if ( fields.size() != 3 ) {
      _printError ( true, "Malformed Grid Line." );
    }
    else {
        _nbGCellsX = _getLong ( fields[0] );
        _nbGCellsY = _getLong ( fields[1] );
        _nbLayers  = _getLong ( fields[2] );
    }
    if ( _nbLayers != 2 )
        _printError ( false, "Global routing only supports two layers right now !" );
  }

  void  _parseVerti ()
  {
    if ( strncmp(_rawLine, "vertical capacity", 17) )
        _printError ( true, "Missing Vertical Declaration." );
          
    vector<char*>  fields = _splitString ( _rawLine+18 );

    if ( fields.size() < _nbLayers )
      _printError ( true, "Malformed Vertical line." );
    else 
        for ( unsigned i = 0 ; i < _nbLayers ; i++ ) {
            _vertiCap.push_back ( _getLong ( fields[i] ) );
        }
  }

  void  _parseHoriz ()
  {
    if ( strncmp(_rawLine, "horizontal capacity", 19) )
        _printError ( true, "Missing Horizontal Declaration." );
          
    vector<char*>  fields = _splitString ( _rawLine+20 );

    if ( fields.size() < _nbLayers )
      _printError ( true, "Malformed Horizontal line." );
    else
        for ( unsigned i = 0 ; i < _nbLayers ; i++ ) {
            _horizCap.push_back ( _getLong ( fields[i] ) );
        }
  }

  void  _parseWidth ()
  {
    if ( strncmp(_rawLine, "minimum width", 13) )
        _printError ( true, "Missing Minimum Width Declaration." );
          
    vector<char*>  fields = _splitString ( _rawLine+14 );

    if ( fields.size() < _nbLayers )
      _printError ( true, "Malformed Minimum Width line." );
    else
        for ( unsigned i = 0 ; i < _nbLayers ; i++ ) {
            _minWidth.push_back ( _getLong ( fields[i] ) );
        }
  }

  void  _parseSpacing ()
  {
    if ( strncmp(_rawLine, "minimum spacing", 15) )
        _printError ( true, "Missing Minimum Spacing Declaration." );
          
    vector<char*>  fields = _splitString ( _rawLine+16 );

    if ( fields.size() < _nbLayers )
      _printError ( true, "Malformed Minimum Spacing line." );
    else
        for ( unsigned i = 0 ; i < _nbLayers ; i++ ) {
            _minSpacing.push_back ( _getLong ( fields[i] ) );
        }
  }

  void  _parseVia ()
  {
    if ( strncmp(_rawLine, "via spacing", 11) )
        _printError ( true, "Missing Via Spacing Declaration." );
          
    vector<char*>  fields = _splitString ( _rawLine+12 );

    if ( fields.size() < _nbLayers )
      _printError ( true, "Malformed Via Spacing line." );
    else
        for ( unsigned i = 0 ; i < _nbLayers ; i++ ) {
            _viaSpacing.push_back ( _getLong ( fields[i] ) );
        }
  }

  void  _parseDim ()
  {
    vector<char*>  fields = _splitString ( _rawLine );

    if ( fields.size() < 4 )
      _printError ( true, "Malformed Dimension line." );
    else {
        _lowerLeftX = DbU::lambda ( _getLong ( fields[0] ) );
        _lowerLeftY = DbU::lambda ( _getLong ( fields[1] ) );
        _tileWidth  = DbU::lambda ( _getLong ( fields[2] ) );
        _tileHeight = DbU::lambda ( _getLong ( fields[3] ) );
    }

    DbU::Unit cellWidth  = (2*_lowerLeftX)+(_nbGCellsX*_tileWidth);
    DbU::Unit cellHeight = (2*_lowerLeftY)+(_nbGCellsY*_tileHeight);

    cmess1 << "  o  Creating cell ..." << endl
           << "     - " << _nbGCellsX << "x" << _nbGCellsY << " -> " << DbU::getValueString(cellWidth) << "x" << DbU::getValueString(cellHeight) << endl
           << "     - congestion: " << __congestion__ << endl
           << "     - precongestion: " << __precongestion__ << endl
           << "     - edge cost: " << __edge_cost__ << endl;
    _cell = Cell::create ( _af->getLibrary(0), _filePath );
    assert ( _cell );
    _cell->setTerminal(0);
    _cell->setAbutmentBox ( Box ( DbU::lambda(0), DbU::lambda(0), cellWidth, cellHeight ) );
    _knik = KnikEngine::get ( _cell );
    if ( !_knik )
        _knik = KnikEngine::create ( _cell, __congestion__, __precongestion__, true, true, __edge_cost__ );

    unsigned hcapacity = 0;
    for ( unsigned i = 0 ; i < _horizCap.size() ; i++ )
        hcapacity += _horizCap[i];
    hcapacity = hcapacity / ( _minWidth[0]+_minSpacing[0] );

    unsigned vcapacity = 0;
    for ( unsigned i = 0 ; i < _vertiCap.size() ; i++ )
        vcapacity += _vertiCap[i];
    vcapacity = vcapacity / ( _minWidth[1]+_minSpacing[1] ); // XXX we consider only 2 layers!!!
    
    _knik->createRoutingGrid ( _nbGCellsX, _nbGCellsY, _lowerLeftX, _lowerLeftY, _tileWidth, _tileHeight, hcapacity, vcapacity );
    // for ispd07 reload
    _knik->createRoutingGraph();
  }

  void  _parseNets ()
  {
    if ( strncmp(_rawLine, "num net", 7) )
        _printError ( true, "Missing Number of Nets Declaration." );
          
    cmess1 << "  o  Parsing nets ..." << endl;

    vector<char*>  fields = _splitString ( _rawLine+8 );
    if ( fields.size() != 1 )
      _printError ( true, "Malformed Number of Nets line." );
    else 
        _nbNets = _getLong ( fields[0] );
    
    cmess1 << "     - " << _nbNets << " nets found" << endl;
  }

  void _parseNet ( Net* &net, unsigned &nbPins)
  {
      vector<char*> fields = _splitString ( _rawLine );
      if ( fields.size() != 4 )
          _printError ( true, "Malformed Net Line." );
      else {
          Name netName = Name ( fields[0] );
          long netID = _getLong ( fields[1] );

          nbPins = _getLong ( fields[2] );
          net    = Net::create ( _cell, netName );

          //net->put ( StandardPrivateProperty<unsigned>::create(netID) );
          NetExtension::setId ( net, netID );
      }
  }

  void _parseNode ( Net* net, RoutingPad* &firstRoutingPad )
  {
      DbU::Unit x,y;
      long layerID;
      vector<char*> fields = _splitString ( _rawLine );
      if ( fields.size() != 3 ) {
          for (unsigned i = 0 ; i < fields.size(); i ++ ){
              cerr << fields[i] << ",";
          }
          cerr << endl;
          _printError ( true, "Malformed Node Line." );
      }
      else {
          x = DbU::lambda ( _getLong ( fields[0] ) );
          y = DbU::lambda ( _getLong ( fields[1] ) );
          layerID = _getLong ( fields[2] ) - 1;
      }
      //UpdateSession::open();
      Contact*    contact    = Contact::create ( net, _layers[layerID], x, y, DbU::lambda(2), DbU::lambda(2) );
      RoutingPad* routingPad = RoutingPad::create ( net, Occurrence ( contact ) );

      // Dans le cas d'un chargment de solution, il se peut que le routingPad ne soit pas au centre du vertex -> on crée arbitrairement un contact au centre qu'on attache au vertex
      if ( !_createRings )
        _knik->addRoutingPadToGraph ( routingPad );

      if ( _createRings ) {
          if ( firstRoutingPad )
              routingPad->getBodyHook()->attach ( firstRoutingPad->getBodyHook() );
          else
              firstRoutingPad = routingPad;
      }
      //UpdateSession::close();
  }

  void _parseObs ()
  {
    cmess1 << "  o  Parsing obstacles ..." << endl;

    vector<char*>  fields = _splitString ( _rawLine );
    if ( fields.size() != 1 )
      _printError ( true, "Malformed Number of Obstacles line." );
    else 
        _nbObs = _getLong ( fields[0] );
    
    cmess1 << "     - " << _nbObs << " obstacles found" << endl;
  }

  void _parseObstacle ()
  {
      unsigned col1, row1, lID1, col2, row2, lID2, cap;

      vector<char*>  fields = _splitString ( _rawLine );
      if ( fields.size() != 7 )
          _printError ( true, "Malformed Obstacle line." );
      else {
          col1 = _getLong ( fields[0] );
          row1 = _getLong ( fields[1] );
          lID1 = _getLong ( fields[2] );
          col2 = _getLong ( fields[3] );
          row2 = _getLong ( fields[4] );
          lID2 = _getLong ( fields[5] );
          cap  = _getLong ( fields[6] );

          if ( lID1 != lID2 )
              _printError( true, "Layers must be the same on Obstacle line." );

          cap = cap / (_minWidth[lID1-1]+_minSpacing[lID1-1]);
          _knik->updateEdgeCapacity ( col1, row1, col2, row2, cap );
      }
  }

  Cell* loadFromFile()
  {
    string fullPath = _filePath;
    fullPath += ".gr";
    cmess1 << "  o  Loading cell :" << fullPath << endl;


    IoFile fileStream ( fullPath );
    fileStream.open ( "r" );
    if ( !fileStream.isOpen() ) {
        cerr << " [ERROR] Can't find file : " << fullPath  << " !" << endl;
        exit (48);
    }

    _db = DataBase::getDB ();
    _layers[0] = _db->getTechnology()->getLayer ( Name ( "metal1" ) );
    _layers[1] = _db->getTechnology()->getLayer ( Name ( "metal2" ) );
    
    _lineNumber  = 0;
    _parserState = StateGrid;

    try {
      while ( !fileStream.eof() ) {
        fileStream.readLine ( _rawLine, LINE_SIZE );
        _lineNumber++;

        if ( _rawLine[0] == '\0' ) {
          if ( _parserState == StateEOF ) break;
          continue;
        } else {
          if ( _parserState == StateEOF )
            _printError ( true, "Garbage after EOF." );
        }
        if ( !strcmp(_rawLine,"EOF") ) { _parserState = StateEOF; continue; }

        if ( _parserState == StateGrid ) {
          _parseGrid ();
          _parserState = StateVerti;
          continue;
        }

        if ( _parserState == StateVerti ) {
          _parseVerti ();
          _parserState = StateHoriz;
          continue;
        }

        if ( _parserState == StateHoriz ) {
          _parseHoriz ();
          _parserState = StateWidth;
          continue;
        }

        if ( _parserState == StateWidth ) {
          _parseWidth ();
          _parserState = StateSpacing;
          continue;
        }

        if ( _parserState == StateSpacing ) {
          _parseSpacing ();
          _parserState = StateVia;
          continue;
        }

        if ( _parserState == StateVia ) {
          _parseVia ();
          _parserState = StateDim;
          continue;
        }

        if ( _parserState == StateDim ) {
          _parseDim ();
          _parserState = StateNet;
          continue;
        }

        if ( _parserState == StateNet ) {
          _parseNets ();
          for ( unsigned i = 0 ; i < _nbNets ; i++ ) {
              fileStream.readLine ( _rawLine, LINE_SIZE );
              _lineNumber++;
              Net*        net = NULL;
              RoutingPad* firstRoutingPad = NULL;
              unsigned    nbPins = 0;
              _parseNet( net, nbPins );
              for ( unsigned j = 0 ; j < nbPins ; j++ ) {
                  fileStream.readLine ( _rawLine, LINE_SIZE );
                  _lineNumber++;
                  _parseNode ( net, firstRoutingPad );
              }
              //cmess1 << "     [";
              //cmess1.width(3);
              //cmess1 << floor((float)(i*100/(float)(_nbNets)));
              //cmess1 << "%]\r";
          }
          cmess1 << "     [100%] Done." << endl;

          _knik->initGlobalRouting();
          _parserState = StateObs;
          continue;
        }

        if ( _parserState == StateObs ) {
            _parseObs ();
            for ( unsigned i = 0 ; i < _nbObs ; i++ )
            {
                fileStream.readLine ( _rawLine, LINE_SIZE );
                _lineNumber++;
                _parseObstacle ();
                //cmess1 << "     [" << floor((float)(i*100/(float)(_nbObs))) << "%]\r";
            }
            cmess1 << "     [100%] Done." << endl;
            _parserState = StateEOF;
            continue;
        }
      }
    } catch ( Error& e ) {
      if ( e.what() != "[ERROR] GrParser processed" )
        cerr << e.what() << endl;
    }

    fileStream.close ();

    return _cell;
  }

  void loadSolutionFile()
  // ********************
  {
    cmess1 << "  o  Loading solution :" << _filePath << endl;


    IoFile fileStream ( _filePath );
    fileStream.open ( "r" );
    if ( !fileStream.isOpen() ) {
        cerr << " [ERROR] Can't find file : " << _filePath  << " !" << endl;
        exit (48);
    }

    _db = DataBase::getDB ();
    Layer* _gmetalh  = _db->getTechnology()->getLayer ( Name ( "gmetalh"  ) );
    Layer* _gmetalv  = _db->getTechnology()->getLayer ( Name ( "gmetalv"  ) );
    Layer* _gcontact = _db->getTechnology()->getLayer ( Name ( "gcontact" ) );
    
    _lineNumber  = 0;
    unsigned _uselessContact = 0;
    unsigned _illegalVerti   = 0;
    unsigned _illegalHoriz   = 0;
    unsigned _illegalDiag    = 0;
    unsigned _totalVias      = 0;
    unsigned _validSegments  = 0;

    try {
      while ( !fileStream.eof() ) {
        fileStream.readLine ( _rawLine, LINE_SIZE );
        _lineNumber++;

        if ( _rawLine[0] == '\0' )
            break;
        if ( _rawLine[0] == '\n' ) 
            continue;

        vector<char*> fields = _splitString ( _rawLine );
        if ( fields.size() != 3 )
            _printError ( true, "Malformed Net Line." );
        else {
            Name netName = Name ( fields[0] );
            long netID = _getLong ( fields[1] );

            unsigned nbPins = _getLong ( fields[2] );
            Net* net = _cell->getNet ( netName );
            if ( !net ) {
                string message = "Parse solution failed : cannot find net : ";
                message += getString(netName);
                _printError ( true , message.c_str() );
            }
            
            vector<Segment*> segments;
            for ( unsigned i = 0 ; i < nbPins ; i++ ) {
                fileStream.readLine ( _rawLine, LINE_SIZE );
                _lineNumber++;
                fields = _splitSegmentString ( _rawLine );
                if ( fields.size() != 6 )
                    _printError ( true, "Malformed Net Line." );
                else {
                    DbU::Unit xSource = DbU::lambda(_getLong(fields[0]));
                    DbU::Unit ySource = DbU::lambda(_getLong(fields[1]));
                    unsigned  zSource =  (unsigned)(_getLong(fields[2]));
                    DbU::Unit xTarget = DbU::lambda(_getLong(fields[3]));
                    DbU::Unit yTarget = DbU::lambda(_getLong(fields[4]));
                    unsigned  zTarget =  (unsigned)(_getLong(fields[5]));

                    if ( xSource == xTarget ) {
                        if ( ySource == yTarget ) { //contact
                            if ( zSource != zTarget ) {
                                //UpdateSession::open();
                                Contact::create ( net, _gcontact, xSource, ySource );
                                //UpdateSession::close();
                                _totalVias++;
                            }
                            else
                                _uselessContact++;
                        }
                        else { // segment vertical
                            if ( zSource != zTarget ) // illegal segment
                                _illegalVerti++;
                            else {
                                //UpdateSession::open();
                                Vertical* verti = Vertical::create ( net, _gmetalv, xSource );
                                segments.push_back(verti);
                                if ( ySource < yTarget ) {
                                    verti->setDySource ( ySource );
                                    verti->setDyTarget ( yTarget );
                                }
                                else {
                                    verti->setDySource ( yTarget );
                                    verti->setDyTarget ( ySource );
                                }
                                _knik->insertSegment ( verti );
                                //UpdateSession::close();
                                _validSegments++;
                            }
                        }
                    }
                    else { // segment horizontal
                        if ( ySource != yTarget )
                            _illegalDiag++;
                        else {
                            if ( zSource != zTarget )
                                _illegalHoriz++;
                            else {
                                //UpdateSession::open();
                                Horizontal* horiz = Horizontal::create ( net, _gmetalh, ySource );
                                segments.push_back(horiz);
                                if ( xSource < xTarget ) {
                                    horiz->setDxSource ( xSource );
                                    horiz->setDxTarget ( xTarget );
                                }
                                else {
                                    horiz->setDxSource ( xTarget );
                                    horiz->setDxTarget ( xSource );
                                }
                                _knik->insertSegment ( horiz );
                                //UpdateSession::close();
                                _validSegments++;
                            }
                        }
                    }
                }
            }
            fileStream.readLine ( _rawLine, LINE_SIZE );
            _lineNumber++;
            if ( _rawLine[0] != '!' ) 
                throw Error ("gnagnagnagnagna"+getString(_lineNumber));
            // on va relier les segments et les contacts :
            for ( unsigned i = 0 ; i < segments.size() ; i++ ) {
                Segment* segment = segments[i];
                Point sourcePos = segment->getSourcePosition();
                Point targetPos = segment->getTargetPosition();
                Contact* source = NULL;
                Contact* target = NULL;
                forEach ( Contact*, contact, net->getContacts() ) {
                    Point pos = contact->getPosition();
                    if ( pos == sourcePos )
                        source = *contact;
                    else if ( pos == targetPos )
                        target = *contact;
                    if ( source && target )
                        break;
                }
                if ( !source ) {
                    string message = "Cannot find source contact for ";
                    message += getString(segment);
                    throw Error (message);
                }
                if ( !target ) {
                    string message = "Cannot find target contact for ";
                    message += getString(segment);
                    throw Error (message);
                }
                //UpdateSession::open();
                if ( Horizontal* horiz = dynamic_cast<Horizontal*>(segment) ) {
                    horiz->setDxSource(0);
                    horiz->setDxTarget(0);
                }
                else if ( Vertical* verti = dynamic_cast<Vertical*>(segment) ) {
                    verti->setDySource(0);
                    verti->setDyTarget(0);
                }
                else
                    throw Error ("A segment which is not a Horizontal nor a Vertical !!!");
                segment->getSourceHook()->attach(source->getBodyHook());
                segment->getTargetHook()->attach(target->getBodyHook());
                //UpdateSession::close();
            }
        }
      }
    } catch ( Error& e ) {
      if ( e.what() != "[ERROR] GrParser processed" )
        cerr << e.what() << endl;
    }

    fileStream.close ();
  }

  void printToFile(IspdGui* ispd)
  // ****************************
  {
      float width  = DbU::getLambda(_cell->getAbutmentBox().getWidth());
      float height = DbU::getLambda(_cell->getAbutmentBox().getHeight());
      assert(width);
      assert(height);
      float minSize = 800;
      unsigned wWidth  = (unsigned int)((width < height) ? minSize : floor(width*minSize/height));
      unsigned wHeight = (unsigned int)((width < height) ? floor(height*minSize/width) : minSize);
      //unsigned wWidth = floor(width);
      //unsigned wHeight = floor(height);
      CellWidget* widget = ispd->getCellWidget();
      widget->setFixedSize(wWidth, wHeight);
      widget->fitToContents();
      forEach ( Layer*, layer, DataBase::getDB()->getTechnology()->getLayers() )
          widget->setLayerVisible ( layer->getName(), false );
      widget->setLayerVisible ( Name("Knik::Edges"), true );

      QImage image (wWidth, wHeight+60, QImage::Format_RGB32);
      widget->copyToImage(&image);
      string savePath = _filePath;
      savePath += "_map.png";
      image.save(savePath.c_str(),"png");
  }

} // End of anonymous namespace.




// x-----------------------------------------------------------------x
// |                      Fonctions Definitions                      |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Function  :  "main()".

int main ( int argc, char *argv[] )
{
  int  returnCode = 0;

  try {
    float         edgeCapacity;
    unsigned int  traceLevel;
    bool          verbose1;
    bool          verbose2;
    bool          coreDump;
    bool          textMode;
    bool          graphicMode;
    bool          knikSimple;
    bool          knikOverflow;
    bool          loadSolution;
    bool          saveSolution;

    poptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"        , "Print this help." )
      ( "verbose,v"     , poptions::bool_switch(&verbose1)->default_value(false)
                        , "First level of verbosity.")
      ( "very-verbose,V", poptions::bool_switch(&verbose2)->default_value(false)
                        , "Second level of verbosity.")
      ( "text,t"        , poptions::bool_switch(&textMode)->default_value(false)
                        , "Run in pure text mode.")
      ( "core-dump,D"   , poptions::bool_switch(&coreDump)->default_value(false)
                        , "Enable core dumping.")
      ( "trace-level,l" , poptions::value<unsigned int>(&traceLevel)->default_value(1000)
                        , "Set the level of trace, trace messages with a level superior to "
                          "<arg> will be printed on <stderr>." )
      ( "cell,c"        , poptions::value<string>()
                        , "The name of the cell to load, whithout extension." )
      ( "knik,k"        , poptions::bool_switch(&knikSimple)->default_value(false)
                        , "Perform a simple routing pass.");
      ( "KNIK,K"        , poptions::bool_switch(&knikOverflow)->default_value(false)
                        , "Perform a routing pass, then analyse & re-route overflowed edges.");
      ( "solution,s"    , poptions::bool_switch(&loadSolution)->default_value(true)
                        , "Load a previously generated routing solution.");
      ( "Save,S"        , poptions::bool_switch(&saveSolution)->default_value(false)
                        , "Save the routed design.");
      ( "editor,e"      , poptions::bool_switch(&graphicMode)->default_value(false)
                        , "Launch the graphical editor.");

    poptions::variables_map arguments;
    poptions::store  ( poptions::parse_command_line(argc,argv,options), arguments );
    poptions::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl;
      exit ( 0 );
    }

    System::getSystem()->setCatchCore ( not coreDump ); 
    if ( verbose1 ) mstream::enable ( mstream::VerboseLevel1 );
    if ( verbose2 ) mstream::enable ( mstream::VerboseLevel2 ); 
    ltracelevel ( traceLevel );
    _createRings = not loadSolution;

    if ( arguments.count("cell") ) {
      _filePath = arguments["cell"].as<string>();
      UpdateSession::open();
      _cell = loadFromFile();
      UpdateSession::close();
      if (!_cell) {
        cerr << "[ERROR] Cell not found: " << arguments["cell"].as<string>() << endl;
        exit ( -45 );
      }
    }

    if ( loadSolution ) {
      _filePath = arguments["cell"].as<string>();
      UpdateSession::open();
      loadSolutionFile();
      UpdateSession::close();
    }

    if ( not textMode ) {
      QApplication* qa = new HApplication ( argc, argv );
      Graphics::enable ();

      IspdGui* ispd = IspdGui::create ();
      cmess1 << ispd->getBanner() << endl;;

      GraphicKnikEngine* grKnik = Knik::GraphicKnikEngine::grab();
      ispd->registerTool ( grKnik );
      ispd->registerTool ( Katabatic::GraphicKatabaticEngine::grab() );

      ispd->setCell ( _cell );
      ispd->show();
      if ( arguments.count("knik") ) {
        grKnik->route();
        grKnik->analyse();
      }
      else if ( arguments.count("KNIK") ) {
        grKnik->run();
      //bool done = grKnik->analyse();
      //while ( !done ) {
      //    grKnik->unroute();
      //    grKnik->reroute();
      //    done = grKnik->analyse();
      //}
      }
      else if ( saveSolution ) {
      //grKnik->analyse();
        printToFile(ispd);
      }

      if ( not saveSolution or graphicMode )
        returnCode = qa->exec();

      ispd->destroy();
      delete qa;
    } else {
      KnikEngine* knik = Knik::KnikEngine::get(_cell);
      if ( arguments.count("knik") ) {
        knik->Route();
        knik->analyseRouting();
        if ( saveSolution ) knik->saveSolution();
      } else if ( arguments.count("KNIK") ) {
      //knik->Route();
      //bool done = knik->analyseRouting();
      //while ( !done ) {
      //    knik->unrouteOvSegments();
      //    knik->reroute();
      //    done = knik->analyseRouting();
      //}
        knik->run();
        if ( saveSolution )
          knik->saveSolution();
      } else if ( loadSolution ) {
        knik->analyseRouting();
      } else
        cerr << "[ERROR] Using text mode without -k, -K or -s option is useless." << endl;
    }

    _af->destroy();
  }
  catch ( Error& e ) {
    cerr << e.what() << endl;
    exit ( 1 );
  }
  catch ( ... ) {
    cout << "[ERROR] Abnormal termination: unmanaged exception.\n" << endl;
    exit ( 2 );
  }

  return returnCode;
}


