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

        public: virtual Hook* getElement() const;
        public: virtual Hurricane::Locator<Hook*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

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

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

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

    if (!anchor->getNet())
        throw Error("Can't create " + _TName("Contact") + " : unconnected anchor");

    if (anchor->getNet() != getNet())
        throw Error("Can't create " + _TName("Contact") + " : incompatible anchor");

    if (!_layer)
        throw Error("Can't create " + _TName("Contact") + " : null layer");

    _anchorHook.Attach(anchor->getBodyHook());
}

Contact* Contact::create(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
    Contact* contact = new Contact(net, layer, x, y, width, height);

    contact->_postCreate();

    return contact;
}

Contact* Contact::create(Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width, const Unit& height)
// ****************************************************************************************************
{
    if (!anchor)
        throw Error("Can't create " + _TName("Contact") + " : null anchor");

    Contact* contact = new Contact(anchor->getNet(), anchor, layer, dx, dy, width, height);

    contact->_postCreate();

    return contact;
}

Hooks Contact::getHooks() const
// ****************************
{
    return Contact_Hooks(this);
}

Unit Contact::getX() const
// ***********************
{
    Component* anchor = getAnchor();
    return (!anchor) ? _dx : anchor->getX() + _dx;
}

Unit Contact::getY() const
// ***********************
{
    Component* anchor = getAnchor();
    return (!anchor) ? _dy : anchor->getY() + _dy;
}

Point Contact::getPosition() const
// *******************************
{
    Component* anchor = getAnchor();
    return (!anchor) ? Point(_dx, _dy) : anchor->getPosition().translate(_dx, _dy);
}

Box Contact::getBoundingBox() const
// ********************************
{
    Unit size = _getSize();

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Box Contact::getBoundingBox(const BasicLayer* basicLayer) const
// ******************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    Unit size = _getSize(basicLayer);

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Component* Contact::getAnchor() const
// **********************************
{
    Hook* masterHook = _anchorHook.getMasterHook();
    return (masterHook) ? masterHook->getComponent() : NULL;
}

void Contact::translate(const Unit& dx, const Unit& dy)
// ****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
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
        invalidate(false);
        _layer = layer;
    }
}

void Contact::SetWidth(const Unit& width)
// **************************************
{
    if (width != _width) {
        invalidate(false);
        _width = width;
    }
}

void Contact::SetHeight(const Unit& height)
// ****************************************
{
    if (height != _height) {
        invalidate(false);
        _height = height;
    }
}

void Contact::SetSizes(const Unit& width, const Unit& height)
// **********************************************************
{
    if ((width != _width) || (height != _height)) {
        invalidate(false);
        _width = width;
        _height = height;
    }
}

void Contact::SetX(const Unit& x)
// ******************************
{
    SetPosition(x, getY());
}

void Contact::SetY(const Unit& y)
// ******************************
{
    SetPosition(getX(), y);
}

void Contact::SetPosition(const Unit& x, const Unit& y)
// ****************************************************
{
    Component* anchor = getAnchor();
    if (!anchor)
        SetOffset(x, y);
    else
        SetOffset(x - anchor->getX(), y - anchor->getY());
}

void Contact::SetPosition(const Point& position)
// *********************************************
{
    SetPosition(position.getX(), position.getY());
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
        invalidate(true);
        _dx = dx;
        _dy = dy;
    }
}

void Contact::_preDestroy()
// ***********************
{
// trace << "entering Contact::PreDestroy " << this << endl;
// trace_in();

    Inherit::_preDestroy();

    _anchorHook.Detach();

// trace << "exiting Contact::PreDestroy" << endl;
// trace_out();
}

string Contact::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " [" + getValueString(getX()));
    s.insert(s.length() - 1, " " + getValueString(getY()));
    s.insert(s.length() - 1, "] " + getValueString(_width));
    s.insert(s.length() - 1, "x" + getValueString(_height));
    return s;
}

Record* Contact::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("AnchorHook", &_anchorHook));
        record->Add(getSlot("Anchor", getAnchor()));
        record->Add(getSlot("Layer", _layer));
        record->Add(getSlot("Dx", &_dx));
        record->Add(getSlot("Dy", &_dy));
        record->Add(getSlot("Width", &_width));
        record->Add(getSlot("Height", &_height));
    }
    return record;
}

Unit Contact::_getSize() const
// ***************************
{
    Unit size = 0;

    Layer* layer = getLayer();
    if (is_a<CompositeLayer*>(layer))
        size = ((CompositeLayer*)layer)->getMaximalContactSize();

    return size;
}

Unit Contact::_getSize(const BasicLayer* basicLayer) const
// *******************************************************
{
    Layer* layer = getLayer();

    if (!layer->contains(basicLayer)) return 0;

    Unit size = 0;

    if (is_a<CompositeLayer*>(layer))
        size = ((CompositeLayer*)layer)->getContactSize(basicLayer);

    return size;
}

// ****************************************************************************************************
// Contact::AnchorHook implementation
// ****************************************************************************************************

static int ANCHOR_HOOK_OFFSET = -1;

Contact::AnchorHook::AnchorHook(Contact* contact)
// **********************************************
:    Inherit()
{
    if (!contact)
        throw Error("Can't create " + _TName("Contact::AnchorHook") + " : null contact");

        if (ANCHOR_HOOK_OFFSET == -1)
        ANCHOR_HOOK_OFFSET = (unsigned long)this - (unsigned long)contact;
}

Component* Contact::AnchorHook::getComponent() const
// *************************************************
{
    return (Component*)((unsigned long)this - ANCHOR_HOOK_OFFSET);
}

string Contact::AnchorHook::_getString() const
// *******************************************
{
    return "<" + _TName("Contact::AnchorHook") + " " + getString(getComponent()) + ">";
}



// ****************************************************************************************************
// Contact_Hooks implementation
// ****************************************************************************************************

Contact_Hooks::Contact_Hooks(const Contact* contact)
// *************************************************
:     Inherit(),
    _contact(contact)
{
}

Contact_Hooks::Contact_Hooks(const Contact_Hooks& hooks)
// *****************************************************
:     Inherit(),
    _contact(hooks._contact)
{
}

Contact_Hooks& Contact_Hooks::operator=(const Contact_Hooks& hooks)
// ****************************************************************
{
    _contact = hooks._contact;
    return *this;
}

Collection<Hook*>* Contact_Hooks::getClone() const
// ***********************************************
{
    return new Contact_Hooks(*this);
}

Locator<Hook*>* Contact_Hooks::getLocator() const
// **********************************************
{
    return new Locator(_contact);
}

string Contact_Hooks::_getString() const
// *************************************
{
    string s = "<" + _TName("Contact::Hooks");
    if (_contact) s += " " + getString(_contact);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Contact_Hooks::Locator implementation
// ****************************************************************************************************

Contact_Hooks::Locator::Locator(const Contact* contact)
// ****************************************************
:    Inherit(),
    _contact(contact),
    _hook(NULL)
{
    if (_contact) _hook = ((Contact*)_contact)->getBodyHook();
}

Contact_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:    Inherit(),
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

Hook* Contact_Hooks::Locator::getElement() const
// *********************************************
{
    return _hook;
}

Locator<Hook*>* Contact_Hooks::Locator::getClone() const
// *****************************************************
{
    return new Locator(*this);
}

bool Contact_Hooks::Locator::isValid() const
// *****************************************
{
    return (_hook != NULL);
}

void Contact_Hooks::Locator::progress()
// ************************************
{
    if (_hook) {
        if (_hook == ((Contact*)_contact)->getBodyHook())
            _hook = ((Contact*)_contact)->getAnchorHook();
        else
            _hook = NULL;
    }
}

string Contact_Hooks::Locator::_getString() const
// **********************************************
{
    string s = "<" + _TName("Contact::Hooks::Locator");
    if (_contact) s += " " + getString(_contact);
    s += ">";
    return s;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
