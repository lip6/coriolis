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

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsCellNetFilter(*this);};

    public: virtual bool Accept(Net* net) const {return !net->IsDeepNet();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsCellNetFilter>");};

};

class Net_IsDeepNetFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsDeepNetFilter() {};

    public: Net_IsDeepNetFilter(const Net_IsDeepNetFilter& filter) {};

    public: Net_IsDeepNetFilter& operator=(const Net_IsDeepNetFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsDeepNetFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsDeepNet();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsDeepNetFilter>");};

};

class Net_IsGlobalFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsGlobalFilter() {};

    public: Net_IsGlobalFilter(const Net_IsGlobalFilter& filter) {};

    public: Net_IsGlobalFilter& operator=(const Net_IsGlobalFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsGlobalFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsGlobal();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsGlobalFilter>");};

};

class Net_IsExternalFilter : public Filter<Net*> {
// *********************************************

    public: Net_IsExternalFilter() {};

    public: Net_IsExternalFilter(const Net_IsExternalFilter& filter) {};

    public: Net_IsExternalFilter& operator=(const Net_IsExternalFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsExternalFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsExternal();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsExternalFilter>");};

};

class Net_IsClockFilter : public Filter<Net*> {
// ******************************************

    public: Net_IsClockFilter() {};

    public: Net_IsClockFilter(const Net_IsClockFilter& filter) {};

    public: Net_IsClockFilter& operator=(const Net_IsClockFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsClockFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsClock();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsClockFilter>");};

};

class Net_IsSupplyFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsSupplyFilter() {};

    public: Net_IsSupplyFilter(const Net_IsSupplyFilter& filter) {};

    public: Net_IsSupplyFilter& operator=(const Net_IsSupplyFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsSupplyFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsSupply();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsSupplyFilter>");};

};

class Net_IsPowerFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsPowerFilter() {};

    public: Net_IsPowerFilter(const Net_IsPowerFilter& filter) {};

    public: Net_IsPowerFilter& operator=(const Net_IsPowerFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsPowerFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsPower();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsPowerFilter>");};

};

class Net_IsGroundFilter : public Filter<Net*> {
// *******************************************

    public: Net_IsGroundFilter() {};

    public: Net_IsGroundFilter(const Net_IsGroundFilter& filter) {};

    public: Net_IsGroundFilter& operator=(const Net_IsGroundFilter& filter) {return *this;};

    public: virtual Filter<Net*>* GetClone() const {return new Net_IsGroundFilter(*this);};

    public: virtual bool Accept(Net* net) const {return net->IsGround();};

    public: virtual string _GetString() const {return "<" + _TName("Net::IsGroundFilter>");};

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

        public: virtual Plug* GetElement() const;
        public: virtual Hurricane::Locator<Plug*>* GetClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _GetString() const;

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

    public: virtual Collection<Plug*>* GetClone() const;
    public: virtual Hurricane::Locator<Plug*>* GetLocator() const;

// Others
// ******

    public: virtual string _GetString() const;

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

    if (_cell->GetNet(_name))
        throw Error("Can't create " + _TName("Net") + " : already exists");
}

Net* Net::Create(Cell* cell, const Name& name)
// *******************************************
{
    Net* net = new Net(cell, name);

    net->_PostCreate();

    return net;
}

Box Net::GetBoundingBox() const
// ****************************
{
    Box boundingBox;
    for_each_component(component, GetComponents()) {
        boundingBox.merge(component->GetBoundingBox());
        end_for;
    }
    return boundingBox;
}

RoutingPads Net::GetRoutingPads() const
// ************************
{
    // return GetComponents().GetSubSet<RoutingPad*>();
    return SubTypeCollection<Component*, RoutingPad*>(GetComponents());
}

Plugs Net::GetPlugs() const
// ************************
{
    // return GetComponents().GetSubSet<Plug*>();
    return SubTypeCollection<Component*, Plug*>(GetComponents());
}

Pins Net::GetPins() const
// **********************
{
    // return GetComponents().GetSubSet<Pin*>();
    return SubTypeCollection<Component*, Pin*>(GetComponents());
}

Contacts Net::GetContacts() const
// ******************************
{
    // return GetComponents().GetSubSet<Contact*>();
    return SubTypeCollection<Component*, Contact*>(GetComponents());
}

Segments Net::GetSegments() const
// ******************************
{
    // return GetComponents().GetSubSet<Segment*>();
    return SubTypeCollection<Component*, Segment*>(GetComponents());
}

Verticals Net::GetVerticals() const
// ********************************
{
    // return GetComponents().GetSubSet<Vertical*>();
    return SubTypeCollection<Component*, Vertical*>(GetComponents());
}

Horizontals Net::GetHorizontals() const
// ************************************
{
    // return GetComponents().GetSubSet<Horizontal*>();
    return SubTypeCollection<Component*, Horizontal*>(GetComponents());
}

Pads Net::GetPads() const
// **********************
{
    // return GetComponents().GetSubSet<Pad*>();
    return SubTypeCollection<Component*, Pad*>(GetComponents());
}

Plugs Net::GetSlavePlugs() const
// *****************************
{
    return Net_SlavePlugs(this);
}

Plugs Net::GetConnectedSlavePlugs() const
// **************************************
{
    return GetSlavePlugs().GetSubSet(Plug::GetIsConnectedFilter());
}

Plugs Net::GetUnconnectedSlavePlugs() const
// ****************************************
{
    return GetSlavePlugs().GetSubSet(Plug::GetIsUnconnectedFilter());
}

NetFilter Net::GetIsCellNetFilter()
// *******************************
{
    return Net_IsCellNetFilter();
}

NetFilter Net::GetIsDeepNetFilter()
// *******************************
{
    return Net_IsDeepNetFilter();
}

NetFilter Net::GetIsGlobalFilter()
// *******************************
{
    return Net_IsGlobalFilter();
}

NetFilter Net::GetIsExternalFilter()
// *********************************
{
    return Net_IsExternalFilter();
}

NetFilter Net::GetIsInternalFilter()
// *********************************
{
    return !Net_IsExternalFilter();
}

NetFilter Net::GetIsClockFilter()
// ******************************
{
    return Net_IsClockFilter();
}

NetFilter Net::GetIsSupplyFilter()
// *******************************
{
    return Net_IsSupplyFilter();
}

NetFilter Net::GetIsPowerFilter()
// *******************************
{
    return Net_IsPowerFilter();
}

NetFilter Net::GetIsGroundFilter()
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

        if (_cell->GetNet(name))
            throw Error("Can't change net name : already exists");

        _cell->_GetNetMap()._Remove(this);
        _name = name;
        _cell->_GetNetMap()._Insert(this);
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
            if (!GetConnectedSlavePlugs().IsEmpty())
                throw Error("Can't set internal : has connected slave plugs");
            _direction = Direction::UNDEFINED;
        }
        _isExternal = isExternal;
        if (_isExternal) {
            OpenUpdateSession();
            SetPosition(Point(0, 0));
            for_each_instance(instance, _cell->GetSlaveInstances()) {
                Plug::_Create(instance, this);
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
        for_each_plug(plug, GetSlavePlugs()) {
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

void Net::Materialize()
// ********************
{
    for_each_component(component, GetComponents()) {
        component->Materialize();
        end_for;
    }
    for_each_rubber(rubber, GetRubbers()) {
        rubber->Materialize();
        end_for;
    }
}

void Net::Unmaterialize()
// **********************
{
    for_each_rubber(rubber, GetRubbers()) {
        rubber->Unmaterialize();
        end_for;
    }
    for_each_component(component, GetComponents()) {
        component->Unmaterialize();
        end_for;
    }
}

static void MergeNets(Net* net1, Net* net2)
// ****************************************
{
    assert(net1);
    assert(net2);

    if (net2->GetName()[0] != '~') {
        if ((net1->GetName()[0] == '~') ||
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

    if (net->GetCell() != _cell)
        throw Error("Can't merge net : incompatible net");

    if (!IsExternal() && net->IsExternal() && !net->GetConnectedSlavePlugs().IsEmpty())
        throw Error("Can't merge net : incompatible net");

    for_each_rubber(rubber, net->GetRubbers()) rubber->_SetNet(this); end_for;
    for_each_component(component, net->GetComponents()) component->_SetNet(this); end_for;

    if (IsExternal() && net->IsExternal()) {
        for_each_plug(plug, net->GetConnectedSlavePlugs()) {
            Plug* mainPlug = plug->GetInstance()->GetPlug(this);
            if (mainPlug->IsConnected() && (mainPlug->GetNet() != plug->GetNet()))
                MergeNets(mainPlug->GetNet(), plug->GetNet());
            end_for;
        }
        for_each_plug(plug, net->GetConnectedSlavePlugs()) {
            Plug* mainPlug = plug->GetInstance()->GetPlug(this);
            if (!mainPlug->IsConnected()) mainPlug->SetNet(plug->GetNet());
            Hook* masterHook = plug->GetBodyHook();
            Hook* nextMasterHook = masterHook->GetNextMasterHook();
            if (nextMasterHook != masterHook) {
                masterHook->Detach();
                mainPlug->GetBodyHook()->Merge(nextMasterHook);
            }
            Hooks slaveHooks = masterHook->GetSlaveHooks();
            while (!slaveHooks.IsEmpty()) {
                Hook* slaveHook = slaveHooks.GetFirst();
                slaveHook->Detach();
                slaveHook->Attach(mainPlug->GetBodyHook());
            }
            plug->_Delete();
            end_for;
        }
    }

    net->Delete();
}

void Net::_PostCreate()
// ********************
{
    _cell->_GetNetMap()._Insert(this);

    if (_isExternal) {
        for_each_instance(instance, _cell->GetSlaveInstances()) {
            Plug::_Create(instance, this);
            end_for;
        }
    }

    Inherit::_PostCreate();
}

void Net::_PreDelete()
// *******************
{
    Inherit::_PreDelete();

    for_each_plug(slavePlug, GetSlavePlugs()) slavePlug->_Delete(); end_for;

    Unmaterialize();

    for_each_rubber(rubber, GetRubbers()) rubber->_Delete(); end_for;

    for_each_component(component, GetComponents()) {
        for_each_hook(hook, component->GetHooks()) {
            // 15 05 2006 xtof : detach all hooks in rings when
            // a net deletion occurs, can't see why master hooks were not detached.
            //if (!hook->IsMaster()) hook->Detach();
            hook->Detach();
            end_for;
        }
        end_for;
    }

    for_each_component(component, GetComponents()) {
        if (!is_a<Plug*>(component))
            component->Delete();
        else
            ((Plug*)component)->SetNet(NULL);
        end_for;
    }

    _cell->_GetNetMap()._Remove(this);
}

string Net::_GetString() const
// ***************************
{
    string s = Inherit::_GetString();
    s.insert(s.length() - 1, " " + GetString(_name));
    return s;
}

Record* Net::_GetRecord() const
// **********************
{
    Record* record = Inherit::_GetRecord();
    if (record) {
        record->Add(GetSlot("Cell", _cell));
        record->Add(GetSlot("Name", &_name));
        record->Add(GetSlot("Arity", &_arity));
        record->Add(GetSlot("Global", &_isGlobal));
        record->Add(GetSlot("External", &_isExternal));
        record->Add(GetSlot("Type", _type));
        record->Add(GetSlot("Direction", _direction));
        record->Add(GetSlot("Position", &_position));
        record->Add(GetSlot("Components", &_componentSet));
        record->Add(GetSlot("Rubbers", &_rubberSet));
        record->Add(GetSlot("External", &_isExternal));
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

string Net::Type::_GetString() const
// *********************************
{
  return GetString(&_code);
}

Record* Net::Type::_GetRecord() const
// ****************************
{
    Record* record = new Record(GetString(this));
    record->Add(GetSlot("Code", &_code));
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

string Net::Direction::_GetString() const
// **************************************
{
  return GetString(&_code);
}

Record* Net::Direction::_GetRecord() const
// *********************************
{
    Record* record = new Record(GetString(this));
    record->Add(GetSlot("Code", &_code));
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

unsigned Net::ComponentSet::_GetHashValue(Component* component) const
// ******************************************************************
{
    return ( (unsigned int)( (unsigned long)component ) ) / 8;
}

Component* Net::ComponentSet::_GetNextElement(Component* component) const
// **********************************************************************
{
    return component->_GetNextOfNetComponentSet();
}

void Net::ComponentSet::_SetNextElement(Component* component, Component* nextComponent) const
// ******************************************************************************************
{
    component->_SetNextOfNetComponentSet(nextComponent);
}



// ****************************************************************************************************
// Net::RubberSet implementation
// ****************************************************************************************************

Net::RubberSet::RubberSet()
// ************************
:    Inherit()
{
}

unsigned Net::RubberSet::_GetHashValue(Rubber* rubber) const
// *********************************************************
{
    return ( (unsigned int)( (unsigned long)rubber ) ) / 8;
}

Rubber* Net::RubberSet::_GetNextElement(Rubber* rubber) const
// **********************************************************
{
    return rubber->_GetNextOfNetRubberSet();
}

void Net::RubberSet::_SetNextElement(Rubber* rubber, Rubber* nextRubber) const
// ***************************************************************************
{
    rubber->_SetNextOfNetRubberSet(nextRubber);
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

Collection<Plug*>* Net_SlavePlugs::GetClone() const
// ************************************************
{
    return new Net_SlavePlugs(*this);
}

Locator<Plug*>* Net_SlavePlugs::GetLocator() const
// ***********************************************
{
    return new Locator(_net);
}

string Net_SlavePlugs::_GetString() const
// **************************************
{
    string s = "<" + _TName("Net::SlavePlugs");
    if (_net) s += " " + GetString(_net);
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
        _instanceLocator = _net->GetCell()->GetSlaveInstances().GetLocator();
        while (!_plug && _instanceLocator.IsValid()) {
            _plug = _instanceLocator.GetElement()->GetPlug(_net);
            _instanceLocator.Progress();
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

Plug* Net_SlavePlugs::Locator::GetElement() const
// **********************************************
{
    return _plug;
}

Locator<Plug*>* Net_SlavePlugs::Locator::GetClone() const
// ******************************************************
{
    return new Locator(*this);
}

bool Net_SlavePlugs::Locator::IsValid() const
// ******************************************
{
    return (_plug != NULL);
}

void Net_SlavePlugs::Locator::Progress()
// *************************************
{
    if (IsValid()) {
        _plug = NULL;
        while (!_plug && _instanceLocator.IsValid()) {
            _plug = _instanceLocator.GetElement()->GetPlug(_net);
            _instanceLocator.Progress();
        }
    }
}

string Net_SlavePlugs::Locator::_GetString() const
// ***********************************************
{
    string s = "<" + _TName("Net::SlavePlugs::Locator");
    if (_net) s += " " + GetString(_net);
    s += ">";
    return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
