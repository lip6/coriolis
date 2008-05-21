// ****************************************************************************************************
// File: Contacts.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_CONTACTS
#define HURRICANE_CONTACTS

#include "hurricane/Collection.h"

namespace Hurricane {

class Contact;



// ****************************************************************************************************
// Contacts declaration
// ****************************************************************************************************

typedef GenericCollection<Contact*> Contacts;



// ****************************************************************************************************
// ContactLocator declaration
// ****************************************************************************************************

typedef GenericLocator<Contact*> ContactLocator;



// ****************************************************************************************************
// ContactFilter declaration
// ****************************************************************************************************

typedef GenericFilter<Contact*> ContactFilter;



// ****************************************************************************************************
// for_each_contact declaration
// ****************************************************************************************************

#define for_each_contact(contact, contacts)\
/******************************************/\
{\
	ContactLocator _locator = contacts.getLocator();\
	while (_locator.isValid()) {\
		Contact* contact = _locator.getElement();\
		_locator.progress();



} // End of Hurricane namespace.

#endif // HURRICANE_CONTACTS

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
