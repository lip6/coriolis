// ****************************************************************************************************
// File: ./hurricane/Path.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#pragma  once
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
    public: int32_t contains(Instance*) const;

// Updators
// ********

    public: void makeEmpty();
    public: static void setNameSeparator(char nameSeparator);

// Others
// ******

    public: string getCompactString(bool showBrackets=true) const;
    public: string getJsonString(unsigned long flags) const;
    public: string _getTypeName() const { return _TName("Occurrence"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    public: SharedPath* _getSharedPath() const {return _sharedPath;};

};



} // End of Hurricane namespace.


INSPECTOR_PR_SUPPORT(Hurricane::Path);


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
