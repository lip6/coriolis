//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_SPLITTERCONTACT__
#define __NIMBUS_SPLITTERCONTACT__

#include "hurricane/Component.h"
#include "crlcore/RoutingLayerGauge.h"
#include "nimbus/GCells.h"
#include "nimbus/SplitterContacts.h"


namespace Nimbus {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;

  class Splitter;


// ****************************************************************************************************
// SplitterContact declaration
// ****************************************************************************************************

class SplitterContact : public Component {
// *************************************

// Types
// *****

    public: typedef Component Inherit;

    public: class AnchorHook : public Hook {
    // ***********************************

        friend class SplitterContact;

        public: typedef Hook Inherit;

        private: AnchorHook(SplitterContact* splittercontact);

        public: virtual Component* getComponent() const;

        public: virtual bool isMaster() const {return false;};

        public: virtual string _getTypeName() const { return _TName("SplitterContact::AnchorHook"); };
        public: virtual string _getString() const;
    };

    enum SPType { SP_1      = (1 << 2)
                , SP_2      = (1 << 3)
                , SP_LEFT   = SP_1 | Constant::Horizontal
                , SP_RIGHT  = SP_2 | Constant::Horizontal
                , SP_TOP    = SP_1 | Constant::Vertical
                , SP_BOTTOM = SP_2 | Constant::Vertical
                };


// Attributes
// **********

    private: AnchorHook _anchorHook;
    private: Layer* _layer;
    private: DbU::Unit _dx;
    private: DbU::Unit _dy;
    private: DbU::Unit _width;
    private: DbU::Unit _height;
    private: GCell* _gcell;

    private: unsigned _oppositePlugCount;
    private: unsigned _oppositeSplitterCount;
    private: unsigned _oppositePinCount;

// Constructors
// ************

    protected: SplitterContact(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width = 0, const DbU::Unit& height = 0, GCell* gcell = NULL);
    protected: SplitterContact(Net* net, Component* anchor, Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width = 0, const DbU::Unit& height = 0, GCell* gcell = NULL);

    public: static SplitterContact* create(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width = 0, const DbU::Unit& height = 0, GCell* gcell = NULL);
    public: static SplitterContact* create(Component* anchor, Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width = 0, const DbU::Unit& height = 0, GCell* gcell = NULL);

// Accessors
// *********

    public: Splitter* getSplitter () const;
    public: Hook* getOppositeBodyHook ();// const;
    public: SplitterContact* getOppositeSplitterContact ();// const;

    public: unsigned getOppositeSplitterCount();
    public: unsigned getOppositePlugCount();
    public: unsigned getOppositePinCount();
    private: void computeOppositeCounts();
    public: void invalidateOppositeCounts();

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
    public: GCell* getGCell() const { return _gcell; };
    public: GCells getSubGCells();
    //public: GCells getSubPlacementLeaves();
    public: unsigned int getSPType();

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setGCell(GCell* gcell);
    public: void setLayer(Layer* layer);
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

    public: virtual string _getTypeName() const {return _TName("SplitterContact");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: DbU::Unit _getSize() const;
    public: DbU::Unit _getSize(const BasicLayer* basicLayer) const;

};


string getStringDir ( unsigned int direction );

}



#endif // __NIMBUS_SPLITTERCONTACT__
