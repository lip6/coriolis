// ****************************************************************************************************
// File: ./UpdateSession.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#include "hurricane/UpdateSession.h"
#include "hurricane/Go.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// UpdateSession implementation
// ****************************************************************************************************

stack<UpdateSession*>* UPDATOR_STACK = NULL;

UpdateSession::UpdateSession()
// ***************************
:    Inherit()
{ }

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
  // ******************************
  {
    if (not UPDATOR_STACK or UPDATOR_STACK->empty())
      throw Error( "Invalid update session deletion : empty stack" );

    if (UPDATOR_STACK->top() != this)
      throw Error( "Invalid update session deletion : not on top" );

    UPDATOR_STACK->pop();

    vector<Cell*> changedCells;
    for ( DBo* owner : getOwners() ) {
      Cell* cell = dynamic_cast<Cell*>(owner);
      if (cell) {
      //cerr << "Notify Cell::CellChanged to: " << cell << endl; 
        changedCells.push_back( cell );
      } else {
        Go* go = dynamic_cast<Go*>(owner);
        if (go) go->materialize();
      }
    }

  // Changed cells must be notified *after* all the Gos are materialized.
  // They also should be sorted according to their hierarchical depth and
  // revalidated bottom-up (TODO).
    for ( auto icell : changedCells ) {
      icell->notify( Cell::Flags::CellChanged );
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
    if ( not dynamic_cast<Go*>(owner) and not dynamic_cast<Cell*>(owner) )
      throw Error( "Bad update session capture : not a graphic object (Go) or a Cell" );
    
    Inherit::onCapturedBy(owner);
  }

void UpdateSession::onNotOwned()
// *****************************
{ }



// ****************************************************************************************************
// Go::invalidate implementation : located here to access UPDATOR_STACK variable
// ****************************************************************************************************

void Go::invalidate(bool propagateFlag)
// ************************************
{
  cdebug_log(18,1) << "Go::invalidate(" << this << ") " << endl;

  if (not UPDATOR_STACK or UPDATOR_STACK->empty())
    throw Error( "Can't invalidate go: empty update session stack" );

  Property* property = getProperty( UpdateSession::getPropertyName() );

  if (property) {
    if (not dynamic_cast<UpdateSession*>(property))
      throw Error( "Can't invalidate go : bad update session type" );
  } else {
    SlaveEntityMap::iterator  it;
    SlaveEntityMap::iterator  end;
    getCell()->_getSlaveEntities( this, it, end );
    for( ; it!=end ; it++ ) {
      Go* go = dynamic_cast<Go*>( it->second );
      if (go) go->invalidate( propagateFlag );
    }

    if (isMaterialized() or not Go::autoMaterializationIsDisabled()) {
      unmaterialize();
      put( UPDATOR_STACK->top() );
    }

    Property* cellUpdateSession = getCell()->getProperty( UpdateSession::getPropertyName() );
    if (not cellUpdateSession) {
    // Put the cell in the UpdateSession relation, but *do not* unmaterialize it.
    //cerr << "Notify Cell::CellAboutToChange to: " << getCell() << endl; 
      getCell()->put   ( UPDATOR_STACK->top() );
      getCell()->notify( Cell::Flags::CellAboutToChange );
      for ( Instance* instance : getCell()->getSlaveInstances() ) {
        instance->invalidate( false );
      }
    }
  }
  
  cdebug_tabw(18,-1);
  cdebug_log(18,0) << "Go::invalidate(" << this << ") - Completed." << endl;
}

void UpdateSession::open()
// ***********************
{
  cdebug_log(18,1) << "UpdateSession::open()" << endl;
  UpdateSession::_create();
}

void UpdateSession::close()
// ************************
{
  cdebug_tabw(18,-1);
  cdebug_log(18,1) << "UpdateSession::close() - Start materialization." << endl;

  if (!UPDATOR_STACK || UPDATOR_STACK->empty())
    throw Error("Can't end update : empty update session stack");

  UPDATOR_STACK->top()->_destroy();

  cdebug_tabw(18,-1);
  cdebug_log(18,0) << "UpdateSession::close() - Materialization completed." << endl;
}

void UpdateSession::reset()
// ************************
{
  cdebug_log(18,1) << "UpdateSession::reset()" << endl;

  while ( UPDATOR_STACK and not UPDATOR_STACK->empty() ) close();

  cdebug_tabw(18,-1);
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
