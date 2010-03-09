
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
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./DisconnectError.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x
#include <hurricane/Cell.h>
#include <hurricane/Net.h>
#include <equinox/Strategy.h>
#include <solstice/SolsticeEngine.h>
#include <solstice/DisconnectError.h>
#include <string>

namespace Solstice {
  
  using namespace Hurricane;
  DisconnectError::DisconnectError(Cell*cell, const Occurrence& occurrence, int count)  : 
    RoutingError(cell,count),
    _occurrence(occurrence)
  {
    
    if (dynamic_cast<Net*>(occurrence.getEntity())) 
      forEach ( Component*, component, dynamic_cast<Net*>(occurrence.getEntity())->getComponents() ) {
	_occurrences->insert(Occurrence ( *component ) );
      }
    // _area = occurrence.getBoundingBox();
    _area = Box(); 
  }
  DisconnectError::~DisconnectError() {};
  


  string DisconnectError::getErrorDescription () const 
  {
    return "Deconnexion du Net " + _occurrence.getEntity()->_getString() ;
  } ;
  
  
} // End of Solstice namespace.
