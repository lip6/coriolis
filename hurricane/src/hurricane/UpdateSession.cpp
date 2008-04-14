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
:    Inherit()
{
}

void UpdateSession::destroy()
// *************************
{
    throw Error("Abnormal deletion of " + _TName("UpdateSession"));
}

const Name& UpdateSession::getPropertyName()
// *****************************************
{
    static Name NAME = _PName("UpdateSession");
    return NAME;
}

UpdateSession* UpdateSession::_create()
// ************************************
{
    UpdateSession* updateSession = new UpdateSession();

    updateSession->_postCreate();

    return updateSession;
}

void UpdateSession::_postCreate()
// ******************************
{
    Inherit::_postCreate();

    if (!UPDATOR_STACK) UPDATOR_STACK = new stack<UpdateSession*>();

    UPDATOR_STACK->push(this);
}

void UpdateSession::_destroy()
// **************************
{
    Inherit::destroy();
}

void UpdateSession::_preDestroy()
// *****************************
{
    if (!UPDATOR_STACK || UPDATOR_STACK->empty())
        throw Error("Invalid update session deletion : empty stack");

    if (UPDATOR_STACK->top() != this)
        throw Error("Invalid update session deletion : not on top");

    UPDATOR_STACK->pop();

    for_each_dbo(owner, getOwners()) {
        if (is_a<Go*>(owner)) ((Go*)owner)->materialize();
        end_for;
    }

    Inherit::_preDestroy();
}

string UpdateSession::_getString() const
// *************************************
{
    return "<" + _TName("UpdateSession") + ">";
}

Record* UpdateSession::_getRecord() const
// ********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
    }
    return record;
}

void UpdateSession::onCapturedBy(DBo* owner)
// *****************************************
{
    if (!is_a<Go*>(owner))
        throw Error("Bad update session capture : not a graphic object");

    Inherit::onCapturedBy(owner);
}

void UpdateSession::onNotOwned()
// *****************************
{
}



// ****************************************************************************************************
// Go::invalidate implementation : located here to access UPDATOR_STACK variable
// ****************************************************************************************************

void Go::invalidate(bool propagateFlag)
// ************************************
{
// trace << "invalidate(" << this << ")" << endl;
// trace_in();

    if (!UPDATOR_STACK || UPDATOR_STACK->empty())
        throw Error("Can't invalidate go : empty update session stack");

    Property* property = getProperty(UpdateSession::getPropertyName());

    if (property) {
        if (!is_a<UpdateSession*>(property))
            throw Error("Can't invalidate go : bad update session type");
    }
    else {
        SlaveEntityMap::iterator  it;
        SlaveEntityMap::iterator  end;
        getCell()->_getSlaveEntities(this,it,end);
        for(; it != end ; it++) {
          Go* go = dynamic_cast<Go*>(it->second);
          if (go) go->invalidate(propagateFlag);
        }

        if (isMaterialized()) {
            unmaterialize();
            put(UPDATOR_STACK->top());
        }
    }
// trace << "done" << endl;
// trace_out();
}

void UpdateSession::open() {
// trace << "OpenUpdateSession()" << endl;
// trace_in();
    UpdateSession::_create();
}

void UpdateSession::close() {
// trace << "CloseUpdateSession()" << endl;
// trace_in();

    if (!UPDATOR_STACK || UPDATOR_STACK->empty())
        throw Error("Can't end update : empty update session stack");

    UPDATOR_STACK->top()->_destroy();

// trace << "done" << endl;
// trace_out();
// trace << "done" << endl;
// trace_out();
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
