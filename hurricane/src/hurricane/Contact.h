// ****************************************************************************************************
// File: Contact.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_CONTACT
#define HURRICANE_CONTACT

#include "Component.h"
#include "Contacts.h"

namespace Hurricane {



// ****************************************************************************************************
// Contact declaration
// ****************************************************************************************************

class Contact : public Component {
// *****************************

// Types
// *****

	public: typedef Component Inherit;

	public: class AnchorHook : public Hook {
	// ***********************************

		friend class Contact;

		public: typedef Hook Inherit;

		private: AnchorHook(Contact* contact);

		public: virtual Component* GetComponent() const;

		public: virtual bool IsMaster() const {return false;};

        public: virtual string _GetTypeName() const { return _TName("Contact::AnchorHook"); };
		public: virtual string _GetString() const;
	};

// Attributes
// **********

	private:   AnchorHook _anchorHook;
	private:   Layer* _layer;
	private:   Unit _dx;
	private:   Unit _dy;
	protected: Unit _width;
	protected: Unit _height;

// Constructors
// ************

	protected: Contact(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);
	protected: Contact(Net* net, Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width = 0, const Unit& height = 0);

	public: static Contact* Create(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);
	public: static Contact* Create(Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width = 0, const Unit& height = 0);

// Accessors
// *********

	public: virtual Hooks GetHooks() const;
	public: virtual Unit GetX() const;
	public: virtual Unit GetY() const;
	public: virtual Point GetPosition() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Layer* GetLayer() const {return _layer;};
	public: virtual Box GetBoundingBox(const BasicLayer* basicLayer) const;
	public: Hook* GetAnchorHook() {return &_anchorHook;};
	public: Component* GetAnchor() const;
	public: const Unit& GetDx() const {return _dx;};
	public: const Unit& GetDy() const {return _dy;};
	public: const Unit& GetWidth() const {return _width;};
	public: Unit GetHalfWidth() const {return (_width / 2);};
	public: const Unit& GetHeight() const {return _height;};
	public: Unit GetHalfHeight() const {return (_height / 2);};

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

	public: void SetLayer(Layer* layer);
	public: void SetWidth(const Unit& width);
	public: void SetHeight(const Unit& height);
	public: void SetSizes(const Unit& width, const Unit& height);
	public: void SetX(const Unit& x);
	public: void SetY(const Unit& y);
	public: void SetPosition(const Unit& x, const Unit& y);
	public: void SetPosition(const Point& position);
	public: void SetDx(const Unit& dx);
	public: void SetDy(const Unit& dy);
	public: void SetOffset(const Unit& dx, const Unit& dy);

// Others
// ******

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("Contact");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

	public: Unit _GetSize() const;
	public: Unit _GetSize(const BasicLayer* basicLayer) const;

	//public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
	//public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Contact)

#endif // HURRICANE_CONTACT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
