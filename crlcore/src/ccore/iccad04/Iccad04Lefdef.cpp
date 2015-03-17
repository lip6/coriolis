// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         ICCAD04 IBM Standard Cell Benchmarks                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Iccad04Lefdef.cpp"                      |
// +-----------------------------------------------------------------+


#include  <cstdio>
#include  <cstring>
#include  <memory>
#if HAVE_LEFDEF
#  include  "lefrReader.hpp"
#  include  "defrReader.hpp"
#endif
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DataBase.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Cell.h"
#include  "hurricane/Library.h"
#include  "hurricane/UpdateSession.h"
#include  "crlcore/Utilities.h"
#include  "crlcore/ToolBox.h"
#include  "crlcore/AllianceFramework.h"
#include  "crlcore/Iccad04Lefdef.h"


#if HAVE_LEFDEF

namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;

  class LefParser;


  string  toLower ( const char* s )
  {
    if ( s == NULL ) return "";

    string lowered;
    for ( const char* p=s; (*p) != '\0' ; ++p ) lowered += tolower(*p);

    return lowered;
  }


  void  addSupplyNets ( Cell* cell )
  {
    Net* vss = Net::create ( cell, "vss" );
    vss->setExternal ( true );
    vss->setGlobal   ( true );
    vss->setType     ( Net::Type::GROUND );

    Net* vdd = Net::create ( cell, "vdd" );
    vdd->setExternal ( true );
    vdd->setGlobal   ( true );
    vdd->setType     ( Net::Type::POWER );
  }


  class Ports {
    public:
            Ports         ( LefParser* );
      void  copyPinNames  ( vector<string>& pinNames );
      bool  addGeometry   ( double x, double y, const char* pinName );
      bool  addGeometry   ( size_t pitchNb, size_t sliceNb, const char* pinName );
      void  normalize     ();
      void  makeTerminals ();
      void  makeBlockages ();
      void  clear         ();
      void  toStream      ( ostream& );
    private:
      typedef  pair<size_t,string>      PinPosition;
      typedef  map<size_t,PinPosition>  Slice;
      typedef  vector<Slice>            Area;
    private:
      LefParser* _parser;
      Area       _area;
      size_t     _coronaPitch;
      size_t     _coronaSlice;
  };


  class LefParser {
    public:
      static double             getCoreSiteWidth  ();
      static double             getCoreSiteHeight ();
      static DbU::Unit          getSliceHeight    ();
      static DbU::Unit          getPitchWidth     ();
      static AllianceFramework* getFramework      ();
      static bool               hasPadMacro       ();
      static bool               isPadMacro        ( const char* );
      static bool               isPadMacro        ( const string& );
      static void               setPadMacro       ( const string& );
      static void               parse             ( string file );
                                LefParser         ( string& file, AllianceLibrary* );
                               ~LefParser         ();
      inline bool               hasErrors         ();
      inline AllianceLibrary*   getLibrary        ();
      inline Cell*              getCell           ();
      inline size_t             getPitchs         () const;
      inline size_t             getSlices         () const;
      inline size_t             getReadPins       () const;
      inline Ports&             getPorts          ();
      inline vector<string>&    getErrors         ();
      inline void               pushError         ( const string& );
             int                flushErrors       ( const string& );
      inline void               clearErrors       ();
      inline void               clearPorts        ();
      inline void               resetReadPins     ();
      inline void               setPitchs         ( size_t );
      inline void               setSlices         ( size_t );
      inline void               incReadPins       ();
             void               makeBlock         ();
    private:                                      
      static int                _macroCbk         ( lefrCallbackType_e, lefiMacro*, lefiUserData );
      static int                _pinCbk           ( lefrCallbackType_e, lefiPin*  , lefiUserData );
             Cell*              _createCell       ( const char* name );
    private:
      static const double       _coreSiteWidth;
      static const double       _coreSiteHeight;
      static DbU::Unit          _sliceHeight;
      static DbU::Unit          _pitchWidth;
      static AllianceFramework* _framework;
      static Name               _padMacro;
             string             _file;
             AllianceLibrary*   _library;
             Cell*              _cell;
             size_t             _pitchs;
             size_t             _slices;
             size_t             _readPins;
             Ports              _ports;
             vector<string>     _errors;
  };


  Ports::Ports ( LefParser* parser )
    : _parser     (parser)
    , _area       ()
    , _coronaPitch(0)
    , _coronaSlice(0)
  {
  //cerr << "Ports::Ports() - " << _pitch << "," << _sliceHeight << endl;
  }


  void  Ports::copyPinNames ( vector<string>& pinNames )
  {
    for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
      Slice::iterator ipos = _area[islice].begin ();
      for ( ; ipos != _area[islice].end() ; ++ipos ) {
        pinNames.push_back ( (*ipos).second.second );
      }
    }
  }


  bool  Ports::addGeometry ( size_t pitchNb, size_t sliceNb, const char* pinName )
  {
  //cerr << "re-addGeom: " << pitchNb << "x" << sliceNb << " " << pinName << endl;

    while ( _area.size() < sliceNb+1 ) _area.push_back ( Slice() );

    Slice& slice = _area[sliceNb];

    Slice::iterator ipos = slice.find ( pitchNb );
    while ( ipos != slice.end() ) {
      ipos = slice.find ( ++pitchNb );
    }

    slice.insert ( make_pair(pitchNb,PinPosition(pitchNb,pinName)) );

    if ( pitchNb > _coronaPitch ) _coronaPitch = pitchNb;
    if ( sliceNb > _coronaSlice ) _coronaSlice = sliceNb;

    return true;
  }


  bool  Ports::addGeometry ( double x, double y, const char* pinName )
  {
    size_t pitchNb = (size_t)floor ( x / LefParser::getCoreSiteWidth () );
    size_t sliceNb = (size_t)floor ( y / LefParser::getCoreSiteHeight() );

  //cerr << "Add Geometry: " << x << "," << y
  //     << " " << pitchNb << "," << sliceNb << " "
  //     << pinName << endl;

    while ( _area.size() < sliceNb+1 ) _area.push_back ( Slice() );

    Slice& slice = _area[sliceNb];

    Slice::iterator ipos = slice.find ( pitchNb );
    while ( ipos != slice.end() ) {
      ipos = slice.find ( ++pitchNb );
    }

    slice.insert ( make_pair(pitchNb,PinPosition(pitchNb,pinName)) );

    if ( pitchNb > _coronaPitch ) _coronaPitch = pitchNb;
    if ( sliceNb > _coronaSlice ) _coronaSlice = sliceNb;

    return true;
  }


  void  Ports::normalize ()
  {
  // Check that no terminal is outside the AbutmentBox.
  // The corona must be inside the AB. If not, shrink coordinates.
    if ( _coronaPitch >= _parser->getPitchs() ) {
      cerr << Warning("Some terminals of <%s> are outside the X abutment-box (shrinkeds, "
                      "X corona %zd vs. %zd)."
                     ,getString(_parser->getCell()->getName()).c_str()
                     ,_coronaPitch
                     ,_parser->getPitchs()
                     ) << endl;

      double xshrinkFactor = (double)(_parser->getPitchs()-1) / (double)_coronaPitch;

      for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
        if ( _area[islice].empty() ) continue;
        if ( _area[islice].size () == _parser->getPitchs() ) continue;

        Slice::iterator ipos = _area[islice].begin();
        for ( ; ipos != _area[islice].end() ; ++ipos ) {
          PinPosition& position = (*ipos).second;

          if ( position.first == 0 )
            position.first = 1;
          else
            position.first = (size_t)round( xshrinkFactor * (double)position.first );
        }
      }
    }

  // Re-inserting pins to in-range slices.
    if ( _coronaSlice > _parser->getSlices()-1 ) {
      cerr << Warning("Terminals of block <%s> are outside the Y abutment-box (shrinkeds, "
                      "Y corona %zd vs. %zd)."
                     ,getString(_parser->getCell()->getName()).c_str()
                     ,_coronaSlice
                     ,_parser->getSlices()
                     ) << endl;

      double yshrinkFactor = (double)(_parser->getSlices()-1) / (double)_coronaSlice;

      for ( size_t islice=_parser->getSlices() ; islice<_area.size() ; ++islice ) {
        if ( _area[islice].empty() ) continue;

        Slice::iterator ipos = _area[islice].begin();
        for ( ; ipos != _area[islice].end() ; ++ipos ) {
          PinPosition& position = (*ipos).second;

          size_t isliceShrink = (size_t)round( yshrinkFactor * (double)islice );
          addGeometry ( position.first, isliceShrink, position.second.c_str() );
        }
      }

      _area.resize ( _parser->getSlices() );
    }

    if ( _coronaSlice < _parser->getSlices()-1 ) {
      cerr << Warning("Terminals of block <%s> are inside the Y abutment-box (shrinkeds, "
                      "Y corona %zd vs. %zd)."
                     ,getString(_parser->getCell()->getName()).c_str()
                     ,_coronaSlice
                     ,_parser->getSlices()
                     ) << endl;
      size_t missings = _parser->getSlices()-1-_coronaSlice;
      for ( ; missings > 0 ; --missings )
        _area.insert ( ++_area.begin(), Slice() );
    }

  // Resolving overlaps
    for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
      if ( _area[islice].empty() ) continue;
      if ( _area[islice].size () == _parser->getPitchs() ) continue;
      
    // Fisrt pass: unoverlap from left to right.
      size_t          lastx = 0;
      Slice::iterator ipos = _area[islice].begin();
      for ( ; ipos != _area[islice].end() ; ++ipos ) {
        PinPosition& position = (*ipos).second;

        if ( position.first <= lastx ) {
        // Shift right overlapping terminals.
          ++lastx;
        //cerr << "  shift right: " << position.second << " "
        //     << position.first << " -> " << lastx << endl; 
          position.first = lastx;
        } else {
        //cerr << "  unmoved: " << position.second << " "
        //     << position.first << endl;
          lastx = position.first;
        }
      }
      
    // Second pass: unoverlap from right to left.
      lastx = _parser->getPitchs();
      Slice::reverse_iterator ripos = _area[islice].rbegin();
      for ( ; ripos != _area[islice].rend() ; ++ripos ) {
        PinPosition& position = (*ripos).second;

        if ( position.first >= lastx ) {
        // Shift right overlapping terminals.
        //cerr << "  shift left: " << position.second << " "
        //     << position.first << " <- " << lastx-1 << endl; 
          position.first = lastx-1;
        }
        
        lastx = position.first;
      }
    }

    if ( _parser->getSlices() > 1 ) {
    // Block case.
      for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
        if ( _area[islice].empty() ) continue;
        PinPosition& pos = (*_area[islice].rbegin()).second;

        if ( pos.first < _parser->getPitchs()-1 ) {
          pos.first = _parser->getPitchs()-1;
        }
      }

    // Compact extraneous rows.
      bool canCompact = true;
      while ( canCompact and (_area.size() > _parser->getSlices()) ) {
        canCompact = false;

        Area::iterator islice = _area.begin();
        for ( ; islice != _area.end() ; ++islice ) {
          if ( (*islice).empty() ) {
            _area.erase ( islice );
            canCompact = true;
            break;
          }
        }
      }
    }
  }


  void  Ports::makeTerminals ()
  {
    Technology* technology    = DataBase::getDB()->getTechnology();
    Layer*      METAL1        = technology->getLayer ( "METAL1" );
    Box         abutmentBox   ( _parser->getCell()->getAbutmentBox() );
    
  //abutmentBox.inflate ( DbU::lambda(1.0) );

    for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
      if ( _area[islice].empty() ) continue;

      DbU::Unit ymin = LefParser::getSliceHeight()* islice    + LefParser::getPitchWidth()*2;
      DbU::Unit ymax = LefParser::getSliceHeight()*(islice+1) - LefParser::getPitchWidth()*2;
      
      Slice::iterator ipos = _area[islice].begin();
      for ( ; ipos != _area[islice].end() ; ++ipos ) {
        PinPosition& pos = (*ipos).second;

      //cerr << "    Net: " << toLower(pos.second.c_str())
      //     << " [" << pos.first << "] of " << _parser->getCell()->getName() << endl;

        Net* net = Net::create ( _parser->getCell(), toLower(pos.second.c_str()) );
        net->setExternal ( true );

        Segment* segment = Vertical::create ( net
                                            , METAL1
                                            , LefParser::getPitchWidth()*pos.first
                                            , DbU::lambda(1.0)
                                            , ymin
                                            , ymax
                                            );
        NetExternalComponents::setExternal ( segment );

        if ( not abutmentBox.contains(segment->getBoundingBox()) ) {
          cerr << Error("Terminal <%s> of <%s> is outside abutment box."
                       ,getString(net->getName()).c_str()
                       ,getString(_parser->getCell()->getName()).c_str()
                       ) << endl;
        }
      }
    }
  }


  void  Ports::makeBlockages ()
  {
#if BLOCKAGES_ENABLEDS
    Technology* technology   = DataBase::getDB()->getTechnology();
    Layer*      BLOCKAGE2    = technology->getLayer ( "BLOCKAGE2" );
    Layer*      BLOCKAGE3    = technology->getLayer ( "BLOCKAGE3" );
    Cell*       cell         = _parser->getCell ();
    size_t      slices       = _parser->getSlices ();
    size_t      pitchs       = _parser->getPitchs ();
    DbU::Unit   pitch        = LefParser::getPitchWidth  ();
    DbU::Unit   sliceHeight  = LefParser::getSliceHeight ();

    if ( _parser->getSlices() < 2 ) return;

    DbU::Unit axis  = (sliceHeight * (slices-2)) / 2 + sliceHeight;
    DbU::Unit width =  sliceHeight * (slices-2);

    Net* net = Net::create ( cell, "blockageNet" );

    Horizontal::create ( net
                       , BLOCKAGE2
                       , axis
                       , width
                       , pitch * 2
                       , pitch * (pitchs-2)
                       );

    axis  = (pitch * (pitchs-4)) / 2 + 2*pitch;
    width =  pitch * (pitchs-4);

    Vertical::create ( net
                     , BLOCKAGE3
                     , axis
                     , width
                     , sliceHeight
                     , sliceHeight * (slices-1)
                     );
#endif
  }


  void  Ports::clear ()
  {
    _area.clear ();
    _coronaPitch = 0;
    _coronaSlice = 0;
  }


  void  Ports::toStream ( ostream& out )
  {
    for ( size_t islice=0 ; islice<_area.size() ; ++islice ) {
      out << "[" << setw(3) << islice << "]: ";
      
      Slice::iterator ipos = _area[islice].begin ();
      for ( ; ipos != _area[islice].end() ; ++ipos ) {
        out << (*ipos).first << ":" << (*ipos).second.second << " ";
      }

      out << endl;
    }
  }


  const double       LefParser::_coreSiteWidth  = 0.01;
  const double       LefParser::_coreSiteHeight = 0.16;
  DbU::Unit          LefParser::_sliceHeight    = 0;
  DbU::Unit          LefParser::_pitchWidth     = 0;
  AllianceFramework* LefParser::_framework      = NULL;
  Name               LefParser::_padMacro       = "";
        


  LefParser::LefParser ( string& file, AllianceLibrary* library )
    : _file    (file)
    , _library (library)
    , _cell    (NULL)
    , _pitchs  (0)
    , _slices  (0)
    , _readPins(0)
    , _ports   (this)
    , _errors  ()
  {
    _sliceHeight = DbU::lambda ( 50.0 );
    _pitchWidth  = DbU::lambda (  5.0 );

    lefrInit ();
    lefrSetMacroCbk      ( _macroCbk );
    lefrSetPinCbk        ( _pinCbk );
  }


  LefParser::~LefParser ()
  {
    lefrReset ();
  }


         double             LefParser::getCoreSiteWidth  () { return _coreSiteWidth; }
         double             LefParser::getCoreSiteHeight () { return _coreSiteHeight; }
         DbU::Unit          LefParser::getSliceHeight    () { return _sliceHeight; }
         DbU::Unit          LefParser::getPitchWidth     () { return _pitchWidth; }
         AllianceFramework* LefParser::getFramework      () { return _framework; }
         bool               LefParser::hasPadMacro       () { return _padMacro!=Name(); }
         bool               LefParser::isPadMacro        ( const char* modelName ) { return _padMacro==Name(modelName); }
         bool               LefParser::isPadMacro        ( const string& modelName ) { return _padMacro==Name(modelName); }
         void               LefParser::setPadMacro       ( const string& modelName ) { _padMacro=modelName; }
  inline bool               LefParser::hasErrors         () { return not _errors.empty(); }
  inline AllianceLibrary*   LefParser::getLibrary        () { return _library; }
  inline Cell*              LefParser::getCell           () { return _cell; }
  inline size_t             LefParser::getPitchs         () const { return _pitchs; }
  inline size_t             LefParser::getSlices         () const { return _slices; }
  inline size_t             LefParser::getReadPins       () const { return _readPins; }
  inline Ports&             LefParser::getPorts          () { return _ports; }
  inline vector<string>&    LefParser::getErrors         () { return _errors; }
  inline void               LefParser::pushError         ( const string& error ) { _errors.push_back(error); }
  inline void               LefParser::clearErrors       () { return _errors.clear(); }
  inline void               LefParser::clearPorts        () { return _ports.clear(); }
  inline void               LefParser::resetReadPins     () { _readPins=0; }
  inline void               LefParser::setPitchs         ( size_t pitchs ) { _pitchs=pitchs; }
  inline void               LefParser::setSlices         ( size_t slices ) { _slices=slices; }
  inline void               LefParser::incReadPins       () { ++_readPins; }


  int  LefParser::flushErrors ( const string& macroName )
  {
    int code = (hasErrors()) ? 1 : 0;

    for ( size_t ierror=0 ; ierror < _errors.size() ; ++ierror ) {
        string message = "Iccad04Lefdef::load(): " + _errors[ierror];
        cerr << Error(message.c_str(),macroName.c_str()) << endl;
    }
    clearErrors ();

    return code;
  }


  void  LefParser::makeBlock ()
  {
    size_t  perimeter = _readPins + (_readPins%2);

    _slices = 4;
    _pitchs = (perimeter - 8) + 1;

  //cerr << "_slices: " << _slices << endl;
  //cerr << "_pitchs: " << _pitchs << endl;

    vector<string> pinNames;
    _ports.copyPinNames ( pinNames );
    _ports.clear ();

    size_t bottomCount = _pitchs/2;
    size_t rightCount  = bottomCount + _slices;
    size_t topCount    = rightCount  + _pitchs/2;

    for ( size_t ipin=0 ; ipin<pinNames.size() ; ++ipin ) {
    //cerr << "ipin: " << ipin << endl;
      if ( ipin < bottomCount ) {
      // bottom horizontal row.
        _ports.addGeometry ( 1+ipin*2, 0 , pinNames[ipin].c_str() );
      } else if ( ipin < rightCount ) {
      // right vertical row.
        _ports.addGeometry ( _pitchs-1, ipin-bottomCount, pinNames[ipin].c_str() );
      } else if ( ipin < topCount ) {
      // top horizontal row.
        _ports.addGeometry ( 1+(ipin-rightCount)*2, _slices-1, pinNames[ipin].c_str() );
      } else {
      // left vertical row.
        _ports.addGeometry ( 0, ipin-topCount, pinNames[ipin].c_str() );
      }
    }
  //cerr << "makeBlock finished." << endl;
  }


  Cell* LefParser::_createCell ( const char* name )
  {
    _cell = LefParser::getFramework()->createCell ( toLower(name), _library );

    Box  abutmentBox ( 0
                     , 0
                     , _pitchs*LefParser::getPitchWidth ()
                     , _slices*LefParser::getSliceHeight()
                     );
    _cell->setAbutmentBox ( abutmentBox );
    addSupplyNets ( _cell );

    Technology* technology = DataBase::getDB()->getTechnology();
    Layer*      METAL1     = technology->getLayer ( "METAL1" );
    size_t      slices     = (size_t)_slices;
    DbU::Unit   vssWidth   = DbU::lambda(6.0);
    DbU::Unit   vddWidth   = DbU::lambda(6.0);
    DbU::Unit   vssAxis;
    DbU::Unit   vddAxis;

    Net* vss = _cell->getNet ( "vss" );
    Net* vdd = _cell->getNet ( "vdd" );

    for ( size_t islice=0 ; islice<slices ; ++islice ) {
      if ( islice % 2 ) {
        vssAxis = DbU::lambda( (islice+1)*50.0 - 3.0 );
        vddAxis = DbU::lambda(  islice   *50.0 + 3.0 );
      } else {
        vssAxis = DbU::lambda(  islice   *50.0 + 3.0 );
        vddAxis = DbU::lambda( (islice+1)*50.0 - 3.0 );
      }

      Segment* segment = Horizontal::create ( vss
                                            , METAL1
                                            , abutmentBox.getYMin()+vssAxis
                                            , vssWidth
                                            , abutmentBox.getXMin()
                                            , abutmentBox.getXMax()
                                            );
      NetExternalComponents::setExternal ( segment );

      segment = Horizontal::create ( vdd
                                   , METAL1
                                   , abutmentBox.getYMin()+vddAxis
                                   , vddWidth
                                   , abutmentBox.getXMin()
                                   , abutmentBox.getXMax()
                                   );
      NetExternalComponents::setExternal ( segment );
    }

    return _cell;
  }


  int  LefParser::_macroCbk ( lefrCallbackType_e c, lefiMacro* macro, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;

    // parser->getPorts().toStream ( cerr );
    string macroName = toLower(macro->name());

    if ( not macro->hasSize() ) {
      cerr << Error("Iccad04Lefdef::load(): Macro <%s> doesn't have size.",macroName.c_str()) << endl;
      return 0;
    }

    if ( (macro->originX() != 0.0) or (macro->originY() != 0.0) )
      throw Error("Iccad04Lefdef::load(): Macro <%s> is not at (0,0) origin (%.2f,%.2f)."
                 ,macroName.c_str()
                 ,macro->originX()
                 ,macro->originY()
                 );

    parser->setPitchs ( (size_t)round ( macro->sizeX() / LefParser::getCoreSiteWidth () ) );
    parser->setSlices ( (size_t)round ( macro->sizeY() / LefParser::getCoreSiteHeight() ) );

    if ( (parser->getSlices() == 0) and not LefParser::hasPadMacro() ) {
      if ( abs(macro->sizeY() - LefParser::getCoreSiteWidth()) < 1e-9 ) {
        parser->setPadMacro ( macroName );
        parser->clearPorts();
        cmess2 << "     - Pad/Macro (1x1): " << macroName << endl;
        return 0;
      }
    } 

    if ( abs((double)parser->getPitchs()*LefParser::getCoreSiteWidth() - macro->sizeX()) > 1e-9 ) {
      parser->_ports.clear ();
      cerr << Error("Iccad04Lefdef::load(): Macro <%s> doesn't have a pitched size, %.2f."
                   ,macroName.c_str(),macro->sizeX()) << endl;
      return 0;
    }

    if ( abs((double)parser->getSlices()*LefParser::getCoreSiteHeight() - macro->sizeY()) > 1e-9 ) {
      parser->clearPorts ();
      cerr << Error("Iccad04Lefdef::load(): Macro <%s> doesn't have a sliced size, %.2f."
                   ,macroName.c_str(),macro->sizeY()) << endl;
      return 0;
    }

    if ( parser->getReadPins() > parser->getPitchs()-1 ) {
      cerr << Error("Iccad04Lefdef::load(): Macro <%s> has more ports than pitches (%zd vs. %zd)."
                   ,macroName.c_str(),parser->getReadPins(),parser->getPitchs()) << endl;
      if ( parser->getReadPins() > 40 )
        parser->makeBlock ();
      else
        parser->setPitchs ( parser->getReadPins() + 1 );
    }

    parser->flushErrors ( macroName );
    parser->_createCell ( macroName.c_str() );

    if ( parser->getSlices() > 1 ) {
      cmess2 << "     - Macro-block: " << macroName
             << " " << macro->sizeX() << "x" << macro->sizeY()
             << " " << parser->getPitchs() << "x" << parser->getSlices()
             << endl;
    }
 // } else {
 //   cmess2 << "     - Macro: " << macro->name()
 //          << " " << macro->sizeX() << "x" << macro->sizeY()
 //          << " " << parser->getPitchs() << "x" << parser->getSlices()
 //          << endl;
 // }
    
    parser->getPorts().normalize ();
    parser->getPorts().makeTerminals ();
    parser->getPorts().makeBlockages ();
    parser->clearPorts ();
    parser->resetReadPins ();
    return 0;
  }


  int  LefParser::_pinCbk ( lefrCallbackType_e c, lefiPin* pin, lefiUserData ud )
  {
    LefParser* parser = (LefParser*)ud;
    parser->incReadPins ();

    ostringstream message;

    int numPorts = pin->numPorts ();
    for ( int iport=0 ; iport<numPorts ; ++iport ) {
      lefiGeometries* geometries = pin->port ( iport );

      if ( geometries->numItems() > 2 ) {
        message << "Pin <" << pin->name() << "> of <%s> have more than one geometries, "
                << geometries->numItems() << ".";
        parser->pushError ( message.str() );
        message.str ( "" );
      }
      if ( geometries->numItems() == 0 ) {
        message << "Pin <" << pin->name() << "> of <%s> have no geometries.";
        parser->pushError ( message.str() );
        message.str ( "" );
        continue;
      }
      if ( geometries->itemType(1) != lefiGeomRectE ) {
        message << "Geometry [1] of pin <" << pin->name() << "> of <%s> is not a RECT.";
        parser->pushError ( message.str() );
        message.str ( "" );
        continue;
      }

      lefiGeomRect* rect = geometries->getRect(1);

      // cerr << "RECT " << rect->xl << " " << rect->yl << " "  << rect->xh << " " << rect->yh << " " << endl; 
      parser->_ports.addGeometry ( (rect->xl < 0.0) ? 0.0 : rect->xl
                                 , (rect->yl < 0.0) ? 0.0 : rect->yl
                                 , pin->name() );
    }

    
    return 0;
  }


  void  LefParser::parse ( string file )
  {
    cmess1 << "  o  LEF: <" << file << ">" << endl;

    size_t iext = file.rfind ( '.' );
    if ( file.compare(iext,4,".lef") != 0 )
      throw Error ("Iccad04Lefdef::load(): LEF files must have  \".lef\" extension <%s>.",file.c_str());

    _framework = AllianceFramework::get ();

    unsigned int         libraryFlags = 0;
    string               libraryPath  = "./" + file.substr ( 0, file.size()-4 ) + "lib";
    AllianceLibrary*     library      = _framework->createLibrary ( libraryPath, libraryFlags );
    auto_ptr<LefParser>  parser       ( new LefParser(file,library) );

    FILE* lefStream = fopen ( file.c_str(), "r" );
    if ( lefStream == NULL )
      throw Error ("Iccad04Lefdef::load(): Cannot open LEF file <%s>.",file.c_str());

    lefrRead  ( lefStream, file.c_str(), (lefiUserData)parser.get() );

    fclose ( lefStream );

    _framework->saveLibrary ( library );
  }


  class DefParser {
    public:
      static AllianceFramework* getFramework      ();
      static Cell*              parse             ( string file, unsigned int flags );
                                DefParser         ( string& file, AllianceLibrary* );
                               ~DefParser         ();
      inline bool               hasErrors         ();
      inline bool               isPad             ( const string& );
      inline AllianceLibrary*   getLibrary        ();
      inline Cell*              getCell           ();
      inline size_t             getPitchs         () const;
      inline size_t             getSlices         () const;
      inline const Box&         getRowsArea       () const;
      inline vector<string>&    getErrors         ();
      inline void               pushError         ( const string& );
      inline void               addPad            ( const string& );
             int                flushErrors       ();
      inline void               clearErrors       ();
      inline void               clearPads         ();
      inline void               setPitchs         ( size_t );
      inline void               setSlices         ( size_t );
             void               mergeWithRowsArea ( const Box& );
    private:                                      
      static int                _designEndCbk     ( defrCallbackType_e, void*         , defiUserData );
      static int                _dieAreaCbk       ( defrCallbackType_e, defiBox*      , defiUserData );
      static int                _rowCbk           ( defrCallbackType_e, defiRow*      , defiUserData );
      static int                _componentCbk     ( defrCallbackType_e, defiComponent*, defiUserData );
      static int                _componentEndCbk  ( defrCallbackType_e, void*         , defiUserData );
      static int                _netCbk           ( defrCallbackType_e, defiNet*      , defiUserData );
             Cell*              _createCell       ( const char* name );
    private:
      static AllianceFramework* _framework;
             string             _file;
             AllianceLibrary*   _library;
             Cell*              _cell;
             size_t             _pitchs;
             size_t             _slices;
             Box                _rowsArea;
             set<string>        _pads;
             vector<string>     _errors;
  };


  AllianceFramework* DefParser::_framework = NULL;


  DefParser::DefParser ( string& file, AllianceLibrary* library )
    : _file    (file)
    , _library (library)
    , _cell    (NULL)
    , _pitchs  (0)
    , _slices  (0)
    , _rowsArea()
    , _pads    ()
    , _errors  ()
  {
    defrInit               ();
    defrSetDesignEndCbk    ( _designEndCbk );
    defrSetDieAreaCbk      ( _dieAreaCbk );
    defrSetRowCbk          ( _rowCbk );
    defrSetComponentCbk    ( _componentCbk );
    defrSetComponentEndCbk ( _componentEndCbk );
    defrSetNetCbk          ( _netCbk );
    defrSetNetEndCbk       ( _componentEndCbk );
  }


  DefParser::~DefParser ()
  {
    defrReset ();
  }


         AllianceFramework* DefParser::getFramework () { return _framework; }
  inline bool               DefParser::hasErrors    () { return not _errors.empty(); }
  inline bool               DefParser::isPad        ( const string& pad ) { return _pads.find(pad)!=_pads.end(); }
  inline AllianceLibrary*   DefParser::getLibrary   () { return _library; }
  inline Cell*              DefParser::getCell      () { return _cell; }
  inline size_t             DefParser::getPitchs    () const { return _pitchs; }
  inline size_t             DefParser::getSlices    () const { return _slices; }
  inline const Box&         DefParser::getRowsArea  () const { return _rowsArea; }
  inline vector<string>&    DefParser::getErrors    () { return _errors; }
  inline void               DefParser::pushError    ( const string& error ) { _errors.push_back(error); }
  inline void               DefParser::addPad       ( const string& pad ) { _pads.insert(pad); }
  inline void               DefParser::clearErrors  () { return _errors.clear(); }
  inline void               DefParser::clearPads    () { return _pads.clear(); }
  inline void               DefParser::setPitchs    ( size_t pitchs ) { _pitchs=pitchs; }
  inline void               DefParser::setSlices    ( size_t slices ) { _slices=slices; }


  Cell* DefParser::_createCell ( const char* name )
  {
    _cell = DefParser::getFramework()->createCell ( toLower(name), NULL );
    addSupplyNets ( _cell );

    return _cell;
  }


  int  DefParser::flushErrors ()
  {
    int code = (hasErrors()) ? 1 : 0;

    for ( size_t ierror=0 ; ierror < _errors.size() ; ++ierror ) {
        string message = "Iccad04Lefdef::load(): " + _errors[ierror];
        cerr << Error(message.c_str(),getString(_cell->getName()).c_str()) << endl;
    }
    clearErrors ();

    return code;
  }


  void  DefParser::mergeWithRowsArea ( const Box& rowBox )
  {
    _rowsArea.merge(rowBox);
  }


  int  DefParser::_designEndCbk ( defrCallbackType_e c, void*, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    Box rowsArea = parser->getRowsArea();

    if ( not rowsArea.isEmpty() ) {
    // Ensure the rowsArea is an integral number of slices/pitchs.
      DbU::Unit  widthRemainder = rowsArea.getWidth() % LefParser::getPitchWidth();
      if ( widthRemainder != 0 ) widthRemainder -= LefParser::getPitchWidth();

      DbU::Unit  heightRemainder = rowsArea.getWidth() % LefParser::getSliceHeight();
      if ( heightRemainder != 0 ) heightRemainder -= LefParser::getSliceHeight();

      rowsArea.inflate ( 0, 0, -widthRemainder, -heightRemainder );

      if ( parser->getCell()->getAbutmentBox().isEmpty() ) {
        parser->getCell()->setAbutmentBox ( rowsArea );
      } else {
        if ( parser->getCell()->getAbutmentBox().contains(rowsArea ) )
          parser->getCell()->setAbutmentBox ( rowsArea );
      }
    }

    return 0;
  }


  int  DefParser::_dieAreaCbk ( defrCallbackType_e c, defiBox* box, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    DbU::Unit width  = LefParser::getPitchWidth () * (DbU::Unit)(0.01 * (box->xh() - box->xl()) / LefParser::getCoreSiteWidth ());
    DbU::Unit height = LefParser::getSliceHeight() * (DbU::Unit)(0.01 * (box->yh() - box->yl()) / LefParser::getCoreSiteHeight());

    parser->getCell()->setAbutmentBox ( Box(0,0,width,height) );

    return 0;
  }


  int  DefParser::_rowCbk ( defrCallbackType_e c, defiRow* row, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    double rowBoxXMax = row->x();
    double rowBoxYMax = row->y();

    if ( row->hasDo() ) {
      if ( row->hasDoStep() ) {
        rowBoxXMax += (row->xNum() - 1.0) * row->xStep() + LefParser::getCoreSiteWidth () * 100.0;
        rowBoxYMax += (row->yNum() - 1.0) * row->yStep() + LefParser::getCoreSiteHeight() * 100.0;
      } else {
        rowBoxXMax += row->xNum() * LefParser::getCoreSiteWidth () * 100.0;
        rowBoxYMax += row->yNum() * LefParser::getCoreSiteHeight() * 100.0;
      }
    }

  //cerr << "[" << row->x()   << " " << row->y()
  //     << " " << rowBoxXMax << " " << rowBoxYMax << "]" << endl;

    parser->mergeWithRowsArea ( Box(DbU::lambda(row->x())
                                   ,DbU::lambda(row->y())
                                   ,DbU::lambda(rowBoxXMax)
                                   ,DbU::lambda(rowBoxYMax)
                                   ) );

    return 0;
  }


  int  DefParser::_componentCbk ( defrCallbackType_e c, defiComponent* component, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

    string componentName = toLower(component->name());
    string componentId   = toLower(component->id());

    if ( LefParser::isPadMacro(componentName) ) {
      parser->addPad ( componentId );
      return 0;
    }

    Cell* masterCell = DefParser::getFramework()->getCell ( componentName, Catalog::State::Views );
    if ( masterCell == NULL ) {
      ostringstream message;
      message << "Unknown model (LEF MACRO) " << componentName << " in <%s>.";
      parser->pushError ( message.str() );
      return 0;
    }

    Instance::create ( parser->getCell(), componentId, masterCell );

    return 0;
  }


  int  DefParser::_netCbk ( defrCallbackType_e c, defiNet* net, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;

  //cout << "     - Net " << net->name() << endl;

    Net* hnet = Net::create ( parser->getCell(), toLower(net->name()) );

    int numConnections = net->numConnections();
    for ( int icon=0 ; icon<numConnections ; ++icon ) {
      string instanceName = toLower(net->instance(icon));
      string pinName      = toLower(net->pin(icon));

      if ( parser->isPad(net->instance(icon)) ) {
        hnet->setExternal ( true );
        continue;
      }

      Instance* instance = parser->getCell()->getInstance ( instanceName );
      if ( instance == NULL ) {
        ostringstream message;
        message << "Unknown instance (DEF COMPONENT) " << instanceName << " in <%s>.";
        parser->pushError ( message.str() );
        continue;
      }

      Net* masterNet = instance->getMasterCell()->getNet ( pinName );
      if ( masterNet == NULL ) {
        ostringstream message;
        message << "Unknown PIN <" << pinName << "> in instance <"
                << instanceName << "> of <" << instance->getMasterCell()->getName()
                << "> (LEF MACRO) in <%s>.";
        parser->pushError ( message.str() );
        continue;
      }

      instance->getPlug(masterNet)->setNet(hnet);
    }

    return 0;
  }


  int  DefParser::_componentEndCbk ( defrCallbackType_e c, void*, lefiUserData ud )
  {
    DefParser* parser = (DefParser*)ud;
    return parser->flushErrors ();
  }


  Cell* DefParser::parse ( string file, unsigned int flags )
  {
    cmess1 << "  o  DEF: <" << file << ">" << endl;

    size_t iext = file.rfind ( '.' );
    if ( file.compare(iext,4,".def") != 0 )
      throw Error ("Iccad04Lefdef::load(): DEF files must have  \".def\" extension <%s>.",file.c_str());

    _framework = AllianceFramework::get ();

    unsigned int        libraryFlags = 0;
    size_t              islash       = file.rfind  ( '/' );
    string              designName   = file.substr ( ((islash == string::npos) ? 0 : islash), file.size()-4 );
    AllianceLibrary*    library      = _framework->getAllianceLibrary ( designName+"lib", libraryFlags );
    auto_ptr<DefParser> parser       ( new DefParser(file,library) );

    FILE* defStream = fopen ( file.c_str(), "r" );
    if ( defStream == NULL )
      throw Error ("Iccad04Lefdef::load(): Cannot open DEF file <%s>.",file.c_str());

    parser->_createCell ( designName.c_str() );
    defrRead  ( defStream, file.c_str(), (defiUserData)parser.get(), 1 );

    fclose ( defStream );

    return parser->getCell();
  }


} // End of anonymous namespace.

#endif // HAVE_LEFDEF.


namespace CRL {

  using std::string;
  using std::cerr;
  using std::endl;
  using Hurricane::Library;
  using Hurricane::Transformation;
  using Hurricane::UpdateSession;


  Cell* Iccad04Lefdef::load ( string benchmark, unsigned int flags )
  {
    Cell* cell = NULL;

#if HAVE_LEFDEF
    UpdateSession::open ();

    LefParser::parse ( benchmark+".lef" );
    cell = DefParser::parse ( benchmark+".def", flags );

    UpdateSession::close ();
#else
    cerr << "[ERROR] CRL::Iccad04Lefdef::load(): \n"
         << "  Coriolis2 hasn't been compiled with LEF/DEF support. To enable LEF/DEF\n"
         << "  support, you may obtain parser/driver from Si2 (www.si2.org) then recompile."
         << endl;
#endif

    return cell;
  }


}  // End of CRL namespace.
