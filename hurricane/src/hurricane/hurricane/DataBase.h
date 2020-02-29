// ****************************************************************************************************
// File: ./hurricane/DataBase.h
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

#ifndef HURRICANE_DATA_BASE
#define HURRICANE_DATA_BASE

#include <functional>
#include <map>
#include "hurricane/DBo.h"
#include "hurricane/DbU.h"

namespace Hurricane {

class Cell;
class Library;
class Technology;



// ****************************************************************************************************
// DataBase declaration
// ****************************************************************************************************

class DataBase : public DBo {
// ************************

// Types
// *****

  public: typedef DBo Inherit;
  public: enum Flags { NoFlags      = 0
                     , CreateLib    =(1<<0)
                     , WarnCreateLib=(1<<2) };

// Attributes
// **********

    private: static DataBase* _db;
    private: Technology* _technology;
    private: Library* _rootLibrary;
    private: function<Hurricane::Cell*(string)> _cellLoader;

// Constructors
// ************

    protected: DataBase();

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual void _toJson(JsonWriter*) const;
    public: virtual string _getTypeName() const {return _TName("DataBase");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: void _setTechnology(Technology* technology) {_technology = technology;};
    public: void _setRootLibrary(Library* rootLibrary) {_rootLibrary = rootLibrary;};
    public: void _setCellLoader(function<Hurricane::Cell*(string)> loader) { _cellLoader=loader; };

    public: static DataBase* create();

// Accessors
// *********

    public: Technology* getTechnology() const {return _technology;};
    public: Library* getRootLibrary() const {return _rootLibrary;};
    public: Library* getLibrary(string,unsigned int flags);
    public: Cell* getCell(string, unsigned int flags);
    public: Cell* getCell(string);
    public: void  clear();
    public: static DataBase* getDB();

};



// ****************************************************************************************************
// JsonDataBase declaration
// ****************************************************************************************************

class JsonDataBase : public JsonDBo {
// ********************************

  public: static void initialize();
  public: JsonDataBase(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonDataBase* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::DataBase);


#endif // HURRICANE_DATA_BASE


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
