// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: UserGo.cpp
// Authors: C. Alexandre
// ****************************************************************************************************

#include "Cell.h"
#include "UserGo.h"

namespace Hurricane {


// ****************************************************************************************************
// UserGo_CellUserGos implementation
// ****************************************************************************************************

class UserGo_CellUserGos : public Collection<UserGo*> {
// **************************************************

// Types
// *****

	public: typedef Collection<UserGo*> Inherit;

	public: class Locator : public Hurricane::Locator<UserGo*> {
	// *******************************************************

		public: typedef Hurricane::Locator<UserGo*> Inherit;

		private: const Cell* _cell;
        private: DisplaySlotLocator _displaySlotLocator;
		private: UserGoLocator _userGoLocator;

		public: Locator();
		public: Locator(const Cell* cell);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual UserGo* GetElement() const;
		public: virtual Hurricane::Locator<UserGo*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Cell* _cell;

// Constructors
// ************

	public: UserGo_CellUserGos();
	public: UserGo_CellUserGos(const Cell* cell);
	public: UserGo_CellUserGos(const UserGo_CellUserGos& cellusergos);

// Operators
// *********

	public: UserGo_CellUserGos& operator=(const UserGo_CellUserGos& usergos);

// Accessors
// *********

	public: virtual Collection<UserGo*>* GetClone() const;
	public: virtual Hurricane::Locator<UserGo*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};

// ****************************************************************************************************
// UserGo implementation
// ****************************************************************************************************

UserGo::UserGo(DisplaySlot* displaySlot)
// *************************************
        :  Inherit()
	, _displaySlot(displaySlot)
{
	if (!_displaySlot)
		throw Error("Can't Create " + _TName("UserGo") + " null displaySlot");
}

void UserGo::Materialize()
// ***********************
{
	if (!IsMaterialized()) {
		QuadTree& quadTree = _displaySlot->_GetQuadTree();
		quadTree.Insert(this);
		GetCell()->_Fit(quadTree.GetBoundingBox());
	}
}

void UserGo::Unmaterialize()
// *************************
{
	if (IsMaterialized()) {
		QuadTree& quadTree = _displaySlot->_GetQuadTree();
		GetCell()->_Unfit(GetBoundingBox());
		quadTree.Remove(this);
	}
}

void UserGo::_PostCreate()
// ***********************
{
	Inherit::_PostCreate();
}

void UserGo::_PreDelete()
// **********************
{
	Inherit::_PreDelete();
}

string UserGo::_GetString() const
// ******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_displaySlot->GetName()));
	return s;
}

Record* UserGo::_GetRecord() const
// *************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("DisplaySlot", _displaySlot));
	}
	return record;
}

UserGos GetUserGos(const Cell* cell)
// *********************************
{
    if (!cell)
		throw Error("Null pointer on cell while getting usergos");
    return UserGo_CellUserGos(cell); 
}

// ****************************************************************************************************
// UserGo_CellUserGos implementation
// ****************************************************************************************************

UserGo_CellUserGos::UserGo_CellUserGos()
// *************************************
: 	Inherit(),
	_cell(NULL)
{
}

UserGo_CellUserGos::UserGo_CellUserGos(const Cell* cell)
// *****************************************************
: 	Inherit(),
	_cell(cell)
{
}

UserGo_CellUserGos::UserGo_CellUserGos(const UserGo_CellUserGos& usergos)
// **********************************************************************
: 	Inherit(),
	_cell(usergos._cell)
{
}

UserGo_CellUserGos& UserGo_CellUserGos::operator=(const UserGo_CellUserGos& usergos)
// *********************************************************************************
{
	_cell = usergos._cell;
	return *this;
}

Collection<UserGo*>* UserGo_CellUserGos::GetClone() const
// ******************************************************
{
	return new UserGo_CellUserGos(*this);
}

Locator<UserGo*>* UserGo_CellUserGos::GetLocator() const
// *****************************************************
{
	return new Locator(_cell);
}

string UserGo_CellUserGos::_GetString() const
// ******************************************
{
	string s = "<" + _TName("Cell::UserGos");
	if (_cell) {
		s += " " + GetString(_cell);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// UserGo_CellUserGos::Locator implementation
// ****************************************************************************************************

UserGo_CellUserGos::Locator::Locator()
// ****************************
:	Inherit(),
	_cell(NULL),
        _displaySlotLocator(),
	_userGoLocator()
{
}

UserGo_CellUserGos::Locator::Locator(const Cell* cell)
// ***************************************************
:	Inherit(),
	_cell(cell),
    _displaySlotLocator(),
	_userGoLocator()
{
	if (_cell) {
        _displaySlotLocator = GetDisplaySlots(cell).GetLocator();
        if (_displaySlotLocator.IsValid())
        {
            DisplaySlot* displaySlot = _displaySlotLocator.GetElement();
            _userGoLocator = displaySlot->GetUserGos().GetLocator();
        }
	}
}

UserGo_CellUserGos::Locator::Locator(const Locator& locator)
// **************************************************
:	Inherit(),
	_cell(locator._cell),
    _displaySlotLocator(locator._displaySlotLocator),
    _userGoLocator(locator._userGoLocator)
{
}

UserGo_CellUserGos::Locator& UserGo_CellUserGos::Locator::operator=(const Locator& locator)
// ****************************************************************************************
{
	_cell = locator._cell;
    _displaySlotLocator = locator._displaySlotLocator;
    _userGoLocator =locator._userGoLocator;
	return *this;
}

UserGo* UserGo_CellUserGos::Locator::GetElement() const
// ********************************************
{
	return _userGoLocator.GetElement();
}

Locator<UserGo*>* UserGo_CellUserGos::Locator::GetClone() const
// ************************************************************
{
	return new Locator(*this);
}

bool UserGo_CellUserGos::Locator::IsValid() const
// ***************************************
{
	return _userGoLocator.IsValid();
}

void UserGo_CellUserGos::Locator::Progress()
// *****************************************
{
	if (_userGoLocator.IsValid()) {
        _userGoLocator.Progress();
    }
    else if (_displaySlotLocator.IsValid()) {
        _displaySlotLocator.Progress();
        if (_displaySlotLocator.IsValid())
        {
            DisplaySlot* displaySlot = _displaySlotLocator.GetElement();
            _userGoLocator = displaySlot->GetUserGos().GetLocator();
        }
    }
}

string UserGo_CellUserGos::Locator::_GetString() const
// ********************************************
{
	string s = "<" + _TName("Cell::UserGos::Locator");
	if (_cell) {
		s += " " + GetString(_cell);
	}
	s += ">";
	return s;
}

} // End of Hurricane namespace.
