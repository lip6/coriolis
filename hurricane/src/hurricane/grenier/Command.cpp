// ****************************************************************************************************
// File: Command.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Command.h"
#include "View.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Command implementation
// ****************************************************************************************************

Command::Command(const Name& name, unsigned button)
// ************************************************
:	_name(name),
	_button(button),
	_viewSet()
{
	if (_name.IsEmpty())
		throw Error("Can't create " + _TName("Command") + " : empty name");
}

bool Command::IsInstalledOn(View* view) const
// ******************************************
{
	return (_viewSet.find(view) != _viewSet.end());
}

void Command::InstallOn(View* view)
// ********************************
{
	if (!view)
		throw Error("Can't install command : null view");

	Command* command = view->GetCommand(_button);
	if (command != this) {
		if (command) command->UninstallFrom(view);
		view->_GetCommandMap()[_button] = this;
		_OnInstalledOn(view);
	}

	_viewSet.insert(view);
}

void Command::UninstallFrom(View* view)
// ************************************
{
	if (!view)
		throw Error("Can't uninstall command : null view");

	if (view->GetCommand(_button) != this)
		throw Error("Can't uninstall command : not installed");

	_OnUninstalledFrom(view);
	view->_GetCommandMap().erase(_button);

	_viewSet.erase(view);
}

void Command::_PreDelete()
// ***********************
{
	Inherit::_PreDelete();

	for_each_view(view, GetViews()) UninstallFrom(view); end_for;
}

string Command::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	s.insert(s.length() - 1, " " + GetString(_button));
	return s;
}

Record* Command::_GetRecord() const
// **************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("Button", &_button));
		record->Add(GetSlot("Views", &_viewSet));
	}
	return record;
}

void Command::_OnInstalledOn(View* view)
// *************************************
{
}

void Command::_OnUninstalledFrom(View* view)
// *****************************************
{
}

void Command::_OnDrawGhost(View* view)
// ***********************************
{
}

void Command::_OnMouseEnter(View* view, const Point& position)
// ***********************************************************
{
}

void Command::_OnMouseMove(View* view, const Point& position, unsigned state)
// **************************************************************************
{
}

void Command::_OnMouseLeave(View* view, const Point& position)
// ***********************************************************
{
}

void Command::_OnButtonPress(View* view, const Point& position, unsigned state)
// ****************************************************************************
{
}

void Command::_OnButtonRelease(View* view, const Point& position, unsigned state)
// ******************************************************************************
{
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
