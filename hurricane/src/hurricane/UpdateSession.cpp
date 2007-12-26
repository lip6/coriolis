// ****************************************************************************************************
// File: UpdateSession.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "UpdateSession.h"
#include "Go.h"
#include "Cell.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// UpdateSession implementation
// ****************************************************************************************************

stack<UpdateSession*>* UPDATOR_STACK = NULL;

UpdateSession::UpdateSession()
// ***************************
:	Inherit()
{
}

void UpdateSession::Delete()
// *************************
{
	throw Error("Abnormal deletion of " + _TName("UpdateSession"));
}

const Name& UpdateSession::GetPropertyName()
// *****************************************
{
	static Name NAME = _PName("UpdateSession");
	return NAME;
}

UpdateSession* UpdateSession::_Create()
// ************************************
{
	UpdateSession* updateSession = new UpdateSession();

	updateSession->_PostCreate();

	return updateSession;
}

void UpdateSession::_PostCreate()
// ******************************
{
	Inherit::_PostCreate();

	if (!UPDATOR_STACK) UPDATOR_STACK = new stack<UpdateSession*>();

	UPDATOR_STACK->push(this);
}

void UpdateSession::_Delete()
// **************************
{
	Inherit::Delete();
}

void UpdateSession::_PreDelete()
// *****************************
{
	if (!UPDATOR_STACK || UPDATOR_STACK->empty())
		throw Error("Invalid update session deletion : empty stack");

	if (UPDATOR_STACK->top() != this)
		throw Error("Invalid update session deletion : not on top");

	UPDATOR_STACK->pop();

	for_each_dbo(owner, GetOwners()) {
		if (is_a<Go*>(owner)) ((Go*)owner)->Materialize();
		end_for;
	}

	Inherit::_PreDelete();
}

string UpdateSession::_GetString() const
// *************************************
{
	return "<" + _TName("UpdateSession") + ">";
}

Record* UpdateSession::_GetRecord() const
// ********************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
	}
	return record;
}

void UpdateSession::OnCapturedBy(DBo* owner)
// *****************************************
{
	if (!is_a<Go*>(owner))
		throw Error("Bad update session capture : not a graphic object");

	Inherit::OnCapturedBy(owner);
}

void UpdateSession::OnNotOwned()
// *****************************
{
}



// ****************************************************************************************************
// Go::Invalidate implementation : located here to access UPDATOR_STACK variable
// ****************************************************************************************************

void Go::Invalidate(bool propagateFlag)
// ************************************
{
// trace << "Invalidate(" << this << ")" << endl;
// trace_in();

	if (!UPDATOR_STACK || UPDATOR_STACK->empty())
		throw Error("Can't invalidate go : empty update session stack");

	Property* property = GetProperty(UpdateSession::GetPropertyName());

	if (property) {
		if (!is_a<UpdateSession*>(property))
			throw Error("Can't invalidate go : bad update session type");
	}
	else {
        SlaveEntityMap::iterator  it;
        SlaveEntityMap::iterator  end;
        GetCell()->_GetSlaveEntities(this,it,end);
        for(; it != end ; it++) {
          Go* go = dynamic_cast<Go*>(it->second);
          if (go) go->Invalidate(propagateFlag);
        }

		if (IsMaterialized()) {
			Unmaterialize();
			Put(UPDATOR_STACK->top());
		}
	}
// trace << "done" << endl;
// trace_out();
}



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

void OpenUpdateSession()
// *********************
{
// trace << "OpenUpdateSession()" << endl;
// trace_in();
	UpdateSession::_Create();
}

void CloseUpdateSession()
// **********************
{
// trace << "CloseUpdateSession()" << endl;
// trace_in();

	if (!UPDATOR_STACK || UPDATOR_STACK->empty())
		throw Error("Can't end update : empty update session stack");

	UPDATOR_STACK->top()->_Delete();

// trace << "done" << endl;
// trace_out();
// trace << "done" << endl;
// trace_out();
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
