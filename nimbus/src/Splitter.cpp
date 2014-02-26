
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


// ****************************************************************************************************
// File: Splitter.cpp
// ****************************************************************************************************

#include "hurricane/Net.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/ContactLayer.h"
#include "hurricane/Plug.h"
#include "hurricane/Error.h"
#include "hurricane/Collection.h"
#include "nimbus/Fence.h"
#include "nimbus/HFence.h"
#include "nimbus/VFence.h"
#include "nimbus/GCell.h"
#include "nimbus/GCells.h"
#include "nimbus/Splitter.h"
#include "nimbus/SplitterContact.h"

namespace Nimbus {

  using namespace std;
  using namespace Hurricane;

// ****************************************************************************************************
// Splitter_Hooks declaration
// ****************************************************************************************************

class Splitter_Hooks : public Collection<Hook*> {
// *******************************************

// Types
// *****

    public: typedef Collection<Hook*> Inherit;

    public: class Locator : public Hurricane::Locator<Hook*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Hook*> Inherit;

        private: const Splitter* _splitter;
        private: Hook* _hook;

        public: Locator(const Splitter* splitter = NULL);
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

    private: const Splitter* _splitter;

// Constructors
// ************

    public: Splitter_Hooks(const Splitter* splitter = NULL);
    public: Splitter_Hooks(const Splitter_Hooks& hooks);

// Operators
// *********

    public: Splitter_Hooks& operator=(const Splitter_Hooks& hooks);

// Accessors
// *********

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Splitter implementation
// ****************************************************************************************************

Splitter::Splitter(Net* net, Layer* layer, const SplitterOrientation direction, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& height, const DbU::Unit& halfLength)
// ****************************************************************************************************
:  Inherit(net),
    _layer(layer),
    _direction (direction),
    _dx(x),
    _dy(y),
    _width(width),
    _height(height),
    _halfLength(halfLength),
    _c1(NULL),
    _c2(NULL),
    _fence(NULL)
{
    if (!_layer)
        throw Error("Can't create " + _TName("Splitter") + " : null layer");
}

Splitter* Splitter::create(
        Net* net,
        Layer* layer,
        const DbU::Unit& x,
        const DbU::Unit& y,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    Splitter* splitter = new Splitter(net, layer, undefined, x, y, width, height, halfLength);

    splitter->_postCreate();

    return splitter;
}

Splitter* Splitter::create(
        Net* net,
        Layer* layer,
        Fence* fence,
        GCell* gc1,
        GCell* gc2,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    Splitter* splitter =
        new Splitter(
            net,
            layer,
            (
             (dynamic_cast<HFence*> (fence)) ?
             Vertical :
             Horizontal
            ),
            fence->getXCenter(),
            fence->getYCenter(),
            width,
            height,
            halfLength
            )
        ;

    splitter->_postCreate();
    splitter->setFence(fence);
    //splitter->_attachTo(fence, gc1, gc2);
    
    return splitter;
}

// ********************************************************************

Splitter* Splitter::createHorizontal(
        Net* net,
        Layer* layer,
        Fence* fence,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    assert (fence);

    Splitter* splitter = new Splitter(net, layer, Horizontal, fence->getXCenter(), fence->getY(), halfLength, width, height);

    splitter->_postCreate();
    splitter->setFence(fence);

    return splitter;
}

Splitter* Splitter::createHorizontal(
        Net* net,
        Layer* layer,
        const DbU::Unit& x,
        const DbU::Unit& y,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    Splitter* splitter = new Splitter(net, layer, Horizontal, x, y, halfLength, width, height);

    splitter->_postCreate();

    return splitter;
}

// ********************************************************************

Splitter* Splitter::createVertical(
        Net* net,
        Layer* layer,
        Fence* fence,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    assert (fence);

    Splitter* splitter = new Splitter(net, layer, Vertical, fence->getX(), fence->getYCenter(), halfLength, width, height);

    splitter->_postCreate();
    splitter->setFence(fence);

    return splitter;
}

Splitter* Splitter::createVertical(
        Net* net,
        Layer* layer,
        const DbU::Unit& x,
        const DbU::Unit& y,
        const DbU::Unit& halfLength,
        const DbU::Unit& width,
        const DbU::Unit& height)
{
    Splitter* splitter = new Splitter(net, layer, Vertical, x, y, halfLength, width, height);

    splitter->_postCreate();

    return splitter;
}

// ********************************************************************


void Splitter::_postCreate ()
// ****************************************************************************************************
{
    if (_direction == Horizontal) {
        _c1 = SplitterContact::create (this, _layer, -_halfLength, 0, _width, _height); // XXX FIXME
        _c2 = SplitterContact::create (this, _layer,  _halfLength, 0, _width, _height); // XXX FIXME
    } else if (_direction == Vertical) {
        _c1 = SplitterContact::create (this, _layer, 0,  _halfLength, _width, _height); // XXX FIXME
        _c2 = SplitterContact::create (this, _layer, 0, -_halfLength, _width, _height); // XXX FIXME
    } else {
        _c1 = SplitterContact::create (this, _layer, 0, 0, _width, _height); // XXX FIXME
        _c2 = SplitterContact::create (this, _layer, 0, 0, _width, _height); // XXX FIXME
    }

    Inherit::_postCreate();
    /* class RoutShell -> tiens en main la largeur de segments, contacts, splitter, layer de routage etc. */


    return;
}

void Splitter::_attachTo(Fence* fence, GCell* gc1, GCell* gc2)
{
    _fence = fence;
    _c1->setGCell(gc1);
    _c2->setGCell(gc2);
    return;
}

void Splitter::setFence(Fence* fence)
{
    //if (_fence)
    //    throw Error ("Splitter is already attached to a Fence");

    _fence = fence;

    _c1->setGCell(_fence->getGCell1());
    _c2->setGCell(_fence->getGCell2());

    return;
}

void Splitter::onDeletedFence() {

    this->destroy();


    return;
}

Hook* Splitter::getHook1() const {
    return (_c1->getBodyHook());
}

Hook* Splitter::getHook2() const {
    return (_c2->getBodyHook());
}

Hook* Splitter::getLeftHook () const {
    return getHook1();
}
Hook* Splitter::getBottomHook () const {
    return getHook2();
}
Hook* Splitter::getDownHook () const {
    return getHook2();
}
Hook* Splitter::getRightHook () const {
    return getHook2();
}
Hook* Splitter::getUpHook () const {
    return getHook1();
}
Hook* Splitter::getTopHook () const {
    return getHook1();
}


Hook* Splitter::getOppositeBodyHook(const Hook* hook) const
// **********************************************
{
    if (hook) {
        if (hook == _c1->getBodyHook()) return _c2->getBodyHook();
        if (hook == _c2->getBodyHook()) return _c1->getBodyHook();
    }
    return NULL;
}

Hooks Splitter::getHooks() const
// ****************************
{
    return Splitter_Hooks(this);
}

DbU::Unit Splitter::getX() const
// ***********************
{
    return _dx;
}

DbU::Unit Splitter::getY() const
// ***********************
{
    return _dy;
}

Point Splitter::getPosition() const
// *******************************
{
    return Point(_dx, _dy);
}

Box Splitter::getBoundingBox() const
// ********************************
{
    DbU::Unit size = _getSize();

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Box Splitter::getBoundingBox(const BasicLayer* basicLayer) const
// ******************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    DbU::Unit size = _getSize(basicLayer);

    return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

void Splitter::invalidate(bool propagateFlag)
// ************************
{
    Inherit::invalidate(propagateFlag);
    _c1->invalidate(true);
    _c2->invalidate(true);
    return;
}

void Splitter::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _dx += dx;
        _dy += dy;
    }
}

void Splitter::setLayer(Layer* layer)
// *********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Splitter::setWidth(const DbU::Unit& width)
// **************************************
{
    if (width != _width) {
        invalidate(false);
        _width = width;
    }
}

void Splitter::setHeight(const DbU::Unit& height)
// ****************************************
{
    if (height != _height) {
        invalidate(false);
        _height = height;
    }
}

void Splitter::setSizes(const DbU::Unit& width, const DbU::Unit& height)
// **********************************************************
{
    if ((width != _width) || (height != _height)) {
        invalidate(false);
        _width = width;
        _height = height;
    }
}

void Splitter::setX(const DbU::Unit& x)
// ******************************
{
    setPosition(x, getY());
}

void Splitter::setY(const DbU::Unit& y)
// ******************************
{
    setPosition(getX(), y);
}

void Splitter::setPosition(const DbU::Unit& x, const DbU::Unit& y)
// ****************************************************
{
    setOffset(x, y);
}

void Splitter::setPosition(const Point& position)
// *********************************************
{
    setPosition(position.getX(), position.getY());
}

void Splitter::setDx(const DbU::Unit& dx)
// ********************************
{
    setOffset(dx, _dy);
}

void Splitter::setDy(const DbU::Unit& dy)
// ********************************
{
    setOffset(_dx, dy);
}

void Splitter::setOffset(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
    if ((dx != _dx) || (dy != _dy)) {
        invalidate(true);
        _dx = dx;
        _dy = dy;
    }
}

void Splitter::_preDestroy()
// ***********************
{
    Inherit::_preDestroy();
    if (_fence) _fence->detachSplitter(this);
}

string Splitter::_getString() const
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

Record* Splitter::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Layer", _layer));
        
        record->add(getSlot("SplitterContact 1", _c1));
        record->add(getSlot("SplitterContact 2", _c2));
        record->add(getSlot("Dx", &_dx));
        record->add(getSlot("Dy", &_dy));
        record->add(getSlot("Width", &_width));
        record->add(getSlot("Height", &_height));

        record->add(getSlot("Fence", _fence));
        record->add(getSlot("GCell1", _c1->getGCell()));
        record->add(getSlot("GCell2", _c2->getGCell()));
    }
    return record;
}

DbU::Unit Splitter::_getSize() const
// ***************************
{

    //Layer* layer = getLayer();
    //if (is_a<CompositeLayer*>(layer))
    //    size = ((CompositeLayer*)layer)->getMaximalSplitterSize();

    return (_height < _width ? _width : _height);
}

DbU::Unit Splitter::_getSize(const BasicLayer* basicLayer) const
// *************************************************
{
    const Layer* layer = getLayer();

    if (!layer->contains(basicLayer)) return 0;

    //if (is_a<CompositeLayer*>(layer))
    //    size = ((CompositeLayer*)layer)->getSplitterSize(basicLayer);

    return _getSize();
}

// void Splitter::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
// // ****************************************************************************************************
// {
//     if (_width && _height) {
//         if (1 < view->getScreenSize(max(_width, _height)))
//             basicLayer->_Fill(view, transformation.getBox(getBoundingBox(basicLayer)));
//     }
//     if (basicLayer == _layer->_getSymbolicBasicLayer()) {
//         if (view->CutPointsAreVisible() && (3 < view->getScale())) {
//             Point position = getPosition();
//             view->DrawPoint(transformation.getPoint(position), 3);
//             if (_width) {
//                 Box box = transformation.getBox(Box(position).inflate(getHalfWidth(), 0));
//                 view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//             }
//             if (_height) {
//                 Box box = transformation.getBox(Box(position).inflate(0, getHalfHeight()));
//                 view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//             }
//         }
//     }
// }

// void Splitter::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
// // **********************************************************************************************
// {
//     if (_width && _height) {
//         if (1 < view->getScreenSize(max(_width, _height))) {
//             for_each_basic_layer(basicLayer, getLayer()->getBasicLayers()) {
//                 basicLayer->_Fill(view, transformation.getBox(getBoundingBox(basicLayer)));
//                 end_for;
//             }
//         }
//     }
//     if (view->getScale() <= 1)
//         view->DrawPoint(transformation.getPoint(getPosition()), 1);
//     else if (view->getScale() <= 3)
//     {
//         view->DrawPoint(transformation.getPoint(getPosition()), 2);

//         if ( view->isTextVisible() )
//         {
//             string text = "("
//                         + getString ( getValue ( getX() ) ) + ","
//                         + getString ( getValue ( getY() ) ) + ") : Fence("
//                         + getString ( getValue ( _fence->getXMin() ) ) + ":" + getString ( getValue ( _fence->getXMax() ) ) + ","
//                         + getString ( getValue ( _fence->getY() ) ) + ":" + getString ( getValue ( _fence->getYMax() ) ) + ")";
//             view->DrawString ( text,
//                                transformation.getBox ( getBoundingBox() ).getXMin(),
//                                transformation.getBox ( getBoundingBox() ).getYMax() );
//         }
//     }
//     else {
//         Point position = getPosition();
//         view->DrawPoint(transformation.getPoint(position), 3);
//         if (_width) {
//             Box box = transformation.getBox(Box(position).inflate(getHalfWidth(), 0));
//             view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//         }
//         if (_height) {
//             Box box = transformation.getBox(Box(position).inflate(0, getHalfHeight()));
//             view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
//         }

//         if ( view->isTextVisible() )
//         {
//             string text = "("
//                         + getString ( getValue ( getX() ) ) + ","
//                         + getString ( getValue ( getY() ) ) + ") : Fence("
//                         + getString ( getValue ( _fence->getXMin() ) ) + ":" + getString ( getValue ( _fence->getXMax() ) ) + ","
//                         + getString ( getValue ( _fence->getY() ) ) + ":" + getString ( getValue ( _fence->getYMax() ) ) + ")";
//             view->DrawString ( text,
//                                transformation.getBox ( getBoundingBox() ).getXMin(),
//                                transformation.getBox ( getBoundingBox() ).getYMax() );
//         }
//     }
//     view->DrawRectangle ( transformation.getBox ( getBoundingBox() ) );
// }

// void Splitter::_SaveHeaderTo(OutputFile& outputFile)
// // ************************************************
// {
//     Inherit::_SaveHeaderTo(outputFile);

//     outputFile << " " << outputFile.getId(getLayer());
//     outputFile << " " << getValueString(getDx());
//     outputFile << " " << getValueString(getDy());
//     outputFile << " " << getValueString(getWidth());
//     outputFile << " " << getValueString(getHeight());
// }



// ****************************************************************************************************
// Splitter_Hooks implementation
// ****************************************************************************************************

Splitter_Hooks::Splitter_Hooks(const Splitter* splitter)
// *************************************************
:     Inherit(),
    _splitter(splitter)
{
}

Splitter_Hooks::Splitter_Hooks(const Splitter_Hooks& hooks)
// *****************************************************
:     Inherit(),
    _splitter(hooks._splitter)
{
}

Splitter_Hooks& Splitter_Hooks::operator=(const Splitter_Hooks& hooks)
// ****************************************************************
{
    _splitter = hooks._splitter;
    return *this;
}

Collection<Hook*>* Splitter_Hooks::getClone() const
// ***********************************************
{
    return new Splitter_Hooks(*this);
}

Locator<Hook*>* Splitter_Hooks::getLocator() const
// **********************************************
{
    return new Locator(_splitter);
}

string Splitter_Hooks::_getString() const
// *************************************
{
    string s = "<" + _TName("Splitter::Hooks");
    if (_splitter) s += " " + getString(_splitter);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Splitter_Hooks::Locator implementation
// ****************************************************************************************************

Splitter_Hooks::Locator::Locator(const Splitter* splitter)
// ****************************************************
:    Inherit(),
    _splitter(splitter),
    _hook(NULL)
{
    if (_splitter) _hook = ((Splitter*)_splitter)->getBodyHook();
}

Splitter_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:    Inherit(),
    _splitter(locator._splitter),
    _hook(locator._hook)
{
}

Splitter_Hooks::Locator& Splitter_Hooks::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
    _splitter = locator._splitter;
    _hook = locator._hook;
    return *this;
}

Hook* Splitter_Hooks::Locator::getElement() const
// *********************************************
{
    return _hook;
}

Locator<Hook*>* Splitter_Hooks::Locator::getClone() const
// *****************************************************
{
    return new Locator(*this);
}

bool Splitter_Hooks::Locator::isValid() const
// *****************************************
{
    return (_hook != NULL);
}

void Splitter_Hooks::Locator::progress()
// ************************************
{
    if (_hook) {
        //if (_hook = ((Splitter*)_splitter)->getBodyHook())
        //if (_hook = ((Splitter*)_splitter)->getLeftHook())
        //        _hook = ((Splitter*)_splitter)->getRightHook();
        //else
        //        _hook = ((Splitter*)_splitter)->getLeftHook();
        //else
            _hook = NULL;
#if 0
        if (_hook == ((Splitter*)_splitter)->getBodyHook())
            _hook = ((Splitter*)_splitter)->getAnchorHook();
        else
#endif
#if 0
            _hook = NULL;
#endif
    }
}

string Splitter_Hooks::Locator::_getString() const
// **********************************************
{
    string s = "<" + _TName("Splitter::Hooks::Locator");
    if (_splitter) s += " " + getString(_splitter);
    s += ">";
    return s;
}

}
