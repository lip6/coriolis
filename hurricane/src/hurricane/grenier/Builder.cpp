// ****************************************************************************************************
// File: Builder.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

#include "Builder.h"

#include "Hurricane.h"

namespace Hurricane {



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

typedef map<string, Builder*> Builder_BuilderMap;

static Builder_BuilderMap* BUILDER_MAP = NULL;



// ****************************************************************************************************
// Builder implementation
// ****************************************************************************************************

Builder::Builder(const string& token)
// **********************************
:	_token(token)
{
	if (_token.empty())
		throw Error("Can't create Builder : empty token");

	if (!BUILDER_MAP)
		BUILDER_MAP = new Builder_BuilderMap();
	else {
		if (GetBuilder(_token))
			throw Error("Can't create Builder : already exists");
	}

	(*BUILDER_MAP)[_token] = this;
}

Builder::~Builder()
// ****************
{
	BUILDER_MAP->erase(_token);

	if (BUILDER_MAP->empty()) {
		delete BUILDER_MAP;
		BUILDER_MAP = NULL;
	}
}

void Builder::Scan(InputFile& inputFile, char*& arguments)
// *******************************************************
{
}

string Builder::_GetString() const
// *******************************
{
	string s = "<" + _GetTypeName() + ">";
	return s;
}

Record* Builder::_GetRecord() const
// **************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("Token", GetToken()));
	}
	return record;
}



} // End of Hurricane namespace.



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

H::Builder* GetBuilder(const string& token)
// *************************************
{
	if (H::BUILDER_MAP) {
		H::Builder_BuilderMap::iterator it = H::BUILDER_MAP->find(token);
		if (it != H::BUILDER_MAP->end()) return (*it).second;
	}

	return NULL;
}

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
