// ****************************************************************************************************
// File: Segment.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#include "Segment.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Error.h"

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

		public: virtual Hook* GetElement() const;
		public: virtual Hurricane::Locator<Hook*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Hook*>* GetClone() const;
	public: virtual Hurricane::Locator<Hook*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

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

		public: virtual Component* GetElement() const;
		public: virtual Hurricane::Locator<Component*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

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

	public: virtual Collection<Component*>* GetClone() const;
	public: virtual Hurricane::Locator<Component*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

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
		if (!source->GetNet())
			throw Error("Can't create " + _TName("Segment") + " : unconnected source");
		if (source->GetNet() != net)
			throw Error("Can't create " + _TName("Segment") + " : incompatible source");
	}

	if (target) {
		if (!target->GetNet())
			throw Error("Can't create " + _TName("Segment") + " : unconnected target");
		if (target->GetNet() != net)
			throw Error("Can't create " + _TName("Segment") + " : incompatible target");
	}

	if (!_layer)
		throw Error("Can't create " + _TName("Segment") + " : null layer");

	if (source) _sourceHook.Attach(source->GetBodyHook());
	if (target) _targetHook.Attach(target->GetBodyHook());
}

Hooks Segment::GetHooks() const
// ****************************
{
	return Segment_Hooks(this);
}

Hook* Segment::GetOppositeHook(const Hook* hook) const
// ***************************************************
{
	if (hook) {
		if (hook == &_sourceHook) return (Hook*)&_targetHook;
		if (hook == &_targetHook) return (Hook*)&_sourceHook;
	}
	return NULL;
}

Component* Segment::GetSource() const
// **********************************
{
	Hook* masterHook = _sourceHook.GetMasterHook();
	return (masterHook) ? masterHook->GetComponent() : NULL;
}

Component* Segment::GetTarget() const
// **********************************
{
	Hook* masterHook = _targetHook.GetMasterHook();
	return (masterHook) ? masterHook->GetComponent() : NULL;
}

Components Segment::GetAnchors() const
// ***********************************
{
	return Segment_Anchors(this);
}

Component* Segment::GetOppositeAnchor(Component* anchor) const
// ***********************************************************
{
	if (anchor) {
		Component* source = GetSource();
		Component* target = GetTarget();
		if (anchor == source) return target;
		if (anchor == target) return source;
	}
	return NULL;
}

Point Segment::GetSourcePosition() const
// *************************************
{
	return Point(GetSourceX(), GetSourceY());
}

Point Segment::GetTargetPosition() const
// *************************************
{
	return Point(GetTargetX(), GetTargetY());
}

void Segment::SetLayer(Layer* layer)
// *********************************
{
	if (!layer)
		throw Error("Can't set layer : null layer");

	if (layer != _layer) {
		Invalidate(false);
		_layer = layer;
	}
}

void Segment::SetWidth(const Unit& width)
// **************************************
{
	if (width != _width) {
		Invalidate(false);
		_width = width;
	}
}

void Segment::Invert()
// *******************
{
	Component* source = GetSource();
	Component* target = GetTarget();
	if (source && target && (target != source)) {
		GetSourceHook()->Detach();
		GetTargetHook()->Detach();
		GetSourceHook()->Attach(target->GetBodyHook());
		GetTargetHook()->Attach(source->GetBodyHook());
	}
}

void Segment::_PreDelete()
// ***********************
{
// trace << "entering Segment::_PreDelete: " << this << endl;
// trace_in();

	Inherit::_PreDelete();

	_sourceHook.Detach();
	_targetHook.Detach();

// trace << "exiting Segment::_PreDelete:" << endl;
// trace_out();
}

string Segment::_GetString() const
// *******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_layer->GetName()));
	s.insert(s.length() - 1, " [" + GetValueString(GetSourceX()) + " " + GetValueString(GetSourceY()) + "]");
	s.insert(s.length() - 1, " [" + GetValueString(GetTargetX()) + " " + GetValueString(GetTargetY()) + "]");
	s.insert(s.length() - 1, " " + GetValueString(_width));
	return s;
}

Record* Segment::_GetRecord() const
// **************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("SourceHook", &_sourceHook));
		record->Add(GetSlot("Source", GetSource()));
		record->Add(GetSlot("TargetHook", &_targetHook));
		record->Add(GetSlot("Target", GetTarget()));
		record->Add(GetSlot("Layer", _layer));
		record->Add(GetSlot("Width", &_width));
	}
	return record;
}

Unit Segment::_GetSize() const
// ***************************
{
	Unit size = 0;

	Layer* layer = GetLayer();
	if (is_a<CompositeLayer*>(layer))
		size = ((CompositeLayer*)layer)->GetMaximalSegmentSize();

	return size;
}

Unit Segment::_GetExtention() const
// ********************************
{
	Unit extention = 0;

	Layer* layer = GetLayer();
	if (is_a<CompositeLayer*>(layer))
		extention = ((CompositeLayer*)layer)->GetMaximalSegmentExtention();

	return extention;
}

Unit Segment::_GetSize(const BasicLayer* basicLayer) const
// *************************************************
{
	Layer* layer = GetLayer();

	if (!layer->Contains(basicLayer)) return 0;

	Unit size = 0;

	if (is_a<CompositeLayer*>(layer))
		size = ((CompositeLayer*)layer)->GetSegmentSize(basicLayer);

	return size;
}

Unit Segment::_GetExtention(const BasicLayer* basicLayer) const
// ******************************************************
{
	Layer* layer = GetLayer();

	if (!layer->Contains(basicLayer)) return 0;

	Unit extention = 0;

	if (is_a<CompositeLayer*>(layer))
		extention = ((CompositeLayer*)layer)->GetSegmentExtention(basicLayer);

	return extention;
}

//bool Segment::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// ***************************************************************************************
//{
//	Box area(point);
//	area.Inflate(aperture);
//	Layer* layer = GetLayer();
//	for_each_basic_layer(basicLayer, layer->GetBasicLayers()) {
//		if (view->IsVisible(basicLayer))
//			if (GetBoundingBox(basicLayer).Intersect(area)) return true;
//		end_for;
//	}
//
//	return false;
//}
//

//void Segment::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//	if (1 < view->GetScreenSize(GetWidth()))
//		basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//	else
//		if (basicLayer == GetLayer()->_GetSymbolicBasicLayer())
//			view->DrawLine(transformation.GetPoint(GetSourcePosition()),
//								transformation.GetPoint(GetTargetPosition()));
//}
//

// ****************************************************************************************************
// Segment::SourceHook implementation
// ****************************************************************************************************

static int SOURCE_HOOK_OFFSET = -1;

Segment::SourceHook::SourceHook(Segment* segment)
// **********************************************
:	Inherit()
{
	if (!segment)
		throw Error("Can't create " + _TName("Segment::SourceHook") + " (null segment)");

	if (SOURCE_HOOK_OFFSET == -1)
		SOURCE_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
}

Component* Segment::SourceHook::GetComponent() const
// *************************************************
{
	return (Component*)((unsigned long)this - SOURCE_HOOK_OFFSET);
}

string Segment::SourceHook::_GetString() const
// *******************************************
{
	return "<" + _TName("Segment::SourceHook") + " " + GetString(GetComponent()) + ">";
}



// ****************************************************************************************************
// Segment::TargetHook implementation
// ****************************************************************************************************

static int TARGET_HOOK_OFFSET = -1;

Segment::TargetHook::TargetHook(Segment* segment)
// **********************************************
:	Inherit()
{
	if (!segment)
		throw Error("Can't create " + _TName("Segment::TargetHook") + " (null segment)");

	if (TARGET_HOOK_OFFSET == -1)
		TARGET_HOOK_OFFSET = (unsigned long)this - (unsigned long)segment;
}

Component* Segment::TargetHook::GetComponent() const
// *************************************************
{
	return (Component*)((unsigned long)this - TARGET_HOOK_OFFSET);
}

string Segment::TargetHook::_GetString() const
// *******************************************
{
	return "<" + _TName("Segment::TargetHook") + " " + GetString(GetComponent()) + ">";
}

// ****************************************************************************************************
// Segment_Hooks implementation
// ****************************************************************************************************

Segment_Hooks::Segment_Hooks(const Segment* segment)
// *************************************************
: 	Inherit(),
	_segment(segment)
{
}

Segment_Hooks::Segment_Hooks(const Segment_Hooks& hooks)
// *****************************************************
: 	Inherit(),
	_segment(hooks._segment)
{
}

Segment_Hooks& Segment_Hooks::operator=(const Segment_Hooks& hooks)
// ****************************************************************
{
	_segment = hooks._segment;
	return *this;
}

Collection<Hook*>* Segment_Hooks::GetClone() const
// ***********************************************
{
	return new Segment_Hooks(*this);
}

Locator<Hook*>* Segment_Hooks::GetLocator() const
// **********************************************
{
	return new Locator(_segment);
}

string Segment_Hooks::_GetString() const
// *************************************
{
	string s = "<" + _TName("Segment::Hooks");
	if (_segment) s += " " + GetString(_segment);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Segment_Hooks::Locator implementation
// ****************************************************************************************************

Segment_Hooks::Locator::Locator(const Segment* segment)
// ****************************************************
:	Inherit(),
	_segment(segment),
	_hook(NULL)
{
	if (_segment) _hook = ((Segment*)_segment)->GetBodyHook();
}

Segment_Hooks::Locator::Locator(const Locator& locator)
// ****************************************************
:	Inherit(),
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

Hook* Segment_Hooks::Locator::GetElement() const
// *********************************************
{
	return _hook;
}

Locator<Hook*>* Segment_Hooks::Locator::GetClone() const
// *****************************************************
{
	return new Locator(*this);
}

bool Segment_Hooks::Locator::IsValid() const
// *****************************************
{
	return (_hook != NULL);
}

void Segment_Hooks::Locator::Progress()
// ************************************
{
	if (_hook) {
		if (_hook == ((Segment*)_segment)->GetBodyHook())
			_hook = ((Segment*)_segment)->GetSourceHook();
		else {
			if (_hook == ((Segment*)_segment)->GetSourceHook())
				_hook = ((Segment*)_segment)->GetTargetHook();
			else
				_hook = NULL;
		}
	}
}

string Segment_Hooks::Locator::_GetString() const
// **********************************************
{
	string s = "<" + _TName("Segment::Hooks::Locator");
	if (_segment) s += " " + GetString(_segment);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Segment_Anchors implementation
// ****************************************************************************************************

Segment_Anchors::Segment_Anchors(const Segment* segment)
// *****************************************************
: 	Inherit(),
	_segment(segment)
{
}

Segment_Anchors::Segment_Anchors(const Segment_Anchors& hooks)
// ***********************************************************
: 	Inherit(),
	_segment(hooks._segment)
{
}

Segment_Anchors& Segment_Anchors::operator=(const Segment_Anchors& hooks)
// **********************************************************************
{
	_segment = hooks._segment;
	return *this;
}

Collection<Component*>* Segment_Anchors::GetClone() const
// ******************************************************
{
	return new Segment_Anchors(*this);
}

Locator<Component*>* Segment_Anchors::GetLocator() const
// *****************************************************
{
	return new Locator(_segment);
}

string Segment_Anchors::_GetString() const
// ***************************************
{
	string s = "<" + _TName("Segment::Anchors");
	if (_segment) s += " " + GetString(_segment);
	s += ">";
	return s;
}



// ****************************************************************************************************
// Segment_Anchors::Locator implementation
// ****************************************************************************************************

Segment_Anchors::Locator::Locator(const Segment* segment)
// ******************************************************
:	Inherit(),
	_segment(segment),
	_anchor(NULL)
{
	if (_segment) {
		_anchor = ((Segment*)_segment)->GetSource();
		if (!_anchor) _anchor = ((Segment*)_segment)->GetTarget();
	}
}

Segment_Anchors::Locator::Locator(const Locator& locator)
// ******************************************************
:	Inherit(),
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

Component* Segment_Anchors::Locator::GetElement() const
// ****************************************************
{
	return _anchor;
}

Locator<Component*>* Segment_Anchors::Locator::GetClone() const
// ************************************************************
{
	return new Locator(*this);
}

bool Segment_Anchors::Locator::IsValid() const
// *******************************************
{
	return (_anchor != NULL);
}

void Segment_Anchors::Locator::Progress()
// **************************************
{
	if (_anchor) {
		if (_anchor == ((Segment*)_segment)->GetTarget())
			_anchor = NULL;
		else
			_anchor = ((Segment*)_segment)->GetTarget();
	}
}

string Segment_Anchors::Locator::_GetString() const
// ************************************************
{
	string s = "<" + _TName("Segment::Anchors::Locator");
	if (_segment) s += " " + GetString(_segment);
	s += ">";
	return s;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
