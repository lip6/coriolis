// ****************************************************************************************************
// File: DataBase.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_DATA_BASE
#define HURRICANE_DATA_BASE

#include "DBo.h"
#include "Unit.h"

namespace Hurricane {

class Library;
class Technology;



// ****************************************************************************************************
// DataBase declaration
// ****************************************************************************************************

class DataBase : public DBo {
// ************************

#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

    public: typedef DBo Inherit;

// Attributes
// **********

    private: Technology* _technology;
    private: Library* _rootLibrary;

// Constructors
// ************

    protected: DataBase();

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("DataBase");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: void _setTechnology(Technology* technology) {_technology = technology;};
    public: void _setRootLibrary(Library* rootLibrary) {_rootLibrary = rootLibrary;};

#   endif

    public: static DataBase* create();

// Accessors
// *********

    public: Technology* getTechnology() const {return _technology;};
    public: Library* getRootLibrary() const {return _rootLibrary;};

};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

DataBase* getDataBase();



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::DataBase)

#endif // HURRICANE_DATA_BASE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
