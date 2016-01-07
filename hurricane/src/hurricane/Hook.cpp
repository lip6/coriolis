// ****************************************************************************************************
// File: ./Hook.cpp
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

#include "hurricane/Hook.h"
#include "hurricane/Component.h"
#include "hurricane/Rubber.h"
#include "hurricane/Error.h"

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

    public: virtual bool accept(Hook* hook) const {return hook->isMaster();};

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

        public: virtual bool isValid() const;

        public: virtual void progress();

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

        public: virtual bool isValid() const;

        public: virtual void progress();

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

map<string,Hook::compToHook_t>  Hook::_compToHookMap;


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
        if (hook->isMaster()) return hook;
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
        if (hook->isMaster()) previousMasterHook = hook;
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

bool Hook::isAttached() const
// **************************
{
    if (!isMaster())
        return (getMasterHook() != NULL);
    else
        return (getNextMasterHook() != this);
}

Hook* Hook::detach()
// *****************
{
    Hook* previousHook = NULL;
    Hook* hook = _nextHook;
    while (hook != this) {
        if (!isMaster() || hook->isMaster()) previousHook = hook;
        hook = hook->_nextHook;
    }
    if (previousHook) {
        Hook* nextHook = _nextHook;
        _nextHook = previousHook->_nextHook;
        previousHook->_nextHook = nextHook;

        // /*
        if (isMaster()) {
            assert(previousHook->isMaster());
            Component* component = getComponent();
            Rubber* rubber = component->getRubber();
            if (rubber) {
                rubber->_setHook(previousHook);
                component->_setRubber(NULL);
            }
        }
        // */

    }
    return previousHook;
}

Hook* Hook::attach(Hook* hook)
// ***************************
{
    if (isAttached())
        throw Error("Can't attach : already attached");

    if (!hook)
        throw Error("Can't attach : null hook");

    if (!hook->isMaster())
        throw Error("Can't attach : not a master hook");

    if (hook == this)
        throw Error("Can't attach : itself");

    Hook* masterHook = hook->getPreviousMasterHook();
    Hook* nextHook = masterHook->_nextHook;
    masterHook->_nextHook = _nextHook;
    _nextHook = nextHook;

    // /*
    if (isMaster()) {
        Rubber* rubber = hook->getComponent()->getRubber();
        if (rubber)
            getComponent()->_setRubber(rubber);
        else
            Rubber::_create(this);
    }
    // */

    return this;
}

void Hook::_setNextHook(Hook* hook)
{
    if (isMaster())
    {
        Rubber* rubber = hook->getComponent()->getRubber();
        if (rubber)
            rubber->_destroy();
    }
    _nextHook = hook;
}

Hook* Hook::merge(Hook* hook)
// **************************
{
    if (!isMaster())
        throw Error("Can't merge : not a master");

    if (!hook)
        throw Error("Can't merge : null hook");

    if (!hook->isMaster())
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
        record->add(getSlot("Component", getComponent()));
        record->add(getSlot("NextHook", _nextHook));
    }
    return record;
}

string Hook::toJson() const
// ************************
{
  if (_nextHook == this) return "";
  string s = _getTypeName()+"."+getString(getComponent()->getId());
  return s;
}

void Hook::addCompToHook(const string& tname, Hook::compToHook_t converter)
// ************************************************************************
{
  _compToHookMap.insert( make_pair(tname,converter) );
}

Hook* Hook::compToHook(const string& tname, Component* component)
// **************************************************************
{
  map<string,compToHook_t>::const_iterator iconv = _compToHookMap.find(tname);
  if (iconv == _compToHookMap.end()) {
    throw Error( "Hook::fromJson(): No converter registered for type name \"%s\""
               , tname.c_str() );
  }
  return (*iconv).second(component);
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

bool Hook_Hooks::Locator::isValid() const
// **************************************
{
    return (_currentHook != NULL);
}

void Hook_Hooks::Locator::progress()
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
    if (_hook && _hook->isMaster()) {
        _currentHook = _hook->getPreviousMasterHook();
        if (_currentHook) {
            _currentHook = _currentHook->getNextHook();
            if (_currentHook == _hook) _currentHook = NULL;
        }
        assert(!_currentHook || !_currentHook->isMaster());
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

bool Hook_SlaveHooks::Locator::isValid() const
// *******************************************
{
    return (_currentHook != NULL);
}

void Hook_SlaveHooks::Locator::progress()
// **************************************
{
    if (_currentHook) {
        _currentHook = _currentHook->getNextHook();
        if (_currentHook == _hook) _currentHook = NULL;
        assert(!_currentHook || !_currentHook->isMaster());
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
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
