// ****************************************************************************************************
// File: OutputFile.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_OUTPUT_FILE
#define HURRICANE_OUTPUT_FILE

#include "Commons.h"
#include "Cells.h"

namespace Hurricane {

class DBo;
class Cell;
class Component;



// ****************************************************************************************************
// OutputFile declaration
// ****************************************************************************************************

class OutputFile {
// *************

// Types
// *****

	public: typedef map<DBo*, unsigned> DBoIdMap;
	public: typedef map<Component*, unsigned> ComponentIdMap;
	public: typedef set<Cell*> CellSet;
	public: typedef vector<Cell*> CellVector;

// Attributes
// **********

	private: string _filePathName;
	private: ofstream _file;
	private: Tabulation _tab;
	private: DBoIdMap _dboIdMap;
	private: ComponentIdMap _componentIdMap;
	private: CellSet _cellSet;
	private: CellVector _cellVector;

// Constructors
// ************

	public: OutputFile(const string& filePathName);

	private: OutputFile(const OutputFile& outputFile); // not implemented to forbid copy

// Operators
// *********

	private: OutputFile& operator=(const OutputFile& outputFile); // not implemented to forbid assinment

	public: template<class Data> OutputFile& operator<<(Data data)
	// ***********************************************************
	{
		_file << GetString(data);
		return *this;
	}

// Accessors
// *********

	public: const string& GetFilePathName() const {return _filePathName;}

	public: unsigned GetId(DBo* dbo);
	public: unsigned GetId(Component* component);

	public: Cells GetCells(); // {return GetCollection(_cellVector);}; NOON!!!

// Managers
// ********

	public: void Push(Cell* cell);

	public: void Register(DBo* dbo);
	public: void Register(Component* component);

	public: void OpenHeader();
	public: void CloseHeader();
	public: void OpenContent();
	public: void CloseContent();

	public: void ClearComponentIdMap();

// Others
// ******

    public: string _GetTypeName() const { return _TName("OutputFile"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

};



} // End of Hurricane namespace.

#endif // HURRICANE_OUTPUT_FILE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
