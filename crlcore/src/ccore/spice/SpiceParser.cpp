// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           S P I C E  / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./spice/SpiceParser.cpp"                      |
// +-----------------------------------------------------------------+


#include <cctype>
#include <ctime>
#include <cstdio>
#include <string>
#include <bitset>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <list>
using namespace std;

#include "hurricane/configuration/Configuration.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
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
#include "crlcore/Spice.h"
#include "crlcore/SpiceBit.h"
#include "crlcore/SpiceEntity.h"
using namespace CRL;


namespace {

  using namespace std;
  using CRL::NamingScheme;


  string& uppercase ( string& s )
  {
    for ( size_t i=0 ; i<s.size() ; ++i )
      s[i] = toupper( s[i] );
    return s;
  }


// -------------------------------------------------------------------
// Class  :  "::Tokenize".


  class Tokenize {
    public:
      static const uint32_t  COMMENT          = (1<< 0);
      static const uint32_t  END              = (1<< 1);
      static const uint32_t  RESISTOR         = (1<< 2);
      static const uint32_t  CAPACITOR        = (1<< 3);
      static const uint32_t  INDUCTOR         = (1<< 4);
      static const uint32_t  COUPLED_INDUCTOR = (1<< 5);
      static const uint32_t  DIODE            = (1<< 6);
      static const uint32_t  BJT              = (1<< 7);
      static const uint32_t  JFET             = (1<< 8);
      static const uint32_t  MOSFET           = (1<< 9);
      static const uint32_t  MODEL            = (1<< 0);
      static const uint32_t  SUBCKT           = (1<<10);
      static const uint32_t  ENDS             = (1<<11);
      static const uint32_t  SUBCIRCUIT       = (1<<12);
    public:
                                   Tokenize   ( string blifFile );
      inline size_t                lineno     () const;
      inline uint32_t              flags      () const;
             list<string>&         tokens     ();
             bool                  readEntry  ();
    private:                                  
             bool                  _readline  ();
    private:
      size_t        _lineno;
      uint32_t      _flags;
      ifstream      _stream;
      list<string>  _tokens;
  };


  Tokenize::Tokenize ( string spiceFile )
    : _lineno    (0)
    , _flags     (0)
    , _stream    ()
    , _tokens    ()
  { 
    _stream.open( spiceFile );
    if (_stream.fail())
      throw Error( "Unable to open SPICE file %s\n", spiceFile.c_str() );
  }


  inline size_t        Tokenize::lineno () const { return (_lineno) ? _lineno-1 : 0; }
  inline uint32_t      Tokenize::flags  () const { return _flags; }
  inline list<string>& Tokenize::tokens ()       { return _tokens; }


  bool  Tokenize::readEntry ()
  {
    _flags = 0;

    if (_stream.eof()) return false;

    if (not _readline()) return false;
    string first = _tokens.front();
    uppercase( first );
    if      (first[0] == '*' ) { _flags |= COMMENT;   }
    else if (first[0] == '.' ) {
      if      (first == ".MODEL"  ) { _flags |= MODEL; }
      else if (first == ".END"    ) { _flags |= END; }
      else if (first == ".SUBCKT" ) { _flags |= SUBCKT; }
      else if (first == ".ENDS"   ) { _flags |= ENDS; }
    }
    else if (first[0] == 'R') { _flags |= RESISTOR; }
    else if (first[0] == 'C') { _flags |= CAPACITOR; }
    else if (first[0] == 'L') { _flags |= INDUCTOR; }
    else if (first[0] == 'K') { _flags |= COUPLED_INDUCTOR; }
    else if (first[0] == 'D') { _flags |= DIODE; }
    else if (first[0] == 'Q') { _flags |= BJT; }
    else if (first[0] == 'J') { _flags |= JFET; }
    else if (first[0] == 'M') { _flags |= MOSFET; }
    else if (first[0] == 'X') { _flags |= SUBCIRCUIT; }

    return true;
  }


  bool  Tokenize::_readline ()
  {
    _tokens.clear();

    bool nextLine = true;

    while ( nextLine ) {
      if (_stream.eof()) return false;

      nextLine = false;
      ++_lineno;

      string line;
      getline( _stream, line );

      size_t tokstart = 0;
      for ( size_t i=0 ; i<line.size() ; ++i ) {
        if ((i == 0) and (line[i] == '*')) {
          _tokens.push_back( line );
          return true;
        }

        size_t nextTokstart = i+1;
        switch ( line[i] ) {
          case ' ':
          case '\t': break;
          default:   continue;
        }
        if (i > tokstart)
          _tokens.push_back( line.substr(tokstart,i-tokstart) );
        tokstart = nextTokstart;
      }

      if (tokstart < line.size()) {
        if (line.substr(tokstart) == ";")
          nextLine = true;
        else
          _tokens.push_back( line.substr(tokstart) );
      }

      if (_tokens.empty())
        nextLine = true;
    }

    return not _tokens.empty();
  }


}  // Anonymous namespace.


namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::Spice".

  bool  Spice::load ( Library* library, string spiceFile, uint64_t mode )
  {
    if (mode != PIN_ORDERING) {
      cerr << Error( "Spice::load(): SPICE parser only support PIN_ORDERING mode.\n"
                     "        \"%s\"."
                   , spiceFile.c_str()
                   ) << endl;
      return false;
    }

  //DebugSession::open( 101, 110 );
    UpdateSession::open();

    Tokenize tokenize ( spiceFile );

    while ( tokenize.readEntry() ) {
      if (tokenize.flags() & Tokenize::SUBCKT) {
        list<string>& tokens = tokenize.tokens();
        // cerr << "Found SUBCKT:";
        // for ( const string& item : tokens )
        //   cerr << " " << item;
        // cerr << endl;

        if (tokens.size() < 2) {
          cerr << Error( "Spice::load(): Invalid SUBCKT, no parameters at all.\n"
                         "        File %s at line %u."
                       , spiceFile.c_str()
                       , tokenize.lineno()
                       ) << endl;
          continue;
        }

        string cellName = *(next( tokens.begin(), 1));
        Cell*  cell     = library->getCell( cellName );
        if (not cell) {
          cerr << Error( "Spice::load(): Library \"%s\" has no Cell named \"%s\".\n"
                         "        File %s at line %u."
                       , getString( library->getName() ).c_str()
                       , getString( cellName ).c_str()
                       , spiceFile.c_str()
                       , tokenize.lineno()
                       ) << endl;
          continue;
        }

        tokens.pop_front();
        tokens.pop_front();
        // cerr << "Net order of " << cell;
        // for ( auto& name : tokens )
        //   cerr << " " << name;
        // cerr << endl;
        bool         hasErrors = false;
        vector<Net*> orderedNets;
        for ( const string& netName : tokens ) {
          Net* net = cell->getNet( netName );
          if (not net) {
            cerr << Error( "Spice::load(): Cell \"%s\" has no Net \"%s\".\n"
                           "        File %s at line %u."
                         , getString( cell->getName() ).c_str()
                         , netName.c_str()
                         , spiceFile.c_str()
                         , tokenize.lineno()
                         ) << endl;
            hasErrors = true;
            continue;
          }
          if (not net->isExternal()) {
            cerr << Error( "Spice::load(): In cell \"%s\", net \"%s\" is *not* external.\n"
                           "        File %s at line %u."
                         , getString( cell->getName() ).c_str()
                         , getString( net ->getName() ).c_str()
                         , spiceFile.c_str()
                         , tokenize.lineno()
                         ) << endl;
            hasErrors = true;
            continue;
          }
          orderedNets.push_back( net );
        }
        map< string, Net*, greater<string> >  internalNets;
        for ( Net* net : cell->getNets() ) {
          if (net->isExternal()) continue;
          internalNets.insert( make_pair( getString(net->getName()), net ) );
        }
        for ( auto item : internalNets ) {
          orderedNets.push_back( item.second );
        }
        // for ( Net* net : orderedNets )
        //   cerr << " " << net->getName();
        // cerr << endl;

        for ( Net* net : cell->getExternalNets() ) {
          if (find( orderedNets.begin(), orderedNets.end(), net) == orderedNets.end()) {
            cerr << Error( "Spice::load(): In cell \"%s\", external net \"%s\" *not* ordered.\n"
                           "        File %s at line %u."
                         , getString( cell->getName() ).c_str()
                         , getString( net ->getName() ).c_str()
                         , spiceFile.c_str()
                         , tokenize.lineno()
                         ) << endl;
            hasErrors = true;
            break;
          }
        }
        if (hasErrors) continue;

        ::Spice::Entity* spiceEntity = ::Spice::EntityExtension::get( cell );
        if (spiceEntity) {
          if (spiceEntity->getFlags() & ::Spice::Entity::ReferenceCell) {
            cerr << Warning( "Spice::load(): Redefinition of external net order of \"%s\".\n"
                             "          (from file: \"%s\")"
                           , getString( cell->getName() ).c_str()
                           , spiceFile.c_str()
                           ) << endl;
          } else {
            spiceEntity->setFlags( ::Spice::Entity::ReferenceCell );
          }
        } else {
          spiceEntity = ::Spice::EntityExtension::create( cell, ::Spice::Entity::ReferenceCell );
        }
        spiceEntity->setOrder( orderedNets );
      }
    }

    UpdateSession::close();
  //DebugSession::close();

    return true;
  }


}  // CRL namespace.
