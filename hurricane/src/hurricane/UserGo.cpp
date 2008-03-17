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

        public: virtual UserGo* getElement() const;
        public: virtual Hurricane::Locator<UserGo*>* getClone() const;

        public: virtual bool IsValid() const;

        public: virtual void Progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<UserGo*>* getClone() const;
    public: virtual Hurricane::Locator<UserGo*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

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
        QuadTree& quadTree = _displaySlot->_getQuadTree();
        quadTree.Insert(this);
        getCell()->_Fit(quadTree.getBoundingBox());
    }
}

void UserGo::Unmaterialize()
// *************************
{
    if (IsMaterialized()) {
        QuadTree& quadTree = _displaySlot->_getQuadTree();
        getCell()->_Unfit(getBoundingBox());
        quadTree.Remove(this);
    }
}

string UserGo::_getString() const
// ******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_displaySlot->getName()));
    return s;
}

Record* UserGo::_getRecord() const
// *************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("DisplaySlot", _displaySlot));
    }
    return record;
}

UserGos getUserGos(const Cell* cell)
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
:     Inherit(),
    _cell(NULL)
{
}

UserGo_CellUserGos::UserGo_CellUserGos(const Cell* cell)
// *****************************************************
:     Inherit(),
    _cell(cell)
{
}

UserGo_CellUserGos::UserGo_CellUserGos(const UserGo_CellUserGos& usergos)
// **********************************************************************
:     Inherit(),
    _cell(usergos._cell)
{
}

UserGo_CellUserGos& UserGo_CellUserGos::operator=(const UserGo_CellUserGos& usergos)
// *********************************************************************************
{
    _cell = usergos._cell;
    return *this;
}

Collection<UserGo*>* UserGo_CellUserGos::getClone() const
// ******************************************************
{
    return new UserGo_CellUserGos(*this);
}

Locator<UserGo*>* UserGo_CellUserGos::getLocator() const
// *****************************************************
{
    return new Locator(_cell);
}

string UserGo_CellUserGos::_getString() const
// ******************************************
{
    string s = "<" + _TName("Cell::UserGos");
    if (_cell) {
        s += " " + getString(_cell);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// UserGo_CellUserGos::Locator implementation
// ****************************************************************************************************

UserGo_CellUserGos::Locator::Locator()
// ****************************
:    Inherit(),
    _cell(NULL),
        _displaySlotLocator(),
    _userGoLocator()
{
}

UserGo_CellUserGos::Locator::Locator(const Cell* cell)
// ***************************************************
:    Inherit(),
    _cell(cell),
    _displaySlotLocator(),
    _userGoLocator()
{
    if (_cell) {
        _displaySlotLocator = getDisplaySlots(cell).getLocator();
        if (_displaySlotLocator.IsValid())
        {
            DisplaySlot* displaySlot = _displaySlotLocator.getElement();
            _userGoLocator = displaySlot->getUserGos().getLocator();
        }
    }
}

UserGo_CellUserGos::Locator::Locator(const Locator& locator)
// **************************************************
:    Inherit(),
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

UserGo* UserGo_CellUserGos::Locator::getElement() const
// ********************************************
{
    return _userGoLocator.getElement();
}

Locator<UserGo*>* UserGo_CellUserGos::Locator::getClone() const
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
            DisplaySlot* displaySlot = _displaySlotLocator.getElement();
            _userGoLocator = displaySlot->getUserGos().getLocator();
        }
    }
}

string UserGo_CellUserGos::Locator::_getString() const
// ********************************************
{
    string s = "<" + _TName("Cell::UserGos::Locator");
    if (_cell) {
        s += " " + getString(_cell);
    }
    s += ">";
    return s;
}

} // End of Hurricane namespace.
