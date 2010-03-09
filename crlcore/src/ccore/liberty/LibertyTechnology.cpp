
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
#include  "hurricane/Error.h"
using namespace Hurricane;

#include  "LibertyTechnology.h"


namespace {
using namespace CRL;

map<CLuTableTemplate::Variable::Type,CLibertyTechnology::Unit> __grandeurs;

void Init()
// ********
{
    static bool done=false;
    __grandeurs[CLuTableTemplate::Variable::Type::CONSTRAINED_PIN_TRANSITION]=   CLibertyTechnology::Unit::TIME;
    __grandeurs[CLuTableTemplate::Variable::Type::RELATED_PIN_TRANSITION]=       CLibertyTechnology::Unit::TIME;
    __grandeurs[CLuTableTemplate::Variable::Type::INPUT_NET_TRANSITION]=         CLibertyTechnology::Unit::TIME;
    __grandeurs[CLuTableTemplate::Variable::Type::TOTAL_OUTPUT_NET_CAPACITANCE]= CLibertyTechnology::Unit::CAPACITANCE;
    done=true;
}

} //namespace

namespace CRL {

CLibertyTechnology::CLibertyTechnology()
// *************************************
    : Inherit()
    , _templates()
    , _units()
{
}

CLibertyTechnology* CLibertyTechnology::create()
// *********************************************
{
    CLibertyTechnology* technology = new CLibertyTechnology();

    technology->_postCreate();

    return technology;
}

void CLibertyTechnology::_postCreate()
// ***********************************
{
    Inherit::_postCreate();
    Init();
}

Name CLibertyTechnology::getName() const
// *************************************
{
    return getPropertyName();
}

const Name& CLibertyTechnology::getPropertyName()
// **********************************************
{
    static Name NAME = "Coriolis::CLibertyTechnology";
    return NAME;
}
    
string CLibertyTechnology::_getString() const
// ******************************************
{
    string s = Inherit::_getString();
	s.insert(s.length() - 1, " " + getString(getOwner()));
    return s;
}

Record* CLibertyTechnology::_getRecord() const
// *************************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
      //record->add(getSlot("Units", &_units));
      //record->add(getSlot("Fundamental Quantities Map", &__grandeurs));
    }
    return record;
}

CLuTableTemplate* CLibertyTechnology::getTemplate(string name) const
// *****************************************************************
{
    static CLuTableTemplate searchTemplate(name,NULL);

    searchTemplate._name=name;

    TemplateSet::iterator tit = _templates.find(&searchTemplate);

    if (tit != _templates.end()) return *tit;
    return NULL;
}

double CLibertyTechnology::getUnit(Unit unit) const
// ************************************************
{
    map<Unit,double>::const_iterator cit=_units.find(unit);
    if (cit == _units.end())
        throw Error("Unknown unit unit");
    return cit->second;
}

double CLibertyTechnology::getUnit(CLuTableTemplate::Variable::Type type) const
// ****************************************************************************
{
    if (__grandeurs.find(type) == __grandeurs.end())
        throw Error("Oups : variable unit "+getString(type)+" not registered (in CLibertyTechnology::Init())");

    return getUnit(__grandeurs[type]);
}

void CLibertyTechnology::AddTemplate(CLuTableTemplate* lutemplate)
// ***************************************************************
{
    _templates.insert(lutemplate);
}

void CLibertyTechnology::SetUnit(Unit unit,double val)
// ***************************************************
{
  ltrace(359) << "Setting Unit " << getString((long)unit) << " = " << val << endl;
  _units[unit]=val;
}


// ****************************************************************************************************
// CLibertyTechnology::Unit definition
// ****************************************************************************************************

CLibertyTechnology::Unit::Unit(const Code& code)
// *********************************************
:	_code(code)
{
}

CLibertyTechnology::Unit::Unit(const CLibertyTechnology::Unit& unit)
// *****************************************************************
:	_code(unit._code)
{
}

CLibertyTechnology::Unit& CLibertyTechnology::Unit::operator=(const CLibertyTechnology::Unit& unit)
// ************************************************************************************************
{
	_code = unit._code;
	return *this;
}

string CLibertyTechnology::Unit::_getString() const
// ************************************************
{
	switch (_code) {
		case TIME :       return "TIME";
        case CAPACITANCE: return "CAPACITANCE";
        default:
          break;
	}
	return "ABNORMAL";
}

Record* CLibertyTechnology::Unit::_getRecord() const
// *******************************************
{
  Record* record = new Record(getString((long)this));
	record->add(getSlot("Code", (unsigned*)&_code));
	return record;
}

} //namespace CRL
