// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: DisplaySlot.cpp
// Author: C. Alexandre
// ****************************************************************************************************

#include "Error.h"
#include "UserGo.h"
#include "Relation.h"
#include "Cell.h"
#include "UpdateSession.h"

#include "DisplaySlot.h"

namespace Hurricane {

static Name DisplaySlotsCellRelationName("DisplaySlotsCellRelation");

static StandardRelation* getDisplaySlotRelation(const Cell* cell)
{
        Property* property = cell->getProperty(DisplaySlotsCellRelationName);
        if (!property)
                return NULL;
        else
        {
                StandardRelation* relation = dynamic_cast<StandardRelation*>(property);
                if (!relation)
                        throw Error("Bad Property type: Must be a Standard Relation");
                return relation;
        }
}

// ****************************************************************************************************
// DisplaySlot implementation
// ****************************************************************************************************

DisplaySlot::DisplaySlot(Cell* cell,const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1, unsigned linewidth1, unsigned short red2, unsigned short green2, unsigned short blue2, const string& pattern2, unsigned linewidth2)
// ***********************
        :  Inherit()
        , _cell(cell)
        , _name(name)
        , _quadTree()
    //, _drawGC(NULL)
    //, _fillGC(NULL)
        , _isVisible(true)
{
        //_drawGC = gtk_gc_new(red1, green1, blue1, pattern1, linewidth1);
        //_fillGC = gtk_gc_new(red2, green2, blue2, pattern2, linewidth2);
}

DisplaySlot* DisplaySlot::create(Cell* cell, const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1, unsigned linewidth1, unsigned short red2, unsigned short green2, unsigned short blue2, const string& pattern2, unsigned linewidth2)
// ******************************************
{
        DisplaySlot* displaySlot = new DisplaySlot(cell, name, red1, green1, blue1, pattern1, linewidth1, red2, green2, blue2, pattern2, linewidth2);

        displaySlot->_postCreate();

        return displaySlot;
}

void DisplaySlot::_postCreate()
// ***********************
{
        Inherit::_postCreate();
        StandardRelation* relation = getDisplaySlotRelation(_cell);
        if (!relation)
                relation = StandardRelation::create(_cell, DisplaySlotsCellRelationName);
        put(relation);
}

void DisplaySlot::show()
// *********************
{
        if (!_isVisible) {
                _isVisible = true;
                //for_each_view(view, _cell->getViews())
                //{
                //        view->Invalidate();
                //        end_for;
                //}
        }
}

void DisplaySlot::hide()
// *********************
{
    if (_isVisible) {
        _isVisible = false;
    }
}

void DisplaySlot::flush()
// **********************
{
    openUpdateSession();
    vector<Go*> govect;
    _quadTree.getGos().fill(govect);
    for (unsigned i = 0 ; i < govect.size() ; i++) {
        govect[i]->destroy();
    }
    closeUpdateSession();
}

UserGos DisplaySlot::getUserGos() const
// ************************************
{
    return _quadTree.getGos().getSubSet<UserGo*>();
}

UserGos DisplaySlot::getUserGosUnder(const Box& area) const
// ********************************************************
{
    return _quadTree.getGosUnder(area).getSubSet<UserGo*>();
}

string DisplaySlot::_getString() const
// ***********************************
{
        string s = Inherit::_getString();
        s.insert(s.length() - 1, " " + getString(getName()));
        return s;
}

Record* DisplaySlot::_getRecord() const
// ******************************
{
        Record* record = Inherit::_getRecord();
        if (record) {
                record->add(getSlot("Cell", _cell));
                record->add(getSlot("Name", _name));
                record->add(getSlot("QuadTree", &_quadTree));
                record->add(getSlot("Is Visible", _isVisible));
        }
        return record;
}

DisplaySlots getDisplaySlots(const Cell* cell)
{
        if (!cell)
                throw Error("Null pointer on cell while getting display slots");

        StandardRelation* relation = getDisplaySlotRelation(cell);
        if (!relation)
                return DisplaySlots();
        return relation->getSlaveOwners().getSubSet<DisplaySlot*>();
}

DisplaySlot* getDisplaySlot(const Cell* cell,const Name& name)
{
        if (!cell)
                throw Error("Null pointer on cell while getting display slots");

        StandardRelation* relation = getDisplaySlotRelation(cell);
        if (!relation)
                return NULL;
        else
        {
                for_each_display_slot(displaySlot,relation->getSlaveOwners().getSubSet<DisplaySlot*>()) {
                        if (displaySlot->getName() == name)
                                return displaySlot;
                        end_for;
                }
                return NULL;
        }
}


} // End of Hurricane namespace.
