// ****************************************************************************************************
// File: Region.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Region.h"
#include "Error.h"


namespace Hurricane {

// ****************************************************************************************************
// Debugging purpose
// ****************************************************************************************************
#define DEBUG_REGION 0
#if DEBUG_REGION
// to Stop and show region in graphic mode (for debugging)
void DblSupply_Stop(const Region& region, const string msg, const Box selBox, const int level);
#endif
#if DEBUG_REGION
    // Calling sequence
		string msg = "Region::GetIntersection avant Groove";
		DblSupply_Stop(region, msg, box, -10);
#endif


// ****************************************************************************************************
// Region_Tile declaration
// ****************************************************************************************************

class Region_Tile {
// **************
	
// Types
// *****
	
	public: class IsVoidFilter : public Filter<Region_Tile*> {
	// *****************************************************

		public: IsVoidFilter();

		public: IsVoidFilter(const IsVoidFilter& filter);

		public: IsVoidFilter& operator=(const IsVoidFilter& filter);

		public: virtual Filter<Region_Tile*>* GetClone() const;

		public: virtual bool Accept(Region_Tile* tile) const;

		public: virtual string _GetString() const;

	};

// Attributes
// **********
	
	public: Box _boundingBox;
	public: bool _isVoid;
	public: Region_Tile* _leftTile;
	public: Region_Tile* _bottomTile;
	public: Region_Tile* _topTile;
	public: Region_Tile* _rightTile;

// Constructors
// ************

	public: Region_Tile(const Box& boundingBox, bool isVoid = false);

	private: Region_Tile(const Region_Tile& tile); // not implemented to forbid copy

// Destructor
// **********

	public: ~Region_Tile();

// Operators
// *********

	private: Region_Tile& operator=(const Region_Tile& tile); // not implemented to forbid assignment

// Accessors
// *********

	public: const Box& GetBoundingBox() const {return _boundingBox;};

	public: const Unit& GetXMin() const {return _boundingBox.GetXMin();};
	public: const Unit& GetYMin() const {return _boundingBox.GetYMin();};
	public: const Unit& GetXMax() const {return _boundingBox.GetXMax();};
	public: const Unit& GetYMax() const {return _boundingBox.GetYMax();};

	public: Region_Tile* GetTileAt(const Point& point) const;
	public: Region_Tile* GetNonVoidTileAt(const Point& point) const;

	public: Region_Tile* GetLeftTile(const Unit& y) const;
	public: Region_Tile* GetBottomTile(const Unit& x) const;
	public: Region_Tile* GetTopTile(const Unit& x) const;
	public: Region_Tile* GetRightTile(const Unit& y) const;

// Filters
// *******

	public: static GenericFilter<Region_Tile*> GetIsVoidFilter();

// Predicates
// **********

	public: bool IsVoid() const {return _isVoid;};
	public: bool Contains(const Point& point) const;

// Updators
// ********

	public: void SplitVertical(Region* region, const Unit& x);
	public: void SplitHorizontal(Region* region, const Unit& y);
	public: bool MergeLeftTile(Region* region);
	public: bool MergeBottomTile(Region* region);
	public: bool MergeTopTile(Region* region);
	public: bool MergeRightTile(Region* region);
	public: void MergeNeighbours(Region* region);
	public: void CleanNeighbours(Region* region);
	public: bool VerticalEnhancement(Region* region);
	public: bool HorizontalEnhancement(Region* region);

// Others
// ******

    public: string _GetTypeName() const { return _TName("Region_Tile"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

	private: bool _IsTopFull() const;
	private: bool _IsBottomFull() const;
	private: bool _IsLeftFull() const;
	private: bool _IsRightFull() const;
	private: Unit _TopSplitAtVerticalSize (Region* region);
	private: Unit _BottomSplitAtVerticalSize (Region* region);
	private: Unit _LeftSplitAtHorizontalSize (Region* region);
	private: Unit _RightSplitAtHorizontalSize (Region* region);
	private: bool _TopSplitAtHorizontalSize (Region* region, const Unit height);
	private: bool _BottomSplitAtHorizontalSize (Region* region, const Unit height);
	private: bool _LeftSplitAtVerticalSize (Region* region, const Unit width);
	private: bool _RightSplitAtVerticalSize (Region* region, const Unit width);

	public: Interval _GetTopNeighbour () const;
	public: Interval _GetBottomNeighbour () const;
	public: Interval _GetLeftNeighbour () const;
	public: Interval _GetRightNeighbour () const;
};				// class Region_Tile



// ****************************************************************************************************
// Region_Tile implementation
// ****************************************************************************************************

Region_Tile::Region_Tile(const Box& boundingBox, bool isVoid)
// **********************************************************
:	_boundingBox(boundingBox),
	_isVoid(isVoid),
	_leftTile(NULL),
	_bottomTile(NULL),
	_topTile(NULL),
	_rightTile(NULL)
{
	if (boundingBox.IsEmpty())
		throw Error("Can't create " + _TName("Region::Tile") + " : empty bounding box");
}

Region_Tile::~Region_Tile()
// ************************
{
}

bool Region_Tile::Contains(const Point& point) const
// *************************************************
{
	return ((GetXMin() <= point.GetX()) &&
			  (GetYMin() <= point.GetY()) &&
			  ((point.GetX() < GetXMax()) || (!_rightTile && (point.GetX() == GetXMax()))) &&
			  ((point.GetY() < GetYMax()) || (!_topTile && (point.GetY() == GetYMax()))));
}

Region_Tile* Region_Tile::GetTileAt(const Point& point) const
// **********************************************************
{
	if (Contains(point)) return (Region_Tile*)this;

	if (_topTile && (GetYMax() <= point.GetY())) return _topTile->GetTileAt(point);

	if (_leftTile && (point.GetX() < GetXMin())) return _leftTile->GetTileAt(point);

	return NULL;
}

Region_Tile* Region_Tile::GetNonVoidTileAt(const Point& point) const
// *****************************************************************
{
	if (Contains(point)) {
		if (!_isVoid) return (Region_Tile*)this;
		if ((GetXMin() < point.GetX()) && (GetYMin() < point.GetY())) return NULL;
	}

	if (_topTile && (GetYMax() <= point.GetY())) return _topTile->GetNonVoidTileAt(point);

	if (_leftTile && (point.GetX() <= GetXMin())) return _leftTile->GetNonVoidTileAt(point);

	return NULL;
}

Region_Tile* Region_Tile::GetLeftTile(const Unit& y) const
// *******************************************************
{
	Region_Tile* tile = _leftTile;
	while (tile && (tile->GetYMax() <= y)) tile = tile->_topTile;
	return tile;
}

Region_Tile* Region_Tile::GetBottomTile(const Unit& x) const
// *********************************************************
{
	Region_Tile* tile = _bottomTile;
	while (tile && (tile->GetXMax() <= x)) tile = tile->_rightTile;
	return tile;
}

Region_Tile* Region_Tile::GetTopTile(const Unit& x) const
// ******************************************************
{
	Region_Tile* tile = _topTile;
	while (tile && (x < tile->GetXMin())) tile = tile->_leftTile;
	return tile;
}

Region_Tile* Region_Tile::GetRightTile(const Unit& y) const
// ********************************************************
{
	Region_Tile* tile = _rightTile;
	while (tile && (y < tile->GetYMin())) tile = tile->_bottomTile;
	return tile;
}

GenericFilter<Region_Tile*> Region_Tile::GetIsVoidFilter()
// *******************************************************
{
	return Region_Tile::IsVoidFilter();
}

void Region_Tile::SplitVertical(Region* region, const Unit& x)
// ***********************************************************
{
	if ((GetXMin() < x) && (x < GetXMax())) {
		Region_Tile* newTile = new Region_Tile(Box(x, GetYMin(), GetXMax(), GetYMax()), IsVoid());

		newTile->_leftTile = this;
		newTile->_rightTile = _rightTile;
		if (_rightTile) {
			Region_Tile* tile = _rightTile;
			while (tile && (tile->_leftTile == this)) {
				tile->_leftTile = newTile;
				tile = tile->_bottomTile;
			}
		}
		_rightTile = newTile;

		Region_Tile* tile = _bottomTile;
		while (tile && (tile->_topTile == this)) {
			if (x < tile->GetXMax()) tile->_topTile = newTile;
			tile = tile->_rightTile;
		}
		if (_bottomTile) {
			newTile->_bottomTile = _bottomTile;
			while (newTile->_bottomTile->GetXMax() <= newTile->GetXMin())
				newTile->_bottomTile = newTile->_bottomTile->_rightTile;
		}

		newTile->_topTile = _topTile;
		while (_topTile && (_topTile->_bottomTile == this)) {
			if (_topTile->GetXMin() < x) break;
			_topTile->_bottomTile = newTile;
			_topTile = _topTile->_leftTile;
		}

		_boundingBox = Box(GetXMin(), GetYMin(), x, GetYMax());

		if (region->_GetBottomRightTile() == this) region->_SetBottomRightTile(newTile);
	}
}

void Region_Tile::SplitHorizontal(Region* region, const Unit& y)
// *************************************************************
{
	if ((GetYMin() < y) && (y < GetYMax())) {
		Region_Tile* newTile = new Region_Tile(Box(GetXMin(), y, GetXMax(), GetYMax()), IsVoid());

		newTile->_bottomTile = this;
		newTile->_topTile = _topTile;
		if (_topTile) {
			Region_Tile* tile = _topTile;
			while (tile && (tile->_bottomTile == this)) {
				tile->_bottomTile = newTile;
				tile = tile->_leftTile;
			}
		}
		_topTile = newTile;

		Region_Tile* tile = _leftTile;
		while (tile && (tile->_rightTile == this)) {
			if (y < tile->GetYMax()) tile->_rightTile = newTile;
			tile = tile->_topTile;
		}
		if (_leftTile) {
			newTile->_leftTile = _leftTile;
			while (newTile->_leftTile->GetYMax() <= newTile->GetYMin())
				newTile->_leftTile = newTile->_leftTile->_topTile;
		}

		newTile->_rightTile = _rightTile;
		while (_rightTile && (_rightTile->_leftTile == this)) {
			if (_rightTile->GetYMin() < y) break;
			_rightTile->_leftTile = newTile;
			_rightTile = _rightTile->_bottomTile;
		}

		_boundingBox = Box(GetXMin(), GetYMin(), GetXMax(), y);

		if (region->_GetTopLeftTile() == this) region->_SetTopLeftTile(newTile);
	}
}

bool Region_Tile::MergeLeftTile(Region* region)
// ********************************************
{
	Region_Tile* uselessTile = _leftTile;

	if (!uselessTile) return false;
	if (uselessTile->_rightTile != this) return false;
	if (uselessTile->_isVoid != _isVoid) return false;
	if (uselessTile->GetXMax() != GetXMin()) return false;
	if (uselessTile->GetYMin() != GetYMin()) return false;
	if (uselessTile->GetYMax() != GetYMax()) return false;

	Region_Tile* tile = uselessTile->_topTile;
	while (tile && (tile->_bottomTile == uselessTile)) {
		tile->_bottomTile = this;
		tile = tile->_leftTile;
	}

	tile = uselessTile->_bottomTile;
	while (tile && (tile->_topTile == uselessTile)) {
		tile->_topTile = this;
		tile = tile->_rightTile;
	}

	tile = uselessTile->_leftTile;
	while (tile && (tile->_rightTile == uselessTile)) {
		tile->_rightTile = this;
		tile = tile->_topTile;
	}

	_leftTile = uselessTile->_leftTile;
	_bottomTile = uselessTile->_bottomTile;

	_boundingBox.Merge(uselessTile->_boundingBox);

	if (region->_GetTopLeftTile() == uselessTile) region->_SetTopLeftTile(this);

	delete uselessTile;

	return true;
}

bool Region_Tile::MergeBottomTile(Region* region)
// **********************************************
{
	Region_Tile* uselessTile = _bottomTile;

	if (!uselessTile) return false;
	if (uselessTile->_topTile != this) return false;
	if (uselessTile->_isVoid != _isVoid) return false;
	if (uselessTile->GetYMax() != GetYMin()) return false;
	if (uselessTile->GetXMin() != GetXMin()) return false;
	if (uselessTile->GetXMax() != GetXMax()) return false;

	Region_Tile* tile = uselessTile->_rightTile;
	while (tile && (tile->_leftTile == uselessTile)) {
		tile->_leftTile = this;
		tile = tile->_bottomTile;
	}

	tile = uselessTile->_leftTile;
	while (tile && (tile->_rightTile == uselessTile)) {
		tile->_rightTile = this;
		tile = tile->_topTile;
	}

	tile = uselessTile->_bottomTile;
	while (tile && (tile->_topTile == uselessTile)) {
		tile->_topTile = this;
		tile = tile->_rightTile;
	}

	_leftTile = uselessTile->_leftTile;
	_bottomTile = uselessTile->_bottomTile;

	_boundingBox.Merge(uselessTile->_boundingBox);

	if (region->_GetBottomRightTile() == uselessTile) region->_SetBottomRightTile(this);

	delete uselessTile;

	return true;
}

bool Region_Tile::MergeTopTile(Region* region)
// *******************************************
{
	Region_Tile* uselessTile = _topTile;

	if (!uselessTile) return false;
	if (uselessTile->_bottomTile != this) return false;
	if (uselessTile->_isVoid != _isVoid) return false;
	if (uselessTile->GetYMin() != GetYMax()) return false;
	if (uselessTile->GetXMin() != GetXMin()) return false;
	if (uselessTile->GetXMax() != GetXMax()) return false;

	Region_Tile* tile = uselessTile->_rightTile;
	while (tile && (tile->_leftTile == uselessTile)) {
		tile->_leftTile = this;
		tile = tile->_bottomTile;
	}

	tile = uselessTile->_leftTile;
	while (tile && (tile->_rightTile == uselessTile)) {
		tile->_rightTile = this;
		tile = tile->_topTile;
	}

	tile = uselessTile->_topTile;
	while (tile && (tile->_bottomTile == uselessTile)) {
		tile->_bottomTile = this;
		tile = tile->_leftTile;
	}

	_topTile = uselessTile->_topTile;
	_rightTile = uselessTile->_rightTile;

	_boundingBox.Merge(uselessTile->_boundingBox);

	if (region->_GetTopLeftTile() == uselessTile) region->_SetTopLeftTile(this);

	delete uselessTile;

	return true;
}

bool Region_Tile::MergeRightTile(Region* region)
// *********************************************
{
	Region_Tile* uselessTile = _rightTile;

	if (!uselessTile) return false;
	if (uselessTile->_leftTile != this) return false;
	if (uselessTile->_isVoid != _isVoid) return false;
	if (uselessTile->GetXMin() != GetXMax()) return false;
	if (uselessTile->GetYMin() != GetYMin()) return false;
	if (uselessTile->GetYMax() != GetYMax()) return false;

	Region_Tile* tile = uselessTile->_topTile;
	while (tile && (tile->_bottomTile == uselessTile)) {
		tile->_bottomTile = this;
		tile = tile->_leftTile;
	}

	tile = uselessTile->_bottomTile;
	while (tile && (tile->_topTile == uselessTile)) {
		tile->_topTile = this;
		tile = tile->_rightTile;
	}

	tile = uselessTile->_rightTile;
	while (tile && (tile->_leftTile == uselessTile)) {
		tile->_leftTile = this;
		tile = tile->_bottomTile;
	}

	_topTile = uselessTile->_topTile;
	_rightTile = uselessTile->_rightTile;

	_boundingBox.Merge(uselessTile->_boundingBox);

	if (region->_GetBottomRightTile() == uselessTile) region->_SetBottomRightTile(this);

	delete uselessTile;

	return true;
}

void Region_Tile::MergeNeighbours(Region* region)
// **********************************************
{
	while (MergeLeftTile(region) || MergeTopTile(region) ||
	  		 MergeBottomTile(region) || MergeRightTile(region)) {
	}
}

void Region_Tile::CleanNeighbours(Region* region)
// **********************************************
{
	Region_Tile* tile = NULL;

	set<Region_Tile*> tileSet;

	tile = _leftTile;
	while (tile && (tile->GetYMin() <= GetYMax())) {
		tileSet.insert(tile);
		tile = tile->_topTile;
	}

	tile = _topTile;
	while (tile && (GetXMin() <= tile->GetXMax())) {
		tileSet.insert(tile);
		tile = tile->_leftTile;
	}

	tile = _bottomTile;
	while (tile && (tile->GetXMin() <= GetXMax())) {
		tileSet.insert(tile);
		tile = tile->_rightTile;
	}

	tile = _rightTile;
	while (tile && (GetYMin() <= tile->GetYMax())) {
		tileSet.insert(tile);
		tile = tile->_bottomTile;
	}

	while (!tileSet.empty()) {
		tile = *tileSet.begin();
		while (true) {
			Region_Tile* leftTile = tile->_leftTile;
			if ((leftTile == this) || !tile->MergeLeftTile(region)) break;
			tileSet.erase(leftTile);
		}
		while (true) {
			Region_Tile* topTile = tile->_topTile;
			if ((topTile == this) || !tile->MergeTopTile(region)) break;
			tileSet.erase(topTile);
		}
		while (true) {
			Region_Tile* bottomTile = tile->_bottomTile;
			if ((bottomTile == this) || !tile->MergeBottomTile(region)) break;
			tileSet.erase(bottomTile);
		}
		while (true) {
			Region_Tile* rightTile = tile->_rightTile;
			if ((rightTile == this) || !tile->MergeRightTile(region)) break;
			tileSet.erase(rightTile);
		}
		tileSet.erase(tile);
	}
};				// Region_Tile::CleanNeighbours


bool 
Region_Tile::_IsTopFull() const
// *******************************
// Retourne true si toutes les tuiles sur this sont de meme isVoid,
{
  Region_Tile* upTile = _topTile;
  while (upTile && upTile->_isVoid == _isVoid &&
	 upTile->GetXMin() > GetXMin()) {
    upTile = upTile->_leftTile;
  }
  return (upTile && upTile->_isVoid == _isVoid &&
      upTile->GetXMin() <= GetXMin());
};				// Region_Tile::_IsTopFull

bool 
Region_Tile::_IsBottomFull() const
// *******************************
// Retourne true si toutes les tuiles sous this sont de meme isVoid,
{
  Region_Tile* downTile = _bottomTile;
  while (downTile && downTile->_isVoid == _isVoid &&
	 downTile->GetXMax() < GetXMax()) {
    downTile = downTile->_rightTile;
  }
  return (downTile && downTile->_isVoid == _isVoid &&
      downTile->GetXMax() >= GetXMax());
};				// Region_Tile::_IsBottomFull

bool 
Region_Tile::_IsLeftFull() const
// *******************************
// Retourne true si toutes les tuiles a gauche de this sont de meme isVoid,
{
  Region_Tile* leftTile = _leftTile;
  while (leftTile && leftTile->_isVoid == _isVoid &&
	 leftTile->GetYMax() < GetYMax()) {
    leftTile = leftTile->_topTile;
  }
  return (leftTile && leftTile->_isVoid == _isVoid &&
      leftTile->GetYMax() >= GetYMax());
};				// Region_Tile::_IsLeftFull

bool 
Region_Tile::_IsRightFull() const
// *******************************
// Retourne true si toutes les tuiles a droite de this sont de meme isVoid,
{
  Region_Tile* rightTile = _rightTile;
  while (rightTile && rightTile->_isVoid == _isVoid &&
	 rightTile->GetYMin() > GetYMin()) {
    rightTile = rightTile->_bottomTile;
  }
  return (rightTile && rightTile->_isVoid == _isVoid &&
      rightTile->GetYMin() <= GetYMin());
};				// Region_Tile::_IsRightFull

Unit
Region_Tile::_TopSplitAtVerticalSize (Region* region)
// **************************************************
// Decoupe les tuiles top-left et top-right a la taille de this
// Retourne la hauteur minimale de la rangee
// Doit etre appele apres verif _IsTopFull()
{
  Region_Tile* upTile = _topTile;
  Unit height = upTile->GetYMax() - GetYMax();
  if (upTile && upTile->GetXMax() != GetXMax()) {
    upTile->SplitVertical (region, GetXMax());
    upTile = _topTile;
  }
  Unit ht;
  while (upTile && upTile->GetXMin() > GetXMin()) {
    ht = upTile->GetYMax() - GetYMax();
    if (height > ht) height = ht;
    upTile = upTile->_leftTile;
  }
  ht =  upTile->GetYMax() - GetYMax();
  if (height > ht) height = ht;
  if (upTile && upTile->GetXMin() != GetXMin()) {
    upTile->SplitVertical (region, GetXMin());
  }
  return height;
};				// Region_Tile::_TopSplitAtVerticalSize

Unit
Region_Tile::_BottomSplitAtVerticalSize (Region* region)
// *****************************************************
// Decoupe les tuiles bottom-left et bottom-right a la taille de this
// Retourne la hauteur minimale de la rangee
// Doit etre appele apres verif _IsBottomFull()
{
  Region_Tile* downTile = _bottomTile;
  Unit height = GetYMin() - downTile->GetYMin();
  if (downTile && downTile->GetXMin() != GetXMin()) {
    downTile->SplitVertical (region, GetXMin());
    downTile = _bottomTile;
  }
  Unit ht;
  while (downTile && downTile->GetXMax() < GetXMax()) {
    ht = GetYMin() - downTile->GetYMin();
    if (height > ht) height = ht;
    downTile = downTile->_rightTile;
  }
  ht = GetYMin() - downTile->GetYMin();
  if (height > ht) height = ht;
  if (downTile && downTile->GetXMax() != GetXMax()) {
    downTile->SplitVertical (region, GetXMax());
  }
  return height;
};				// Region_Tile::_BottomSplitAtVerticalSize


Unit
Region_Tile::_LeftSplitAtHorizontalSize (Region* region)
// *****************************************************
// Decoupe les tuiles left-top et left-bottom a la taille de this
// Retourne la largeur minimale de la colonne
// Doit etre appele apres verif _IsLeftFull()
{
  Region_Tile* leftTile = _leftTile;
  Unit width = GetXMin() - leftTile->GetXMin();
  if (leftTile && leftTile->GetYMin() != GetYMin()) {
    leftTile->SplitHorizontal (region, GetYMin());
    leftTile = _leftTile;
  }
  Unit wt;
  while (leftTile && leftTile->GetYMax() < GetYMax()) {
    wt = GetXMin() - leftTile->GetXMin();
    if (width > wt) width = wt;
    leftTile = leftTile->_topTile;
  }
  wt = GetXMin() - leftTile->GetXMin();
  if (width > wt) width = wt;
  if (leftTile && leftTile->GetYMax() != GetYMax()) {
    leftTile->SplitHorizontal (region, GetYMax());
  }
  return width;
};				// Region_Tile::_LeftSplitAtHorizontalSize


Unit
Region_Tile::_RightSplitAtHorizontalSize (Region* region)
// ******************************************************
// Decoupe les tuiles right-bottom et right-top a la taille de this
// Retourne la largeur minimale de la colonne
// Doit etre appele apres verif _IsRightFull()
{
  Region_Tile* rightTile = _rightTile;
  Unit width = rightTile->GetXMax() - GetXMax();
  if (rightTile && rightTile->GetYMax() != GetYMax()) {
    rightTile->SplitHorizontal (region, GetYMax());
    rightTile = _rightTile;
  }
  Unit wt;
  while (rightTile && rightTile->GetYMin() > GetYMin()) {
    wt = rightTile->GetXMax() - GetXMax();
    if (width > wt) width = wt;
    rightTile = rightTile->_bottomTile;
  }
  wt =  rightTile->GetXMax() - GetXMax();
  if (width > wt) width = wt;
  if (rightTile && rightTile->GetYMin() != GetYMin()) {
    rightTile->SplitHorizontal (region, GetYMin());
  }
  return width;
};				// Region_Tile::_RightSplitAtHorizontalSize


bool
Region_Tile::_TopSplitAtHorizontalSize (Region* region, const Unit height)
// ***********************************************************************
// Retaille la hauteur de toutes les tuiles sur this a la hauteur height
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* upTile = _topTile;

  while (upTile && upTile->GetXMin() >= GetXMin()) {
    if (upTile->GetYMax() - GetYMax() > height) {
      upTile->SplitHorizontal (region, GetYMax() + height);
      if (upTile->_bottomTile != this) upTile = upTile->_bottomTile;
      modif = true;
    }
    if (upTile->_rightTile && upTile->_rightTile->GetXMax() == GetXMax()) {
      // Merge upTile et upTile->_rightTile
      modif = upTile->MergeRightTile (region) || modif;
    }
    upTile = upTile->_leftTile;
  }
  return modif;
};				// Region_Tile::_TopSplitAtHorizontalSize


bool
Region_Tile::_BottomSplitAtHorizontalSize (Region* region, const Unit height)
// **************************************************************************
// Retaille la hauteur de toutes les tuiles sous this a la hauteur height
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* downTile = _bottomTile;

  while (downTile && downTile->GetXMax() <= GetXMax()) {
    if (GetYMin() - downTile->GetYMin() > height) {
      downTile->SplitHorizontal (region, GetYMin() - height);
      modif = true;
      if (downTile->_topTile != this) downTile = downTile->_topTile;
    }
    if (downTile->_leftTile && downTile->_leftTile->GetXMin() == GetXMin()) {
      // Merge downTile et downTile->_leftTile
      modif = downTile->MergeLeftTile (region) || modif;
    }
    downTile = downTile->_rightTile;
  }
  return modif;
};				// Region_Tile::_BottomSplitAtHorizontalSize

bool
Region_Tile::_LeftSplitAtVerticalSize (Region* region, const Unit width)
// **************************************************************************
// Retaille la largeur de toutes les tuiles a gauche de this a la largeur width
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* leftTile = _leftTile;

  while (leftTile && leftTile->GetYMax() <= GetYMax()) {
    if (GetXMin() - leftTile->GetXMin() > width) {
      leftTile->SplitVertical (region, GetXMin() - width);
      modif = true;
      if (leftTile->_rightTile != this) leftTile = leftTile->_rightTile;
    }
    if (leftTile->_bottomTile && leftTile->_bottomTile->GetYMin() == GetYMin()) {
      // Merge leftTile et leftTile->_bottomTile
      modif = leftTile->MergeBottomTile (region) || modif;
    }
    leftTile = leftTile->_topTile;
  }
  return modif;
};				// Region_Tile::_LeftSplitAtVerticalSize


bool
Region_Tile::_RightSplitAtVerticalSize (Region* region, const Unit width)
// **************************************************************************
// Retaille la hauteur de toutes les tuiles a droite de this a la largeur width
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* rightTile = _rightTile;

  while (rightTile && rightTile->GetYMin() >= GetYMin()) {
    if (rightTile->GetXMax() - GetXMax() > width) {
      rightTile->SplitVertical (region, GetXMax() + width);
      modif = true;
      if (rightTile->_leftTile != this) rightTile = rightTile->_leftTile;
    }
    if (rightTile->_topTile && rightTile->_topTile->GetYMax() == GetYMax()) {
      // Merge rightTile et rightTile->_leftTile
      modif = rightTile->MergeTopTile (region) || modif;
    }
    rightTile = rightTile->_bottomTile;
  }
  return modif;
};				// Region_Tile::_RightSplitAtVerticalSize


Interval 
Region_Tile::_GetTopNeighbour () const
// ***********************************
// Retourne le merge des intervals de voisins sur this
{
  Interval result = Interval();
  Region_Tile* topTile = _topTile;
  while (topTile && topTile->GetXMax() > GetXMin()) {
    if (topTile->_isVoid == _isVoid) {
      result.Merge (Interval (topTile->GetXMin(), topTile->GetXMax()));
    }
    topTile = topTile->_leftTile;
  }
  return result.GetIntersection (Interval (GetXMin(), GetXMax()));
};				// Region_Tile::_GetTopNeighbour

Interval 
Region_Tile::_GetBottomNeighbour () const
// ***********************************
// Retourne le merge des intervals de voisins sur this
{
  Interval result = Interval();
  Region_Tile* bottomTile = _bottomTile;
  while (bottomTile && bottomTile->GetXMin() < GetXMax()) {
    if (bottomTile->_isVoid == _isVoid) {
      result.Merge (Interval (bottomTile->GetXMin(), bottomTile->GetXMax()));
    }
    bottomTile = bottomTile->_rightTile;
  }
  return result.GetIntersection (Interval (GetXMin(), GetXMax()));
};				// Region_Tile::_GetBottomNeighbour

Interval 
Region_Tile::_GetLeftNeighbour () const
// ************************************
// Retourne le merge des intervals de voisins de gauche de this
{
  Interval result = Interval();
  Region_Tile* leftTile = _leftTile;
  while (leftTile && leftTile->GetYMin() < GetYMax()) {
    if (leftTile->_isVoid == _isVoid) {
      result.Merge (Interval (leftTile->GetYMin(), leftTile->GetYMax()));
    }
    leftTile = leftTile->_topTile;
  }
  return result.GetIntersection (Interval (GetYMin(), GetYMax()));
};				// Region_Tile::_GetLeftNeighbour

Interval 
Region_Tile::_GetRightNeighbour () const
// *************************************
// Retourne le merge des intervals de voisins de droite de this
{
  Interval result = Interval();
  Region_Tile* rightTile = _rightTile;
  while (rightTile && rightTile->GetYMax() > GetYMin()) {
    if (rightTile->_isVoid == _isVoid) {
      result.Merge (Interval (rightTile->GetYMin(), rightTile->GetYMax()));
    }
    rightTile = rightTile->_bottomTile;
  }
  return result.GetIntersection (Interval (GetYMin(), GetYMax()));
};				// Region_Tile::_GetRightNeighbour

bool
Region_Tile::VerticalEnhancement(Region* region) 
// *********************************************  
{
  bool modif = false;

  while (_IsTopFull ()) {
    Unit height = _TopSplitAtVerticalSize (region);
    modif = _TopSplitAtHorizontalSize (region, height);
    modif = MergeTopTile (region) || modif;
  }
  while (_IsBottomFull ()) {
    Unit height = _BottomSplitAtVerticalSize (region);
    modif = _BottomSplitAtHorizontalSize (region, height);
    modif = MergeBottomTile (region) || modif;
  }
  return modif;
};				// Region_Tile::VerticalEnhancement

bool
Region_Tile::HorizontalEnhancement(Region* region) 
// ***********************************************  
{
  bool modif = false;

  while (_IsLeftFull ()) {
    Unit width = _LeftSplitAtHorizontalSize (region);
    modif = _LeftSplitAtVerticalSize (region, width);
    modif = MergeLeftTile (region) || modif;
  }
  while (_IsRightFull ()) {
    Unit width = _RightSplitAtHorizontalSize (region);
    modif = _RightSplitAtVerticalSize (region, width);
    modif = MergeRightTile (region) || modif;
  }
  return modif;
};				// Region_Tile::HorizontalEnhancement


string Region_Tile::_GetString() const
// ***********************************
{
	string s = "<" + _TName("Region::Tile") + ">";
	s.insert(s.length() - 1, " " + GetString(_boundingBox));
	if (_isVoid) s.insert(s.length() - 1, " VOID");
	return s;
};				// ion_Tile::_Get

Record* Region_Tile::_GetRecord() const
// ******************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("BoundingBox", &_boundingBox));
		record->Add(GetSlot("IsVoid", &_isVoid));
		record->Add(GetSlot("LeftTile", _leftTile));
		record->Add(GetSlot("BottomTile", _bottomTile));
		record->Add(GetSlot("TopTile", _topTile));
		record->Add(GetSlot("RightTile", _rightTile));
	}
	return record;
}



// ****************************************************************************************************
// Region_Tile::IsVoidFilter implementation
// ****************************************************************************************************

Region_Tile::IsVoidFilter::IsVoidFilter()
// **************************************
{
}

Region_Tile::IsVoidFilter::IsVoidFilter(const Region_Tile::IsVoidFilter& filter)
// *****************************************************************************
{
}

Region_Tile::IsVoidFilter& Region_Tile::IsVoidFilter::operator=(const IsVoidFilter& filter)
// ****************************************************************************************
{
	return *this;
}

Filter<Region_Tile*>* Region_Tile::IsVoidFilter::GetClone() const
// **************************************************************
{
	return new Region_Tile::IsVoidFilter(*this);
}

bool Region_Tile::IsVoidFilter::Accept(Region_Tile* tile) const
// ************************************************************
{
	return tile->IsVoid();
}

string Region_Tile::IsVoidFilter::_GetString() const
// *************************************************
{
	return "<" + _TName("RegionTile::IsVoidFilter") + ">";
}



// ****************************************************************************************************
// Region_Tiles declaration
// ****************************************************************************************************

class Region_Tiles : public Collection<Region_Tile*> {
// *************************************************

// Types
// *****

	public: typedef Collection<Region_Tile*> Inherit;

	public: class Locator : public Hurricane::Locator<Region_Tile*> {
	// ************************************************************

		public: typedef Hurricane::Locator<Region_Tile*> Inherit;

		private: const Region* _region;
		private: stack<Region_Tile*> _tileStack;

		public: Locator(const Region* region = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Region_Tile* GetElement() const;
		public: virtual Hurricane::Locator<Region_Tile*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Region* _region;

// Constructors
// ************

	public: Region_Tiles(const Region* region = NULL);
	public: Region_Tiles(const Region_Tiles& Tiles);

// Operators
// *********

	public: Region_Tiles& operator=(const Region_Tiles& Tiles);

// Accessors
// *********

	public: virtual Collection<Region_Tile*>* GetClone() const;
	public: virtual Hurricane::Locator<Region_Tile*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Region_Tiles implementation
// ****************************************************************************************************

Region_Tiles::Region_Tiles(const Region* region)
// *********************************************
: 	Inherit(),
	_region(region)
{
}

Region_Tiles::Region_Tiles(const Region_Tiles& tiles)
// **************************************************
: 	Inherit(),
	_region(tiles._region)
{
}

Region_Tiles& Region_Tiles::operator=(const Region_Tiles& tiles)
// *************************************************************
{
	_region = tiles._region;
	return *this;
}

Collection<Region_Tile*>* Region_Tiles::GetClone() const
// *****************************************************
{
	return new Region_Tiles(*this);
}

Locator<Region_Tile*>* Region_Tiles::GetLocator() const
// ****************************************************
{
	return new Locator(_region);
}

string Region_Tiles::_GetString() const
// ************************************
{
	string s = "<" + _TName("Region::Tiles");
	if (_region) s += " " + GetString(_region);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_Tiles::Locator implementation
// ****************************************************************************************************

Region_Tiles::Locator::Locator(const Region* region)
// *************************************************
:	Inherit(),
	_region(region),
	_tileStack()
{
	if (_region) {
		Region_Tile* tile = _region->_GetBottomRightTile();
		while (tile) {
			_tileStack.push(tile);
			tile = tile->_leftTile;
		}
	}
}

Region_Tiles::Locator::Locator(const Locator& locator)
// ***************************************************
:	Inherit(),
	_region(locator._region),
	_tileStack(locator._tileStack)
{
}

Region_Tiles::Locator& Region_Tiles::Locator::operator=(const Locator& locator)
// ****************************************************************************
{
	_region = locator._region;
	_tileStack = locator._tileStack;
	return *this;
}

Region_Tile* Region_Tiles::Locator::GetElement() const
// ***************************************************
{
	return (!_tileStack.empty()) ? _tileStack.top() : NULL;
}

Locator<Region_Tile*>* Region_Tiles::Locator::GetClone() const
// ***********************************************************
{
	return new Locator(*this);
}

bool Region_Tiles::Locator::IsValid() const
// ***************************************
{
	return !_tileStack.empty();
}

void Region_Tiles::Locator::Progress()
// **********************************
{
	if (!_tileStack.empty()) {
		Region_Tile* tile = _tileStack.top();
		_tileStack.pop();
		Unit xMin = tile->GetXMin();
		Unit xMax = tile->GetXMax();
		Region_Tile* topTile = tile->_topTile;
		while (topTile && (xMin < topTile->GetXMax())) {
			if (topTile->GetXMax() <= xMax) _tileStack.push(topTile);
			topTile = topTile->_leftTile;
		}
	}
}

string Region_Tiles::Locator::_GetString() const
// *********************************************
{
	string s = "<" + _TName("Region::Tiles::Locator");
	if (_region) s += " " + GetString(_region);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_TilesUnder declaration
// ****************************************************************************************************

class Region_TilesUnder : public Collection<Region_Tile*> {
// ******************************************************

// Types
// *****

	public: typedef Collection<Region_Tile*> Inherit;

	public: class Locator : public Hurricane::Locator<Region_Tile*> {
	// ************************************************************

		public: typedef Hurricane::Locator<Region_Tile*> Inherit;

		private: const Region* _region;
		private: Box _area;
		private: Region_Tile* _startTile;
		private: stack<Region_Tile*> _tileStack;
		private: set<Region_Tile*> _tileSet; // to check never stack 2 times the same tile

		public: Locator();
		public: Locator(const Region* region, const Box& area, Region_Tile* startTile = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Region_Tile* GetElement() const;
		public: virtual Hurricane::Locator<Region_Tile*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Region* _region;
	private: Box _area;
	private: Region_Tile* _startTile;

// Constructors
// ************

	public: Region_TilesUnder();
	public: Region_TilesUnder(const Region* region, const Box& area, Region_Tile* startTile = NULL);
	public: Region_TilesUnder(const Region_TilesUnder& Tiles);

// Operators
// *********

	public: Region_TilesUnder& operator=(const Region_TilesUnder& Tiles);

// Accessors
// *********

	public: virtual Collection<Region_Tile*>* GetClone() const;
	public: virtual Hurricane::Locator<Region_Tile*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Region_TilesUnder implementation
// ****************************************************************************************************

Region_TilesUnder::Region_TilesUnder()
// ***********************************
: 	Inherit(),
	_region(NULL),
	_area(),
	_startTile(NULL)
{
}

Region_TilesUnder::Region_TilesUnder(const Region* region, const Box& area, Region_Tile* startTile)
// ************************************************************************************************
: 	Inherit(),
	_region(region),
	_startTile(startTile)
{
	_area = area.GetIntersection(region->GetBoundingBox());
}

Region_TilesUnder::Region_TilesUnder(const Region_TilesUnder& tiles)
// *****************************************************************
: 	Inherit(),
	_region(tiles._region),
	_area(tiles._area),
	_startTile(tiles._startTile)
{
}

Region_TilesUnder& Region_TilesUnder::operator=(const Region_TilesUnder& tiles)
// ****************************************************************************
{
	_region = tiles._region;
	_area = tiles._area;
	_startTile = tiles._startTile;
	return *this;
}

Collection<Region_Tile*>* Region_TilesUnder::GetClone() const
// **********************************************************
{
	return new Region_TilesUnder(*this);
}

Locator<Region_Tile*>* Region_TilesUnder::GetLocator() const
// *********************************************************
{
	return new Locator(_region, _area, _startTile);
}

string Region_TilesUnder::_GetString() const
// *****************************************
{
	string s = "<" + _TName("Region::TilesUnder");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
		s += " " + GetString(_startTile);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_TilesUnder::Locator implementation
// ****************************************************************************************************

Region_TilesUnder::Locator::Locator()
// **********************************
:	Inherit(),
	_region(NULL),
	_area(),
	_startTile(NULL),
	_tileStack()
{
}

Region_TilesUnder::Locator::Locator(const Region* region, const Box& area, Region_Tile* startTile)
// ***********************************************************************************************
:	Inherit(),
	_region(region),
	_area(area),
	_startTile(startTile),
	_tileStack()
{
	if (_region && !_area.IsEmpty()) {
		Region_Tile* tile = _region->_GetTileAt(Point(_area.GetXMax(), _area.GetYMin()), _startTile);
		while (tile && (_area.GetXMin() < tile->GetXMax())) {
			while (tile && (tile->GetYMax() <= _area.GetYMin())) tile = tile->_topTile;
			if (tile) {
				if (_tileSet.find(tile) == _tileSet.end()) {
					_tileSet.insert(tile);
					_tileStack.push(tile);
				}
				tile = tile->_leftTile;
			}
		}
	}
}

Region_TilesUnder::Locator::Locator(const Locator& locator)
// ********************************************************
:	Inherit(),
	_region(locator._region),
	_area(locator._area),
	_startTile(locator._startTile),
	_tileStack(locator._tileStack)
{
}

Region_TilesUnder::Locator& Region_TilesUnder::Locator::operator=(const Locator& locator)
// **************************************************************************************
{
	_region = locator._region;
	_area = locator._area;
	_startTile = locator._startTile;
	_tileStack = locator._tileStack;
	return *this;
}

Region_Tile* Region_TilesUnder::Locator::GetElement() const
// ********************************************************
{
	return (!_tileStack.empty()) ? _tileStack.top() : NULL;
}

Locator<Region_Tile*>* Region_TilesUnder::Locator::GetClone() const
// ****************************************************************
{
	return new Locator(*this);
}

bool Region_TilesUnder::Locator::IsValid() const
// *********************************************
{
	return !_tileStack.empty();
}

void Region_TilesUnder::Locator::Progress()
// ****************************************
{
	if (!_tileStack.empty()) {
		Region_Tile* tile = _tileStack.top();
		_tileStack.pop();
		Unit xMin = max(tile->GetXMin(), _area.GetXMin());
		Unit xMax = tile->GetXMax();
		Region_Tile* topTile = tile->_topTile;
		while (topTile && (xMin < topTile->GetXMax())) {
			if ((topTile->GetXMin() <= _area.GetXMax()) &&
					(topTile->GetYMin() <= _area.GetYMax()) &&
					(min(topTile->GetXMax(), _area.GetXMax()) <= xMax)) { 
				if (_tileSet.find(topTile) == _tileSet.end()) {
					_tileSet.insert(topTile);
					_tileStack.push(topTile);
				}
			}
			topTile = topTile->_leftTile;
		}
	}
}

string Region_TilesUnder::Locator::_GetString() const
// **************************************************
{
	string s = "<" + _TName("Region::TilesUnder::Locator");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
		s += " " + GetString(_startTile);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_BoxesUnder declaration
// ****************************************************************************************************

class Region_BoxesUnder : public Collection<Box> {
// *********************************************

// Types
// *****

	public: typedef Collection<Box> Inherit;

	public: class Locator : public Hurricane::Locator<Box> {
	// ***************************************************

		public: typedef Hurricane::Locator<Box> Inherit;

		private: const Region* _region;
		private: Box _area;
		private: GenericLocator<Region_Tile*> _tileLocator;

		public: Locator();
		public: Locator(const Region* region, const Box& area = Box());
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Box GetElement() const;
		public: virtual Hurricane::Locator<Box>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Region* _region;
	private: Box _area;

// Constructors
// ************

	public: Region_BoxesUnder();
	public: Region_BoxesUnder(const Region* region, const Box& area = Box());
	public: Region_BoxesUnder(const Region_BoxesUnder& boxes);

// Operators
// *********

	public: Region_BoxesUnder& operator=(const Region_BoxesUnder& boxes);

// Accessors
// *********

	public: virtual Collection<Box>* GetClone() const;
	public: virtual Hurricane::Locator<Box>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Region_BoxesUnder implementation
// ****************************************************************************************************

Region_BoxesUnder::Region_BoxesUnder()
// ***********************************
: 	Inherit(),
	_region(NULL),
	_area()
{
}

Region_BoxesUnder::Region_BoxesUnder(const Region* region, const Box& area)
// ************************************************************************
: 	Inherit(),
	_region(region),
	_area(area)
{
}

Region_BoxesUnder::Region_BoxesUnder(const Region_BoxesUnder& boxes)
// *****************************************************************
: 	Inherit(),
	_region(boxes._region),
	_area(boxes._area)
{
}

Region_BoxesUnder& Region_BoxesUnder::operator=(const Region_BoxesUnder& boxes)
// ****************************************************************************
{
	_region = boxes._region;
	_area = boxes._area;
	return *this;
}

Collection<Box>* Region_BoxesUnder::GetClone() const
// *************************************************
{
	return new Region_BoxesUnder(*this);
}

Locator<Box>* Region_BoxesUnder::GetLocator() const
// ************************************************
{
	return new Locator(_region, _area);
}

string Region_BoxesUnder::_GetString() const
// *****************************************
{
	string s = "<" + _TName("Region::BoxesUnder");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_BoxesUnder::Locator implementation
// ****************************************************************************************************

Region_BoxesUnder::Locator::Locator()
// **********************************
:	Inherit(),
	_region(NULL),
	_area(),
	_tileLocator()
{
}

Region_BoxesUnder::Locator::Locator(const Region* region, const Box& area)
// ***********************************************************************
:	Inherit(),
	_region(region),
	_area(area),
	_tileLocator()
{
	if (_region) {
		_tileLocator =
			(_area.IsEmpty()) ?
				_region->_GetTiles().GetSubSet(!Region_Tile::GetIsVoidFilter()).GetLocator() :
				_region->_GetTilesUnder(_area).GetSubSet(!Region_Tile::GetIsVoidFilter()).GetLocator();
	}
}

Region_BoxesUnder::Locator::Locator(const Locator& locator)
// ********************************************************
:	Inherit(),
	_region(locator._region),
	_area(locator._area),
	_tileLocator(locator._tileLocator)
{
}

Region_BoxesUnder::Locator& Region_BoxesUnder::Locator::operator=(const Locator& locator)
// **************************************************************************************
{
	_region = locator._region;
	_area = locator._area;
	_tileLocator = locator._tileLocator;
	return *this;
}

Box Region_BoxesUnder::Locator::GetElement() const
// ***********************************************
{
	return (_tileLocator.IsValid()) ? _tileLocator.GetElement()->GetBoundingBox() : Box();
}

Locator<Box>* Region_BoxesUnder::Locator::GetClone() const
// *******************************************************
{
	return new Locator(*this);
}

bool Region_BoxesUnder::Locator::IsValid() const
// ********************************************
{
	return _tileLocator.IsValid();
}

void Region_BoxesUnder::Locator::Progress()
// ****************************************
{
	_tileLocator.Progress();
}

string Region_BoxesUnder::Locator::_GetString() const
// **************************************************
{
	string s = "<" + _TName("Region::BoxesUnder::Locator");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_VoidBoxesUnder declaration
// ****************************************************************************************************

class Region_VoidBoxesUnder : public Collection<Box> {
// *************************************************

// Types
// *****

	public: typedef Collection<Box> Inherit;

	public: class Locator : public Hurricane::Locator<Box> {
	// ***************************************************

		public: typedef Hurricane::Locator<Box> Inherit;

		private: const Region* _region;
		private: Box _area;
		private: GenericLocator<Region_Tile*> _tileLocator;

		public: Locator();
		public: Locator(const Region* region, const Box& area = Box());
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Box GetElement() const;
		public: virtual Hurricane::Locator<Box>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Region* _region;
	private: Box _area;

// Constructors
// ************

	public: Region_VoidBoxesUnder();
	public: Region_VoidBoxesUnder(const Region* region, const Box& area = Box());
	public: Region_VoidBoxesUnder(const Region_VoidBoxesUnder& boxes);

// Operators
// *********

	public: Region_VoidBoxesUnder& operator=(const Region_VoidBoxesUnder& boxes);

// Accessors
// *********

	public: virtual Collection<Box>* GetClone() const;
	public: virtual Hurricane::Locator<Box>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Region_VoidBoxesUnder implementation
// ****************************************************************************************************

Region_VoidBoxesUnder::Region_VoidBoxesUnder()
// *******************************************
: 	Inherit(),
	_region(NULL),
	_area()
{
}

Region_VoidBoxesUnder::Region_VoidBoxesUnder(const Region* region, const Box& area)
// ********************************************************************************
: 	Inherit(),
	_region(region),
	_area(area)
{
}

Region_VoidBoxesUnder::Region_VoidBoxesUnder(const Region_VoidBoxesUnder& boxes)
// *****************************************************************************
: 	Inherit(),
	_region(boxes._region),
	_area(boxes._area)
{
}

Region_VoidBoxesUnder& Region_VoidBoxesUnder::operator=(const Region_VoidBoxesUnder& boxes)
// ****************************************************************************************
{
	_region = boxes._region;
	_area = boxes._area;
	return *this;
}

Collection<Box>* Region_VoidBoxesUnder::GetClone() const
// *****************************************************
{
	return new Region_VoidBoxesUnder(*this);
}

Locator<Box>* Region_VoidBoxesUnder::GetLocator() const
// ****************************************************
{
	return new Locator(_region, _area);
}

string Region_VoidBoxesUnder::_GetString() const
// *********************************************
{
	string s = "<" + _TName("Region::VoidBoxesUnder");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_VoidBoxesUnder::Locator implementation
// ****************************************************************************************************

Region_VoidBoxesUnder::Locator::Locator()
// **************************************
:	Inherit(),
	_region(NULL),
	_area(),
	_tileLocator()
{
}

Region_VoidBoxesUnder::Locator::Locator(const Region* region, const Box& area)
// ***************************************************************************
:	Inherit(),
	_region(region),
	_area(area),
	_tileLocator()
{
	if (_region) {
		_tileLocator =
			(_area.IsEmpty()) ?
				_region->_GetTiles().GetSubSet(Region_Tile::GetIsVoidFilter()).GetLocator() :
				_region->_GetTilesUnder(_area).GetSubSet(Region_Tile::GetIsVoidFilter()).GetLocator();
	}
}

Region_VoidBoxesUnder::Locator::Locator(const Locator& locator)
// ************************************************************
:	Inherit(),
	_region(locator._region),
	_area(locator._area),
	_tileLocator(locator._tileLocator)
{
}

Region_VoidBoxesUnder::Locator& Region_VoidBoxesUnder::Locator::operator=(const Locator& locator)
// **********************************************************************************************
{
	_region = locator._region;
	_area = locator._area;
	_tileLocator = locator._tileLocator;
	return *this;
}

Box Region_VoidBoxesUnder::Locator::GetElement() const
// ***************************************************
{
	return (_tileLocator.IsValid()) ? _tileLocator.GetElement()->GetBoundingBox() : Box();
}

Locator<Box>* Region_VoidBoxesUnder::Locator::GetClone() const
// ***********************************************************
{
	return new Locator(*this);
}

bool Region_VoidBoxesUnder::Locator::IsValid() const
// *************************************************
{
	return _tileLocator.IsValid();
}

void Region_VoidBoxesUnder::Locator::Progress()
// ********************************************
{
	_tileLocator.Progress();
}

string Region_VoidBoxesUnder::Locator::_GetString() const
// ******************************************************
{
	string s = "<" + _TName("Region::VoidBoxesUnder::Locator");
	if (_region) {
		s += " " + GetString(_region);
		s += " " + GetString(_area);
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_Intervals declaration
// ****************************************************************************************************

class Region_Intervals : public Collection<Interval> {
// *************************************************

// Types
// *****

	public: typedef Collection<Interval> Inherit;

	public: class Locator : public Hurricane::Locator<Interval> {
	// ********************************************************

		public: typedef Hurricane::Locator<Interval> Inherit;

		private: const Region::SwapLine* _swapLine;
		private: Region::Tile* _lowerTile;
		private: Region::Tile* _upperTile;

		public: Locator(const Region::SwapLine* swapLine = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Interval GetElement() const;
		public: virtual Hurricane::Locator<Interval>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Region::SwapLine* _swapLine;

// Constructors
// ************

	public: Region_Intervals(const Region::SwapLine* swapLine = NULL);
	public: Region_Intervals(const Region_Intervals& intervals);

// Operators
// *********

	public: Region_Intervals& operator=(const Region_Intervals& intervals);

// Accessors
// *********

	public: virtual Collection<Interval>* GetClone() const;
	public: virtual Hurricane::Locator<Interval>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Region_Intervals implementation
// ****************************************************************************************************

Region_Intervals::Region_Intervals(const Region::SwapLine* swapLine)
// *****************************************************************
: 	Inherit(),
	_swapLine(swapLine)
{
}

Region_Intervals::Region_Intervals(const Region_Intervals& intervals)
// ******************************************************************
: 	Inherit(),
	_swapLine(intervals._swapLine)
{
}

Region_Intervals& Region_Intervals::operator=(const Region_Intervals& intervals)
// *****************************************************************************
{
	_swapLine = intervals._swapLine;
	return *this;
}

Collection<Interval>* Region_Intervals::GetClone() const
// *****************************************************
{
	return new Region_Intervals(*this);
}

Locator<Interval>* Region_Intervals::GetLocator() const
// ****************************************************
{
	return new Locator(_swapLine);
}

string Region_Intervals::_GetString() const
// ****************************************
{
	string s = "<" + _TName("Region::SwapLine::Intervals");
	if (_swapLine) s += " " + GetString(_swapLine);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region_Intervals::Locator implementation
// ****************************************************************************************************

Region_Intervals::Locator::Locator(const Region::SwapLine* swapLine)
// *****************************************************************
:	Inherit(),
	_swapLine(swapLine),
	_lowerTile(NULL),
	_upperTile(NULL)
{
	if (_swapLine) {
		switch (_swapLine->GetType()) {
			case Region::SwapLine::Type::VERTICAL : {
				Unit x = _swapLine->GetPosition();
				Unit yMax = _swapLine->GetExtention().GetVMax();
				_lowerTile = _swapLine->_GetBaseTile();
				while (_lowerTile && _lowerTile->IsVoid())
					_lowerTile = _lowerTile->GetTopTile(x);
				if (_lowerTile && (yMax < _lowerTile->GetYMin())) _lowerTile = NULL;
				if (_lowerTile) {
					Region::Tile* tile = _lowerTile;
					while (tile && !tile->IsVoid()) {
						_upperTile = tile;
						tile = tile->GetTopTile(x);
						if (tile && (yMax < tile->GetYMin())) tile = NULL;
					}
				}
				break;
			}
			case Region::SwapLine::Type::HORIZONTAL : {
				Unit y = _swapLine->GetPosition();
				Unit xMax = _swapLine->GetExtention().GetVMax();
				_lowerTile = _swapLine->_GetBaseTile();
				while (_lowerTile && _lowerTile->IsVoid())
					_lowerTile = _lowerTile->GetRightTile(y);
				if (_lowerTile && (xMax < _lowerTile->GetXMin())) _lowerTile = NULL;
				if (_lowerTile) {
					Region::Tile* tile = _lowerTile;
					while (tile && !tile->IsVoid()) {
						_upperTile = tile;
						tile = tile->GetRightTile(y);
						if (tile && (xMax < tile->GetXMin())) tile = NULL;
					}
				}
				break;
			}
		}
	}
}

Region_Intervals::Locator::Locator(const Locator& locator)
// *******************************************************
:	Inherit(),
	_swapLine(locator._swapLine),
	_lowerTile(locator._lowerTile),
	_upperTile(locator._upperTile)
{
}

Region_Intervals::Locator& Region_Intervals::Locator::operator=(const Locator& locator)
// ************************************************************************************
{
	_swapLine = locator._swapLine;
	_lowerTile = locator._lowerTile;
	_upperTile = locator._upperTile;
	return *this;
}

Interval Region_Intervals::Locator::GetElement() const
// ***************************************************
{
	if (!IsValid()) return Interval();

	Interval interval;
	switch (_swapLine->GetType()) {
		case Region::SwapLine::Type::VERTICAL : {
			interval = Interval(_lowerTile->GetYMin(), _upperTile->GetYMax());
			break;
		}
		case Region::SwapLine::Type::HORIZONTAL : {
			interval = Interval(_lowerTile->GetXMin(), _upperTile->GetXMax());
			break;
		}
	}

	return interval.GetIntersection(_swapLine->GetExtention());
}

Locator<Interval>* Region_Intervals::Locator::GetClone() const
// ***********************************************************
{
	return new Locator(*this);
}

bool Region_Intervals::Locator::IsValid() const
// ********************************************
{
	return (_lowerTile && _upperTile);
}

void Region_Intervals::Locator::Progress()
// ***************************************
{
	if (IsValid()) {
		switch (_swapLine->GetType()) {
			case Region::SwapLine::Type::VERTICAL : {
				Unit x = _swapLine->GetPosition();
				Unit yMax = _swapLine->GetExtention().GetVMax();
				_lowerTile = _upperTile->GetTopTile(x);
				while (_lowerTile && _lowerTile->IsVoid())
					_lowerTile = _lowerTile->GetTopTile(x);
				if (_lowerTile && (yMax < _lowerTile->GetYMin())) _lowerTile = NULL;
				_upperTile = NULL;
				if (_lowerTile) {
					Region::Tile* tile = _lowerTile;
					while (tile && !tile->IsVoid()) {
						_upperTile = tile;
						tile = tile->GetTopTile(x);
						if (tile && (yMax < tile->GetYMin())) tile = NULL;
					}
				}
				break;
			}
			case Region::SwapLine::Type::HORIZONTAL : {
				Unit y = _swapLine->GetPosition();
				Unit xMax = _swapLine->GetExtention().GetVMax();
				_lowerTile = _upperTile->GetRightTile(y);
				while (_lowerTile && _lowerTile->IsVoid())
					_lowerTile = _lowerTile->GetRightTile(y);
				if (_lowerTile && (xMax < _lowerTile->GetXMin())) _lowerTile = NULL;
				_upperTile = NULL;
				if (_lowerTile) {
					Region::Tile* tile = _lowerTile;
					while (tile && !tile->IsVoid()) {
						_upperTile = tile;
						tile = tile->GetRightTile(y);
						if (tile && (xMax < tile->GetXMin())) tile = NULL;
					}
				}
				break;
			}
		}
	}
}

string Region_Intervals::Locator::_GetString() const
// *************************************************
{
	string s = "<" + _TName("Region::SwapLine::Intervals::Locator");
	if (_swapLine) s += " " + GetString(_swapLine);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Region::SwapLine::Type implementation
// ****************************************************************************************************

Region::SwapLine::Type::Type(const Code& code)
// *******************************************
:	_code(code)
{
}

Region::SwapLine::Type::Type(const Type& type)
// *******************************************
:	_code(type._code)
{
}

Region::SwapLine::Type& Region::SwapLine::Type::operator=(const Type& type)
// ************************************************************************
{
	_code = type._code;
	return *this;
}

string Region::SwapLine::Type::_GetString() const
// **********************************************
{
	switch (_code) {
		case VERTICAL : return "VERTICAL";
		case HORIZONTAL : return "HORIZONTAL";
	}
	return "ABNORMAL";
}

Record* Region::SwapLine::Type::_GetRecord() const
// *****************************************
{
	Record* record = new Record(GetString(this));
	record->Add ( GetSlot ( "Code", ((unsigned int*)((void*)&_code)) ) );
	return record;
}



// ****************************************************************************************************
// Region::SwapLine implementation
// ****************************************************************************************************

Region::SwapLine::SwapLine()
// *************************
:	_region(NULL),
	_type(),
	_position(),
	_extention(),
	_baseTile(NULL)
{
}

Region::SwapLine::SwapLine(Region* region, const Type& type, const Interval& extention)
// ************************************************************************************
:	_region(region),
	_type(type),
	_position(),
	_extention(extention),
	_baseTile(NULL)
{
	if (!_region)
		throw Error("Can't create " + _TName("Region::SwapLine") + " : null region");

	if (!_region->IsEmpty()) {
		switch (_type) {
			case Type::VERTICAL : {
				_position = _region->GetXMin();
				if (_extention.IsEmpty()) _extention = Interval(_region->GetYMin(), _region->GetYMax());
				_baseTile = _region->_GetTileAt(Point(_position, _extention.GetVMin()));
				break;
			}
			case Type::HORIZONTAL : {
				_position = _region->GetYMin();
				if (_extention.IsEmpty()) _extention = Interval(_region->GetXMin(), _region->GetXMax());
				_baseTile = _region->_GetTileAt(Point(_extention.GetVMin(), _position));
				break;
			}
		}
	}
}

Region::SwapLine::SwapLine(Region* region, const Type& type, const Unit& position, const Interval& extention)
// ****************************************************************************************************
:	_region(region),
	_type(type),
	_position(position),
	_extention(extention),
	_baseTile(NULL)
{
	if (!_region)
		throw Error("Can't create " + _TName("Region::SwapLine") + " : null region");

	if (!_region->IsEmpty()) {
		switch (_type) {
			case Type::VERTICAL : {
				if (_extention.IsEmpty()) _extention = Interval(_region->GetYMin(), _region->GetYMax());
				_baseTile = _region->_GetTileAt(Point(_position, _extention.GetVMin()));
				break;
			}
			case Type::HORIZONTAL : {
				if (_extention.IsEmpty()) _extention = Interval(_region->GetXMin(), _region->GetXMax());
				_baseTile = _region->_GetTileAt(Point(_extention.GetVMin(), _position));
				break;
			}
		}
	}
}

Region::SwapLine::SwapLine(const SwapLine& swapLine)
// *************************************************
:	_region(swapLine._region),
	_type(swapLine._type),
	_position(swapLine._position),
	_extention(swapLine._extention),
	_baseTile(swapLine._baseTile)
{
}

Region::SwapLine::~SwapLine()
// **************************
{
}

Region::SwapLine& Region::SwapLine::operator=(const SwapLine& swapLine)
// ********************************************************************
{
	_region = swapLine._region;
	_type = swapLine._type;
	_position = swapLine._position;
	_extention = swapLine._extention;
	_baseTile = swapLine._baseTile;
	return *this;
}

Intervals Region::SwapLine::GetIntervals() const
// *********************************************
{
	return Region_Intervals(this);
}

void Region::SwapLine::Progress(int n)
		  // ***************************
{
	if (IsValid() && n) {
		if (0 < n) {
			switch (_type) {
				case Type::VERTICAL : {
					while (n--) {
						Unit yMin = GetExtention().GetVMin();
						Unit yMax = GetExtention().GetVMax();
						Unit x = _region->GetXMax() + 1;
						Region::Tile* tile = _baseTile;
						while (tile) {
							if (_position < tile->GetXMax())
								x = min(tile->GetXMax(), x);
							else {
								assert(tile->GetXMax() == _position);
								Region::Tile* rightTile = tile->_rightTile;
								while (rightTile && (yMax < rightTile->GetYMin()))
									rightTile = rightTile->_bottomTile;
								while (rightTile && (yMin < rightTile->GetYMax())) {
									x = min(rightTile->GetXMax(), x);
									rightTile = rightTile->_bottomTile;
								}
							}
							tile = tile->GetTopTile(_position);
							if (tile && (yMax < tile->GetYMin())) tile = NULL;
						}
						_position = x;
						while (_baseTile && (_baseTile->GetXMax() <= x))
							_baseTile = _baseTile->GetRightTile(yMin);
					}
					break;
				}
				case Type::HORIZONTAL : {
					while (n--) {
						Unit xMin = GetExtention().GetVMin();
						Unit xMax = GetExtention().GetVMax();
						Unit y = _region->GetYMax() + 1;
						Region::Tile* tile = _baseTile;
						while (tile) {
							if (_position < tile->GetYMax())
								y = min(tile->GetYMax(), y);
							else {
								assert(tile->GetYMax() == _position);
								Region::Tile* topTile = tile->_topTile;
								while (topTile && (xMax < topTile->GetXMin()))
									topTile = topTile->_leftTile;
								while (topTile && (xMin < topTile->GetXMax())) {
									y = min(topTile->GetYMax(), y);
									topTile = topTile->_leftTile;
								}
							}
							tile = tile->GetRightTile(_position);
							if (tile && (xMax < tile->GetXMin())) tile = NULL;
						}
						_position = y;
						while (_baseTile && (_baseTile->GetYMax() <= y))
							_baseTile = _baseTile->GetTopTile(xMin);
					}
					break;
				}
			}
		}
		else {
			throw Error("SORRY, NOT IMPLEMENTED TODAY !");
		}
	}
}

void Region::SwapLine::Translate(const Unit& quantity)
// ***************************************************
{
	if (quantity) SetPosition(GetPosition() + quantity);
}

void Region::SwapLine::SetPosition(const Unit& position)
// *****************************************************
{
	if (position != _position) {
		_position = position;
		switch (_type) {
			case Type::VERTICAL : {
				_baseTile = _region->_GetTileAt(Point(_position, _extention.GetVMin()));
				break;
			}
			case Type::HORIZONTAL : {
				_baseTile = _region->_GetTileAt(Point(_extention.GetVMin(), _position));
				break;
			}
		}
	}
}

string Region::SwapLine::_GetString() const
// ****************************************
{
	string s = "<" + _GetTypeName() + ">";
	if (IsValid()) {
		s.insert(s.length() - 1, " " + GetString(_type));
		s.insert(s.length() - 1, " " + GetString(_position));
		s.insert(s.length() - 1, " " + GetString(_extention));
	}
	return s;
}

Record* Region::SwapLine::_GetRecord() const
// ***********************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("Region", _region));
		record->Add(GetSlot("Type", &_type));
		record->Add(GetSlot("Position", &_position));
		record->Add(GetSlot("Extention", &_extention));
		record->Add(GetSlot("BaseTile", _baseTile));
	}
	return record;
}



// ****************************************************************************************************
// Region implementation
// ****************************************************************************************************

Region::Region()
// *************
:	_bottomRightTile(NULL),
	_topLeftTile(NULL)
{
}

Region::Region(const Box& box, bool isVoid)
// ****************************************
{
	_bottomRightTile = new Tile(box, isVoid);
	_topLeftTile = _bottomRightTile;
}

Region::Region(const Region& region)
// *********************************
{
	// keep trace (as void tile) of the initial bounding box
	Box initialBoundingBox = region.GetBoundingBox();
	if (! initialBoundingBox.IsEmpty()) {
		_bottomRightTile = new Tile(region.GetBoundingBox(), true);
		_topLeftTile = _bottomRightTile;
		for_each_box(box, region.GetBoxes()) {
			Fill(box);
			end_for;
		}
	}
}

Region::~Region()
// **************
{
	Clear();
}

Region& Region::operator=(const Region& region)
// ********************************************
{
	Clear();
	// keep trace (as void tile) of the initial bounding box
	Box initialBoundingBox = region.GetBoundingBox();
	if (! initialBoundingBox.IsEmpty()) {
		_bottomRightTile = new Tile (initialBoundingBox, true);
		_topLeftTile = _bottomRightTile;
		for_each_box(box, region.GetBoxes()) {
			Fill(box);
			end_for;
		}
	}
	return *this;
}

Box Region::GetBoundingBox() const
// *******************************
{
	Box boundingBox;
	if (_bottomRightTile) boundingBox = _bottomRightTile->GetBoundingBox();
	if (_topLeftTile) boundingBox.Merge(_topLeftTile->GetBoundingBox());
	return boundingBox;
}

Unit Region::GetXMin() const
// *************************
{
	return (_topLeftTile) ? _topLeftTile->GetXMin() : Unit();
}

Unit Region::GetYMin() const
// *************************
{
	return (_bottomRightTile) ? _bottomRightTile->GetYMin() : Unit();
}

Unit Region::GetXMax() const
// *************************
{
	return (_bottomRightTile) ? _bottomRightTile->GetXMax() : Unit();
}

Unit Region::GetYMax() const
// *************************
{
	return (_topLeftTile) ? _topLeftTile->GetYMax() : Unit();
}

Boxes Region::GetBoxes() const
// ***************************
{
	return Region_BoxesUnder(this);
}

Boxes Region::GetBoxesUnder(const Box& area) const
// ***********************************************
{
	return Region_BoxesUnder(this, area);
}

Boxes Region::GetVoidBoxes() const
// *******************************
{
	return Region_VoidBoxesUnder(this);
}

Boxes Region::GetVoidBoxesUnder(const Box& area) const
// ***************************************************
{
	return Region_VoidBoxesUnder(this, area);
}

Region::SwapLine Region::GetVerticalSwapLine(const Interval& extention) const
// **************************************************************************
{
	return SwapLine((Region*)this, Region::SwapLine::Type::VERTICAL, extention);
}

Region::SwapLine Region::GetVerticalSwapLine(const Unit& x, const Interval& extention) const
// *****************************************************************************************
{
	return SwapLine((Region*)this, Region::SwapLine::Type::VERTICAL, x, extention);
}

Region::SwapLine Region::GetHorizontalSwapLine(const Interval& extention) const
// ****************************************************************************
{
	return SwapLine((Region*)this, Region::SwapLine::Type::HORIZONTAL, extention);
}

Region::SwapLine Region::GetHorizontalSwapLine(const Unit& y, const Interval& extention) const
// *******************************************************************************************
{
	return SwapLine((Region*)this, Region::SwapLine::Type::HORIZONTAL, y, extention);
}

bool Region::IsEmpty() const
// *************************
{
	return Region_Tiles(this).GetSubSet(!Tile::GetIsVoidFilter()).IsEmpty();
}

bool Region::Contains(const Point& point) const
// ********************************************
{
	return GetBoundingBox().Contains(point) && _GetNonVoidTileAt(point);
}

bool Region::Contains(const Box& box) const
// ****************************************
{
	if (box.IsPonctual()) return Contains(box.GetCenter());
	return GetBoundingBox().Contains(box) &&
		Region_TilesUnder (this, Box(box).Inflate(-1))
		                 .GetSubSet(Tile::GetIsVoidFilter()).IsEmpty();
}

bool Region::Contains(const Region& region) const
// **********************************************
{
	for_each_box(box, region.GetBoxesUnder(GetBoundingBox())) {
		if (!Contains(box)) return false;
		end_for;
	}
	return true;
}

#if 0														// pas encore teste
bool Region::Intersect(const Box& box) const
// *****************************************
{
	if (box.IsPonctual()) return Contains(box.GetCenter());
	if (! GetBoundingBox().Intersect(box)) return false;
	if (! Region_TilesUnder (this, Box(box).Inflate(1))
			           .GetSubSet(! Tile::GetIsVoidFilter()).IsEmpty()) return true;
	return false;
}

bool Region::Intersect(const Region& region) const
// ***********************************************
{
	for_each_box(box, region.GetBoxesUnder(GetBoundingBox())) {
		if (Intersect(box)) return true;
		end_for;
	}
	return false;
}
#endif

Region& Region::Clear()
// ********************
{
	stack<Tile*> tileStack;

	Tile* tile = _bottomRightTile;
	while (tile) {
		tileStack.push(tile);
		tile = tile->_leftTile;
	}
	// AD (11Aug03) to avoid delete Tiles in the while loop, because
	// topTile->_leftTile may have been already deleted
	vector<Tile*> accumulate;

	while (!tileStack.empty()) {
		tile = tileStack.top();
		tileStack.pop();
		Unit xMin = tile->GetXMin();
		Unit xMax = tile->GetXMax();

		Tile* topTile = tile->_topTile;
		while (topTile && (xMin < topTile->GetXMax())) {
			if (topTile->GetXMax() <= xMax) tileStack.push(topTile);
			topTile = topTile->_leftTile;
		}
		accumulate.push_back (tile);
		//delete tile;
	}
	for_each_object (Tile*, t, GetCollection (accumulate)) {
		delete t;
		end_for;
	}
	_bottomRightTile = NULL;
	_topLeftTile = NULL;

	return *this;
}

Region& Region::Fill(const Box& box)
// *********************************
{
	if (box.IsEmpty() || !box.GetWidth() || !box.GetHeight()) return *this;

	if (!_bottomRightTile) {
		_bottomRightTile = new Tile(box);
		_topLeftTile = _bottomRightTile;
		return *this;
	}

	if (!GetBoundingBox().Contains(box))
		_Update(box, false);
	else {
		Tile* startTile = _GetStartTile(_GetTileAt(Point(box.GetXMax(), box.GetYMin())));
		GenericCollection<Tile*> tiles = _GetTilesUnder(Box(box).Inflate(0, 0, -1, -1), startTile);
		if (!tiles.GetSubSet(Tile::GetIsVoidFilter()).IsEmpty()) _Update(box, false, startTile);
	}

	return *this;
}

Region& Region::Fill(const Region& region)
// ***************************************
{
	for_each_box(box, region.GetBoxes()) {
		Fill(box);
		end_for;
	}
	return *this;
}

Region& Region::GetUnion (const Region& region)
// ********************************************
{
	return Fill(region);
}

Region& Region::Groove(const Box& box)
// ***********************************
{
	if (!_bottomRightTile) return *this;

	Box correctedBox = GetBoundingBox().GetIntersection(box);

	if (correctedBox.IsEmpty() || !correctedBox.GetWidth() || !correctedBox.GetHeight()) return *this;

	Tile* startTile = _GetStartTile(_GetTileAt(Point(correctedBox.GetXMax(), correctedBox.GetYMin())));
	GenericCollection<Tile*> tiles = _GetTilesUnder(Box(correctedBox).Inflate(0, 0, -1, -1), startTile);
	if (!tiles.GetSubSet(!Tile::GetIsVoidFilter()).IsEmpty()) _Update(box, true, startTile);

	return *this;
}

Region& Region::Groove(const Region& region)
// *****************************************
{
	Box boundingBox = GetBoundingBox();
	for_each_box(box, region.GetBoxesUnder(boundingBox)) {
		Groove(box);
		end_for;
	}
	return *this;
}

Region& Region::GetIntersection (const Region& region)
// ***************************************************
{
	Box boundingBox = GetBoundingBox();
	for_each_box (box, region.GetVoidBoxesUnder (boundingBox)) {
		//for_each_box (box, region.GetVoidBoxes ()) {
		//if (! boundingBox.Intersect (box)) continue;
		Groove (box);
		end_for;
	}
	return *this;
}

Region& Region::Inflate(const Unit& quantity)
// ******************************************
{
	if (!IsEmpty()) {
		if (0 < quantity) {
			list<Box> boxList;
			for_each_object(Tile*, tile, Region_Tiles(this).GetSubSet(!Tile::GetIsVoidFilter())) {
				boxList.push_back(tile->GetBoundingBox());
				end_for;
			}
			for_each_box(box, GetCollection(boxList)) {
				Fill(box.Inflate(quantity));
				end_for;
			}
		}
		else if (quantity < 0) {
			_GrowthToFit(GetBoundingBox().Inflate(GetUnit(1)));
			list<Box> boxList;
			for_each_object(Tile*, tile, Region_Tiles(this).GetSubSet(Tile::GetIsVoidFilter())) {
				boxList.push_back(tile->GetBoundingBox());
				end_for;
			}
			for_each_box(box, GetCollection(boxList)) {
				Groove(box.Inflate(-quantity));
				end_for;
			}
		}
	}

	return *this;
}

Region& Region::Translate(const Unit& dx, const Unit& dy)
// ******************************************************
{
	if ((dx != 0) || (dy != 0)) {
		set<Tile*> tileSet;
		_GetTiles().Fill(tileSet);
		for_each_object(Tile*, tile, GetCollection(tileSet)) {
			tile->_boundingBox.Translate(dx, dy);
			end_for;
		}
	}
	return *this;
}

string Region::_GetString() const
// ******************************
{
	string s = "<" + _GetTypeName() + ">";
	return s;
}

Record* Region::_GetRecord() const
// *************************
{
	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("BottomRightTile", _bottomRightTile));
		record->Add(GetSlot("TopLeftTile", _topLeftTile));
	}
	return record;
}

Region_Tile* Region::_GetTileAt(const Point& point, Tile* startTile) const
// ***********************************************************************
{
	if (!GetBoundingBox().Contains(point)) return NULL;

	return (startTile) ? startTile->GetTileAt(point) : _bottomRightTile->GetTileAt(point);
}

Region_Tile* Region::_GetNonVoidTileAt(const Point& point, Tile* startTile) const
// ******************************************************************************
{
	if (!GetBoundingBox().Contains(point)) return NULL;

	return (startTile) ?  startTile->GetNonVoidTileAt(point) : _bottomRightTile->GetNonVoidTileAt(point);
}

Region_Tile* Region::_GetStartTile(Tile* tile) const
// *************************************************
{
	Tile* startTile = NULL;

	if (tile) {
		if (!startTile && tile->_rightTile) {
			Unit y = tile->GetYMin();
			startTile = tile->_rightTile;
			while (startTile && (y <= startTile->GetYMax())) startTile = startTile->_bottomTile;
		}
		if (!startTile && tile->_bottomTile) {
			Unit x = tile->GetXMax();
			startTile = tile->_bottomTile;
			while (startTile && (startTile->GetXMin() <= x)) startTile = startTile->_rightTile;
		}
	}

	return startTile;
}

GenericCollection<Region_Tile*> Region::_GetTiles() const
// ******************************************************
{
	return Region_Tiles(this);
}

GenericCollection<Region_Tile*> Region::_GetTilesUnder(const Box& area, Tile* startTile) const
// *******************************************************************************************
{
	return Region_TilesUnder(this, area, startTile);
}

void Region::_Split(const Box& box)
// ********************************
{
	if (GetBoundingBox().Intersect(box)) {

		Tile* startTile = _GetStartTile(_GetTileAt(Point(box.GetXMax(), box.GetYMin())));

		list<Tile*> tileList;
		Box line = Box(box.GetXMin(), box.GetYMin(), box.GetXMax() - 1, box.GetYMin());
		_GetTilesUnder(line, startTile).Fill(tileList);
		for_each_object(Tile*, tile, GetCollection(tileList)) {
			tile->SplitHorizontal(this, box.GetYMin());
			end_for;
		}

		tileList.clear();
		line = Box(box.GetXMin(), box.GetYMax(), box.GetXMax() - 1, box.GetYMax());
		_GetTilesUnder(line, startTile).Fill(tileList);
		for_each_object(Tile*, tile, GetCollection(tileList)) {
			tile->SplitHorizontal(this, box.GetYMax());
			end_for;
		}

		tileList.clear();
		line = Box(box.GetXMin(), box.GetYMin(), box.GetXMin(), box.GetYMax() - 1);
		_GetTilesUnder(line, startTile).Fill(tileList);
		for_each_object(Tile*, tile, GetCollection(tileList)) {
			tile->SplitVertical(this, box.GetXMin());
			end_for;
		}

		tileList.clear();
		line = Box(box.GetXMax(), box.GetYMin(), box.GetXMax(), box.GetYMax() - 1);
		_GetTilesUnder(line, startTile).Fill(tileList);
		for_each_object(Tile*, tile, GetCollection(tileList)) {
			tile->SplitVertical(this, box.GetXMax());
			end_for;
		}
	}
}

void Region::_GrowthToFit(const Box& box)
// **************************************
{
	if (box.IsEmpty()) return;

	if (!_bottomRightTile) {
		_bottomRightTile = new Tile(box, true);
		_topLeftTile = _bottomRightTile;
		return;
	}

	if (GetBoundingBox().Contains(box)) return;

	if (box.GetYMin() < GetYMin()) {
		Tile* newTile = new Tile(Box(GetXMin(), box.GetYMin(), GetXMax(), GetYMin()), true);
		Tile* tile = _bottomRightTile;
		while (tile) {
			tile->_bottomTile = newTile;
			tile = tile->_leftTile;
		}
		newTile->_topTile = _bottomRightTile;
		_bottomRightTile = newTile;
	}

	if (GetYMax() < box.GetYMax()) {
		Tile* newTile = new Tile(Box(GetXMin(), GetYMax(), GetXMax(), box.GetYMax()), true);
		Tile* tile = _topLeftTile;
		while (tile) {
			tile->_topTile = newTile;
			tile = tile->_rightTile;
		}
		newTile->_bottomTile = _topLeftTile;
		_topLeftTile = newTile;
	}

	if (box.GetXMin() < GetXMin()) {
		Tile* newTile = new Tile(Box(box.GetXMin(), GetYMin(), GetXMin(), GetYMax()), true);
		Tile* tile = _topLeftTile;
		while (tile) {
			tile->_leftTile = newTile;
			tile = tile->_bottomTile;
		}
		newTile->_rightTile = _topLeftTile;
		_topLeftTile = newTile;
	}

	if (GetXMax() < box.GetXMax()) {
		Tile* newTile = new Tile(Box(GetXMax(), GetYMin(), box.GetXMax(), GetYMax()), true);
		Tile* tile = _bottomRightTile;
		while (tile) {
			tile->_rightTile = newTile;
			tile = tile->_topTile;
		}
		newTile->_leftTile = _bottomRightTile;
		_bottomRightTile = newTile;
	}
}

void Region::_Update(const Box& box, bool isVoid, Tile* startTile)
// **************************************************************
{
	if (box.IsEmpty() || !_bottomRightTile || !box.GetWidth() || !box.GetHeight()) return;

	if (!GetBoundingBox().Contains(box)) _GrowthToFit(box);

	_Split(box);

	Tile* newTile = new Tile(box, isVoid);

	list<Tile*> tileList;
	_GetTilesUnder(Box(box).Inflate(0, 0, -1, -1), startTile).Fill(tileList);
	for_each_object(Tile*, tile, GetCollection(tileList)) {
		if (_topLeftTile == tile) _topLeftTile = newTile;
		if (_bottomRightTile == tile) _bottomRightTile = newTile;
		if (tile->GetXMin() == box.GetXMin()) {
			if (tile->GetYMin() == box.GetYMin()) {
				newTile->_leftTile = tile->_leftTile;
				newTile->_bottomTile = tile->_bottomTile;
			}
			Tile* leftTile = tile->_leftTile;
			while (leftTile && (leftTile->_rightTile == tile)) {
				leftTile->_rightTile = newTile;
				leftTile = leftTile->_topTile;
			}
		}
		if (tile->GetYMin() == box.GetYMin()) {
			Tile* bottomTile = tile->_bottomTile;
			while (bottomTile && (bottomTile->_topTile == tile)) {
				bottomTile->_topTile = newTile;
				bottomTile = bottomTile->_rightTile;
			}
		}
		if (tile->GetYMax() == box.GetYMax()) {
			if (tile->GetXMax() == box.GetXMax()) {
				newTile->_topTile = tile->_topTile;
				newTile->_rightTile = tile->_rightTile;
			}
			Tile* topTile = tile->_topTile;
			while (topTile && (topTile->_bottomTile == tile)) {
				topTile->_bottomTile = newTile;
				topTile = topTile->_leftTile;
			}
		}
		if (tile->GetXMax() == box.GetXMax()) {
			Tile* rightTile = tile->_rightTile;
			while (rightTile && (rightTile->_leftTile == tile)) {
				rightTile->_leftTile = newTile;
				rightTile = rightTile->_bottomTile;
			}
		}
		end_for;
	}
	for_each_object(Tile*, tile, GetCollection(tileList)) {
		delete tile;
		end_for;
	}

	newTile->CleanNeighbours(this);
	newTile->MergeNeighbours(this);
};				// Region::_Update

bool 
Region::VerticalEnhancement()
// **************************
{
  bool modif = false;
  Region result;
  double minArea = 1.*GetUnit(5)*GetUnit(5);
  do {
    // Rechercher la box de plus grande surface
    Box maxBox = Box();
    double area = minArea;
    for_each_box (box, GetBoxes()) {
      if (! box.IsEmpty()) {
				double a = 1. * box.GetWidth() * box.GetHeight();
				if (area < a) {
					area = a;
					maxBox = box;
				}
      }
      end_for;
    }
    if (maxBox.IsEmpty()) break;
    Tile* tile = _GetTileAt (maxBox.GetCenter());
    if (maxBox.GetWidth() >= GetUnit(2)) {
      modif = tile->VerticalEnhancement (this);
    }
    result.Fill (tile->GetBoundingBox());
    Groove (tile->GetBoundingBox());
  } while (! IsEmpty());
  Fill (result);
  return modif;
};				// Region::VerticalEnhancement


bool 
Region::VerticalEnhancement(Point point)
// ***************************************
// Amelioration de la tuile contenant point
{
	bool modif = false;
	Tile* tile = _GetTileAt (point);
	if (tile) modif = tile->VerticalEnhancement (this);
	return modif;
};				// Region::VerticalEnhancement


bool 
Region::HorizontalEnhancement()
// ****************************
{
  bool modif = false;
  Region result;
  double minArea = 1.*GetUnit(5)*GetUnit(5);
  do {
    // Rechercher la box de plus grande surface
    Box maxBox = Box();
    double area = minArea;
    for_each_box (box, GetBoxes()) {
      if (! box.IsEmpty()) {
				double a = 1. * box.GetWidth() * box.GetHeight();
				if (area < a) {
					area = a;
					maxBox = box;
				}
      }
      end_for;
    }
    if (maxBox.IsEmpty()) break;
    Tile* tile = _GetTileAt (maxBox.GetCenter());
    if (maxBox.GetWidth() >= GetUnit(2)) {
      modif = tile->HorizontalEnhancement (this);
    }
    result.Fill (tile->GetBoundingBox());
    Groove (tile->GetBoundingBox());
  } while (! IsEmpty());
  Fill (result);
  return modif;
};				// Region::HorizontalEnhancement

bool 
Region::HorizontalEnhancement(Point point)
// ***************************************
// Amelioration de la tuile contenant point
{
	bool modif = false;
	Tile* tile = _GetTileAt (point);
  if (tile) modif = tile->HorizontalEnhancement (this);
	return modif;
};				// Region::HorizontalEnhancement


Interval 
Region::TopBottomFacing (const Box box) const
// **********************************************
// Retourne le Merge des intervales en vis a vis entre les boites
// dessus et dessous de box
{
  Interval result = Interval();
  if (box.IsEmpty()) return result;
  Tile* tile = _GetTileAt (box.GetCenter());
  Interval it1 = tile->_GetTopNeighbour ();
  Interval it2 = tile->_GetBottomNeighbour ();
  return it1.GetIntersection (it2);
};				// Region::TopBottomFacing

Interval 
Region::LeftRightFacing (const Box box) const
// **********************************************
// Retourne le Merge des intervales en vis a vis entre les boites
// a gauche et droite de box
{
  Interval result = Interval();
  if (box.IsEmpty()) return result;
  Tile* tile = _GetTileAt (box.GetCenter());
  Interval it1 = tile->_GetLeftNeighbour ();
  Interval it2 = tile->_GetRightNeighbour ();
  return it1.GetIntersection (it2);
};				// Region::LeftRightFacing



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
