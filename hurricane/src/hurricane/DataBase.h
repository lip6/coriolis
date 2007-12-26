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

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("DataBase");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: void _SetTechnology(Technology* technology) {_technology = technology;};
	public: void _SetRootLibrary(Library* rootLibrary) {_rootLibrary = rootLibrary;};

#   endif

	public: static DataBase* Create();

// Accessors
// *********

	public: Technology* GetTechnology() const {return _technology;};
	public: Library* GetRootLibrary() const {return _rootLibrary;};

};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

DataBase* GetDataBase();



} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::DataBase)

#endif // HURRICANE_DATA_BASE

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
