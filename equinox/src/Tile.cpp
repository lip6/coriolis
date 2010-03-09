
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
// |                      C O R I O L I S                            |
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Tile.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/Error.h"
#include "hurricane/Occurrence.h"

#include "crlcore/Utilities.h"

#include "equinox/Tile.h"
#include "equinox/Equi.h"


namespace Equinox {

  using Hurricane::Error;
  

  // -------------------------------------------------------------------
  // Class  :  "EQUINOX::Tile".



  Tile::Tile (const Occurrence occurrence, const Box box, BasicLayer* basicLayer, Equi * equi) 
    : _occurrence(occurrence)
    , _box(box)
    , _basicLayer(basicLayer)
    , _equi(equi)
  { 
  }
  
  

  
  
  Tile* Tile::create(const Occurrence occurrence, Box box,BasicLayer* basiclayer, Equi * equi) 
  {
    Tile* tile = new Tile(occurrence, box, basiclayer, equi);
    if(!tile){
      throw Error("can't create Tile : allocation failed"); 
    }
    tile->_postCreate();
    
    
    return tile;
  }
  

   
  
  string Tile::_getString() const 
  {
    string s;
    
    if (!_equi)
      {
	s = "<" + _getTypeName() + " " +  getString(_occurrence) +  " <NULL equi> >"; 
      } else {
      s = "<" + _getTypeName() + " " + getString(_occurrence) +  " " + _equi->_getString() + ">"; 
    }
    return s; 
  }
  



}// End of namespace Equinox
