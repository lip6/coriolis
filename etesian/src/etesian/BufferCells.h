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
// |  C++ Header  :       "./etesian/BufferCells.h"                  |
// +-----------------------------------------------------------------+


#pragma  once
#include <map>
#include "hurricane/Cell.h"


namespace Etesian {

  using Hurricane::Cell;
  using Hurricane::Net;
  using Hurricane::Instance;
  using Hurricane::Plug;
  class EtesianEngine;


// -------------------------------------------------------------------
// Class : "Etesian::BufferDatas".

  class BufferDatas {
    public:
                       BufferDatas ( Cell*, int32_t drive, uint32_t maxSinks );
      inline int32_t   getDrive    () const;
      inline uint32_t  getMaxSinks () const;
      inline Cell*     getCell     () const;
      inline Net*      getInput    () const;
      inline Net*      getOutput   () const;
      inline Plug*     getInput    ( Instance* ) const;
      inline Plug*     getOutput   ( Instance* ) const;
    private:           
             Net*      _findIo     ( uint32_t direction ) const;
    private:
      int32_t   _drive;
      uint32_t  _maxSinks;
      Cell*     _cell;
      Net*      _input;
      Net*      _output;
  };

  
  inline int32_t   BufferDatas::getDrive    () const { return _drive; }
  inline uint32_t  BufferDatas::getMaxSinks () const { return _maxSinks; }
  inline Cell*     BufferDatas::getCell     () const { return _cell; }
  inline Net*      BufferDatas::getInput    () const { return _input; }
  inline Net*      BufferDatas::getOutput   () const { return _output; }
  inline Plug*     BufferDatas::getInput    ( Instance* instance ) const { return instance->getPlug( getInput() ); }
  inline Plug*     BufferDatas::getOutput   ( Instance* instance ) const { return instance->getPlug( getOutput() ); }


// -------------------------------------------------------------------
// Class : "Etesian::BufferCells".

  class BufferCells {
    public:
      inline              BufferCells       ( EtesianEngine* );
      inline             ~BufferCells       ();
             void         useBuffer         ( Cell*, int32_t drive, uint32_t maxSinks );
             BufferDatas* getBiggestBuffer  () const;
             BufferDatas* getSmallestBuffer () const;
             BufferDatas* getBuffer         ( int32_t drive ) const;
    private:
      EtesianEngine*              _etesian;
      std::map<int,BufferDatas*>  _bufferCells;
  };


  inline  BufferCells::BufferCells ( EtesianEngine* etesian )
    : _etesian    (etesian)
    , _bufferCells()
  { }

  inline  BufferCells::~BufferCells ()
  {
    for ( auto item : _bufferCells )
      delete item.second;
  }

} // Etesian namespace.
