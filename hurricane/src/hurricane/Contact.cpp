// ****************************************************************************************************
// File: Contact.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Contact.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Contact_Hooks declaration
// ****************************************************************************************************

class Contact_Hooks : public Collection<Hook*> {
// *******************************************

// Types
// *****

	public: typedef Collection<Hook*> Inherit;

	public: class Locator : public Hurricane::Locator<Hook*> {
	// *****************************************************

		public: typedef Hurricane::Locator<Hook*> Inherit;

		private: const Contact* _contact;
		private: Hook* _hook;

		public: Locator(const Contact* contact = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Hook* GetElement() const;
		public: virtual Hurricane::Locator<Hook*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const Contact* _contact;

// Constructors
// ************

	public: Contact_Hooks(const Contact* contact = NULL);
	public: Contact_Hooks(const Contact_Hooks& hooks);

// Operators
// *********

	public: Contact_Hooks& operator=(const Contact_Hooks& hooks);

// Accessors
// ********^

	public: virtual Collection<Hook*>* GetClone() const;
	public: virtual Hurricane::Locator<Hook*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// Contact implementation
// ****************************************************************************************************

Contact::Contact(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
:  Inherit(net),
	_anchorHook(this),
	_layer(layer),
	_dx(x),
	_dy(y),
	_width(width),
	_height(height)
{
	if (!_layer)
		throw Error("Can't create " + _TName("Contact") + " : null layer");
}

Contact::Contact(Net* net, Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width, const Unit& height)
// ****************************************************************************************************
:  Inherit(net),
	_anchorHook(this),
	_layer(layer),
	_dx(dx),
	_dy(dy),
	_width(width),
	_height(height)
{
	if (!anchor)
		throw Error("Can't create " + _TName("Contact") + " : null anchor");

	if (!anchor->GetNet())
		throw Error("Can't create " + _TName("Contact") + " : unconnected anchor");

	if (anchor->GetNet() != GetNet())
		throw Error("Can't create " + _TName("Contact") + " : incompatible anchor");

	if (!_layer)
		throw Error("Can't create " + _TName("Contact") + " : null layer");

	_anchorHook.Attach(anchor->GetBodyHook());
}

Contact* Contact::Create(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
	Contact* contact = new Contact(net, layer, x, y, width, height);

	contact->_PostCreate();

	return contact;
}

Contact* Contact::Create(Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
	if (!anchor)
		throw Error("Can't create " + _TName("Contact") + " : null anchor");

	Contact* contact = new Contact(anchor->GetNet(), anchor, layer, dx, dy, width, height);

	contact->_PostCreate();

	return contact;
}

Hooks Contact::GetHooks() const
// ****************************
{
	return Contact_Hooks(this);
}

Unit Contact::GetX() const
// ***********************
{
	Component* anchor = GetAnchor();
	return (!anchor) ? _dx : anchor->GetX() + _dx;
}

Unit Contact::GetY() const
// ***********************
{
	Component* anchor = GetAnchor();
	return (!anchor) ? _dy : anchor->GetY() + _dy;
}

Point Contact::GetPosition() const
// *******************************
{
	Component* anchor = GetAnchor();
	return (!anchor) ? Point(_dx, _dy) : anchor->GetPosition().Translate(_dx, _dy);
}

Box Contact::GetBoundingBox() const
// ********************************
{
	Unit size = _GetSize();

	return Box(GetPosition()).Inflate(GetHalfWidth() + size, GetHalfHeight() + size);
}

Box Contact::GetBoundingBox(BasicLayer* basicLayer) const
// ******************************************************
{
	if (!_layer->Contains(basicLayer)) return Box();

	Unit size = _GetSize(basicLayer);

	return Box(GetPosition()).Inflate(GetHalfWidth() + size, GetHalfHeight() + size);
}

Component* Contact::GetAnchor() const
// **********************************
{
	Hook* masterHook = _anchorHook.GetMasterHook();
	return (masterHook) ? masterHook->GetComponent() : NULL;
}

void Contact::Translate(const Unit& dx, const Unit& dy)
// ****************************************************
{
	if ((dx != 0) || (dy != 0)) {
		Invalidate(true);
		_dx += dx;
		_dy += dy;
	}
}

void Contact::SetLayer(Layer* layer)
// *********************************
{
	if (!layer)
		throw Error("Can't set layer : null layer");

	if (layer != _layer) {
		Invalidate(false);
		_layer = layer;
	}
}

void Contact::SetWidth(const Unit& width)
// **************************************
{
	if (width != _width) {
		Invalidate(false);
		_width = width;
	}
}

void Contact::SetHeight(const Unit& height)
// ****************************************
{
	if (height != _height) {
		Invalidate(false);
		_height = height;
	}
}

void Contact::SetSizes(const Unit& width, const Unit& height)
// **********************************************************
{
	if ((width != _width) || (height != _height)) {
		Invalidate(false);
		_width = width;
		_height = height;
	}
}

void Contact::SetX(const Unit& x)
// ******************************
{
	SetPosition(x, GetY());
}

void Contact::SetY(const Unit& y)
// ******************************
{
	SetPosition(GetX(), y);
}

void Contact::SetPosition(const Unit& x, const Unit& y)
// ****************************************************
{
	Component* anchor = GetAnchor();
	if (!anchor)
		SetOffset(x, y);
	else
		SetOffset(x - anchor->GetX(), y - anchor->GetY());
}

void Contact::SetPosition(const Point& position)
// *********************************************
{
	SetPosition(position.GetX(), position.GetY());
}

void Contact::SetDx(const Unit& dx)
// ********************************
{
	SetOffset(dx, _dy);
}

void Contact::SetDy(const Unit& dy)
// ********************************
{
	SetOffset(_dx, dy);
}

void Contact::SetOffset(const Unit& dx, const Unit& dy)
// ****************************************************
{
	if ((dx != _dx) || (dy != _dy)) {
		Invalidate(true);
		_dx = dx;
		_dy = dy;
	}
}

void Contact::_PreDelete()
// ***********************
{
// trace << "entering Contact::PreDelete: " << this << endl;
// trace_in();

	Inherit::_PreDelete();

	_anchorHook.Detach();

// trace << "exiting Contact::PreDelete:" << endl;
// trace_out();
}

string Contact::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_layer->GetName()));
	s.insert(s.length() - 1, " [" + GetValueString(GetX()));
	s.insert(s.length() - 1, " " + GetValueString(GetY()));
	s.insert(s.length() - 1, "] " + GetValueString(_width));
	s.insert(s.length() - 1, "x" + GetValueString(_height));
	return s;
}

Record* Contact::_GetRecord() const
// **************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("AnchorHook", &_anchorHook));
		record->Add(GetSlot("Anchor", GetAnchor()));
		record->Add(GetSlot("Layer", _layer));
		record->Add(GetSlot("Dx", &_dx));
		record->Add(GetSlot("Dy", &_dy));
		record->Add(GetSlot("Width", &_width));
		record->Add(GetSlot("Height", &_height));
	}
	return record;
}

Unit Contact::_GetSize() const
// ***************************
{
	Unit size = 0;

	Layer* layer = GetLayer();
	if (is_a<CompositeLayer*>(layer))
		size = ((CompositeLayer*)layer)->GetMaximalContactSize();

	return size;
}

Unit Contact::_GetSize(BasicLayer* basicLayer) const
// *************************************************
{
	Layer* layer = GetLayer();

	if (!layer->Contains(basicLayer)) return 0;

	Unit size = 0;

	if (is_a<CompositeLayer*>(layer))
		size = ((CompositeLayer*)layer)->GetContactSize(basicLayer);

	return size;
}

//void Contact::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//	if (_width && _height) {
//		if (1 < view->GetScreenSize(max(_width, _height)))
//			basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//	}
//	if (basicLayer == _layer->_GetSymbolicBasicLayer()) {
//		if (view->CutPointsAreVisible() && (3 < view->GetScale())) {
//			Point position = GetPosition();
//			view->DrawPoint(transformation.GetPoint(position), 3);
//			if (_width) {
//				Box box = transformation.GetBox(Box(position).Inflate(GetHalfWidth(), 0));
//				view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//			}
//			if (_height) {
//				Box box = transformation.GetBox(Box(position).Inflate(0, GetHalfHeight()));
//				view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//			}
//		}
//	}
//}
//
//void Contact::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
//	if (_width && _height) {
//		if (1 < view->GetScreenSize(max(_width, _height))) {
//			for_each_basic_layer(basicLayer, GetLayer()->GetBasicLayers()) {
//				basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//				end_for;
//			}
//		}
//	}
//	if (view->GetScale() <= 1)
//		view->DrawPoint(transformation.GetPoint(GetPosition()), 1);
//	else if (view->GetScale() <= 3)
//		view->DrawPoint(transformation.GetPoint(GetPosition()), 2);
//	else {
//		Point position = GetPosition();
//		view->DrawPoint(transformation.GetPoint(position), 3);
//		if (_width) {
//			Box box = transformation.GetBox(Box(position).Inflate(GetHalfWidth(), 0));
//			view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//		}
//		if (_height) {
//			Box box = transformation.GetBox(Box(position).Inflate(0, GetHalfHeight()));
//			view->DrawLine(box.GetXMin(), box.GetYMin(), box.GetXMax(), box.GetYMax());
//		}
//	}
//}
//

// ****************************************************************************************************
// Contact::AnchorHook implementation
// ****************************************************************************************************

static int ANCHOR_HOOK_OFFSET = -1;

Contact::AnchorHook::AnchorHook(Contact* contact)
// **********************************************
:	Inherit()
{
	if (!contact)
		throw Error("Can't create " + _TName("Contact::AnchorHook") + " : null contact");

		if (ANCHOR_HOOK_OFFSET == -1)
		ANCHOR_HOOK_OFFSET = (unsigned long)this - (unsigned long)contact;
}

Component* Contact::AnchorHook::GetComponent() const
// *************************************************
{
	return (Component*)((unsigned long)this - ANCHOR_HOOK_OFFSET);
}

string Contact::AnchorHook::_GetString() const
// *******************************************
{
	return "<" + _TName("Contact::AnchorHook") + " " + GetString(GetComponent()) + ">";
}



// ****************************************************************************************************
// Contact_Hooks implementation
// ****************************************************************************************************

Contact_Hooks::Contact_Hooks(const Contact* contact)
// *************************************************
: 	Inherit(),
	_contact(contact)
{
}

Contact_Hooks::Contact_Hooks(const Contact_Hooks& hooks)
// *****************************************************
: 	Inherit(),
	_contact(hooks._contact)
{
}

Contact_Hooks& Contact_Hooks::operator=(const Contact_Hooks& hooks)
// ****************************************************************
{
	_contact = hooks._contact;
	return *this;
}

Collection<Hook*>* Contact_Hooks::GetClone() const
// ***********************************************
{
	return new Contact_Hooks(*this);
}

Locator<Hook*>* Contact_Hooks::GetLocator() const
// **********************************************
{
	return new Locator(_contact);
}

string Contact_Hooks::_GetString() const
// *************************************
{
	string s = "<" + _TName("Contact::Hooks");
	if (_contact) s += " " + GetString(_contact);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Contact_Hooks::Locator implementation
// ****************************************************************************************************

Contact_Hooks::Locator::Locator(const Contact* contact)
// ****************************************************
:	Inherit(),
	_contact(contact),
	_hook(NULL)
{
	if (_contact) _hook = ((Contact*)_contact)->GetBodyHook();
}

Contact_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:	Inherit(),
	_contact(locator._contact),
	_hook(locator._hook)
{
}

Contact_Hooks::Locator& Contact_Hooks::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
	_contact = locator._contact;
	_hook = locator._hook;
	return *this;
}

Hook* Contact_Hooks::Locator::GetElement() const
// *********************************************
{
	return _hook;
}

Locator<Hook*>* Contact_Hooks::Locator::GetClone() const
// *****************************************************
{
	return new Locator(*this);
}

bool Contact_Hooks::Locator::IsValid() const
// *****************************************
{
	return (_hook != NULL);
}

void Contact_Hooks::Locator::Progress()
// ************************************
{
	if (_hook) {
		if (_hook == ((Contact*)_contact)->GetBodyHook())
			_hook = ((Contact*)_contact)->GetAnchorHook();
		else
			_hook = NULL;
	}
}

string Contact_Hooks::Locator::_GetString() const
// **********************************************
{
	string s = "<" + _TName("Contact::Hooks::Locator");
	if (_contact) s += " " + GetString(_contact);
	s += ">";
	return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
