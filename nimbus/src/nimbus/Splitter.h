
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


// ***********************************************************************
// File: Splitter.h
// ***********************************************************************

#ifndef __NIMBUS_SPLITTER__
#define __NIMBUS_SPLITTER__

#include "hurricane/Component.h"
#include "nimbus/Splitters.h"

namespace Nimbus {

  using namespace Hurricane;

  class SplitterContact;
  class Fence;
  class GCell;


// ***********************************************************************
// Splitter declaration
// ***********************************************************************

class Splitter : public Component {
// *****************************

// Types
// *****

    public: typedef Component Inherit;

    private: typedef enum {
                 Horizontal = 1,
                 Vertical   = 2,
                 undefined  = 64,
             } SplitterOrientation;

// Attributes
// **********

   private: Layer* _layer;
   private: SplitterOrientation _direction;
   private: DbU::Unit _dx;
   private: DbU::Unit _dy;
   private: DbU::Unit _width;
   private: DbU::Unit _height;
   private: DbU::Unit _halfLength;
   private: SplitterContact* _c1;
   private: SplitterContact* _c2;
   private: Fence* _fence;

// Constructors
// ************

    protected: Splitter(Net* net, Layer* layer, const SplitterOrientation direction, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width = 0, const DbU::Unit& height = 0, const DbU::Unit& halfLength = 2);

    public: static Splitter* create(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: static Splitter* create(Net* net, Layer* layer, Fence* fence, GCell* gc1, GCell* gc2, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: static Splitter* createHorizontal(Net* net, Layer* layer, Fence* fence, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: static Splitter* createHorizontal(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: static Splitter* createVertical(Net* net, Layer* layer, Fence* fence, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: static Splitter* createVertical(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& halfLength = 2, const DbU::Unit& width = 0, const DbU::Unit& height = 0);
    public: void _postCreate ();
    public: void _attachTo (Fence* fence, GCell* gc1, GCell* gc2);

// Accessors
// *********

    public: virtual Hooks getHooks() const;
    public: SplitterContact* getSplitterContact1() const { return _c1; };
    public: SplitterContact* getSplitterContact2() const { return _c2; };
    public: Hook* getHook1() const;
    public: Hook* getHook2() const;
    public: Hook* getLeftHook() const;
    public: Hook* getRightHook() const;
    public: Hook* getUpHook() const;
    public: Hook* getTopHook() const;
    public: Hook* getBottomHook() const;
    public: Hook* getDownHook() const;
    public: Hook* getOppositeBodyHook(const Hook* hook) const;
    public: bool isHorizontal() const { return _direction == Horizontal; };
    public: bool isVertical() const { return _direction == Vertical; };
    public: virtual DbU::Unit getX() const;
    public: virtual DbU::Unit getY() const;
    public: virtual Point getPosition() const;
    public: virtual Box getBoundingBox() const;
    public: virtual const Layer* getLayer() const {return _layer;};
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: const DbU::Unit& getDx() const {return _dx;};
    public: const DbU::Unit& getDy() const {return _dy;};
    public: const DbU::Unit& getWidth() const {return _width;};
    public: DbU::Unit getHalfWidth() const {return (_width / 2);};
    public: const DbU::Unit& getHeight() const {return _height;};
    public: DbU::Unit getHalfHeight() const {return (_height / 2);};
    public: void onDeletedFence();
    public: Fence* getFence() const { return _fence; };
    public: void setFence(Fence* fence);

// Updators
// ********

    public: virtual void invalidate(bool propagateFlag = true);
    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

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

    public: virtual string _getTypeName() const {return _TName("Splitter");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: DbU::Unit _getSize() const;
    public: DbU::Unit _getSize(const BasicLayer* basicLayer) const;

};

} // namespace nimbus

//typedef set<Nimbus::Splitter*> SplitterSet;


#endif // HURRICANE_SPLITTER
