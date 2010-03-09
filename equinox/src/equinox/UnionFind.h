
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
// |  C++ Header  :       "./equinox/UnionFind.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __EQUINOX_UNIONFIND__
#define  __EQUINOX_UNIONFIND__


namespace Equinox {
  

  class Tile;
  class Equi;
  
  class UnionFind {
    
  public: 
    inline static Equi *     getRootEqui                (Tile* tile); /* internal inline */
    /**/   static Equi *     getRootEquiWithCompression (Tile* tile); 
    /**/   static Equi *     findValidEqui              (Equi* equi);
 
 
    		      
  private :	      
    /**/              UnionFind                 ();
    /**/              UnionFind                 (const UnionFind&);
    /**/   UnionFind& operator=                 (const UnionFind&);	    
        
    
  };
  
  
} //End of equinox namespace


#endif //End of  __EQUINOX_UNIONFIND__
