// ****************************************************************************************************
// File: Region.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Region.h"
#include "hurricane/Error.h"


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
        string msg = "Region::getIntersection avant Groove";
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

        public: virtual Filter<Region_Tile*>* getClone() const;

        public: virtual bool accept(Region_Tile* tile) const;

        public: virtual string _getString() const;

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

    public: const Box& getBoundingBox() const {return _boundingBox;};

    public: const DbU::Unit& getXMin() const {return _boundingBox.getXMin();};
    public: const DbU::Unit& getYMin() const {return _boundingBox.getYMin();};
    public: const DbU::Unit& getXMax() const {return _boundingBox.getXMax();};
    public: const DbU::Unit& getYMax() const {return _boundingBox.getYMax();};

    public: Region_Tile* getTileAt(const Point& point) const;
    public: Region_Tile* getNonVoidTileAt(const Point& point) const;

    public: Region_Tile* getLeftTile(const DbU::Unit& y) const;
    public: Region_Tile* getBottomTile(const DbU::Unit& x) const;
    public: Region_Tile* getTopTile(const DbU::Unit& x) const;
    public: Region_Tile* getRightTile(const DbU::Unit& y) const;

// Filters
// *******

    public: static GenericFilter<Region_Tile*> getIsVoidFilter();

// Predicates
// **********

    public: bool isVoid() const {return _isVoid;};
    public: bool contains(const Point& point) const;

// Updators
// ********

    public: void splitVertical(Region* region, const DbU::Unit& x);
    public: void splitHorizontal(Region* region, const DbU::Unit& y);
    public: bool mergeLeftTile(Region* region);
    public: bool mergeBottomTile(Region* region);
    public: bool mergeTopTile(Region* region);
    public: bool mergeRightTile(Region* region);
    public: void mergeNeighbours(Region* region);
    public: void CleanNeighbours(Region* region);
    public: bool VerticalEnhancement(Region* region);
    public: bool HorizontalEnhancement(Region* region);

// Others
// ******

    public: string _getTypeName() const { return _TName("Region_Tile"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    private: bool _IsTopFull() const;
    private: bool _IsBottomFull() const;
    private: bool _IsLeftFull() const;
    private: bool _IsRightFull() const;
    private: DbU::Unit _TopSplitAtVerticalSize (Region* region);
    private: DbU::Unit _BottomSplitAtVerticalSize (Region* region);
    private: DbU::Unit _LeftSplitAtHorizontalSize (Region* region);
    private: DbU::Unit _RightSplitAtHorizontalSize (Region* region);
    private: bool _TopSplitAtHorizontalSize (Region* region, const DbU::Unit height);
    private: bool _BottomSplitAtHorizontalSize (Region* region, const DbU::Unit height);
    private: bool _LeftSplitAtVerticalSize (Region* region, const DbU::Unit width);
    private: bool _RightSplitAtVerticalSize (Region* region, const DbU::Unit width);

    public: Interval _getTopNeighbour () const;
    public: Interval _getBottomNeighbour () const;
    public: Interval _getLeftNeighbour () const;
    public: Interval _getRightNeighbour () const;
};                // class Region_Tile



// ****************************************************************************************************
// Region_Tile implementation
// ****************************************************************************************************

Region_Tile::Region_Tile(const Box& boundingBox, bool isVoid)
// **********************************************************
:    _boundingBox(boundingBox),
    _isVoid(isVoid),
    _leftTile(NULL),
    _bottomTile(NULL),
    _topTile(NULL),
    _rightTile(NULL)
{
    if (boundingBox.isEmpty())
        throw Error("Can't create " + _TName("Region::Tile") + " : empty bounding box");
}

Region_Tile::~Region_Tile()
// ************************
{
}

bool Region_Tile::contains(const Point& point) const
// *************************************************
{
    return ((getXMin() <= point.getX()) &&
              (getYMin() <= point.getY()) &&
              ((point.getX() < getXMax()) || (!_rightTile && (point.getX() == getXMax()))) &&
              ((point.getY() < getYMax()) || (!_topTile && (point.getY() == getYMax()))));
}

Region_Tile* Region_Tile::getTileAt(const Point& point) const
// **********************************************************
{
    if (contains(point)) return (Region_Tile*)this;

    if (_topTile && (getYMax() <= point.getY())) return _topTile->getTileAt(point);

    if (_leftTile && (point.getX() < getXMin())) return _leftTile->getTileAt(point);

    return NULL;
}

Region_Tile* Region_Tile::getNonVoidTileAt(const Point& point) const
// *****************************************************************
{
    if (contains(point)) {
        if (!_isVoid) return (Region_Tile*)this;
        if ((getXMin() < point.getX()) && (getYMin() < point.getY())) return NULL;
    }

    if (_topTile && (getYMax() <= point.getY())) return _topTile->getNonVoidTileAt(point);

    if (_leftTile && (point.getX() <= getXMin())) return _leftTile->getNonVoidTileAt(point);

    return NULL;
}

Region_Tile* Region_Tile::getLeftTile(const DbU::Unit& y) const
// *******************************************************
{
    Region_Tile* tile = _leftTile;
    while (tile && (tile->getYMax() <= y)) tile = tile->_topTile;
    return tile;
}

Region_Tile* Region_Tile::getBottomTile(const DbU::Unit& x) const
// *********************************************************
{
    Region_Tile* tile = _bottomTile;
    while (tile && (tile->getXMax() <= x)) tile = tile->_rightTile;
    return tile;
}

Region_Tile* Region_Tile::getTopTile(const DbU::Unit& x) const
// ******************************************************
{
    Region_Tile* tile = _topTile;
    while (tile && (x < tile->getXMin())) tile = tile->_leftTile;
    return tile;
}

Region_Tile* Region_Tile::getRightTile(const DbU::Unit& y) const
// ********************************************************
{
    Region_Tile* tile = _rightTile;
    while (tile && (y < tile->getYMin())) tile = tile->_bottomTile;
    return tile;
}

GenericFilter<Region_Tile*> Region_Tile::getIsVoidFilter()
// *******************************************************
{
    return Region_Tile::IsVoidFilter();
}

void Region_Tile::splitVertical(Region* region, const DbU::Unit& x)
// ***********************************************************
{
    if ((getXMin() < x) && (x < getXMax())) {
        Region_Tile* newTile = new Region_Tile(Box(x, getYMin(), getXMax(), getYMax()), isVoid());

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
            if (x < tile->getXMax()) tile->_topTile = newTile;
            tile = tile->_rightTile;
        }
        if (_bottomTile) {
            newTile->_bottomTile = _bottomTile;
            while (newTile->_bottomTile->getXMax() <= newTile->getXMin())
                newTile->_bottomTile = newTile->_bottomTile->_rightTile;
        }

        newTile->_topTile = _topTile;
        while (_topTile && (_topTile->_bottomTile == this)) {
            if (_topTile->getXMin() < x) break;
            _topTile->_bottomTile = newTile;
            _topTile = _topTile->_leftTile;
        }

        _boundingBox = Box(getXMin(), getYMin(), x, getYMax());

        if (region->_getBottomRightTile() == this) region->_setBottomRightTile(newTile);
    }
}

void Region_Tile::splitHorizontal(Region* region, const DbU::Unit& y)
// *************************************************************
{
    if ((getYMin() < y) && (y < getYMax())) {
        Region_Tile* newTile = new Region_Tile(Box(getXMin(), y, getXMax(), getYMax()), isVoid());

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
            if (y < tile->getYMax()) tile->_rightTile = newTile;
            tile = tile->_topTile;
        }
        if (_leftTile) {
            newTile->_leftTile = _leftTile;
            while (newTile->_leftTile->getYMax() <= newTile->getYMin())
                newTile->_leftTile = newTile->_leftTile->_topTile;
        }

        newTile->_rightTile = _rightTile;
        while (_rightTile && (_rightTile->_leftTile == this)) {
            if (_rightTile->getYMin() < y) break;
            _rightTile->_leftTile = newTile;
            _rightTile = _rightTile->_bottomTile;
        }

        _boundingBox = Box(getXMin(), getYMin(), getXMax(), y);

        if (region->_getTopLeftTile() == this) region->_setTopLeftTile(newTile);
    }
}

bool Region_Tile::mergeLeftTile(Region* region)
// ********************************************
{
    Region_Tile* uselessTile = _leftTile;

    if (!uselessTile) return false;
    if (uselessTile->_rightTile != this) return false;
    if (uselessTile->_isVoid != _isVoid) return false;
    if (uselessTile->getXMax() != getXMin()) return false;
    if (uselessTile->getYMin() != getYMin()) return false;
    if (uselessTile->getYMax() != getYMax()) return false;

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

    _boundingBox.merge(uselessTile->_boundingBox);

    if (region->_getTopLeftTile() == uselessTile) region->_setTopLeftTile(this);

    delete uselessTile;

    return true;
}

bool Region_Tile::mergeBottomTile(Region* region)
// **********************************************
{
    Region_Tile* uselessTile = _bottomTile;

    if (!uselessTile) return false;
    if (uselessTile->_topTile != this) return false;
    if (uselessTile->_isVoid != _isVoid) return false;
    if (uselessTile->getYMax() != getYMin()) return false;
    if (uselessTile->getXMin() != getXMin()) return false;
    if (uselessTile->getXMax() != getXMax()) return false;

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

    _boundingBox.merge(uselessTile->_boundingBox);

    if (region->_getBottomRightTile() == uselessTile) region->_setBottomRightTile(this);

    delete uselessTile;

    return true;
}

bool Region_Tile::mergeTopTile(Region* region)
// *******************************************
{
    Region_Tile* uselessTile = _topTile;

    if (!uselessTile) return false;
    if (uselessTile->_bottomTile != this) return false;
    if (uselessTile->_isVoid != _isVoid) return false;
    if (uselessTile->getYMin() != getYMax()) return false;
    if (uselessTile->getXMin() != getXMin()) return false;
    if (uselessTile->getXMax() != getXMax()) return false;

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

    _boundingBox.merge(uselessTile->_boundingBox);

    if (region->_getTopLeftTile() == uselessTile) region->_setTopLeftTile(this);

    delete uselessTile;

    return true;
}

bool Region_Tile::mergeRightTile(Region* region)
// *********************************************
{
    Region_Tile* uselessTile = _rightTile;

    if (!uselessTile) return false;
    if (uselessTile->_leftTile != this) return false;
    if (uselessTile->_isVoid != _isVoid) return false;
    if (uselessTile->getXMin() != getXMax()) return false;
    if (uselessTile->getYMin() != getYMin()) return false;
    if (uselessTile->getYMax() != getYMax()) return false;

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

    _boundingBox.merge(uselessTile->_boundingBox);

    if (region->_getBottomRightTile() == uselessTile) region->_setBottomRightTile(this);

    delete uselessTile;

    return true;
}

void Region_Tile::mergeNeighbours(Region* region)
// **********************************************
{
    while (mergeLeftTile(region) || mergeTopTile(region) ||
               mergeBottomTile(region) || mergeRightTile(region)) {
    }
}

void Region_Tile::CleanNeighbours(Region* region)
// **********************************************
{
    Region_Tile* tile = NULL;

    set<Region_Tile*> tileSet;

    tile = _leftTile;
    while (tile && (tile->getYMin() <= getYMax())) {
        tileSet.insert(tile);
        tile = tile->_topTile;
    }

    tile = _topTile;
    while (tile && (getXMin() <= tile->getXMax())) {
        tileSet.insert(tile);
        tile = tile->_leftTile;
    }

    tile = _bottomTile;
    while (tile && (tile->getXMin() <= getXMax())) {
        tileSet.insert(tile);
        tile = tile->_rightTile;
    }

    tile = _rightTile;
    while (tile && (getYMin() <= tile->getYMax())) {
        tileSet.insert(tile);
        tile = tile->_bottomTile;
    }

    while (!tileSet.empty()) {
        tile = *tileSet.begin();
        while (true) {
            Region_Tile* leftTile = tile->_leftTile;
            if ((leftTile == this) || !tile->mergeLeftTile(region)) break;
            tileSet.erase(leftTile);
        }
        while (true) {
            Region_Tile* topTile = tile->_topTile;
            if ((topTile == this) || !tile->mergeTopTile(region)) break;
            tileSet.erase(topTile);
        }
        while (true) {
            Region_Tile* bottomTile = tile->_bottomTile;
            if ((bottomTile == this) || !tile->mergeBottomTile(region)) break;
            tileSet.erase(bottomTile);
        }
        while (true) {
            Region_Tile* rightTile = tile->_rightTile;
            if ((rightTile == this) || !tile->mergeRightTile(region)) break;
            tileSet.erase(rightTile);
        }
        tileSet.erase(tile);
    }
};                // Region_Tile::CleanNeighbours


bool 
Region_Tile::_IsTopFull() const
// *******************************
// Retourne true si toutes les tuiles sur this sont de meme isVoid,
{
  Region_Tile* upTile = _topTile;
  while (upTile && upTile->_isVoid == _isVoid &&
     upTile->getXMin() > getXMin()) {
    upTile = upTile->_leftTile;
  }
  return (upTile && upTile->_isVoid == _isVoid &&
      upTile->getXMin() <= getXMin());
};                // Region_Tile::_IsTopFull

bool 
Region_Tile::_IsBottomFull() const
// *******************************
// Retourne true si toutes les tuiles sous this sont de meme isVoid,
{
  Region_Tile* downTile = _bottomTile;
  while (downTile && downTile->_isVoid == _isVoid &&
     downTile->getXMax() < getXMax()) {
    downTile = downTile->_rightTile;
  }
  return (downTile && downTile->_isVoid == _isVoid &&
      downTile->getXMax() >= getXMax());
};                // Region_Tile::_IsBottomFull

bool 
Region_Tile::_IsLeftFull() const
// *******************************
// Retourne true si toutes les tuiles a gauche de this sont de meme isVoid,
{
  Region_Tile* leftTile = _leftTile;
  while (leftTile && leftTile->_isVoid == _isVoid &&
     leftTile->getYMax() < getYMax()) {
    leftTile = leftTile->_topTile;
  }
  return (leftTile && leftTile->_isVoid == _isVoid &&
      leftTile->getYMax() >= getYMax());
};                // Region_Tile::_IsLeftFull

bool 
Region_Tile::_IsRightFull() const
// *******************************
// Retourne true si toutes les tuiles a droite de this sont de meme isVoid,
{
  Region_Tile* rightTile = _rightTile;
  while (rightTile && rightTile->_isVoid == _isVoid &&
     rightTile->getYMin() > getYMin()) {
    rightTile = rightTile->_bottomTile;
  }
  return (rightTile && rightTile->_isVoid == _isVoid &&
      rightTile->getYMin() <= getYMin());
};                // Region_Tile::_IsRightFull

DbU::Unit
Region_Tile::_TopSplitAtVerticalSize (Region* region)
// **************************************************
// Decoupe les tuiles top-left et top-right a la taille de this
// Retourne la hauteur minimale de la rangee
// Doit etre appele apres verif _IsTopFull()
{
  Region_Tile* upTile = _topTile;
  DbU::Unit height = upTile->getYMax() - getYMax();
  if (upTile && upTile->getXMax() != getXMax()) {
    upTile->splitVertical (region, getXMax());
    upTile = _topTile;
  }
  DbU::Unit ht;
  while (upTile && upTile->getXMin() > getXMin()) {
    ht = upTile->getYMax() - getYMax();
    if (height > ht) height = ht;
    upTile = upTile->_leftTile;
  }
  ht =  upTile->getYMax() - getYMax();
  if (height > ht) height = ht;
  if (upTile && upTile->getXMin() != getXMin()) {
    upTile->splitVertical (region, getXMin());
  }
  return height;
};                // Region_Tile::_TopSplitAtVerticalSize

DbU::Unit
Region_Tile::_BottomSplitAtVerticalSize (Region* region)
// *****************************************************
// Decoupe les tuiles bottom-left et bottom-right a la taille de this
// Retourne la hauteur minimale de la rangee
// Doit etre appele apres verif _IsBottomFull()
{
  Region_Tile* downTile = _bottomTile;
  DbU::Unit height = getYMin() - downTile->getYMin();
  if (downTile && downTile->getXMin() != getXMin()) {
    downTile->splitVertical (region, getXMin());
    downTile = _bottomTile;
  }
  DbU::Unit ht;
  while (downTile && downTile->getXMax() < getXMax()) {
    ht = getYMin() - downTile->getYMin();
    if (height > ht) height = ht;
    downTile = downTile->_rightTile;
  }
  ht = getYMin() - downTile->getYMin();
  if (height > ht) height = ht;
  if (downTile && downTile->getXMax() != getXMax()) {
    downTile->splitVertical (region, getXMax());
  }
  return height;
};                // Region_Tile::_BottomSplitAtVerticalSize


DbU::Unit
Region_Tile::_LeftSplitAtHorizontalSize (Region* region)
// *****************************************************
// Decoupe les tuiles left-top et left-bottom a la taille de this
// Retourne la largeur minimale de la colonne
// Doit etre appele apres verif _IsLeftFull()
{
  Region_Tile* leftTile = _leftTile;
  DbU::Unit width = getXMin() - leftTile->getXMin();
  if (leftTile && leftTile->getYMin() != getYMin()) {
    leftTile->splitHorizontal (region, getYMin());
    leftTile = _leftTile;
  }
  DbU::Unit wt;
  while (leftTile && leftTile->getYMax() < getYMax()) {
    wt = getXMin() - leftTile->getXMin();
    if (width > wt) width = wt;
    leftTile = leftTile->_topTile;
  }
  wt = getXMin() - leftTile->getXMin();
  if (width > wt) width = wt;
  if (leftTile && leftTile->getYMax() != getYMax()) {
    leftTile->splitHorizontal (region, getYMax());
  }
  return width;
};                // Region_Tile::_LeftSplitAtHorizontalSize


DbU::Unit
Region_Tile::_RightSplitAtHorizontalSize (Region* region)
// ******************************************************
// Decoupe les tuiles right-bottom et right-top a la taille de this
// Retourne la largeur minimale de la colonne
// Doit etre appele apres verif _IsRightFull()
{
  Region_Tile* rightTile = _rightTile;
  DbU::Unit width = rightTile->getXMax() - getXMax();
  if (rightTile && rightTile->getYMax() != getYMax()) {
    rightTile->splitHorizontal (region, getYMax());
    rightTile = _rightTile;
  }
  DbU::Unit wt;
  while (rightTile && rightTile->getYMin() > getYMin()) {
    wt = rightTile->getXMax() - getXMax();
    if (width > wt) width = wt;
    rightTile = rightTile->_bottomTile;
  }
  wt =  rightTile->getXMax() - getXMax();
  if (width > wt) width = wt;
  if (rightTile && rightTile->getYMin() != getYMin()) {
    rightTile->splitHorizontal (region, getYMin());
  }
  return width;
};                // Region_Tile::_RightSplitAtHorizontalSize


bool
Region_Tile::_TopSplitAtHorizontalSize (Region* region, const DbU::Unit height)
// ***********************************************************************
// Retaille la hauteur de toutes les tuiles sur this a la hauteur height
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* upTile = _topTile;

  while (upTile && upTile->getXMin() >= getXMin()) {
    if (upTile->getYMax() - getYMax() > height) {
      upTile->splitHorizontal (region, getYMax() + height);
      if (upTile->_bottomTile != this) upTile = upTile->_bottomTile;
      modif = true;
    }
    if (upTile->_rightTile && upTile->_rightTile->getXMax() == getXMax()) {
      // merge upTile et upTile->_rightTile
      modif = upTile->mergeRightTile (region) || modif;
    }
    upTile = upTile->_leftTile;
  }
  return modif;
};                // Region_Tile::_TopSplitAtHorizontalSize


bool
Region_Tile::_BottomSplitAtHorizontalSize (Region* region, const DbU::Unit height)
// **************************************************************************
// Retaille la hauteur de toutes les tuiles sous this a la hauteur height
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* downTile = _bottomTile;

  while (downTile && downTile->getXMax() <= getXMax()) {
    if (getYMin() - downTile->getYMin() > height) {
      downTile->splitHorizontal (region, getYMin() - height);
      modif = true;
      if (downTile->_topTile != this) downTile = downTile->_topTile;
    }
    if (downTile->_leftTile && downTile->_leftTile->getXMin() == getXMin()) {
      // merge downTile et downTile->_leftTile
      modif = downTile->mergeLeftTile (region) || modif;
    }
    downTile = downTile->_rightTile;
  }
  return modif;
};                // Region_Tile::_BottomSplitAtHorizontalSize

bool
Region_Tile::_LeftSplitAtVerticalSize (Region* region, const DbU::Unit width)
// **************************************************************************
// Retaille la largeur de toutes les tuiles a gauche de this a la largeur width
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* leftTile = _leftTile;

  while (leftTile && leftTile->getYMax() <= getYMax()) {
    if (getXMin() - leftTile->getXMin() > width) {
      leftTile->splitVertical (region, getXMin() - width);
      modif = true;
      if (leftTile->_rightTile != this) leftTile = leftTile->_rightTile;
    }
    if (leftTile->_bottomTile && leftTile->_bottomTile->getYMin() == getYMin()) {
      // merge leftTile et leftTile->_bottomTile
      modif = leftTile->mergeBottomTile (region) || modif;
    }
    leftTile = leftTile->_topTile;
  }
  return modif;
};                // Region_Tile::_LeftSplitAtVerticalSize


bool
Region_Tile::_RightSplitAtVerticalSize (Region* region, const DbU::Unit width)
// **************************************************************************
// Retaille la hauteur de toutes les tuiles a droite de this a la largeur width
// Retourne true si une modif a ete faite
{
  bool modif = false;
  Region_Tile* rightTile = _rightTile;

  while (rightTile && rightTile->getYMin() >= getYMin()) {
    if (rightTile->getXMax() - getXMax() > width) {
      rightTile->splitVertical (region, getXMax() + width);
      modif = true;
      if (rightTile->_leftTile != this) rightTile = rightTile->_leftTile;
    }
    if (rightTile->_topTile && rightTile->_topTile->getYMax() == getYMax()) {
      // merge rightTile et rightTile->_leftTile
      modif = rightTile->mergeTopTile (region) || modif;
    }
    rightTile = rightTile->_bottomTile;
  }
  return modif;
};                // Region_Tile::_RightSplitAtVerticalSize


Interval 
Region_Tile::_getTopNeighbour () const
// ***********************************
// Retourne le merge des intervals de voisins sur this
{
  Interval result = Interval();
  Region_Tile* topTile = _topTile;
  while (topTile && topTile->getXMax() > getXMin()) {
    if (topTile->_isVoid == _isVoid) {
      result.merge (Interval (topTile->getXMin(), topTile->getXMax()));
    }
    topTile = topTile->_leftTile;
  }
  return result.getIntersection (Interval (getXMin(), getXMax()));
};                // Region_Tile::_getTopNeighbour

Interval 
Region_Tile::_getBottomNeighbour () const
// ***********************************
// Retourne le merge des intervals de voisins sur this
{
  Interval result = Interval();
  Region_Tile* bottomTile = _bottomTile;
  while (bottomTile && bottomTile->getXMin() < getXMax()) {
    if (bottomTile->_isVoid == _isVoid) {
      result.merge (Interval (bottomTile->getXMin(), bottomTile->getXMax()));
    }
    bottomTile = bottomTile->_rightTile;
  }
  return result.getIntersection (Interval (getXMin(), getXMax()));
};                // Region_Tile::_getBottomNeighbour

Interval 
Region_Tile::_getLeftNeighbour () const
// ************************************
// Retourne le merge des intervals de voisins de gauche de this
{
  Interval result = Interval();
  Region_Tile* leftTile = _leftTile;
  while (leftTile && leftTile->getYMin() < getYMax()) {
    if (leftTile->_isVoid == _isVoid) {
      result.merge (Interval (leftTile->getYMin(), leftTile->getYMax()));
    }
    leftTile = leftTile->_topTile;
  }
  return result.getIntersection (Interval (getYMin(), getYMax()));
};                // Region_Tile::_getLeftNeighbour

Interval 
Region_Tile::_getRightNeighbour () const
// *************************************
// Retourne le merge des intervals de voisins de droite de this
{
  Interval result = Interval();
  Region_Tile* rightTile = _rightTile;
  while (rightTile && rightTile->getYMax() > getYMin()) {
    if (rightTile->_isVoid == _isVoid) {
      result.merge (Interval (rightTile->getYMin(), rightTile->getYMax()));
    }
    rightTile = rightTile->_bottomTile;
  }
  return result.getIntersection (Interval (getYMin(), getYMax()));
};                // Region_Tile::_getRightNeighbour

bool
Region_Tile::VerticalEnhancement(Region* region) 
// *********************************************  
{
  bool modif = false;

  while (_IsTopFull ()) {
    DbU::Unit height = _TopSplitAtVerticalSize (region);
    modif = _TopSplitAtHorizontalSize (region, height);
    modif = mergeTopTile (region) || modif;
  }
  while (_IsBottomFull ()) {
    DbU::Unit height = _BottomSplitAtVerticalSize (region);
    modif = _BottomSplitAtHorizontalSize (region, height);
    modif = mergeBottomTile (region) || modif;
  }
  return modif;
};                // Region_Tile::VerticalEnhancement

bool
Region_Tile::HorizontalEnhancement(Region* region) 
// ***********************************************  
{
  bool modif = false;

  while (_IsLeftFull ()) {
    DbU::Unit width = _LeftSplitAtHorizontalSize (region);
    modif = _LeftSplitAtVerticalSize (region, width);
    modif = mergeLeftTile (region) || modif;
  }
  while (_IsRightFull ()) {
    DbU::Unit width = _RightSplitAtHorizontalSize (region);
    modif = _RightSplitAtVerticalSize (region, width);
    modif = mergeRightTile (region) || modif;
  }
  return modif;
};                // Region_Tile::HorizontalEnhancement


string Region_Tile::_getString() const
// ***********************************
{
    string s = "<" + _TName("Region::Tile") + ">";
    s.insert(s.length() - 1, " " + getString(_boundingBox));
    if (_isVoid) s.insert(s.length() - 1, " VOID");
    return s;
};                // ion_Tile::_get

Record* Region_Tile::_getRecord() const
// ******************************
{
    Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("BoundingBox", &_boundingBox));
        record->add(getSlot("IsVoid", &_isVoid));
        record->add(getSlot("LeftTile", _leftTile));
        record->add(getSlot("BottomTile", _bottomTile));
        record->add(getSlot("TopTile", _topTile));
        record->add(getSlot("RightTile", _rightTile));
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

Filter<Region_Tile*>* Region_Tile::IsVoidFilter::getClone() const
// **************************************************************
{
    return new Region_Tile::IsVoidFilter(*this);
}

bool Region_Tile::IsVoidFilter::accept(Region_Tile* tile) const
// ************************************************************
{
    return tile->isVoid();
}

string Region_Tile::IsVoidFilter::_getString() const
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

        public: virtual Region_Tile* getElement() const;
        public: virtual Hurricane::Locator<Region_Tile*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Region_Tile*>* getClone() const;
    public: virtual Hurricane::Locator<Region_Tile*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Region_Tiles implementation
// ****************************************************************************************************

Region_Tiles::Region_Tiles(const Region* region)
// *********************************************
:     Inherit(),
    _region(region)
{
}

Region_Tiles::Region_Tiles(const Region_Tiles& tiles)
// **************************************************
:     Inherit(),
    _region(tiles._region)
{
}

Region_Tiles& Region_Tiles::operator=(const Region_Tiles& tiles)
// *************************************************************
{
    _region = tiles._region;
    return *this;
}

Collection<Region_Tile*>* Region_Tiles::getClone() const
// *****************************************************
{
    return new Region_Tiles(*this);
}

Locator<Region_Tile*>* Region_Tiles::getLocator() const
// ****************************************************
{
    return new Locator(_region);
}

string Region_Tiles::_getString() const
// ************************************
{
    string s = "<" + _TName("Region::Tiles");
    if (_region) s += " " + getString(_region);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region_Tiles::Locator implementation
// ****************************************************************************************************

Region_Tiles::Locator::Locator(const Region* region)
// *************************************************
:    Inherit(),
    _region(region),
    _tileStack()
{
    if (_region) {
        Region_Tile* tile = _region->_getBottomRightTile();
        while (tile) {
            _tileStack.push(tile);
            tile = tile->_leftTile;
        }
    }
}

Region_Tiles::Locator::Locator(const Locator& locator)
// ***************************************************
:    Inherit(),
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

Region_Tile* Region_Tiles::Locator::getElement() const
// ***************************************************
{
    return (!_tileStack.empty()) ? _tileStack.top() : NULL;
}

Locator<Region_Tile*>* Region_Tiles::Locator::getClone() const
// ***********************************************************
{
    return new Locator(*this);
}

bool Region_Tiles::Locator::isValid() const
// ***************************************
{
    return !_tileStack.empty();
}

void Region_Tiles::Locator::progress()
// **********************************
{
    if (!_tileStack.empty()) {
        Region_Tile* tile = _tileStack.top();
        _tileStack.pop();
        DbU::Unit xMin = tile->getXMin();
        DbU::Unit xMax = tile->getXMax();
        Region_Tile* topTile = tile->_topTile;
        while (topTile && (xMin < topTile->getXMax())) {
            if (topTile->getXMax() <= xMax) _tileStack.push(topTile);
            topTile = topTile->_leftTile;
        }
    }
}

string Region_Tiles::Locator::_getString() const
// *********************************************
{
    string s = "<" + _TName("Region::Tiles::Locator");
    if (_region) s += " " + getString(_region);
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

        public: virtual Region_Tile* getElement() const;
        public: virtual Hurricane::Locator<Region_Tile*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Region_Tile*>* getClone() const;
    public: virtual Hurricane::Locator<Region_Tile*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Region_TilesUnder implementation
// ****************************************************************************************************

Region_TilesUnder::Region_TilesUnder()
// ***********************************
:     Inherit(),
    _region(NULL),
    _area(),
    _startTile(NULL)
{
}

Region_TilesUnder::Region_TilesUnder(const Region* region, const Box& area, Region_Tile* startTile)
// ************************************************************************************************
:     Inherit(),
    _region(region),
    _startTile(startTile)
{
    _area = area.getIntersection(region->getBoundingBox());
}

Region_TilesUnder::Region_TilesUnder(const Region_TilesUnder& tiles)
// *****************************************************************
:     Inherit(),
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

Collection<Region_Tile*>* Region_TilesUnder::getClone() const
// **********************************************************
{
    return new Region_TilesUnder(*this);
}

Locator<Region_Tile*>* Region_TilesUnder::getLocator() const
// *********************************************************
{
    return new Locator(_region, _area, _startTile);
}

string Region_TilesUnder::_getString() const
// *****************************************
{
    string s = "<" + _TName("Region::TilesUnder");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
        s += " " + getString(_startTile);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region_TilesUnder::Locator implementation
// ****************************************************************************************************

Region_TilesUnder::Locator::Locator()
// **********************************
:    Inherit(),
    _region(NULL),
    _area(),
    _startTile(NULL),
    _tileStack()
{
}

Region_TilesUnder::Locator::Locator(const Region* region, const Box& area, Region_Tile* startTile)
// ***********************************************************************************************
:    Inherit(),
    _region(region),
    _area(area),
    _startTile(startTile),
    _tileStack()
{
    if (_region && !_area.isEmpty()) {
        Region_Tile* tile = _region->_getTileAt(Point(_area.getXMax(), _area.getYMin()), _startTile);
        while (tile && (_area.getXMin() < tile->getXMax())) {
            while (tile && (tile->getYMax() <= _area.getYMin())) tile = tile->_topTile;
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
:    Inherit(),
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

Region_Tile* Region_TilesUnder::Locator::getElement() const
// ********************************************************
{
    return (!_tileStack.empty()) ? _tileStack.top() : NULL;
}

Locator<Region_Tile*>* Region_TilesUnder::Locator::getClone() const
// ****************************************************************
{
    return new Locator(*this);
}

bool Region_TilesUnder::Locator::isValid() const
// *********************************************
{
    return !_tileStack.empty();
}

void Region_TilesUnder::Locator::progress()
// ****************************************
{
    if (!_tileStack.empty()) {
        Region_Tile* tile = _tileStack.top();
        _tileStack.pop();
        DbU::Unit xMin = max(tile->getXMin(), _area.getXMin());
        DbU::Unit xMax = tile->getXMax();
        Region_Tile* topTile = tile->_topTile;
        while (topTile && (xMin < topTile->getXMax())) {
            if ((topTile->getXMin() <= _area.getXMax()) &&
                    (topTile->getYMin() <= _area.getYMax()) &&
                    (min(topTile->getXMax(), _area.getXMax()) <= xMax)) { 
                if (_tileSet.find(topTile) == _tileSet.end()) {
                    _tileSet.insert(topTile);
                    _tileStack.push(topTile);
                }
            }
            topTile = topTile->_leftTile;
        }
    }
}

string Region_TilesUnder::Locator::_getString() const
// **************************************************
{
    string s = "<" + _TName("Region::TilesUnder::Locator");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
        s += " " + getString(_startTile);
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

        public: virtual Box getElement() const;
        public: virtual Hurricane::Locator<Box>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Box>* getClone() const;
    public: virtual Hurricane::Locator<Box>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Region_BoxesUnder implementation
// ****************************************************************************************************

Region_BoxesUnder::Region_BoxesUnder()
// ***********************************
:     Inherit(),
    _region(NULL),
    _area()
{
}

Region_BoxesUnder::Region_BoxesUnder(const Region* region, const Box& area)
// ************************************************************************
:     Inherit(),
    _region(region),
    _area(area)
{
}

Region_BoxesUnder::Region_BoxesUnder(const Region_BoxesUnder& boxes)
// *****************************************************************
:     Inherit(),
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

Collection<Box>* Region_BoxesUnder::getClone() const
// *************************************************
{
    return new Region_BoxesUnder(*this);
}

Locator<Box>* Region_BoxesUnder::getLocator() const
// ************************************************
{
    return new Locator(_region, _area);
}

string Region_BoxesUnder::_getString() const
// *****************************************
{
    string s = "<" + _TName("Region::BoxesUnder");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region_BoxesUnder::Locator implementation
// ****************************************************************************************************

Region_BoxesUnder::Locator::Locator()
// **********************************
:    Inherit(),
    _region(NULL),
    _area(),
    _tileLocator()
{
}

Region_BoxesUnder::Locator::Locator(const Region* region, const Box& area)
// ***********************************************************************
:    Inherit(),
    _region(region),
    _area(area),
    _tileLocator()
{
    if (_region) {
        _tileLocator =
            (_area.isEmpty()) ?
                _region->_getTiles().getSubSet(!Region_Tile::getIsVoidFilter()).getLocator() :
                _region->_getTilesUnder(_area).getSubSet(!Region_Tile::getIsVoidFilter()).getLocator();
    }
}

Region_BoxesUnder::Locator::Locator(const Locator& locator)
// ********************************************************
:    Inherit(),
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

Box Region_BoxesUnder::Locator::getElement() const
// ***********************************************
{
    return (_tileLocator.isValid()) ? _tileLocator.getElement()->getBoundingBox() : Box();
}

Locator<Box>* Region_BoxesUnder::Locator::getClone() const
// *******************************************************
{
    return new Locator(*this);
}

bool Region_BoxesUnder::Locator::isValid() const
// ********************************************
{
    return _tileLocator.isValid();
}

void Region_BoxesUnder::Locator::progress()
// ****************************************
{
    _tileLocator.progress();
}

string Region_BoxesUnder::Locator::_getString() const
// **************************************************
{
    string s = "<" + _TName("Region::BoxesUnder::Locator");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
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

        public: virtual Box getElement() const;
        public: virtual Hurricane::Locator<Box>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Box>* getClone() const;
    public: virtual Hurricane::Locator<Box>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Region_VoidBoxesUnder implementation
// ****************************************************************************************************

Region_VoidBoxesUnder::Region_VoidBoxesUnder()
// *******************************************
:     Inherit(),
    _region(NULL),
    _area()
{
}

Region_VoidBoxesUnder::Region_VoidBoxesUnder(const Region* region, const Box& area)
// ********************************************************************************
:     Inherit(),
    _region(region),
    _area(area)
{
}

Region_VoidBoxesUnder::Region_VoidBoxesUnder(const Region_VoidBoxesUnder& boxes)
// *****************************************************************************
:     Inherit(),
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

Collection<Box>* Region_VoidBoxesUnder::getClone() const
// *****************************************************
{
    return new Region_VoidBoxesUnder(*this);
}

Locator<Box>* Region_VoidBoxesUnder::getLocator() const
// ****************************************************
{
    return new Locator(_region, _area);
}

string Region_VoidBoxesUnder::_getString() const
// *********************************************
{
    string s = "<" + _TName("Region::VoidBoxesUnder");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
    }
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region_VoidBoxesUnder::Locator implementation
// ****************************************************************************************************

Region_VoidBoxesUnder::Locator::Locator()
// **************************************
:    Inherit(),
    _region(NULL),
    _area(),
    _tileLocator()
{
}

Region_VoidBoxesUnder::Locator::Locator(const Region* region, const Box& area)
// ***************************************************************************
:    Inherit(),
    _region(region),
    _area(area),
    _tileLocator()
{
    if (_region) {
        _tileLocator =
            (_area.isEmpty()) ?
                _region->_getTiles().getSubSet(Region_Tile::getIsVoidFilter()).getLocator() :
                _region->_getTilesUnder(_area).getSubSet(Region_Tile::getIsVoidFilter()).getLocator();
    }
}

Region_VoidBoxesUnder::Locator::Locator(const Locator& locator)
// ************************************************************
:    Inherit(),
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

Box Region_VoidBoxesUnder::Locator::getElement() const
// ***************************************************
{
    return (_tileLocator.isValid()) ? _tileLocator.getElement()->getBoundingBox() : Box();
}

Locator<Box>* Region_VoidBoxesUnder::Locator::getClone() const
// ***********************************************************
{
    return new Locator(*this);
}

bool Region_VoidBoxesUnder::Locator::isValid() const
// *************************************************
{
    return _tileLocator.isValid();
}

void Region_VoidBoxesUnder::Locator::progress()
// ********************************************
{
    _tileLocator.progress();
}

string Region_VoidBoxesUnder::Locator::_getString() const
// ******************************************************
{
    string s = "<" + _TName("Region::VoidBoxesUnder::Locator");
    if (_region) {
        s += " " + getString(_region);
        s += " " + getString(_area);
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

        public: virtual Interval getElement() const;
        public: virtual Hurricane::Locator<Interval>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Interval>* getClone() const;
    public: virtual Hurricane::Locator<Interval>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Region_Intervals implementation
// ****************************************************************************************************

Region_Intervals::Region_Intervals(const Region::SwapLine* swapLine)
// *****************************************************************
:     Inherit(),
    _swapLine(swapLine)
{
}

Region_Intervals::Region_Intervals(const Region_Intervals& intervals)
// ******************************************************************
:     Inherit(),
    _swapLine(intervals._swapLine)
{
}

Region_Intervals& Region_Intervals::operator=(const Region_Intervals& intervals)
// *****************************************************************************
{
    _swapLine = intervals._swapLine;
    return *this;
}

Collection<Interval>* Region_Intervals::getClone() const
// *****************************************************
{
    return new Region_Intervals(*this);
}

Locator<Interval>* Region_Intervals::getLocator() const
// ****************************************************
{
    return new Locator(_swapLine);
}

string Region_Intervals::_getString() const
// ****************************************
{
    string s = "<" + _TName("Region::SwapLine::Intervals");
    if (_swapLine) s += " " + getString(_swapLine);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region_Intervals::Locator implementation
// ****************************************************************************************************

Region_Intervals::Locator::Locator(const Region::SwapLine* swapLine)
// *****************************************************************
:    Inherit(),
    _swapLine(swapLine),
    _lowerTile(NULL),
    _upperTile(NULL)
{
    if (_swapLine) {
        switch (_swapLine->getType()) {
            case Region::SwapLine::Type::VERTICAL : {
                DbU::Unit x = _swapLine->getPosition();
                DbU::Unit yMax = _swapLine->getExtention().getVMax();
                _lowerTile = _swapLine->_getBaseTile();
                while (_lowerTile && _lowerTile->isVoid())
                    _lowerTile = _lowerTile->getTopTile(x);
                if (_lowerTile && (yMax < _lowerTile->getYMin())) _lowerTile = NULL;
                if (_lowerTile) {
                    Region::Tile* tile = _lowerTile;
                    while (tile && !tile->isVoid()) {
                        _upperTile = tile;
                        tile = tile->getTopTile(x);
                        if (tile && (yMax < tile->getYMin())) tile = NULL;
                    }
                }
                break;
            }
            case Region::SwapLine::Type::HORIZONTAL : {
                DbU::Unit y = _swapLine->getPosition();
                DbU::Unit xMax = _swapLine->getExtention().getVMax();
                _lowerTile = _swapLine->_getBaseTile();
                while (_lowerTile && _lowerTile->isVoid())
                    _lowerTile = _lowerTile->getRightTile(y);
                if (_lowerTile && (xMax < _lowerTile->getXMin())) _lowerTile = NULL;
                if (_lowerTile) {
                    Region::Tile* tile = _lowerTile;
                    while (tile && !tile->isVoid()) {
                        _upperTile = tile;
                        tile = tile->getRightTile(y);
                        if (tile && (xMax < tile->getXMin())) tile = NULL;
                    }
                }
                break;
            }
        }
    }
}

Region_Intervals::Locator::Locator(const Locator& locator)
// *******************************************************
:    Inherit(),
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

Interval Region_Intervals::Locator::getElement() const
// ***************************************************
{
    if (!isValid()) return Interval();

    Interval interval;
    switch (_swapLine->getType()) {
        case Region::SwapLine::Type::VERTICAL : {
            interval = Interval(_lowerTile->getYMin(), _upperTile->getYMax());
            break;
        }
        case Region::SwapLine::Type::HORIZONTAL : {
            interval = Interval(_lowerTile->getXMin(), _upperTile->getXMax());
            break;
        }
    }

    return interval.getIntersection(_swapLine->getExtention());
}

Locator<Interval>* Region_Intervals::Locator::getClone() const
// ***********************************************************
{
    return new Locator(*this);
}

bool Region_Intervals::Locator::isValid() const
// ********************************************
{
    return (_lowerTile && _upperTile);
}

void Region_Intervals::Locator::progress()
// ***************************************
{
    if (isValid()) {
        switch (_swapLine->getType()) {
            case Region::SwapLine::Type::VERTICAL : {
                DbU::Unit x = _swapLine->getPosition();
                DbU::Unit yMax = _swapLine->getExtention().getVMax();
                _lowerTile = _upperTile->getTopTile(x);
                while (_lowerTile && _lowerTile->isVoid())
                    _lowerTile = _lowerTile->getTopTile(x);
                if (_lowerTile && (yMax < _lowerTile->getYMin())) _lowerTile = NULL;
                _upperTile = NULL;
                if (_lowerTile) {
                    Region::Tile* tile = _lowerTile;
                    while (tile && !tile->isVoid()) {
                        _upperTile = tile;
                        tile = tile->getTopTile(x);
                        if (tile && (yMax < tile->getYMin())) tile = NULL;
                    }
                }
                break;
            }
            case Region::SwapLine::Type::HORIZONTAL : {
                DbU::Unit y = _swapLine->getPosition();
                DbU::Unit xMax = _swapLine->getExtention().getVMax();
                _lowerTile = _upperTile->getRightTile(y);
                while (_lowerTile && _lowerTile->isVoid())
                    _lowerTile = _lowerTile->getRightTile(y);
                if (_lowerTile && (xMax < _lowerTile->getXMin())) _lowerTile = NULL;
                _upperTile = NULL;
                if (_lowerTile) {
                    Region::Tile* tile = _lowerTile;
                    while (tile && !tile->isVoid()) {
                        _upperTile = tile;
                        tile = tile->getRightTile(y);
                        if (tile && (xMax < tile->getXMin())) tile = NULL;
                    }
                }
                break;
            }
        }
    }
}

string Region_Intervals::Locator::_getString() const
// *************************************************
{
    string s = "<" + _TName("Region::SwapLine::Intervals::Locator");
    if (_swapLine) s += " " + getString(_swapLine);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Region::SwapLine::Type implementation
// ****************************************************************************************************

Region::SwapLine::Type::Type(const Code& code)
// *******************************************
:    _code(code)
{
}

Region::SwapLine::Type::Type(const Type& type)
// *******************************************
:    _code(type._code)
{
}

Region::SwapLine::Type& Region::SwapLine::Type::operator=(const Type& type)
// ************************************************************************
{
    _code = type._code;
    return *this;
}

string Region::SwapLine::Type::_getString() const
// **********************************************
{
    switch (_code) {
        case VERTICAL : return "VERTICAL";
        case HORIZONTAL : return "HORIZONTAL";
    }
    return "ABNORMAL";
}

Record* Region::SwapLine::Type::_getRecord() const
// *****************************************
{
    Record* record = new Record(getString(this));
    record->add ( getSlot ( "Code", ((unsigned int*)((void*)&_code)) ) );
    return record;
}



// ****************************************************************************************************
// Region::SwapLine implementation
// ****************************************************************************************************

Region::SwapLine::SwapLine()
// *************************
:    _region(NULL),
    _type(),
    _position(),
    _extention(),
    _baseTile(NULL)
{
}

Region::SwapLine::SwapLine(Region* region, const Type& type, const Interval& extention)
// ************************************************************************************
:    _region(region),
    _type(type),
    _position(),
    _extention(extention),
    _baseTile(NULL)
{
    if (!_region)
        throw Error("Can't create " + _TName("Region::SwapLine") + " : null region");

    if (!_region->isEmpty()) {
        switch (_type) {
            case Type::VERTICAL : {
                _position = _region->getXMin();
                if (_extention.isEmpty()) _extention = Interval(_region->getYMin(), _region->getYMax());
                _baseTile = _region->_getTileAt(Point(_position, _extention.getVMin()));
                break;
            }
            case Type::HORIZONTAL : {
                _position = _region->getYMin();
                if (_extention.isEmpty()) _extention = Interval(_region->getXMin(), _region->getXMax());
                _baseTile = _region->_getTileAt(Point(_extention.getVMin(), _position));
                break;
            }
        }
    }
}

Region::SwapLine::SwapLine(Region* region, const Type& type, const DbU::Unit& position, const Interval& extention)
// ****************************************************************************************************
:    _region(region),
    _type(type),
    _position(position),
    _extention(extention),
    _baseTile(NULL)
{
    if (!_region)
        throw Error("Can't create " + _TName("Region::SwapLine") + " : null region");

    if (!_region->isEmpty()) {
        switch (_type) {
            case Type::VERTICAL : {
                if (_extention.isEmpty()) _extention = Interval(_region->getYMin(), _region->getYMax());
                _baseTile = _region->_getTileAt(Point(_position, _extention.getVMin()));
                break;
            }
            case Type::HORIZONTAL : {
                if (_extention.isEmpty()) _extention = Interval(_region->getXMin(), _region->getXMax());
                _baseTile = _region->_getTileAt(Point(_extention.getVMin(), _position));
                break;
            }
        }
    }
}

Region::SwapLine::SwapLine(const SwapLine& swapLine)
// *************************************************
:    _region(swapLine._region),
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

Intervals Region::SwapLine::getIntervals() const
// *********************************************
{
    return Region_Intervals(this);
}

void Region::SwapLine::progress(int n)
          // ***************************
{
    if (isValid() && n) {
        if (0 < n) {
            switch (_type) {
                case Type::VERTICAL : {
                    while (n--) {
                        DbU::Unit yMin = getExtention().getVMin();
                        DbU::Unit yMax = getExtention().getVMax();
                        DbU::Unit x = _region->getXMax() + 1;
                        Region::Tile* tile = _baseTile;
                        while (tile) {
                            if (_position < tile->getXMax())
                                x = min(tile->getXMax(), x);
                            else {
                                assert(tile->getXMax() == _position);
                                Region::Tile* rightTile = tile->_rightTile;
                                while (rightTile && (yMax < rightTile->getYMin()))
                                    rightTile = rightTile->_bottomTile;
                                while (rightTile && (yMin < rightTile->getYMax())) {
                                    x = min(rightTile->getXMax(), x);
                                    rightTile = rightTile->_bottomTile;
                                }
                            }
                            tile = tile->getTopTile(_position);
                            if (tile && (yMax < tile->getYMin())) tile = NULL;
                        }
                        _position = x;
                        while (_baseTile && (_baseTile->getXMax() <= x))
                            _baseTile = _baseTile->getRightTile(yMin);
                    }
                    break;
                }
                case Type::HORIZONTAL : {
                    while (n--) {
                        DbU::Unit xMin = getExtention().getVMin();
                        DbU::Unit xMax = getExtention().getVMax();
                        DbU::Unit y = _region->getYMax() + 1;
                        Region::Tile* tile = _baseTile;
                        while (tile) {
                            if (_position < tile->getYMax())
                                y = min(tile->getYMax(), y);
                            else {
                                assert(tile->getYMax() == _position);
                                Region::Tile* topTile = tile->_topTile;
                                while (topTile && (xMax < topTile->getXMin()))
                                    topTile = topTile->_leftTile;
                                while (topTile && (xMin < topTile->getXMax())) {
                                    y = min(topTile->getYMax(), y);
                                    topTile = topTile->_leftTile;
                                }
                            }
                            tile = tile->getRightTile(_position);
                            if (tile && (xMax < tile->getXMin())) tile = NULL;
                        }
                        _position = y;
                        while (_baseTile && (_baseTile->getYMax() <= y))
                            _baseTile = _baseTile->getTopTile(xMin);
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

void Region::SwapLine::translate(const DbU::Unit& quantity)
// ***************************************************
{
  if (quantity) setPosition(getPosition() + quantity);
}

void Region::SwapLine::setPosition(const DbU::Unit& position)
// *****************************************************
{
    if (position != _position) {
        _position = position;
        switch (_type) {
            case Type::VERTICAL : {
                _baseTile = _region->_getTileAt(Point(_position, _extention.getVMin()));
                break;
            }
            case Type::HORIZONTAL : {
                _baseTile = _region->_getTileAt(Point(_extention.getVMin(), _position));
                break;
            }
        }
    }
}

string Region::SwapLine::_getString() const
// ****************************************
{
    string s = "<" + _getTypeName() + ">";
    if (isValid()) {
        s.insert(s.length() - 1, " " + getString(_type));
        s.insert(s.length() - 1, " " + getString(_position));
        s.insert(s.length() - 1, " " + getString(_extention));
    }
    return s;
}

Record* Region::SwapLine::_getRecord() const
// ***********************************
{
    Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("Region", _region));
        record->add(getSlot("Type", &_type));
        record->add(getSlot("Position", &_position));
        record->add(getSlot("Extention", &_extention));
        record->add(getSlot("BaseTile", _baseTile));
    }
    return record;
}



// ****************************************************************************************************
// Region implementation
// ****************************************************************************************************

Region::Region()
// *************
:    _bottomRightTile(NULL),
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
    Box initialBoundingBox = region.getBoundingBox();
    if (! initialBoundingBox.isEmpty()) {
        _bottomRightTile = new Tile(region.getBoundingBox(), true);
        _topLeftTile = _bottomRightTile;
        for_each_box(box, region.getBoxes()) {
            fill(box);
            end_for;
        }
    }
}

Region::~Region()
// **************
{
    clear();
}

Region& Region::operator=(const Region& region)
// ********************************************
{
    clear();
    // keep trace (as void tile) of the initial bounding box
    Box initialBoundingBox = region.getBoundingBox();
    if (! initialBoundingBox.isEmpty()) {
        _bottomRightTile = new Tile (initialBoundingBox, true);
        _topLeftTile = _bottomRightTile;
        for_each_box(box, region.getBoxes()) {
            fill(box);
            end_for;
        }
    }
    return *this;
}

Box Region::getBoundingBox() const
// *******************************
{
    Box boundingBox;
    if (_bottomRightTile) boundingBox = _bottomRightTile->getBoundingBox();
    if (_topLeftTile) boundingBox.merge(_topLeftTile->getBoundingBox());
    return boundingBox;
}

DbU::Unit Region::getXMin() const
// *************************
{
    return (_topLeftTile) ? _topLeftTile->getXMin() : DbU::Unit();
}

DbU::Unit Region::getYMin() const
// *************************
{
    return (_bottomRightTile) ? _bottomRightTile->getYMin() : DbU::Unit();
}

DbU::Unit Region::getXMax() const
// *************************
{
    return (_bottomRightTile) ? _bottomRightTile->getXMax() : DbU::Unit();
}

DbU::Unit Region::getYMax() const
// *************************
{
    return (_topLeftTile) ? _topLeftTile->getYMax() : DbU::Unit();
}

Boxes Region::getBoxes() const
// ***************************
{
    return Region_BoxesUnder(this);
}

Boxes Region::getBoxesUnder(const Box& area) const
// ***********************************************
{
    return Region_BoxesUnder(this, area);
}

Boxes Region::getVoidBoxes() const
// *******************************
{
    return Region_VoidBoxesUnder(this);
}

Boxes Region::getVoidBoxesUnder(const Box& area) const
// ***************************************************
{
    return Region_VoidBoxesUnder(this, area);
}

Region::SwapLine Region::getVerticalSwapLine(const Interval& extention) const
// **************************************************************************
{
    return SwapLine((Region*)this, Region::SwapLine::Type::VERTICAL, extention);
}

Region::SwapLine Region::getVerticalSwapLine(const DbU::Unit& x, const Interval& extention) const
// *****************************************************************************************
{
    return SwapLine((Region*)this, Region::SwapLine::Type::VERTICAL, x, extention);
}

Region::SwapLine Region::getHorizontalSwapLine(const Interval& extention) const
// ****************************************************************************
{
    return SwapLine((Region*)this, Region::SwapLine::Type::HORIZONTAL, extention);
}

Region::SwapLine Region::getHorizontalSwapLine(const DbU::Unit& y, const Interval& extention) const
// *******************************************************************************************
{
    return SwapLine((Region*)this, Region::SwapLine::Type::HORIZONTAL, y, extention);
}

bool Region::isEmpty() const
// *************************
{
    return Region_Tiles(this).getSubSet(!Tile::getIsVoidFilter()).isEmpty();
}

bool Region::contains(const Point& point) const
// ********************************************
{
    return getBoundingBox().contains(point) && _getNonVoidTileAt(point);
}

bool Region::contains(const Box& box) const
// ****************************************
{
    if (box.isPonctual()) return contains(box.getCenter());
    return getBoundingBox().contains(box) &&
      Region_TilesUnder (this, Box(box).inflate(-1))
                         .getSubSet(Tile::getIsVoidFilter()).isEmpty();
}

bool Region::contains(const Region& region) const
// **********************************************
{
    for_each_box(box, region.getBoxesUnder(getBoundingBox())) {
        if (!contains(box)) return false;
        end_for;
    }
    return true;
}

#if 0                                                        // pas encore teste
bool Region::Intersect(const Box& box) const
// *****************************************
{
    if (box.isPonctual()) return contains(box.getCenter());
    if (! getBoundingBox().Intersect(box)) return false;
    if (! Region_TilesUnder (this, Box(box).inflate(1))
                       .getSubSet(! Tile::getIsVoidFilter()).isEmpty()) return true;
    return false;
}

bool Region::Intersect(const Region& region) const
// ***********************************************
{
    for_each_box(box, region.getBoxesUnder(getBoundingBox())) {
        if (Intersect(box)) return true;
        end_for;
    }
    return false;
}
#endif

Region& Region::clear()
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
        DbU::Unit xMin = tile->getXMin();
        DbU::Unit xMax = tile->getXMax();

        Tile* topTile = tile->_topTile;
        while (topTile && (xMin < topTile->getXMax())) {
            if (topTile->getXMax() <= xMax) tileStack.push(topTile);
            topTile = topTile->_leftTile;
        }
        accumulate.push_back (tile);
        //delete tile;
    }
    for_each_object (Tile*, t, getCollection (accumulate)) {
        delete t;
        end_for;
    }
    _bottomRightTile = NULL;
    _topLeftTile = NULL;

    return *this;
}

Region& Region::fill(const Box& box)
// *********************************
{
  if (box.isEmpty() || !box.getWidth() || !box.getHeight()) return *this;

    if (!_bottomRightTile) {
        _bottomRightTile = new Tile(box);
        _topLeftTile = _bottomRightTile;
        return *this;
    }

    if (!getBoundingBox().contains(box))
        _update(box, false);
    else {
        Tile* startTile = _getStartTile(_getTileAt(Point(box.getXMax(), box.getYMin())));
        GenericCollection<Tile*> tiles = _getTilesUnder(Box(box).inflate(0,0,-1,-1), startTile);
        if (!tiles.getSubSet(Tile::getIsVoidFilter()).isEmpty()) _update(box, false, startTile);
    }

    return *this;
}

Region& Region::fill(const Region& region)
// ***************************************
{
    for_each_box(box, region.getBoxes()) {
        fill(box);
        end_for;
    }
    return *this;
}

Region& Region::getUnion (const Region& region)
// ********************************************
{
    return fill(region);
}

Region& Region::groove(const Box& box)
// ***********************************
{
    if (!_bottomRightTile) return *this;

    Box correctedBox = getBoundingBox().getIntersection(box);

    if (correctedBox.isEmpty() || !correctedBox.getWidth() || !correctedBox.getHeight()) return *this;

    Tile* startTile = _getStartTile(_getTileAt(Point(correctedBox.getXMax(), correctedBox.getYMin())));
    GenericCollection<Tile*> tiles = _getTilesUnder(Box(correctedBox).inflate(0,0,-1,-1), startTile);
    if (!tiles.getSubSet(!Tile::getIsVoidFilter()).isEmpty()) _update(box, true, startTile);

    return *this;
}

Region& Region::groove(const Region& region)
// *****************************************
{
    Box boundingBox = getBoundingBox();
    for_each_box(box, region.getBoxesUnder(boundingBox)) {
        groove(box);
        end_for;
    }
    return *this;
}

Region& Region::getIntersection (const Region& region)
// ***************************************************
{
    Box boundingBox = getBoundingBox();
    for_each_box (box, region.getVoidBoxesUnder (boundingBox)) {
        //for_each_box (box, region.getVoidBoxes ()) {
        //if (! boundingBox.Intersect (box)) continue;
        groove (box);
        end_for;
    }
    return *this;
}

Region& Region::Inflate(const DbU::Unit& quantity)
// ******************************************
{
    if (!isEmpty()) {
      if (0 < quantity) {
            list<Box> boxList;
            for_each_object(Tile*, tile, Region_Tiles(this).getSubSet(!Tile::getIsVoidFilter())) {
                boxList.push_back(tile->getBoundingBox());
                end_for;
            }
            for_each_box(box, getCollection(boxList)) {
                fill(box.inflate(quantity));
                end_for;
            }
        }
      else if (quantity < 0) {
        _growthToFit(getBoundingBox().inflate(DbU::grid(1.0)));
            list<Box> boxList;
            for_each_object(Tile*, tile, Region_Tiles(this).getSubSet(Tile::getIsVoidFilter())) {
                boxList.push_back(tile->getBoundingBox());
                end_for;
            }
            for_each_box(box, getCollection(boxList)) {
                groove(box.inflate(-quantity));
                end_for;
            }
        }
    }

    return *this;
}

Region& Region::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ******************************************************
{
    if ((dx != 0) || (dy != 0)) {
        set<Tile*> tileSet;
        _getTiles().fill(tileSet);
        for_each_object(Tile*, tile, getCollection(tileSet)) {
            tile->_boundingBox.translate(dx, dy);
            end_for;
        }
    }
    return *this;
}

string Region::_getString() const
// ******************************
{
    string s = "<" + _getTypeName() + ">";
    return s;
}

Record* Region::_getRecord() const
// *************************
{
    Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("BottomRightTile", _bottomRightTile));
        record->add(getSlot("TopLeftTile", _topLeftTile));
    }
    return record;
}

Region_Tile* Region::_getTileAt(const Point& point, Tile* startTile) const
// ***********************************************************************
{
    if (!getBoundingBox().contains(point)) return NULL;

    return (startTile) ? startTile->getTileAt(point) : _bottomRightTile->getTileAt(point);
}

Region_Tile* Region::_getNonVoidTileAt(const Point& point, Tile* startTile) const
// ******************************************************************************
{
    if (!getBoundingBox().contains(point)) return NULL;

    return (startTile) ?  startTile->getNonVoidTileAt(point) : _bottomRightTile->getNonVoidTileAt(point);
}

Region_Tile* Region::_getStartTile(Tile* tile) const
// *************************************************
{
    Tile* startTile = NULL;

    if (tile) {
        if (!startTile && tile->_rightTile) {
            DbU::Unit y = tile->getYMin();
            startTile = tile->_rightTile;
            while (startTile && (y <= startTile->getYMax())) startTile = startTile->_bottomTile;
        }
        if (!startTile && tile->_bottomTile) {
            DbU::Unit x = tile->getXMax();
            startTile = tile->_bottomTile;
            while (startTile && (startTile->getXMin() <= x)) startTile = startTile->_rightTile;
        }
    }

    return startTile;
}

GenericCollection<Region_Tile*> Region::_getTiles() const
// ******************************************************
{
    return Region_Tiles(this);
}

GenericCollection<Region_Tile*> Region::_getTilesUnder(const Box& area, Tile* startTile) const
// *******************************************************************************************
{
    return Region_TilesUnder(this, area, startTile);
}

void Region::_split(const Box& box)
// ********************************
{
    if (getBoundingBox().intersect(box)) {

        Tile* startTile = _getStartTile(_getTileAt(Point(box.getXMax(), box.getYMin())));

        list<Tile*> tileList;
        Box line = Box(box.getXMin(), box.getYMin(), box.getXMax() - 1, box.getYMin());
        _getTilesUnder(line, startTile).fill(tileList);
        for_each_object(Tile*, tile, getCollection(tileList)) {
            tile->splitHorizontal(this, box.getYMin());
            end_for;
        }

        tileList.clear();
        line = Box(box.getXMin(), box.getYMax(), box.getXMax() - 1, box.getYMax());
        _getTilesUnder(line, startTile).fill(tileList);
        for_each_object(Tile*, tile, getCollection(tileList)) {
            tile->splitHorizontal(this, box.getYMax());
            end_for;
        }

        tileList.clear();
        line = Box(box.getXMin(), box.getYMin(), box.getXMin(), box.getYMax() - 1);
        _getTilesUnder(line, startTile).fill(tileList);
        for_each_object(Tile*, tile, getCollection(tileList)) {
            tile->splitVertical(this, box.getXMin());
            end_for;
        }

        tileList.clear();
        line = Box(box.getXMax(), box.getYMin(), box.getXMax(), box.getYMax() - 1);
        _getTilesUnder(line, startTile).fill(tileList);
        for_each_object(Tile*, tile, getCollection(tileList)) {
            tile->splitVertical(this, box.getXMax());
            end_for;
        }
    }
}

void Region::_growthToFit(const Box& box)
// **************************************
{
    if (box.isEmpty()) return;

    if (!_bottomRightTile) {
        _bottomRightTile = new Tile(box, true);
        _topLeftTile = _bottomRightTile;
        return;
    }

    if (getBoundingBox().contains(box)) return;

    if (box.getYMin() < getYMin()) {
        Tile* newTile = new Tile(Box(getXMin(), box.getYMin(), getXMax(), getYMin()), true);
        Tile* tile = _bottomRightTile;
        while (tile) {
            tile->_bottomTile = newTile;
            tile = tile->_leftTile;
        }
        newTile->_topTile = _bottomRightTile;
        _bottomRightTile = newTile;
    }

    if (getYMax() < box.getYMax()) {
        Tile* newTile = new Tile(Box(getXMin(), getYMax(), getXMax(), box.getYMax()), true);
        Tile* tile = _topLeftTile;
        while (tile) {
            tile->_topTile = newTile;
            tile = tile->_rightTile;
        }
        newTile->_bottomTile = _topLeftTile;
        _topLeftTile = newTile;
    }

    if (box.getXMin() < getXMin()) {
        Tile* newTile = new Tile(Box(box.getXMin(), getYMin(), getXMin(), getYMax()), true);
        Tile* tile = _topLeftTile;
        while (tile) {
            tile->_leftTile = newTile;
            tile = tile->_bottomTile;
        }
        newTile->_rightTile = _topLeftTile;
        _topLeftTile = newTile;
    }

    if (getXMax() < box.getXMax()) {
        Tile* newTile = new Tile(Box(getXMax(), getYMin(), box.getXMax(), getYMax()), true);
        Tile* tile = _bottomRightTile;
        while (tile) {
            tile->_rightTile = newTile;
            tile = tile->_topTile;
        }
        newTile->_leftTile = _bottomRightTile;
        _bottomRightTile = newTile;
    }
}

void Region::_update(const Box& box, bool isVoid, Tile* startTile)
// **************************************************************
{
  if (box.isEmpty() || !_bottomRightTile || !box.getWidth() || !box.getHeight()) return;

    if (!getBoundingBox().contains(box)) _growthToFit(box);

    _split(box);

    Tile* newTile = new Tile(box, isVoid);

    list<Tile*> tileList;
    _getTilesUnder(Box(box).inflate(0,0,-1,-1), startTile).fill(tileList);
    for_each_object(Tile*, tile, getCollection(tileList)) {
        if (_topLeftTile == tile) _topLeftTile = newTile;
        if (_bottomRightTile == tile) _bottomRightTile = newTile;
        if (tile->getXMin() == box.getXMin()) {
            if (tile->getYMin() == box.getYMin()) {
                newTile->_leftTile = tile->_leftTile;
                newTile->_bottomTile = tile->_bottomTile;
            }
            Tile* leftTile = tile->_leftTile;
            while (leftTile && (leftTile->_rightTile == tile)) {
                leftTile->_rightTile = newTile;
                leftTile = leftTile->_topTile;
            }
        }
        if (tile->getYMin() == box.getYMin()) {
            Tile* bottomTile = tile->_bottomTile;
            while (bottomTile && (bottomTile->_topTile == tile)) {
                bottomTile->_topTile = newTile;
                bottomTile = bottomTile->_rightTile;
            }
        }
        if (tile->getYMax() == box.getYMax()) {
            if (tile->getXMax() == box.getXMax()) {
                newTile->_topTile = tile->_topTile;
                newTile->_rightTile = tile->_rightTile;
            }
            Tile* topTile = tile->_topTile;
            while (topTile && (topTile->_bottomTile == tile)) {
                topTile->_bottomTile = newTile;
                topTile = topTile->_leftTile;
            }
        }
        if (tile->getXMax() == box.getXMax()) {
            Tile* rightTile = tile->_rightTile;
            while (rightTile && (rightTile->_leftTile == tile)) {
                rightTile->_leftTile = newTile;
                rightTile = rightTile->_bottomTile;
            }
        }
        end_for;
    }
    for_each_object(Tile*, tile, getCollection(tileList)) {
        delete tile;
        end_for;
    }

    newTile->CleanNeighbours(this);
    newTile->mergeNeighbours(this);
};                // Region::_update

bool 
Region::verticalEnhancement()
// **************************
{
  bool modif = false;
  Region result;
  double minSide = DbU::getGrid(DbU::grid(5.0));
  double minArea = minSide*minSide;
  do {
    // Rechercher la box de plus grande surface
    Box maxBox = Box();
    double area = minArea;
    for_each_box (box, getBoxes()) {
      if (! box.isEmpty()) {
        double a = DbU::getGrid(box.getWidth()) * DbU::getGrid(box.getHeight());
                if (area < a) {
                    area = a;
                    maxBox = box;
                }
      }
      end_for;
    }
    if (maxBox.isEmpty()) break;
    Tile* tile = _getTileAt (maxBox.getCenter());
    if (maxBox.getWidth() >= DbU::grid(2.0)) {
      modif = tile->VerticalEnhancement (this);
    }
    result.fill (tile->getBoundingBox());
    groove (tile->getBoundingBox());
  } while (! isEmpty());
  fill (result);
  return modif;
};                // Region::VerticalEnhancement


bool 
Region::verticalEnhancement(Point point)
// ***************************************
// Amelioration de la tuile contenant point
{
    bool modif = false;
    Tile* tile = _getTileAt (point);
    if (tile) modif = tile->VerticalEnhancement (this);
    return modif;
};                // Region::VerticalEnhancement


bool 
Region::horizontalEnhancement()
// ****************************
{
  bool modif = false;
  Region result;
  const double minSide = DbU::getGrid(DbU::grid(5.0));
  double minArea = minSide*minSide;
  do {
    // Rechercher la box de plus grande surface
    Box maxBox = Box();
    double area = minArea;
    for_each_box (box, getBoxes()) {
      if (! box.isEmpty()) {
        double a = DbU::getGrid(box.getWidth()) * DbU::getGrid(box.getHeight());
                if (area < a) {
                    area = a;
                    maxBox = box;
                }
      }
      end_for;
    }
    if (maxBox.isEmpty()) break;
    Tile* tile = _getTileAt (maxBox.getCenter());
    if (maxBox.getWidth() >= DbU::grid(2.0)) {
      modif = tile->HorizontalEnhancement (this);
    }
    result.fill (tile->getBoundingBox());
    groove (tile->getBoundingBox());
  } while (! isEmpty());
  fill (result);
  return modif;
};

bool 
Region::horizontalEnhancement(Point point)
// ***************************************
// Amelioration de la tuile contenant point
{
    bool modif = false;
    Tile* tile = _getTileAt (point);
  if (tile) modif = tile->HorizontalEnhancement (this);
    return modif;
};                // Region::HorizontalEnhancement


Interval 
Region::topBottomFacing (const Box box) const
// **********************************************
// Retourne le merge des intervales en vis a vis entre les boites
// dessus et dessous de box
{
  Interval result = Interval();
  if (box.isEmpty()) return result;
  Tile* tile = _getTileAt (box.getCenter());
  Interval it1 = tile->_getTopNeighbour ();
  Interval it2 = tile->_getBottomNeighbour ();
  return it1.getIntersection (it2);
};                // Region::TopBottomFacing

Interval 
Region::leftRightFacing (const Box box) const
// **********************************************
// Retourne le merge des intervales en vis a vis entre les boites
// a gauche et droite de box
{
  Interval result = Interval();
  if (box.isEmpty()) return result;
  Tile* tile = _getTileAt (box.getCenter());
  Interval it1 = tile->_getLeftNeighbour ();
  Interval it2 = tile->_getRightNeighbour ();
  return it1.getIntersection (it2);
};                // Region::LeftRightFacing



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
