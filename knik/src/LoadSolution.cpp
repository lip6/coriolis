
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |               K n i k - Global Router                           |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               Damien.Dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LoadSolution.cpp"                       |
// +-----------------------------------------------------------------+


#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Layer.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Contact.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/Measures.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/CellGauge.h"
#include "knik/Configuration.h"
#include "knik/Graph.h"
#include "knik/KnikEngine.h"


namespace {

  using std::string;
  using std::cerr;
  using std::endl;
  using std::vector;
  using std::map;
  using std::make_pair;

  using CRL::IoFile;
  using CRL::AllianceFramework;

  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DbU;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::Name;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::NetRoutingExtension;
  using Hurricane::NetRoutingState;
  using Hurricane::Contact;
  using Hurricane::Segment;
  using Hurricane::Vertical;
  using Hurricane::Horizontal;
  using Hurricane::RoutingPad;
  using Hurricane::Cell;
  using Hurricane::UpdateSession;

  using Knik::Configuration;
  using Knik::KnikEngine;
  using Knik::Vertex;


  const char* LoadError      = "KnikEngine::LoadSolution(): %s.\n        (file: %s, at line: %d)";
  const char* NotManhattan   =
    "KnikEngine::LoadSolution(): Encountered a Segment neither Horizontal nor Vertical.\n"
    "        ([%s %s] [%s %s], file: %s, at line: %d)";
  const char* BadLong        =
    "KnikEngine::LoadSolution(): Incomplete string to integer conversion for \"%s\" (%ld).\n"
    "        (file: %s, at line: %d)";


  struct PointCompare {
      bool operator() ( const Point& lhs, const Point& rhs ) const;
  };


  bool  PointCompare::operator() ( const Point& lhs, const Point& rhs ) const
  {
    if ( lhs.getX() < rhs.getX() ) return true;
    if ( lhs.getX() > rhs.getX() ) return false;

    if ( lhs.getY() < rhs.getY() ) return true;
    return false;
  }


  class GContactMap {
    public:
                   GContactMap       ( KnikEngine*  knik
                                     , const Layer* gmetalh
                                     , const Layer* gmetalv
                                     );
      void         setNet            ( Net* );
      const Layer* getLayer          ( unsigned int z );
      Contact*     find              ( DbU::Unit x, DbU::Unit y, unsigned int z );
      Contact*     findVertexContact ( const Box& );
      size_t       size              () const;
      void         clear             ();
    private:                         
      void         _mergeLayer       ( Contact*, unsigned int );
    private:
      KnikEngine*                       _knik;
      Technology*                       _technology;
      Net*                              _net;
      map<Point,Contact*,PointCompare>  _contactMap;
      map<Vertex*,Contact*>             _vertexMap;
      const Layer*                      _gmetalh;
      const Layer*                      _gmetalv;
  };


  GContactMap::GContactMap ( KnikEngine*  knik
                           , const Layer* gmetalh
                           , const Layer* gmetalv
                           )
    : _knik      (knik)
    , _technology(DataBase::getDB()->getTechnology())
    , _net       (NULL)
    , _contactMap()
    , _vertexMap ()
    , _gmetalh   (gmetalh)
    , _gmetalv   (gmetalv)
  { }


  void  GContactMap::setNet ( Net* net )
  { _net = net; }


  const Layer* GContactMap::getLayer ( unsigned int z )
  { return (z%2) ? _gmetalh : _gmetalv; }


  Contact* GContactMap::find ( DbU::Unit x, DbU::Unit y, unsigned int z )
  {
    map<Point,Contact*,PointCompare>::iterator icontact = _contactMap.find ( Point(x,y) );
    Contact* contact;

    if ( icontact == _contactMap.end() ) {
      contact = Contact::create ( _net, getLayer(z), x, y, DbU::lambda(3.0), DbU::lambda(3.0) );
      Vertex* vertex = _knik->getVertex ( x, y );

      _contactMap.insert ( make_pair(Point(x,y),contact) );
      _vertexMap .insert ( make_pair(vertex    ,contact) );
    } else {
      contact = icontact->second;
    }
    _mergeLayer ( contact, z );
    return contact;
  }


  Contact* GContactMap::findVertexContact ( const Box& boundingBox )
  {
    Point reference = boundingBox.getCenter();

    Vertex* vertex = _knik->getVertex ( reference );
    if ( !vertex ) return NULL;

    map<Vertex*,Contact*>::iterator icontact = _vertexMap.find ( vertex );
    if ( icontact == _vertexMap.end() ) return NULL;

    return icontact->second;
  }


  size_t  GContactMap::size () const
  { return _contactMap.size(); }


  void  GContactMap::clear ()
  {
    _contactMap.clear();
    _vertexMap.clear();
  }


  void  GContactMap::_mergeLayer ( Contact* contact, unsigned int z )
  {
    const Layer* mergeLayer   = getLayer ( z );
    const Layer* contactLayer = contact->getLayer();

    if ( contactLayer->contains(mergeLayer) ) return;

    contactLayer = _technology->getViaBetween ( contactLayer, mergeLayer );
    if ( contactLayer )
      contact->setLayer ( contactLayer );
  }


  class SolutionParser {
    public:
                     SolutionParser      ( KnikEngine*, const string& loadFileName );
      void           load                ();
    private:
      long           _getLong            ( const char* );
      vector<char*>  _splitSegmentString ( char* );
      vector<char*>  _splitString        ( char* );
    private:
      enum Constants { RawLineSize = 4096 };
    private:
      char         _rawLine [ RawLineSize ];
      size_t       _lineNumber;
      string       _fileName;
      KnikEngine*  _knik;
      const Layer* _gmetalh;
      const Layer* _gmetalv;
      const Layer* _gcontact;
  };


  SolutionParser::SolutionParser ( KnikEngine* knik, const string& fileName )
    : _lineNumber(0)
    , _fileName  (fileName)
    , _knik      (knik)
    , _gmetalh   (Configuration::getGMetalH())
    , _gmetalv   (Configuration::getGMetalV())
    , _gcontact  (Configuration::getGContact())
  { }


  long  SolutionParser::_getLong ( const char* s )
  {
    char* end;
    long value = strtol ( s, &end, 10 );
    if ( *end != '\0' )
      cerr << Error(BadLong,s,value,_fileName.c_str(),_lineNumber) << endl;
    return value;
  }


  vector<char*>  SolutionParser::_splitString ( char* s )
  {
    vector<char*> fields;

    fields.push_back ( s );
    while ( *s != '\0' ) {
      unsigned i = 0;
      if ( (*s == ' ') || (*s == '\t') ) {
        i++;
        *s = '\0';
        while ( (*(s+i) == ' ') || (*(s+i) == '\t') ) i++;
        fields.push_back ( s+i );
        s += i;
      } else 
        s++;
    }

    return fields;
  }


  vector<char*>  SolutionParser::_splitSegmentString ( char* s )
  {
    vector<char*> fields;

    while ( *s != '\0' ) {
      switch ( *s ) {
        case '(':
        case ',': *s = '\0'; ++s; break;
        case ')':
          *s = '\0'; ++s;
          if ( *s == '-' ) { *s = '\0'; ++s; }
          break;
        default:
          fields.push_back ( s );
          while ( *s && ( (*s != ')') && (*s != ',') ) ) ++s;
      }
    }

    return fields;
  }


  void SolutionParser::load ()
  {
    UpdateSession::open();
    
    try {
      cmess1 << "  o  Loading solution: \"" << _fileName << "\"." << endl;


      DbU::Unit   sliceHeight = AllianceFramework::get()->getCellGauge()->getSliceHeight();
      CRL::IoFile fileStream ( _fileName );
      fileStream.open( "r" );
      if (not fileStream.isOpen())
        throw Error( "Can't open/read file: %s.", _fileName.c_str() );

      unsigned int missingGlobalRouting = 0;
      unsigned int contactCount         = 0;
      unsigned int segmentCount         = 0;
      GContactMap  contactMap           ( _knik, _gmetalh, _gmetalv );

      while ( not fileStream.eof() ) {
        fileStream.readLine( _rawLine, RawLineSize );
        _lineNumber++;

        if (_rawLine[0] == '\0') break;
        if (_rawLine[0] == '\n') continue;

        vector<char*> fields = _splitString( _rawLine );
        if ( fields.size() != 3 )
          throw Error( LoadError, "Malformed Net Line", _fileName.c_str(), _lineNumber );
        else {
          Name          netName      = Name    ( fields[0] );
          unsigned int  nbPins       = _getLong( fields[2] );
          unsigned int  nbSegments   = 0;
          unsigned int  nbRoutingPad = 0;
          Net*          net          = _knik->getCell()->getNet( netName );

          if (not net) {
            string message = "Cell has no Net: ";
            message += getString(netName);
            throw Error( LoadError, message.c_str(), _fileName.c_str(), _lineNumber );
          }

          contactMap.setNet( net );

          for ( unsigned i = 0 ; i < nbPins ; i++ ) {
            fileStream.readLine( _rawLine, RawLineSize );
            _lineNumber++;
            fields = _splitSegmentString( _rawLine );
            if (fields.size() != 6)
              throw Error( LoadError, "Malformed Net Line", _fileName.c_str(), _lineNumber );
            else {
              DbU::Unit     xSource = DbU::lambda( _getLong(fields[0]) );
              DbU::Unit     ySource = DbU::lambda( _getLong(fields[1]) );
              unsigned int  zSource = (unsigned) ( _getLong(fields[2]) );
              DbU::Unit     xTarget = DbU::lambda( _getLong(fields[3]) );
              DbU::Unit     yTarget = DbU::lambda( _getLong(fields[4]) );
              unsigned int  zTarget = (unsigned) ( _getLong(fields[5]) );

              Contact* source  = contactMap.find( xSource, ySource, zSource );
              Contact* target  = contactMap.find( xTarget, yTarget, zTarget );
              Segment* segment = NULL;

              unsigned int type = ((ySource == yTarget)?1:0) + ((xSource == xTarget)?2:0);
              switch ( type ) {
                case 0:
                  throw Error( NotManhattan
                             , DbU::getValueString(xSource).c_str()
                             , DbU::getValueString(ySource).c_str()
                             , DbU::getValueString(xTarget).c_str()
                             , DbU::getValueString(yTarget).c_str()
                             , _fileName.c_str()
                             , _lineNumber
                             );
                case 1:
                  segment = Horizontal::create( source, target, _gmetalh, ySource, DbU::lambda(2.0) );
                  ++nbSegments;
                  break;
                case 2:
                  segment = Vertical::create( source, target, _gmetalv, xSource, DbU::lambda(2.0) );
                  ++nbSegments;
                  break;
                case 3:
                  break;
              }

              if (segment) _knik->insertSegment( segment );
            }
          }
          fileStream.readLine( _rawLine, RawLineSize );
          _lineNumber++;
          if (_rawLine[0] != '!') 
            throw Error( "KnikEngine::loadSolution(): Tu t'es vu quand t'as bu! (%ld)."
                       , getString(_lineNumber).c_str());

          Box         rpBox;
          RoutingPad* previousRp  = NULL;
          if (NetRoutingExtension::isAutomaticGlobalRoute(net)) {
            forEach ( RoutingPad*, rp, net->getRoutingPads() ) {
              rpBox.merge( rp->getBoundingBox() );
              Contact* gcontact = contactMap.findVertexContact( rp->getBoundingBox() );
              if (gcontact) {
                rp->getBodyHook()->attach( gcontact->getBodyHook() );
              } else {
                if (previousRp)
                  rp->getBodyHook()->attach( previousRp->getBodyHook() );
              }
              previousRp = *rp;
              ++nbRoutingPad;
            //cerr << rp->_getString() << " should be attached to " << gcontact << endl;
            }
          }

          if (   (nbRoutingPad > 1)
             and (not contactMap.size())
             and (  (rpBox.getHeight() > sliceHeight)
                 or (rpBox.getWidth () > sliceHeight) ) ) {
            ++missingGlobalRouting;
            cerr << Warning( "Net <%s> is missing global routing."
                           , getString(net->getName()).c_str() ) << endl;
          }

          segmentCount += nbSegments;
          contactCount += contactMap.size();
          contactMap.clear();
        }
      }

      fileStream.close();

      if (missingGlobalRouting)
        throw Error( "At least %d nets are missing global routing. Maybe a corrupted <.kgr> file?"
                   , missingGlobalRouting
                   );
    }
    catch ( Error& e ) {
      UpdateSession::close ();
    //cerr << e << endl;
      throw;
    }
    UpdateSession::close ();
  }


} // End of anonymous namespace.


namespace Knik {

  using CRL::addMeasure;


  void  KnikEngine::loadSolution ( const string& fileName )
  {
    string loadFileName = fileName;
    if ( loadFileName.empty() )
      loadFileName = _getSolutionName();

    SolutionParser parser ( this, loadFileName );
    parser.load ();

    addMeasure<double> ( getCell(), "knikT",  0.0, 8 );
    addMeasure<size_t> ( getCell(), "knikS",  0  , 8 );

    computeSymbolicWireLength ();
  }


} // End of Knik namespace.
