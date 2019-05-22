// ****************************************************************************************************
// File: ./hurricane/UpdateSession.h
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

#ifndef HURRICANE_UPDATE_SESSION
#define HURRICANE_UPDATE_SESSION

#include "hurricane/Property.h"

namespace Hurricane {

class Go;



// ****************************************************************************************************
// UpdateSession declaration
// ****************************************************************************************************

class UpdateSession : public SharedProperty {
// ****************************************

// Types
// *****

    public: typedef SharedProperty Inherit;

// Constructors
// ************

    protected: UpdateSession();

    public: virtual void destroy();

// Accessors
// *********

    public: static const Name& getPropertyName();
    public: virtual Name getName() const {return getPropertyName();};

// Managers
// ********

    public: virtual void onCapturedBy(DBo* owner);
    public: virtual void onNotOwned();

// Ohers
// *****

    public: static UpdateSession* _create();
    protected: virtual void _postCreate();

    public: void _destroy();
    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("UpdateSession");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: static void open();
    public: static void close();
    public: static void reset();
    public: static size_t  getStackSize();


};


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

//void openUpdateSession();
//
//void closeUpdateSession();
//


} // End of Hurricane namespace.


#endif // HURRICANE_UPDATE_SESSION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
