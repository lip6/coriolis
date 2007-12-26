#include "Error.h"
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
#include "Relation.h"
#include "Cell.h"

#include "VisualMap.h"

namespace Hurricane {

static Name VisualMapsCellRelationName("VisualMapsCellRelation");

static StandardRelation* GetVisualMapRelation(const Cell* cell)
{
	Property* property = cell->GetProperty(VisualMapsCellRelationName);
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
// VisualMap implementation
// ****************************************************************************************************

VisualMap::VisualMap(Cell* cell,const Name& name)
// ***********************
	:  Inherit()
	, _cell(cell)
	, _name(name)
	, _GCs()
	, _isVisible(true)
{
	//_GC = gtk_gc_new(red, green, blue, pattern);
    _GCs.resize(256);
}


void VisualMap::_PostCreate()
// ***********************
{
	Inherit::_PostCreate();
	StandardRelation* relation = GetVisualMapRelation(_cell);
	if (!relation)
		relation = StandardRelation::Create(_cell, VisualMapsCellRelationName);
	Put(relation);
}

void VisualMap::SetGC (unsigned short index, unsigned short red, unsigned short green, unsigned short blue, const string& pattern)
// ***********************
{
    if (_GCs[index])
    {
        gdk_gc_destroy(_GCs[index]);
        _GCs[index] = NULL;
    }
   _GCs[index] = gtk_gc_new(red, green, blue, pattern);
   return;
}

void VisualMap::Show()
// *********************
{
	if (!_isVisible) {
		_isVisible = true;
		for_each_view(view, _cell->GetViews())
		{
			view->Invalidate();
			end_for;
		}
	}
}

void VisualMap::Hide()
// *********************
{
	if (_isVisible) {
		_isVisible = false;
		for_each_view(view, _cell->GetViews())
		{
		    view->Invalidate();
		    end_for;
		}
	}
}

void VisualMap::_PreDelete()
// ***************************
{
	Inherit::_PreDelete();
    for (unsigned short i = 0 ; i < 255 ; i ++)
    {
        if (_GCs[i])
        {
            gdk_gc_destroy(_GCs[i]);
            _GCs[i] = NULL;
        }
        _GCs[i] = NULL;
    }
}

string VisualMap::_GetString() const
// ***********************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(GetName()));
	return s;
}

Record* VisualMap::_GetRecord() const
// ******************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Cell", _cell));
		record->Add(GetSlot("Name", _name));
		record->Add(GetSlot("Is Visible", _isVisible));
	}
	return record;
}

VisualMaps GetVisualMaps(const Cell* cell)
{
	if (!cell)
		throw Error("Null pointer on cell while getting display slots");

	StandardRelation* relation = GetVisualMapRelation(cell);
	if (!relation)
		return VisualMaps();
	return relation->GetSlaveOwners().GetSubSet<VisualMap*>();
}

VisualMap* GetVisualMap(const Cell* cell,const Name& name)
{
	if (!cell)
		throw Error("Null pointer on cell while getting display slots");

	StandardRelation* relation = GetVisualMapRelation(cell);
	if (!relation)
		return NULL;
	else
	{
		for_each_visual_map(visualMap,relation->GetSlaveOwners().GetSubSet<VisualMap*>())
		{
			if (visualMap->GetName() == name)
				return visualMap;
			end_for;
		}
		return NULL;
	}
}


} // End of Hurricane namespace.
