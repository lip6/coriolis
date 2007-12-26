// ****************************************************************************************************
// File: Entity.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Entity.h"
#include "Quark.h"
#include "Cell.h"
#include "Instance.h"
#include "SharedPath.h"

namespace Hurricane {



// ****************************************************************************************************
// Entity implementation
// ****************************************************************************************************

Entity::Entity()
// *************
:	Inherit()
{
}

void Entity::_PreDelete()
// **********************
{
// trace << "entering Entity::_PreDelete: " << this << endl;
// trace_in();

    vector<Entity*> slaveEntities;
    SlaveEntityMap::iterator  it;
    SlaveEntityMap::iterator  end;
    GetCell()->_GetSlaveEntities(this,it,end);
    for(; it != end ; it++)
      slaveEntities.push_back(it->second);
    for(; slaveEntities.size() ; slaveEntities.pop_back()) {
      cerr << "Erasing " << slaveEntities.back() << endl;
      slaveEntities.back()->Delete();
    }

	Quark* quark = _GetQuark();
	if (quark) quark->Delete();

	stack<SharedPath*> sharedPathStack;
	for_each_instance(instance, GetCell()->GetSlaveInstances()) {
		SharedPath* sharedPath = instance->_GetSharedPath(NULL);
		if (sharedPath) sharedPathStack.push(sharedPath);
		end_for;
	}
	while (!sharedPathStack.empty()) {
		SharedPath* sharedPath = sharedPathStack.top();
		sharedPathStack.pop();
		Quark* quark = _GetQuark(sharedPath);
		if (quark) quark->Delete();
		Cell* cell = sharedPath->GetOwnerCell();
		for_each_instance(instance, cell->GetSlaveInstances()) {
			SharedPath* sharedPath2 = instance->_GetSharedPath(sharedPath);
			if (sharedPath2) sharedPathStack.push(sharedPath2);
			end_for;
		}
	}

	Inherit::_PreDelete();

// trace << "exiting Entity::_PreDelete:" << endl;
// trace_out();
}

string Entity::_GetString() const
// ******************************
{
	return Inherit::_GetString();
}

Record* Entity::_GetRecord() const
// *************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		Occurrence occurrence = Occurrence(this);
		if (occurrence.HasProperty())
			record->Add(GetSlot("Occurrence", occurrence));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
