
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
// 
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 16/04/2007
// Author : Marek Sroka                    <Marek.Sroka@lip6.fr>
//
// Authors-Tag 

#include  "hurricane/Net.h"
#include  "hurricane/HyperNet.h"
#include  "hurricane/Pin.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Filter.h"
using namespace Hurricane;

// ****************************************************************************************************
// Filters
// ****************************************************************************************************

namespace {
    
class InputPin : public Filter<Pin*> {  
// ************************************  
    public: InputPin() {}
    public: InputPin(const InputPin&) {}
    public: InputPin& operator=(const InputPin&) {  return *this;  }  
    public: virtual Filter<Pin*>* getClone() const { return new InputPin(*this); }  
    public: virtual bool accept(Pin* pin) const  {  
        switch (pin->getNet()->getDirection())
        {
            case Net::Direction::IN:
            case Net::Direction::INOUT:     return true;    
            case Net::Direction::TRISTATE:  // treated as OUT
            case Net::Direction::OUT:       return false;
            case Net::Direction::UNDEFINED:
            default:  cerr << "Warning : pin " << pin << " has UNDEFINED Direction" << endl; return false;
        }
    }  
    public: virtual string _getString() const { return "<InputPinFilter>"; }
};  

class OutputPin : public Filter<Pin*> {  
// ************************************  
    public: OutputPin() {}
    public: OutputPin(const OutputPin&) {}
    public: OutputPin& operator=(const OutputPin&) {  return *this;  }  
    public: virtual Filter<Pin*>* getClone() const { return new OutputPin(*this); }  
    public: virtual bool accept(Pin* pin) const  {  
        switch (pin->getNet()->getDirection())
        {
            case Net::Direction::IN:        return false;
            case Net::Direction::INOUT:
            case Net::Direction::TRISTATE:  // treated as OUT
            case Net::Direction::OUT:       return true;
            case Net::Direction::UNDEFINED:
            default:  cerr << "Warning : pin " << pin << " has UNDEFINED Direction" << endl; return false;
        }
    }  
    public: virtual string _getString() const { return "<OutputPinFilter>"; }
};  

class InputPlugOccurrence : public Filter<Occurrence> {  
// ****************************************************  
    public: InputPlugOccurrence() {}
    public: InputPlugOccurrence(const InputPlugOccurrence&) {}
    public: InputPlugOccurrence& operator=(const InputPlugOccurrence&) {  return *this;  }  
    public: virtual Filter<Occurrence>* getClone() const { return new InputPlugOccurrence(*this); }  
    public: virtual bool accept(Occurrence plugoccurrence) const  {  
        Plug* plug = static_cast<Plug*>(plugoccurrence.getEntity());
        switch (plug->getMasterNet()->getDirection())
        {
            case Net::Direction::IN:
            case Net::Direction::INOUT:     return true;
            case Net::Direction::TRISTATE:  // treated as OUT
            case Net::Direction::OUT:       return false;
            case Net::Direction::UNDEFINED:
            default:  cerr << "Warning : plug " << plug << " has UNDEFINED Direction" << endl; return false;
        }
    }  
    public: virtual string _getString() const { return "<InputPlugOccurrenceFilter>"; }
};  

class OutputPlugOccurrence : public Filter<Occurrence> {  
// *****************************************************  
    public: OutputPlugOccurrence() {}
    public: OutputPlugOccurrence(const OutputPlugOccurrence&) {}
    public: OutputPlugOccurrence& operator=(const OutputPlugOccurrence&) {  return *this;  }  
    public: virtual Filter<Occurrence>* getClone() const { return new OutputPlugOccurrence(*this); }  
    public: virtual bool accept(Occurrence plugoccurrence) const  {  
        Plug* plug = static_cast<Plug*>(plugoccurrence.getEntity());
        switch (plug->getMasterNet()->getDirection())
        {
            case Net::Direction::IN:        return false;
            case Net::Direction::INOUT:
            case Net::Direction::TRISTATE:  // treated as OUT
            case Net::Direction::OUT:       return true;
            case Net::Direction::UNDEFINED:
            default:  cerr << "Warning : plug " << plug << " has UNDEFINED Direction" << endl; return false;
        }
    }  
    public: virtual string _getString() const { return "<OutputPlugOccurrenceFilter>"; }
};

} //namespace

namespace CRL {

// ****************************************************************************************************
// CHyperNetDriverPortOccurrences declaration
// ****************************************************************************************************

class CHyperNetDriverPortOccurrences : public Collection<Occurrence> {
// *******************************************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// ************************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

		private: PinLocator        _pinLocator;
        private: OccurrenceLocator _plugOccurrenceLocator;

		public: Locator();
		public: Locator(Occurrence hnrnoccurrence);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence getElement() const;
		public: virtual Hurricane::Locator<Occurrence>* getClone() const;

		public: virtual bool isValid() const;

		public: virtual void progress();

		public: virtual string _getString() const;

	};

// Attributes
// **********

	private: Occurrence _hnrnOccurrence;

// Constructors
// ************

	public: CHyperNetDriverPortOccurrences();
	public: CHyperNetDriverPortOccurrences(Occurrence netoccurrence);
	public: CHyperNetDriverPortOccurrences(const CHyperNetDriverPortOccurrences& nodes);

// Operators
// *********

	public: CHyperNetDriverPortOccurrences& operator=(const CHyperNetDriverPortOccurrences& nodes);

// Accessors
// *********

	public: virtual Collection<Occurrence>* getClone() const;
	public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

	public: virtual string _getString() const;

};

// ****************************************************************************************************
// CHyperNetReceiverPortOccurrences declaration
// ****************************************************************************************************

class CHyperNetReceiverPortOccurrences : public Collection<Occurrence> {
// *********************************************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// ************************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

		private: PinLocator        _pinLocator;
        private: OccurrenceLocator _plugOccurrenceLocator;

		public: Locator();
		public: Locator(Occurrence hnrnoccurrence);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence getElement() const;
		public: virtual Hurricane::Locator<Occurrence>* getClone() const;

		public: virtual bool isValid() const;

		public: virtual void progress();

		public: virtual string _getString() const;

	};

// Attributes
// **********

	private: Occurrence _hnrnOccurrence;

// Constructors
// ************

	public: CHyperNetReceiverPortOccurrences();
	public: CHyperNetReceiverPortOccurrences(Occurrence netoccurrence);
	public: CHyperNetReceiverPortOccurrences(const CHyperNetReceiverPortOccurrences& nodes);

// Operators
// *********

	public: CHyperNetReceiverPortOccurrences& operator=(const CHyperNetReceiverPortOccurrences& nodes);

// Accessors
// *********

	public: virtual Collection<Occurrence>* getClone() const;
	public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

	public: virtual string _getString() const;

};

// ****************************************************************************************************
// Generic Functions
// ****************************************************************************************************

Occurrences getHyperNetDriverPortOccurrences(Occurrence netoccurence)
// ******************************************************************
{
    return CHyperNetDriverPortOccurrences(netoccurence);
}

Occurrences getHyperNetReceiverPortOccurrences(Occurrence netoccurence)
// ********************************************************************
{
    return CHyperNetReceiverPortOccurrences(netoccurence);
}

// ****************************************************************************************************
// CHyperNetDriverPortOccurrences implementation
// ****************************************************************************************************

CHyperNetDriverPortOccurrences::CHyperNetDriverPortOccurrences()
// *************************************************************
    : Inherit()
	, _hnrnOccurrence()
{
}

CHyperNetDriverPortOccurrences::CHyperNetDriverPortOccurrences(Occurrence netoccurrence)
// *************************************************************************************
    : Inherit()
	, _hnrnOccurrence()
{
    _hnrnOccurrence = getHyperNetRootNetOccurrence(netoccurrence);
}

CHyperNetDriverPortOccurrences::CHyperNetDriverPortOccurrences(const CHyperNetDriverPortOccurrences& nodes)
// ********************************************************************************************************
    : Inherit()
	, _hnrnOccurrence(nodes._hnrnOccurrence)
{
}

CHyperNetDriverPortOccurrences& CHyperNetDriverPortOccurrences::operator=(const CHyperNetDriverPortOccurrences& nodes)
// *******************************************************************************************************************
{
	_hnrnOccurrence = nodes._hnrnOccurrence;
	return *this;
}

Collection<Occurrence>* CHyperNetDriverPortOccurrences::getClone() const
// *********************************************************************
{
	return new CHyperNetDriverPortOccurrences(*this);
}

Locator<Occurrence>* CHyperNetDriverPortOccurrences::getLocator() const
// ********************************************************************
{
	return new Locator(_hnrnOccurrence);
}

string CHyperNetDriverPortOccurrences::_getString() const
// ******************************************************
{
	string s = "<" + _TName("CHyperNetDriverPortOccurrences");
	s += " " + getString(_hnrnOccurrence);
    s += ">";
	return s;
}

// ****************************************************************************************************
// CHyperNetDriverPortOccurrences::Locator implementation
// ****************************************************************************************************

CHyperNetDriverPortOccurrences::Locator::Locator()
// ***********************************************
    : Inherit()
	, _pinLocator()
	, _plugOccurrenceLocator()
{
}

CHyperNetDriverPortOccurrences::Locator::Locator(Occurrence hnrnoccurrence)
// ************************************************************************
    : Inherit()
	, _pinLocator()
	, _plugOccurrenceLocator()
{
    Pins pins;
    if (hnrnoccurrence.getPath().isEmpty() and static_cast<Net*>(hnrnoccurrence.getEntity())->isExternal())
        _pinLocator = static_cast<Net*>(hnrnoccurrence.getEntity())->getPins().getSubSet(InputPin()).getLocator();

    _plugOccurrenceLocator = HyperNet(hnrnoccurrence).getLeafPlugOccurrences().getSubSet(OutputPlugOccurrence()).getLocator();
}

CHyperNetDriverPortOccurrences::Locator::Locator(const Locator& locator)
// *********************************************************************
    : Inherit()
	, _pinLocator(locator._pinLocator)
	, _plugOccurrenceLocator(locator._plugOccurrenceLocator)
{
}

CHyperNetDriverPortOccurrences::Locator& CHyperNetDriverPortOccurrences::Locator::operator=(const Locator& locator)
// ****************************************************************************************************************
{
	_pinLocator=locator._pinLocator;
	_plugOccurrenceLocator=locator._plugOccurrenceLocator;
	return *this;
}

Occurrence CHyperNetDriverPortOccurrences::Locator::getElement() const
// *******************************************************************
{
    if (_pinLocator.isValid())
        return _pinLocator.getElement();
    return _plugOccurrenceLocator.getElement();
}

Locator<Occurrence>* CHyperNetDriverPortOccurrences::Locator::getClone() const
// ***************************************************************************
{
	return new Locator(*this);
}

bool CHyperNetDriverPortOccurrences::Locator::isValid() const
// **********************************************************
{
	return _pinLocator.isValid() or _plugOccurrenceLocator.isValid();
}

void CHyperNetDriverPortOccurrences::Locator::progress()
// *****************************************************
{
    if (_pinLocator.isValid())
        _pinLocator.progress();
    if (!_pinLocator.isValid() and _plugOccurrenceLocator.isValid())
        _plugOccurrenceLocator.progress();
}


string CHyperNetDriverPortOccurrences::Locator::_getString() const
// ***************************************************************
{
	string s = "<" + _TName("CHyperNetDriverPortOccurrences::Locator");
    s += " " + getString(_pinLocator);
    s += " " + getString(_plugOccurrenceLocator);
	s += ">";
	return s;
}

// ****************************************************************************************************
// CHyperNetReceiverPortOccurrences implementation
// ****************************************************************************************************

CHyperNetReceiverPortOccurrences::CHyperNetReceiverPortOccurrences()
// *****************************************************************
    : Inherit()
	, _hnrnOccurrence()
{
}

CHyperNetReceiverPortOccurrences::CHyperNetReceiverPortOccurrences(Occurrence netoccurrence)
// *****************************************************************************************
    : Inherit()
	, _hnrnOccurrence()
{
    _hnrnOccurrence = getHyperNetRootNetOccurrence(netoccurrence);
}

CHyperNetReceiverPortOccurrences::CHyperNetReceiverPortOccurrences(const CHyperNetReceiverPortOccurrences& nodes)
// **************************************************************************************************************
    : Inherit()
	, _hnrnOccurrence(nodes._hnrnOccurrence)
{
}

CHyperNetReceiverPortOccurrences& CHyperNetReceiverPortOccurrences::operator=(const CHyperNetReceiverPortOccurrences& nodes)
// *************************************************************************************************************************
{
	_hnrnOccurrence = nodes._hnrnOccurrence;
	return *this;
}

Collection<Occurrence>* CHyperNetReceiverPortOccurrences::getClone() const
// ***********************************************************************
{
	return new CHyperNetReceiverPortOccurrences(*this);
}

Locator<Occurrence>* CHyperNetReceiverPortOccurrences::getLocator() const
// **********************************************************************
{
	return new Locator(_hnrnOccurrence);
}

string CHyperNetReceiverPortOccurrences::_getString() const
// ********************************************************
{
	string s = "<" + _TName("CHyperNetReceiverPortOccurrences");
	s += " " + getString(_hnrnOccurrence);
    s += ">";
	return s;
}

// ****************************************************************************************************
// CHyperNetReceiverPortOccurrences::Locator implementation
// ****************************************************************************************************

CHyperNetReceiverPortOccurrences::Locator::Locator()
// *************************************************
    : Inherit()
	, _pinLocator()
	, _plugOccurrenceLocator()
{
}

CHyperNetReceiverPortOccurrences::Locator::Locator(Occurrence hnrnoccurrence)
// **************************************************************************
    : Inherit()
	, _pinLocator()
	, _plugOccurrenceLocator()
{
    Pins pins;
    if (hnrnoccurrence.getPath().isEmpty() and static_cast<Net*>(hnrnoccurrence.getEntity())->isExternal())
        _pinLocator = static_cast<Net*>(hnrnoccurrence.getEntity())->getPins().getSubSet(OutputPin()).getLocator();

    _plugOccurrenceLocator = HyperNet(hnrnoccurrence).getLeafPlugOccurrences().getSubSet(InputPlugOccurrence()).getLocator();
}

CHyperNetReceiverPortOccurrences::Locator::Locator(const Locator& locator)
// ***********************************************************************
    : Inherit()
	, _pinLocator(locator._pinLocator)
	, _plugOccurrenceLocator(locator._plugOccurrenceLocator)
{
}

CHyperNetReceiverPortOccurrences::Locator& CHyperNetReceiverPortOccurrences::Locator::operator=(const Locator& locator)
// ********************************************************************************************************************
{
	_pinLocator=locator._pinLocator;
	_plugOccurrenceLocator=locator._plugOccurrenceLocator;
	return *this;
}

Occurrence CHyperNetReceiverPortOccurrences::Locator::getElement() const
// *********************************************************************
{
    if (_pinLocator.isValid())
        return _pinLocator.getElement();
	return _plugOccurrenceLocator.getElement();
}

Locator<Occurrence>* CHyperNetReceiverPortOccurrences::Locator::getClone() const
// *****************************************************************************
{
	return new Locator(*this);
}

bool CHyperNetReceiverPortOccurrences::Locator::isValid() const
// ************************************************************
{
	return _pinLocator.isValid() or _plugOccurrenceLocator.isValid();
}

void CHyperNetReceiverPortOccurrences::Locator::progress()
// *******************************************************
{
    if (_pinLocator.isValid())
        _pinLocator.progress();
    if (!_pinLocator.isValid() and _plugOccurrenceLocator.isValid())
        _plugOccurrenceLocator.progress();
}


string CHyperNetReceiverPortOccurrences::Locator::_getString() const
// *****************************************************************
{
	string s = "<" + _TName("CHyperNetReceiverPortOccurrences::Locator");
    s += " " + getString(_pinLocator);
    s += " " + getString(_plugOccurrenceLocator);
	s += ">";
	return s;
}

} //namespace CRL
