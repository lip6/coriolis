// ****************************************************************************************************
// File: HyperNet.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6

#include "HyperNet.h"
#include "Cell.h"
#include "Net.h"
#include "Instance.h"
#include "Rubber.h"
#include "Plug.h"
#include "BasicLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// HyperNet_NetOccurrences definition
// ****************************************************************************************************

class HyperNet_NetOccurrences : public Collection<Occurrence> {
// ********************************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// *********************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

		private: const HyperNet* _hyperNet;
		private: bool _doExtraction;
		private: bool _allowInterruption;
		private: set<Occurrence> _netOccurrenceSet;
		private: stack<Occurrence> _netOccurrenceStack;

		public: Locator();
		public: Locator(const HyperNet* hyperNet, bool doExtraction = false, bool allowInterruption = false);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence GetElement() const;
		public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const HyperNet* _hyperNet;
	private: bool _doExtraction;
	private: bool _allowInterruption;

// Constructors
// ************

	public: HyperNet_NetOccurrences();
	public: HyperNet_NetOccurrences(const HyperNet* hyperNet, bool doExtraction = false, bool allowInterruption = false);
	public: HyperNet_NetOccurrences(const HyperNet_NetOccurrences& netOccurrences);

// Operators
// *********

	public: HyperNet_NetOccurrences& operator=(const HyperNet_NetOccurrences& netOccurrences);

// Accessors
// *********

	public: virtual Collection<Occurrence>* GetClone() const;
	public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};

// ****************************************************************************************************
// HyperNet_NetOccurrencesUnder definition
// ****************************************************************************************************

class HyperNet_NetOccurrencesUnder : public Collection<Occurrence> {
// ***************************************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// *********************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

		private: const HyperNet* _hyperNet;
    private: Box  _area;
		private: bool _doExtraction;
		private: bool _allowInterruption;
		private: set<Occurrence> _netOccurrenceSet;
		private: stack<Occurrence> _netOccurrenceStack;

		public: Locator();
		public: Locator(const HyperNet* hyperNet, Box area,
							 bool doExtraction = false, bool allowInterruption = false);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence GetElement() const;
		public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const HyperNet* _hyperNet;
  private: Box  _area;
	private: bool _doExtraction;
	private: bool _allowInterruption;

// Constructors
// ************

	public: HyperNet_NetOccurrencesUnder();
	public: HyperNet_NetOccurrencesUnder(const HyperNet* hyperNet,
													Box area,
													bool doExtraction = false,
													bool allowInterruption = false);
	public: HyperNet_NetOccurrencesUnder(const HyperNet_NetOccurrencesUnder& netOccurrences);

// Operators
// *********

	public: HyperNet_NetOccurrencesUnder& operator=(const HyperNet_NetOccurrencesUnder& netOccurrences);

// Accessors
// *********

	public: virtual Collection<Occurrence>* GetClone() const;
	public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};


// ****************************************************************************************************
// HyperNet_LeafPlugOccurrences definition
// ****************************************************************************************************

class HyperNet_LeafPlugOccurrences : public Collection<Occurrence> {
// *****************************************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// *********************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

		private: bool _doExtraction;
		private: bool _allowInterruption;
        private: OccurrenceLocator _netOccurrenceLocator;
        private: Occurrence _plugOccurrence;

		public: Locator();
		public: Locator(const HyperNet* hyperNet, bool doExtraction = false, bool allowInterruption = false);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence GetElement() const;
		public: virtual Hurricane::Locator<Occurrence>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Attributes
// **********

	private: const HyperNet* _hyperNet;
	private: bool _doExtraction;
	private: bool _allowInterruption;

// Constructors
// ************

	public: HyperNet_LeafPlugOccurrences();
	public: HyperNet_LeafPlugOccurrences(const HyperNet* hyperNet, bool doExtraction = false, bool allowInterruption = false);
	public: HyperNet_LeafPlugOccurrences(const HyperNet_LeafPlugOccurrences& netOccurrences);

// Operators
// *********

	public: HyperNet_LeafPlugOccurrences& operator=(const HyperNet_LeafPlugOccurrences& netOccurrences);

// Accessors
// *********

	public: virtual Collection<Occurrence>* GetClone() const;
	public: virtual Hurricane::Locator<Occurrence>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// HyperNet implementation
// ****************************************************************************************************

HyperNet::HyperNet(const Occurrence& occurrence)
// *******************************************
:	_netOccurrence()
{
	if (occurrence.IsValid()) {
		Entity* entity = occurrence.GetEntity();
		if (is_a<Net*>(entity))
			_netOccurrence = occurrence;
		else {
			if (is_a<Rubber*>(entity)) {
				Rubber* rubber = (Rubber*)entity;
				_netOccurrence = Occurrence(rubber->GetNet(), occurrence.GetPath());
			}
			else {
				if (is_a<Component*>(entity)) {
					Component* component = (Component*)entity;
					_netOccurrence = Occurrence(component->GetNet(), occurrence.GetPath());
				}
				else
					throw Error("Can't create " + _TName("HyperNet") + " : bad occurrence entity type");
			}
		}
	}
}

Occurrences HyperNet::GetNetOccurrences(bool doExtraction, bool allowInterruption) const
// ***********************************************************************************
{
	return HyperNet_NetOccurrences(this, doExtraction, allowInterruption);
}

Occurrences HyperNet::GetNetOccurrencesUnder(Box area, bool doExtraction, bool allowInterruption) const
// **************************************************************************************************
{
	return HyperNet_NetOccurrencesUnder(this, area, doExtraction, allowInterruption);
}

Occurrences HyperNet::GetLeafPlugOccurrences(bool doExtraction, bool allowInterruption) const
// ********************************************************************************************
{
	return HyperNet_LeafPlugOccurrences(this, doExtraction, allowInterruption);
}

string HyperNet::_GetString() const
// ********************************
{
	string s = "<" + _TName("HyperNet");
	if (IsValid()) s += " " + GetString(_netOccurrence);
	s += ">";
	return s;
}

Record* HyperNet::_GetRecord() const
// ***************************
{
 	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("NetOccurrence", &_netOccurrence));
	}
	return record;
}

Occurrence GetHyperNetRootNetOccurrence(const Occurrence& netoccurrence)
// *********************************************************************
{
    if (!netoccurrence.IsValid())
        throw Error("GetHyperNetRootNetOccurrence : invalid occurrence");
    
    Net* net = dynamic_cast<Net*>(netoccurrence.GetEntity());
    if (!net)
        throw Error("GetHyperNetRootNetOccurrence : not a net occurrence");
    
    if (!net->IsExternal())
        return netoccurrence;

    Path path = netoccurrence.GetPath();
    
    if (path.IsEmpty())
        return netoccurrence;
    
    Instance* instance = path.GetTailInstance();

    Plug* plug = instance->GetPlug(net);
    if (!plug)
        throw Error("GetHyperNetRootNetOccurrence : no plug for external net !");
    
    if(!plug->IsConnected())
        return netoccurrence;
    
    return GetHyperNetRootNetOccurrence(Occurrence(plug->GetNet(), path.GetHeadPath()));
}


bool IsHyperNetRootNetOccurrence(Occurrence netoccurrence)
// *******************************************************
{
    Net* net=dynamic_cast<Net*>(netoccurrence.GetEntity());
    if (!net) return false;
    if (!net->IsExternal())                return true;
    if (netoccurrence.GetPath().IsEmpty()) return true;
    if (net->IsGlobal())                   return false;
    if (!netoccurrence.GetPath().GetTailInstance()->GetPlug(net)->IsConnected())
                                           return true;
    return false;
}



// ****************************************************************************************************
// HyperNet_NetOccurrences implementation
// ****************************************************************************************************

HyperNet_NetOccurrences::HyperNet_NetOccurrences()
// *********************************************
: 	Inherit(),
	_hyperNet(NULL),
	_doExtraction(false),
	_allowInterruption(false)
{
}

HyperNet_NetOccurrences::HyperNet_NetOccurrences(const HyperNet* hyperNet, bool doExtraction, bool allowInterruption)
// ****************************************************************************************************
: 	Inherit(),
	_hyperNet(hyperNet),
	_doExtraction(doExtraction),
	_allowInterruption(allowInterruption)
{
}

HyperNet_NetOccurrences::HyperNet_NetOccurrences(const HyperNet_NetOccurrences& netOccurrences)
// ****************************************************************************************
: 	Inherit(),
	_hyperNet(netOccurrences._hyperNet),
	_doExtraction(netOccurrences._doExtraction),
	_allowInterruption(netOccurrences._allowInterruption)
{
}

HyperNet_NetOccurrences& HyperNet_NetOccurrences::operator=(const HyperNet_NetOccurrences& netOccurrences)
// ***************************************************************************************************
{
	_hyperNet = netOccurrences._hyperNet;
	_doExtraction = netOccurrences._doExtraction;
	_allowInterruption = netOccurrences._allowInterruption;
	return *this;
}

Collection<Occurrence>* HyperNet_NetOccurrences::GetClone() const
// ************************************************************
{
	return new HyperNet_NetOccurrences(*this);
}

Locator<Occurrence>* HyperNet_NetOccurrences::GetLocator() const
// ***********************************************************
{
	return new Locator(_hyperNet, _doExtraction, _allowInterruption);
}

string HyperNet_NetOccurrences::_GetString() const
// **********************************************
{
	string s = "<" + _TName("HyperNet::NetOccurrences");
	if (_hyperNet) {
		s += " " + GetString(_hyperNet);
		if (_doExtraction) {
			s += " DO_EXTRACTION";
			if (_allowInterruption) s += " ALLOW_INTERRUPTION";
		}
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// HyperNet_NetOccurrences::Locator implementation
// ****************************************************************************************************

HyperNet_NetOccurrences::Locator::Locator()
// ***************************************
:	Inherit(),
	_hyperNet(NULL),
	_doExtraction(false),
	_allowInterruption(false),
	_netOccurrenceSet(),
	_netOccurrenceStack()
{
}

HyperNet_NetOccurrences::Locator::Locator(const HyperNet* hyperNet, bool doExtraction, bool allowInterruption)
// ****************************************************************************************************
:	Inherit(),
	_hyperNet(hyperNet),
	_doExtraction(doExtraction),
	_allowInterruption(allowInterruption),
	_netOccurrenceSet(),
	_netOccurrenceStack()
{
	if (_hyperNet) {
		Occurrence netOccurrence = _hyperNet->GetNetOccurrence();
		if (netOccurrence.IsValid()) {
			_netOccurrenceSet.insert(netOccurrence);
			_netOccurrenceStack.push(netOccurrence);
		}
	}
}

HyperNet_NetOccurrences::Locator::Locator(const Locator& locator)
// *************************************************************
:	Inherit(),
	_hyperNet(locator._hyperNet),
	_doExtraction(locator._doExtraction),
	_allowInterruption(locator._allowInterruption),
	_netOccurrenceSet(locator._netOccurrenceSet),
	_netOccurrenceStack(locator._netOccurrenceStack)
{
}

HyperNet_NetOccurrences::Locator& HyperNet_NetOccurrences::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
	_hyperNet = locator._hyperNet;
	_doExtraction = locator._doExtraction;
	_allowInterruption = locator._allowInterruption;
	_netOccurrenceSet = locator._netOccurrenceSet;
	_netOccurrenceStack = locator._netOccurrenceStack;
	return *this;
}

Occurrence HyperNet_NetOccurrences::Locator::GetElement() const
// **********************************************************
{
	return (!_netOccurrenceStack.empty()) ? _netOccurrenceStack.top() : Occurrence();
}

Locator<Occurrence>* HyperNet_NetOccurrences::Locator::GetClone() const
// ******************************************************************
{
	return new Locator(*this);
}

bool HyperNet_NetOccurrences::Locator::IsValid() const
// **************************************************
{
	return !_netOccurrenceStack.empty();
}

static bool IsConnex(const Occurrence& componentOccurrence1, const Occurrence& componentOccurrence2)
// *********************************************************************************************
{
	Component* component1 = (Component*)componentOccurrence1.GetEntity();
	Component* component2 = (Component*)componentOccurrence2.GetEntity();
	Layer* layer1 = component1->GetLayer();
	Layer* layer2 = component2->GetLayer();
	if (layer1->GetExtractMask() & layer2->GetExtractMask()) {
		Transformation transformation1 = componentOccurrence1.GetPath().GetTransformation();
		Transformation transformation2 = componentOccurrence2.GetPath().GetTransformation();
		for_each_basic_layer(basicLayer1, layer1->GetBasicLayers()) {
			Box box1 = transformation1.GetBox(component1->GetBoundingBox(basicLayer1));
			for_each_basic_layer(basicLayer2, layer2->GetBasicLayers()) {
				if (basicLayer1->GetExtractMask() & basicLayer2->GetExtractMask()) {
					Box box2 = transformation2.GetBox(component2->GetBoundingBox(basicLayer2));
					if (box1.Intersect(box2)) return true;
				}
				end_for;
			}
			end_for;
		}
	}
	return false;
}

void HyperNet_NetOccurrences::Locator::Progress()
// *********************************************
{
	if (!_netOccurrenceStack.empty()) {
		Occurrence netOccurrence = _netOccurrenceStack.top();
		_netOccurrenceStack.pop();
		Net* net = (Net*)netOccurrence.GetEntity();
		Path path = netOccurrence.GetPath();

		if (_doExtraction) {
			Cell* cell = netOccurrence.GetOwnerCell();
			for_each_component(component, net->GetComponents()) {
				if (!is_a<Plug*>(component)) {
					//if (_allowInterruption && !((i++) % 200)) gtk_check_for_interruption();
					Occurrence occurrence = Occurrence(component, path);
					Box area = occurrence.GetBoundingBox();
					for_each_occurrence(occurrence2, cell->GetOccurrencesUnder(area)) {
						if (is_a<Component*>(occurrence2.GetEntity())) {
							Component* component2 = (Component*)occurrence2.GetEntity();
							if (IsConnex(occurrence, occurrence2)) {
								Occurrence net2Occurrence =
									Occurrence(component2->GetNet(), occurrence2.GetPath());
								if (_netOccurrenceSet.find(net2Occurrence) == _netOccurrenceSet.end()) {
									_netOccurrenceSet.insert(net2Occurrence);
									_netOccurrenceStack.push(net2Occurrence);
								}
							}
						}
						end_for;
					}
				}
				end_for;
			}
		}

		for_each_plug(plug, net->GetPlugs()) {
			Occurrence occurrence = Occurrence(plug->GetMasterNet(), Path(path, plug->GetInstance()));
			if (_netOccurrenceSet.find(occurrence) == _netOccurrenceSet.end()) {
				_netOccurrenceSet.insert(occurrence);
				_netOccurrenceStack.push(occurrence);
			}
			end_for;
		}

		if (net->IsExternal()) {
			Instance* instance = path.GetTailInstance();
			if (instance) {
				Plug* plug = instance->GetPlug(net);
				if (plug) {
					Net* net = plug->GetNet();
					if (net) {
						Occurrence occurrence = Occurrence(net, path.GetHeadPath());
						if (_netOccurrenceSet.find(occurrence) == _netOccurrenceSet.end()) {
							_netOccurrenceSet.insert(occurrence);
							_netOccurrenceStack.push(occurrence);
						}
					}
				}
			}
		}
	}
}

string HyperNet_NetOccurrences::Locator::_GetString() const
// *******************************************************
{
	string s = "<" + _TName("HyperNet::NetOccurrences::Locator");
	if (_hyperNet) {
		s += " " + GetString(_hyperNet);
		if (_doExtraction) {
			s += " DO_EXTRACTION";
			if (_allowInterruption) s += " ALLOW_INTERRUPTION";
		}
	}
	s += ">";
	return s;
}

// ****************************************************************************************************
// HyperNet_NetOccurrencesUnder implementation
// ****************************************************************************************************

HyperNet_NetOccurrencesUnder::HyperNet_NetOccurrencesUnder()
// *******************************************************
: 	Inherit(),
		_hyperNet(NULL),
		_area(),
		_doExtraction(false),
		_allowInterruption(false)
{
}

HyperNet_NetOccurrencesUnder::HyperNet_NetOccurrencesUnder(const HyperNet* hyperNet,
																			 Box area,  bool doExtraction,
																			 bool allowInterruption)
// ***********************************************************************************
: 	Inherit(),
		_hyperNet(hyperNet),
		_area(area),
		_doExtraction(doExtraction),
		_allowInterruption(allowInterruption)
{
}

HyperNet_NetOccurrencesUnder::HyperNet_NetOccurrencesUnder(const HyperNet_NetOccurrencesUnder& netOccurrences)
// *******************************************************************************************************
: 	Inherit(),
		_hyperNet(netOccurrences._hyperNet),
		_area(netOccurrences._area),
		_doExtraction(netOccurrences._doExtraction),
		_allowInterruption(netOccurrences._allowInterruption)
{
}

HyperNet_NetOccurrencesUnder& HyperNet_NetOccurrencesUnder::operator=(const HyperNet_NetOccurrencesUnder& netOccurrences)
// ******************************************************************************************************************
{
	_hyperNet = netOccurrences._hyperNet;
	_area = netOccurrences._area;
	_doExtraction = netOccurrences._doExtraction;
	_allowInterruption = netOccurrences._allowInterruption;
	return *this;
}

Collection<Occurrence>* HyperNet_NetOccurrencesUnder::GetClone() const
// *****************************************************************
{
	return new HyperNet_NetOccurrencesUnder(*this);
}

Locator<Occurrence>* HyperNet_NetOccurrencesUnder::GetLocator() const
// ****************************************************************
{
	return new Locator(_hyperNet, _area, _doExtraction, _allowInterruption);
}

string HyperNet_NetOccurrencesUnder::_GetString() const
// ***************************************************
{
	string s = "<" + _TName("HyperNet::NetOccurrences");
	if (_hyperNet) {
		s += " " + GetString(_hyperNet);
		if (_doExtraction) {
			s += " DO_EXTRACTION";
			if (_allowInterruption) s += " ALLOW_INTERRUPTION";
		}
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// HyperNet_NetOccurrencesUnder::Locator implementation
// ****************************************************************************************************

HyperNet_NetOccurrencesUnder::Locator::Locator()
// ********************************************
:	Inherit(),
	_hyperNet(NULL),
	_area(),
	_doExtraction(false),
	_allowInterruption(false),
	_netOccurrenceSet(),
	_netOccurrenceStack()
{
}

HyperNet_NetOccurrencesUnder::Locator::Locator(const HyperNet* hyperNet, Box area,
																bool doExtraction, bool allowInterruption)
// *************************************************************************************
:	Inherit(),
	_hyperNet(hyperNet),
	_area(area),
	_doExtraction(doExtraction),
	_allowInterruption(allowInterruption),
	_netOccurrenceSet(),
	_netOccurrenceStack()
{
	if (_hyperNet) {
		Occurrence netOccurrence = _hyperNet->GetNetOccurrence();
		if (netOccurrence.IsValid()) {
			_netOccurrenceSet.insert(netOccurrence);
			_netOccurrenceStack.push(netOccurrence);
		}
	}
}

HyperNet_NetOccurrencesUnder::Locator::Locator(const Locator& locator)
// ******************************************************************
:	Inherit(),
	_hyperNet(locator._hyperNet),
	_area(locator._area),
	_doExtraction(locator._doExtraction),
	_allowInterruption(locator._allowInterruption),
	_netOccurrenceSet(locator._netOccurrenceSet),
	_netOccurrenceStack(locator._netOccurrenceStack)
{
}

HyperNet_NetOccurrencesUnder::Locator& HyperNet_NetOccurrencesUnder::Locator::operator=(const Locator& locator)
// **********************************************************************************************************
{
	_hyperNet = locator._hyperNet;
	_area = locator._area;
	_doExtraction = locator._doExtraction;
	_allowInterruption = locator._allowInterruption;
	_netOccurrenceSet = locator._netOccurrenceSet;
	_netOccurrenceStack = locator._netOccurrenceStack;
	return *this;
}

Occurrence HyperNet_NetOccurrencesUnder::Locator::GetElement() const
// ***************************************************************
{
	return (!_netOccurrenceStack.empty()) ? _netOccurrenceStack.top() : Occurrence();
}

Locator<Occurrence>* HyperNet_NetOccurrencesUnder::Locator::GetClone() const
// ***********************************************************************
{
	return new Locator(*this);
}

bool HyperNet_NetOccurrencesUnder::Locator::IsValid() const
// *******************************************************
{
	return !_netOccurrenceStack.empty();
}

void HyperNet_NetOccurrencesUnder::Locator::Progress()
// **************************************************
{
	if (!_netOccurrenceStack.empty()) {
		Occurrence netOccurrence = _netOccurrenceStack.top();
		_netOccurrenceStack.pop();
		Net* net = (Net*)netOccurrence.GetEntity();
		Path path = netOccurrence.GetPath();

		if (_doExtraction) {
			Cell* cell = netOccurrence.GetOwnerCell();
			for_each_component(component, net->GetComponents()) {
				Occurrence occurrence = Occurrence(component, path);
				Box area = occurrence.GetBoundingBox();
				if (! area.Intersect (_area)) {
					// Outside useful area
				} else if (is_a<Plug*>(component)) {
					// Will be  processed below
				} else if (is_a<Rubber*>(component)) {
					// Don't go through the Rubbers (go only trough connecting layers)
				} else {
					//if (_allowInterruption && !((i++) % 200)) gtk_check_for_interruption();
					Box under = area.GetIntersection (_area);
					for_each_occurrence(occurrence2, cell->GetOccurrencesUnder(under)) {
						if (is_a<Component*>(occurrence2.GetEntity())) {
							Component* component2 = (Component*)occurrence2.GetEntity();
							if (IsConnex(occurrence, occurrence2)) {
								Occurrence net2Occurrence =
									Occurrence(component2->GetNet(), occurrence2.GetPath());
								if (_netOccurrenceSet.find(net2Occurrence) == _netOccurrenceSet.end()) {
									_netOccurrenceSet.insert(net2Occurrence);
									_netOccurrenceStack.push(net2Occurrence);
								}
							}
						}
						end_for;
					}
				}
				end_for;
			}
		}

		for_each_plug(plug, net->GetPlugs()) {
			Occurrence occurrence = Occurrence(plug->GetMasterNet(), Path(path, plug->GetInstance()));
			if (_netOccurrenceSet.find(occurrence) == _netOccurrenceSet.end()) {
				_netOccurrenceSet.insert(occurrence);
				_netOccurrenceStack.push(occurrence);
			}
			end_for;
		}

		if (net->IsExternal()) {
			Instance* instance = path.GetTailInstance();
			if (instance) {
				Plug* plug = instance->GetPlug(net);
				if (plug) {
					Net* net = plug->GetNet();
					if (net) {
						Occurrence occurrence = Occurrence(net, path.GetHeadPath());
						if (_netOccurrenceSet.find(occurrence) == _netOccurrenceSet.end()) {
							_netOccurrenceSet.insert(occurrence);
							_netOccurrenceStack.push(occurrence);
						}
					}
				}
			}
		}
	}
}

string HyperNet_NetOccurrencesUnder::Locator::_GetString() const
// ***********************************************************
{
	string s = "<" + _TName("HyperNet::NetOccurrences::Locator");
	if (_hyperNet) {
		s += " " + GetString(_hyperNet);
		if (_doExtraction) {
			s += " DO_EXTRACTION";
			if (_allowInterruption) s += " ALLOW_INTERRUPTION";
		}
	}
	s += ">";
	return s;
}

// ****************************************************************************************************
// HyperNet_LeafPlugOccurrences implementation
// ****************************************************************************************************

HyperNet_LeafPlugOccurrences::HyperNet_LeafPlugOccurrences()
// *********************************************
: 	Inherit(),
	_hyperNet(NULL),
	_doExtraction(false),
	_allowInterruption(false)
{
}

HyperNet_LeafPlugOccurrences::HyperNet_LeafPlugOccurrences(const HyperNet* hyperNet, bool doExtraction, bool allowInterruption)
// ****************************************************************************************************
: 	Inherit(),
	_hyperNet(hyperNet),
	_doExtraction(doExtraction),
	_allowInterruption(allowInterruption)
{
}

HyperNet_LeafPlugOccurrences::HyperNet_LeafPlugOccurrences(const HyperNet_LeafPlugOccurrences& netOccurrences)
// ****************************************************************************************
: 	Inherit(),
	_hyperNet(netOccurrences._hyperNet),
	_doExtraction(netOccurrences._doExtraction),
	_allowInterruption(netOccurrences._allowInterruption)
{
}

HyperNet_LeafPlugOccurrences& HyperNet_LeafPlugOccurrences::operator=(const HyperNet_LeafPlugOccurrences& netOccurrences)
// ***************************************************************************************************
{
	_hyperNet = netOccurrences._hyperNet;
	_doExtraction = netOccurrences._doExtraction;
	_allowInterruption = netOccurrences._allowInterruption;
	return *this;
}

Collection<Occurrence>* HyperNet_LeafPlugOccurrences::GetClone() const
// ************************************************************
{
	return new HyperNet_LeafPlugOccurrences(*this);
}

Locator<Occurrence>* HyperNet_LeafPlugOccurrences::GetLocator() const
// ***********************************************************
{
	return new Locator(_hyperNet, _doExtraction, _allowInterruption);
}

string HyperNet_LeafPlugOccurrences::_GetString() const
// **********************************************
{
	string s = "<" + _TName("HyperNet::LeafPlugOccurrences");
	if (_hyperNet) {
		s += " " + GetString(_hyperNet);
		if (_doExtraction) {
			s += " DO_EXTRACTION";
			if (_allowInterruption) s += " ALLOW_INTERRUPTION";
		}
	}
	s += ">";
	return s;
}



// ****************************************************************************************************
// HyperNet_LeafPlugOccurrences::Locator implementation
// ****************************************************************************************************

HyperNet_LeafPlugOccurrences::Locator::Locator()
// ***************************************
:	Inherit(),
    _netOccurrenceLocator(),
    _plugOccurrence()
    
{
}

HyperNet_LeafPlugOccurrences::Locator::Locator(const HyperNet* hyperNet, bool doExtraction, bool allowInterruption)
// ****************************************************************************************************
:	Inherit(),
    _netOccurrenceLocator(),
    _plugOccurrence()
{
	if (hyperNet) {
        _netOccurrenceLocator = hyperNet->GetNetOccurrences(doExtraction,allowInterruption).GetLocator();
        Progress();
	}
}

HyperNet_LeafPlugOccurrences::Locator::Locator(const Locator& locator)
// *************************************************************
:	Inherit(),
    _netOccurrenceLocator(),
    _plugOccurrence()
{
}

HyperNet_LeafPlugOccurrences::Locator& HyperNet_LeafPlugOccurrences::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
    _netOccurrenceLocator = locator._netOccurrenceLocator;
    _plugOccurrence = locator._plugOccurrence;
	return *this;
}

Occurrence HyperNet_LeafPlugOccurrences::Locator::GetElement() const
// **********************************************************
{
	return _plugOccurrence;
}

Locator<Occurrence>* HyperNet_LeafPlugOccurrences::Locator::GetClone() const
// ******************************************************************
{
	return new Locator(*this);
}

bool HyperNet_LeafPlugOccurrences::Locator::IsValid() const
// **************************************************
{
	return _plugOccurrence.IsValid();
}


void HyperNet_LeafPlugOccurrences::Locator::Progress()
// *********************************************
{
    _plugOccurrence = Occurrence();
    while(_netOccurrenceLocator.IsValid() && !_plugOccurrence.IsValid())
    {
        Occurrence netOccurrence = _netOccurrenceLocator.GetElement();
        _netOccurrenceLocator.Progress();
        Net* net = (Net*)netOccurrence.GetEntity();
        Path path = netOccurrence.GetPath();
        if (!path.IsEmpty() && net->GetCell()->IsLeaf())
        {
            Instance *instance = path.GetTailInstance();
            Plug *plug=instance->GetPlug(net);
            if (plug)
                _plugOccurrence=Occurrence(plug,path.GetHeadPath());
        }
    }
}

string HyperNet_LeafPlugOccurrences::Locator::_GetString() const
// *******************************************************
{
	string s = "<" + _TName("HyperNet::LeafPlugOccurrences::Locator");
    s += " " + GetString(_netOccurrenceLocator);
	s += ">";
	return s;
}


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
