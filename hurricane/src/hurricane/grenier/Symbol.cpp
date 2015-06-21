// ****************************************************************************************************
// File: Symbol.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#include "Symbol.h"
#include "Library.h"
#include "Net.h"
#include "View.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Net comparator
// ****************************************************************************************************

struct Symbol_NetSetComparator {
// ***************************

	bool operator()(Net* net1, Net* net2) const
	// ****************************************
	{
		return (net1->GetName() < net2->GetName());
	};

};



// ****************************************************************************************************
// Symbol implementation
// ****************************************************************************************************

Symbol::Symbol(Library* library, const Name& name)
// ***********************************************
:	Inherit(),
	_library(library),
	_name(name),
	_cellSet(),
	_primitiveList(),
	_boundingBox(),
	_nextOfLibrarySymbolMap(NULL)
{
	if (!_library)
		throw Error("Can't create " + _TName("Symbol") + " : null library");

	if (name.IsEmpty())
		throw Error("Can't create " + _TName("Symbol") + " : empty name");

	if (_library->GetSymbol(_name))
		throw Error("Can't create " + _TName("Symbol") + " : already exists");
}

Symbol* Symbol::Create(Library* library, const Name& name)
// *******************************************************
{
	Symbol* symbol = new Symbol(library, name);

	symbol->_PostCreate();

	return symbol;
}

Symbol* Symbol::CreateFor(Cell* cell)
// **********************************
{
	if (!cell)
		throw Error("Can't create " + _TName("Symbol") + " for cell : null cell");

	char name[32];
	static unsigned NEW_SYMBOL_ID = 1;
	sprintf(name, "~symbol%d", NEW_SYMBOL_ID++);

	Symbol* symbol = new Symbol(cell->GetLibrary(), name);

	symbol->_PostCreate();

	set<Net*, Symbol_NetSetComparator> westNetSet;
	set<Net*, Symbol_NetSetComparator> eastNetSet;
	set<Net*, Symbol_NetSetComparator> northNetSet;
	set<Net*, Symbol_NetSetComparator> southNetSet;

	for_each_net(net, cell->GetExternalNets()) {
		if (!net->IsSupply()) {
			switch (net->GetDirection()) {
				case Net::Direction::IN : westNetSet.insert(net); break;
				case Net::Direction::OUT : eastNetSet.insert(net); break;
				case Net::Direction::UNDEFINED : northNetSet.insert(net); break;
				default : southNetSet.insert(net); break;
			}
		}
		end_for;
	}

	int westSize = westNetSet.size();
	int eastSize = eastNetSet.size();
	int northSize = northNetSet.size();
	int southSize = southNetSet.size();

	int width = max(max(northSize, southSize), 1) * 4;
	int halfWidth = width / 2;
	int height = max(max(westSize, eastSize), 1) * 4;
	int halfHeight = height / 2;

	Rectangle::Create(symbol, Box(0, 0).Inflate(GetUnit(halfWidth), GetUnit(halfHeight)), true);

	if (westSize) {
		int offset = (height - ((westSize - 1) * 4)) / 2;
		int x = - halfWidth;
		int y = offset - halfHeight;
		for_each_net(net, GetCollection(westNetSet)) {
			Line::Create(symbol, GetUnit(x), GetUnit(y), GetUnit(x - 1), GetUnit(y));
			Port::Create(symbol, net->GetName(), GetUnit(x - 1), GetUnit(y));
			y += 4;
			end_for;
		}
	}

	if (eastSize) {
		int offset = (height - ((eastSize - 1) * 4)) / 2;
		int x = halfWidth;
		int y = offset - halfHeight;
		for_each_net(net, GetCollection(eastNetSet)) {
			Line::Create(symbol, GetUnit(x), GetUnit(y), GetUnit(x + 1), GetUnit(y));
			Port::Create(symbol, net->GetName(), GetUnit(x + 1), GetUnit(y));
			y += 4;
			end_for;
		}
	}

	if (northSize) {
		int offset = (width - ((northSize - 1) * 4)) / 2;
		int x = offset - halfWidth;
		int y = halfHeight;
		for_each_net(net, GetCollection(northNetSet)) {
			Line::Create(symbol, GetUnit(x), GetUnit(y), GetUnit(x), GetUnit(y + 1));
			Port::Create(symbol, net->GetName(), GetUnit(x), GetUnit(y + 1));
			x += 4;
			end_for;
		}
	}

	if (southSize) {
		int offset = (width - ((southSize - 1) * 4)) / 2;
		int x = offset - halfWidth;
		int y = - halfHeight;
		for_each_net(net, GetCollection(southNetSet)) {
			Line::Create(symbol, GetUnit(x), GetUnit(y), GetUnit(x), GetUnit(y - 1));
			Port::Create(symbol, net->GetName(), GetUnit(x), GetUnit(y - 1));
			x += 4;
			end_for;
		}
	}

	return symbol;
}

Box Symbol::GetBoundingBox() const
// *******************************
{
	if (_boundingBox.IsEmpty()) {
		Box& boundingBox = (Box&)_boundingBox;
		for_each_primitive(primitive, GetPrimitives()) {
			boundingBox.Merge(primitive->GetBoundingBox());
			end_for;
		}
	}
	
	return _boundingBox;
}

Port* Symbol::GetPort(const Name& name) const
// ******************************************
{
	for_each_primitive(primitive, GetPrimitives()) {
		if (is_a<Port*>(primitive) && (((Port*)primitive)->GetName() == name))
			return (Port*)primitive;
		end_for;
	}
	return NULL;
}

void Symbol::SetName(const Name& name)
// ***********************************
{
	if (name != _name) {
		if (name.IsEmpty())
			throw Error("Can't change " + _TName("Symbol") + " name : empty name");

		if (_library->GetSymbol(name))
			throw Error("Can't change " + _TName("Symbol") + " name : already exists");

		_library->_GetSymbolMap()._Remove(this);
		_name = name;
		_library->_GetSymbolMap()._Insert(this);
	}
}

void Symbol::_PostCreate()
// ***********************
{
	_library->_GetSymbolMap()._Insert(this);

	Inherit::_PostCreate();
}

void Symbol::_PreDelete()
// **********************
{
	Inherit::_PreDelete();

	for_each_cell(cell, GetCells()) cell->SetSymbol(NULL); end_for;
	for_each_primitive(primitive, GetPrimitives()) primitive->Delete(); end_for;

	_library->_GetSymbolMap()._Remove(this);
}

string Symbol::_GetString() const
// ******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_name));
	return s;
}

Record* Symbol::_GetRecord() const
// *************************
{
 	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Library", _library));
		record->Add(GetSlot("Name", &_name));
		record->Add(GetSlot("Cells", &_cellSet));
		record->Add(GetSlot("Primitives", &_primitiveList));
		record->Add(GetSlot("BoundingBox", &_boundingBox));
	}
	return record;
}

void Symbol::_Fit(const Box& box)
// ******************************
{
	if (box.IsEmpty()) return;
	if (_boundingBox.IsEmpty()) return;
	if (_boundingBox.Contains(box)) return;
	_boundingBox.Merge(box);
	for_each_cell(cell, GetCells()) {
		cell->_Fit(box);
		end_for;
	}
}

void Symbol::_Unfit(const Box& box)
// ********************************
{
	if (box.IsEmpty()) return;
	if (_boundingBox.IsEmpty()) return;
	if (!_boundingBox.IsConstrainedBy(box)) return;
	for_each_cell(cell, GetCells()) {
		cell->_Unfit(box);
		end_for;
	}
	_boundingBox.MakeEmpty();
}

bool Symbol::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
// **************************************************************************************
{
	for_each_primitive(primitive, GetPrimitives()) {
		if (primitive->_IsInterceptedBy(view, point, aperture)) return true;
		end_for;
	}
	return false;
}

void Symbol::_Draw(View* view, const Box& updateArea, const Transformation& transformation)
// ****************************************************************************************
{
	for_each_primitive(primitive, GetPrimitives()) {
		primitive->_Draw(view, updateArea, transformation);
		end_for;
	}
}

void Symbol::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
// *********************************************************************************************
{
	for_each_primitive(primitive, GetPrimitives()) {
		primitive->_Highlight(view, updateArea, transformation);
		end_for;
	}
}

void Symbol::_SaveHeaderTo(OutputFile& outputFile)
// ***********************************************
{
}

void Symbol::_SaveContentTo(OutputFile& outputFile)
// ************************************************
{
}



// ****************************************************************************************************
// Symbol::CellSet implementation
// ****************************************************************************************************

Symbol::CellSet::CellSet()
// ***********************
:	Inherit()
{
}

unsigned Symbol::CellSet::_GetHashValue(Cell* cell) const
// ******************************************************
{
	return ( (unsigned int)( (unsigned long)cell ) ) / 8;
}

Cell* Symbol::CellSet::_GetNextElement(Cell* cell) const
// *****************************************************
{
	return cell->_GetNextOfSymbolCellSet();
}

void Symbol::CellSet::_SetNextElement(Cell* cell, Cell* nextCell) const
// ********************************************************************
{
	cell->_SetNextOfSymbolCellSet(nextCell);
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
