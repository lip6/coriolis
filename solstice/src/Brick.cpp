
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
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Brick.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include <string>

#include "hurricane/Occurrence.h"
#include "hurricane/Box.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/DbU.h"

#include "hurricane/Error.h"
#include  "crlcore/Utilities.h"
#include<equinox/Interval.h>

#include<solstice/Brick.h>




namespace Solstice {
  
  using namespace Hurricane;
  using namespace std;
  
  
  const  DbU::Unit&  Brick::getXmin() const {  return _box.getXMin(); };	   
  const  DbU::Unit&  Brick::getYmin() const {  return _box.getYMin(); };	   
  const  DbU::Unit&  Brick::getXmax() const {  return _box.getXMax(); };	   
  const  DbU::Unit&  Brick::getYmax() const {  return _box.getYMax(); };
  long Brick::GetLowPoint() const { return _box.getYMin(); }  
  long Brick::GetHighPoint() const { return _box.getYMax(); }  
  
  bool Brick::isEmpty () const { return _box.isEmpty(); };
  
  
  Brick::Brick(const Occurrence& hypernet, const Occurrence& componentoccurrence, const Box& box,
	       BasicLayer* basiclayer)
    : _hypernet(hypernet)
    , _componentoccurrence(componentoccurrence)
    , _box(box)
    , _basicLayer(basiclayer)
  { 

  }

  Brick::~Brick() {};
  
  
  
  
  
   Brick* Brick::create(const Occurrence& hypernet, const Occurrence& componentoccurrence, const Box& box,
			BasicLayer* basiclayer)
   {
     Brick* brick = new Brick(hypernet, componentoccurrence, box, basiclayer);
     if(!brick) {
       throw Error("can't create Brick : allocation failed"); 
     }
     
     return brick;
   }
  
  void Brick::destroy() {
    delete this;
  }
  
  string Brick::_getString() const {
    string s = "< Brick " + getString(_hypernet) + " " + getString(_componentoccurrence) + " ";
    s += "\n" + getString(_box) + " " + getString(_basicLayer->getName()) + " >";
    return s; 
  }

  
}// End of namespace Solstice



