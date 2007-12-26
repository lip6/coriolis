// ****************************************************************************************************
// File: Segment.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_SEGMENT
#define HURRICANE_SEGMENT

#include "Component.h"
#include "Segments.h"

namespace Hurricane {



// ****************************************************************************************************
// Segment declaration
// ****************************************************************************************************

class Segment : public Component {
// *****************************

// Types
// *****

	public: typedef Component Inherit;

	public: class SourceHook : public Hook {
	// ***********************************

		friend class Segment;

		public: typedef Hook Inherit;

		private: SourceHook(Segment* segment);

		public: virtual Component* GetComponent() const;

		public: virtual bool IsMaster() const {return false;};

        public: virtual string _GetTypeName() const { return _TName("Segment::SourceHook"); };
		public: virtual string _GetString() const;
	};

	public: class TargetHook : public Hook {
	// ***********************************

		friend class Segment;

		public: typedef Hook Inherit;

		private: TargetHook(Segment* segment);

		public: virtual Component* GetComponent() const;

		public: virtual bool IsMaster() const {return false;};

        public: virtual string _GetTypeName() const { return _TName("Segment::TargetHook"); };
		public: virtual string _GetString() const;
	};

// Attributes
// **********

	private: SourceHook _sourceHook;
	private: TargetHook _targetHook;
	private: Layer* _layer;
	private: Unit _width;

// Constructors
// ************

	protected: Segment(Net* net, Component* source, Component* target, Layer* layer, const Unit& width = 0);

// Accessors
// *********

	public: virtual Hooks GetHooks() const;
	public: virtual Unit GetX() const {return 0;};
	public: virtual Unit GetY() const {return 0;};
	public: virtual Layer* GetLayer() const {return _layer;};
	public: Hook* GetSourceHook() {return &_sourceHook;};
	public: Hook* GetTargetHook() {return &_targetHook;};
	public: Hook* GetOppositeHook(const Hook* hook) const;
	public: Component* GetSource() const;
	public: Component* GetTarget() const;
	public: Components GetAnchors() const;
	public: Component* GetOppositeAnchor(Component* anchor) const;
	public: const Unit& GetWidth() const {return _width;};
	public: Unit GetHalfWidth() const {return (_width / 2);};
	public: virtual Unit GetSourceX() const = 0;
	public: virtual Unit GetSourceY() const = 0;
	public: virtual Point GetSourcePosition() const;
	public: virtual Unit GetTargetX() const = 0;
	public: virtual Unit GetTargetY() const = 0;
	public: virtual Point GetTargetPosition() const;
	public: virtual Unit GetLength() const = 0;

// Updators
// ********

	public: void SetLayer(Layer* layer);
	public: void SetWidth(const Unit& width);
	public: void Invert();

// Others
// ******

	protected: virtual void _PreDelete();

	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: Unit _GetSize() const;
	public: Unit _GetExtention() const;
	public: Unit _GetSize(BasicLayer* basicLayer) const;
	public: Unit _GetExtention(BasicLayer* basicLayer) const;

//	public: virtual bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
//	public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Segment)
SetNestedSlotAdapter(Hurricane::Segment::SourceHook)
SetNestedSlotAdapter(Hurricane::Segment::TargetHook)

#endif // HURRICANE_SEGMENT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
