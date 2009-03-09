
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
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                Christophe Alexandre              |
// |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetExternalComponents.cpp"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/Error.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"


namespace Hurricane {


  const Name NetExternalComponents::_name = "ExternalComponentsRelation";


  StandardRelation* NetExternalComponents::getRelation ( const Net* net )
  {
    Property* property = net->getProperty(_name);
    if (!property) {
        return NULL;
    } else {
        StandardRelation* relation = dynamic_cast<StandardRelation*>(property);
        if (!relation)
            throw Error("Bad Property type: Must be a Standard Relation");
        return relation;
    }
  }


  Components NetExternalComponents::get ( const Net* net )
  {
    if (!net->isExternal())
      throw Error("Impossible to retrieve external components on non external net "
                 + net->getName()._getString());
    
    StandardRelation* externalComponentsRelation = getRelation(net);
    if (!externalComponentsRelation)
      return Components();
    return externalComponentsRelation->getSlaveOwners().getSubSet<Component*>();
  }


  void NetExternalComponents::setExternal ( Component* component )
  {
    Net* net = component->getNet();
    if (!net->isExternal())
      throw Error("Impossible to set as external a component member of non external net "
                 + net->getName()._getString());
    StandardRelation* externalComponentsRelation = getRelation(net);
    if (!externalComponentsRelation)
      externalComponentsRelation = StandardRelation::create(net, _name);
    component->put(externalComponentsRelation);
  }


  bool  NetExternalComponents::isExternal ( Component* component )
  {
    Net* net = component->getNet();
    if (!net->isExternal()) return false;

    return getRelation(net) != NULL;
  }


} // End of Hurricane namespace.
