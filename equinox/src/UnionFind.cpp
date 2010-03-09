
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
// |  C++ Header  :       "./UnionFind.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/Occurrence.h"

#include  "crlcore/Utilities.h"

#include "equinox/UnionFind.h"
#include "equinox/Tile.h"
#include "equinox/Equi.h"


namespace Equinox { 

  // -------------------------------------------------------------------
  // Class  :  "EQUINOX::UnionFind".


  inline Equi * UnionFind::getRootEqui(Tile* tile)
  {
    assert(tile->getEqui()!=NULL);
    Equi * equi = tile->getEqui();
    while( equi->getNextEqui())
      equi = equi->getNextEqui(); 
    return equi;
  }; 
  
  
  
  Equi * UnionFind::findValidEqui(Equi* equi)
  {
    if( (equi->getCount()>1) || (equi == NULL) ) 
      return equi;
    else                   
      return findValidEqui(equi->getNextEqui());
  };
  
  
  
  Equi * UnionFind::getRootEquiWithCompression(Tile* tile)
  {
    Equi * root = getRootEqui(tile);
    Equi * x = tile->getEqui(); 
    Equi * y = NULL;       
    
    while(x!= root) {
      y = x->getNextEqui();
      
      if( y == root ) 
	break;
      x->setNextEqui(root);
      root->incrementCount();
      
      if(y->getCount() == 1) {            
	Equi * tmpptr = findValidEqui(y);
	assert (tmpptr!=NULL); 
	y->decrementCount();
	y = tmpptr;
      } 
      else 
	y->decrementCount();
      x = y;
    }
    return root; 
  };  

}// End of namespace Equinox
