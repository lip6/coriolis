// ****************************************************************************************************
// File: Go.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_GO
#define HURRICANE_GO

#include "Entity.h"
#include "Gos.h"
#include "Transformation.h"

namespace Hurricane {

//class View;
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

// Predicates
// **********

    public: static bool AutoMaterializationIsDisabled();

    public: bool IsMaterialized() const {return (_quadTree != NULL);};

// Updators
// ********

    public: static void EnableAutoMaterialization();
    public: static void DisableAutoMaterialization();

    public: virtual void Materialize() = 0;
    public: virtual void Unmaterialize() = 0;

    public: virtual void Invalidate(bool propagateFlag = true);
              // implementation located on file UpdateSession.cpp to access local variables

    public: virtual void Translate(const Unit& dx, const Unit& dy) = 0;

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: Go* _getNextOfQuadTreeGoSet() const {return _nextOfQuadTreeGoSet;};

    public: void _SetNextOfQuadTreeGoSet(Go* go) {_nextOfQuadTreeGoSet = go;};

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Go)

#endif // HURRICANE_GO

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
