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

        public: virtual Component* getComponent() const;

        public: virtual bool IsMaster() const {return true;};

        public: virtual string _getTypeName() const { return _TName("Component::BodyHook"); };
        public: virtual string _getString() const;

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

    public: virtual Cell* getCell() const;
    public: Net* getNet() const {return _net;};
    public: Rubber* getRubber() const {return _rubber;};
    public: Hook* getBodyHook() {return &_bodyHook;};
    public: virtual Hooks getHooks() const;
    public: virtual Unit getX() const = 0;
    public: virtual Unit getY() const = 0;
    public: virtual Point getPosition() const {return Point(getX(), getY());};
    public: virtual Point getCenter() const {return getPosition();};
    public: virtual Layer* getLayer() const = 0;
    public: virtual Box getBoundingBox() const = 0;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const = 0;
    public: Components getConnexComponents() const;
    public: Components getSlaveComponents() const;

// Updators
// ********

    public: virtual void materialize();
    public: virtual void unmaterialize();
    public: virtual void Invalidate(bool propagateFlag = true);

// Filters
// *******

    public: static ComponentFilter getIsUnderFilter(const Box& area);

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Component* _getNextOfNetComponentSet() const {return _nextOfNetComponentSet;};

    public: void _setNet(Net* net);
    public: void _setRubber(Rubber* rubber);
    public: void _setNextOfNetComponentSet(Component* component) {_nextOfNetComponentSet = component;};

};


double  getArea ( Component* component );



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Component)
SetNestedSlotAdapter(Hurricane::Component::BodyHook)

#endif // HURRICANE_COMPONENT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
