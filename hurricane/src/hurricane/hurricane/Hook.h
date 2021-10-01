// ****************************************************************************************************
// File: ./hurricane/Hook.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#pragma  once
#include "hurricane/Error.h"
#include "hurricane/Hooks.h"

namespace Hurricane {

class Component;



// ****************************************************************************************************
// Hook declaration
// ****************************************************************************************************
  
class Hook {
// *********

    typedef  Hook* (*compToHook_t)(Component*);

// Attributes
// **********

    private: static map<string,compToHook_t> _compToHookMap;
    private: Hook* _nextHook;

// Constructors
// ************

    protected: Hook();

    private: Hook(const Hook& hook); // not implemented to forbid copy construction

// Destructor
// **********

  protected: virtual ~Hook() noexcept(false);

// Operators
// *********

    private: Hook& operator=(const Hook& hook); // not implemented to forbid assignment

// Accessors
// *********

    public: virtual Component* getComponent() const = 0;

    public: Hook* getNextHook() const;
    public: Hook* getPreviousHook() const;

    public: Hook* getMasterHook() const;
    public: Hook* getNextMasterHook() const;
    public: Hook* getPreviousMasterHook() const;

    public: Hooks getHooks() const;
    public: Hooks getSlaveHooks() const;

// Filters
// *******

    public: static HookFilter getIsMasterFilter();

// Predicates
// **********

    public: virtual bool isMaster() const = 0;

    public: bool isAttached() const;

// Updators
// ********

    public: Hook* detach();
    public: Hook* attach(Hook* hook);
    public: Hook* merge(Hook* hook);
    public: void _setNextHook(Hook* hook);

// Others
// ******

    public: static void addCompToHook(const string&, compToHook_t );
    public: static Hook* compToHook(const string& tname, Component* );
    public: string toJson() const;
    public: virtual string _getTypeName() const = 0;
    public: virtual string _getString() const = 0;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Hook);


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
