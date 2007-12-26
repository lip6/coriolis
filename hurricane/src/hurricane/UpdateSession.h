// ****************************************************************************************************
// File: UpdateSession.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_UPDATE_SESSION
#define HURRICANE_UPDATE_SESSION

#include "Property.h"

namespace Hurricane {

class Go;



// ****************************************************************************************************
// UpdateSession declaration
// ****************************************************************************************************

class UpdateSession : public SharedProperty {
// ****************************************

// Types
// *****

	public: typedef SharedProperty Inherit;

// Constructors
// ************

	protected: UpdateSession();

	public: virtual void Delete();

// Accessors
// *********

	public: static const Name& GetPropertyName();
	public: virtual Name GetName() const {return GetPropertyName();};

// Managers
// ********

	public: virtual void OnCapturedBy(DBo* owner);
	public: virtual void OnNotOwned();

// Ohers
// *****

	public: static UpdateSession* _Create();
	protected: virtual void _PostCreate();

	public: void _Delete();
	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("UpdateSession");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

void OpenUpdateSession();

void CloseUpdateSession();



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::UpdateSession)

#endif // HURRICANE_UPDATE_SESSION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
