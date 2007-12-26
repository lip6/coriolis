// ****************************************************************************************************
// File: Component.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Component.h"
#include "Net.h"
#include "Cell.h"
#include "Rubber.h"
#include "Slice.h"
#include "BasicLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Component_IsUnderFilter : public Filter<Component*> {
// ******************************************************

	public: Box _area;

	public: Component_IsUnderFilter(const Box& area)
	// *********************************************
	:	_area(area)
	{
	};

	public: Component_IsUnderFilter(const Component_IsUnderFilter& filter)
	// *******************************************************************
	:	_area(filter._area)
	{
	};

	public: Component_IsUnderFilter& operator=(const Component_IsUnderFilter& filter)
	// ******************************************************************************
	{
		_area = filter._area;
		return *this;
	};

	public: virtual Filter<Component*>* GetClone() const
	// *************************************************
	{
		return new Component_IsUnderFilter(*this);
	};

	public: virtual bool Accept(Component* component) const
	// ****************************************************
	{
		return _area.Intersect(component->GetBoundingBox());
	};

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _TName("Component::IsUnderFilter") + " " + GetString(_area) + ">";
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

		public: virtual Hook* GetElement() const;
		public: virtual Hurricane::Locator<Hook*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Hook*>* GetClone() const;
	public: virtual Hurricane::Locator<Hook*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

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

		public: virtual Component* GetElement() const;
		public: virtual Hurricane::Locator<Component*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Component*>* GetClone() const;
	public: virtual Hurricane::Locator<Component*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

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

		public: virtual Component* GetElement() const;
		public: virtual Hurricane::Locator<Component*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Component*>* GetClone() const;
	public: virtual Hurricane::Locator<Component*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;
};



// ****************************************************************************************************
// Component implementation
// ****************************************************************************************************

Component::Component(Net* net, bool inPlugCreate)
// **********************************************
:	Inherit(),
	_net(net),
	_rubber(NULL),
	_bodyHook(this),
	_nextOfNetComponentSet(NULL)
{
	if (!inPlugCreate && !_net)
		throw Error("Can't create " + _TName("Component") + " : null net");
}

Cell* Component::GetCell() const
// *****************************
{
	return _net->GetCell();
}

Hooks Component::GetHooks() const
// ******************************
{
	return Component_Hooks(this);
}

Components Component::GetConnexComponents() const
// **********************************************
{
	return Component_ConnexComponents(this);
}

Components Component::GetSlaveComponents() const
// *********************************************
{
	return Component_SlaveComponents(this);
}

ComponentFilter Component::GetIsUnderFilter(const Box& area)
// *********************************************************
{
	return Component_IsUnderFilter(area);
}

void Component::Materialize()
// **************************
{
// trace << "Materialize() - " << this << endl;

	if (!IsMaterialized()) {
		Cell*  cell  = GetCell();
		Layer* layer = GetLayer();
        if (cell && layer) {
            Slice* slice = cell->GetSlice(layer);
            if (!slice) slice = Slice::_Create(cell, layer);
            QuadTree* quadTree = slice->_GetQuadTree();
            quadTree->Insert(this);
            cell->_Fit(quadTree->GetBoundingBox());
        } else {
          //cerr << "[WARNING] " << this << " not inserted into QuadTree." << endl;
        }
	}
}

void Component::Unmaterialize()
// ****************************
{
// trace << "Unmaterializing " << this << endl;

	if (IsMaterialized()) {
		Cell* cell = GetCell();
		Slice* slice = cell->GetSlice(GetLayer());
		if (slice) {
			cell->_Unfit(GetBoundingBox());
			slice->_GetQuadTree()->Remove(this);
			if (slice->IsEmpty()) slice->_Delete();
		}
	}
}

void Component::Invalidate(bool propagateFlag)
// *******************************************
{
	Inherit::Invalidate(false);

	if (propagateFlag) {
		Rubber* rubber = GetRubber();
		if (rubber) rubber->Invalidate();
		for_each_component(component, GetSlaveComponents()) {
			component->Invalidate(false);
			end_for;
		}
	}
}

void Component::_PostCreate()
// **************************
{
	if (_net) _net->_GetComponentSet()._Insert(this);

	Inherit::_PostCreate();
}

void Component::_PreDelete()
// *************************
{
// trace << "entering Component::_PreDelete: " << this << endl;
// trace_in();

    ClearProperties();

    set<Component*> componentSet;
    GetSlaveComponents().Fill(componentSet);

    set<Hook*> masterHookSet;
    componentSet.insert(this);
    for_each_component(component, GetCollection(componentSet)) {
        component->Unmaterialize();
        for_each_hook(hook, component->GetHooks()) {
            for_each_hook(hook, hook->GetHooks()) {
                if (hook->IsMaster() && (componentSet.find(hook->GetComponent()) == componentSet.end()))
                    masterHookSet.insert(hook);
                end_for;
            }
            if (!hook->IsMaster()) hook->Detach();
            end_for;
        }
        end_for;
    }

    componentSet.erase(this);
    for_each_component(component, GetCollection(componentSet)) {
        component->Delete();
        end_for;
    }

    set<Rubber*> rubberSet;
    set<Hook*> mainMasterHookSet;
    for_each_hook(hook, GetCollection(masterHookSet)) {
        Rubber* rubber = hook->GetComponent()->GetRubber();
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
    for_each_hook(hook, GetCollection(mainMasterHookSet)) {
        if (!masterHook)
            masterHook = hook;
        else
            hook->Merge(masterHook);
        end_for;
    }
    /**/

    _bodyHook.Detach();

    Inherit::_PreDelete();

    if (_net) _net->_GetComponentSet()._Remove(this);


    // trace << "exiting Component::_PreDelete:" << endl;
    // trace_out();
}

string Component::_GetString() const
// *********************************
{
	string s = Inherit::_GetString();
	if (!_net)
		s.insert(s.length() - 1, " UNCONNECTED");
	else
		s.insert(s.length() - 1, " " + GetString(_net->GetName()));
	return s;
}

Record* Component::_GetRecord() const
// ****************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Net", _net));
		record->Add(GetSlot("Rubber", _rubber));
		record->Add(GetSlot("BodyHook", &_bodyHook));
	}
	return record;
}

void Component::_SetNet(Net* net)
// ******************************
{
	if (net != _net) {
		if (_net) _net->_GetComponentSet()._Remove(this);
		_net = net;
		if (_net) _net->_GetComponentSet()._Insert(this);
	}
}

void Component::_SetRubber(Rubber* rubber)
// ***************************************
{
	if (rubber != _rubber) {
		if (_rubber) _rubber->_Release();
		_rubber = rubber;
		if (_rubber) _rubber->_Capture();
	}
}

//bool Component::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// *****************************************************************************************
//{
//	Box area(point);
//	area.Inflate(aperture);
//	for_each_basic_layer(basicLayer, GetLayer()->GetBasicLayers()) {
//		if (view->IsVisible(basicLayer) && GetBoundingBox(basicLayer).Intersect(area))
//			return true;
//		end_for;
//	}
//
//	return false;
//}
//
//void Component::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// ************************************************************************************************
//{
//	for_each_basic_layer(basicLayer, GetLayer()->GetBasicLayers()) {
//		_Draw(view, basicLayer, updateArea, transformation);
//		end_for;
//	}
//}
//

// ****************************************************************************************************
// Component::BodyHook implementation
// ****************************************************************************************************

static int BODY_HOOK_OFFSET = -1;

Component::BodyHook::BodyHook(Component* component)
// ************************************************
:	Inherit()
{
	if (!component)
		throw Error("Can't create " + _TName("Component::BodyHook") + " : null component");

	if (BODY_HOOK_OFFSET == -1)
		BODY_HOOK_OFFSET = (unsigned long)this - (unsigned long)component;
}

Component* Component::BodyHook::GetComponent() const
// *************************************************
{
	return (Component*)((unsigned long)this - BODY_HOOK_OFFSET);
}

string Component::BodyHook::_GetString() const
// *******************************************
{
	return "<" + _TName("Component::BodyHook") + " " + GetString(GetComponent()) + ">";
}

// ****************************************************************************************************
// Component_Hooks implementation
// ****************************************************************************************************

Component_Hooks::Component_Hooks(const Component* component)
// *********************************************************
: 	Inherit(),
	_component(component)
{
}

Component_Hooks::Component_Hooks(const Component_Hooks& hooks)
// ***********************************************************
: 	Inherit(),
	_component(hooks._component)
{
}

Component_Hooks& Component_Hooks::operator=(const Component_Hooks& hooks)
// **********************************************************************
{
	_component = hooks._component;
	return *this;
}

Collection<Hook*>* Component_Hooks::GetClone() const
// *************************************************
{
	return new Component_Hooks(*this);
}

Locator<Hook*>* Component_Hooks::GetLocator() const
// ************************************************
{
	return new Locator(_component);
}

string Component_Hooks::_GetString() const
// ***************************************
{
	string s = "<" + _TName("Component::Hooks");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Component_Hooks::Locator implementation
// ****************************************************************************************************

Component_Hooks::Locator::Locator(const Component* component)
// **********************************************************
:	Inherit(),
	_component(component),
	_hook(NULL)
{
	if (_component) _hook = ((Component*)_component)->GetBodyHook();
}

Component_Hooks::Locator::Locator(const Locator& locator)
// ******************************************************
:	Inherit(),
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

Hook* Component_Hooks::Locator::GetElement() const
// ***********************************************
{
	return _hook;
}

Locator<Hook*>* Component_Hooks::Locator::GetClone() const
// *******************************************************
{
	return new Locator(*this);
}

bool Component_Hooks::Locator::IsValid() const
// *******************************************
{
	return (_hook != NULL);
}

void Component_Hooks::Locator::Progress()
// **************************************
{
	_hook = NULL;
}

string Component_Hooks::Locator::_GetString() const
// ************************************************
{
	string s = "<" + _TName("Component::Hooks::Locator");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Component_ConnexComponents implementation
// ****************************************************************************************************

Component_ConnexComponents::Component_ConnexComponents(const Component* component)
// *******************************************************************************
: 	Inherit(),
	_component(component)
{
}

Component_ConnexComponents::Component_ConnexComponents(const Component_ConnexComponents& connexComponents)
// ****************************************************************************************************
: 	Inherit(),
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

Collection<Component*>* Component_ConnexComponents::GetClone() const
// *****************************************************************
{
	return new Component_ConnexComponents(*this);
}

Locator<Component*>* Component_ConnexComponents::GetLocator() const
// ****************************************************************
{
	return new Locator(_component);
}

string Component_ConnexComponents::_GetString() const
// **************************************************
{
	string s = "<" + _TName("Component::ConnexComponents");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Component_ConnexComponents::Locator implementation
// ****************************************************************************************************

Component_ConnexComponents::Locator::Locator(const Component* component)
// *********************************************************************
:	Inherit(),
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
:	Inherit(),
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

Component* Component_ConnexComponents::Locator::GetElement() const
// ***************************************************************
{
	return _componentStack.top();
}

Locator<Component*>* Component_ConnexComponents::Locator::GetClone() const
// ***********************************************************************
{
	return new Locator(*this);
}

bool Component_ConnexComponents::Locator::IsValid() const
// ******************************************************
{
	return !_componentStack.empty();
}

void Component_ConnexComponents::Locator::Progress()
// *************************************************
{
	if (!_componentStack.empty()) {
		Component* component = _componentStack.top();
		_componentStack.pop();
		for_each_hook(componentHook, component->GetHooks()) {
			Hook* masterHook = componentHook->GetMasterHook();
			if (masterHook) {
				for_each_hook(slaveHook, masterHook->GetSlaveHooks()) {
					Component* component = slaveHook->GetComponent();
					if (_componentSet.find(component) == _componentSet.end()) {
						_componentSet.insert(component);
						_componentStack.push(component);
					}
					end_for;
				}
				Component* component = masterHook->GetComponent();
				if (_componentSet.find(component) == _componentSet.end()) {
					_componentSet.insert(component);
					_componentStack.push(component);
				}
			}
			end_for;
		}
	}
}

string Component_ConnexComponents::Locator::_GetString() const
// ***********************************************************
{
	string s = "<" + _TName("Component::ConnexComponents::Locator");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Component_SlaveComponents implementation
// ****************************************************************************************************

Component_SlaveComponents::Component_SlaveComponents(const Component* component)
// *****************************************************************************
: 	Inherit(),
	_component(component)
{
}

Component_SlaveComponents::Component_SlaveComponents(const Component_SlaveComponents& slaveComponents)
// ***************************************************************************************************
: 	Inherit(),
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

Collection<Component*>* Component_SlaveComponents::GetClone() const
// ****************************************************************
{
	return new Component_SlaveComponents(*this);
}

Locator<Component*>* Component_SlaveComponents::GetLocator() const
// ***************************************************************
{
	return new Locator(_component);
}

string Component_SlaveComponents::_GetString() const
// *************************************************
{
	string s = "<" + _TName("Component::SlaveComponents");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Component_SlaveComponents::Locator implementation
// ****************************************************************************************************

Component_SlaveComponents::Locator::Locator(const Component* component)
// ********************************************************************
:	Inherit(),
	_component(component),
	_componentSet(),
	_componentStack()
{
	if (_component) {
		_componentSet.insert((Component*)_component);
		Hook* masterHook = ((Component*)_component)->GetBodyHook();
		for_each_hook(slaveHook, masterHook->GetSlaveHooks()) {
			Component* component = slaveHook->GetComponent();
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
:	Inherit(),
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

Component* Component_SlaveComponents::Locator::GetElement() const
// **************************************************************
{
	return _componentStack.top();
}

Locator<Component*>* Component_SlaveComponents::Locator::GetClone() const
// **********************************************************************
{
	return new Locator(*this);
}

bool Component_SlaveComponents::Locator::IsValid() const
// *****************************************************
{
	return !_componentStack.empty();
}

void Component_SlaveComponents::Locator::Progress()
// ************************************************
{
	if (!_componentStack.empty()) {
		Component* component = _componentStack.top();
		_componentStack.pop();
		Hook* masterHook = component->GetBodyHook();
		for_each_hook(slaveHook, masterHook->GetSlaveHooks()) {
			Component* component = slaveHook->GetComponent();
			if (_componentSet.find(component) == _componentSet.end()) {
				_componentSet.insert(component);
				_componentStack.push(component);
			}
			end_for;
		}
	}
}

string Component_SlaveComponents::Locator::_GetString() const
// **********************************************************
{
	string s = "<" + _TName("Component::SlaveComponents::Locator");
	if (_component) s += " " + GetString(_component);
	s += ">";
	return s;
}

double  GetArea ( Component* component )
//**************************************
{
  Box  bb = component->GetBoundingBox ();

  return GetValue(bb.GetWidth()) * GetValue(bb.GetHeight());
}




} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
