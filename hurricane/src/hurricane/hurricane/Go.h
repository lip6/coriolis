// ****************************************************************************************************
// File: ./hurricane/Go.h
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
#include "hurricane/Entity.h"
#include "hurricane/Gos.h"
#include "hurricane/Transformation.h"

namespace Hurricane {

class BasicLayer;
class QuadTree;

// ****************************************************************************************************
// Go declaration
// ****************************************************************************************************

class Go : public Entity {
// *********************

// Friends
// *******

    friend class QuadTree;

// Types
// *****

    public: typedef Entity Inherit;

// Attributes
// **********

    private: QuadTree* _quadTree;
    private: Go* _nextOfQuadTreeGoSet;

// Constructors
// ************

    protected: Go();
    protected: virtual ~Go();

// Predicates
// **********

    public: static bool autoMaterializationIsDisabled();

    public: bool isMaterialized() const {return (_quadTree != NULL);};

// Updators
// ********

    public: static void enableAutoMaterialization();
    public: static void disableAutoMaterialization();

    public: virtual void materialize() = 0;
    public: virtual void unmaterialize() = 0;

    public: virtual void invalidate(bool propagateFlag = true);
              // implementation located on file UpdateSession.cpp to access local variables

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy) = 0;

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Go* _getNextOfQuadTreeGoSet() const {return _nextOfQuadTreeGoSet;};

    public: void _setNextOfQuadTreeGoSet(Go* go) {_nextOfQuadTreeGoSet = go;};

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Go);


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
