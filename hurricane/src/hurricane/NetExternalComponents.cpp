// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: NetExternalComponents.cpp
// Author: C. Alexandre
// ****************************************************************************************************

#include "hurricane/Error.h"
#include "hurricane/Relation.h"
#include "hurricane/Net.h"

#include "hurricane/NetExternalComponents.h"

namespace Hurricane {

static Name ExternalComponentsRelationName("ExternalComponentsRelation");

static StandardRelation* getExternalComponentsRelation(const Net* net) {
    Property* property = net->getProperty(ExternalComponentsRelationName);
    if (!property) {
        return NULL;
    } else {
        StandardRelation* relation = dynamic_cast<StandardRelation*>(property);
        if (!relation)
            throw Error("Bad Property type: Must be a Standard Relation");
        return relation;
    }
}

Components getExternalComponents(const Net* net) {
    if (!net->isExternal())
        throw Error("Impossible to retrieve external components on non external net "
                + net->getName()._getString());
    
    StandardRelation* externalComponentsRelation = getExternalComponentsRelation(net);
    if (!externalComponentsRelation)
        return Components();
    return externalComponentsRelation->getSlaveOwners().getSubSet<Component*>();
}

void setExternal(Component* component) {
    Net* net = component->getNet();
    if (!net->isExternal())
        throw Error("Impossible to set as external a component member of non external net "
                + net->getName()._getString());
    StandardRelation* externalComponentsRelation = getExternalComponentsRelation(net);
    if (!externalComponentsRelation)
        externalComponentsRelation = StandardRelation::create(net, ExternalComponentsRelationName);
    component->put(externalComponentsRelation);
}

} // End of Hurricane namespace.
