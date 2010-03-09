
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
// | =============================================================== |
// |  C++ Header  :       "./Tiles.h"                                 |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __EQUINOX_TILES__
#define  __EQUINOX_TILES__

namespace Equinox {

  class Tile;
  using namespace std;


// *********************************************************************************************************
//  Functeurs 
// *********************************************************************************************************

  template<typename T1> 
  class CompByXmin 
  // ******************
  {
  public:  bool operator()(const T1 t1, const T1 t2)
           // **************************************
    {
      if( t1->getXmin() < t2->getXmin() )
	return true;
      
      if( t1->getXmin() == t2->getXmin() ) {   
	if( t1->getYmin() < t2->getYmin() )  
	  return true;
	
	if( t1->getYmin() == t2->getYmin() )  
	  if( t1< t2 )  
	    return true;
      }	
      return false;
    }
  };
  
  
  template<typename T1> 
  class CompByXmax 
  // ******************
  {
  public:  bool operator()(const T1 t1, const T1 t2)
           // ***************************************
    {
      if( t1->getXmax() < t2->getXmax() )
	return true;
      
      if( t1->getXmax() == t2->getXmax() ) {   
	
	if( t1->getYmax() < t2->getYmax() )  
	  return true;
	
	if( t1->getYmax() == t2->getYmax() )  
	  if(t1 < t2)
	    return true;
      }	
      return false;
    }
    
  };
  
  
  template<typename T1> 
  class CompByXmin_Upper_Bound
  // ***************************
  {
  public:  bool operator()(const T1 t1, const T1 t2)
	   // ***************************************
    {
      if( t1->getXmin() < t2->getXmin() )
	return true;
      return false;
    }
  };
  
  
  template<typename T1> 
  class CompByXmax_Upper_Bound 
  // ****************************
  {
    
  public:  bool operator()(const T1 t1, const T1 t2)
           // *************************************
    {
      if( t1->getXmax() < t2->getXmax() )
	return true;
      return false;
    }
    
  };
  
  
  //typedef set<Tile*,CompByXmin <Tile*> > TilesByXmin;
  //typedef set<Tile*,CompByXmax <Tile*> > TilesByXmax;
  
    typedef vector<Tile*> TileVector;
} // End of Equinox namespace.
  
  
  
  
  

#endif  // __EQUINOX_TILES__
