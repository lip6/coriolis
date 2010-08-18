
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ParserDriver.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#include  "hurricane/DBo.h"

#include  "crlcore/Utilities.h"
#include  "crlcore/Environment.h"
#include  "crlcore/Catalog.h"
#include  "crlcore/ParsersDrivers.h"
#include  "Ap.h"
#include  "Vst.h"
#include  "Spice.h"
#include  "Bookshelf.h"
#include  "AcmSigda.h"
#include  "openaccess/OpenAccess.h"


namespace {


// Error Messages.
  const char* BadSlotIterator = "%s:\n\n"
                                "    Attempt to use an unitialised iterator. Must be set up before\n"
                                "    with either ::lbegin() or ::cbegin().\n";
  const char* DupParser       = "%s:\n\n"
                                "    Attempt to overwrite registered parser for format \"%s\"\n"
                                "    and extention \"%s\".\n";

} // End of anonymous namespace.




namespace CRL {


// x-----------------------------------------------------------------x
// |                     Variables Definitions                       |
// x-----------------------------------------------------------------x

  const char* BadParserType =
    "%s:\n\n"
    "    No registered parser avalaible to load cell \"%s\"\n"
    "    in format \"%s\".\n";
  const char* BadDriverType =
    "%s:\n\n"
    "    No registered driver avalaible to save cell \"%s\"\n"
    "    in format \"%s\".\n";
  const char* BadInputMode =
    "%s:\n\n"
    "    Invalid loading mode requested for cell \"%s\".\n"
    "    (neither logical or physical has been set)\n";
  const char* BadOutputMode =
    "%s:\n\n"
    "    Invalid save mode requested for cell \"%s\".\n"
    "    (neither logical or physical has been set)\n";


// x-----------------------------------------------------------------x
// |               Methods of Class "ParserFormatSlot"               |
// x-----------------------------------------------------------------x


  bool  ParserFormatSlot::cend () {
    if ( !_itset )
      throw Error ( BadSlotIterator, "ParserFormatSlot::cend()" );
    
    return _it == _cells.end();
  }


  bool  ParserFormatSlot::lend ()
  {
    if ( !_itset )
      throw Error ( BadSlotIterator, "ParserFormatSlot::lend()" );

    return _it == _libs.end();
  }


  void  ParserFormatSlot::operator++ (int)
  {
    if ( !_itset )
      throw Error ( BadSlotIterator, "ParserFormatSlot::operator++()" );

    _it++;
  }


  void  ParserFormatSlot::registerCell ( const string& tag, CellParser_t* p, const string& ext )
  {
    ParserSlot      slot ( tag, ext, (void*)p );
    ParserSlotIter  it   = _cells.begin();

    for ( ; it != _cells.end() ; it++ ) {
      if ( it->_ext == slot._ext )
        throw Error ( DupParser, "ParserFormatSlot::registerCell()", tag.c_str(), ext.c_str() );
    }

    _cells.push_back ( slot );
    cbegin();
  }


  void  ParserFormatSlot::registerLib ( const string& tag, LibraryParser_t* p, const string& ext )
  {
    ParserSlot      slot ( tag, ext, (void*)p );
    ParserSlotIter  it   = _libs.begin();

    for ( ; it != _libs.end() ; it++ ) {
      if ( it->_ext == slot._ext )
        throw Error ( DupParser, "ParserFormatSlot::registerLib()", tag.c_str(), ext.c_str() );
    }

    _libs.push_back ( slot );
    lbegin();

    _loadByLib = true;
  }


  bool ParserFormatSlot::unRegisterCell ( const Name& ext )
  {
    ParserSlotIter  it = _cells.begin();

    for ( ; it != _cells.end() ; it++ ) {
      if ( it->_ext == ext ) {
        _cells.erase ( it );
        cbegin ();
        return true;
      }
    }

    cbegin ();
    return false;
  }


  bool ParserFormatSlot::unRegisterLib ( const Name& ext )
  {
    ParserSlotIter  it = _libs.begin();

    for ( ; it != _libs.end() ; it++ ) {
      if ( it->_ext == ext ) {
        _libs.erase ( it );
        if ( _libs.size() == 0 ) _loadByLib = false;
      
        lbegin ();
        return true;
      }
    }

    lbegin ();
    return false;
  }


// x-----------------------------------------------------------------x
// |                  Methods of Class "ParsersMap"                  |
// x-----------------------------------------------------------------x


  ParsersMap::ParsersMap (): map<Name,ParserFormatSlot>()
  {
    // Register the Alliance default parsers.
    registerSlot ( "ap"   , (CellParser_t*)apParser       , "ap"  );
    registerSlot ( "vst"  , (CellParser_t*)vstParser      , "vst" );
    registerSlot ( "vst"  , (CellParser_t*)vstParser      , "vbe" );
    registerSlot ( "spi"  , (CellParser_t*)spiceParser    , "spi" );
    registerSlot ( "oa"   , (CellParser_t*)OpenAccess::oaCellParser  , "oa" );
  //registerSlot ( "oa"   , (LibraryParser_t*)OpenAccess::oaLibParser, "oa" );
    registerSlot ( "aux"  , (CellParser_t*)bookshelfParser, "aux" );
    registerSlot ( "bench", (CellParser_t*)acmSigdaParser , "bench" );
  }


  ParserFormatSlot& ParsersMap::getParserSlot ( const string& tag )
  {
    ParserFormatSlot* slot;
    iterator          it;

    if ( ( it = find(Name(tag)) ) != end() ) {
      slot = & ( it->second );
    } else {
      (*this)[tag] = ParserFormatSlot ();

      // Needed as the slot is copied into the map.
      // (a container own is elements)
      slot = &( (*this)[tag] );
    }

    return *slot;
  }


  ParserFormatSlot &ParsersMap::getParserSlot ( const string&      name
                                              , unsigned int       mode
                                              , const Environment& environment )
  {
  // Look for the parser tag (i.e. file extention).
    Name format;
    switch ( mode & (Catalog::State::Views) ) {
      case Catalog::State::Physical: format = environment.getIN_PH(); break;
      case Catalog::State::Logical:  format = environment.getIN_LO(); break;
      default:
        throw Error ( BadInputMode, "ParserMap::getParserSlot()", getString(name).c_str() );
    }

    iterator  it = find ( format );
    if ( it == end() )
      throw Error ( BadParserType
                  , "ParserMap::getParserSlot()"
                  , getString(name).c_str() 
                  , getString(format).c_str() );

    return it->second;
  }


  DriverSlot &DriversMap::getDriverSlot ( const string&      name
                                        , unsigned int       mode
                                        , const Environment& environment )
  {
  // Look for the parser tag (i.e. file extention).
    Name format;
    switch ( mode & (Catalog::State::Views) ) {
      case Catalog::State::Physical: format = environment.getOUT_PH(); break;
      case Catalog::State::Logical:  format = environment.getOUT_LO(); break;
      default:
        throw Error ( BadOutputMode, "DriverMap::getDriverSlot()", getString(name).c_str() );
    }

    iterator  it = find ( format );
    if ( it == end() )
      throw Error ( BadDriverType
                  , "DriverMap::getDriverSlot()"
                  , getString(name).c_str()
                  , getString(format).c_str() );

    return it->second;
  }


  void  ParsersMap::registerSlot ( const string& tag, CellParser_t* p, const string& ext )
  {
    ParserFormatSlot& slot = getParserSlot ( tag );
  
    slot.registerCell ( tag, p, ext );
  }


  void  ParsersMap::registerSlot ( const string& tag, LibraryParser_t* p, const string& ext )
  {
    ParserFormatSlot& slot = getParserSlot ( tag );
  
    slot.registerLib ( tag, p, ext );
  }


  void  ParsersMap::unRegisterSlot ( const Name& tag, const Name& ext, bool lib )
  {
    iterator  it = find(tag);

    if ( it == end() ) return;

    if ( lib ) it->second.unRegisterLib  ( ext );
    else       it->second.unRegisterCell ( ext );
  }




// x-----------------------------------------------------------------x
// |                  Methods of Class "DriversMap"                  |
// x-----------------------------------------------------------------x


  DriversMap::DriversMap () : map<Name,DriverSlot>()
  {
  // Register the Alliance default drivers.
    registerSlot ( "ap" , (CellDriver_t*)apDriver       , "ap"       );
    registerSlot ( "vst", (CellDriver_t*)vstDriver      , "vst"      );
  //registerSlot ( "def", (CellDriver_t*)defDriver      , "def"      );
    registerSlot ( "aux", (CellDriver_t*)bookshelfDriver, "test.aux" );
    registerSlot ( "spi", (CellDriver_t*)spiceDriver    , "spi"      );
  //registerSlot ( "oa" , (CellDriver_t*)OpenAccess::oaDriver, "oa");
  }


  DriverSlot& DriversMap::getDriverSlot ( const string& tag )
  {
    return (*this)[ tag ];
  }


  void  DriversMap::registerSlot ( const string& tag, LibraryDriver_t* d, const string& ext )
  {
    iterator    it = find ( tag );
    DriverSlot* p;

    if ( it == end() ) {
      (*this)[ tag ] = DriverSlot ( tag );
      p = & ( (*this)[ tag ] );
    } else
      p = & ( it->second );

    p->setExtLib  ( ext );
    p->setDrivLib ( d );
  }


  void  DriversMap::registerSlot ( const string& tag, CellDriver_t* d, const string& ext )
  {
    iterator    it = find ( tag );
    DriverSlot* p;

    if ( it == end() ) {
      (*this)[ tag ] = DriverSlot ( tag );
      p = & ( (*this)[ tag ] );
    } else
      p = & ( it->second );

    p->setExtCell  ( ext );
    p->setDrivCell ( d );
  }


  void  DriversMap::unRegisterSlot ( const Name& tag )
  {
    iterator  it = find ( tag );

    if ( it == end() ) return;
    erase ( it );
  }


}
