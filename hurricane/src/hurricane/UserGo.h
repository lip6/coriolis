// ****************************************************************************************************
// 
// This file is part of the Tsunami Project.
// Copyright (c) 2001-2004  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: UserGo.h
// Authors: C. Alexandre
// ****************************************************************************************************

#ifndef HURRICANE_USER_GO
#define HURRICANE_USER_GO

#include "Go.h"
#include "UserGos.h"
#include "DisplaySlot.h"

namespace Hurricane {

// ****************************************************************************************************
// UserGo declaration
// ****************************************************************************************************

class UserGo : public Go {
// *********************

// Types
// *****

    public: typedef Go Inherit;

// Attributes
// **********
    protected: DisplaySlot* _displaySlot;

// Constructors
// ************

    protected: UserGo(DisplaySlot* displaySlot);

// Updators
// ********

    public: virtual void materialize();
    public: virtual void unmaterialize();

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("UserGo");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
};

UserGos getUserGos(const Cell* cell);

} // End of Hurricane namespace.

#endif // HURRICANE_USER_GO
