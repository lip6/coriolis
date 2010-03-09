
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
#include <iostream>
#include <map>
using namespace std;

#include  "hurricane/Error.h"
using namespace Hurricane;

#include  "LuTableTemplate.h"
#include  "LibertyTechnology.h"


namespace CRL {
    
extern void LibertyParserError(string message);

// **************************************************************************
// CLuTableTemplate definition
// **************************************************************************

CLuTableTemplate::CLuTableTemplate(string name, CLibertyTechnology* technology)
// ****************************************************************************
    : Inherit()
    , _name(name)
    , _technology(technology)
{
    _variables[0]=_variables[1]=_variables[2]=NULL;
}

CLuTableTemplate* CLuTableTemplate::create(string name, CLibertyTechnology* technology)
// ************************************************************************************
{
    CLuTableTemplate* luTableTemplate = new CLuTableTemplate(name,technology);

    luTableTemplate->_postCreate();

    return luTableTemplate;
}

void CLuTableTemplate::_postCreate()
// *********************************
{
    Inherit::_postCreate();
    _technology->AddTemplate(this);
}

CLuTableTemplate::~CLuTableTemplate()
// **********************************
{
    for(unsigned short i=0;i<3;i++)
        if (_variables[i])
            delete _variables[i];
}

string CLuTableTemplate::_getString() const
// ****************************************
{
    string s = Inherit::_getString();
    s.insert(s.length() -1, " "+_name);
    return s;
}

Record* CLuTableTemplate::_getRecord() const
// ***********************************
{
    Record* record = Inherit::_getRecord();
    if (record)
    {
        record->add(getSlot("Name", _name));
        record->add(getSlot("Technology", _technology));
        record->add(getSlot("Dimension", (unsigned int)getDimension()));

        record->add(getSlot("Variable 1", _variables[0]));
        record->add(getSlot("Variable 2", _variables[1]));
        record->add(getSlot("Variable 3", _variables[2]));
    }
    return record;
}

void CLuTableTemplate::AddVariable(unsigned short index,Variable::Type type)
// ************************************************************************
{
    assert((index > 0) && (index < 4));
    if (_variables[index-1])
        LibertyParserError("variable already defined");
    
    _variables[index-1] = new Variable(type);
}

void CLuTableTemplate::SetVariableIndexes(unsigned short index,unsigned short count,double* values)
// ***********************************************************************************************
{
    assert((index > 0) && (index < 4));
    if (_variables[index-1] == NULL)
        LibertyParserError("variable not defined");

    _variables[index-1]->SetIndexes(count,values);
}

unsigned short CLuTableTemplate::getDimension() const
// *************************************************
{
    unsigned short dimension=0;
    while((dimension<3) && (_variables[dimension])) dimension++;
    return dimension;
}

int CLuTableTemplate::getTableSize() const
// ***************************************
{
    return getVariableIndexSize(1)*getVariableIndexSize(2)*getVariableIndexSize(3);
}

int CLuTableTemplate::getVariableIndexSize(unsigned short index) const
// ******************************************************************
{
    assert((index > 0) && (index < 4));
    if (_variables[index-1])
        return _variables[index-1]->getSize();
    else
        return 1;
}

vector<double> CLuTableTemplate::getVariableIndexes(unsigned short index) const
// ***************************************************************************
{
    assert((index > 0) && (index < 4));
    if (!_variables[index-1])
        throw Error("getVariableIndexes : variable "+getString(index)+" not defined");
    return _variables[index-1]->getIndexes();
}
double CLuTableTemplate::getVariableUnit(unsigned short index) const
// ****************************************************************
{
    assert((index > 0) && (index < 4));
    if (!_variables[index-1])
      throw Error("getVariableUnit : variable "+getString((unsigned int)index)+" not defined");
    return _technology->getUnit(_variables[index-1]->getType());
}
    

void CLuTableTemplate::FindIndexes(unsigned short index,double index_value,double& index_min,double& index_max
                                  ,unsigned short& index_index) const
// **********************************************************************************************************
{
    assert((index > 0) && (index < 4));
    _variables[index-1]->FindIndexes(index_value,index_min,index_max,index_index);
}

CLuTableTemplate* CLuTableTemplate::getClone() const
// *************************************************
{
    static unsigned int clone_count=0;
    string clone_name;
    
    do {
        clone_name= _name+"__crl_clone_"+getString(clone_count++);
    } while (_technology->getTemplate(clone_name));
    
    CLuTableTemplate* clone = create(clone_name,_technology);
    for (unsigned int i=0;i<3;i++)
        if (_variables[i])
            clone->_variables[i]=_variables[i]->getClone();
    return clone;
}

CLuTableTemplate* CLuTableTemplate::RemoveVariable(unsigned short index)
// ********************************************************************
{
    assert((index > 0) && (index < 4));

    CLuTableTemplate* clone = getClone();
    for(unsigned int i=index;i<3;i++)
        clone->_variables[i-1]=clone->_variables[i];
    clone->_variables[3]=NULL;

    return clone;
}

// **************************************************************************
// CLuTableTemplate::Variable definition
// **************************************************************************

CLuTableTemplate::Variable::Variable(Type type)
// ********************************************
    : _type(type)
    , _indexes()
{    
}

string CLuTableTemplate::Variable::_getString() const
// **************************************************
{
    string s="<CLuTableTemplate::Variable "+getString(_type)+" [";
    for (int i=0;i<getSize();i++)
        s+= (i?", ":"")+ getString(_indexes[i]);
    return s+"]>";
}

Record* CLuTableTemplate::Variable::_getRecord() const
// *********************************************
{
 	Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("Type",_type));
        record->add(getSlot("Indexes",&_indexes));
    }
    return record;
}

void CLuTableTemplate::Variable::SetIndexes(unsigned short count,double* values)
// *****************************************************************************
{
    if (!_indexes.empty())
        LibertyParserError("variable indexes already defined");

    _indexes.reserve(count);

    for(int i=0;i<count;i++) _indexes.push_back(values[i]);
    //while(count--) _indexes[count]=values[count]; //a priori ne met pas à jour size()... génant.
}

void CLuTableTemplate::Variable::FindIndexes(double index_value,double& index_min,double& index_max
                                            ,unsigned short& index_index) const
// ************************************************************************************************
{    
    index_index=0;
    while ( (index_index<(getSize()-1)) && (index_value > _indexes[++index_index]) );
    index_max=_indexes[index_index];
    index_min=_indexes[--index_index];
}

CLuTableTemplate::Variable* CLuTableTemplate::Variable::getClone() const
// *********************************************************************
{
    Variable* variable= new Variable(_type);
    variable->_indexes = _indexes;
    return variable;
}

// ****************************************************************************************************
// CLuTableTemplate::Variable::Type definition
// ****************************************************************************************************

CLuTableTemplate::Variable::Type::Type(const Code& code)
// *****************************************************
:	_code(code)
{
}

CLuTableTemplate::Variable::Type::Type(const CLuTableTemplate::Variable::Type& type)
// *********************************************************************************
:	_code(type._code)
{
}

CLuTableTemplate::Variable::Type& CLuTableTemplate::Variable::Type::operator=(const CLuTableTemplate::Variable::Type& type)
// ************************************************************************************************************************
{
	_code = type._code;
	return *this;
}

string CLuTableTemplate::Variable::Type::_getString() const
// ********************************************************
{
	switch (_code) {
		case UNDEFINED :     return "UNDEFINED";
        case CONSTRAINED_PIN_TRANSITION: return "CONSTRAINED_PIN_TRANSITION";
        case RELATED_PIN_TRANSITION: return "RELATED_PIN_TRANSITION";
        case INPUT_NET_TRANSITION:   return "INPUT_NET_TRANSITION";
        case TOTAL_OUTPUT_NET_CAPACITANCE:    return "TOTAL_OUTPUT_NET_CAPACITANCE";
	}
	return "ABNORMAL";
}

Record* CLuTableTemplate::Variable::Type::_getRecord() const
// ***************************************************
{
	Record* record = new Record(getString(this));
	record->add(getSlot("Code", (unsigned*)&_code));
	return record;
}

} //namespace CRL
