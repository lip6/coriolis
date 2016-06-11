// ****************************************************************************************************
// File: ./Component.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
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

#include "hurricane/Component.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "hurricane/Rubber.h"
#include "hurricane/Slice.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Component_IsUnderFilter : public Filter<Component*> {
// ******************************************************

    public: Box _area;

    public: Component_IsUnderFilter(const Box& area)
    // *********************************************
    :    _area(area)
    {
    };

    public: Component_IsUnderFilter(const Component_IsUnderFilter& filter)
    // *******************************************************************
    :    _area(filter._area)
    {
    };

    public: Component_IsUnderFilter& operator=(const Component_IsUnderFilter& filter)
    // ******************************************************************************
    {
        _area = filter._area;
        return *this;
    };

    public: virtual Filter<Component*>* getClone() const
    // *************************************************
    {
        return new Component_IsUnderFilter(*this);
    };

    public: virtual bool accept(Component* component) const
    // ****************************************************
    {
        return _area.intersect(component->getBoundingBox());
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Component::IsUnderFilter") + " " + getString(_area) + ">";
    };

};



// ****************************************************************************************************
// Component_Hooks declaration
// ****************************************************************************************************

class Component_Hooks : public Collection<Hook*> {
// *********************************************

// Types
// *****

    public: typedef Collection<Hook*> Inherit;

    public: class Locator : public Hurricane::Locator<Hook*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Hook*> Inherit;

        private: const Component* _component;
        private: Hook* _hook;

        public: Locator(const Component* component = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Hook* getElement() const;
        public: virtual Hurricane::Locator<Hook*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Component* _component;

// Constructors
// ************

    public: Component_Hooks(const Component* component = NULL);
    public: Component_Hooks(const Component_Hooks& hooks);

// Operators
// *********

    public: Component_Hooks& operator=(const Component_Hooks& hooks);

// Accessors
// *********

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Component_ConnexComponents declaration
// ****************************************************************************************************

class Component_ConnexComponents : public Collection<Component*> {
// *************************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Component* _component;
        private: set<Component*> _componentSet;
        private: stack<Component*> _componentStack;

        public: Locator(const Component* component = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* getElement() const;
        public: virtual Hurricane::Locator<Component*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Component* _component;

// Constructors
// ************

    public: Component_ConnexComponents(const Component* component = NULL);
    public: Component_ConnexComponents(const Component_ConnexComponents& connexComponents);

// Operators
// *********

    public: Component_ConnexComponents& operator=(const Component_ConnexComponents& connexComponents);

// Accessors
// *********

    public: virtual Collection<Component*>* getClone() const;
    public: virtual Hurricane::Locator<Component*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Component_SlaveComponents declaration
// ****************************************************************************************************

class Component_SlaveComponents : public Collection<Component*> {
// ************************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Component* _component;
        private: set<Component*> _componentSet;
        private: stack<Component*> _componentStack;

        public: Locator(const Component* component = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* getElement() const;
        public: virtual Hurricane::Locator<Component*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

};

// Attributes
// **********

    private: const Component* _component;

// Constructors
// ************

    public: Component_SlaveComponents(const Component* component = NULL);
    public: Component_SlaveComponents(const Component_SlaveComponents& slaveComponents);

// Operators
// *********

    public: Component_SlaveComponents& operator=(const Component_SlaveComponents& slaveComponents);

// Accessors
// *********

    public: virtual Collection<Component*>* getClone() const;
    public: virtual Hurricane::Locator<Component*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;
};



// ****************************************************************************************************
// Component implementation
// ****************************************************************************************************

Component::Component(Net* net, bool inPlugCreate)
// **********************************************
:    Inherit(),
    _net(net),
    _rubber(NULL),
    _bodyHook(this),
    _nextOfNetComponentSet(NULL)
{
    if (!inPlugCreate && !_net)
        throw Error("Can't create " + _TName("Component") + " : null net");
}

Cell* Component::getCell() const
// *****************************
{
    return _net->getCell();
}

Hooks Component::getHooks() const
// ******************************
{
    return Component_Hooks(this);
}

Components Component::getConnexComponents() const
// **********************************************
{
    return Component_ConnexComponents(this);
}

Components Component::getSlaveComponents() const
// *********************************************
{
    return Component_SlaveComponents(this);
}

ComponentFilter Component::getIsUnderFilter(const Box& area)
// *********************************************************
{
    return Component_IsUnderFilter(area);
}

void Component::materialize()
// **************************
{
  cdebug_log(18,0) << "Component::materialize() - " << this << endl;

  if (!isMaterialized()) {
    Cell*  cell  = getCell();
    const Layer* layer = getLayer();
    if (cell && layer) {
      Slice* slice = cell->getSlice(layer);
      if (!slice) slice = Slice::_create(cell, layer);
      QuadTree* quadTree = slice->_getQuadTree();
      quadTree->insert(this);
      cell->_fit(quadTree->getBoundingBox());
    } else {
    //cerr << "[WARNING] " << this << " not inserted into QuadTree." << endl;
    }
  }
}

void Component::unmaterialize()
// ****************************
{
  cdebug_log(18,0) << "Component::unmaterialize() " << this << endl;

  if (isMaterialized()) {
    Cell* cell = getCell();
    Slice* slice = cell->getSlice(getLayer());
    if (slice) {
      cell->_unfit(getBoundingBox());
      slice->_getQuadTree()->remove(this);
      if (slice->isEmpty()) slice->_destroy();
    }
  }
}

void Component::invalidate(bool propagateFlag)
// *******************************************
{
    Inherit::invalidate(false);

    if (propagateFlag) {
        Rubber* rubber = getRubber();
        if (rubber) rubber->invalidate();
        for_each_component(component, getSlaveComponents()) {
            component->invalidate(false);
            end_for;
        }
    }
}


void Component::forceId(unsigned int id)
// *************************************
{
  if (not inForcedIdMode())
    throw Error( "Component::forceId(): DataBase *must* be in forced id mode to call this method." );

  if (getId() == id) return;

  bool materialized = isMaterialized();
  if (materialized) unmaterialize();
  if (_net) _net->_getComponentSet()._remove(this);

  setId( id );

  if (_net) _net->_getComponentSet()._insert(this);
  if (materialized) materialize();
}

void Component::_postCreate()
// **************************
{
    if (_net) _net->_getComponentSet()._insert(this);

    Inherit::_postCreate();
}

void Component::_preDestroy()
// *************************
{
    cdebug_log(18,1) << "entering Component::_Predestroy: " << this << endl;

    clearProperties();

    set<Component*> componentSet;
    getSlaveComponents().fill(componentSet);

    set<Hook*> masterHookSet;
    componentSet.insert(this);
    for_each_component(component, getCollection(componentSet)) {
        component->unmaterialize();
        for_each_hook(hook, component->getHooks()) {
            for_each_hook(hook, hook->getHooks()) {
                if (hook->isMaster() && (componentSet.find(hook->getComponent()) == componentSet.end()))
                    masterHookSet.insert(hook);
                end_for;
            }
            if (!hook->isMaster()) hook->detach();
            end_for;
        }
        end_for;
    }

    componentSet.erase(this);
    for_each_component(component, getCollection(componentSet)) {
        component->destroy();
        end_for;
    }

    set<Rubber*> rubberSet;
    set<Hook*> mainMasterHookSet;
    for_each_hook(hook, getCollection(masterHookSet)) {
        Rubber* rubber = hook->getComponent()->getRubber();
        if (!rubber)
            mainMasterHookSet.insert(hook);
        else {
            if (rubberSet.find(rubber) == rubberSet.end()) {
                rubberSet.insert(rubber);
                mainMasterHookSet.insert(hook);
            }
        }
        end_for;
    }
    Hook* masterHook = NULL;
    for_each_hook(hook, getCollection(mainMasterHookSet)) {
        if (!masterHook)
            masterHook = hook;
        else
            hook->merge(masterHook);
        end_for;
    }
    /**/

    _bodyHook.detach();

    Inherit::_preDestroy();

    if (_net) _net->_getComponentSet()._remove(this);


    cdebug_log(18,0) << "exiting Component::_Predestroy:" << endl;
    cdebug_tabw(18,-1);
}

void Component::_toJson( JsonWriter* writer ) const
// ************************************************
{
  Inherit::_toJson( writer );
  jsonWrite( writer, "_bodyHook", _bodyHook.getNextHook()->toJson() );
}

void Component::_toJsonSignature( JsonWriter* writer ) const
// *********************************************************
{
  jsonWrite( writer, "_net" , getNet()->getName() );
  _toJson( writer );
}

string Component::_getString() const
// *********************************
{
    string s = Inherit::_getString();
    if (!_net)
        s.insert(s.length() - 1, " UNCONNECTED");
    else
        s.insert(s.length() - 1, " " + getString(_net->getName()));
    return s;
}

Record* Component::_getRecord() const
// ****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Net", _net));
        record->add(getSlot("Rubber", _rubber));
        record->add(getSlot("BodyHook", &_bodyHook));
    }
    return record;
}

void Component::_setNet(Net* net)
// ******************************
{
    if (net != _net) {
        if (_net) _net->_getComponentSet()._remove(this);
        _net = net;
        if (_net) _net->_getComponentSet()._insert(this);
    }
}

void Component::_setRubber(Rubber* rubber)
// ***************************************
{
    if (rubber != _rubber) {
        if (_rubber) _rubber->_release();
        _rubber = rubber;
        if (_rubber) _rubber->_capture();
    }
}

//bool Component::_IsInterceptedBy(View* view, const Point& point, const DbU::Unit& aperture) const
//// *****************************************************************************************
//{
//    Box area(point);
//    area.Inflate(aperture);
//    for_each_basic_layer(basicLayer, getLayer()->getBasicLayers()) {
//        if (view->IsVisible(basicLayer) && getBoundingBox(basicLayer).intersect(area))
//            return true;
//        end_for;
//    }
//
//    return false;
//}
//

// ****************************************************************************************************
// Component::BodyHook implementation
// ****************************************************************************************************

static int BODY_HOOK_OFFSET = -1;

Component::BodyHook::BodyHook(Component* component)
// ************************************************
:    Inherit()
{
    if (!component)
      throw Error("Can't create " + _getTypeName() + " : null component");

    if (BODY_HOOK_OFFSET == -1) {
        BODY_HOOK_OFFSET = (unsigned long)this - (unsigned long)component;
        Hook::addCompToHook(_getTypeName(),_compToHook);
    }
}

Component* Component::BodyHook::getComponent() const
// *************************************************
{
    return (Component*)((unsigned long)this - BODY_HOOK_OFFSET);
}

string Component::BodyHook::_getString() const
// *******************************************
{
    return "<" + _TName("Component::BodyHook") + " " + getString(getComponent()) + ">";
}

Hook* Component::BodyHook::_compToHook(Component* component)
// *************************************************************
{ return &(component->_bodyHook); }


// ****************************************************************************************************
// Component_Hooks implementation
// ****************************************************************************************************

Component_Hooks::Component_Hooks(const Component* component)
// *********************************************************
:     Inherit(),
    _component(component)
{
}

Component_Hooks::Component_Hooks(const Component_Hooks& hooks)
// ***********************************************************
:     Inherit(),
    _component(hooks._component)
{
}

Component_Hooks& Component_Hooks::operator=(const Component_Hooks& hooks)
// **********************************************************************
{
    _component = hooks._component;
    return *this;
}

Collection<Hook*>* Component_Hooks::getClone() const
// *************************************************
{
    return new Component_Hooks(*this);
}

Locator<Hook*>* Component_Hooks::getLocator() const
// ************************************************
{
    return new Locator(_component);
}

string Component_Hooks::_getString() const
// ***************************************
{
    string s = "<" + _TName("Component::Hooks");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Component_Hooks::Locator implementation
// ****************************************************************************************************

Component_Hooks::Locator::Locator(const Component* component)
// **********************************************************
:    Inherit(),
    _component(component),
    _hook(NULL)
{
    if (_component) _hook = ((Component*)_component)->getBodyHook();
}

Component_Hooks::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _component(locator._component),
    _hook(locator._hook)
{
}

Component_Hooks::Locator& Component_Hooks::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _component = locator._component;
    _hook = locator._hook;
    return *this;
}

Hook* Component_Hooks::Locator::getElement() const
// ***********************************************
{
    return _hook;
}

Locator<Hook*>* Component_Hooks::Locator::getClone() const
// *******************************************************
{
    return new Locator(*this);
}

bool Component_Hooks::Locator::isValid() const
// *******************************************
{
    return (_hook != NULL);
}

void Component_Hooks::Locator::progress()
// **************************************
{
    _hook = NULL;
}

string Component_Hooks::Locator::_getString() const
// ************************************************
{
    string s = "<" + _TName("Component::Hooks::Locator");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Component_ConnexComponents implementation
// ****************************************************************************************************

Component_ConnexComponents::Component_ConnexComponents(const Component* component)
// *******************************************************************************
:     Inherit(),
    _component(component)
{
}

Component_ConnexComponents::Component_ConnexComponents(const Component_ConnexComponents& connexComponents)
// ****************************************************************************************************
:     Inherit(),
    _component(connexComponents._component)
{
}

Component_ConnexComponents&
    Component_ConnexComponents::operator=(const Component_ConnexComponents& connexComponents)
// *****************************************************************************************
{
    _component = connexComponents._component;
    return *this;
}

Collection<Component*>* Component_ConnexComponents::getClone() const
// *****************************************************************
{
    return new Component_ConnexComponents(*this);
}

Locator<Component*>* Component_ConnexComponents::getLocator() const
// ****************************************************************
{
    return new Locator(_component);
}

string Component_ConnexComponents::_getString() const
// **************************************************
{
    string s = "<" + _TName("Component::ConnexComponents");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Component_ConnexComponents::Locator implementation
// ****************************************************************************************************

Component_ConnexComponents::Locator::Locator(const Component* component)
// *********************************************************************
:    Inherit(),
    _component(component),
    _componentSet(),
    _componentStack()
{
    if (_component) {
        _componentSet.insert((Component*)_component);
        _componentStack.push((Component*)_component);
    }
}

Component_ConnexComponents::Locator::Locator(const Locator& locator)
// *****************************************************************
:    Inherit(),
    _component(locator._component),
    _componentSet(locator._componentSet),
    _componentStack(locator._componentStack)
{
}

Component_ConnexComponents::Locator& Component_ConnexComponents::Locator::operator=(const Locator& locator)
// ****************************************************************************************************
{
    _component = locator._component;
    _componentSet = locator._componentSet;
    _componentStack = locator._componentStack;
    return *this;
}

Component* Component_ConnexComponents::Locator::getElement() const
// ***************************************************************
{
    return _componentStack.top();
}

Locator<Component*>* Component_ConnexComponents::Locator::getClone() const
// ***********************************************************************
{
    return new Locator(*this);
}

bool Component_ConnexComponents::Locator::isValid() const
// ******************************************************
{
    return !_componentStack.empty();
}

void Component_ConnexComponents::Locator::progress()
// *************************************************
{
    if (!_componentStack.empty()) {
        Component* component = _componentStack.top();
        _componentStack.pop();
        for_each_hook(componentHook, component->getHooks()) {
            Hook* masterHook = componentHook->getMasterHook();
            if (masterHook) {
                for_each_hook(slaveHook, masterHook->getSlaveHooks()) {
                    Component* component = slaveHook->getComponent();
                    if (_componentSet.find(component) == _componentSet.end()) {
                        _componentSet.insert(component);
                        _componentStack.push(component);
                    }
                    end_for;
                }
                Component* component = masterHook->getComponent();
                if (_componentSet.find(component) == _componentSet.end()) {
                    _componentSet.insert(component);
                    _componentStack.push(component);
                }
            }
            end_for;
        }
    }
}

string Component_ConnexComponents::Locator::_getString() const
// ***********************************************************
{
    string s = "<" + _TName("Component::ConnexComponents::Locator");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Component_SlaveComponents implementation
// ****************************************************************************************************

Component_SlaveComponents::Component_SlaveComponents(const Component* component)
// *****************************************************************************
:     Inherit(),
    _component(component)
{
}

Component_SlaveComponents::Component_SlaveComponents(const Component_SlaveComponents& slaveComponents)
// ***************************************************************************************************
:     Inherit(),
    _component(slaveComponents._component)
{
}

Component_SlaveComponents&
    Component_SlaveComponents::operator=(const Component_SlaveComponents& slaveComponents)
// **************************************************************************************
{
    _component = slaveComponents._component;
    return *this;
}

Collection<Component*>* Component_SlaveComponents::getClone() const
// ****************************************************************
{
    return new Component_SlaveComponents(*this);
}

Locator<Component*>* Component_SlaveComponents::getLocator() const
// ***************************************************************
{
    return new Locator(_component);
}

string Component_SlaveComponents::_getString() const
// *************************************************
{
    string s = "<" + _TName("Component::SlaveComponents");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Component_SlaveComponents::Locator implementation
// ****************************************************************************************************

Component_SlaveComponents::Locator::Locator(const Component* component)
// ********************************************************************
:    Inherit(),
    _component(component),
    _componentSet(),
    _componentStack()
{
    if (_component) {
        _componentSet.insert((Component*)_component);
        Hook* masterHook = ((Component*)_component)->getBodyHook();
        for_each_hook(slaveHook, masterHook->getSlaveHooks()) {
            Component* component = slaveHook->getComponent();
            if (_componentSet.find(component) == _componentSet.end()) {
                _componentSet.insert(component);
                _componentStack.push(component);
            }
            end_for;
        }
    }
}

Component_SlaveComponents::Locator::Locator(const Locator& locator)
// ****************************************************************
:    Inherit(),
    _component(locator._component),
    _componentSet(locator._componentSet),
    _componentStack(locator._componentStack)
{
}

Component_SlaveComponents::Locator&
    Component_SlaveComponents::Locator::operator=(const Locator& locator)
// *********************************************************************
{
    _component = locator._component;
    _componentSet = locator._componentSet;
    _componentStack = locator._componentStack;
    return *this;
}

Component* Component_SlaveComponents::Locator::getElement() const
// **************************************************************
{
    return _componentStack.top();
}

Locator<Component*>* Component_SlaveComponents::Locator::getClone() const
// **********************************************************************
{
    return new Locator(*this);
}

bool Component_SlaveComponents::Locator::isValid() const
// *****************************************************
{
    return !_componentStack.empty();
}

void Component_SlaveComponents::Locator::progress()
// ************************************************
{
    if (!_componentStack.empty()) {
        Component* component = _componentStack.top();
        _componentStack.pop();
        Hook* masterHook = component->getBodyHook();
        for_each_hook(slaveHook, masterHook->getSlaveHooks()) {
            Component* component = slaveHook->getComponent();
            if (_componentSet.find(component) == _componentSet.end()) {
                _componentSet.insert(component);
                _componentStack.push(component);
            }
            end_for;
        }
    }
}

string Component_SlaveComponents::Locator::_getString() const
// **********************************************************
{
    string s = "<" + _TName("Component::SlaveComponents::Locator");
    if (_component) s += " " + getString(_component);
    s += ">";
    return s;
}

double  getArea ( Component* component )
//**************************************
{
  Box  bb = component->getBoundingBox ();

  return DbU::getGrid(bb.getWidth()) * DbU::getGrid(bb.getHeight());
}


// ****************************************************************************************************
// JsonComponent implementation
// ****************************************************************************************************

JsonComponent::JsonComponent(unsigned long flags)
// **********************************************
  : JsonEntity(flags)
{
  add( "_bodyHook", typeid(string) );
}


} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
