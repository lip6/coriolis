// ****************************************************************************************************
// File: Tag.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_TAG
#define HURRICANE_TAG

#include "Tags.h"

namespace Hurricane {



// ****************************************************************************************************
// Tag declaration
// ****************************************************************************************************

class Tag {
// ******

// Types
// *****

	public: typedef list<Tag*> TagList;

// Attributes
// **********

	private: Tag* _tag;
	private: unsigned _line;
	private: unsigned _rank;
	private: string _name;
	private: string _argument;
	private: TagList _tagList;

// Constructors
// ************

	private: Tag(Tag* tag, unsigned line, unsigned rank, const string& name, const string& argument);

	public: Tag(const string& filePathName, const string& name);

	private: Tag(const Tag& tag); // not implemented to forbid copy

// Destructor
// **********

	public: ~Tag();

// Operators
// *********

	private: Tag& operator=(const Tag& tag); // not implemented to forbid assignment

// Accessors
// *********

	public: Tag* GetTag() const {return _tag;};
	public: unsigned GetLine() const {return _line;};
	public: unsigned GetRank() const {return _rank;};
	public: const string& GetName() const {return _name;};
	public: const string& GetArgument() const {return _argument;};
	public: Tags GetTags() const {return GetCollection(_tagList);};

// Others
// ******

    public: string _GetTypeName() const { return _TName("Tag"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;
	public: TagList& _GetTagList() {return _tagList;};

};



} // End of Hurricane namespace.



// ****************************************************************************************************
// Globals
// ****************************************************************************************************


#endif // HURRICANE_TAG

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
