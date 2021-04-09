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
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./gds/GdsParser.cpp"                          |
// +-----------------------------------------------------------------+


#include <ctime>
#include <cstdio>
#include <string>
#include <bitset>
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
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Gds.h"
using namespace CRL;


namespace {


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
      static const uint16_t  PATH            = 0x0900 | NoData;
      static const uint16_t  SREF            = 0x0a00 | NoData;
      static const uint16_t  AREF            = 0x0b00 | NoData;
      static const uint16_t  TEXT            = 0x0c00 | NoData;
      static const uint16_t  LAYER           = 0x0d00 | TwoByteInteger;
      static const uint16_t  DATATYPE        = 0x0e00 | TwoByteInteger;
      static const uint16_t  WIDTH           = 0x0f00 | FourByteInteger;
      static const uint16_t  XY              = 0x1000 | FourByteInteger;
      static const uint16_t  ENDEL           = 0x1100 | NoData;
      static const uint16_t  SNAME           = 0x1200 | String;
      static const uint16_t  COLROW          = 0x1300 | TwoByteInteger;
      static const uint16_t  TEXTNODE        = 0x1400 | NoData;  // Unused.
      static const uint16_t  NODE            = 0x1500 | NoData;
      static const uint16_t  TEXTTYPE        = 0x1600 | TwoByteInteger;
      static const uint16_t  PRESENTATION    = 0x1700 | BitArray;
      static const uint16_t  SPACING         = 0x1800 | NoData;  // Discontinued.
      static const uint16_t  STRING          = 0x1900 | String;
      static const uint16_t  STRANS          = 0x1a00 | BitArray;
      static const uint16_t  MAG             = 0x1b00 | EightByteReal;
      static const uint16_t  ANGLE           = 0x1c00 | EightByteReal;
      static const uint16_t  REFLIBS         = 0x1f00 | String;
      static const uint16_t  FONTS           = 0x2000 | String;
      static const uint16_t  PATHTYPE        = 0x2100 | TwoByteInteger;
      static const uint16_t  GENERATIONS     = 0x2200 | TwoByteInteger;
      static const uint16_t  ATTRTABLE       = 0x2300 | String;
      static const uint16_t  STYPTABLE       = 0x2400 | String;          // Unreleased.
      static const uint16_t  STRTYPE         = 0x2500 | TwoByteInteger;  // Unreleased.
      static const uint16_t  ELFLAGS         = 0x2600 | BitArray;
      static const uint16_t  ELKEY           = 0x2700 | FourByteInteger; // Unreleased.
      static const uint16_t  LINKTYPE        = 0x2800 | TwoByteInteger;  // Unreleased.
      static const uint16_t  LINKKEYS        = 0x2900 | FourByteInteger; // Unreleased.
      static const uint16_t  NODETYPE        = 0x2a00 | TwoByteInteger;
      static const uint16_t  PROPATTR        = 0x2b00 | TwoByteInteger;
      static const uint16_t  PROPVALUE       = 0x2c00 | String;
      static const uint16_t  BOX             = 0x2d00 | NoData;
      static const uint16_t  BOXTYPE         = 0x2e00 | TwoByteInteger;
      static const uint16_t  PLEX            = 0x2f00 | FourByteInteger;
      static const uint16_t  BGNEXTN         = 0x3000 | FourByteInteger; // CustomPlus.
      static const uint16_t  ENDEXTN         = 0x3100 | FourByteInteger; // CustomPlus.
      static const uint16_t  TAPENUM         = 0x3200 | TwoByteInteger;
      static const uint16_t  TAPECODE        = 0x3300 | TwoByteInteger;
      static const uint16_t  STRCLASS        = 0x3400 | BitArray;        // CustomPlus.
      static const uint16_t  RESERVED        = 0x3500 | FourByteInteger; // Future use.
      static const uint16_t  FORMAT          = 0x3600 | TwoByteInteger;
      static const uint16_t  MASK            = 0x3700 | String;          // Filtered format.
      static const uint16_t  ENDMASKS        = 0x3800 | NoData;          // Filtered format.
      static const uint16_t  LIBDIRSIZE      = 0x3900 | TwoByteInteger;
      static const uint16_t  SRFNAME         = 0x3a00 | String;
      static const uint16_t  LIBSECUR        = 0x3b00 | TwoByteInteger;
    public:
                                     GdsRecord      ();
      inline       bool              isHEADER       () const;   
      inline       bool              isBGNLIB       () const;   
      inline       bool              isLIBNAME      () const;   
      inline       bool              isUNITS        () const;   
      inline       bool              isENDLIB       () const;   
      inline       bool              isBGNSTR       () const;   
      inline       bool              isSTRNAME      () const;   
      inline       bool              isENDSTR       () const;   
      inline       bool              isBOUNDARY     () const;   
      inline       bool              isPATH         () const;   
      inline       bool              isSREF         () const;   
      inline       bool              isAREF         () const;   
      inline       bool              isTEXT         () const;   
      inline       bool              isLAYER        () const;   
      inline       bool              isDATATYPE     () const;   
      inline       bool              isWIDTH        () const;
      inline       bool              isXY           () const;
      inline       bool              isENDEL        () const;   
      inline       bool              isSNAME        () const;   
      inline       bool              isCOLROW       () const;   
      inline       bool              isTEXTNODE     () const;
      inline       bool              isNODE         () const;   
      inline       bool              isTEXTTYPE     () const;   
      inline       bool              isPRESENTATION () const;   
      inline       bool              isSPACING      () const;
      inline       bool              isSTRING       () const;   
      inline       bool              isSTRANS       () const;   
      inline       bool              isMAG          () const;   
      inline       bool              isANGLE        () const;   
      inline       bool              isREFLIBS      () const;   
      inline       bool              isFONTS        () const;   
      inline       bool              isPATHTYPE     () const;   
      inline       bool              isGENERATIONS  () const;   
      inline       bool              isATTRTABLE    () const;   
      inline       bool              isSTYPTABLE    () const;
      inline       bool              isSTRTYPE      () const;
      inline       bool              isELFLAGS      () const;   
      inline       bool              isELKEY        () const;
      inline       bool              isLINKTYPE     () const;
      inline       bool              isLINKKEYS     () const;
      inline       bool              isNODETYPE     () const;   
      inline       bool              isPROPATTR     () const;   
      inline       bool              isPROPVALUE    () const;   
      inline       bool              isBOX          () const;   
      inline       bool              isBOXTYPE      () const;   
      inline       bool              isPLEX         () const;
      inline       bool              isBGNEXTN      () const;
      inline       bool              isENDEXTN      () const;
      inline       bool              isTAPENUM      () const;   
      inline       bool              isTAPECODE     () const;   
      inline       bool              isSTRCLASS     () const;
      inline       bool              isRESERVED     () const;
      inline       bool              isFORMAT       () const;   
      inline       bool              isMASK         () const;
      inline       bool              isENDMASKS     () const;
      inline       bool              isLIBDIRSIZE   () const;   
      inline       bool              isSRFNAME      () const;   
      inline       bool              isLIBSECUR     () const;   
      inline       bool              hasXReflection () const;
      inline       uint16_t          getType        () const;
      inline       uint16_t          getLength      () const;
      inline const vector<uint16_t>& getMasks       () const;
      inline const vector<int16_t >& getInt16s      () const;
      inline const vector<int32_t >& getInt32s      () const;
      inline const vector<double  >& getDoubles     () const;
      inline       string            getName        () const;
                   void              clear          ();
                   void              read           ( istream* );
                   void              readDummy      ( bool showError );
                   void              readStrans     ();
                   void              readString     ();
                   void              readUnits      ();
                   void              readLayer      ();
                   void              readWidth      ();
                   void              readPathtype   ();
                   void              readStrname    ();
                   void              readXy         ();
      static       string            toStrType      ( uint16_t );
                   GdsRecord&        operator=      ( const GdsRecord& );
    private:
      template< typename IntType> IntType  _readInt    ();
                                  string   _readString ();
                                  double   _readDouble ();
    private:
      istream*          _stream;
      uint32_t          _offset;
      uint16_t          _length;
      uint16_t          _count;
      uint16_t          _type;
      bool              _xReflection;
      string            _name;
      vector<uint16_t>  _masks;
      vector<int16_t>   _int16s;
      vector<int32_t>   _int32s;
      vector<double>    _doubles;
      char              _buffer[64];
  };


  inline       bool              GdsRecord::isHEADER       () const { return (_type == HEADER      ); }   
  inline       bool              GdsRecord::isBGNLIB       () const { return (_type == BGNLIB      ); }   
  inline       bool              GdsRecord::isLIBNAME      () const { return (_type == LIBNAME     ); }   
  inline       bool              GdsRecord::isUNITS        () const { return (_type == UNITS       ); }   
  inline       bool              GdsRecord::isENDLIB       () const { return (_type == ENDLIB      ); }   
  inline       bool              GdsRecord::isBGNSTR       () const { return (_type == BGNSTR      ); }   
  inline       bool              GdsRecord::isSTRNAME      () const { return (_type == STRNAME     ); }   
  inline       bool              GdsRecord::isENDSTR       () const { return (_type == ENDSTR      ); }   
  inline       bool              GdsRecord::isBOUNDARY     () const { return (_type == BOUNDARY    ); }   
  inline       bool              GdsRecord::isPATH         () const { return (_type == PATH        ); }   
  inline       bool              GdsRecord::isSREF         () const { return (_type == SREF        ); }   
  inline       bool              GdsRecord::isAREF         () const { return (_type == AREF        ); }   
  inline       bool              GdsRecord::isTEXT         () const { return (_type == TEXT        ); }   
  inline       bool              GdsRecord::isLAYER        () const { return (_type == LAYER       ); }   
  inline       bool              GdsRecord::isDATATYPE     () const { return (_type == DATATYPE    ); }   
  inline       bool              GdsRecord::isWIDTH        () const { return (_type == WIDTH       ); }
  inline       bool              GdsRecord::isXY           () const { return (_type == XY          ); }
  inline       bool              GdsRecord::isENDEL        () const { return (_type == ENDEL       ); }   
  inline       bool              GdsRecord::isSNAME        () const { return (_type == SNAME       ); }   
  inline       bool              GdsRecord::isCOLROW       () const { return (_type == COLROW      ); }   
  inline       bool              GdsRecord::isTEXTNODE     () const { return (_type == TEXTNODE    ); }
  inline       bool              GdsRecord::isNODE         () const { return (_type == NODE        ); }   
  inline       bool              GdsRecord::isTEXTTYPE     () const { return (_type == TEXTTYPE    ); }   
  inline       bool              GdsRecord::isPRESENTATION () const { return (_type == PRESENTATION); }   
  inline       bool              GdsRecord::isSPACING      () const { return (_type == SPACING     ); }
  inline       bool              GdsRecord::isSTRING       () const { return (_type == STRING      ); }   
  inline       bool              GdsRecord::isSTRANS       () const { return (_type == STRANS      ); }   
  inline       bool              GdsRecord::isMAG          () const { return (_type == MAG         ); }   
  inline       bool              GdsRecord::isANGLE        () const { return (_type == ANGLE       ); }   
  inline       bool              GdsRecord::isREFLIBS      () const { return (_type == REFLIBS     ); }   
  inline       bool              GdsRecord::isFONTS        () const { return (_type == FONTS       ); }   
  inline       bool              GdsRecord::isPATHTYPE     () const { return (_type == PATHTYPE    ); }   
  inline       bool              GdsRecord::isGENERATIONS  () const { return (_type == GENERATIONS ); }   
  inline       bool              GdsRecord::isATTRTABLE    () const { return (_type == ATTRTABLE   ); }   
  inline       bool              GdsRecord::isSTYPTABLE    () const { return (_type == STYPTABLE   ); }
  inline       bool              GdsRecord::isSTRTYPE      () const { return (_type == STRTYPE     ); }
  inline       bool              GdsRecord::isELFLAGS      () const { return (_type == ELFLAGS     ); }   
  inline       bool              GdsRecord::isELKEY        () const { return (_type == ELKEY       ); }
  inline       bool              GdsRecord::isLINKTYPE     () const { return (_type == LINKTYPE    ); }
  inline       bool              GdsRecord::isLINKKEYS     () const { return (_type == LINKKEYS    ); }
  inline       bool              GdsRecord::isNODETYPE     () const { return (_type == NODETYPE    ); }   
  inline       bool              GdsRecord::isPROPATTR     () const { return (_type == PROPATTR    ); }   
  inline       bool              GdsRecord::isPROPVALUE    () const { return (_type == PROPVALUE   ); }   
  inline       bool              GdsRecord::isBOX          () const { return (_type == BOX         ); }   
  inline       bool              GdsRecord::isBOXTYPE      () const { return (_type == BOXTYPE     ); }   
  inline       bool              GdsRecord::isPLEX         () const { return (_type == PLEX        ); }
  inline       bool              GdsRecord::isBGNEXTN      () const { return (_type == BGNEXTN     ); }
  inline       bool              GdsRecord::isENDEXTN      () const { return (_type == ENDEXTN     ); }
  inline       bool              GdsRecord::isTAPENUM      () const { return (_type == TAPENUM     ); }   
  inline       bool              GdsRecord::isTAPECODE     () const { return (_type == TAPECODE    ); }   
  inline       bool              GdsRecord::isSTRCLASS     () const { return (_type == STRCLASS    ); }
  inline       bool              GdsRecord::isRESERVED     () const { return (_type == RESERVED    ); }
  inline       bool              GdsRecord::isFORMAT       () const { return (_type == FORMAT      ); }   
  inline       bool              GdsRecord::isMASK         () const { return (_type == MASK        ); }
  inline       bool              GdsRecord::isENDMASKS     () const { return (_type == ENDMASKS    ); }
  inline       bool              GdsRecord::isLIBDIRSIZE   () const { return (_type == LIBDIRSIZE  ); }   
  inline       bool              GdsRecord::isSRFNAME      () const { return (_type == SRFNAME     ); }   
  inline       bool              GdsRecord::isLIBSECUR     () const { return (_type == LIBSECUR    ); }   
  inline       bool              GdsRecord::hasXReflection () const { return _xReflection; }   
  inline       uint16_t          GdsRecord::getType        () const { return _type; }
  inline       uint16_t          GdsRecord::getLength      () const { return _length; }
  inline const vector<uint16_t>& GdsRecord::getMasks       () const { return _masks; }
  inline const vector<int16_t >& GdsRecord::getInt16s      () const { return _int16s; }
  inline const vector<int32_t >& GdsRecord::getInt32s      () const { return _int32s; }
  inline const vector<double  >& GdsRecord::getDoubles     () const { return _doubles; }
  inline       string            GdsRecord::getName        () const { return _name; }


  GdsRecord::GdsRecord ()
    : _stream     (NULL)
    , _offset     (0)
    , _length     (0)
    , _count      (0)
    , _type       (0)
    , _xReflection(false)
    , _name       ()
    , _masks      ()
    , _int16s     ()
    , _int32s     ()
    , _doubles    ()
  { }


  void  GdsRecord::clear ()
  {
    _stream      = NULL;
    _length      = 0;
    _count       = 0;
    _type        = 0;
    _xReflection = false;
    _name   .clear();
    _masks  .clear();
    _int16s .clear();
    _int32s .clear();
    _doubles.clear();
  }


  void  GdsRecord::read ( istream* stream )
  {
    clear();

    _stream = stream;
    _length = _readInt<uint16_t>();
    _type   = _readInt<uint16_t>();

    switch ( _type ) {
      case HEADER:       readDummy( false ); break;
      case BGNLIB:       readDummy( false ); break;
      case LIBNAME:      readDummy( false ); break;
      case UNITS:        readUnits();        break;
      case ENDLIB:       readDummy( false ); break;
      case BGNSTR:       readDummy( false ); break;
      case STRNAME:      readStrname();      break;
      case ENDSTR:       readDummy( false ); break;
      case BOUNDARY:     readDummy( false ); break;
      case PATH:         readDummy( false ); break;
      case SREF:         readDummy( false ); break;
      case AREF:         readDummy( false ); break;
      case TEXT:         readDummy( false ); break;
      case LAYER:        readLayer();        break;
      case DATATYPE:     readDummy( false ); break;
      case WIDTH:        readWidth();        break;
      case XY:           readXy();           break;
      case ENDEL:        readDummy( false ); break;
      case SNAME:        readStrname();      break;
      case COLROW:       readDummy( false ); break;
      case TEXTNODE:     readDummy( false ); break;
      case NODE:         readDummy( false ); break;
      case TEXTTYPE:     readDummy( false ); break;
      case PRESENTATION: readDummy( false ); break;
      case SPACING:      readDummy( false ); break;
      case STRING:       readString();       break;
      case STRANS:       readStrans();       break;
      case MAG:          readDummy( false ); break;
      case ANGLE:        _doubles.push_back( _readDouble() ); break;
      case REFLIBS:      readDummy( false ); break;
      case FONTS:        readDummy( false ); break;
      case PATHTYPE:     readPathtype();     break;
      case GENERATIONS:  readDummy( false ); break;
      case ATTRTABLE:    readDummy( false ); break;
      case STYPTABLE:    readDummy( false ); break;
      case STRTYPE:      readDummy( false ); break;
      case ELFLAGS:      readDummy( false ); break;
      case ELKEY:        readDummy( false ); break;
      case LINKTYPE:     readDummy( false ); break;
      case LINKKEYS:     readDummy( false ); break;
      case NODETYPE:     readDummy( false ); break;
      case PROPATTR:     readDummy( false ); break;
      case PROPVALUE:    readDummy( false ); break;
      case BOX:          readDummy( false ); break;
      case BOXTYPE:      readDummy( false ); break;
      case PLEX:         readDummy( false ); break;
      case BGNEXTN:      readDummy( false ); break;
      case ENDEXTN:      readDummy( false ); break;
      case TAPENUM:      readDummy( false ); break;
      case TAPECODE:     readDummy( false ); break;
      case STRCLASS:     readDummy( false ); break;
      case RESERVED:     readDummy( false ); break;
      case FORMAT:       readDummy( false ); break;
      case MASK:         readDummy( false ); break;
      case ENDMASKS:     readDummy( false ); break;
      case LIBDIRSIZE:   readDummy( false ); break;
      case SRFNAME:      readDummy( false ); break;
      case LIBSECUR:     readDummy( false ); break;
    }

    ostringstream s;
    s << " (0x" << std::setfill('0') << std::setw(4) << std::hex << _type << ")";
    cdebug_log(101,0) << "GdsRecord::read() " << toStrType(_type)
                      << s.str()
                      << " _bytes:"  <<  _length
                      << " (offset:" << (_offset-4) << ")"
                      << endl;
  }


  GdsRecord& GdsRecord::operator= ( const GdsRecord& other )
  {
    _length   = other._length;
    _count    = other._count;
    _type     = other._type;
    _name     = other._name;
    _masks    = other._masks;
    _int16s   = other._int16s;
    _int32s   = other._int32s;
    _doubles  = other._doubles;
    return *this;
  }


  template< typename IntType>
  IntType  GdsRecord::_readInt ()
  {
    const size_t typeSize = sizeof(IntType);
    
    union UType {
        IntType       integer;
        unsigned char bytes[ typeSize ];
    };

    unsigned char bytes[typeSize];
    _stream->read( (char*)(&bytes[0]), typeSize );
    _count  += typeSize;

    // cdebug_log(101,0) << "GdsRecord::_readInt() " << endl;
    // for ( size_t i=0 ; i<typeSize ; ++i ) {
    //   sprintf( _buffer, "0x%02x", bytes[i] );
    //   cdebug(101,0) << setw(6) << hex << _offset++ << " | " << _buffer << endl; 
    // }

    UType uvalue;
  // Little endian (x86).
    for ( size_t i=0 ; i<typeSize ; ++i ) uvalue.bytes[i] = bytes[typeSize-i-1];

    return uvalue.integer;
  }


  double  GdsRecord::_readDouble ()
  {
    union gdsUInt64 {
        uint64_t      uint64;
        unsigned char bytes[8];
    };

    char bytes[8];
    _stream->read( bytes, 8 );
    _count += 8;

    // cdebug_log(101,0) << "GdsRecord::_readDouble() " << endl;
    // for ( size_t i=0 ; i<8 ; ++i ) {
    //   sprintf( _buffer, "0x%02x", bytes[i] );
    //   cdebug(101,0) << setw(6) << hex << _offset++ << " | " << _buffer << endl; 
    // }

    gdsUInt64 mantisse;
    for ( size_t i=0 ; i<7 ; ++i ) mantisse.bytes[i] = bytes[7-i];
    mantisse.bytes[7] = 0;
    double value = mantisse.uint64;

    if (bytes[0] & 0x80) value = -value;

    int32_t exponent = (bytes[0] & 0x7f) - (64+14);
    if (exponent) value *= pow( 16.0, (double)exponent );

    return value;
  }


  string  GdsRecord::_readString ()
  {
    cdebug_log(101,0) << "GdsRecord::_readDouble() " << endl;
    string s;
    char c;
    for ( ; _count<_length ; ++_count ) {
      _stream->get( c );
      // sprintf( _buffer, "0x%02x", c );
      // cdebug_log(101,0) << setw(6) << hex << _offset++ << " | " << _buffer << endl; 

      if (c != (char)0) s.push_back(c);
    }
    cdebug_log(101,0) << "GdsRecord::_readString(): \"" << s << "\"" << endl;
    return s;
  }


  void  GdsRecord::readDummy ( bool showError )
  {
    cdebug_log(101,0) << "GdsRecord::readDummy() " << endl;
    char c;
    for ( ; _count<_length ; ++_count ) {
      _stream->get( c );   
      sprintf( _buffer, "0x%02x", c );
      cdebug_log(101,0) << setw(6) << hex << _offset++ << " | " << _buffer << endl; 
    }
    if (showError) {
      cdebug_log(101,0) << Error( "GdsRecord type %s unsupported.", toStrType(_type).c_str() ) << endl;
    }
  }


  void  GdsRecord::readStrans ()
  {
    const uint16_t XRMask = 0x8000;
          uint16_t flags  = _readInt<uint16_t>();
    _xReflection = (flags & XRMask);

    // cerr << "GdsRecord::readStrans(): length=" << _length
    //      << " flags:" << bitset<16>(flags)
    //      << " mask:" << bitset<16>(XRMask)
    //      << " _xR:" << _xReflection << endl;
  }


  void  GdsRecord::readUnits ()
  { for ( size_t i=0 ; i<2 ; ++i ) _doubles.push_back( _readDouble() ); }


  void  GdsRecord::readStrname ()
  { _name = _readString(); }


  void  GdsRecord::readString ()
  { _name = _readString(); }


  void  GdsRecord::readLayer ()
  { _int16s.push_back( _readInt<uint16_t>() ); }


  void  GdsRecord::readWidth ()
  { _int32s.push_back( _readInt<uint32_t>() ); }


  void  GdsRecord::readPathtype ()
  { _int16s.push_back( _readInt<uint16_t>() ); }


  void  GdsRecord::readXy ()
  { while ( _count < _length ) _int32s.push_back( _readInt<int32_t>() ); }


  string  GdsRecord::toStrType ( uint16_t type )
  {
    switch ( type ) {
      case HEADER:      return "HEADER";
      case BGNLIB:      return "BGNLIB";
      case LIBNAME:     return "LIBNAME";
      case UNITS:       return "UNITS";
      case ENDLIB:      return "ENDLIB";
      case BGNSTR:      return "BGNSTR";
      case STRNAME:     return "STRNAME";
      case ENDSTR:      return "ENDSTR";
      case BOUNDARY:    return "BOUNDARY";
      case PATH:        return "PATH";
      case SREF:        return "SREF";
      case AREF:        return "AREF";
      case TEXT:        return "TEXT";
      case LAYER:       return "LAYER";
      case DATATYPE:    return "DATATYPE";
      case WIDTH:       return "WIDTH";
      case XY:          return "XY";
      case ENDEL:       return "ENDEL";
      case SNAME:       return "SNAME";
      case COLROW:      return "COLROW";
      case TEXTNODE:    return "TEXTNODE";
      case NODE:        return "NODE";
      case TEXTTYPE:    return "TEXTTYPE";
      case PRESENTATION:return "PRESENTATION";
      case SPACING:     return "SPACING";
      case STRING:      return "STRING";
      case STRANS:      return "STRANS";
      case MAG:         return "MAG";
      case ANGLE:       return "ANGLE";
      case REFLIBS:     return "REFLIBS";
      case FONTS:       return "FONTS";
      case PATHTYPE:    return "PATHTYPE";
      case GENERATIONS: return "GENERATIONS";
      case ATTRTABLE:   return "ATTRTABLE";
      case STYPTABLE:   return "STYPTABLE";
      case STRTYPE:     return "STRTYPE";
      case ELFLAGS:     return "ELFLAGS";
      case ELKEY:       return "ELKEY";
      case LINKTYPE:    return "LINKTYPE";
      case LINKKEYS:    return "LINKKEYS";
      case NODETYPE:    return "NODETYPE";
      case PROPATTR:    return "PROPATTR";
      case PROPVALUE:   return "PROPVALUE";
      case BOX:         return "BOX";
      case BOXTYPE:     return "BOXTYPE";
      case PLEX:        return "PLEX";
      case BGNEXTN:     return "BGNEXTN";
      case ENDEXTN:     return "ENDEXTN";
      case TAPENUM:     return "TAPENUM";
      case TAPECODE:    return "TAPECODE";
      case STRCLASS:    return "STRCLASS";
      case RESERVED:    return "RESERVED";
      case FORMAT:      return "FORMAT";
      case MASK:        return "MASK";
      case ENDMASKS:    return "ENDMASKS";
      case LIBDIRSIZE:  return "LIBDIRSIZE";
      case SRFNAME:     return "SRFNAME";
      case LIBSECUR:    return "LIBSECUR";
    }

    ostringstream error;
    error << "undefined GDS record type (0x" << hex << type << ")"; 
    return error.str();
  }


  istream& operator>> ( istream& stream, GdsRecord& record )
  { record.read( &stream ); return stream; }


// -------------------------------------------------------------------
// Class  :  "::GdsStream".

  class GdsStream {
    public:
      static const Layer* gdsToLayer       ( uint16_t );
    public:                                
      static       void   _staticInit      ();
                          GdsStream        ( string gdsPath );
      inline       bool   isValidSyntax    () const;
                   bool   misplacedRecord  ();
      inline       void   resetStrans      ();
                   bool   read             ( Library* );
                   bool   readFormatType   ();
                   bool   readStructure    ();
                   bool   readBoundary     ();
                   bool   readPath         ();
                   bool   readSref         ();
                   bool   readAref         ();
                   bool   readNode         ();
                   bool   readBox          ();
                   bool   readText         ();
                   bool   readTextbody     ( const Layer* );
                   bool   readStrans       ();
                   bool   readProperty     ();
                   void   xyToComponent    ( const Layer* );
                   void   xyToPath         ( uint16_t pathtype, const Layer*, DbU::Unit width );
                   void   makeInstances    ();
                   void   makeExternals    ();
                   Net*   fusedNet         ();
                   void   addNetReference  ( Net*, const Layer*, DbU::Unit x, DbU::Unit y );
    private:
      struct DelayedInstance {
          inline DelayedInstance ( Cell* owner, string masterName, const Transformation& );

          Cell*           _owner;
          string          _masterName;
          Transformation  _transformation;
      };
    private:
      struct PinPoint {
          inline PinPoint ( const Layer*, DbU::Unit x, DbU::Unit y );
          const Layer* _layer;
          Point        _position;
      };
    private:
      static vector<const Layer*>     _gdsLayerTable;
             vector<DelayedInstance>  _delayedInstances;
             string                   _gdsPath;
             ifstream                 _stream;
             GdsRecord                _record;
             double                   _angle;
             bool                     _xReflection;
             Library*                 _library;
             Cell*                    _cell;
             Component*               _component;
             string                   _text;
             DbU::Unit                _scale;
             int64_t                  _SREFCount;
             bool                     _validSyntax;
             bool                     _skipENDEL;
             map< Net*
                , vector<PinPoint>
                , DBo::CompareById >  _netReferences;
  };


  inline GdsStream::DelayedInstance::DelayedInstance (       Cell*           owner
                                                     ,       string          masterName
                                                     , const Transformation& transf )
    : _owner(owner), _masterName(masterName), _transformation(transf)
  { }


  inline GdsStream::PinPoint::PinPoint ( const Layer* layer, DbU::Unit x, DbU::Unit y )
    : _layer(layer), _position(x,y)
  { }

  
  vector<const Layer*>  GdsStream::_gdsLayerTable;


  void  GdsStream::_staticInit ()
  {
    _gdsLayerTable = vector<const Layer*>( 64, NULL );
    for ( const BasicLayer* layer : DataBase::getDB()->getTechnology()->getBasicLayers() ) {
      unsigned int gdsNumber = layer->getGds2Layer();
      if (gdsNumber < 64) _gdsLayerTable[gdsNumber] = layer;
    }
  }


  inline  void  GdsStream::resetStrans () { _angle = 0.0; _xReflection = false; }


  const Layer* GdsStream::gdsToLayer ( uint16_t gdsLayer )
  { return (gdsLayer < _gdsLayerTable.size()) ? _gdsLayerTable[gdsLayer] : NULL; }


  inline bool  GdsStream::isValidSyntax () const { return _validSyntax; }


  GdsStream::GdsStream ( string gdsPath )
    : _delayedInstances()
    , _gdsPath         (gdsPath)
    , _stream          ()
    , _record          ()
    , _angle           (0.0)
    , _xReflection     (false)
    , _library         (NULL)
    , _cell            (NULL)
    , _component       (NULL)
    , _text            ()
    , _scale           (1)
    , _SREFCount       (0)
    , _validSyntax     (true)
    , _skipENDEL       (false)
  {
    if (_gdsLayerTable.empty()) _staticInit();
    
    _stream.open( gdsPath, ios_base::in|ios_base::binary );
    if (not _stream.is_open()) {
      cerr << Error( "GdsStream::GdsStream(): Unable to open stream, check path.\n"
                     "        \"%s\""
                   , _gdsPath.c_str() ) << endl;
      _validSyntax = false;
      return;
    }

    _stream >> _record;
    if (not _record.isHEADER()) {
      cerr << Error( "GdsStream::GdsStream(): First record is not a HEADER.\n"
                     "        in \"%s\""
                   , _gdsPath.c_str() ) << endl;
      _validSyntax = false;
      return;
    }
    _stream >> _record;
  }


  bool  GdsStream::misplacedRecord ()
  {
    cerr << Error( "GdsStream: Misplaced record %s.\n"
                 "        in \"%s\""
                 , GdsRecord::toStrType(_record.getType()).c_str()
                 , _gdsPath.c_str() ) << endl;
    _validSyntax = false;
    return _validSyntax;
  }


  bool  GdsStream::read ( Library* library )
  {
    cdebug_log(101,1) << "GdsStream::read(Library*)" << endl;
    
    _library = library;

    if (not _record.isBGNLIB()) {
      cerr << Error( "GdsStream::read(Library*): Starting record is not a BGNLIB.\n"
                     "        in \"%s\""
                   , _gdsPath.c_str() ) << endl;
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }
    _stream >> _record;

    if (_record.isLIBDIRSIZE ()) { _stream >> _record; }
    if (_record.isSRFNAME    ()) { _stream >> _record; }
    if (_record.isLIBSECUR   ()) { _stream >> _record; }
    if (_record.isLIBNAME    ()) { _stream >> _record; }
    else {
      cerr << Error( "GdsStream::read(Library*): Missing LIBNAME record.\n"
                     "        in \"%s\""
                   , _gdsPath.c_str() ) << endl;
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }
    if (_record.isREFLIBS    ()) { _stream >> _record; }
    if (_record.isFONTS      ()) { _stream >> _record; }
    if (_record.isATTRTABLE  ()) { _stream >> _record; }
    if (_record.isGENERATIONS()) { _stream >> _record; }
    if (_record.isFORMAT     ()) {
      readFormatType();
      if (not _validSyntax) {
        cdebug_tabw(101,-1);
        return _validSyntax;
      }
    }

    if (_record.isUNITS()) {
      _scale = DbU::fromPhysical( _record.getDoubles()[1], DbU::Unity );
      _stream >> _record;
    }

    while ( _validSyntax and _record.isBGNSTR() ) {
      _stream >> _record;
      readStructure();
    }

    if (_validSyntax and not _record.isENDLIB()) { misplacedRecord(); }

    if (_validSyntax) {
      makeInstances();
      makeExternals();
    }

    _library = NULL;
    cdebug_log(101,-1) << "    GdsStream::read(Library*) - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readFormatType ()
  {
    cdebug_log(101,0) << "GdsStream::readFormatType()" << endl;

    if (_record.isMASK()) {
      _stream >> _record;
      while ( _record.isMASK() ) {
        _stream >> _record;
      }

      if (_record.isENDMASKS()) { _stream >> _record; }
      else { _validSyntax = false; return _validSyntax; }
    }

  //cdebug(101,0) << "GdsStream::readFormatType() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readStructure ()
  {
    cdebug_log(101,1) << "GdsStream::readStructure()" << endl;
    
    if (_record.isSTRNAME()) {
      if (_library) {
        string cellName = "gds_" + _record.getName();
        _cell = _library->getCell( cellName );
        if (not _cell) {
          cerr << Warning( "GdsStream::readStructure(): No Cell named \"gds_%s\" in Library \"%s\" (created)."
                         , _record.getName().c_str()
                         , getString(_library).c_str()
                         ) << endl;
          _cell = Cell::create( _library, cellName );
        }
      }
      _stream >> _record;
    }

    if (_record.isSTRCLASS()) { _stream >> _record; }

    while ( not _record.isENDSTR() ) {
      uint16_t rtype = _record.getType();
      switch ( rtype ) {
        case GdsRecord::BOUNDARY:  _stream >> _record; readBoundary(); break;
        case GdsRecord::PATH:      _stream >> _record; readPath    (); break;
        case GdsRecord::SREF:      _stream >> _record; readSref    (); break;
        case GdsRecord::AREF:      _stream >> _record; readAref    (); break;
        case GdsRecord::TEXT:      _stream >> _record; readText    (); break;
        case GdsRecord::NODE:      _stream >> _record; readNode    (); break;
        case GdsRecord::BOX:       _stream >> _record; readBox     (); break;
        case GdsRecord::PROPATTR:  _stream >> _record; readProperty(); break;
      }
      if (not _skipENDEL) {
        if (_record.isENDEL()) { _stream >> _record; }
        else {
          _validSyntax = false;
          cdebug_log(101,0) << "Missing ENDEL inside STRUCTURE loop" << endl;
          break;
        }
      } else
        _skipENDEL = false;
      if (rtype != GdsRecord::BOUNDARY) _component = NULL;
    }

    if (_validSyntax) _stream >> _record;

    UpdateSession::close();
    _cell->setAbutmentBox( _cell->getBoundingBox() );
    UpdateSession::open();
    _cell = NULL;
    cdebug_log(101,-1) << "    GdsStream::readStructure() - return:" << _validSyntax << endl;

    return _validSyntax;
  }


  bool  GdsStream::readText ()
  {
    const Layer* layer = NULL;
    
    cdebug_log(101,0) << "GdsStream::readText()" << endl;
    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }
    if (_record.isLAYER  ()) {
      layer = gdsToLayer( _record.getInt16s()[0] );
      if (not layer) {
        cerr << Error( "GdsStream::readText(): No BasicLayer id:%d in GDS conversion table (skipped)."
                     , _record.getInt16s()[0]
                     ) << endl;
      }
      _stream >> _record;
    } else { _validSyntax = false; return _validSyntax; }

    readTextbody( layer  );

  //cdebug(101,0) << "GdsStream::readText() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readTextbody ( const Layer* layer )
  {
    cdebug_log(101,1) << "GdsStream::readTextbody()" << endl;

    DbU::Unit xpos = 0;
    DbU::Unit ypos = 0;

    if (_record.isTEXTTYPE()) { _stream >> _record; }
    else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isPRESENTATION()) { _stream >> _record; }
    if (_record.isPATH        ()) { _stream >> _record; }
    if (_record.isWIDTH       ()) { _stream >> _record; }

    if (_record.isSTRANS()) {
      _stream >> _record;
      readStrans();
      if (not _validSyntax) {
        cdebug_tabw(101,-1);
        return _validSyntax;
      }
    }

    if (_record.isXY()) {
      vector<int32_t> coordinates = _record.getInt32s();
      if (coordinates.size() != 2) {
        _validSyntax = false;
        cdebug_tabw(101,-1);
        return _validSyntax;
      }
      xpos = coordinates[ 0 ]*_scale;
      ypos = coordinates[ 1 ]*_scale;
      _stream >> _record;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isSTRING()) {
      _text = _record.getName();
      _stream >> _record;
    }
    else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (not _text.empty()) {
      Net* net = _cell->getNet( _text );
      if (not net) {
        net = Net::create( _cell, _text );
        net->setExternal( true );
      }
      addNetReference( net, layer, xpos, ypos );
    }

    cdebug_log(101,-1) << "GdsStream::readTextbody() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readStrans ()
  {
    cdebug_log(101,0) << "GdsStream::readStrans()" << endl;
    
    _angle       = 0.0;
    _xReflection = _record.hasXReflection();
    _stream >> _record;
    if (_record.isMAG  ()) { _stream >> _record; }
    if (_record.isANGLE()) { _angle = _record.getDoubles()[0]; _stream >> _record; }

  //cdebug(101,0) << "GdsStream::readStrans() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readBoundary ()
  {
    cdebug_log(101,1) << "GdsStream::readBoundary()" << endl;
    
    const Layer* layer = NULL;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isLAYER()) {
      layer = gdsToLayer( _record.getInt16s()[0] );
      if (not layer) {
        cerr << Error( "GdsStream::readBoundary(): No BasicLayer id:%d in GDS conversion table (skipped)."
                     , _record.getInt16s()[0]
                     ) << endl;
      }
      _stream >> _record;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isDATATYPE()) { _stream >> _record; }
    else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isXY()) {
      if (_cell and layer) xyToComponent( layer );
      else                 _stream >> _record;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    cdebug_log(101,-1) << "GdsStream::readBoundary() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readPath ()
  {
    cdebug_log(101,1) << "GdsStream::readPath()" << endl;
    
    const Layer*    layer    = NULL;
          DbU::Unit width    = 0;
          uint16_t  pathtype = 0;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isLAYER()) {
      layer = gdsToLayer( _record.getInt16s()[0] );
      if (not layer) {
        cerr << Error( "GdsStream::readPath(): No BasicLayer id \"%d\" in GDS conversion table (skipped)."
                     , _record.getInt16s()[0]
                     ) << endl;
      }
      _stream >> _record;
      cdebug_log(101,0) << layer << endl;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isDATATYPE()) { _stream >> _record; }
    else {
      cerr << Error( "GdsStream::readPath(): Expecting DATATYPE record, got %s."
                   , GdsRecord::toStrType(_record.getType()).c_str()
                   ) << endl;
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isPATHTYPE()) {
      pathtype = _record.getInt16s()[0];
      _stream >> _record;
    }
    if (_record.isWIDTH   ()) {
      width = _record.getInt32s()[0] * _scale;
      _stream >> _record;
    }
    if (_record.isBGNEXTN ()) { _stream >> _record; }
    if (_record.isENDEXTN ()) { _stream >> _record; }

    if (_record.isXY()) {
      if (_cell and layer) xyToPath( pathtype, layer, width );
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    cdebug_log(101,-1) << "GdsStream::readPath() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readSref ()
  {
    cdebug_log(101,1) << "GdsStream::readSref()" << endl;
    resetStrans();
    
    string    masterName;
    DbU::Unit xpos       = 0;
    DbU::Unit ypos       = 0;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isSNAME()) {
      masterName = _record.getName();
      _stream >> _record;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (_record.isSTRANS()) {
      readStrans();
    //_stream >> _record;
      if (not _validSyntax) {
        cdebug_tabw(101,-1);
        return _validSyntax;
      }
    }

    if (_record.isXY()) {
      vector<int32_t> coordinates = _record.getInt32s();
      if (coordinates.size() != 2) {
        _validSyntax = false;
        cdebug_tabw(101,-1);
        return _validSyntax;
      }
      
      xpos = coordinates[ 0 ]*_scale;
      ypos = coordinates[ 1 ]*_scale;

      _stream >> _record;
    } else {
      _validSyntax = false;
      cdebug_tabw(101,-1);
      return _validSyntax;
    }

    if (not masterName.empty()) {
      Transformation::Orientation orient = Transformation::Orientation::ID;
      if      (_angle ==  90.0) orient = Transformation::Orientation::R1;
      else if (_angle == 180.0) orient = Transformation::Orientation::R2;
      else if (_angle == 270.0) orient = Transformation::Orientation::R3;
      else if (_angle !=   0.0) {
        cerr << Warning( "GdsStream::readSref(): Unsupported angle %d.2 for SREF (Instance) of \"%s\""
                       , _angle, masterName.c_str() ) << endl;
      }

      if (_xReflection) {
        switch ( orient ) {
          case Transformation::Orientation::ID: orient = Transformation::Orientation::MY; break;
          case Transformation::Orientation::R1: orient = Transformation::Orientation::YR; break;
          case Transformation::Orientation::R2: orient = Transformation::Orientation::MX; break;
          case Transformation::Orientation::R3: orient = Transformation::Orientation::XR; break;
          default:
            cerr << Warning( "GdsStream::readSref(): Unsupported MX+Orientation (%s) combination for SREF (Instance) of \"%s\""
                           , getString(orient).c_str(), masterName.c_str() ) << endl;
        }
      }

      // cerr << "Delayed Instance: "  << masterName
      //      << " XR:" << _xReflection << " angle:" << _angle
      //      << " " << Transformation(xpos,ypos,orient)
      //      << " in " << _cell << endl;
      _delayedInstances.push_back( DelayedInstance( _cell
                                                  , "gds_" + masterName
                                                  , Transformation(xpos,ypos,orient)) );
    }

    cdebug_log(101,-1) << "GdsStream::readSref() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readAref ()
  {
    cdebug_log(101,0) << "GdsStream::readAref()" << endl;
    
    const Layer* layer = NULL;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isSNAME()) {
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    if (_record.isSTRANS()) {
      _stream >> _record;
      readStrans();
      if (not _validSyntax) return _validSyntax;
    }

    if (_record.isCOLROW()) {
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    if (_record.isXY()) {
      if (_cell and layer) xyToComponent( layer );
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    cdebug_log(101,0) << "GdsStream::readAref() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readNode ()
  {
    cdebug_log(101,0) << "GdsStream::readNode()" << endl;
    
    const Layer* layer = NULL;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isLAYER  ()) { _stream >> _record; }
    else { _validSyntax = false; return _validSyntax; }

    if (_record.isNODETYPE()) {
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    if (_record.isXY()) {
      if (_cell and layer) xyToComponent( layer );
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    cdebug_log(101,0) << "GdsStream::readNode() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readBox ()
  {
    cdebug_log(101,0) << "GdsStream::readBox()" << endl;
    
    const Layer* layer = NULL;

    if (_record.isELFLAGS()) { _stream >> _record; }
    if (_record.isPLEX   ()) { _stream >> _record; }

    if (_record.isLAYER  ()) { _stream >> _record; }
    else { _validSyntax = false; return _validSyntax; }

    if (_record.isBOXTYPE()) {
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    if (_record.isXY()) {
      if (_cell and layer) xyToComponent( layer );
      _stream >> _record;
    } else {
      _validSyntax = false; return _validSyntax;
    }

    cdebug_log(101,0) << "GdsStream::readBox() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  bool  GdsStream::readProperty ()
  {
    cdebug_log(101,0) << "GdsStream::readProperty()" << endl;
    
    if (_record.isPROPVALUE  ()) { _stream >> _record; }
    else { _validSyntax = false; return _validSyntax; }

    cdebug_log(101,0) << "GdsStream::readProperty() - return:" << _validSyntax << endl;
    return _validSyntax;
  }


  void  GdsStream::xyToComponent ( const Layer* layer )
  {
    vector<Point>   points;
    vector<int32_t> coordinates = _record.getInt32s();
    for ( size_t i=0 ; i<coordinates.size() ; i += 2 )
      points.push_back( Point( coordinates[i  ]*_scale
                             , coordinates[i+1]*_scale ) );

    _stream >> _record;

    if (  (_record.getType() == GdsRecord::ENDEL)
       or (_record.getType() == GdsRecord::STRING)) {
    //_stream >> _record;
    } else {
      _validSyntax = false;
      return;
    }
    
    Net* net = NULL;
    if (_record.getType() == GdsRecord::TEXT) {
      _stream >> _record;
      readText();

      if (not _text.empty()) {
        net = _cell->getNet( _text );
        if (not net) {
          net = Net::create( _cell, _text );
          net->setExternal( true );
        }
      }
    } else
      _skipENDEL = true;

    if (not net) net = fusedNet();

    if (points.size() > 2) {
      bool isRectilinear = true;
      for ( size_t i=1 ; i<points.size() ; ++i ) {
        if (   (points[i-1].getX() != points[i].getX()) 
           and (points[i-1].getY() != points[i].getY()) ) {
          isRectilinear = false;
          break;
        }
      }

      if (isRectilinear and (points.size() == 5)) {
        Box boundingBox; 
        for ( Point p : points ) boundingBox.merge( p );
        _component = Pad::create( net, layer, boundingBox );
      } else {
        _component = Rectilinear::create( net, layer, points );
      }
      // cdebug(101,0) << "| " << net->getCell() << endl;
      // cdebug(101,0) << "| " << _component << endl;

      if (not net->isAutomatic()) NetExternalComponents::setExternal( _component );
    }
  }


  void  GdsStream::xyToPath ( uint16_t pathtype, const Layer* layer, DbU::Unit width )
  {
    cdebug_log(101,0) << "GdsStream::xyToPath(): pathtype=" << pathtype
                      << " layer=" << layer->getName()
                      << " width=" << DbU::getValueString(width) << endl;

    vector<Point>   points;
    vector<int32_t> coordinates = _record.getInt32s();
    for ( size_t i=0 ; i<coordinates.size() ; i += 2 )
      points.push_back( Point( coordinates[i  ]*_scale
                             , coordinates[i+1]*_scale ) );
    _stream >> _record;
    if (_record.getType() != GdsRecord::ENDEL) {
      _validSyntax = false;
      return;
    }

    Net* net = fusedNet();

    for ( size_t i=1 ; i<points.size() ; ++i ) {
      if (   (points[i-1].getX() != points[i].getX()) 
         and (points[i-1].getY() != points[i].getY()) ) {
        cerr << Error( "GdsStream::xyToPath(): Non-rectilinear paths are not supporteds (skipped)."
                     ) << endl;
        return;
      }
    }

    DbU::Unit hWidthCap = width;
    DbU::Unit vWidthCap = width;
    if (pathtype == 0) {
      if (points[0].getX() == points[1].getX()) vWidthCap = 0;
      else                                      hWidthCap = 0;
    }
    Contact* source = Contact::create( net
                                     , layer
                                     , points[0].getX()
                                     , points[0].getY()
                                     , hWidthCap
                                     , vWidthCap );
    hWidthCap = width;
    vWidthCap = width;
    Contact* target  = NULL;
    Segment* segment = NULL;
    for ( size_t i=1 ; i<points.size() ; ++i ) {
      if (i == points.size()-1) {
        if (pathtype == 0) {
          if (points[0].getX() == points[1].getX()) vWidthCap = 0;
          else                                      hWidthCap = 0;
        }
      }
      target = Contact::create( net
                              , layer
                              , points[i].getX()
                              , points[i].getY()
                              , hWidthCap
                              , vWidthCap );
      if (points[i-1].getY() == points[i].getY()) {
        segment = Horizontal::create( source
                                    , target
                                    , layer
                                    , points[i].getY()
                                    , width
                                    , 0, 0 );
      } else {
        segment = Vertical::create( source
                                  , target
                                  , layer
                                  , points[i].getX()
                                  , width
                                  , 0, 0 );
      }
      if (not net->isAutomatic()) NetExternalComponents::setExternal( segment );
      source = target;
    }
  }


  void  GdsStream::makeInstances ()
  {
    cdebug_log(101,1) << "GdsStream::makeInstances(): " << endl;

    for ( const DelayedInstance& di : _delayedInstances ) {
      Cell* masterCell = _library->getCell( di._masterName );

      if (masterCell) {
        string    insName  = "sref_" + getString(_SREFCount++);
        Instance* instance =
          Instance::create( di._owner
                          , insName
                          , masterCell
                          , di._transformation
                          , Instance::PlacementStatus::FIXED
                          );
          cdebug_log(101,0) << "| " << instance << " @" << di._transformation << " in " << di._owner << endl;
      }
    }
    cdebug_tabw(101,-1);
  }


  void  GdsStream::addNetReference  ( Net* net, const Layer* layer, DbU::Unit x, DbU::Unit y )
  {
    if (not layer) return;

    auto inet = _netReferences.find( net );
    if (inet == _netReferences.end()) {
      _netReferences[ net ] = vector<PinPoint>();
    }
    _netReferences[ net ].push_back( PinPoint(layer,x,y) );
  }

  
  void  GdsStream::makeExternals ()
  {
    UpdateSession::close();
    UpdateSession::open();
    for ( auto netPins : _netReferences ) {
      Net* net = netPins.first;
      for ( const PinPoint& ref : netPins.second ) {
        const Layer* layer = ref._layer;
        string layerName = getString( layer->getName() );
        if (layerName.substr(layerName.size()-4) == ".pin") {
          layer = DataBase::getDB()->getTechnology()->getLayer( layerName.substr(0,layerName.size()-4) );
        }
        cdebug_log(101,0) << "Looking for components of \"" << net->getName()
                          << "\" in " << layer
                          << " @" << ref._position
                          << endl;
        if (not layer) continue;
        for ( Component* component : net->getCell()
                ->getComponentsUnder( Box(ref._position).inflate(1),layer->getMask() ) ) {
          cdebug_log(101,0) << "| " << component << endl;
          Horizontal* href = dynamic_cast<Horizontal*>( component );
          if (href) {
            Horizontal* h = Horizontal::create( net
                                              , href->getLayer()
                                              , href->getY()
                                              , href->getWidth()
                                              , href->getSourceX()
                                              , href->getTargetX()
                                              );
            NetExternalComponents::setExternal( h );
          } else {
            Vertical* vref = dynamic_cast<Vertical*>( component );
            if (vref) {
              Vertical* v = Vertical::create( net
                                            , vref->getLayer()
                                            , vref->getX()
                                            , vref->getWidth()
                                            , vref->getSourceY()
                                            , vref->getTargetY()
                                            );
              NetExternalComponents::setExternal( v );
            }
          }
        }
      }
    }
  }


  Net* GdsStream::fusedNet ()
  {
    if (not _cell) return NULL;

    string netName = "all_nets_merged_in_gds";
    Net*   net     = _cell->getNet( netName );
    if (not net) {
      net = Net::create( _cell, netName );
      net->setAutomatic( true );
      net->setType     ( Net::Type::FUSED );
    }
    return net;
  }


}  // Anonymous namespace.


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::Gds".

  bool  Gds::load ( Library* library, string gdsPath )
  {
    UpdateSession::open();

    GdsStream gstream ( gdsPath );

    if (not gstream.read( library ))
      cerr << Error( "Gds::load(): An error occurred while reading GDSII stream\n"
                     "        \"%s\"."
                   , gdsPath.c_str()
                   ) << endl;

    UpdateSession::close();

    return true;
  }


}  // CRL namespace.
