// ****************************************************************************************************
// File: Path.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PATH
#define HURRICANE_PATH

#include "Instances.h"
#include "Transformation.h"

namespace Hurricane {

class Cell;
class SharedPath;



// ****************************************************************************************************
// Path declaration
// ****************************************************************************************************

class Path {
// *******

// Attributes
// **********

	private: SharedPath* _sharedPath;

// Constructors
// ************

	public: Path(SharedPath* sharedPath = NULL);
	public: Path(Instance* instance);
	public: Path(Instance* headInstance, const Path& tailPath);
	public: Path(const Path& headPath, Instance* tailInstance);
	public: Path(Cell* cell, const string& pathName);
    public: Path(const Path& headPath, const Path& tailPath);
	public: Path(const Path& path);

// Destructor
// **********

	public: ~Path();

// Operators
// *********

	public: Path& operator=(const Path& path);

	public: bool operator==(const Path& path) const;
	public: bool operator!=(const Path& path) const;

	public: bool operator<(const Path& path) const; // for stl set -> less predicate

// Accessors
// *********

	public: static char GetNameSeparator();

	public: Instance* GetHeadInstance() const;
	public: Path GetTailPath() const;
	public: Path GetHeadPath() const;
	public: Instance* GetTailInstance() const;
	public: string GetName() const;
	public: Cell* GetOwnerCell() const;
	public: Cell* GetMasterCell() const;
	public: Instances GetInstances() const;
	public: Transformation GetTransformation(const Transformation& transformation = Transformation()) const;

// Predicates
// **********

	public: bool IsEmpty() const;

// Updators
// ********

	public: void MakeEmpty();
	public: static void SetNameSeparator(char nameSeparator);

// Others
// ******

    public: string _GetTypeName() const { return _TName("Occurrence"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

	public: SharedPath* _GetSharedPath() const {return _sharedPath;};

};



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Path)


#endif // HURRICANE_PATH

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
