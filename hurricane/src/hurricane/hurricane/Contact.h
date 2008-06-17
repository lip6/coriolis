// ****************************************************************************************************
// File: Contact.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_CONTACT
#define HURRICANE_CONTACT

#include "hurricane/Component.h"
#include "hurricane/Contacts.h"

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

        public: virtual bool isMaster() const {return false;};

        public: virtual string _getTypeName() const { return _TName("Contact::AnchorHook"); };
        public: virtual string _getString() const;
    };

// Attributes
// **********

    private:   AnchorHook _anchorHook;
    private:   const Layer* _layer;
    private:   DbU::Unit _dx;
    private:   DbU::Unit _dy;
    protected: DbU::Unit _width;
    protected: DbU::Unit _height;

// Constructors
// ************

  protected: Contact( Net* net
                    , const Layer* layer
                    , const DbU::Unit& x
                    , const DbU::Unit& y
                    , const DbU::Unit& width  = 0
                    , const DbU::Unit& height = 0
                    );
  protected: Contact( Net* net
                    , Component* anchor
                    , const Layer* layer
                    , const DbU::Unit& dx
                    , const DbU::Unit& dy
                    , const DbU::Unit& width  = 0
                    , const DbU::Unit& height = 0
                    );

  public: static Contact* create( Net* net
                                , const Layer* layer
                                , const DbU::Unit& x
                                , const DbU::Unit& y
                                , const DbU::Unit& width  = 0
                                , const DbU::Unit& height = 0
                                );
  public: static Contact* create( Component* anchor
                                , const Layer* layer
                                , const DbU::Unit& dx
                                , const DbU::Unit& dy
                                , const DbU::Unit& width  = 0
                                , const DbU::Unit& height = 0
                                );

// Accessors
// *********

    public: virtual Hooks getHooks() const;
    public: virtual DbU::Unit getX() const;
    public: virtual DbU::Unit getY() const;
    public: virtual Point getPosition() const;
    public: virtual Box getBoundingBox() const;
    public: virtual const Layer* getLayer() const {return _layer;};
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: Hook* getAnchorHook() {return &_anchorHook;};
    public: Component* getAnchor() const;
    public: const DbU::Unit& getDx() const {return _dx;};
    public: const DbU::Unit& getDy() const {return _dy;};
    public: const DbU::Unit& getWidth() const {return _width;};
    public: DbU::Unit getHalfWidth() const {return (_width / 2);};
    public: const DbU::Unit& getHeight() const {return _height;};
    public: DbU::Unit getHalfHeight() const {return (_height / 2);};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setLayer(const Layer* layer);
    public: void setWidth(const DbU::Unit& width);
    public: void setHeight(const DbU::Unit& height);
    public: void setSizes(const DbU::Unit& width, const DbU::Unit& height);
    public: void setX(const DbU::Unit& x);
    public: void setY(const DbU::Unit& y);
    public: void setPosition(const DbU::Unit& x, const DbU::Unit& y);
    public: void setPosition(const Point& position);
    public: void setDx(const DbU::Unit& dx);
    public: void setDy(const DbU::Unit& dy);
    public: void setOffset(const DbU::Unit& dx, const DbU::Unit& dy);

// Others
// ******

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("Contact");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Contact);
INSPECTOR_P_SUPPORT(Hurricane::Contact::AnchorHook);


#endif // HURRICANE_CONTACT

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
