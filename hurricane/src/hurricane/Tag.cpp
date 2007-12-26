// ****************************************************************************************************
// File: Tag.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Tag.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Utilitarians
// ****************************************************************************************************

static bool Read(FILE* file, unsigned& rank, string& name, string& argument)
// *************************************************************************
{
	rank = 0;
	name = "";
	argument = "";

	char c = fgetc(file);

	if (c == EOF) return false;

	while ((c != EOF) && (c == '\t')) {
		rank++;
		c = fgetc(file);
	}

	if (c == '#') {
		while ((c != EOF) && (c != '\n')) c = fgetc(file);
		return true;
	}

	while ((c != EOF) && ((c == '\t') || (c == ' ')))
		c = fgetc(file);

	while ((c != EOF) && (c != '\t') && (c != ' ') && (c != '\n')) {
		name += c;
		c = fgetc(file);
	}

	while ((c != EOF) && ((c == '\t') || (c == ' ')))
		c = fgetc(file);

	while ((c != EOF) && (c != '\n')) {
		argument += c;
		c = fgetc(file);
	}

	return true;
}



// ****************************************************************************************************
// Tag implementation
// ****************************************************************************************************

Tag::Tag(Tag* tag, unsigned line, unsigned rank, const string& name, const string& argument)
// *****************************************************************************************
:	_tag(tag),
	_line(line),
	_rank(rank),
	_name(name),
	_argument(argument),
	_tagList()
{
	if (_tag) _tag->_tagList.push_back(this);
}

Tag::Tag(const string& filePathName, const string& name)
// *****************************************************
:	_tag(NULL),
	_line(0),
	_rank(0),
	_name(name),
	_argument(filePathName),
	_tagList()
{
	if (filePathName.empty())
		throw Error("Can't create " + _TName("Tag") + " from file : empty file pathname");

	FILE* file = fopen(filePathName.c_str(), "r");
	if (!file)
		throw Error("Can't create " + _TName("Tag") + ", unreadable file : " + filePathName);

	unsigned rank;
	string token;
	string argument;
	unsigned line = 1;

	while (Read(file, rank, token, argument)) {
		if (!token.empty()) new Tag(this, line, rank, token, argument);
		line++;
	}
	fclose(file);

	stack<Tag*> tagStack;
	TagList::iterator it = _tagList.begin();
	while (it != _tagList.end()) {
		Tag* tag = *it++;
		unsigned rank = tag->_rank;
		while (!tagStack.empty() && (rank <= tagStack.top()->_rank)) tagStack.pop();
		Tag* parent = (tagStack.empty()) ? NULL : tagStack.top();
		if (parent && (parent->_rank != (rank - 1))) parent = NULL;
		if (!parent) {
			if (rank)
				throw Error("Syntax error : bad indentation (line " + GetString(tag->_line) + ")");
		}
		else {
			_tagList.remove(tag);
			tag->_tag = parent;
			parent->_tagList.push_back(tag);
		}
		tagStack.push(tag);
	}
}

Tag::~Tag()
// ********
{
	if (_tag) _tag->_tagList.remove(this);

	while (!_tagList.empty()) delete *_tagList.begin();
}

string Tag::_GetString() const
// ***************************
{
	return "<" + _TName("Tag") + " " + _name + " " + _argument + ">";
}

Record* Tag::_GetRecord() const
// **********************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Tag", _tag));
	record->Add(GetSlot("Line", &_line));
	record->Add(GetSlot("Rank", &_rank));
	record->Add(GetSlot("Name", &_name));
	record->Add(GetSlot("Argument", &_argument));
	record->Add(GetSlot("Tags", &_tagList));
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
