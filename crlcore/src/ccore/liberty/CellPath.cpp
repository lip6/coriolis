
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
// Date   : 15/05/2007
// Author : Marek Sroka                    <Marek.Sroka@lip6.fr>
//
// Authors-Tag 
#include  "hurricane/Property.h"
#include  "hurricane/Cell.h"
using namespace Hurricane;

#include  "LuTable.h"

#include "crlcore/CellPath.h"
#include "crlcore/CellPaths.h"

namespace CRL {

// ****************************************************************************************************
// CCellPath_Property declaration
// ****************************************************************************************************

class CCellPath_Property : public PrivateProperty {

    // Types
    // *****
    public: typedef PrivateProperty Inherit;

    // Attributes
    // **********
    private:list<CCellPath*> _cellPathList;

    // Constructors
    // ************
    private: CCellPath_Property(CCellPath* cellPath);
    public: static CCellPath_Property* create(CCellPath* cellPath);

    // Accessors
    // *********
    public: virtual Name getName() const;
    public: CCellPaths getCellPaths() const { return getCollection(_cellPathList); }
            
    // Others
    // ******
    public: virtual string _getTypeName() const { return _TName("CCellPath_Property"); }
    public: virtual Record* _getRecord() const;
    public: static const Name& getPropertyName();
    
    public: static void RegisterCellPath(CCellPath* cellPath);
            
    public: void AddCellPath(CCellPath* cellPath);
    public: CCellPath* getCellPath(Net* relatedNet) const;

};

// ****************************************************************************************************
// CCellPath definition
// ****************************************************************************************************
    
CCellPath::CCellPath(Net* net)
// ***************************
    : Inherit()
    , _net(net)
    , _relatedNet(NULL)
    , _type()
    , _riseDelay(NULL)
    , _fallDelay(NULL)
    , _riseTransition(NULL)
    , _fallTransition(NULL)
{
}

CCellPath* CCellPath::create(Net* net)
// ***********************************
{
    CCellPath* cCellPath= new CCellPath(net);

    cCellPath->_postCreate();

    return cCellPath;
}

void CCellPath::_postCreate()
// **************************
{
    Inherit::_postCreate();

    CCellPath_Property::RegisterCellPath(this);
}

string CCellPath::_getString() const
// *********************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1," "+getString(_net->getCell()->getName())+ " "+getString(_net->getName()));
    if (_relatedNet)
        s.insert(s.length() - 1," ("+getString(_relatedNet->getName())+") ");
    s.insert(s.length() -1, getString(_type));
    return s;
}

Record* CCellPath::_getRecord() const
// ****************************
{
    Record* record = Inherit::_getRecord();
    if (record)
    {
        record->add(getSlot("Net", _net));
        record->add(getSlot("Related Net", _relatedNet));
        record->add(getSlot("Type", _type));
        record->add(getSlot("Rise Delay Table", _riseDelay));
        record->add(getSlot("Fall Delay Table", _fallDelay));
        record->add(getSlot("Rise Transition Table", _riseTransition));
        record->add(getSlot("Fall Transition Table", _fallTransition));
    }
    return record;
}

CLuTable* CCellPath::_createRiseDelayTable(CLuTableTemplate* luTemplate)
// *********************************************************************
{
    return _riseDelay= CLuTable::create(luTemplate);
}

CLuTable* CCellPath::_createFallDelayTable(CLuTableTemplate* luTemplate)
// *********************************************************************
{
    return _fallDelay= CLuTable::create(luTemplate);
}

CLuTable* CCellPath::_createRiseTransitionTable(CLuTableTemplate* luTemplate)
// **************************************************************************
{
    return _riseTransition= CLuTable::create(luTemplate);
}

CLuTable* CCellPath::_createFallTransitionTable(CLuTableTemplate* luTemplate)
// **************************************************************************
{
    return _fallTransition= CLuTable::create(luTemplate);
}

CLuTable* CCellPath::getDelayTable(CTimingEvent event) const
// *********************************************************
{
    switch(event)
    {
        case CRL::CTimingEvent::FALLING_EDGE: return _fallDelay;
        case CRL::CTimingEvent::RISING_EDGE: return _riseDelay;
        default:
          throw Error("Undefined Timing Event");
    }

    return NULL;
}

CLuTable* CCellPath::getTransitionTable(CTimingEvent event) const
// **************************************************************
{
    switch(event)
    {
        case CRL::CTimingEvent::FALLING_EDGE: return _fallTransition;
        case CRL::CTimingEvent::RISING_EDGE: return _riseTransition;
        default:
          throw Error("Undefined Timing Event");
    }
    return NULL;
}

// ****************************************************************************************************
// getCellPaths(Cell*) definition
// ****************************************************************************************************

CCellPaths getCellPaths(const Cell* cell)
// **************************************
{
    Property* property=cell->getProperty(CCellPath_Property::getPropertyName());
    if (!property)
        return CCellPaths();
    if (CCellPath_Property* ccpProperty = dynamic_cast<CCellPath_Property*>(property)) {
        return ccpProperty->getCellPaths();
    } else {
        throw Error(getString(CCellPath_Property::getPropertyName())+" : bad property type");
    }
}

// ****************************************************************************************************
// CCellPath_Property definition
// ****************************************************************************************************

CCellPath_Property::CCellPath_Property(CCellPath* cellPath)
// ********************************************************
: _cellPathList()
{
    _cellPathList.push_back(cellPath);
}

CCellPath_Property* CCellPath_Property::create(CCellPath* cellPath)
// ****************************************************************
{
    CCellPath_Property* property = new CCellPath_Property(cellPath);

    property->_postCreate();

    return property;
}

void CCellPath_Property::RegisterCellPath(CCellPath* cellPath)
// ***********************************************************
{
    Cell* cell = cellPath->getNet()->getCell();
    Property* property=cell->getProperty(getPropertyName());
    if (property) {
        if (CCellPath_Property* ccpProperty = dynamic_cast<CCellPath_Property*>(property)) {
            ccpProperty->AddCellPath(cellPath);
        } else {
            throw Error(getString(getPropertyName())+" : bad property type");
        }
    } else {
        property = CCellPath_Property::create(cellPath);
        cell->put(property);
    }
}

Record* CCellPath_Property::_getRecord() const
// *************************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("CellPathList",&_cellPathList));
    }
    return record;
}

Name CCellPath_Property::getName() const
// *************************************
{
    return getPropertyName();
}

const Name& CCellPath_Property::getPropertyName()  
// **********************************************
{
    static Name NAME = "Coriolis::TimingCellPaths";  
    return NAME;  
}

void CCellPath_Property::AddCellPath(CCellPath* cellPath)
// ******************************************************
{
    _cellPathList.push_back(cellPath);
}

CCellPath* CCellPath_Property::getCellPath(Net* relatedNet) const
// **************************************************************
{
    for(list<CCellPath*>::const_iterator it=_cellPathList.begin();it!=_cellPathList.end();it++)
    {
        if ((*it)->getRelatedNet() == relatedNet)
            return (*it);
    }
    return NULL;
}

// ****************************************************************************************************
// CCellPath::Type definition
// ****************************************************************************************************

CCellPath::Type::Type(const Code& code)
// ************************************
:	_code(code)
{
}

CCellPath::Type::Type(const CCellPath::Type& type)
// ***********************************************
:	_code(type._code)
{
}

CCellPath::Type& CCellPath::Type::operator=(const CCellPath::Type& type)
// *********************************************************************
{
	_code = type._code;
	return *this;
}

string CCellPath::Type::_getString() const
// ***************************************
{
	switch (_code) {
		case UNDEFINED :     return "UNDEFINED";
        case POSITIVE_UNATE: return "POSITIVE_UNATE";
        case NEGATIVE_UNATE: return "NEGATIVE_UNATE";
        case LATCH_ACCESS:   return "LATCH_ACCESS";
        case LATCH_SETUP:    return "LATCH_SETUP";
        case LATCH_HOLD:     return "LATCH_HOLD";
	}
	return "ABNORMAL";
}

Record* CCellPath::Type::_getRecord() const
// **********************************
{
	Record* record = new Record(getString(this));
	record->add(getSlot("Code", (unsigned*)&_code));
	return record;
}

} //namespace CRL
