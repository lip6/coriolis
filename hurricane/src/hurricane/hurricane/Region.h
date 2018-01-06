// ****************************************************************************************************
// File: ./hurricane/Region.h
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

#ifndef HURRICANE_REGION
#define HURRICANE_REGION

#include "hurricane/Boxes.h"
#include "hurricane/Intervals.h"

namespace Hurricane {

class Region_Tile; // defined in file Region.cpp to hide the implementation



// ****************************************************************************************************
// Region declaration
// ****************************************************************************************************

class Region {
// *********

// Types
// *****

    public: typedef Region_Tile Tile;

    public: class SwapLine {
    // *******************
    
    public: class Type {
    // ***************

        public: enum Code {VERTICAL=0, HORIZONTAL=1};

        private: Code _code;

        public: Type(const Code& code = VERTICAL);
        public: Type(const Type& type);

        public: Type& operator=(const Type& type);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Region::SwapLine::Type"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

    // Attributes
    // **********
    
        private: Region* _region;
        private: Type _type;
        private: DbU::Unit _position;
        private: Interval _extention;
        private: Tile* _baseTile;

    // Constructors
    // ************

        public: SwapLine();
        public: SwapLine(Region* region, const Type& type, const Interval& extention = Interval());
        public: SwapLine(Region* region, const Type& type, const DbU::Unit& position, const Interval& extention = Interval());
        public: SwapLine(const SwapLine& swapLine);

    // Destructor
    // **********

        public: ~SwapLine();

    // Operators
    // *********

        public: SwapLine& operator=(const SwapLine& swapLine);

    // Accessors
    // *********

        public: Region* getRegion() const {return _region;};
        public: Type getType() const {return _type;};
        public: const DbU::Unit& getPosition() const {return _position;};
        public: const Interval& getExtention() const {return _extention;};

        public: Intervals getIntervals() const;

    // Predicates
    // **********

        public: bool isValid() const {return _baseTile;};

    // Updators
    // ********

        public: void progress(int n = 1);
        public: void translate(const DbU::Unit& quantity);
        public: void setPosition(const DbU::Unit& position);

    // Others
    // ******

        public: string _getTypeName() const { return _TName("Region::SwapLine"); };
        public: string _getString() const;
        public: Record* _getRecord() const;
        public: Tile* _getBaseTile() const {return _baseTile;};

    };

// Attributes
// **********

    private: Tile* _bottomRightTile;
    private: Tile* _topLeftTile;

// Constructors
// ************

    public: Region();
    public: Region(const Box& box, bool isVoid = false);
    public: Region(const Region& region);

// Destructor
// **********

    public: ~Region();

// Operators
// *********

    public: Region& operator=(const Region& region);

// Accessors
// *********

    public: Box getBoundingBox() const;

    public: DbU::Unit getXMin() const;
    public: DbU::Unit getYMin() const;
    public: DbU::Unit getXMax() const;
    public: DbU::Unit getYMax() const;

    public: Boxes getBoxes() const;
    public: Boxes getBoxesUnder(const Box& area) const;

    public: Boxes getVoidBoxes() const;
    public: Boxes getVoidBoxesUnder(const Box& area) const;

    public: SwapLine getVerticalSwapLine(const Interval& extention = Interval()) const;
    public: SwapLine getVerticalSwapLine(const DbU::Unit& x, const Interval& extention = Interval()) const;

    public: SwapLine getHorizontalSwapLine(const Interval& extention = Interval()) const;
    public: SwapLine getHorizontalSwapLine(const DbU::Unit& y, const Interval& extention = Interval()) const;

// Predicates
// **********

    public: bool isEmpty() const;

    public: bool contains(const Point& point) const;
    public: bool contains(const Box& box) const;
    public: bool contains(const Region& region) const;
#if 0                                                        // pas encore teste
    public: bool Intersect(const Box& box) const;
    public: bool Intersect(const Region& region) const;
#endif

// Updators
// ********

    public: Region& clear();

    public: Region& fill(const Box& box);
    public: Region& fill(const Region& region);
    public: Region& getUnion(const Region& region);

    public: Region& groove(const Box& box);
    public: Region& groove(const Region& region);
    public: Region& getIntersection(const Region& region);

    public: Region& Inflate(const DbU::Unit& quantity);
    public: Region& translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: bool verticalEnhancement();
    public: bool verticalEnhancement(Point point);
    public: bool horizontalEnhancement();
    public: bool horizontalEnhancement(Point point);
    public: Interval topBottomFacing (const Box box) const;
    public: Interval leftRightFacing (const Box box) const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Region"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

    public: Tile* _getBottomRightTile() const {return _bottomRightTile;};
    public: Tile* _getTopLeftTile() const {return _topLeftTile;};
    public: Tile* _getTileAt(const Point& point, Tile* startTile = NULL) const;
    public: Tile* _getNonVoidTileAt(const Point& point, Tile* startTile = NULL) const;
    public: Tile* _getStartTile(Tile* tile) const;
    public: GenericCollection<Tile*> _getTiles() const;
    public: GenericCollection<Tile*> _getTilesUnder(const Box& area, Tile* startTile = NULL) const;

    public: void _setBottomRightTile(Tile* bottomRightTile) {_bottomRightTile = bottomRightTile;};
    public: void _setTopLeftTile(Tile* topLeftTile) {_topLeftTile = topLeftTile;};
    public: void _split(const Box& box);
    public: void _growthToFit(const Box& box);
    public: void _update(const Box& box, bool isVoid, Tile* startTile = NULL);

};




} // End of Hurricane namespace.


#endif // HURRICANE_REGION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
