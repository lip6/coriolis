// ****************************************************************************************************
// File: Selector.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Selector.h"
#include "Quark.h"
#include "View.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Selector implementation
// ****************************************************************************************************

Selector::Selector()
// *****************
:	Inherit(),
	_viewSet()
{
}

const Name& Selector::GetPropertyName()
// ************************************
{
	static Name NAME = _PName("Selector");
	return NAME;
}

Name Selector::GetName() const
// ***************************
{
	return Selector::GetPropertyName();
}

Occurrence Selector::GetOccurrence() const
// *************************************
{
	DBo* owner = GetOwner();
	if (!owner) return Occurrence();
	assert(is_a<Quark*>(owner));
	return ((Quark*)owner)->GetOccurrence();
}

Selector* Selector::_Create(Occurrence& occurrence)
// **********************************************
{
	if (!occurrence.IsValid())
		throw Error("Can't create " + _TName("Selector") + " : invalid occurrence");

	if (occurrence.GetProperty(Selector::GetPropertyName()))
		throw Error("Can't create " + _TName("Selector") + " : already exists");

	Selector* selector = new Selector();

	selector->_PostCreate();

	occurrence.Put(selector);

	return selector;
}

void Selector::_PreDelete()
// ************************
{
	Inherit::_PreDelete();

	for_each_view(view, GetViews()) _DetachFrom(view, true); end_for;
}

string Selector::_GetString() const
// ********************************
{
	return "<" + _TName("Selector") + " " + GetString(GetOccurrence()) + ">";
}

Record* Selector::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Views", &_viewSet));
	}
	return record;
}

void Selector::_AttachTo(View* view)
// *********************************
{
	if (!view)
		throw Error("Can't attach selector : null view)");

	_viewSet.insert(view);
	view->_GetSelectorSet().insert(this);
}

void Selector::_DetachFrom(View* view, bool inDeletion)
// ****************************************************
{
	if (!view)
		throw Error("Can't detach selector : null view");

	view->_GetSelectorSet().erase(this);
	_viewSet.erase(view);

	if (!inDeletion && _viewSet.empty()) Delete();
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
