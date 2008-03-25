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

        public: virtual Component* getComponent() const;

        public: virtual bool IsMaster() const {return false;};

        public: virtual string _getTypeName() const { return _TName("Contact::AnchorHook"); };
        public: virtual string _getString() const;
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

    public: static Contact* create(Net* net, Layer* layer, const Unit& x, const Unit& y, const Unit& width = 0, const Unit& height = 0);
    public: static Contact* create(Component* anchor, Layer* layer, const Unit& dx, const Unit& dy, const Unit& width = 0, const Unit& height = 0);

// Accessors
// *********

    public: virtual Hooks getHooks() const;
    public: virtual Unit getX() const;
    public: virtual Unit getY() const;
    public: virtual Point getPosition() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Layer* getLayer() const {return _layer;};
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: Hook* getAnchorHook() {return &_anchorHook;};
    public: Component* getAnchor() const;
    public: const Unit& getDx() const {return _dx;};
    public: const Unit& getDy() const {return _dy;};
    public: const Unit& getWidth() const {return _width;};
    public: Unit getHalfWidth() const {return (_width / 2);};
    public: const Unit& getHeight() const {return _height;};
    public: Unit getHalfHeight() const {return (_height / 2);};

// Updators
// ********

    public: virtual void translate(const Unit& dx, const Unit& dy);

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

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Contact");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: Unit _getSize() const;
    public: Unit _getSize(const BasicLayer* basicLayer) const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Contact)

#endif // HURRICANE_CONTACT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
