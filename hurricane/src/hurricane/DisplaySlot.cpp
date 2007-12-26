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

static StandardRelation* GetDisplaySlotRelation(const Cell* cell)
{
        Property* property = cell->GetProperty(DisplaySlotsCellRelationName);
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

DisplaySlot* DisplaySlot::Create(Cell* cell, const Name& name, unsigned short red1, unsigned short green1, unsigned short blue1, const string& pattern1, unsigned linewidth1, unsigned short red2, unsigned short green2, unsigned short blue2, const string& pattern2, unsigned linewidth2)
// ******************************************
{
        DisplaySlot* displaySlot = new DisplaySlot(cell, name, red1, green1, blue1, pattern1, linewidth1, red2, green2, blue2, pattern2, linewidth2);

        displaySlot->_PostCreate();

        return displaySlot;
}

void DisplaySlot::_PostCreate()
// ***********************
{
        Inherit::_PostCreate();
        StandardRelation* relation = GetDisplaySlotRelation(_cell);
        if (!relation)
                relation = StandardRelation::Create(_cell, DisplaySlotsCellRelationName);
        Put(relation);
}

void DisplaySlot::Show()
// *********************
{
        if (!_isVisible) {
                _isVisible = true;
                //for_each_view(view, _cell->GetViews())
                //{
                //        view->Invalidate();
                //        end_for;
                //}
        }
}

void DisplaySlot::Hide()
// *********************
{
        if (_isVisible) {
                _isVisible = false;
                //for_each_view(view, _cell->GetViews())
                //{
                //    view->Invalidate();
                //    end_for;
                //}
        }
}

void DisplaySlot::Flush()
// **********************
{
    OpenUpdateSession();
    vector<Go*> govect;
    _quadTree.GetGos().Fill(govect);
    for (unsigned i = 0 ; i < govect.size() ; i++)
    {
        govect[i]->Delete();
    }
    CloseUpdateSession();
}

//void DisplaySlot::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
//// *********************************************************************************************
//{
//        if (GetBoundingBox().Intersect(updateArea)) {
//                for_each_go(go, _quadTree.GetGos())
//                {
//                        go->_Draw(view, NULL, updateArea, transformation);
//                        end_for;
//                }
//        }
//}
//
void DisplaySlot::_PreDelete()
// ***************************
{
        Inherit::_PreDelete();
        //gdk_gc_destroy(_drawGC);
        //gdk_gc_destroy(_fillGC);
}

UserGos DisplaySlot::GetUserGos() const
// ************************************
{
    return _quadTree.GetGos().GetSubSet<UserGo*>();
}

UserGos DisplaySlot::GetUserGosUnder(const Box& area) const
// ********************************************************
{
    return _quadTree.GetGosUnder(area).GetSubSet<UserGo*>();
}

string DisplaySlot::_GetString() const
// ***********************************
{
        string s = Inherit::_GetString();
        s.insert(s.length() - 1, " " + GetString(GetName()));
        return s;
}

Record* DisplaySlot::_GetRecord() const
// ******************************
{
        Record* record = Inherit::_GetRecord();
        if (record) {
                record->Add(GetSlot("Cell", _cell));
                record->Add(GetSlot("Name", _name));
                record->Add(GetSlot("QuadTree", &_quadTree));
                record->Add(GetSlot("Is Visible", _isVisible));
        }
        return record;
}

DisplaySlots GetDisplaySlots(const Cell* cell)
{
        if (!cell)
                throw Error("Null pointer on cell while getting display slots");

        StandardRelation* relation = GetDisplaySlotRelation(cell);
        if (!relation)
                return DisplaySlots();
        return relation->GetSlaveOwners().GetSubSet<DisplaySlot*>();
}

DisplaySlot* GetDisplaySlot(const Cell* cell,const Name& name)
{
        if (!cell)
                throw Error("Null pointer on cell while getting display slots");

        StandardRelation* relation = GetDisplaySlotRelation(cell);
        if (!relation)
                return NULL;
        else
        {
                for_each_display_slot(displaySlot,relation->GetSlaveOwners().GetSubSet<DisplaySlot*>())
                {
                        if (displaySlot->GetName() == name)
                                return displaySlot;
                        end_for;
                }
                return NULL;
        }
}


} // End of Hurricane namespace.
