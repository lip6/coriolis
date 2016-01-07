// ****************************************************************************************************
// File: ./Segment.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#include "hurricane/Segment.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Error.h"
#include "hurricane/Net.h"

namespace Hurricane {



// ****************************************************************************************************
// Segment_Hooks declaration
// ****************************************************************************************************

class Segment_Hooks : public Collection<Hook*> {
// *******************************************

// Types
// *****

    public: typedef Collection<Hook*> Inherit;

    public: class Locator : public Hurricane::Locator<Hook*> {
    // *****************************************************

        public: typedef Hurricane::Locator<Hook*> Inherit;

        private: const Segment* _segment;
        private: Hook* _hook;

        public: Locator(const Segment* segment = NULL);
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

    private: const Segment* _segment;

// Constructors
// ************

    public: Segment_Hooks(const Segment* segment = NULL);
    public: Segment_Hooks(const Segment_Hooks& hooks);

// Operators
// *********

    public: Segment_Hooks& operator=(const Segment_Hooks& hooks);

// Accessors
// *********

    public: virtual Collection<Hook*>* getClone() const;
    public: virtual Hurricane::Locator<Hook*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Segment_Anchors declaration
// ****************************************************************************************************

class Segment_Anchors : public Collection<Component*> {
// **************************************************

// Types
// *****

    public: typedef Collection<Component*> Inherit;

    public: class Locator : public Hurricane::Locator<Component*> {
    // **********************************************************

        public: typedef Hurricane::Locator<Component*> Inherit;

        private: const Segment* _segment;
        private: Component* _anchor;

        public: Locator(const Segment* segment = NULL);
        public: Locator(const Locator& locator);

        public: Locator& operator=(const Locator& locator);

        public: virtual Component* getElement() const;
        public: virtual Hurricane::Locator<Component*>* getClone() const;

        public: virtual bool isValid() const;

        public: virtual void progress();

        public: virtual string _getString() const;

    };

// Attributes
// **********

    private: const Segment* _segment;

// Constructors
// ************

    public: Segment_Anchors(const Segment* segment = NULL);
    public: Segment_Anchors(const Segment_Anchors& anchors);

// Operators
// *********

    public: Segment_Anchors& operator=(const Segment_Anchors& anchors);

// Accessors
// *********

    public: virtual Collection<Component*>* getClone() const;
    public: virtual Hurricane::Locator<Component*>* getLocator() const;

// Others
// ******

    public: virtual string _getString() const;

};



// ****************************************************************************************************
// Segment implementation
// ****************************************************************************************************

Segment::Segment(Net* net, Component* source, Component* target, const Layer* layer, const DbU::Unit& width)
// **********************************************************************************************
:  Inherit(net),
    _sourceHook(this),
    _targetHook(this),
    _layer(layer),
    _width(width)
{
    if (source) {
        if (!source->getNet())
            throw Error("Can't create " + _TName("Segment") + ": unconnected source");
        if (source->getNet() != net)
            throw Error(  "Can't create " + _TName("Segment") + ": incompatible source \n"
                        + "        - Source:" + getString(source->getNet()) + "\n"
                        + "        - Owner: " + getString(net) );
    }

    if (target) {
        if (!target->getNet())
            throw Error("Can't create " + _TName("Segment") + ": unconnected target");
        if (target->getNet() != net)
            throw Error(  "Can't create " + _TName("Segment") + ": incompatible target\n"
                        + "        - Target:" + getString(target->getNet()) + "\n"
                        + "        - Owner: " + getString(net) );
    }

    if (!_layer)
        throw Error("Can't create " + _TName("Segment") + " : null layer");

    if (source) _sourceHook.attach(source->getBodyHook());
    if (target) _targetHook.attach(target->getBodyHook());
}

Hooks Segment::getHooks() const
// ****************************
{
    return Segment_Hooks(this);
}

Hook* Segment::getOppositeHook(const Hook* hook) const
// ***************************************************
{
    if (hook) {
        if (hook == &_sourceHook) return (Hook*)&_targetHook;
        if (hook == &_targetHook) return (Hook*)&_sourceHook;
    }
    return NULL;
}

Component* Segment::getSource() const
// **********************************
{
    Hook* masterHook = _sourceHook.getMasterHook();
    return (masterHook) ? masterHook->getComponent() : NULL;
}

Component* Segment::getTarget() const
// **********************************
{
    Hook* masterHook = _targetHook.getMasterHook();
    return (masterHook) ? masterHook->getComponent() : NULL;
}

Components Segment::getAnchors() const
// ***********************************
{
    return Segment_Anchors(this);
}

Component* Segment::getOppositeAnchor(Component* anchor) const
// ***********************************************************
{
    if (anchor) {
        Component* source = getSource();
        Component* target = getTarget();
        if (anchor == source) return target;
        if (anchor == target) return source;
    }
    return NULL;
}

Point Segment::getSourcePosition() const
// *************************************
{
    return Point(getSourceX(), getSourceY());
}

Point Segment::getTargetPosition() const
// *************************************
{
    return Point(getTargetX(), getTargetY());
}

void Segment::setLayer(const Layer* layer)
// *********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Segment::setWidth(const DbU::Unit& width)
// **************************************
{
    if (width != _width) {
        invalidate(false);
        _width = width;
    }
}

void Segment::invert()
// *******************
{
    Component* source = getSource();
    Component* target = getTarget();
    if (source && target && (target != source)) {
        getSourceHook()->detach();
        getTargetHook()->detach();
        getSourceHook()->attach(target->getBodyHook());
        getTargetHook()->attach(source->getBodyHook());
    }
}

void Segment::_preDestroy()
// ***********************
{
// trace << "entering Segment::_preDestroy: " << this << endl;
// trace_in();

    Inherit::_preDestroy();

    _sourceHook.detach();
    _targetHook.detach();

// trace << "exiting Segment::_preDestroy:" << endl;
// trace_out();
}

void Segment::_toJson(JsonWriter* writer) const
// ********************************************
{
  Inherit::_toJson( writer );

  jsonWrite( writer, "_sourceHook", _sourceHook.getNextHook()->toJson() );
  jsonWrite( writer, "_targetHook", _targetHook.getNextHook()->toJson() );
  jsonWrite( writer, "_layer"     , _layer->getName() );
  jsonWrite( writer, "_width"     , _width );
}

string Segment::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " [" + DbU::getValueString(getSourceX()) + " " + DbU::getValueString(getSourceY()) + "]");
    s.insert(s.length() - 1, " [" + DbU::getValueString(getTargetX()) + " " + DbU::getValueString(getTargetY()) + "]");
    s.insert(s.length() - 1, " " + DbU::getValueString(_width));
    return s;
}

Record* Segment::_getRecord() const
// ********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("SourceHook", &_sourceHook));
        record->add(getSlot("Source", getSource()));
        record->add(getSlot("TargetHook", &_targetHook));
        record->add(getSlot("Target", getTarget()));
        record->add(getSlot("Layer", _layer));
        record->add(DbU::getValueSlot("Width", &_width));
    }
    return record;
}


// ****************************************************************************************************
// Segment::SourceHook implementation
// ****************************************************************************************************

static int SOURCE_HOOK_OFFSET = -1;

Segment::SourceHook::SourceHook(Segment* segment)
// **********************************************
:    Inherit()
{
  if (!segment)
    throw Error("Can't create " + _getTypeName() + " (null segment)");

  if (SOURCE_HOOK_OFFSET == -1) {
    SOURCE_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
    Hook::addCompToHook(_getTypeName(),_compToHook);
  }
}

Component* Segment::SourceHook::getComponent() const
// *************************************************
{
    return (Component*)((unsigned long)this - SOURCE_HOOK_OFFSET);
}

string Segment::SourceHook::_getString() const
// *******************************************
{
    return "<" + _TName("Segment::SourceHook") + " " + getString(getComponent()) + ">";
}

Hook* Segment::SourceHook::_compToHook(Component* component)
// ***************************************************************
{
  Segment* segment = dynamic_cast<Segment*>(component);
  if (not segment) {
    throw Error( "SourceHook::_compToHook(): Unable to cast %s into Segment*."
               , getString(component).c_str() );
  }
  return &(segment->_sourceHook);
}


// ****************************************************************************************************
// Segment::TargetHook implementation
// ****************************************************************************************************

static int TARGET_HOOK_OFFSET = -1;

Segment::TargetHook::TargetHook(Segment* segment)
// **********************************************
:    Inherit()
{
    if (!segment)
        throw Error("Can't create " + _TName("Segment::TargetHook") + " (null segment)");

    if (TARGET_HOOK_OFFSET == -1) {
      TARGET_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
      Hook::addCompToHook(_getTypeName(),_compToHook);
    }
}

Component* Segment::TargetHook::getComponent() const
// *************************************************
{
    return (Component*)((unsigned long)this - TARGET_HOOK_OFFSET);
}

string Segment::TargetHook::_getString() const
// *******************************************
{
    return "<" + _TName("Segment::TargetHook") + " " + getString(getComponent()) + ">";
}

Hook* Segment::TargetHook::_compToHook(Component* component)
// ***************************************************************
{
  Segment* segment = dynamic_cast<Segment*>(component);
  if (not segment) {
    throw Error( "TargetHook::_compToHook(): Unable to cast %s into Segment*."
               , getString(component).c_str() );
  }
  return &(segment->_targetHook);
}


// ****************************************************************************************************
// Segment_Hooks implementation
// ****************************************************************************************************

Segment_Hooks::Segment_Hooks(const Segment* segment)
// *************************************************
:     Inherit(),
    _segment(segment)
{
}

Segment_Hooks::Segment_Hooks(const Segment_Hooks& hooks)
// *****************************************************
:     Inherit(),
    _segment(hooks._segment)
{
}

Segment_Hooks& Segment_Hooks::operator=(const Segment_Hooks& hooks)
// ****************************************************************
{
    _segment = hooks._segment;
    return *this;
}

Collection<Hook*>* Segment_Hooks::getClone() const
// ***********************************************
{
    return new Segment_Hooks(*this);
}

Locator<Hook*>* Segment_Hooks::getLocator() const
// **********************************************
{
    return new Locator(_segment);
}

string Segment_Hooks::_getString() const
// *************************************
{
    string s = "<" + _TName("Segment::Hooks");
    if (_segment) s += " " + getString(_segment);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Segment_Hooks::Locator implementation
// ****************************************************************************************************

Segment_Hooks::Locator::Locator(const Segment* segment)
// ****************************************************
:    Inherit(),
    _segment(segment),
    _hook(NULL)
{
    if (_segment) _hook = ((Segment*)_segment)->getBodyHook();
}

Segment_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:    Inherit(),
    _segment(locator._segment),
    _hook(locator._hook)
{
}

Segment_Hooks::Locator& Segment_Hooks::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
    _segment = locator._segment;
    _hook = locator._hook;
    return *this;
}

Hook* Segment_Hooks::Locator::getElement() const
// *********************************************
{
    return _hook;
}

Locator<Hook*>* Segment_Hooks::Locator::getClone() const
// *****************************************************
{
    return new Locator(*this);
}

bool Segment_Hooks::Locator::isValid() const
// *****************************************
{
    return (_hook != NULL);
}

void Segment_Hooks::Locator::progress()
// ************************************
{
    if (_hook) {
        if (_hook == ((Segment*)_segment)->getBodyHook())
            _hook = ((Segment*)_segment)->getSourceHook();
        else {
            if (_hook == ((Segment*)_segment)->getSourceHook())
                _hook = ((Segment*)_segment)->getTargetHook();
            else
                _hook = NULL;
        }
    }
}

string Segment_Hooks::Locator::_getString() const
// **********************************************
{
    string s = "<" + _TName("Segment::Hooks::Locator");
    if (_segment) s += " " + getString(_segment);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Segment_Anchors implementation
// ****************************************************************************************************

Segment_Anchors::Segment_Anchors(const Segment* segment)
// *****************************************************
:     Inherit(),
    _segment(segment)
{
}

Segment_Anchors::Segment_Anchors(const Segment_Anchors& hooks)
// ***********************************************************
:     Inherit(),
    _segment(hooks._segment)
{
}

Segment_Anchors& Segment_Anchors::operator=(const Segment_Anchors& hooks)
// **********************************************************************
{
    _segment = hooks._segment;
    return *this;
}

Collection<Component*>* Segment_Anchors::getClone() const
// ******************************************************
{
    return new Segment_Anchors(*this);
}

Locator<Component*>* Segment_Anchors::getLocator() const
// *****************************************************
{
    return new Locator(_segment);
}

string Segment_Anchors::_getString() const
// ***************************************
{
    string s = "<" + _TName("Segment::Anchors");
    if (_segment) s += " " + getString(_segment);
    s += ">";
    return s;
}



// ****************************************************************************************************
// Segment_Anchors::Locator implementation
// ****************************************************************************************************

Segment_Anchors::Locator::Locator(const Segment* segment)
// ******************************************************
:    Inherit(),
    _segment(segment),
    _anchor(NULL)
{
    if (_segment) {
        _anchor = ((Segment*)_segment)->getSource();
        if (!_anchor) _anchor = ((Segment*)_segment)->getTarget();
    }
}

Segment_Anchors::Locator::Locator(const Locator& locator)
// ******************************************************
:    Inherit(),
    _segment(locator._segment),
    _anchor(locator._anchor)
{
}

Segment_Anchors::Locator& Segment_Anchors::Locator::operator=(const Locator& locator)
// **********************************************************************************
{
    _segment = locator._segment;
    _anchor = locator._anchor;
    return *this;
}

Component* Segment_Anchors::Locator::getElement() const
// ****************************************************
{
    return _anchor;
}

Locator<Component*>* Segment_Anchors::Locator::getClone() const
// ************************************************************
{
    return new Locator(*this);
}

bool Segment_Anchors::Locator::isValid() const
// *******************************************
{
    return (_anchor != NULL);
}

void Segment_Anchors::Locator::progress()
// **************************************
{
    if (_anchor) {
        if (_anchor == ((Segment*)_segment)->getTarget())
            _anchor = NULL;
        else
            _anchor = ((Segment*)_segment)->getTarget();
    }
}

string Segment_Anchors::Locator::_getString() const
// ************************************************
{
    string s = "<" + _TName("Segment::Anchors::Locator");
    if (_segment) s += " " + getString(_segment);
    s += ">";
    return s;
}



// ****************************************************************************************************
// JsonSegment implementation
// ****************************************************************************************************

JsonSegment::JsonSegment(unsigned long flags)
// ******************************************
  : JsonComponent(flags)
{
  add( "_sourceHook", typeid(string)   );
  add( "_targetHook", typeid(string)   );
  add( "_layer"     , typeid(uint64_t) );
  add( "_width"     , typeid(uint64_t) );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
