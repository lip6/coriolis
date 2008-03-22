// ****************************************************************************************************
// File: QuadTree.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "QuadTree.h"
#include "Go.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Definitions
// ****************************************************************************************************

#define QUAD_TREE_IMPLODE_THRESHOLD 80
#define QUAD_TREE_EXPLODE_THRESHOLD 100



// ****************************************************************************************************
// QuadTree_Gos declaration
// ****************************************************************************************************

class QuadTree_Gos : public Collection<Go*> {
// ****************************************

// Types
// *****

    public: typedef Collection<Go*> Inherit;

    public: class Locator : public Hurricane::Locator<Go*> {
    // ***************************************************

        public: typedef Hurricane::Locator<Go*> Inherit;

        private: const QuadTree* _quadTree;
        private: QuadTree* _currentQuadTree;
        private: GoLocator _goLocator;

        public: Locator(const QuadTree* quadTree = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Go* getElement() const;
        public: virtual Hurricane::Locator<Go*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const QuadTree* _quadTree;

// Constructors
// ************

    public: QuadTree_Gos(const QuadTree* quadTree = NULL);
    public: QuadTree_Gos(const QuadTree_Gos& gos);

// Operators
// *********

    public: QuadTree_Gos& operator=(const QuadTree_Gos& gos);

// Accessors
// *********

    public: virtual Collection<Go*>* getClone() const;
    public: virtual Hurricane::Locator<Go*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// QuadTree_GosUnder declaration
// ****************************************************************************************************

class QuadTree_GosUnder : public Collection<Go*> {
// *********************************************

// Types
// *****

    public: typedef Collection<Go*> Inherit;

    public: class Locator : public Hurricane::Locator<Go*> {
    // ***************************************************

        public: typedef Hurricane::Locator<Go*> Inherit;

        private: const QuadTree* _quadTree;
        private: Box _area;
        private: QuadTree* _currentQuadTree;
        private: GoLocator _goLocator;

        public: Locator();
        public: Locator(const QuadTree* quadTree, const Box& area);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Go* getElement() const;
        public: virtual Hurricane::Locator<Go*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const QuadTree* _quadTree;
    private: Box _area;

// Constructors
// ************

    public: QuadTree_GosUnder();
    public: QuadTree_GosUnder(const QuadTree* quadTree, const Box& area);
    public: QuadTree_GosUnder(const QuadTree_GosUnder& gos);

// Operators
// *********

    public: QuadTree_GosUnder& operator=(const QuadTree_GosUnder& gos);

// Accessors
// *********

    public: virtual Collection<Go*>* getClone() const;
    public: virtual Hurricane::Locator<Go*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// QuadTree declaration
// ****************************************************************************************************

QuadTree::QuadTree()
// *****************
:    _parent(NULL),
    _x(0),
    _y(0),
    _boundingBox(),
    _size(0),
    _goSet(),
    _ulChild(NULL),
    _urChild(NULL),
    _llChild(NULL),
    _lrChild(NULL)
{
}

QuadTree::QuadTree(QuadTree* parent)
// *********************************
:    _parent(parent),
    _x(0),
    _y(0),
    _boundingBox(),
    _size(0),
    _goSet(),
    _ulChild(NULL),
    _urChild(NULL),
    _llChild(NULL),
    _lrChild(NULL)
{
}

QuadTree::~QuadTree()
// ******************
{
    if (_ulChild) delete _ulChild;
    if (_urChild) delete _urChild;
    if (_llChild) delete _llChild;
    if (_lrChild) delete _lrChild;
}

const Box& QuadTree::getBoundingBox() const
// ****************************************
{
    if (_boundingBox.isEmpty()) {
        Box& boundingBox = ((QuadTree*)this)->_boundingBox;
        if (_ulChild) boundingBox.merge(_ulChild->getBoundingBox());
        if (_urChild) boundingBox.merge(_urChild->getBoundingBox());
        if (_llChild) boundingBox.merge(_llChild->getBoundingBox());
        if (_lrChild) boundingBox.merge(_lrChild->getBoundingBox());
        for_each_go(go, _goSet.getElements()) {
            boundingBox.merge(go->getBoundingBox());
            end_for;
        }
    }
    return _boundingBox;
}

Gos QuadTree::getGos() const
// *************************
{
    return QuadTree_Gos(this);
}

Gos QuadTree::getGosUnder(const Box& area) const
// *********************************************
{
    return QuadTree_GosUnder(this, area);
}

void QuadTree::Insert(Go* go)
// **************************
{
    if (!go)
        throw Error("Can't insert go : null go");

    if (!go->isMaterialized()) {
        Box boundingBox = go->getBoundingBox();
        QuadTree* child = _getDeepestChild(boundingBox);
        child->_goSet._Insert(go);
        go->_quadTree = child;
        QuadTree* parent = child;
        while (parent) {
            parent->_size++;
            if (parent->IsEmpty() || !parent->_boundingBox.isEmpty())
                parent->_boundingBox.merge(boundingBox);
            parent = parent->_parent;
        }
        if (QUAD_TREE_EXPLODE_THRESHOLD <= child->_size)
            child->_Explode();
    }
}

void QuadTree::Remove(Go* go)
// **************************
{
    if (!go)
        throw Error("Can't remove go : null go");

    if (go->isMaterialized()) {
        Box boundingBox = go->getBoundingBox();
        QuadTree* child = go->_quadTree;
        child->_goSet._Remove(go);
        go->_quadTree = NULL;
        QuadTree* parent = child;
        while (parent) {
            parent->_size--;
            if (parent->_boundingBox.isConstrainedBy(boundingBox))
                parent->_boundingBox = Box();
            parent = parent->_parent;
        }
        parent = child;
        while (parent) {
            if (!(parent->_size <= QUAD_TREE_IMPLODE_THRESHOLD))
                break;
            parent->_Implode();
            parent = parent->_parent;
        }
    }
}

string QuadTree::_getString() const
// ********************************
{
    string s = "<" + _TName("QuadTree");
    if (!_size)
        s += " empty";
    else
        s += " " + getString(_size);
    s += ">";
    return s;
}

Record* QuadTree::_getRecord() const
// ***************************
{
    Record* record = NULL;
    if (_size) {
        record = new Record(getString(this));
        record->Add(getSlot("Parent", _parent));
        record->Add(getSlot("X", &_x));
        record->Add(getSlot("Y", &_y));
        record->Add(getSlot("BoundingBox", &_boundingBox));
        record->Add(getSlot("Size", &_size));
        record->Add(getSlot("Gos", &_goSet));
        record->Add(getSlot("ULChild", _ulChild));
        record->Add(getSlot("URChild", _urChild));
        record->Add(getSlot("LLChild", _llChild));
        record->Add(getSlot("LRChild", _lrChild));
    }
    return record;
}

QuadTree* QuadTree::_getDeepestChild(const Box& box)
// ************************************************
{
    if (_HasBeenExploded()) {
        if (box.getXMax() < _x) {
            if (box.getYMax() < _y)
                return _llChild->_getDeepestChild(box);
            if (_y < box.getYMin())
                return _ulChild->_getDeepestChild(box);
        }
        if (_x < box.getXMin()) {
            if (box.getYMax() < _y)
                return _lrChild->_getDeepestChild(box);
            if (_y < box.getYMin())
                return _urChild->_getDeepestChild(box);
        }
    }
    return this;
}

QuadTree* QuadTree::_getFirstQuadTree() const
// ******************************************
{
    if (!_goSet.IsEmpty()) return (QuadTree*)this;
    QuadTree* quadTree = NULL;
    if (_HasBeenExploded()) {
        if (!quadTree) quadTree = _ulChild->_getFirstQuadTree();
        if (!quadTree) quadTree = _urChild->_getFirstQuadTree();
        if (!quadTree) quadTree = _llChild->_getFirstQuadTree();
        if (!quadTree) quadTree = _lrChild->_getFirstQuadTree();
    }
    return quadTree;
}

QuadTree* QuadTree::_getFirstQuadTree(const Box& area) const
// *********************************************************
{
    if (getBoundingBox().intersect(area)) {
        if (!_goSet.IsEmpty()) return (QuadTree*)this;
        QuadTree* quadTree = NULL;
        if (_HasBeenExploded()) {
            if (!quadTree) quadTree = _ulChild->_getFirstQuadTree(area);
            if (!quadTree) quadTree = _urChild->_getFirstQuadTree(area);
            if (!quadTree) quadTree = _llChild->_getFirstQuadTree(area);
            if (!quadTree) quadTree = _lrChild->_getFirstQuadTree(area);
        }
        return quadTree;
    }
    return NULL;
}

QuadTree* QuadTree::_getNextQuadTree()
// ***********************************
{
    QuadTree* nextQuadTree = NULL;
    if (_HasBeenExploded()) {
        if (!nextQuadTree) nextQuadTree = _ulChild->_getFirstQuadTree();
        if (!nextQuadTree) nextQuadTree = _urChild->_getFirstQuadTree();
        if (!nextQuadTree) nextQuadTree = _llChild->_getFirstQuadTree();
        if (!nextQuadTree) nextQuadTree = _lrChild->_getFirstQuadTree();
    }
    QuadTree* quadTree = this;
    while (!nextQuadTree && quadTree->_parent) {
        if (quadTree->_parent->_llChild == quadTree)
            nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree();
        else if (quadTree->_parent->_urChild == quadTree) {
            nextQuadTree = quadTree->_parent->_llChild->_getFirstQuadTree();
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree();
        }
        else if (quadTree->_parent->_ulChild == quadTree) {
            nextQuadTree = quadTree->_parent->_urChild->_getFirstQuadTree();
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_llChild->_getFirstQuadTree();
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree();
        }
        quadTree = quadTree->_parent;
    }
    return nextQuadTree;
}

QuadTree* QuadTree::_getNextQuadTree(const Box& area)
// **************************************************
{
    QuadTree* nextQuadTree = NULL;
    if (_HasBeenExploded()) {
        if (!nextQuadTree) nextQuadTree = _ulChild->_getFirstQuadTree(area);
        if (!nextQuadTree) nextQuadTree = _urChild->_getFirstQuadTree(area);
        if (!nextQuadTree) nextQuadTree = _llChild->_getFirstQuadTree(area);
        if (!nextQuadTree) nextQuadTree = _lrChild->_getFirstQuadTree(area);
    }
    QuadTree* quadTree = this;
    while (!nextQuadTree && quadTree->_parent) {
        if (quadTree->_parent->_llChild == quadTree)
            nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree(area);
        else if (quadTree->_parent->_urChild == quadTree) {
            nextQuadTree = quadTree->_parent->_llChild->_getFirstQuadTree(area);
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree(area);
        }
        else if (quadTree->_parent->_ulChild == quadTree) {
            nextQuadTree = quadTree->_parent->_urChild->_getFirstQuadTree(area);
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_llChild->_getFirstQuadTree(area);
            if (!nextQuadTree)
                nextQuadTree = quadTree->_parent->_lrChild->_getFirstQuadTree(area);
        }
        quadTree = quadTree->_parent;
    }
    return nextQuadTree;
}

void QuadTree::_Explode()
// **********************
{
    if (!_HasBeenExploded()) {
        _x = getBoundingBox().getXCenter();
        _y = getBoundingBox().getYCenter();
        _ulChild = new QuadTree(this);
        _urChild = new QuadTree(this);
        _llChild = new QuadTree(this);
        _lrChild = new QuadTree(this);
        set<Go*> goSet;
        for_each_go(go, _goSet.getElements()) {
            _goSet._Remove(go);
            go->_quadTree = NULL;
            goSet.insert(go);
            end_for;
        }
        for_each_go(go, getCollection(goSet)) {
            QuadTree* child = _getDeepestChild(go->getBoundingBox());
            child->_goSet._Insert(go);
            go->_quadTree = child;
            if (child != this) child->_size++;
            end_for;
        }
    }
}

void QuadTree::_Implode()
// **********************
{
    if (_HasBeenExploded()) {
        if (_ulChild->_HasBeenExploded()) _ulChild->_Implode();
        for_each_go(go, _ulChild->_goSet.getElements()) {
            _ulChild->_goSet._Remove(go);
            _goSet._Insert(go);
            go->_quadTree = this;
            end_for;
        }
        delete _ulChild;
        _ulChild = NULL;
        if (_urChild->_HasBeenExploded()) _urChild->_Implode();
        for_each_go(go, _urChild->_goSet.getElements()) {
            _urChild->_goSet._Remove(go);
            _goSet._Insert(go);
            go->_quadTree = this;
            end_for;
        }
        delete _urChild;
        _urChild = NULL;
        if (_llChild->_HasBeenExploded()) _llChild->_Implode();
        for_each_go(go, _llChild->_goSet.getElements()) {
            _llChild->_goSet._Remove(go);
            _goSet._Insert(go);
            go->_quadTree = this;
            end_for;
        }
        delete _llChild;
        _llChild = NULL;
        if (_lrChild->_HasBeenExploded()) _lrChild->_Implode();
        for_each_go(go, _lrChild->_goSet.getElements()) {
            _lrChild->_goSet._Remove(go);
            _goSet._Insert(go);
            go->_quadTree = this;
            end_for;
        }
        delete _lrChild;
        _lrChild = NULL;
    }
}



// ****************************************************************************************************
// QuadTree::GoSet implementation
// ****************************************************************************************************

QuadTree::GoSet::GoSet()
// *********************
:    Inherit()
{
}

unsigned QuadTree::GoSet::_getHashValue(Go* go) const
// **************************************************
{
    return ( (unsigned int)( (unsigned long)go ) ) / 8;
}

Go* QuadTree::GoSet::_getNextElement(Go* go) const
// ***********************************************
{
    return go->_getNextOfQuadTreeGoSet();
}

void QuadTree::GoSet::_setNextElement(Go* go, Go* nextGo) const
// ************************************************************
{
    go->_setNextOfQuadTreeGoSet(nextGo);
}



// ****************************************************************************************************
// QuadTree_Gos implementation
// ****************************************************************************************************

QuadTree_Gos::QuadTree_Gos(const QuadTree* quadTree)
// *************************************************
:     Inherit(),
    _quadTree(quadTree)
{
}

QuadTree_Gos::QuadTree_Gos(const QuadTree_Gos& gos)
// ************************************************
:     Inherit(),
    _quadTree(gos._quadTree)
{
}

QuadTree_Gos& QuadTree_Gos::operator=(const QuadTree_Gos& gos)
// ***********************************************************
{
    _quadTree = gos._quadTree;
    return *this;
}

Collection<Go*>* QuadTree_Gos::getClone() const
// ********************************************
{
    return new QuadTree_Gos(*this);
}

Locator<Go*>* QuadTree_Gos::getLocator() const
// *******************************************
{
    return new Locator(_quadTree);
}

string QuadTree_Gos::_getString() const
// ************************************
{
    string s = "<" + _TName("QuadTree::Gos");
    if (_quadTree) s += " " + getString(_quadTree);
    s += ">";
    return s;
}



// ****************************************************************************************************
// QuadTree_Gos::Locator implementation
// ****************************************************************************************************

QuadTree_Gos::Locator::Locator(const QuadTree* quadTree)
// *****************************************************
:    Inherit(),
    _quadTree(quadTree),
    _currentQuadTree(NULL),
    _goLocator()
{
    if (_quadTree) {
        _currentQuadTree = _quadTree->_getFirstQuadTree();
        if (_currentQuadTree)
            _goLocator = _currentQuadTree->_getGoSet().getElements().getLocator();
    }
}

QuadTree_Gos::Locator::Locator(const Locator& locator)
// ***************************************************
:    Inherit(),
    _quadTree(locator._quadTree),
    _currentQuadTree(locator._currentQuadTree),
    _goLocator(locator._goLocator)
{
}

QuadTree_Gos::Locator& QuadTree_Gos::Locator::operator=(const Locator& locator)
// ****************************************************************************
{
    _quadTree = locator._quadTree;
    _currentQuadTree = locator._currentQuadTree;
    _goLocator = locator._goLocator;
    return *this;
}

Go* QuadTree_Gos::Locator::getElement() const
// ******************************************
{
    return _goLocator.getElement();
}

Locator<Go*>* QuadTree_Gos::Locator::getClone() const
// **************************************************
{
    return new Locator(*this);
}

bool QuadTree_Gos::Locator::isValid() const
// ****************************************
{
    return _goLocator.isValid();
}

void QuadTree_Gos::Locator::progress()
// ***********************************
{
    if (isValid()) {
        _goLocator.progress();
        if (!_goLocator.isValid()) {
            _currentQuadTree = _currentQuadTree->_getNextQuadTree();
            if (_currentQuadTree)
                _goLocator = _currentQuadTree->_getGoSet().getElements().getLocator();
        }
    }
}

string QuadTree_Gos::Locator::_getString() const
// *********************************************
{
    string s = "<" + _TName("QuadTree::Gos::Locator");
    if (_quadTree) s += " " + getString(_quadTree);
    s += ">";
    return s;
}



// ****************************************************************************************************
// QuadTree_GosUnder implementation
// ****************************************************************************************************

QuadTree_GosUnder::QuadTree_GosUnder()
// ***********************************
:     Inherit(),
    _quadTree(NULL),
    _area()
{
}

QuadTree_GosUnder::QuadTree_GosUnder(const QuadTree* quadTree, const Box& area)
// ****************************************************************************
:     Inherit(),
    _quadTree(quadTree),
    _area(area)
{
}

QuadTree_GosUnder::QuadTree_GosUnder(const QuadTree_GosUnder& gos)
// ***************************************************************
:     Inherit(),
    _quadTree(gos._quadTree),
    _area(gos._area)
{
}

QuadTree_GosUnder& QuadTree_GosUnder::operator=(const QuadTree_GosUnder& gos)
// **************************************************************************
{
    _quadTree = gos._quadTree;
    _area = gos._area;
    return *this;
}

Collection<Go*>* QuadTree_GosUnder::getClone() const
// *************************************************
{
    return new QuadTree_GosUnder(*this);
}

Locator<Go*>* QuadTree_GosUnder::getLocator() const
// ************************************************
{
    return new Locator(_quadTree, _area);
}

string QuadTree_GosUnder::_getString() const
// *****************************************
{
    string s = "<" + _TName("QuadTree::GosUnder");
    if (_quadTree) {
        s += " " + getString(_quadTree);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// QuadTree_GosUnder::Locator implementation
// ****************************************************************************************************

QuadTree_GosUnder::Locator::Locator()
// **********************************
:    Inherit(),
    _quadTree(NULL),
    _area(),
    _currentQuadTree(NULL),
    _goLocator()
{
}

QuadTree_GosUnder::Locator::Locator(const QuadTree* quadTree, const Box& area)
// ***************************************************************************
:    Inherit(),
    _quadTree(quadTree),
    _area(area),
    _currentQuadTree(NULL),
    _goLocator()
{
    if (_quadTree && !_area.isEmpty()) {
        _currentQuadTree = _quadTree->_getFirstQuadTree(_area);
        if (_currentQuadTree) {
            _goLocator = _currentQuadTree->_getGoSet().getElements().getLocator();
            if (!getElement()->getBoundingBox().intersect(_area)) progress();
        }
    }
}

QuadTree_GosUnder::Locator::Locator(const Locator& locator)
// ********************************************************
:    Inherit(),
    _quadTree(locator._quadTree),
    _area(locator._area),
    _currentQuadTree(locator._currentQuadTree),
    _goLocator(locator._goLocator)
{
}

QuadTree_GosUnder::Locator& QuadTree_GosUnder::Locator::operator=(const Locator& locator)
// **************************************************************************************
{
    _quadTree = locator._quadTree;
    _area = locator._area;
    _currentQuadTree = locator._currentQuadTree;
    _goLocator = locator._goLocator;
    return *this;
}

Go* QuadTree_GosUnder::Locator::getElement() const
// ***********************************************
{
    return _goLocator.getElement();
}

Locator<Go*>* QuadTree_GosUnder::Locator::getClone() const
// *******************************************************
{
    return new Locator(*this);
}

bool QuadTree_GosUnder::Locator::isValid() const
// *********************************************
{
    return _goLocator.isValid();
}

void QuadTree_GosUnder::Locator::progress()
// ****************************************
{
    if (isValid()) {
        do {
            _goLocator.progress();
            if (!_goLocator.isValid()) {
                _currentQuadTree = _currentQuadTree->_getNextQuadTree(_area);
                if (_currentQuadTree)
                    _goLocator = _currentQuadTree->_getGoSet().getElements().getLocator();
            }
        } while (isValid() && !getElement()->getBoundingBox().intersect(_area));
    }
}

string QuadTree_GosUnder::Locator::_getString() const
// **************************************************
{
    string s = "<" + _TName("QuadTree::GosUnder::Locator");
    if (_quadTree) {
        s += " " + getString(_quadTree);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
