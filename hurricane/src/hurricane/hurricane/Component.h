// ****************************************************************************************************
// File: ./hurricane/Component.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
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

#ifndef HURRICANE_COMPONENT
#define HURRICANE_COMPONENT

#include "hurricane/Go.h"
#include "hurricane/Components.h"
#include "hurricane/Hook.h"
#include "hurricane/Hooks.h"
#include "hurricane/Interval.h"

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

        public: virtual bool isMaster() const {return true;};

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
    public: virtual DbU::Unit getX() const = 0;
    public: virtual DbU::Unit getY() const = 0;
    public: virtual Point getPosition() const {return Point(getX(), getY());};
    public: virtual Point getCenter() const {return getPosition();};
    public: virtual const Layer* getLayer() const = 0;
    public: virtual Box getBoundingBox() const = 0;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const = 0;
    public: Components getConnexComponents() const;
    public: Components getSlaveComponents() const;

// Updators
// ********

    public: virtual void materialize();
    public: virtual void unmaterialize();
    public: virtual void invalidate(bool propagateFlag = true);

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


INSPECTOR_P_SUPPORT(Hurricane::Component);
INSPECTOR_P_SUPPORT(Hurricane::Component::BodyHook);


#endif // HURRICANE_COMPONENT


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
