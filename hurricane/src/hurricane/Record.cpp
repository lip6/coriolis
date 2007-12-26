// ****************************************************************************************************
// File: Record.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Commons.h"

namespace Hurricane {



// ****************************************************************************************************
// Record implementation
// ****************************************************************************************************

Record::Record(const string& name)
// *******************************
:	_name(name),
	_slotList()
{
}

Record::~Record()
// **************
{
	while (!_slotList.empty()) {
		Slot* slot = *_slotList.begin();
		_slotList.remove(slot);
		delete slot;
	}
}

Slot* Record::GetSlot(unsigned no) const
// *************************************
{
	SlotList::const_iterator iterator = _slotList.begin();
	while (no-- && (iterator != _slotList.end())) ++iterator;
	return (iterator == _slotList.end()) ? NULL : *iterator;
}

void Record::Add(Slot* slot)
// *************************
{
  if (!slot) {
    cerr << "[ERROR] Record::Add(): Attempt to add NULL Slot." << endl;
    return;
  }
  _slotList.push_back(slot);
}

string Record::_GetString() const
// ******************************
{
	return "<" + _TName("Record") + " " + GetName() + ">";
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
