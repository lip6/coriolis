// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |            G D S I I / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./gds/GdsDriver.cpp"                          |
// +-----------------------------------------------------------------+


#include <ctime>
#include <cmath>
#include <cstdio>
#include <cfenv>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Polygon.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Cell.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Gds.h"
using namespace CRL;


namespace {

  using namespace std;


  bool hasLayout ( const Cell* cell )
  {
    for ( Instance* instance : cell->getInstances() ) {
      if (instance->getPlacementStatus() != Instance::PlacementStatus::UNPLACED)
        return true;
    }
    for ( Net* net : cell->getNets() ) {
      for ( Component* component : net->getComponents() ) {
        if (dynamic_cast<Plug*>(component) == NULL) return true;
      }
    }
    return false;
  } 


  bool  isOnGrid ( Instance* instance )
  {
    bool      error   = false;
    DbU::Unit oneGrid = DbU::fromGrid( 1.0 );
    Point     position = instance->getTransformation().getTranslation();
    if (position.getX() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        Tx %s is not on grid (%s)"
                   , getString(instance).c_str()
                   , getString(instance->getCell()).c_str()
                   , DbU::getValueString(position.getX()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    if (position.getY() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        Ty %s is not on grid (%s)"
                   , getString(instance).c_str()
                   , getString(instance->getCell()).c_str()
                   , DbU::getValueString(position.getY()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    return error;
  }


  bool  isOnGrid ( Component* component, const Box& bb )
  {
    bool      error   = false;
    DbU::Unit oneGrid = DbU::fromGrid( 1.0 );
    if (bb.getXMin() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        X-Min %s is not on grid (%s)"
                   , getString(component).c_str()
                   , getString(component->getCell()).c_str()
                   , DbU::getValueString(bb.getXMin()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    if (bb.getXMax() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        X-Max %s is not on grid (%s)"
                   , getString(component).c_str()
                   , getString(component->getCell()).c_str()
                   , DbU::getValueString(bb.getXMax()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    if (bb.getYMin() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        Y-Min %s is not on grid (%s)"
                   , getString(component).c_str()
                   , getString(component->getCell()).c_str()
                   , DbU::getValueString(bb.getYMin()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    if (bb.getYMax() % oneGrid) {
      error = true;
      cerr << Error( "isOnGrid(): On %s of %s,\n"
                     "        Y-Max %s is not on grid (%s)"
                   , getString(component).c_str()
                   , getString(component->getCell()).c_str()
                   , DbU::getValueString(bb.getYMax()).c_str()
                   , DbU::getValueString(oneGrid).c_str()
                   ) << endl;
    }
    return error;
  }


  bool  isOnGrid ( Component* component, const vector<Point>& points )
  {
    bool      error   = false;
    DbU::Unit oneGrid = DbU::fromGrid( 1.0 );
    for ( size_t i=0 ; i<points.size() ; ++i ) {
      if (points[i].getX() % oneGrid) {
        error = true;
        cerr << Error( "isOnGrid(): On %s of %s,\n"
                       "        Point [%d] X %s is not on grid (%s)"
                     , getString(component).c_str()
                     , getString(component->getCell()).c_str()
                     , i
                     , DbU::getValueString(points[i].getX()).c_str()
                     , DbU::getValueString(oneGrid).c_str()
                     ) << endl;
      }
      if (points[i].getY() % oneGrid) {
        error = true;
        cerr << Error( "isOnGrid(): On %s of %s,\n"
                       "        Point [%d] Y %s is not on grid (%s)"
                     , getString(component).c_str()
                     , getString(component->getCell()).c_str()
                     , i
                     , DbU::getValueString(points[i].getY()).c_str()
                     , DbU::getValueString(oneGrid).c_str()
                     ) << endl;
      }
    }
    return error;
  }


// -------------------------------------------------------------------
// Class  :  "::DepthOrder".

  class DepthOrder {
    private:
      typedef  map <const Cell*,size_t,Entity::CompareById>  CellMap;
      typedef  pair<const Cell*,size_t>                      CellDepth;

      class CompareDepth {
        public:
          inline bool operator() ( const CellDepth& lhs, const CellDepth& rhs );
      };
    public:
                                      DepthOrder    ( const Cell* top );
      inline const vector<CellDepth>& getCellDepths () const;
    private:
      size_t                          computeDepth  ( CellMap&, const Cell* );
    private:
      vector<CellDepth>  _cellDepths;
  };


  inline bool DepthOrder::CompareDepth::operator() ( const CellDepth& lhs, const CellDepth& rhs )
  {
    if (lhs.second < rhs.second) return true;
    if (lhs.second > rhs.second) return false;
    return lhs.first->getId() < rhs.first->getId();
  }


  inline const vector<DepthOrder::CellDepth>& DepthOrder::getCellDepths () const { return _cellDepths; }


  DepthOrder::DepthOrder ( const Cell* top )
  {
    CellMap  cellMap;

    computeDepth( cellMap, top );

    for ( auto element : cellMap ) _cellDepths.push_back( element );
    sort( _cellDepths.begin(), _cellDepths.end(), CompareDepth() );
  }


  size_t  DepthOrder::computeDepth ( CellMap& cellMap, const Cell* cell )
  {
    auto ielement = cellMap.find( cell );
    if (ielement != cellMap.end()) return (*ielement).second;

    size_t depth = 0;
    for ( const Instance* instance : cell->getInstances() ) {
      depth = std::max( depth, computeDepth(cellMap,instance->getMasterCell()) + 1 );
    }
    cellMap.insert( make_pair(cell,depth) );

    return depth;
  }


// -------------------------------------------------------------------
// Class  :  "::GdsRecord".

  class GdsRecord {
    public:
    // Data Types.
      static const uint16_t  NoData          = 0x0000;
      static const uint16_t  BitArray        = 0x0001;
      static const uint16_t  TwoByteInteger  = 0x0002;  // Signed, 16 bits.
      static const uint16_t  FourByteInteger = 0x0003;  // Signed, 32 bits.
    //static const uint16_t  FourByteReal    = 0x0004;  // Unused.
      static const uint16_t  EightByteReal   = 0x0005;
      static const uint16_t  String          = 0x0006;
    // Record Types.
      static const uint16_t  HEADER          = 0x0000 | TwoByteInteger;
      static const uint16_t  BGNLIB          = 0x0100 | TwoByteInteger;
      static const uint16_t  LIBNAME         = 0x0200 | String;
      static const uint16_t  UNITS           = 0x0300 | EightByteReal;
      static const uint16_t  ENDLIB          = 0x0400 | NoData;
      static const uint16_t  BGNSTR          = 0x0500 | TwoByteInteger;
      static const uint16_t  STRNAME         = 0x0600 | String;
      static const uint16_t  ENDSTR          = 0x0700 | NoData;
      static const uint16_t  BOUNDARY        = 0x0800 | NoData;
    //static const uint16_t  PATH            = 0x0900 | NoData;
      static const uint16_t  SREF            = 0x0a00 | NoData;
    //static const uint16_t  AREF            = 0x0b00 | NoData;
      static const uint16_t  TEXT            = 0x0c00 | NoData;
      static const uint16_t  LAYER           = 0x0d00 | TwoByteInteger;
      static const uint16_t  DATATYPE        = 0x0e00 | TwoByteInteger;
    //static const uint16_t  WIDTH           = 0x0f00 | FourByteInteger;
      static const uint16_t  XY              = 0x1000 | FourByteInteger;
      static const uint16_t  ENDEL           = 0x1100 | NoData;
      static const uint16_t  SNAME           = 0x1200 | String;
    //static const uint16_t  COLROW          = 0x1300 | TwoByteInteger;
    //static const uint16_t  TEXTNODE        = 0x1400 | NoData;  // Unused.
    //static const uint16_t  NODE            = 0x1500 | NoData;
      static const uint16_t  TEXTTYPE        = 0x1600 | TwoByteInteger;
      static const uint16_t  PRESENTATION    = 0x1700 | BitArray;
    //static const uint16_t  SPACING         = 0x1800 | NoData;  // Discontinued.
      static const uint16_t  STRING          = 0x1900 | String;
      static const uint16_t  STRANS          = 0x1a00 | BitArray;
    //static const uint16_t  MAG             = 0x1b00 | EightByteReal;
      static const uint16_t  ANGLE           = 0x1c00 | EightByteReal;
    //static const uint16_t  REFLIBS         = 0x1f00 | String;
    //static const uint16_t  FONTS           = 0x2000 | String;
    //static const uint16_t  PATHTYPE        = 0x2100 | TwoByteInteger;
    //static const uint16_t  GENERATIONS     = 0x2200 | TwoByteInteger;
    //static const uint16_t  ATTRTABLE       = 0x2300 | String;
    //static const uint16_t  STYPTABLE       = 0x2400 | String;          // Unreleased.
    //static const uint16_t  STRTYPE         = 0x2500 | TwoByteInteger;  // Unreleased.
    //static const uint16_t  ELFLAGS         = 0x2600 | BitArray;
    //static const uint16_t  ELKEY           = 0x2700 | FourByteInteger; // Unreleased.
    //static const uint16_t  LINKTYPE        = 0x2800 | TwoByteInteger;  // Unreleased.
    //static const uint16_t  LINKKEYS        = 0x2900 | FourByteInteger; // Unreleased.
    //static const uint16_t  NODETYPE        = 0x2a00 | TwoByteInteger;
      static const uint16_t  PROPATTR        = 0x2b00 | TwoByteInteger;
      static const uint16_t  PROPVALUE       = 0x2c00 | String;
    //static const uint16_t  BOX             = 0x2d00 | NoData;
    //static const uint16_t  BOXTYPE         = 0x2e00 | TwoByteInteger;
    //static const uint16_t  PLEX            = 0x2f00 | FourByteInteger;
    //static const uint16_t  BGNEXTN         = 0x3000 | FourByteInteger; // CustomPlus.
    //static const uint16_t  ENDEXTN         = 0x3100 | FourByteInteger; // CustomPlus.
    //static const uint16_t  TAPENUM         = 0x3200 | TwoByteInteger;
    //static const uint16_t  TAPECODE        = 0x3300 | TwoByteInteger;
    //static const uint16_t  STRCLASS        = 0x3400 | BitArray;        // CustomPlus.
    //static const uint16_t  RESERVED        = 0x3500 | FourByteInteger; // Future use.
    //static const uint16_t  FORMAT          = 0x3600 | TwoByteInteger;
    //static const uint16_t  MASK            = 0x3700 | String;          // Filtered format.
    //static const uint16_t  ENDMASKS        = 0x3800 | NoData;          // Filtered format.
    //static const uint16_t  LIBDIRSIZE      = 0x3900 | TwoByteInteger;
    //static const uint16_t  SRFNAME         = 0x3a00 | String;
    //static const uint16_t  LIBSECUR        = 0x3b00 | TwoByteInteger;
    public:
                       GdsRecord  ( uint16_t type );
                       GdsRecord  ( uint16_t type, int16_t );
                       GdsRecord  ( uint16_t type, int32_t );
                       GdsRecord  ( uint16_t type, string );
      inline uint16_t  getType    () const;
             void      toStream   ( ostream& ) const;
             void      push       ( uint16_t );
             void      push       ( int16_t );
             void      push       ( int32_t );
             void      push       ( string );
             void      push       ( double );
    private:
      vector<char>  _bytes;
  };


  inline uint16_t  GdsRecord::getType () const { return (_bytes[0]<<8) + _bytes[1]; }


  GdsRecord::GdsRecord ( uint16_t type )
    : _bytes()
  {
    push( type );
  }


  GdsRecord::GdsRecord ( uint16_t type, int16_t value )
    : _bytes()
  {
    push( type );
    push( value );
  }


  GdsRecord::GdsRecord ( uint16_t type, int32_t value )
    : _bytes()
  {
    push( type );
    push( value );
  }


  GdsRecord::GdsRecord ( uint16_t type, string s )
    : _bytes()
  {
    push( type );
    push( s );
  }


  void  GdsRecord::push ( uint16_t i )
  {
    const unsigned char* bytearray = reinterpret_cast<const unsigned char*>( &i );
    _bytes.push_back( bytearray[1] );
    _bytes.push_back( bytearray[0] );
  }


  void  GdsRecord::push (  int16_t i )
  {
    const unsigned char* bytearray = reinterpret_cast<const unsigned char*>( &i );
    _bytes.push_back( bytearray[1] );
    _bytes.push_back( bytearray[0] );
  }


  void  GdsRecord::push ( int32_t i )
  {
    const unsigned char* bytearray = reinterpret_cast<const unsigned char*>( &i );
    _bytes.push_back( bytearray[3] );
    _bytes.push_back( bytearray[2] );
    _bytes.push_back( bytearray[1] );
    _bytes.push_back( bytearray[0] );
  }


  void  GdsRecord::push ( string s )
  {
    for ( size_t i=0 ; i<s.size() ; ++i ) _bytes.push_back( s[i]    );
    if  (s.size()%2)                      _bytes.push_back( (char)0 );
  }


  void  GdsRecord::push ( double d )
  {
    size_t end = _bytes.size();
    _bytes.insert( _bytes.end(), 8, (char)0 );
    
    _bytes[end] = 0;
    if (d < 0) {
      _bytes[end] = char (0x80);
      d = -d;
    }

  //  compute the next power of 16 that that value will fit in
    int e = 0;
    if (d < 1e-77 /*~16^-64*/) {
      d = 0;
    } else {
      double log16 = log(d) / log(16.0);
      e = int( ceil(log(d) / log(16.0)) );
      if (e == log16) ++e;
    }

    d /= pow( 16.0, e-14 );

  //tl_assert (e >= -64 && e < 64);
    _bytes[end] |= ((e + 64) & 0x7f);

    uint64_t m = uint64_t(d + 0.5);
    for ( int i=7 ; i>0 ; --i ) {
      _bytes[end+i] = (m & 0xff);
      m >>= 8;
    }
  }


  void  GdsRecord::toStream ( ostream& stream ) const
  {
    // static size_t count = 0;
    
    uint16_t length = (uint16_t)( _bytes.size()+2 );
    const unsigned char* bytearray = reinterpret_cast<const unsigned char*>( &length );

    // char buffer[512];
    // sprintf( buffer, "0x%02x", bytearray[1] );
    // cerr << setw(6) << hex << count++ << " | " << buffer << endl; 
    // sprintf( buffer, "0x%02x", bytearray[0] );
    // cerr << setw(6) << hex << count++ << " | " << buffer << endl; 

  // Little endian (x86).
    stream.put( bytearray[1] );
    stream.put( bytearray[0] );

    for ( unsigned char byte : _bytes ) {
      // uint32_t b = byte;
      // sprintf( buffer, "0x%02x", b );
      // cerr << setw(6) << hex << count++ << " | " << buffer << endl; 
      stream.put( byte );
    }
  }


  ostream& operator<< ( ostream& o, const GdsRecord& r ) { r .toStream( o ); return o; }
//ostream& operator<< ( ostream& o, const GdsRecord* r ) { r->toStream( o ); return o; }


// -------------------------------------------------------------------
// Class  :  "::GdsStream".

  class GdsStream {
    public:
      static const  GdsRecord  BOUNDARY;
      static const  GdsRecord  ENDLIB;
      static const  GdsRecord  ENDEL;
      static const  GdsRecord  ENDSTR;
      static const  GdsRecord  SREF;
      static const  GdsRecord  TEXT;
    public:
                               GdsStream    ( string filename );
                              ~GdsStream    ();
             inline int32_t    toGdsDbu     ( DbU::Unit ) const;
      static inline GdsRecord  PROPATTR     ( int16_t );
      static inline GdsRecord  DATATYPE     ( int16_t );
      static inline GdsRecord  TEXTTYPE     ( int16_t );
      static inline GdsRecord  LAYER        ( int16_t );
      static inline GdsRecord  PRESENTATION ( int16_t );
      static inline GdsRecord  PROPVALUE    ( string );
      static inline GdsRecord  STRNAME      ( string );
      static inline GdsRecord  STRNAME      ( const Name& );
      static inline GdsRecord  LIBNAME      ( string );
      static inline GdsRecord  SNAME        ( string );
      static inline GdsRecord  SNAME        ( const Name& );
      static inline GdsRecord  STRING       ( const Name& );
      static inline GdsRecord  STRING       ( const string );
                    GdsStream& operator<<   ( const GdsRecord& );
                    GdsStream& operator<<   ( const Box& );
                    GdsStream& operator<<   ( const Points );
                    GdsStream& operator<<   ( const Point& point );
                    GdsStream& operator<<   ( const vector<Point>& points );
                    GdsStream& operator<<   ( const Cell* );
                    GdsStream& operator<<   ( const Transformation& );
    private:
      ofstream  _ostream;
      double    _dbuPerUu;
      double    _metricDbU;
  };

  
  const  GdsRecord  GdsStream::BOUNDARY = GdsRecord(GdsRecord::BOUNDARY);
  const  GdsRecord  GdsStream::ENDLIB   = GdsRecord(GdsRecord::ENDLIB);
  const  GdsRecord  GdsStream::ENDEL    = GdsRecord(GdsRecord::ENDEL);
  const  GdsRecord  GdsStream::ENDSTR   = GdsRecord(GdsRecord::ENDSTR);
  const  GdsRecord  GdsStream::SREF     = GdsRecord(GdsRecord::SREF);
  const  GdsRecord  GdsStream::TEXT     = GdsRecord(GdsRecord::TEXT);

  inline GdsRecord  GdsStream::PROPATTR     ( int16_t v )      { return GdsRecord(GdsRecord::PROPATTR,v); }
  inline GdsRecord  GdsStream::DATATYPE     ( int16_t v )      { return GdsRecord(GdsRecord::DATATYPE,v); }
  inline GdsRecord  GdsStream::TEXTTYPE     ( int16_t v )      { return GdsRecord(GdsRecord::TEXTTYPE,v); }
  inline GdsRecord  GdsStream::LAYER        ( int16_t v )      { return GdsRecord(GdsRecord::LAYER,v); }
  inline GdsRecord  GdsStream::PRESENTATION ( int16_t v )      { return GdsRecord(GdsRecord::PRESENTATION,v); }
  inline GdsRecord  GdsStream::PROPVALUE    ( string s )       { return GdsRecord(GdsRecord::PROPVALUE,s); }
  inline GdsRecord  GdsStream::STRNAME      ( string s )       { return GdsRecord(GdsRecord::STRNAME,s); }
  inline GdsRecord  GdsStream::STRNAME      ( const Name& n )  { return GdsRecord(GdsRecord::STRNAME,getString(n)); }
  inline GdsRecord  GdsStream::LIBNAME      ( string s )       { return GdsRecord(GdsRecord::LIBNAME,s); }
  inline GdsRecord  GdsStream::SNAME        ( string s )       { return GdsRecord(GdsRecord::SNAME,s); }
  inline GdsRecord  GdsStream::SNAME        ( const Name& n )  { return GdsRecord(GdsRecord::SNAME,getString(n)); }
  inline GdsRecord  GdsStream::STRING       ( const Name& n )  { return GdsRecord(GdsRecord::STRING,getString(n)); }
  inline GdsRecord  GdsStream::STRING       ( const string s ) { return GdsRecord(GdsRecord::STRING,s); }
  inline int32_t    GdsStream::toGdsDbu     ( DbU::Unit v )   const
  { return uint32_t( std::lrint( DbU::toPhysical( v, DbU::UnitPower::Unity ) / _metricDbU )); }


  GdsStream::GdsStream ( string filename )
    : _ostream  ()
    , _dbuPerUu (Cfg::getParamDouble("gdsDriver.dbuPerUu" ,0.001)->asDouble())  // 1000
    , _metricDbU(Cfg::getParamDouble("gdsDriver.metricDbu",10e-9)->asDouble())  // 1um.
  {
    std::fesetround( FE_TONEAREST );
    _ostream.open( filename, ios_base::out|ios_base::binary );

    GdsRecord record ( GdsRecord::HEADER );
    record.push( (uint16_t)600 );
    _ostream << record;

    time_t t   = time( 0 );
    tm*    now = localtime( &t );

    record = GdsRecord( GdsRecord::BGNLIB );
  // Last modification time.
    record.push( (uint16_t)now->tm_year+1900 );
    record.push( (uint16_t)now->tm_mon   );
    record.push( (uint16_t)now->tm_mday  );
    record.push( (uint16_t)now->tm_hour  );
    record.push( (uint16_t)now->tm_sec   );
  // Last access time.
    record.push( (uint16_t)now->tm_year+1900 );
    record.push( (uint16_t)now->tm_mon   );
    record.push( (uint16_t)now->tm_mday  );
    record.push( (uint16_t)now->tm_hour  );
    record.push( (uint16_t)now->tm_sec   );
    _ostream << record;

    _ostream << LIBNAME( "LIB" );

  // Generate a GDSII which coordinates are relatives to the um.
  // Bug correction courtesy of M. Koefferlein (KLayout).
  //double gridPerUu = DbU::getPhysicalsPerGrid() / 1e-6;

    record = GdsRecord( GdsRecord::UNITS );
    record.push( _dbuPerUu );
    record.push( _metricDbU );
  //record.push( gridPerUu );
  //record.push( DbU::getPhysicalsPerGrid() );
    _ostream << record;
  }

  
  GdsStream::~GdsStream ()
  {
    _ostream << ENDLIB;
    _ostream.close();
  }


  GdsStream& GdsStream::operator<< ( const GdsRecord& record )
  { _ostream << record; return *this; }


#if 0
  GdsStream& GdsStream::operator<< ( const BasicLayer* layer )
  {
    cdebug_log(101,0) << "LAYER" << endl;
    GdsRecord record ( GdsRecord::LAYER );
    record.push( (int16_t)layer->getGds2Layer() );
    _ostream << record;

    cdebug_log(101,0) << "DATATYPE" << endl;
    record = GdsRecord( GdsRecord::DATATYPE );
    record.push( (int16_t)layer->getGds2Datatype() );
    _ostream << record;

    return *this;
  }
#endif


  GdsStream& GdsStream::operator<< ( const Transformation& transf )
  {
    const uint16_t f_reflexion = (1 << 15);
    
    GdsRecord record ( GdsRecord::STRANS );
    uint16_t flags = 0;
    double   angle = 0.0;

    if (transf.getOrientation() != Transformation::Orientation::ID) {
      switch ( transf.getOrientation() ) {
        case Transformation::Orientation::ID:
          break;
        case Transformation::Orientation::R1:
          angle  = 90.0;
          break;
        case Transformation::Orientation::R2:
          angle  = 180.0;
          break;
        case Transformation::Orientation::R3:
          angle  = 270.0;
          break;
        case Transformation::Orientation::MX:
          flags |= f_reflexion;
          angle  = -180.0;
          break;
        case Transformation::Orientation::XR:
          flags |= f_reflexion;
          angle  = -90.0;
          break;
        case Transformation::Orientation::MY:
          flags |= f_reflexion;
          break;
        case Transformation::Orientation::YR:
          flags |= f_reflexion;
          angle  = 90.0;
          break;
      }
    }

    record.push( flags );
    _ostream << record;

    if (angle != 0.0) {
      record = GdsRecord( GdsRecord::ANGLE );
      record.push( angle );
      _ostream << record;
    }

    record = GdsRecord( GdsRecord::XY );
    record.push( (int32_t)toGdsDbu(transf.getTx()) );
    record.push( (int32_t)toGdsDbu(transf.getTy()) );

    _ostream << record;
    return *this;
  }


  GdsStream& GdsStream::operator<< ( const Box& box )
  {
    GdsRecord record ( GdsRecord::XY );
    for ( size_t i=0 ; i<5 ; ++i ) {
      Point p;
      switch ( i%4 ) {
        case 0: p = Point( box.getXMin(), box.getYMin() ); break;
        case 1: p = Point( box.getXMin(), box.getYMax() ); break;
        case 2: p = Point( box.getXMax(), box.getYMax() ); break;
        case 3: p = Point( box.getXMax(), box.getYMin() ); break;
      }
      record.push( (int32_t)toGdsDbu(p.getX()) );
      record.push( (int32_t)toGdsDbu(p.getY()) );
    }
    _ostream << record;
    return *this;
  }


  GdsStream& GdsStream::operator<< ( Points points )
  {
    GdsRecord record ( GdsRecord::XY );
    Point first = points.getFirst();
    for ( Point p : points ) {
      record.push( (int32_t)toGdsDbu(p.getX()) );
      record.push( (int32_t)toGdsDbu(p.getY()) );
    }
    record.push( (int32_t)toGdsDbu(first.getX()) );
    record.push( (int32_t)toGdsDbu(first.getY()) );
    _ostream << record;
    return *this;
  }


  GdsStream& GdsStream::operator<< ( const Point& point )
  {
    GdsRecord record ( GdsRecord::XY );
    record.push( (int32_t)toGdsDbu(point.getX()) );
    record.push( (int32_t)toGdsDbu(point.getY()) );
    _ostream << record;
    return *this;
  }


  GdsStream& GdsStream::operator<< ( const vector<Point>& points )
  {
    GdsRecord record ( GdsRecord::XY );
    for ( Point p : points ) {
      record.push( (int32_t)toGdsDbu(p.getX()) );
      record.push( (int32_t)toGdsDbu(p.getY()) );
    }
    record.push( (int32_t)toGdsDbu(points[0].getX()) );
    record.push( (int32_t)toGdsDbu(points[0].getY()) );
    _ostream << record;
    return *this;
  }


  GdsStream& GdsStream::operator<< ( const Cell* cell )
  {
  // Temporay patch for "amsOTA".
    if (cell->getName() == "control_r") return *this;
    if (not hasLayout(cell)) return *this;

    Technology* tech = DataBase::getDB()->getTechnology();
    
    time_t t   = time( 0 );
    tm*    now = localtime( &t );

    GdsRecord record ( GdsRecord::BGNSTR );
  // Last modification time.
    record.push( (uint16_t)now->tm_year+1900 );
    record.push( (uint16_t)now->tm_mon );
    record.push( (uint16_t)now->tm_mday);
    record.push( (uint16_t)now->tm_hour);
    record.push( (uint16_t)now->tm_sec );
  // Last access time.
    record.push( (uint16_t)now->tm_year+1900 );
    record.push( (uint16_t)now->tm_mon );
    record.push( (uint16_t)now->tm_mday);
    record.push( (uint16_t)now->tm_hour);
    record.push( (uint16_t)now->tm_sec );
    _ostream << record;

    _ostream << STRNAME(cell->getName());

    for ( Instance* instance : cell->getInstances() ) {
      if (instance->getMasterCell()->getName() == "control_r") continue;
      if (not hasLayout(instance->getMasterCell())) continue;

      if (instance->getPlacementStatus() == Instance::PlacementStatus::UNPLACED) continue;

      (*this) << SREF;
      (*this) << SNAME( instance->getMasterCell()->getName() );
      (*this) << instance->getTransformation();
      (*this) << ENDEL;
      isOnGrid( instance );
    }

    for ( Net* net : cell->getNets() ) {
      for ( Component* component : net->getComponents() ) {
        Polygon* polygon  = dynamic_cast<Polygon*>(component);
        if (polygon) {
          vector< vector<Point> > subpolygons;
          polygon->getSubPolygons( subpolygons );

          for ( const vector<Point>& subpolygon : subpolygons ) {
            for ( const BasicLayer* layer : component->getLayer()->getBasicLayers() ) {
              (*this) << BOUNDARY;
              (*this) << LAYER(layer->getGds2Layer());
              (*this) << DATATYPE(layer->getGds2Datatype());
              (*this) << subpolygon;
              (*this) << ENDEL;
            }
          }
        } else {
          Rectilinear* rectilinear  = dynamic_cast<Rectilinear*>(component);
          if (rectilinear) {
            for ( const BasicLayer* layer : component->getLayer()->getBasicLayers() ) {
              (*this) << BOUNDARY;
              (*this) << LAYER(layer->getGds2Layer());
              (*this) << DATATYPE(layer->getGds2Datatype());
              (*this) << rectilinear->getPoints();
              (*this) << ENDEL;
              isOnGrid( component, rectilinear->getPoints() );
            }
          } else {
            Diagonal* diagonal = dynamic_cast<Diagonal*>(component);
            if (diagonal) {
              for ( const BasicLayer* layer : component->getLayer()->getBasicLayers() ) {
                (*this) << BOUNDARY;
                (*this) << LAYER(layer->getGds2Layer());
                (*this) << DATATYPE(layer->getGds2Datatype());
                (*this) << diagonal->getContour();
                (*this) << ENDEL;
              }
            } else if (  dynamic_cast<Horizontal*>(component)
                      or dynamic_cast<Vertical  *>(component)
                      or dynamic_cast<Contact   *>(component)
                      or dynamic_cast<Pad       *>(component)) {
              for ( const BasicLayer* layer : component->getLayer()->getBasicLayers() ) {
                Box bb = component->getBoundingBox(layer);
                if ((bb.getWidth() == 0) or (bb.getHeight() == 0))
                  continue;
                const BasicLayer* exportLayer = layer;
                if (NetExternalComponents::isExternal(component)) {
                  string layerName = getString( layer->getName() );
                  if (layerName.substr(layerName.size()-4) != ".pin") {
                    exportLayer = tech->getBasicLayer( layerName+".pin" );
                    if (not exportLayer) exportLayer = layer;
                  }
                }
                (*this) << BOUNDARY;
                (*this) << LAYER(exportLayer->getGds2Layer());
                (*this) << DATATYPE(exportLayer->getGds2Datatype());
                (*this) << bb;
                (*this) << ENDEL;
                isOnGrid( component, bb );

                if (NetExternalComponents::isExternal(component)) {
                  string name = getString( component->getNet()->getName() );
                  if (name.size() > 511) {
                    cerr << getString(
                              Warning( "GdsStream::operator<<(): Truncate Net name to 511 first characters,\n"
                                       "           on \"%s\"."
                                     , name.c_str() )) << endl;
                    name.erase( 511 );
                  }
                // PRESENTATION: 0b000101 means font:00, vpres:01 (center), hpres:01 (center)
                  cdebug_log(101,0) << "TEXT" << endl;
                  (*this) << TEXT;
                  (*this) << LAYER(exportLayer->getGds2Layer());
                  cdebug_log(101,0) << "TEXTTYPE" << endl;
                  (*this) << TEXTTYPE( 0 );
                  cdebug_log(101,0) << "TEXTYPE end record" << endl;
                  (*this) << PRESENTATION( 5 );
                  (*this) << bb.getCenter();
                  (*this) << STRING( name );
                  (*this) << ENDEL;
                  cdebug_log(101,0) << "TEXT ENDEL" << endl;
                }
              }
            }
          }
        }
      }
    }

    (*this) << ENDSTR;

    return *this;
  }


}  // Anonymous namespace.


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::Gds".

  bool  Gds::save ( Cell* cell )
  {
    string cellFile = getString(cell->getName()) + ".gds";

    GdsStream gstream ( cellFile );

    DepthOrder cellOrder ( cell );
    for ( auto element : cellOrder.getCellDepths() ) {
      gstream << element.first;
    }

    return true;
  }


}  // CRL namespace.
