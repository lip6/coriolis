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

		public: virtual Go* GetElement() const;
		public: virtual Hurricane::Locator<Go*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Go*>* GetClone() const;
	public: virtual Hurricane::Locator<Go*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

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

		public: virtual Go* GetElement() const;
		public: virtual Hurricane::Locator<Go*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Go*>* GetClone() const;
	public: virtual Hurricane::Locator<Go*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// QuadTree declaration
// ****************************************************************************************************

QuadTree::QuadTree()
// *****************
:	_parent(NULL),
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
:	_parent(parent),
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

const Box& QuadTree::GetBoundingBox() const
// ****************************************
{
	if (_boundingBox.IsEmpty()) {
		Box& boundingBox = ((QuadTree*)this)->_boundingBox;
		if (_ulChild) boundingBox.Merge(_ulChild->GetBoundingBox());
		if (_urChild) boundingBox.Merge(_urChild->GetBoundingBox());
		if (_llChild) boundingBox.Merge(_llChild->GetBoundingBox());
		if (_lrChild) boundingBox.Merge(_lrChild->GetBoundingBox());
		for_each_go(go, _goSet.GetElements()) {
			boundingBox.Merge(go->GetBoundingBox());
			end_for;
		}
	}
	return _boundingBox;
}

Gos QuadTree::GetGos() const
// *************************
{
	return QuadTree_Gos(this);
}

Gos QuadTree::GetGosUnder(const Box& area) const
// *********************************************
{
	return QuadTree_GosUnder(this, area);
}

void QuadTree::Insert(Go* go)
// **************************
{
	if (!go)
		throw Error("Can't insert go : null go");

	if (!go->IsMaterialized()) {
		Box boundingBox = go->GetBoundingBox();
		QuadTree* child = _GetDeepestChild(boundingBox);
		child->_goSet._Insert(go);
		go->_quadTree = child;
		QuadTree* parent = child;
		while (parent) {
			parent->_size++;
			if (parent->IsEmpty() || !parent->_boundingBox.IsEmpty())
				parent->_boundingBox.Merge(boundingBox);
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

	if (go->IsMaterialized()) {
		Box boundingBox = go->GetBoundingBox();
		QuadTree* child = go->_quadTree;
		child->_goSet._Remove(go);
		go->_quadTree = NULL;
		QuadTree* parent = child;
		while (parent) {
			parent->_size--;
			if (parent->_boundingBox.IsConstrainedBy(boundingBox))
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

string QuadTree::_GetString() const
// ********************************
{
	string s = "<" + _TName("QuadTree");
	if (!_size)
		s += " empty";
	else
		s += " " + GetString(_size);
	s += ">";
	return s;
}

Record* QuadTree::_GetRecord() const
// ***************************
{
	Record* record = NULL;
	if (_size) {
		record = new Record(GetString(this));
		record->Add(GetSlot("Parent", _parent));
		record->Add(GetSlot("X", &_x));
		record->Add(GetSlot("Y", &_y));
		record->Add(GetSlot("BoundingBox", &_boundingBox));
		record->Add(GetSlot("Size", &_size));
		record->Add(GetSlot("Gos", &_goSet));
		record->Add(GetSlot("ULChild", _ulChild));
		record->Add(GetSlot("URChild", _urChild));
		record->Add(GetSlot("LLChild", _llChild));
		record->Add(GetSlot("LRChild", _lrChild));
	}
	return record;
}

QuadTree* QuadTree::_GetDeepestChild(const Box& box)
// ************************************************
{
	if (_HasBeenExploded()) {
		if (box.GetXMax() < _x) {
			if (box.GetYMax() < _y)
				return _llChild->_GetDeepestChild(box);
			if (_y < box.GetYMin())
				return _ulChild->_GetDeepestChild(box);
		}
		if (_x < box.GetXMin()) {
			if (box.GetYMax() < _y)
				return _lrChild->_GetDeepestChild(box);
			if (_y < box.GetYMin())
				return _urChild->_GetDeepestChild(box);
		}
	}
	return this;
}

QuadTree* QuadTree::_GetFirstQuadTree() const
// ******************************************
{
	if (!_goSet.IsEmpty()) return (QuadTree*)this;
	QuadTree* quadTree = NULL;
	if (_HasBeenExploded()) {
		if (!quadTree) quadTree = _ulChild->_GetFirstQuadTree();
		if (!quadTree) quadTree = _urChild->_GetFirstQuadTree();
		if (!quadTree) quadTree = _llChild->_GetFirstQuadTree();
		if (!quadTree) quadTree = _lrChild->_GetFirstQuadTree();
	}
	return quadTree;
}

QuadTree* QuadTree::_GetFirstQuadTree(const Box& area) const
// *********************************************************
{
	if (GetBoundingBox().Intersect(area)) {
		if (!_goSet.IsEmpty()) return (QuadTree*)this;
		QuadTree* quadTree = NULL;
		if (_HasBeenExploded()) {
			if (!quadTree) quadTree = _ulChild->_GetFirstQuadTree(area);
			if (!quadTree) quadTree = _urChild->_GetFirstQuadTree(area);
			if (!quadTree) quadTree = _llChild->_GetFirstQuadTree(area);
			if (!quadTree) quadTree = _lrChild->_GetFirstQuadTree(area);
		}
		return quadTree;
	}
	return NULL;
}

QuadTree* QuadTree::_GetNextQuadTree()
// ***********************************
{
	QuadTree* nextQuadTree = NULL;
	if (_HasBeenExploded()) {
		if (!nextQuadTree) nextQuadTree = _ulChild->_GetFirstQuadTree();
		if (!nextQuadTree) nextQuadTree = _urChild->_GetFirstQuadTree();
		if (!nextQuadTree) nextQuadTree = _llChild->_GetFirstQuadTree();
		if (!nextQuadTree) nextQuadTree = _lrChild->_GetFirstQuadTree();
	}
	QuadTree* quadTree = this;
	while (!nextQuadTree && quadTree->_parent) {
		if (quadTree->_parent->_llChild == quadTree)
			nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree();
		else if (quadTree->_parent->_urChild == quadTree) {
			nextQuadTree = quadTree->_parent->_llChild->_GetFirstQuadTree();
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree();
		}
		else if (quadTree->_parent->_ulChild == quadTree) {
			nextQuadTree = quadTree->_parent->_urChild->_GetFirstQuadTree();
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_llChild->_GetFirstQuadTree();
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree();
		}
		quadTree = quadTree->_parent;
	}
	return nextQuadTree;
}

QuadTree* QuadTree::_GetNextQuadTree(const Box& area)
// **************************************************
{
	QuadTree* nextQuadTree = NULL;
	if (_HasBeenExploded()) {
		if (!nextQuadTree) nextQuadTree = _ulChild->_GetFirstQuadTree(area);
		if (!nextQuadTree) nextQuadTree = _urChild->_GetFirstQuadTree(area);
		if (!nextQuadTree) nextQuadTree = _llChild->_GetFirstQuadTree(area);
		if (!nextQuadTree) nextQuadTree = _lrChild->_GetFirstQuadTree(area);
	}
	QuadTree* quadTree = this;
	while (!nextQuadTree && quadTree->_parent) {
		if (quadTree->_parent->_llChild == quadTree)
			nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree(area);
		else if (quadTree->_parent->_urChild == quadTree) {
			nextQuadTree = quadTree->_parent->_llChild->_GetFirstQuadTree(area);
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree(area);
		}
		else if (quadTree->_parent->_ulChild == quadTree) {
			nextQuadTree = quadTree->_parent->_urChild->_GetFirstQuadTree(area);
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_llChild->_GetFirstQuadTree(area);
			if (!nextQuadTree)
				nextQuadTree = quadTree->_parent->_lrChild->_GetFirstQuadTree(area);
		}
		quadTree = quadTree->_parent;
	}
	return nextQuadTree;
}

void QuadTree::_Explode()
// **********************
{
	if (!_HasBeenExploded()) {
		_x = GetBoundingBox().GetXCenter();
		_y = GetBoundingBox().GetYCenter();
		_ulChild = new QuadTree(this);
		_urChild = new QuadTree(this);
		_llChild = new QuadTree(this);
		_lrChild = new QuadTree(this);
		set<Go*> goSet;
		for_each_go(go, _goSet.GetElements()) {
			_goSet._Remove(go);
			go->_quadTree = NULL;
			goSet.insert(go);
			end_for;
		}
		for_each_go(go, GetCollection(goSet)) {
			QuadTree* child = _GetDeepestChild(go->GetBoundingBox());
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
		for_each_go(go, _ulChild->_goSet.GetElements()) {
			_ulChild->_goSet._Remove(go);
			_goSet._Insert(go);
			go->_quadTree = this;
			end_for;
		}
		delete _ulChild;
		_ulChild = NULL;
		if (_urChild->_HasBeenExploded()) _urChild->_Implode();
		for_each_go(go, _urChild->_goSet.GetElements()) {
			_urChild->_goSet._Remove(go);
			_goSet._Insert(go);
			go->_quadTree = this;
			end_for;
		}
		delete _urChild;
		_urChild = NULL;
		if (_llChild->_HasBeenExploded()) _llChild->_Implode();
		for_each_go(go, _llChild->_goSet.GetElements()) {
			_llChild->_goSet._Remove(go);
			_goSet._Insert(go);
			go->_quadTree = this;
			end_for;
		}
		delete _llChild;
		_llChild = NULL;
		if (_lrChild->_HasBeenExploded()) _lrChild->_Implode();
		for_each_go(go, _lrChild->_goSet.GetElements()) {
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
:	Inherit()
{
}

unsigned QuadTree::GoSet::_GetHashValue(Go* go) const
// **************************************************
{
	return ( (unsigned int)( (unsigned long)go ) ) / 8;
}

Go* QuadTree::GoSet::_GetNextElement(Go* go) const
// ***********************************************
{
	return go->_GetNextOfQuadTreeGoSet();
}

void QuadTree::GoSet::_SetNextElement(Go* go, Go* nextGo) const
// ************************************************************
{
	go->_SetNextOfQuadTreeGoSet(nextGo);
}



// ****************************************************************************************************
// QuadTree_Gos implementation
// ****************************************************************************************************

QuadTree_Gos::QuadTree_Gos(const QuadTree* quadTree)
// *************************************************
: 	Inherit(),
	_quadTree(quadTree)
{
}

QuadTree_Gos::QuadTree_Gos(const QuadTree_Gos& gos)
// ************************************************
: 	Inherit(),
	_quadTree(gos._quadTree)
{
}

QuadTree_Gos& QuadTree_Gos::operator=(const QuadTree_Gos& gos)
// ***********************************************************
{
	_quadTree = gos._quadTree;
	return *this;
}

Collection<Go*>* QuadTree_Gos::GetClone() const
// ********************************************
{
	return new QuadTree_Gos(*this);
}

Locator<Go*>* QuadTree_Gos::GetLocator() const
// *******************************************
{
	return new Locator(_quadTree);
}

string QuadTree_Gos::_GetString() const
// ************************************
{
	string s = "<" + _TName("QuadTree::Gos");
	if (_quadTree) s += " " + GetString(_quadTree);
	s += ">";
	return s;
}



// ****************************************************************************************************
// QuadTree_Gos::Locator implementation
// ****************************************************************************************************

QuadTree_Gos::Locator::Locator(const QuadTree* quadTree)
// *****************************************************
:	Inherit(),
	_quadTree(quadTree),
	_currentQuadTree(NULL),
	_goLocator()
{
	if (_quadTree) {
		_currentQuadTree = _quadTree->_GetFirstQuadTree();
		if (_currentQuadTree)
			_goLocator = _currentQuadTree->_GetGoSet().GetElements().GetLocator();
	}
}

QuadTree_Gos::Locator::Locator(const Locator& locator)
// ***************************************************
:	Inherit(),
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

Go* QuadTree_Gos::Locator::GetElement() const
// ******************************************
{
	return _goLocator.GetElement();
}

Locator<Go*>* QuadTree_Gos::Locator::GetClone() const
// **************************************************
{
	return new Locator(*this);
}

bool QuadTree_Gos::Locator::IsValid() const
// ****************************************
{
	return _goLocator.IsValid();
}

void QuadTree_Gos::Locator::Progress()
// ***********************************
{
	if (IsValid()) {
		_goLocator.Progress();
		if (!_goLocator.IsValid()) {
			_currentQuadTree = _currentQuadTree->_GetNextQuadTree();
			if (_currentQuadTree)
				_goLocator = _currentQuadTree->_GetGoSet().GetElements().GetLocator();
		}
	}
}

string QuadTree_Gos::Locator::_GetString() const
// *********************************************
{
	string s = "<" + _TName("QuadTree::Gos::Locator");
	if (_quadTree) s += " " + GetString(_quadTree);
	s += ">";
	return s;
}



// ****************************************************************************************************
// QuadTree_GosUnder implementation
// ****************************************************************************************************

QuadTree_GosUnder::QuadTree_GosUnder()
// ***********************************
: 	Inherit(),
	_quadTree(NULL),
	_area()
{
}

QuadTree_GosUnder::QuadTree_GosUnder(const QuadTree* quadTree, const Box& area)
// ****************************************************************************
: 	Inherit(),
	_quadTree(quadTree),
	_area(area)
{
}

QuadTree_GosUnder::QuadTree_GosUnder(const QuadTree_GosUnder& gos)
// ***************************************************************
: 	Inherit(),
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

Collection<Go*>* QuadTree_GosUnder::GetClone() const
// *************************************************
{
	return new QuadTree_GosUnder(*this);
}

Locator<Go*>* QuadTree_GosUnder::GetLocator() const
// ************************************************
{
	return new Locator(_quadTree, _area);
}

string QuadTree_GosUnder::_GetString() const
// *****************************************
{
	string s = "<" + _TName("QuadTree::GosUnder");
	if (_quadTree) {
		s += " " + GetString(_quadTree);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// QuadTree_GosUnder::Locator implementation
// ****************************************************************************************************

QuadTree_GosUnder::Locator::Locator()
// **********************************
:	Inherit(),
	_quadTree(NULL),
	_area(),
	_currentQuadTree(NULL),
	_goLocator()
{
}

QuadTree_GosUnder::Locator::Locator(const QuadTree* quadTree, const Box& area)
// ***************************************************************************
:	Inherit(),
	_quadTree(quadTree),
	_area(area),
	_currentQuadTree(NULL),
	_goLocator()
{
	if (_quadTree && !_area.IsEmpty()) {
		_currentQuadTree = _quadTree->_GetFirstQuadTree(_area);
		if (_currentQuadTree) {
			_goLocator = _currentQuadTree->_GetGoSet().GetElements().GetLocator();
			if (!GetElement()->GetBoundingBox().Intersect(_area)) Progress();
		}
	}
}

QuadTree_GosUnder::Locator::Locator(const Locator& locator)
// ********************************************************
:	Inherit(),
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

Go* QuadTree_GosUnder::Locator::GetElement() const
// ***********************************************
{
	return _goLocator.GetElement();
}

Locator<Go*>* QuadTree_GosUnder::Locator::GetClone() const
// *******************************************************
{
	return new Locator(*this);
}

bool QuadTree_GosUnder::Locator::IsValid() const
// *********************************************
{
	return _goLocator.IsValid();
}

void QuadTree_GosUnder::Locator::Progress()
// ****************************************
{
	if (IsValid()) {
		do {
			_goLocator.Progress();
			if (!_goLocator.IsValid()) {
				_currentQuadTree = _currentQuadTree->_GetNextQuadTree(_area);
				if (_currentQuadTree)
					_goLocator = _currentQuadTree->_GetGoSet().GetElements().GetLocator();
			}
		} while (IsValid() && !GetElement()->GetBoundingBox().Intersect(_area));
	}
}

string QuadTree_GosUnder::Locator::_GetString() const
// **************************************************
{
	string s = "<" + _TName("QuadTree::GosUnder::Locator");
	if (_quadTree) {
		s += " " + GetString(_quadTree);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
