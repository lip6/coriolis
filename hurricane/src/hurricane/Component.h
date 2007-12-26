// ****************************************************************************************************
// File: Component.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_COMPONENT
#define HURRICANE_COMPONENT

#include "Go.h"
#include "Components.h"
#include "Hook.h"
#include "Hooks.h"
#include "Interval.h"

namespace Hurricane {

class Net;
class Rubber;
class Layer;



// ****************************************************************************************************
// Component declaration
// ****************************************************************************************************

class Component : public Go {
// ************************

// Types
// *****

	public: typedef Go Inherit;

	public: class BodyHook : public Hook {
	// *********************************

		friend class Component;

		public: typedef Hook Inherit;

		private: BodyHook(Component* component);

		public: virtual Component* GetComponent() const;

		public: virtual bool IsMaster() const {return true;};

        public: virtual string _GetTypeName() const { return _TName("Component::BodyHook"); };
		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: Net* _net;
	private: Rubber* _rubber;
	private: BodyHook _bodyHook;
	private: Component* _nextOfNetComponentSet;

// Constructors
// ************

	protected: Component(Net* net, bool inPlugCreate = false);

// Accessors
// *********

	public: virtual Cell* GetCell() const;
	public: Net* GetNet() const {return _net;};
	public: Rubber* GetRubber() const {return _rubber;};
	public: Hook* GetBodyHook() {return &_bodyHook;};
	public: virtual Hooks GetHooks() const;
	public: virtual Unit GetX() const = 0;
	public: virtual Unit GetY() const = 0;
	public: virtual Point GetPosition() const {return Point(GetX(), GetY());};
	public: virtual Point GetCenter() const {return GetPosition();};
	public: virtual Layer* GetLayer() const = 0;
	public: virtual Box GetBoundingBox() const = 0;
	public: virtual Box GetBoundingBox(BasicLayer* basicLayer) const = 0;
	public: Components GetConnexComponents() const;
	public: Components GetSlaveComponents() const;

// Updators
// ********

	public: virtual void Materialize();
	public: virtual void Unmaterialize();
	public: virtual void Invalidate(bool propagateFlag = true);

// Filters
// *******

	public: static ComponentFilter GetIsUnderFilter(const Box& area);

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: Component* _GetNextOfNetComponentSet() const {return _nextOfNetComponentSet;};

	public: void _SetNet(Net* net);
	public: void _SetRubber(Rubber* rubber);
	public: void _SetNextOfNetComponentSet(Component* component) {_nextOfNetComponentSet = component;};

	//public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


double  GetArea ( Component* component );



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Component)
SetNestedSlotAdapter(Hurricane::Component::BodyHook)

#endif // HURRICANE_COMPONENT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
