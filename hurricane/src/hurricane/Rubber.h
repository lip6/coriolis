// ****************************************************************************************************
// File: Rubber.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_RUBBER
#define HURRICANE_RUBBER

#include "Go.h"
#include "Hooks.h"
#include "Rubbers.h"

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

        public: virtual void Delete();

// Accessors
// *********

        public: virtual Cell* GetCell() const;
        public: Net* GetNet() const {return _net;};
        public: Hook* GetHook() const {return _hook;};
        public: unsigned GetCount() const {return _count;};
        public: Point GetCenter() const;
        public: Point GetBarycenter() const;
        public: virtual Box GetBoundingBox() const;
        public: Hooks GetHooks() const;

// Updators
// ********

        public: virtual void Materialize();
        public: virtual void Unmaterialize();
        public: virtual void Translate(const Unit& dx, const Unit& dy);
        public: virtual void Invalidate(bool propagateFlag = true);

// Others
// ******

        public: static Rubber* _Create(Hook* hook);
        protected: virtual void _PostCreate();

        public: void _Delete();
        protected: virtual void _PreDelete();

        public: virtual string _GetTypeName() const {return _TName("Rubber");};
        public: virtual string _GetString() const;
        public: virtual Record* _GetRecord() const;
        public: Rubber* _GetNextOfNetRubberSet() const {return _nextOfNetRubberSet;};

        public: void _SetNet(Net* net);
        public: void _SetHook(Hook* hook);
        public: void _SetNextOfNetRubberSet(Rubber* rubber) {_nextOfNetRubberSet = rubber;};

        public: void _Capture();
        public: void _Release();

        //public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
        //public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
        //public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Rubber)

#endif // HURRICANE_RUBBER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
