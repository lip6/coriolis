
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
// |  C++ Header  :       "./RoutingError.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include <hurricane/Name.h>
#include <hurricane/Box.h>
#include <solstice/RoutingError.h>

namespace Solstice {

  const Name  RoutingError::_extensionName = "Solstice::RoutingError";

  RoutingError::~RoutingError()  {};

  RoutingError::RoutingError(Cell* cell,int count):
    ExtensionGo(cell),
    _cell(cell),
    _area(),
    _count(count)
  {
    _occurrences = new  set<Occurrence>;
  }
  

  void RoutingError::translate ( const DbU::Unit& dx, const DbU::Unit& dy )
  {
    // cout << "Translate !! " << endl;
  }


}  // End of Solstice namespace.
