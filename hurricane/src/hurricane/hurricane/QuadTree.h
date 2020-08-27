// ****************************************************************************************************
// File: ./hurricane/QuadTree.h
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
#include "hurricane/Box.h"
#include "hurricane/Gos.h"
#include "hurricane/IntrusiveSet.h"

namespace Hurricane {



// ****************************************************************************************************
// QuadTree declaration
// ****************************************************************************************************

class QuadTree {
// Types
// *****

    public: class GoSet : public IntrusiveSet<Go> {
    // *******************************************

        public: typedef IntrusiveSet<Go> Inherit;
    
        public: GoSet();

        public: virtual unsigned _getHashValue(Go* go) const;
        public: virtual Go* _getNextElement(Go* go) const;
        public: virtual void _setNextElement(Go* go, Go* nextGo) const;

    };

// Attributes
// **********

    private: QuadTree* _parent;
    private: DbU::Unit _x;
    private: DbU::Unit _y;
    private: Box _boundingBox;
    private: unsigned _size;
    private: GoSet _goSet;
    private: QuadTree* _ulChild; // Upper Left Child
    private: QuadTree* _urChild; // Upper Right Child
    private: QuadTree* _llChild; // Lower Left Child
    private: QuadTree* _lrChild; // Lower Right Child

// Constructors
// ************

    public: QuadTree();

    private: QuadTree(QuadTree* parent);

    private: QuadTree(const QuadTree& quadTree); // not implemented to forbid copy construction

// Destructor
// **********

    public: ~QuadTree();

// Operators
// *********

    private: QuadTree& operator=(const QuadTree& quadTree); // not implemented to forbid assignment

// Accessors
// *********

  //public: static size_t getLocatorAllocateds ();
    public: const Box& getBoundingBox() const;
    public: Gos getGos() const;
    public: Gos getGosUnder(const Box& area, DbU::Unit threshold=0) const;

// Predicates
// **********

    public: bool isEmpty() const {return (_size == 0);};

// Updators
// ********

    public: void insert(Go* go);
    public: void remove(Go* go);

// Others
// ******

    public: string _getTypeName() const { return _TName("QuadTree"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    public: GoSet& _getGoSet() {return _goSet;};
    public: QuadTree* _getDeepestChild(const Box& box);
    public: QuadTree* _getFirstQuadTree() const;
    public: QuadTree* _getFirstQuadTree(const Box& area) const;
    public: QuadTree* _getNextQuadTree();
    public: QuadTree* _getNextQuadTree(const Box& area);

    public: bool _hasBeenExploded() const {return (_ulChild != NULL);};

    public: void _explode();
    public: void _implode();

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::QuadTree);
INSPECTOR_P_SUPPORT(Hurricane::QuadTree::GoSet);


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
