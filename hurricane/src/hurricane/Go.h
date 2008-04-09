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

    public: virtual void translate(const Unit& dx, const Unit& dy) = 0;

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


SetNestedSlotAdapter(Hurricane::Go)

#endif // HURRICANE_GO

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
