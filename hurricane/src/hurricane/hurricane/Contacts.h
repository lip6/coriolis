// ****************************************************************************************************
// File: ./hurricane/Contacts.h
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
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
