// ****************************************************************************************************
// File: Path.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PATH
#define HURRICANE_PATH

#include "hurricane/Instances.h"
#include "hurricane/Transformation.h"

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

    public: static char getNameSeparator();

    public: Instance* getHeadInstance() const;
    public: Path getTailPath() const;
    public: Path getHeadPath() const;
    public: Instance* getTailInstance() const;
    public: string getName() const;
    public: Cell* getOwnerCell() const;
    public: Cell* getMasterCell() const;
    public: Instances getInstances() const;
    public: Transformation getTransformation(const Transformation& transformation = Transformation()) const;

// Predicates
// **********

    public: bool isEmpty() const;

// Updators
// ********

    public: void makeEmpty();
    public: static void setNameSeparator(char nameSeparator);

// Others
// ******

    public: string _getTypeName() const { return _TName("Occurrence"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    public: SharedPath* _getSharedPath() const {return _sharedPath;};

};



} // End of Hurricane namespace.


INSPECTOR_PV_SUPPORT(Hurricane::Path);


#endif // HURRICANE_PATH

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
