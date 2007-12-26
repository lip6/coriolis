// ****************************************************************************************************
// File: Hook.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_HOOK
#define HURRICANE_HOOK

#include "Hooks.h"

namespace Hurricane {

class Component;



// ****************************************************************************************************
// Hook declaration
// ****************************************************************************************************

class Hook : public NestedSlotAdapter {
// **********************************

// Attributes
// **********

	private: Hook* _nextHook;

// Constructors
// ************

	protected: Hook();

	private: Hook(const Hook& hook); // not implemented to forbid copy construction

// Destructor
// **********

	protected: virtual ~Hook();

// Operators
// *********

	private: Hook& operator=(const Hook& hook); // not implemented to forbid assignment

// Accessors
// *********

	public: virtual Component* GetComponent() const = 0;

	public: Hook* GetNextHook() const;
	public: Hook* GetPreviousHook() const;

	public: Hook* GetMasterHook() const;
	public: Hook* GetNextMasterHook() const;
	public: Hook* GetPreviousMasterHook() const;

	public: Hooks GetHooks() const;
	public: Hooks GetSlaveHooks() const;

// Filters
// *******

	public: static HookFilter GetIsMasterFilter();

// Predicates
// **********

	public: virtual bool IsMaster() const = 0;

	public: bool IsAttached() const;

// Updators
// ********

	public: Hook* Detach();
	public: Hook* Attach(Hook* hook);
	public: Hook* Merge(Hook* hook);
	public: void _SetNextHook(Hook* hook);

// Others
// ******

	public: virtual string _GetString() const = 0;
	public: virtual Record* _GetRecord() const;

};



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Hook)

#endif // HURRICANE_HOOK

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
