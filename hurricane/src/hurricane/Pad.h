// ****************************************************************************************************
// File: Pad.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#ifndef HURRICANE_PAD
#define HURRICANE_PAD

#include "Component.h"
#include "Pads.h"

namespace Hurricane {

class Layer;



// ****************************************************************************************************
// Pad declaration
// ****************************************************************************************************

class Pad : public Component {
// *************************

// Types
// *****

	public: typedef Component Inherit;

// Attributes
// **********

	private: Layer* _layer;
	private: Box _boundingBox;

// Constructors
// ************

	protected: Pad(Net* net, Layer* layer, const Box& boundingBox);

	public: static Pad* Create(Net* net, Layer* layer, const Box& boundingBox);

// Accessors
// *********

	public: virtual Unit GetX() const;
	public: virtual Unit GetY() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Box GetBoundingBox(BasicLayer* basicLayer) const;
	public: virtual Layer* GetLayer() const {return _layer;};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);
	public: void SetBoundingBox(const Box& boundingBox);

// Others
// ******

	public: virtual string _GetTypeName() const {return _TName("Pad");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

//	public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
//	public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Pad)

#endif // HURRICANE_PAD

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
