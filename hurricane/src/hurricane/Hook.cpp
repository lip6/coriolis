// ****************************************************************************************************
// File: Hook.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Hook.h"
#include "Component.h"
#include "Rubber.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Hook_IsMasterFilter : public Filter<Hook*> {
// *********************************************

    public: Hook_IsMasterFilter() {};

    public: Hook_IsMasterFilter(const Hook_IsMasterFilter& filter) {};

    public: Hook_IsMasterFilter& operator=(const Hook_IsMasterFilter& filter) {return *this;};

    public: virtual Filter<Hook*>* getClone() const {return new Hook_IsMasterFilter(*this);};

    public: virtual bool Accept(Hook* hook) const {return hook->IsMaster();};

    public: virtual string _getString() const {return "<" + _TName("Hook::IsMasterFilter>");};

};



// ****************************************************************************************************
// Hook_Hooks implementation
// ****************************************************************************************************

class Hook_Hooks : public Collection<Hook*> {
// ****************************************

// Types
// *****

    public: typedef Collection<Hook*> Inherit;

    public: class Locator : public Hurricane::Locator<Hook*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Hook*> Inherit;

        private: const Hook* _hook;
        private: Hook* _currentHook;

        public: Locator(const Hook* hook = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Hook* getElement() const;
        public: virtual Hurricane::Locator<Hook*>* getClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Hook* _hook;

// Constructors
// ************

    public: Hook_Hooks(const Hook* hook = NULL);
    public: Hook_Hooks(const Hook_Hooks& hooks);

// Operators
// *********

    public: Hook_Hooks& operator=(const Hook_Hooks& hooks);

// Accessors
// *********

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Hook_SlaveHooks implementation
// ****************************************************************************************************

class Hook_SlaveHooks : public Collection<Hook*> {
// *********************************************

// Types
// *****

    public: typedef Collection<Hook*> Inherit;

    public: class Locator : public Hurricane::Locator<Hook*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Hook*> Inherit;

        private: const Hook* _hook;
        private: Hook* _currentHook;

        public: Locator(const Hook* hook = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Hook* getElement() const;
        public: virtual Hurricane::Locator<Hook*>* getClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Hook* _hook;

// Constructors
// ************

    public: Hook_SlaveHooks(const Hook* hook = NULL);
    public: Hook_SlaveHooks(const Hook_SlaveHooks& hooks);

// Operators
// *********

    public: Hook_SlaveHooks& operator=(const Hook_SlaveHooks& hooks);

// Accessors
// *********

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Hook implementation
// ****************************************************************************************************

Hook::Hook()
// *********
:    _nextHook(this)
{
}

Hook::~Hook()
// **********
{
    if (_nextHook != this)
        throw Error("Abnormal deletion of hook : always attached");
}

Hook* Hook::getNextHook() const
// ****************************
{
    return _nextHook;
}

Hook* Hook::getPreviousHook() const
// ********************************
{
    Hook* hook = (Hook*)this;
    while (hook->_nextHook != this) hook = hook->_nextHook;
    return hook;
}

Hook* Hook::getMasterHook() const
// ******************************
{
    Hook* hook = (Hook*)this;
    do {
        if (hook->IsMaster()) return hook;
        hook = hook->_nextHook;
    } while (hook != this);
    return NULL;
}

Hook* Hook::getNextMasterHook() const
// **********************************
{
    return _nextHook->getMasterHook();
}

Hook* Hook::getPreviousMasterHook() const
// **************************************
{
    Hook* previousMasterHook = NULL;
    Hook* hook = (Hook*)this;
    do {
        if (hook->IsMaster()) previousMasterHook = hook;
        hook = hook->_nextHook;
    } while (hook != this);
    return previousMasterHook;
}

Hooks Hook::getHooks() const
// *************************
{
    return Hook_Hooks(this);
}

Hooks Hook::getSlaveHooks() const
// ******************************
{
    return Hook_SlaveHooks(this);
}

HookFilter Hook::getIsMasterFilter()
// *********************************
{
    return Hook_IsMasterFilter();
}

bool Hook::IsAttached() const
// **************************
{
    if (!IsMaster())
        return (getMasterHook() != NULL);
    else
        return (getNextMasterHook() != this);
}

Hook* Hook::Detach()
// *****************
{
    Hook* previousHook = NULL;
    Hook* hook = _nextHook;
    while (hook != this) {
        if (!IsMaster() || hook->IsMaster()) previousHook = hook;
        hook = hook->_nextHook;
    }
    if (previousHook) {
        Hook* nextHook = _nextHook;
        _nextHook = previousHook->_nextHook;
        previousHook->_nextHook = nextHook;

        // /*
        if (IsMaster()) {
            assert(previousHook->IsMaster());
            Component* component = getComponent();
            Rubber* rubber = component->getRubber();
            if (rubber) {
                rubber->_SetHook(previousHook);
                component->_SetRubber(NULL);
            }
        }
        // */

    }
    return previousHook;
}

Hook* Hook::Attach(Hook* hook)
// ***************************
{
    if (IsAttached())
        throw Error("Can't attach : already attached");

    if (!hook)
        throw Error("Can't attach : null hook");

    if (!hook->IsMaster())
        throw Error("Can't attach : not a master hook");

    if (hook == this)
        throw Error("Can't attach : itself");

    Hook* masterHook = hook->getPreviousMasterHook();
    Hook* nextHook = masterHook->_nextHook;
    masterHook->_nextHook = _nextHook;
    _nextHook = nextHook;

    // /*
    if (IsMaster()) {
        Rubber* rubber = hook->getComponent()->getRubber();
        if (rubber)
            getComponent()->_SetRubber(rubber);
        else
            Rubber::_create(this);
    }
    // */

    return this;
}

void Hook::_SetNextHook(Hook* hook)
{
    if (IsMaster())
    {
        Rubber* rubber = hook->getComponent()->getRubber();
        if (rubber)
            rubber->_destroy();
    }
    _nextHook = hook;
}

Hook* Hook::Merge(Hook* hook)
// **************************
{
    if (!IsMaster())
        throw Error("Can't merge : not a master");

    if (!hook)
        throw Error("Can't merge : null hook");

    if (!hook->IsMaster())
        throw Error("Can't merge : not a master hook");

    if (hook == this)
        throw Error("Can't merge : itself");

    Hook* masterHook = hook->getPreviousMasterHook();
    Hook* nextHook = masterHook->_nextHook;
    masterHook->_nextHook = _nextHook;
    _nextHook = nextHook;

    Rubber* rubber = getComponent()->getRubber();
    if (rubber) rubber->_destroy();
    rubber = hook->getComponent()->getRubber();
    if (rubber) rubber->_destroy();
    Rubber::_create(this);

    return this;
}

Record* Hook::_getRecord() const
// ***********************
{
    Record* record = NULL;
    if (_nextHook != this) {
        record = new Record(getString(this));
        record->Add(getSlot("Component", getComponent()));
        record->Add(getSlot("NextHook", _nextHook));
    }
    return record;
}



// ****************************************************************************************************
// Hook_Hooks implementation
// ****************************************************************************************************

Hook_Hooks::Hook_Hooks(const Hook* hook)
// *************************************
:     Inherit(),
    _hook(hook)
{
}

Hook_Hooks::Hook_Hooks(const Hook_Hooks& hooks)
// ********************************************
:     Inherit(),
    _hook(hooks._hook)
{
}

Hook_Hooks& Hook_Hooks::operator=(const Hook_Hooks& hooks)
// *******************************************************
{
    _hook = hooks._hook;
    return *this;
}

Collection<Hook*>* Hook_Hooks::getClone() const
// ********************************************
{
    return new Hook_Hooks(*this);
}

Locator<Hook*>* Hook_Hooks::getLocator() const
// *******************************************
{
    return new Locator(_hook);
}

string Hook_Hooks::_getString() const
// **********************************
{
    string s = "<" + _TName("Hook::Hooks");
    if (_hook) s += " " + getString(_hook);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Hook_Hooks::Locator implementation
// ****************************************************************************************************

Hook_Hooks::Locator::Locator(const Hook* hook)
// *******************************************
:    Inherit(),
    _hook(hook),
    _currentHook((Hook*)hook)
{
}

Hook_Hooks::Locator::Locator(const Locator& locator)
// *************************************************
:    Inherit(),
    _hook(locator._hook),
    _currentHook(locator._currentHook)
{
}

Hook_Hooks::Locator& Hook_Hooks::Locator::operator=(const Locator& locator)
// ************************************************************************
{
    _hook = locator._hook;
    _currentHook = locator._currentHook;
    return *this;
}

Hook* Hook_Hooks::Locator::getElement() const
// ******************************************
{
    return _currentHook;
}

Locator<Hook*>* Hook_Hooks::Locator::getClone() const
// **************************************************
{
    return new Locator(*this);
}

bool Hook_Hooks::Locator::IsValid() const
// **************************************
{
    return (_currentHook != NULL);
}

void Hook_Hooks::Locator::Progress()
// *********************************
{
    if (_currentHook) {
        _currentHook = _currentHook->getNextHook();
        if (_currentHook == _hook) _currentHook = NULL;
    }
}

string Hook_Hooks::Locator::_getString() const
// *******************************************
{
    string s = "<" + _TName("Hook::Hooks::Locator");
    if (_hook) s += " " + getString(_hook);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Hook_SlaveHooks implementation
// ****************************************************************************************************

Hook_SlaveHooks::Hook_SlaveHooks(const Hook* hook)
// ***********************************************
:     Inherit(),
    _hook(hook)
{
}

Hook_SlaveHooks::Hook_SlaveHooks(const Hook_SlaveHooks& slaveHooks)
// ****************************************************************
:     Inherit(),
    _hook(slaveHooks._hook)
{
}

Hook_SlaveHooks& Hook_SlaveHooks::operator=(const Hook_SlaveHooks& slaveHooks)
// ***************************************************************************
{
    _hook = slaveHooks._hook;
    return *this;
}

Collection<Hook*>* Hook_SlaveHooks::getClone() const
// *************************************************
{
    return new Hook_SlaveHooks(*this);
}

Locator<Hook*>* Hook_SlaveHooks::getLocator() const
// ************************************************
{
    return new Locator(_hook);
}

string Hook_SlaveHooks::_getString() const
// ***************************************
{
    string s = "<" + _TName("Hook::SlaveHooks");
    if (_hook) s += " " + getString(_hook);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Hook_SlaveHooks::Locator implementation
// ****************************************************************************************************

Hook_SlaveHooks::Locator::Locator(const Hook* hook)
// ************************************************
:    Inherit(),
    _hook(hook),
    _currentHook(NULL)
{
    if (_hook && _hook->IsMaster()) {
        _currentHook = _hook->getPreviousMasterHook();
        if (_currentHook) {
            _currentHook = _currentHook->getNextHook();
            if (_currentHook == _hook) _currentHook = NULL;
        }
        assert(!_currentHook || !_currentHook->IsMaster());
    }
}

Hook_SlaveHooks::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _hook(locator._hook),
    _currentHook(locator._currentHook)
{
}

Hook_SlaveHooks::Locator& Hook_SlaveHooks::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _hook = locator._hook;
    _currentHook = locator._currentHook;
    return *this;
}

Hook* Hook_SlaveHooks::Locator::getElement() const
// ***********************************************
{
    return _currentHook;
}

Locator<Hook*>* Hook_SlaveHooks::Locator::getClone() const
// *******************************************************
{
    return new Locator(*this);
}

bool Hook_SlaveHooks::Locator::IsValid() const
// *******************************************
{
    return (_currentHook != NULL);
}

void Hook_SlaveHooks::Locator::Progress()
// **************************************
{
    if (_currentHook) {
        _currentHook = _currentHook->getNextHook();
        if (_currentHook == _hook) _currentHook = NULL;
        assert(!_currentHook || !_currentHook->IsMaster());
    }
}

string Hook_SlaveHooks::Locator::_getString() const
// ************************************************
{
    string s = "<" + _TName("Hook::SlaveHooks::Locator");
    if (_hook) s += " " + getString(_hook);
    s += ">";
    return s;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
