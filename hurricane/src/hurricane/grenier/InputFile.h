// ****************************************************************************************************
// File: InputFile.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_INPUT_FILE
#define HURRICANE_INPUT_FILE

#include "Commons.h"

namespace Hurricane {

class DBo;
class Component;
class Hook;



// ****************************************************************************************************
// InputFile declaration
// ****************************************************************************************************

class InputFile {
// ************

// Types
// *****

	public: typedef stack<DBo*> OwnerStack;
	public: typedef vector<DBo*> DBoVector;
	public: typedef vector<Component*> ComponentVector;
	public: typedef map<Hook*, unsigned> ComponentIdMap;

// Attributes
// **********

	private: string _filePathName;
	private: FILE* _file;
	private: unsigned _lineNumber;
	private: OwnerStack _ownerStack;
	private: DBoVector _dboVector;
	private: ComponentVector _componentVector;
	private: ComponentIdMap _componentIdMap;

// Constructors
// ************

	public: InputFile(const string& filePathName);

	private: InputFile(const InputFile& inputFile); // not implemented to forbid copy

// Destructor
// **********

	public: ~InputFile();

// Operators
// *********

	private: InputFile& operator=(const InputFile& inputFile); // not implemented to forbid assinment

// Accessors
// *********

	public: DBo* GetOwner() const;
	public: DBo* GetDBo(unsigned id) const;
	public: Component* GetComponent(unsigned id) const;

// Updators
// ********

	public: void Register(DBo* dbo);
	public: void Register(Component* component);
	public: void ClearComponentVector();
	public: void Attach(Hook* hook, unsigned componentId);
	public: void AttachRemainingHooks();

// Loader
// ******

	public: void Load();

	public: bool Read(unsigned& rank, char*& token, char*& arguments);

// Others
// ******

    public: string _GetTypeName() const { return _TName("InputFile"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

};



} // End of Hurricane namespace.

#endif // HURRICANE_INPUT_FILE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
