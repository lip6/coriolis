// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2017-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/Block.h"                              |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <vector>
#include <map>
#include "hurricane/DbU.h"
#include "hurricane/Occurrence.h"

namespace Anabatic {
  class GCell;
}


namespace Katana {

  using std::string;
  using std::vector;
  using std::map;
  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Record;
  using Hurricane::Cell;
  using Hurricane::Occurrence;
  using Anabatic::GCell;

  class KatanaEngine;
  class Block;


// -------------------------------------------------------------------
// Class  :  "Katana::Row".

  class Row {
    public:
                        Row                  ( Block*, DbU::Unit );
      inline Block*     getBlock             () const;
      inline Cell*      getCell              () const;
      inline DbU::Unit  getY                 () const;
      inline GCell*     getSouthWest         () const;
      inline uint32_t   getChannelHeight     () const;
             void       setY                 ( DbU::Unit y );
             void       add                  ( DbU::Unit x, Occurrence );
             GCell*     createChannel        ( GCell* southWest, DbU::Unit channelHeight );
             uint32_t   computeChannelHeight ();
             void       routingPadsSubBreak  ();
             string     _getString           () const;
             Record*    _getRecord           () const;
    private:
      Block*                     _block;
      bool                       _isHybrid;
      DbU::Unit                  _y;
      map<DbU::Unit,Occurrence>  _occurrences;
      GCell*                     _southWest;
      uint32_t                   _channelHeight;
  };


  inline DbU::Unit  Row::getY             () const { return _y; }
  inline Block*     Row::getBlock         () const { return _block; }
  inline GCell*     Row::getSouthWest     () const { return _southWest; }
  inline uint32_t   Row::getChannelHeight () const { return _channelHeight; }


// -------------------------------------------------------------------
// Class  :  "Katana::Block".

  class Block {
    public:
                           Block          ( KatanaEngine*, Cell* );
                          ~Block          ();
      inline KatanaEngine* getKatana      () const;
      inline Cell*         getCell        () const;
             Row*          getAt          ( DbU::Unit y ) const;
             void          add            ( Occurrence );
             void          createChannels ();
             void          resizeChannels ();
             string        _getString     () const;
             Record*       _getRecord     () const;
    private:
      KatanaEngine* _katana;
      Cell*         _cell;
      vector<Row*>  _rows;
  };


  inline KatanaEngine* Block::getKatana () const { return _katana; }
  inline Cell*         Block::getCell   () const { return _cell; }


// Deferred Row functions.
  inline Cell* Row::getCell () const { return _block->getCell(); }


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::Row);
INSPECTOR_P_SUPPORT(Katana::Block);
