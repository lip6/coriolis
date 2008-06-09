// ****************************************************************************************************
// File: Rubber.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_RUBBER
#define HURRICANE_RUBBER

#include "hurricane/Go.h"
#include "hurricane/Hooks.h"
#include "hurricane/Rubbers.h"

namespace Hurricane {

class Net;



// ****************************************************************************************************
// Rubber declaration
// ****************************************************************************************************

class Rubber : public Go {
// *********************

// Types
// *****

        public: typedef Go Inherit;


// Attributes
// **********

        private: Net* _net;
        private: Hook* _hook;
        private: unsigned _count;
        private: Box _boundingBox;
        private: Rubber* _nextOfNetRubberSet;

// Constructors
// ************

        protected: Rubber(Net* net, Hook* hook);

// Destructor
// **********

        public: virtual void destroy();

// Accessors
// *********

        public: virtual Cell* getCell() const;
        public: Net* getNet() const {return _net;};
        public: Hook* getHook() const {return _hook;};
        public: unsigned getCount() const {return _count;};
        public: Point getCenter() const;
        public: Point getBarycenter() const;
        public: virtual Box getBoundingBox() const;
        public: Hooks getHooks() const;

// Updators
// ********

        public: virtual void materialize();
        public: virtual void unmaterialize();
        public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);
        public: virtual void invalidate(bool propagateFlag = true);

// Others
// ******

        public: static Rubber* _create(Hook* hook);
        protected: virtual void _postCreate();

        public: void _destroy();
        protected: virtual void _preDestroy();

        public: virtual string _getTypeName() const {return _TName("Rubber");};
        public: virtual string _getString() const;
        public: virtual Record* _getRecord() const;
        public: Rubber* _getNextOfNetRubberSet() const {return _nextOfNetRubberSet;};

        public: void _setNet(Net* net);
        public: void _setHook(Hook* hook);
        public: void _setNextOfNetRubberSet(Rubber* rubber) {_nextOfNetRubberSet = rubber;};

        public: void _capture();
        public: void _release();

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Rubber)

#endif // HURRICANE_RUBBER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
