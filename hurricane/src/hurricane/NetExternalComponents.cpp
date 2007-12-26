// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: NetExternalComponents.cpp
// Author: C. Alexandre
// ****************************************************************************************************

#include "Error.h"
#include "Relation.h"
#include "Net.h"

#include "NetExternalComponents.h"

namespace Hurricane {

static Name ExternalComponentsRelationName("ExternalComponentsRelation");

static StandardRelation* GetExternalComponentsRelation(const Net* net)
{
	Property* property = net->GetProperty(ExternalComponentsRelationName);
	if (!property)
		return NULL;
	else
	{
		StandardRelation* relation = dynamic_cast<StandardRelation*>(property);
		if (!relation)
			throw Error("Bad Property type: Must be a Standard Relation");
		return relation;
	}
    
}

Components GetExternalComponents(const Net* net)
{
    if (!net->IsExternal())
        throw Error("Impossible to retrieve external components on non external net "
                + net->GetName()._GetString());
    
    StandardRelation* externalComponentsRelation = GetExternalComponentsRelation(net);
    if (!externalComponentsRelation)
        return Components();
    return externalComponentsRelation->GetSlaveOwners().GetSubSet<Component*>();
}

void SetExternal(Component* component)
{
    Net* net = component->GetNet();
    if (!net->IsExternal())
        throw Error("Impossible to set as external a component member of non external net "
                + net->GetName()._GetString());
    StandardRelation* externalComponentsRelation = GetExternalComponentsRelation(net);
    if (!externalComponentsRelation)
        externalComponentsRelation = StandardRelation::Create(net, ExternalComponentsRelationName);
    component->Put(externalComponentsRelation);
}

} // End of Hurricane namespace.
