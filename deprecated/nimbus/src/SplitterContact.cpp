//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>

#include <limits>

#include <limits>
#include "hurricane/Net.h"
#include "hurricane/ContactLayer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Plug.h"
#include "hurricane/Pin.h"
#include "hurricane/Error.h"
#include "nimbus/Splitter.h"
#include "nimbus/GCell.h"
#include "nimbus/SplitterContact.h"
#include "nimbus/RoutTools.h"

namespace Nimbus {

  using namespace Hurricane;

// ****************************************************************************************************
// SplitterContact_Hooks declaration
// ****************************************************************************************************

class SplitterContact_Hooks : public Collection<Hook*> {
// *******************************************

// Types
// *****

	public: typedef Collection<Hook*> Inherit;

	public: class Locator : public Hurricane::Locator<Hook*> {
	// *****************************************************

		public: typedef Hurricane::Locator<Hook*> Inherit;

		private: const SplitterContact* _splittercontact;
		private: Hook* _hook;

		public: Locator(const SplitterContact* splittercontact = NULL);
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

	private: const SplitterContact* _splittercontact;

// Constructors
// ************

	public: SplitterContact_Hooks(const SplitterContact* splittercontact = NULL);
	public: SplitterContact_Hooks(const SplitterContact_Hooks& hooks);

// Operators
// *********

	public: SplitterContact_Hooks& operator=(const SplitterContact_Hooks& hooks);

// Accessors
// *********

	public: virtual Collection<Hook*>* getClone() const;
	public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

	public: virtual string _getString() const;

};




// ****************************************************************************************************
// SplitterContact implementation
// ****************************************************************************************************

SplitterContact::SplitterContact(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& height, GCell* gcell)
// ****************************************************************************************************
:  Inherit(net),
	_anchorHook(this),
	_layer(layer),
	_dx(x),
	_dy(y),
	_width(width),
	_height(height),
        _gcell(gcell),
        _oppositePlugCount((std::numeric_limits<unsigned>::max)()),
        _oppositeSplitterCount((std::numeric_limits<unsigned>::max)()),
        _oppositePinCount((std::numeric_limits<unsigned>::max)())
{
	if (!_layer)
		throw Error("Can't create " + _TName("SplitterContact") + " : null layer");
}

SplitterContact::SplitterContact(Net* net, Component* anchor, Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width, const DbU::Unit& height, GCell* gcell)
// ****************************************************************************************************
:  Inherit(net),
	_anchorHook(this),
	_layer(layer),
	_dx(dx),
	_dy(dy),
	_width(width),
	_height(height),
        _gcell(gcell),
        _oppositePlugCount((std::numeric_limits<unsigned>::max)()),
        _oppositeSplitterCount((std::numeric_limits<unsigned>::max)()),
        _oppositePinCount((std::numeric_limits<unsigned>::max)())
    
{
	if (!anchor)
		throw Error("Can't create " + _TName("SplitterContact") + " : null anchor");

	if (!anchor->getNet())
		throw Error("Can't create " + _TName("SplitterContact") + " : unconnected anchor");

	if (anchor->getNet() != getNet())
		throw Error("Can't create " + _TName("SplitterContact") + " : incompatible anchor");

	if (!_layer)
		throw Error("Can't create " + _TName("SplitterContact") + " : null layer");

	_anchorHook.attach(anchor->getBodyHook());
}

SplitterContact* SplitterContact::create(Net* net, Layer* layer, const DbU::Unit& x, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& height, GCell* gcell)
// ****************************************************************************************************
{
	SplitterContact* splittercontact = new SplitterContact(net, layer, x, y, width, height, gcell);

	splittercontact->_postCreate();

	return splittercontact;
}

SplitterContact* SplitterContact::create(Component* anchor, Layer* layer, const DbU::Unit& dx, const DbU::Unit& dy, const DbU::Unit& width, const DbU::Unit& height, GCell* gcell)
// ****************************************************************************************************
{
	if (!anchor)
		throw Error("Can't create " + _TName("SplitterContact") + " : null anchor");

	SplitterContact* splittercontact = new SplitterContact(anchor->getNet(), anchor, layer, dx, dy, width, height, gcell);

	splittercontact->_postCreate();

	return splittercontact;
}

Splitter* SplitterContact::getSplitter () const
// ********************************************
{
    Component* compo = getAnchor ();

    if (!compo) {
        throw Error("Encounter of an unattached SplitterContact");
    }

    if (Splitter* splitter = dynamic_cast<Splitter*>(compo))
        return splitter;
    else 
        throw Error("A SplitterContact is parasiting a component other than a Splitter");
}

Hook* SplitterContact::getOppositeBodyHook() //const
// ***********************************************
{
    Hook* h = getSplitter()->getOppositeBodyHook (getBodyHook());

    return h;
}

SplitterContact* SplitterContact::getOppositeSplitterContact () //const
// *******************************************************
{
    Component* compo = getOppositeBodyHook () -> getComponent ();
    assert (compo);
    if (SplitterContact* sc = dynamic_cast<SplitterContact*>(compo))
        return sc;
    else
        throw Error ("Unexpected component at the opposite side of the Splitter");
}

GCells SplitterContact::getSubGCells() // const
// ***************************************
{
    GCells subs = getGCell()->getSubGCells();
    return subs.getSubSet(getGCellFenceFilter(getSplitter()->getFence()));
}

unsigned int SplitterContact::getSPType()
// **************************************
{
    Splitter*    splitter = getSplitter();
    assert ( splitter );
    unsigned int spType;

    if ( splitter->isHorizontal() ) spType = Constant::Horizontal;
    else                            spType = Constant::Vertical;

    if ( splitter->getSplitterContact1() == this ) spType |= SP_1;
    else                                           spType |= SP_2;

    return spType;
}

Hooks SplitterContact::getHooks() const
// ****************************
{
	return SplitterContact_Hooks(this);
}

DbU::Unit SplitterContact::getX() const
// ***********************
{
	Component* anchor = getAnchor();
	return (!anchor) ? _dx : anchor->getX() + _dx;
}

DbU::Unit SplitterContact::getY() const
// ***********************
{
	Component* anchor = getAnchor();
	return (!anchor) ? _dy : anchor->getY() + _dy;
}

Point SplitterContact::getPosition() const
// *******************************
{
	Component* anchor = getAnchor();
	return (!anchor) ? Point(_dx, _dy) : anchor->getPosition().translate(_dx, _dy);
}

Box SplitterContact::getBoundingBox() const
// ********************************
{
	DbU::Unit size = _getSize();

	return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Box SplitterContact::getBoundingBox(const BasicLayer* basicLayer) const
// *************************************************************
{
  if (!_layer->contains(basicLayer)) return Box();

	DbU::Unit size = _getSize(basicLayer);

	return Box(getPosition()).inflate(getHalfWidth() + size, getHalfHeight() + size);
}

Component* SplitterContact::getAnchor() const
// **********************************
{
	Hook* masterHook = _anchorHook.getMasterHook();
	return (masterHook) ? masterHook->getComponent() : NULL;
}

void SplitterContact::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
	if ((dx != 0) || (dy != 0)) {
		invalidate(true);
		_dx += dx;
		_dy += dy;
	}
}

void SplitterContact::setGCell(GCell* gcell)
// **************************************************
{
    _gcell = gcell;
    return;
}

void SplitterContact::setLayer(Layer* layer)
// *********************************
{
	if (!layer)
		throw Error("Can't set layer : null layer");

	if (layer != _layer) {
		invalidate(false);
		_layer = layer;
	}
}

void SplitterContact::setWidth(const DbU::Unit& width)
// **************************************
{
	if (width != _width) {
		invalidate(false);
		_width = width;
	}
}

void SplitterContact::setHeight(const DbU::Unit& height)
// ****************************************
{
	if (height != _height) {
		invalidate(false);
		_height = height;
	}
}

void SplitterContact::setSizes(const DbU::Unit& width, const DbU::Unit& height)
// **********************************************************
{
	if ((width != _width) || (height != _height)) {
		invalidate(false);
		_width = width;
		_height = height;
	}
}

void SplitterContact::setX(const DbU::Unit& x)
// ******************************
{
	setPosition(x, getY());
}

void SplitterContact::setY(const DbU::Unit& y)
// ******************************
{
	setPosition(getX(), y);
}

void SplitterContact::setPosition(const DbU::Unit& x, const DbU::Unit& y)
// ****************************************************
{
	Component* anchor = getAnchor();
	if (!anchor)
		setOffset(x, y);
	else
		setOffset(x - anchor->getX(), y - anchor->getY());
}

void SplitterContact::setPosition(const Point& position)
// *********************************************
{
	setPosition(position.getX(), position.getY());
}

void SplitterContact::setDx(const DbU::Unit& dx)
// ********************************
{
	setOffset(dx, _dy);
}

void SplitterContact::setDy(const DbU::Unit& dy)
// ********************************
{
	setOffset(_dx, dy);
}

void SplitterContact::setOffset(const DbU::Unit& dx, const DbU::Unit& dy)
// ****************************************************
{
	if ((dx != _dx) || (dy != _dy)) {
		invalidate(true);
		_dx = dx;
		_dy = dy;
	}
}

unsigned SplitterContact::getOppositePlugCount()
// *********************************************
{
    if (_oppositePlugCount == (std::numeric_limits<unsigned>::max)())
        computeOppositeCounts();

    return _oppositePlugCount;
}

unsigned SplitterContact::getOppositeSplitterCount()
// *************************************************
{
    if (_oppositeSplitterCount == (std::numeric_limits<unsigned>::max)())
        computeOppositeCounts();

    return _oppositeSplitterCount;
}

unsigned SplitterContact::getOppositePinCount()
// ********************************************
{
    if (_oppositePinCount == (std::numeric_limits<unsigned>::max)())
        computeOppositeCounts();

    return _oppositePinCount;
}

void SplitterContact::computeOppositeCounts()
// ******************************************
{
    unsigned plugCount     = 0;
    unsigned splitterCount = 0;
    unsigned pinCount      = 0;
    SplitterContact* oppSC = getOppositeSplitterContact();

    for_each_hook(hook, oppSC->getBodyHook()->getHooks())
    {
        Component* compo = hook->getComponent();
        if (SplitterContact* sc = dynamic_cast<SplitterContact*> (compo))
        {
            if (sc == oppSC) continue;
            plugCount += sc->getOppositePlugCount();
            splitterCount += 1 + sc->getOppositeSplitterCount();
            pinCount += sc->getOppositePinCount();
        }
        if (dynamic_cast<Plug*> (compo))
        {
            ++plugCount;
        }
        if (dynamic_cast<Pin*> (compo))
        {
            ++pinCount;
        }
        end_for;
    }
    _oppositePlugCount     = plugCount;
    _oppositeSplitterCount = splitterCount;
    _oppositePinCount      = pinCount;

    return;
}

void SplitterContact::invalidateOppositeCounts()
// *********************************************
{
    _oppositePlugCount = (std::numeric_limits<unsigned>::max)();
    _oppositeSplitterCount = (std::numeric_limits<unsigned>::max)();
    _oppositePinCount = (std::numeric_limits<unsigned>::max)();
    return;
}

void SplitterContact::_preDestroy()
// *******************************
{
// trace << "entering SplitterContact::PreDelete: " << this << endl;
// trace_in();

	Inherit::_preDestroy();


    Component* anchor = getAnchor ();

    if (anchor) {
        _anchorHook.detach();
        anchor->destroy ();
    }

// trace << "exiting SplitterContact::PreDelete:" << endl;
// trace_out();
}

string SplitterContact::_getString() const
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

Record* SplitterContact::_getRecord() const
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
                if (_oppositePlugCount == (std::numeric_limits<unsigned>::max)())
                  const_cast<SplitterContact*>(this)->computeOppositeCounts();
                record->add(getSlot("OppositePlugCount", &_oppositePlugCount));
                record->add(getSlot("OppositeSplitterCount", &_oppositeSplitterCount));
                record->add(getSlot("OppositePinCount", &_oppositePinCount));
	}
	return record;
}

DbU::Unit SplitterContact::_getSize() const
// ***************************
{
	DbU::Unit size = 0;

	const Layer* layer = getLayer();
	if (const ContactLayer* contactLayer = dynamic_cast<const ContactLayer*>(layer))
		size = contactLayer->getEnclosure();

	return size;
}

DbU::Unit SplitterContact::_getSize(const BasicLayer* basicLayer) const
// *************************************************
{
	const Layer* layer = getLayer();

	if (!layer->contains(basicLayer)) return 0;

	DbU::Unit size = 0;

	if (const ContactLayer* contactLayer = dynamic_cast<const ContactLayer*>(layer))
		size = contactLayer->getEnclosure(basicLayer);

	return size;
}

// void SplitterContact::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
// // ****************************************************************************************************
// {
// 	if (_width && _height) {
// 		if (1 < view->getScreenSize(max(_width, _height)))
// 			basicLayer->_Fill(view, transformation.getBox(getBoundingBox(basicLayer)));
// 	}
// 	if (basicLayer == _layer->_getSymbolicBasicLayer()) {
// 		if (view->CutPointsAreVisible() && (3 < view->getScale())) {
// 			Point position = getPosition();
// 			view->DrawPoint(transformation.getPoint(position), 3);
// 			if (_width) {
// 				Box box = transformation.getBox(Box(position).Inflate(getHalfWidth(), 0));
// 				view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
// 			}
// 			if (_height) {
// 				Box box = transformation.getBox(Box(position).Inflate(0, getHalfHeight()));
// 				view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
// 			}
// 		}
// 	}
// }

// void SplitterContact::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
// // **********************************************************************************************
// {
// 	if (_width && _height) {
// 		if (1 < view->getScreenSize(max(_width, _height))) {
// 			for_each_basic_layer(basicLayer, getLayer()->getBasicLayers()) {
// 				basicLayer->_Fill(view, transformation.getBox(getBoundingBox(basicLayer)));
// 				end_for;
// 			}
// 		}
// 	}
// 	if (view->getScale() <= 1)
// 		view->DrawPoint(transformation.getPoint(getPosition()), 1);
// 	else if (view->getScale() <= 3)
// 		view->DrawPoint(transformation.getPoint(getPosition()), 2);
// 	else {
// 		Point position = getPosition();
// 		view->DrawPoint(transformation.getPoint(position), 3);
// 		if (_width) {
// 			Box box = transformation.getBox(Box(position).Inflate(getHalfWidth(), 0));
// 			view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
// 		}
// 		if (_height) {
// 			Box box = transformation.getBox(Box(position).Inflate(0, getHalfHeight()));
// 			view->DrawLine(box.getXMin(), box.getYMin(), box.getXMax(), box.getYMax());
// 		}
// 	}
//     getSplitter()->_Highlight ( view, updateArea, transformation );
// }

// void SplitterContact::_SaveHeaderTo(OutputFile& outputFile)
// // ************************************************
// {
// 	Inherit::_SaveHeaderTo(outputFile);

// 	outputFile << " " << outputFile.getId(getAnchor());
// 	outputFile << " " << outputFile.getId(getLayer());
// 	outputFile << " " << getValueString(getDx());
// 	outputFile << " " << getValueString(getDy());
// 	outputFile << " " << getValueString(getWidth());
// 	outputFile << " " << getValueString(getHeight());
// }


// ****************************************************************************************************
// SplitterContact::AnchorHook implementation
// ****************************************************************************************************

static int ANCHOR_HOOK_OFFSET = -1;

SplitterContact::AnchorHook::AnchorHook(SplitterContact* splittercontact)
// **********************************************
:	Inherit()
{
	if (!splittercontact)
		throw Error("Can't create " + _TName("SplitterContact::AnchorHook") + " : null splittercontact");

	if (ANCHOR_HOOK_OFFSET == -1)
		ANCHOR_HOOK_OFFSET = (unsigned long)this - (unsigned long)splittercontact;
}

Component* SplitterContact::AnchorHook::getComponent() const
// *************************************************
{
	return (Component*)((unsigned long)this - ANCHOR_HOOK_OFFSET);
}

string SplitterContact::AnchorHook::_getString() const
// *******************************************
{
	return "<" + _TName("SplitterContact::AnchorHook") + " " + getString(getComponent()) + ">";
}



// ****************************************************************************************************
// SplitterContact_Hooks implementation
// ****************************************************************************************************

SplitterContact_Hooks::SplitterContact_Hooks(const SplitterContact* splittercontact)
// *************************************************
: 	Inherit(),
	_splittercontact(splittercontact)
{
}

SplitterContact_Hooks::SplitterContact_Hooks(const SplitterContact_Hooks& hooks)
// *****************************************************
: 	Inherit(),
	_splittercontact(hooks._splittercontact)
{
}

SplitterContact_Hooks& SplitterContact_Hooks::operator=(const SplitterContact_Hooks& hooks)
// ****************************************************************
{
	_splittercontact = hooks._splittercontact;
	return *this;
}

Collection<Hook*>* SplitterContact_Hooks::getClone() const
// ***********************************************
{
	return new SplitterContact_Hooks(*this);
}

Locator<Hook*>* SplitterContact_Hooks::getLocator() const
// **********************************************
{
	return new Locator(_splittercontact);
}

string SplitterContact_Hooks::_getString() const
// *************************************
{
	string s = "<" + _TName("SplitterContact::Hooks");
	if (_splittercontact) s += " " + getString(_splittercontact);
	s += ">";
	return s;
}



// ****************************************************************************************************
// SplitterContact_Hooks::Locator implementation
// ****************************************************************************************************

SplitterContact_Hooks::Locator::Locator(const SplitterContact* splittercontact)
// ****************************************************
:	Inherit(),
	_splittercontact(splittercontact),
	_hook(NULL)
{
	if (_splittercontact) _hook = ((SplitterContact*)_splittercontact)->getBodyHook();
}

SplitterContact_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:	Inherit(),
	_splittercontact(locator._splittercontact),
	_hook(locator._hook)
{
}

SplitterContact_Hooks::Locator& SplitterContact_Hooks::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
	_splittercontact = locator._splittercontact;
	_hook = locator._hook;
	return *this;
}

Hook* SplitterContact_Hooks::Locator::getElement() const
// *********************************************
{
	return _hook;
}

Locator<Hook*>* SplitterContact_Hooks::Locator::getClone() const
// *****************************************************
{
	return new Locator(*this);
}

bool SplitterContact_Hooks::Locator::isValid() const
// *****************************************
{
	return (_hook != NULL);
}

void SplitterContact_Hooks::Locator::progress()
// ************************************
{
	if (_hook) {
		if (_hook == ((SplitterContact*)_splittercontact)->getBodyHook())
			_hook = ((SplitterContact*)_splittercontact)->getAnchorHook();
		else
			_hook = NULL;
	}
}

string SplitterContact_Hooks::Locator::_getString() const
// **********************************************
{
	string s = "<" + _TName("SplitterContact::Hooks::Locator");
	if (_splittercontact) s += " " + getString(_splittercontact);
	s += ">";
	return s;
}


// ---------------------------------------------------------------
// Function  :  "getStringDir()".

string  getStringDir ( unsigned int direction )
{
  switch ( direction ) {
    case SplitterContact::SP_1:       return "SP_1";
    case SplitterContact::SP_2:       return "SP_2";
    case SplitterContact::SP_LEFT:    return "SP_LEFT";
    case SplitterContact::SP_RIGHT:   return "SP_RIGHT";
    case SplitterContact::SP_TOP:     return "SP_TOP";
    case SplitterContact::SP_BOTTOM:  return "SP_BOTTOM";
    case Constant::Horizontal:        return "Horizontal";
    case Constant::Vertical:          return "Vertical";
  }

  return "Unknown direction code";
}


} // End of Nimbus namespace.
