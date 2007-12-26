// ****************************************************************************************************
// File: Region.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_REGION
#define HURRICANE_REGION

#include "Boxes.h"
#include "Intervals.h"

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

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("Region::SwapLine::Type"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

	// Attributes
	// **********
	
		private: Region* _region;
		private: Type _type;
		private: Unit _position;
		private: Interval _extention;
		private: Tile* _baseTile;

	// Constructors
	// ************

		public: SwapLine();
		public: SwapLine(Region* region, const Type& type, const Interval& extention = Interval());
		public: SwapLine(Region* region, const Type& type, const Unit& position, const Interval& extention = Interval());
		public: SwapLine(const SwapLine& swapLine);

	// Destructor
	// **********

		public: ~SwapLine();

	// Operators
	// *********

		public: SwapLine& operator=(const SwapLine& swapLine);

	// Accessors
	// *********

		public: Region* GetRegion() const {return _region;};
		public: Type GetType() const {return _type;};
		public: const Unit& GetPosition() const {return _position;};
		public: const Interval& GetExtention() const {return _extention;};

		public: Intervals GetIntervals() const;

	// Predicates
	// **********

		public: bool IsValid() const {return _baseTile;};

	// Updators
	// ********

		public: void Progress(int n = 1);
		public: void Translate(const Unit& quantity);
		public: void SetPosition(const Unit& position);

	// Others
	// ******

        public: string _GetTypeName() const { return _TName("Region::SwapLine"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;
		public: Tile* _GetBaseTile() const {return _baseTile;};

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

	public: Box GetBoundingBox() const;

	public: Unit GetXMin() const;
	public: Unit GetYMin() const;
	public: Unit GetXMax() const;
	public: Unit GetYMax() const;

	public: Boxes GetBoxes() const;
	public: Boxes GetBoxesUnder(const Box& area) const;

	public: Boxes GetVoidBoxes() const;
	public: Boxes GetVoidBoxesUnder(const Box& area) const;

	public: SwapLine GetVerticalSwapLine(const Interval& extention = Interval()) const;
	public: SwapLine GetVerticalSwapLine(const Unit& x, const Interval& extention = Interval()) const;

	public: SwapLine GetHorizontalSwapLine(const Interval& extention = Interval()) const;
	public: SwapLine GetHorizontalSwapLine(const Unit& y, const Interval& extention = Interval()) const;

// Predicates
// **********

	public: bool IsEmpty() const;

	public: bool Contains(const Point& point) const;
	public: bool Contains(const Box& box) const;
	public: bool Contains(const Region& region) const;
#if 0														// pas encore teste
	public: bool Intersect(const Box& box) const;
	public: bool Intersect(const Region& region) const;
#endif

// Updators
// ********

	public: Region& Clear();

	public: Region& Fill(const Box& box);
	public: Region& Fill(const Region& region);
  public: Region& GetUnion(const Region& region);

	public: Region& Groove(const Box& box);
	public: Region& Groove(const Region& region);
  public: Region& GetIntersection(const Region& region);

	public: Region& Inflate(const Unit& quantity);
	public: Region& Translate(const Unit& dx, const Unit& dy);

	public: bool VerticalEnhancement();
	public: bool VerticalEnhancement(Point point);
	public: bool HorizontalEnhancement();
	public: bool HorizontalEnhancement(Point point);
	public: Interval TopBottomFacing (const Box box) const;
	public: Interval LeftRightFacing (const Box box) const;

// Others
// ******

    public: string _GetTypeName() const { return _TName("Region"); };
	public: string _GetString() const;
	public: Record* _GetRecord() const;

	public: Tile* _GetBottomRightTile() const {return _bottomRightTile;};
	public: Tile* _GetTopLeftTile() const {return _topLeftTile;};
	public: Tile* _GetTileAt(const Point& point, Tile* startTile = NULL) const;
	public: Tile* _GetNonVoidTileAt(const Point& point, Tile* startTile = NULL) const;
	public: Tile* _GetStartTile(Tile* tile) const;
	public: GenericCollection<Tile*> _GetTiles() const;
	public: GenericCollection<Tile*> _GetTilesUnder(const Box& area, Tile* startTile = NULL) const;

	public: void _SetBottomRightTile(Tile* bottomRightTile) {_bottomRightTile = bottomRightTile;};
	public: void _SetTopLeftTile(Tile* topLeftTile) {_topLeftTile = topLeftTile;};
	public: void _Split(const Box& box);
	public: void _GrowthToFit(const Box& box);
	public: void _Update(const Box& box, bool isVoid, Tile* startTile = NULL);

};




} // End of Hurricane namespace.


#endif // HURRICANE_REGION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
