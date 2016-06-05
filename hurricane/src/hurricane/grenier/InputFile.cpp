// ****************************************************************************************************
// File: InputFile.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************

#include "InputFile.h"
#include "Builder.h"
#include "Hurricane.h"

namespace Hurricane {



// ****************************************************************************************************
// InputFile implementation
// ****************************************************************************************************

InputFile::InputFile(const string& filePathName)
// *********************************************
:	_filePathName(filePathName),
	_file(NULL),
	_lineNumber(0),
	_ownerStack(),
	_dboVector(),
	_componentVector(),
	_componentIdMap()
{
	if (_filePathName.empty())
		throw Error("Can't create InputFile : empty file pathname");

	_file = fopen(_filePathName.c_str(), "r");

	if (!_file)
		throw Error("Can't create InputFile : bad file");

	_dboVector.push_back(NULL); // to start indexation at 1
}

InputFile::~InputFile()
// ********************
{
	fclose(_file);
}

DBo* InputFile::GetOwner() const
// *****************************
{
	return (!_ownerStack.empty()) ? _ownerStack.top() : NULL;
}

DBo* InputFile::GetDBo(unsigned id) const
// **************************************
{
	return (id < _dboVector.size()) ? _dboVector[id] : NULL;
}

Component* InputFile::GetComponent(unsigned id) const
// **************************************************
{
	return (id < _componentVector.size()) ? _componentVector[id] : NULL;
}

void InputFile::Register(DBo* dbo)
// *******************************
{
	_dboVector.push_back(dbo);
}

void InputFile::Register(Component* component)
// *******************************************
{
	_componentVector.push_back(component);
}

void InputFile::ClearComponentVector()
// ***********************************
{
	_componentVector.clear();
	_componentVector.push_back(NULL); // to start indexation at 1
}

void InputFile::Attach(Hook* hook, unsigned componentId)
// *****************************************************
{
	Component* component = GetComponent(componentId);
	if (!component)
		_componentIdMap[hook] = componentId;
	else {
		if (hook->IsMaster() && hook->IsAttached())
			hook->Merge(component->GetBodyHook());
		else
			hook->Attach(component->GetBodyHook());
	}
}

void InputFile::AttachRemainingHooks()
// ***********************************
{
	for (ComponentIdMap::iterator it = _componentIdMap.begin(); it != _componentIdMap.end(); ++it) {
		Hook* hook = (*it).first;
		Component* component = GetComponent((*it).second);
		if (!component) throw Error("Can't attach hook : no target component");
		if (hook->IsMaster() && hook->IsAttached())
			hook->Merge(component->GetBodyHook());
		else
			hook->Attach(component->GetBodyHook());
	}
	_componentIdMap.clear();
}

void InputFile::Load()
// *******************
{
	unsigned rank;
	char* token;
	char* arguments;
	while (Read(rank, token, arguments)) {

		while (rank < _ownerStack.size()) {
			DBo* dbo = _ownerStack.top();
			if (dbo) dbo->_Finalize(*this);
			_ownerStack.pop();
		}

		Builder* builder = GetBuilder(token);

		if (!builder)
			throw Error("Can't load input file : unknown directive " + string(token) +
							" (line " + GetString(_lineNumber) + ")");

		builder->Scan(*this, arguments);

		DBo* dbo = builder->CreateDBo();

		if (dbo) dbo->_Realize(builder, *this);

		_ownerStack.push(dbo);
	}

	while (!_ownerStack.empty()) {
		DBo* dbo = _ownerStack.top();
		if (dbo) dbo->_Finalize(*this);
		_ownerStack.pop();
	}
}

bool InputFile::Read(unsigned& rank, char*& token, char*& arguments)
// *****************************************************************
{
	static char LINE[1024];
	static char TOKEN[128];

	_lineNumber++;

	LINE[0] = fgetc(_file);

	if (LINE[0] == EOF) return false;

	if (LINE[0] == '\n') return Read(rank, token, arguments);

	fgets(&LINE[1], sizeof(LINE) - 1, _file);

	char* line = &LINE[0];
	while ((line[0] == ' ') || (line[0] == '\t')) line = &line[1];

	if (line[0] == '\n') return Read(rank, token, arguments);

	line = &LINE[0];

	rank = 0;

	while ((line[0] == '\t') && (LINE[0] != '\n')) {
		rank++;
		line = &line[1];
	}

	unsigned n;
	sscanf(line, "%s%n", TOKEN, &n);
	token = &TOKEN[0];
	arguments = &line[n];
	while ((arguments[0] == ' ') || (arguments[0] == '\t')) arguments = &arguments[1];
	arguments[strlen(arguments) - 1] = '\0';

	return true;
}

string InputFile::_GetString() const
// *********************************
{
	return "<" + _TName("InputFile") + " " + _filePathName + ">";
}

Record* InputFile::_GetRecord() const
// ****************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("FilePathName", &_filePathName));
		record->Add(GetSlot("DBos", &_dboVector));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2016, All Rights Reserved
// ****************************************************************************************************
