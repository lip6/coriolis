// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BufferCells.cpp"                        |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/Warning.h"
#include "etesian/BufferCells.h"
#include "etesian/EtesianEngine.h"


namespace Etesian {

  using std::cerr;
  using std::endl;
  using std::map;
  using std::string;
  using std::ostringstream;
  using std::make_pair;
  using Hurricane::Warning;
  using Hurricane::DbU;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class : "Etesian::BufferDatas".


  BufferDatas::BufferDatas ( Cell* cell, int32_t drive, uint32_t maxSinks )
    : _drive   (drive)
    , _maxSinks(maxSinks)
    , _cell    (cell)
    , _input   (_findIo(Net::Direction::DirIn))
    , _output  (_findIo(Net::Direction::DirOut))
  { }


  Net* BufferDatas::_findIo ( uint32_t  direction ) const
  {
    if (not _cell) return NULL;
    for ( Net* net : _cell->getNets() ) {
      if (net->isSupply()) continue;
      if (not net->isExternal()) continue;
      if (net->getDirection() & direction)
        return net;
    }
    cerr << Warning( "BufferDatas::_findIo(): Can't find an I/O net in cell \"%s\"."
                   , getString(_cell->getName()).c_str() ) << endl;
    return NULL;
  }


// -------------------------------------------------------------------
// Class : "Etesian::BufferCells".


  void  BufferCells::useBuffer ( Cell* cell, int drive, uint32_t maxSinks )
  {
    if (not cell) return;
    if (getBuffer(drive)) {
      cerr << Warning( "BufferCells::useBuffer(): \"%s\" duplicate buffer for strenght %d."
                     , getString(cell->getName()).c_str()
                     , drive
                     ) << endl;
      return;
    }
    _bufferCells.insert( make_pair(drive,new BufferDatas(cell,drive,maxSinks)) );
  }


  BufferDatas* BufferCells::getBiggestBuffer () const
  {
    if (_bufferCells.empty()) return NULL;
    return (*(--_bufferCells.end())).second;
  }


  BufferDatas* BufferCells::getSmallestBuffer () const
  {
    if (_bufferCells.empty()) return NULL;
    return (*(_bufferCells.begin())).second;
  }


  BufferDatas* BufferCells::getBuffer ( int drive ) const
  {
    map<int,BufferDatas*>::const_iterator ibuffer = _bufferCells.find( drive );
    if (ibuffer == _bufferCells.end()) return NULL;

    return (*ibuffer).second;
  }


}  // Etesian namespace.
