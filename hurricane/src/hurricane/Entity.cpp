// ****************************************************************************************************
// File: ./Entity.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
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

#include <limits>
#include "hurricane/Error.h"
#include "hurricane/Entity.h"
#include "hurricane/Quark.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/SharedPath.h"

namespace Hurricane {



// ****************************************************************************************************
// Entity implementation
// ****************************************************************************************************


  unsigned int  Entity::_idCounter = 0;


  Entity::Entity()
    : Inherit()
    , _id(_idCounter++)
  {
    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "Entity::Entity(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }
  }


  void Entity::_preDestroy()
  {
  //ltrace(10) << "Entity::_preDestroy() - " << (void*)this << endl;
  //ltracein(10);

    vector<Entity*> slaveEntities;
    SlaveEntityMap::iterator  it;
    SlaveEntityMap::iterator  end;
    getCell()->_getSlaveEntities(this,it,end);
    for(; it != end ; it++)
      slaveEntities.push_back(it->second);
    for(; slaveEntities.size() ; slaveEntities.pop_back()) {
      cerr << "Erasing " << slaveEntities.back() << endl;
      slaveEntities.back()->destroy();
    }

    Quark* quark = _getQuark();
    if (quark) quark->destroy();

    stack<SharedPath*> sharedPathStack;
    for_each_instance(instance, getCell()->getSlaveInstances()) {
      SharedPath* sharedPath = instance->_getSharedPath(NULL);
      if (sharedPath) sharedPathStack.push(sharedPath);
      end_for;
    }
    while (!sharedPathStack.empty()) {
      SharedPath* sharedPath = sharedPathStack.top();
      sharedPathStack.pop();
      Quark* quark = _getQuark(sharedPath);
      if (quark) quark->destroy();
      Cell* cell = sharedPath->getOwnerCell();
      for_each_instance(instance, cell->getSlaveInstances()) {
        SharedPath* sharedPath2 = instance->_getSharedPath(sharedPath);
        if (sharedPath2) sharedPathStack.push(sharedPath2);
        end_for;
      }
    }

    Inherit::_preDestroy();

  //ltrace(10) << "Entity::_preDestroy() - exit" << endl;
  //ltraceout(10);
  }

string Entity::_getString() const
// ******************************
{
    string s = Inherit::_getString();
    s.insert(1, "id:"+getString(_id)+" ");
    return s;
}

Record* Entity::_getRecord() const
// *************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("_id", _id));
        Occurrence occurrence = Occurrence(this);
        if (occurrence.hasProperty())
            record->add(getSlot("Occurrence", occurrence));
    }
    return record;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
