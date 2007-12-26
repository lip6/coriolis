// ****************************************************************************************************
// File: Horizontal.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_HORIZONTAL
#define HURRICANE_HORIZONTAL

#include "Segment.h"
#include "Horizontals.h"

namespace Hurricane {



// ****************************************************************************************************
// Horizontal declaration
// ****************************************************************************************************

class Horizontal : public Segment {
// ******************************

// Types
// *****

	public: typedef Segment Inherit;

// Attributes
// **********

	private: Unit _y;
	private: Unit _dxSource;
	private: Unit _dxTarget;

// Constructors
// ************

	protected: Horizontal(Net* net, Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);

	public: static Horizontal* Create(Net* net, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);
	public: static Horizontal* Create(Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);

// Accessors
// *********

	public: virtual Unit GetY() const {return _y;};
	public: virtual Point GetCenter() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Box GetBoundingBox(BasicLayer* basicLayer) const;
	public: virtual Unit GetSourceX() const;
	public: virtual Unit GetSourceY() const {return GetY();};
	public: virtual Unit GetTargetX() const;
	public: virtual Unit GetTargetY() const {return GetY();};
	public: virtual Unit GetLength() const;
	public: const Unit& GetDxSource() const {return _dxSource;};
	public: const Unit& GetDxTarget() const {return _dxTarget;};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

	public: void SetY(const Unit& y);
	public: void SetDxSource(const Unit& dxSource);
	public: void SetDxTarget(const Unit& dxSource);
	public: void Translate(const Unit& dy);

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Horizontal");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Horizontal)

#endif // HURRICANE_HORIZONTAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
