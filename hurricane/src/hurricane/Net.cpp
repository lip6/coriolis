// ****************************************************************************************************
// File: Net.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Net.h"
#include "Cell.h"
#include "Instance.h"
#include "Plug.h"
#include "RoutingPad.h"
#include "RoutingPads.h"
#include "Pin.h"
#include "Contact.h"
#include "Vertical.h"
#include "Horizontal.h"
#include "Pad.h"
#include "UpdateSession.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Net_IsCellNetFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsCellNetFilter() {};

    public: Net_IsCellNetFilter(const Net_IsCellNetFilter& filter) {};

    public: Net_IsCellNetFilter& operator=(const Net_IsCellNetFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsCellNetFilter(*this);};

    public: virtual bool accept(Net* net) const {return !net->IsDeepNet();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsCellNetFilter>");};

};

class Net_IsDeepNetFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsDeepNetFilter() {};

    public: Net_IsDeepNetFilter(const Net_IsDeepNetFilter& filter) {};

    public: Net_IsDeepNetFilter& operator=(const Net_IsDeepNetFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsDeepNetFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsDeepNet();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsDeepNetFilter>");};

};

class Net_IsGlobalFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsGlobalFilter() {};

    public: Net_IsGlobalFilter(const Net_IsGlobalFilter& filter) {};

    public: Net_IsGlobalFilter& operator=(const Net_IsGlobalFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsGlobalFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsGlobal();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsGlobalFilter>");};

};

class Net_IsExternalFilter : public Filter<Net*> {
// *********************************************

    public: Net_IsExternalFilter() {};

    public: Net_IsExternalFilter(const Net_IsExternalFilter& filter) {};

    public: Net_IsExternalFilter& operator=(const Net_IsExternalFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsExternalFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsExternal();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsExternalFilter>");};

};

class Net_IsClockFilter : public Filter<Net*> {
// ******************************************

    public: Net_IsClockFilter() {};

    public: Net_IsClockFilter(const Net_IsClockFilter& filter) {};

    public: Net_IsClockFilter& operator=(const Net_IsClockFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsClockFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsClock();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsClockFilter>");};

};

class Net_IsSupplyFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsSupplyFilter() {};

    public: Net_IsSupplyFilter(const Net_IsSupplyFilter& filter) {};

    public: Net_IsSupplyFilter& operator=(const Net_IsSupplyFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsSupplyFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsSupply();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsSupplyFilter>");};

};

class Net_IsPowerFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsPowerFilter() {};

    public: Net_IsPowerFilter(const Net_IsPowerFilter& filter) {};

    public: Net_IsPowerFilter& operator=(const Net_IsPowerFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsPowerFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsPower();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsPowerFilter>");};

};

class Net_IsGroundFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsGroundFilter() {};

    public: Net_IsGroundFilter(const Net_IsGroundFilter& filter) {};

    public: Net_IsGroundFilter& operator=(const Net_IsGroundFilter& filter) {return *this;};

    public: virtual Filter<Net*>* getClone() const {return new Net_IsGroundFilter(*this);};

    public: virtual bool accept(Net* net) const {return net->IsGround();};

    public: virtual string _getString() const {return "<" + _TName("Net::IsGroundFilter>");};

};



// ****************************************************************************************************
// Net_SlavePlugs implementation
// ****************************************************************************************************

class Net_SlavePlugs : public Collection<Plug*> {
// ********************************************

// Types
// *****

    public: typedef Collection<Plug*> Inherit;

    public: class Locator : public Hurricane::Locator<Plug*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Plug*> Inherit;

        private: const Net* _net;
        private: Plug* _plug;
        private: InstanceLocator _instanceLocator;

        public: Locator(const Net* net = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Plug* getElement() const;
        public: virtual Hurricane::Locator<Plug*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Net* _net;

// Constructors
// ************

    public: Net_SlavePlugs(const Net* net = NULL);
    public: Net_SlavePlugs(const Net_SlavePlugs& slavePlugs);

// Operators
// *********

    public: Net_SlavePlugs& operator=(const Net_SlavePlugs& slavePlugs);

// Accessors
// *********

    public: virtual Collection<Plug*>* getClone() const;
    public: virtual Hurricane::Locator<Plug*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Net implementation
// ****************************************************************************************************

Net::Net(Cell* cell, const Name& name)
// ***********************************
:    Inherit(),
    _cell(cell),
    _name(name),
    _arity(1),
    _isGlobal(false),
    _isExternal(false),
    _type(),
    _direction(),
    _position(0, 0),
    _componentSet(),
    _rubberSet(),
    _nextOfCellNetMap(NULL)
{
    if (!_cell)
        throw Error("Can't create " + _TName("Net") + " : null cell");

    if (name.IsEmpty())
        throw Error("Can't create " + _TName("Net") + " : empty name");

    if (_cell->getNet(_name))
        throw Error("Can't create " + _TName("Net ") + getString(_name) + " : already exists");
}

Net* Net::create(Cell* cell, const Name& name)
// *******************************************
{
    Net* net = new Net(cell, name);

    net->_postCreate();

    return net;
}

Box Net::getBoundingBox() const
// ****************************
{
    Box boundingBox;
    for_each_component(component, getComponents()) {
        boundingBox.merge(component->getBoundingBox());
        end_for;
    }
    return boundingBox;
}

RoutingPads Net::getRoutingPads() const
// ************************
{
    // return getComponents().getSubSet<RoutingPad*>();
    return SubTypeCollection<Component*, RoutingPad*>(getComponents());
}

Plugs Net::getPlugs() const
// ************************
{
    // return getComponents().getSubSet<Plug*>();
    return SubTypeCollection<Component*, Plug*>(getComponents());
}

Pins Net::getPins() const
// **********************
{
    // return getComponents().getSubSet<Pin*>();
    return SubTypeCollection<Component*, Pin*>(getComponents());
}

Contacts Net::getContacts() const
// ******************************
{
    // return getComponents().getSubSet<Contact*>();
    return SubTypeCollection<Component*, Contact*>(getComponents());
}

Segments Net::getSegments() const
// ******************************
{
    // return getComponents().getSubSet<Segment*>();
    return SubTypeCollection<Component*, Segment*>(getComponents());
}

Verticals Net::getVerticals() const
// ********************************
{
    // return getComponents().getSubSet<Vertical*>();
    return SubTypeCollection<Component*, Vertical*>(getComponents());
}

Horizontals Net::getHorizontals() const
// ************************************
{
    // return getComponents().getSubSet<Horizontal*>();
    return SubTypeCollection<Component*, Horizontal*>(getComponents());
}

Pads Net::getPads() const
// **********************
{
    // return getComponents().getSubSet<Pad*>();
    return SubTypeCollection<Component*, Pad*>(getComponents());
}

Plugs Net::getSlavePlugs() const
// *****************************
{
    return Net_SlavePlugs(this);
}

Plugs Net::getConnectedSlavePlugs() const
// **************************************
{
    return getSlavePlugs().getSubSet(Plug::getIsConnectedFilter());
}

Plugs Net::getUnconnectedSlavePlugs() const
// ****************************************
{
    return getSlavePlugs().getSubSet(Plug::getIsUnconnectedFilter());
}

NetFilter Net::getIsCellNetFilter()
// *******************************
{
    return Net_IsCellNetFilter();
}

NetFilter Net::getIsDeepNetFilter()
// *******************************
{
    return Net_IsDeepNetFilter();
}

NetFilter Net::getIsGlobalFilter()
// *******************************
{
    return Net_IsGlobalFilter();
}

NetFilter Net::getIsExternalFilter()
// *********************************
{
    return Net_IsExternalFilter();
}

NetFilter Net::getIsInternalFilter()
// *********************************
{
    return !Net_IsExternalFilter();
}

NetFilter Net::getIsClockFilter()
// ******************************
{
    return Net_IsClockFilter();
}

NetFilter Net::getIsSupplyFilter()
// *******************************
{
    return Net_IsSupplyFilter();
}

NetFilter Net::getIsPowerFilter()
// *******************************
{
    return Net_IsPowerFilter();
}

NetFilter Net::getIsGroundFilter()
// *******************************
{
    return Net_IsGroundFilter();
}

void Net::SetName(const Name& name)
// ********************************
{
    if (name != _name) {
        if (name.IsEmpty())
            throw Error("Can't change net name : empty name");

        if (_cell->getNet(name))
            throw Error("Can't change net name : already exists");

        _cell->_getNetMap()._Remove(this);
        _name = name;
        _cell->_getNetMap()._Insert(this);
    }
}

void Net::SetArity(const Arity& arity)
// ***********************************
{
    _arity = arity;
}

void Net::SetGlobal(bool isGlobal)
// *******************************
{
    _isGlobal = isGlobal;
}

void Net::SetExternal(bool isExternal)
// ***********************************
{
    if (isExternal != _isExternal) {
        if (!isExternal) {
            if (!getConnectedSlavePlugs().IsEmpty())
                throw Error("Can't set internal : has connected slave plugs");
            _direction = Direction::UNDEFINED;
        }
        _isExternal = isExternal;
        if (_isExternal) {
            OpenUpdateSession();
            SetPosition(Point(0, 0));
            for_each_instance(instance, _cell->getSlaveInstances()) {
                Plug::_create(instance, this);
                end_for;
            }
            CloseUpdateSession();
        }
    }
}

void Net::SetType(const Type& type)
// ********************************
{
    _type = type;
}

void Net::SetPosition(const Point& position)
// *****************************************
{
    if (_position != position) {
        for_each_plug(plug, getSlavePlugs()) {
            plug->Invalidate(true);
            end_for;
        }
        _position = position;
    }
}

void Net::SetDirection(const Direction& direction)
// ***********************************************
{
    _direction = direction;
}

void Net::materialize()
// ********************
{
    for_each_component(component, getComponents()) {
        component->materialize();
        end_for;
    }
    for_each_rubber(rubber, getRubbers()) {
        rubber->materialize();
        end_for;
    }
}

void Net::unmaterialize()
// **********************
{
    for_each_rubber(rubber, getRubbers()) {
        rubber->unmaterialize();
        end_for;
    }
    for_each_component(component, getComponents()) {
        component->unmaterialize();
        end_for;
    }
}

static void MergeNets(Net* net1, Net* net2)
// ****************************************
{
    assert(net1);
    assert(net2);

    if (net2->getName()[0] != '~') {
        if ((net1->getName()[0] == '~') ||
             (net2->IsGlobal() && !net1->IsGlobal()) ||
             (net2->IsExternal() && !net1->IsExternal())) {
            Net* tmpNet = net1;
            net1 = net2;
            net2 = tmpNet;
        }
    }

    if (net2->IsExternal() && !net1->IsExternal()) {
        Net* tmpNet = net1;
        net1 = net2;
        net2 = tmpNet;
    }

    net1->Merge(net2);
}

void Net::Merge(Net* net)
// **********************
{
    if (!net)
        throw Error("Can't merge net : null net");

    if (net == this)
        throw Error("Can't merge net : itself");

    if (net->getCell() != _cell)
        throw Error("Can't merge net : incompatible net");

    if (!IsExternal() && net->IsExternal() && !net->getConnectedSlavePlugs().IsEmpty())
        throw Error("Can't merge net : incompatible net");

    for_each_rubber(rubber, net->getRubbers()) rubber->_setNet(this); end_for;
    for_each_component(component, net->getComponents()) component->_setNet(this); end_for;

    if (IsExternal() && net->IsExternal()) {
        for_each_plug(plug, net->getConnectedSlavePlugs()) {
            Plug* mainPlug = plug->getInstance()->getPlug(this);
            if (mainPlug->IsConnected() && (mainPlug->getNet() != plug->getNet()))
                MergeNets(mainPlug->getNet(), plug->getNet());
            end_for;
        }
        for_each_plug(plug, net->getConnectedSlavePlugs()) {
            Plug* mainPlug = plug->getInstance()->getPlug(this);
            if (!mainPlug->IsConnected()) mainPlug->SetNet(plug->getNet());
            Hook* masterHook = plug->getBodyHook();
            Hook* nextMasterHook = masterHook->getNextMasterHook();
            if (nextMasterHook != masterHook) {
                masterHook->Detach();
                mainPlug->getBodyHook()->Merge(nextMasterHook);
            }
            Hooks slaveHooks = masterHook->getSlaveHooks();
            while (!slaveHooks.IsEmpty()) {
                Hook* slaveHook = slaveHooks.getFirst();
                slaveHook->Detach();
                slaveHook->Attach(mainPlug->getBodyHook());
            }
            plug->_destroy();
            end_for;
        }
    }

    net->destroy();
}

void Net::_postCreate()
// ********************
{
    _cell->_getNetMap()._Insert(this);

    if (_isExternal) {
        for_each_instance(instance, _cell->getSlaveInstances()) {
            Plug::_create(instance, this);
            end_for;
        }
    }

    Inherit::_postCreate();
}

void Net::_preDestroy()
// *******************
{
    Inherit::_preDestroy();

    for_each_plug(slavePlug, getSlavePlugs()) slavePlug->_destroy(); end_for;

    unmaterialize();

    for_each_rubber(rubber, getRubbers()) rubber->_destroy(); end_for;

    for_each_component(component, getComponents()) {
        for_each_hook(hook, component->getHooks()) {
            // 15 05 2006 xtof : detach all hooks in rings when
            // a net deletion occurs, can't see why master hooks were not detached.
            //if (!hook->IsMaster()) hook->Detach();
            hook->Detach();
            end_for;
        }
        end_for;
    }

    for_each_component(component, getComponents()) {
        if (!is_a<Plug*>(component))
            component->destroy();
        else
            ((Plug*)component)->SetNet(NULL);
        end_for;
    }

    _cell->_getNetMap()._Remove(this);
}

string Net::_getString() const
// ***************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_name));
    return s;
}

Record* Net::_getRecord() const
// **********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Cell", _cell));
        record->Add(getSlot("Name", &_name));
        record->Add(getSlot("Arity", &_arity));
        record->Add(getSlot("Global", &_isGlobal));
        record->Add(getSlot("External", &_isExternal));
        record->Add(getSlot("Type", _type));
        record->Add(getSlot("Direction", _direction));
        record->Add(getSlot("Position", &_position));
        record->Add(getSlot("Components", &_componentSet));
        record->Add(getSlot("Rubbers", &_rubberSet));
        record->Add(getSlot("External", &_isExternal));
    }
    return record;
}

// ****************************************************************************************************
// Net::Type implementation
// ****************************************************************************************************

Net::Type::Type(const Code& code)
// ******************************
:    _code(code)
{
}

Net::Type::Type(const Type& type)
// ******************************
:    _code(type._code)
{
}

Net::Type& Net::Type::operator=(const Type& type)
// **********************************************
{
    _code = type._code;
    return *this;
}

string Net::Type::_getString() const
// *********************************
{
  return getString(&_code);
}

Record* Net::Type::_getRecord() const
// ****************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("Code", &_code));
    return record;
}



// ****************************************************************************************************
// Net::Direction implementation
// ****************************************************************************************************

Net::Direction::Direction(const Code& code)
// ****************************************
:    _code(code)
{
}

Net::Direction::Direction(const Direction& direction)
// **************************************************
:    _code(direction._code)
{
}

Net::Direction& Net::Direction::operator=(const Direction& direction)
// ******************************************************************
{
    _code = direction._code;
    return *this;
}

string Net::Direction::_getString() const
// **************************************
{
  return getString(&_code);
}

Record* Net::Direction::_getRecord() const
// *********************************
{
    Record* record = new Record(getString(this));
    record->Add(getSlot("Code", &_code));
    return record;
}



// ****************************************************************************************************
// Net::ComponentSet implementation
// ****************************************************************************************************

Net::ComponentSet::ComponentSet()
// ******************************
:    Inherit()
{
}

unsigned Net::ComponentSet::_getHashValue(Component* component) const
// ******************************************************************
{
    return ( (unsigned int)( (unsigned long)component ) ) / 8;
}

Component* Net::ComponentSet::_getNextElement(Component* component) const
// **********************************************************************
{
    return component->_getNextOfNetComponentSet();
}

void Net::ComponentSet::_setNextElement(Component* component, Component* nextComponent) const
// ******************************************************************************************
{
    component->_setNextOfNetComponentSet(nextComponent);
}



// ****************************************************************************************************
// Net::RubberSet implementation
// ****************************************************************************************************

Net::RubberSet::RubberSet()
// ************************
:    Inherit()
{
}

unsigned Net::RubberSet::_getHashValue(Rubber* rubber) const
// *********************************************************
{
    return ( (unsigned int)( (unsigned long)rubber ) ) / 8;
}

Rubber* Net::RubberSet::_getNextElement(Rubber* rubber) const
// **********************************************************
{
    return rubber->_getNextOfNetRubberSet();
}

void Net::RubberSet::_setNextElement(Rubber* rubber, Rubber* nextRubber) const
// ***************************************************************************
{
    rubber->_setNextOfNetRubberSet(nextRubber);
}



// ****************************************************************************************************
// Net_SlavePlugs implementation
// ****************************************************************************************************

Net_SlavePlugs::Net_SlavePlugs(const Net* net)
// *******************************************
:     Inherit(),
    _net(net)
{
}

Net_SlavePlugs::Net_SlavePlugs(const Net_SlavePlugs& slavePlugs)
// *************************************************************
:     Inherit(),
    _net(slavePlugs._net)
{
}

Net_SlavePlugs& Net_SlavePlugs::operator=(const Net_SlavePlugs& slavePlugs)
// ************************************************************************
{
    _net = slavePlugs._net;
    return *this;
}

Collection<Plug*>* Net_SlavePlugs::getClone() const
// ************************************************
{
    return new Net_SlavePlugs(*this);
}

Locator<Plug*>* Net_SlavePlugs::getLocator() const
// ***********************************************
{
    return new Locator(_net);
}

string Net_SlavePlugs::_getString() const
// **************************************
{
    string s = "<" + _TName("Net::SlavePlugs");
    if (_net) s += " " + getString(_net);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Net_SlavePlugs::Locator implementation
// ****************************************************************************************************

Net_SlavePlugs::Locator::Locator(const Net* net)
// *********************************************
:    Inherit(),
    _net(net),
    _plug(NULL),
    _instanceLocator()
{
    if (_net) {
        _instanceLocator = _net->getCell()->getSlaveInstances().getLocator();
        while (!_plug && _instanceLocator.isValid()) {
            _plug = _instanceLocator.getElement()->getPlug(_net);
            _instanceLocator.progress();
        }
    }
}

Net_SlavePlugs::Locator::Locator(const Locator& locator)
// *****************************************************
:    Inherit(),
    _net(locator._net),
    _plug(locator._plug),
    _instanceLocator(locator._instanceLocator)
{
}

Net_SlavePlugs::Locator& Net_SlavePlugs::Locator::operator=(const Locator& locator)
// ********************************************************************************
{
    _net = locator._net;
    _plug = locator._plug;
    _instanceLocator = locator._instanceLocator;
    return *this;
}

Plug* Net_SlavePlugs::Locator::getElement() const
// **********************************************
{
    return _plug;
}

Locator<Plug*>* Net_SlavePlugs::Locator::getClone() const
// ******************************************************
{
    return new Locator(*this);
}

bool Net_SlavePlugs::Locator::isValid() const
// ******************************************
{
    return (_plug != NULL);
}

void Net_SlavePlugs::Locator::progress()
// *************************************
{
    if (isValid()) {
        _plug = NULL;
        while (!_plug && _instanceLocator.isValid()) {
            _plug = _instanceLocator.getElement()->getPlug(_net);
            _instanceLocator.progress();
        }
    }
}

string Net_SlavePlugs::Locator::_getString() const
// ***********************************************
{
    string s = "<" + _TName("Net::SlavePlugs::Locator");
    if (_net) s += " " + getString(_net);
    s += ">";
    return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
