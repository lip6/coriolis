// ****************************************************************************************************
// File: Contact.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Contact.h"
#include "hurricane/Net.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Plug.h"
#include "hurricane/Error.h"

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

Contact::Contact(Net* net, const Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& height)
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

    if ( _width  < _layer->getMinimalSize() ) _width  = _layer->getMinimalSize();
    if ( _height < _layer->getMinimalSize() ) _height = _layer->getMinimalSize();
}

Contact::Contact(Net* net, Component* anchor, const Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width, const DbU::Unit& height)
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

    _anchorHook.attach(anchor->getBodyHook());

    if ( _width  < _layer->getMinimalSize() ) _width  = _layer->getMinimalSize();
    if ( _height < _layer->getMinimalSize() ) _height = _layer->getMinimalSize();
}

Contact* Contact::create(Net* net, const Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& height)
// ****************************************************************************************************
{
    Contact* contact = new Contact(net, layer, x, y, width, height);

    contact->_postCreate();

    return contact;
}

Contact* Contact::create(Component* anchor, const Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width, const DbU::Unit& height)
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

DbU::Unit Contact::getX() const
// ***********************
{
    Component* anchor = getAnchor();
    return (!anchor) ? _dx : anchor->getX() + _dx;
}

DbU::Unit Contact::getY() const
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
  DbU::Unit size = getLayer()->getEnclosure();

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Box Contact::getBoundingBox(const BasicLayer* basicLayer) const
// ******************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    DbU::Unit size = getLayer()->getEnclosure(basicLayer);

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Component* Contact::getAnchor() const
// **********************************
{
    Hook* masterHook = _anchorHook.getMasterHook();
    return (masterHook) ? masterHook->getComponent() : NULL;
}

void Contact::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _dx += dx;
        _dy += dy;
    }
}

void Contact::setLayer(const Layer* layer)
// *********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Contact::setWidth(const DbU::Unit& width)
// **************************************
{
    if (width != _width) {
        invalidate(false);
        _width = width;
    }
}

void Contact::setHeight(const DbU::Unit& height)
// ****************************************
{
    if (height != _height) {
        invalidate(false);
        _height = height;
    }
}

void Contact::setSizes(const DbU::Unit& width, const DbU::Unit& height)
// **********************************************************
{
    if ((width != _width) || (height != _height)) {
        invalidate(false);
        _width = width;
        _height = height;
    }
}

void Contact::setX(const DbU::Unit& x)
// ******************************
{
    setPosition(x, getY());
}

void Contact::setY(const DbU::Unit& y)
// ******************************
{
    setPosition(getX(), y);
}

void Contact::setPosition(const DbU::Unit& x, const DbU::Unit& y)
// ****************************************************
{
    Component* anchor = getAnchor();
    if (!anchor)
        setOffset(x, y);
    else
        setOffset(x - anchor->getX(), y - anchor->getY());
}

void Contact::setPosition(const Point& position)
// *********************************************
{
    setPosition(position.getX(), position.getY());
}

void Contact::setDx(const DbU::Unit& dx)
// ********************************
{
    setOffset(dx, _dy);
}

void Contact::setDy(const DbU::Unit& dy)
// ********************************
{
    setOffset(_dx, dy);
}

void Contact::setOffset(const DbU::Unit& dx, const DbU::Unit& dy)
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

    _anchorHook.detach();

// trace << "exiting Contact::PreDestroy" << endl;
// trace_out();
}

string Contact::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " [" + DbU::getValueString(getX()));
    s.insert(s.length() - 1, " " + DbU::getValueString(getY()));
    s.insert(s.length() - 1, "] " + DbU::getValueString(_width));
    s.insert(s.length() - 1, "x" + DbU::getValueString(_height));
    return s;
}

Record* Contact::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("AnchorHook", &_anchorHook));
        record->add(getSlot("Anchor", getAnchor()));
        record->add(getSlot("Layer", _layer));
        record->add(getSlot("Dx", &_dx));
        record->add(getSlot("Dy", &_dy));
        record->add(getSlot("Width", &_width));
        record->add(getSlot("Height", &_height));
    }
    return record;
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
