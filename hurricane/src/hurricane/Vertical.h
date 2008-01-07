// ****************************************************************************************************
// File: Vertical.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_VERTICAL
#define HURRICANE_VERTICAL

#include "Segment.h"
#include "Verticals.h"

namespace Hurricane {



// ****************************************************************************************************
// Vertical declaration
// ****************************************************************************************************

class Vertical : public Segment {
// ****************************

// Types
// *****

	public: typedef Segment Inherit;

// Attributes
// **********

	private: Unit _x;
	private: Unit _dySource;
	private: Unit _dyTarget;

// Constructors
// ************

	protected: Vertical(Net* net, Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);

	public: static Vertical* Create(Net* net, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);
	public: static Vertical* Create(Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);

// Accessors
// *********

	public: virtual Unit GetX() const {return _x;};
	public: virtual Point GetCenter() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Box GetBoundingBox(const BasicLayer* basicLayer) const;
	public: virtual Unit GetSourceX() const {return GetX();};
	public: virtual Unit GetSourceY() const;
	public: virtual Unit GetTargetX() const {return GetX();};
	public: virtual Unit GetTargetY() const;
	public: virtual Unit GetLength() const;
	public: const Unit& GetDySource() const {return _dySource;};
	public: const Unit& GetDyTarget() const {return _dyTarget;};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

	public: void SetX(const Unit& x);
	public: void SetDySource(const Unit& dySource);
	public: void SetDyTarget(const Unit& dyTarget);
	public: void Translate(const Unit& dx);

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Vertical");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Vertical)

#endif // HURRICANE_VERTICAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
