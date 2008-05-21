// ****************************************************************************************************
// File: Segment.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#include "hurricane/Segment.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/CompositeLayer.h"
#include "hurricane/Error.h"

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

Segment::Segment(Net* net, Component* source, Component* target, Layer* layer, const Unit& width)
// **********************************************************************************************
:  Inherit(net),
    _sourceHook(this),
    _targetHook(this),
    _layer(layer),
    _width(width)
{
    if (source) {
        if (!source->getNet())
            throw Error("Can't create " + _TName("Segment") + " : unconnected source");
        if (source->getNet() != net)
            throw Error("Can't create " + _TName("Segment") + " : incompatible source");
    }

    if (target) {
        if (!target->getNet())
            throw Error("Can't create " + _TName("Segment") + " : unconnected target");
        if (target->getNet() != net)
            throw Error("Can't create " + _TName("Segment") + " : incompatible target");
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

void Segment::setLayer(Layer* layer)
// *********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Segment::setWidth(const Unit& width)
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

string Segment::_getString() const
// *******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " [" + getValueString(getSourceX()) + " " + getValueString(getSourceY()) + "]");
    s.insert(s.length() - 1, " [" + getValueString(getTargetX()) + " " + getValueString(getTargetY()) + "]");
    s.insert(s.length() - 1, " " + getValueString(_width));
    return s;
}

Record* Segment::_getRecord() const
// **************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("SourceHook", &_sourceHook));
        record->add(getSlot("Source", getSource()));
        record->add(getSlot("TargetHook", &_targetHook));
        record->add(getSlot("Target", getTarget()));
        record->add(getSlot("Layer", _layer));
        record->add(getSlot("Width", &_width));
    }
    return record;
}

Unit Segment::_getSize() const
// ***************************
{
    Unit size = 0;

    Layer* layer = getLayer();
    if (is_a<CompositeLayer*>(layer))
        size = ((CompositeLayer*)layer)->getMaximalSegmentSize();

    return size;
}

Unit Segment::_getExtention() const
// ********************************
{
    Unit extention = 0;

    Layer* layer = getLayer();
    if (is_a<CompositeLayer*>(layer))
        extention = ((CompositeLayer*)layer)->getMaximalSegmentExtention();

    return extention;
}

Unit Segment::_getSize(const BasicLayer* basicLayer) const
// *************************************************
{
    Layer* layer = getLayer();

    if (!layer->contains(basicLayer)) return 0;

    Unit size = 0;

    if (is_a<CompositeLayer*>(layer))
        size = ((CompositeLayer*)layer)->getSegmentSize(basicLayer);

    return size;
}

Unit Segment::_getExtention(const BasicLayer* basicLayer) const
// ******************************************************
{
    Layer* layer = getLayer();

    if (!layer->contains(basicLayer)) return 0;

    Unit extention = 0;

    if (is_a<CompositeLayer*>(layer))
        extention = ((CompositeLayer*)layer)->getSegmentExtention(basicLayer);

    return extention;
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
        throw Error("Can't create " + _TName("Segment::SourceHook") + " (null segment)");

    if (SOURCE_HOOK_OFFSET == -1)
        SOURCE_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
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

    if (TARGET_HOOK_OFFSET == -1)
        TARGET_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
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



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
