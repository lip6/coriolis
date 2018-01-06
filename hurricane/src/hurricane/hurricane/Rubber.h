// ****************************************************************************************************
// File: ./hurricane/Rubber.h
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

#ifndef HURRICANE_RUBBER_H
#define HURRICANE_RUBBER_H

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


INSPECTOR_P_SUPPORT(Hurricane::Rubber);


#endif // HURRICANE_RUBBER_H


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
