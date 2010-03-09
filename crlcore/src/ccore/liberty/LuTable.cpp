
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
#include <sstream>


#include  "LibertyTechnology.h"
#include  "LuTableTemplate.h"

#include  "LuTable.h"

namespace {
    string getUnitString(double val) {
        if (val < 1e-12) return getString(val*1e15)+"f";
        else if (val < 1e-9) return getString(val*1e12)+"p";
        else if (val < 1e-6) return getString(val*1e9)+"n";
        else if (val < 1e-3) return getString(val*1e6)+"µ";
        else if (val < 1) return getString(val*1e3)+"m";
        else if (val < 1e3) return getString(val);
        else if (val < 1e6) return getString(val*1e3)+"k";
        else if (val < 1e9) return getString(val*1e6)+"M";
        else return getString(val*1e9)+"G";
    }
    
    template<class t> string getarraystring(int count,t* values) {
        ostringstream s;
        for (int i=0; i< count ; i++)
            s << (i?", ":"") << values[i];
        return s.str();
    }
    template<class t> string getarraystring(int count,vector<t> values) {
        ostringstream s;
        for (int i=0; i< count ; i++)
            s << (i?", ":"") << values[i];
        return s.str();
    }
    template<class t> string getarraystring(vector<t> values) {
        ostringstream s;
        for (int i=0; i< values.size() ; i++)
            s << (i?", ":"") << values[i];
        return s.str();
    }
}
namespace CRL {
    
CLuTable::CLuTable(CLuTableTemplate* luTemplate)
// *********************************************
    : Inherit()
    , _template(luTemplate)
    , _values()
{
    _values.reserve(_template->getTableSize());
}

CLuTable* CLuTable::create(CLuTableTemplate* luTemplate)
// *****************************************************
{
    if (!luTemplate)
        throw Error("Can't create "+ _TName("CLuTable") + " : null template");

    CLuTable* luTable = new CLuTable(luTemplate);

    luTable->_postCreate();

    return luTable;
}

string CLuTable::_getString() const
// ********************************
{
    string s= Inherit::_getString();
    s.insert(s.length()-1," "+_template->getName()+" [");
    for (size_t i=0;i<_values.size();i++) s.insert(s.length()-1,(i?", ":"")+getString(_values[i]));
    s.insert(s.length()-1,"]");
    return s;
}

Record* CLuTable::_getRecord() const
// ***************************
{
    Record* record=Inherit::_getRecord();
    if (record)
    {
        record->add(getSlot("Template",_template));
        record->add(getSlot("Values",&_values));
    }
    return record;
}

void CLuTable::AddValues(unsigned short count, double* values)
// ***********************************************************
{
    for(int i=0;i<count;i++)
        _values.push_back(values[i]);
}

unsigned short CLuTable::getDimension() const
// *****************************************
{
    return _template->getDimension();
}

double CLuTable::_getValue(unsigned short x,unsigned short y, unsigned short z) const
// **********************************************************************************
{
    unsigned value_index=z + _template->getVariableIndexSize(3)*y
                           + _template->getVariableIndexSize(3)*_template->getVariableIndexSize(2)*x;
    //ltrace(139) << "_getValue(" << x << ", " << y << ", " << z << ")"
    //            << " _values[" << value_index << "]=" << _values[value_index] << endl;
    return _values[value_index];
}

double CLuTable::getValue(double var1,double var2,double var3) const
// *****************************************************************
{
    double vars[3] = { var1,var2,var3 } ;
    //ltrace(141) << "getValue(" << var1 << ", " << var2 << ", " << var3 << ")" << endl;ltracein(141);

    unsigned short index;

    /*
     * coord is [ x0,x1,x2,y0,y1,y2,z0,z1,z2 ]
     *  x0,y0,z0 are the coordonates of requested value in the table
     *  x1,x2 (x1<x2) are the neerest indexes from x0 in the variable template indexes
     *  idem for y1,y2 and z1,z2
     */
    double coord[9];
    /*
     * table_indexes is [ i_x1, i_y1, i_z1 ]
     *  where i_*1 is the position of *i in its template variable index array.
     */
    unsigned short table_indexes[3] = { 0, 0, 0};
    
    for (index=0;index<_template->getDimension();index++)
    {
        /*
         * unapplying the unit modifier to match the unit used in the table
         */

        double value = vars[index] / _template->getVariableUnit(index+1);
        coord[index*3] = value;
        /*
         * finding the neerest indexes in the template
         */
        _template->FindIndexes(index+1,value,coord[index*3+1],coord[index*3+2],table_indexes[index]);
    }
    

    /*
     * at this point : index = the number of dimension of the table
     */

    /*
     * Filling values such as :
     *   values[0]=_getValues(x1,y1,z1)
     *   values[1]=_getValues(x2,y1,z1)
     *   values[2]=_getValues(x1,y2,z1) (if 2 dimension or more)
     *   values[3]=_getValues(x2,y2,z1)  "
     *   values[4]=_getValues(x1,y1,z2) (if 3 dimension)
     *   values[5]=_getValues(x2,y1,z2)  "
     *   values[6]=_getValues(x1,y2,z2)  "
     *   values[7]=_getValues(x2,y2,z2)  "
     */
    double values[8];
    for (unsigned short i=0;i<(1<<index);i++)
        values[i]=_getValue(table_indexes[0]+(i%2)
                           ,table_indexes[1]+((i>>1)%2)
                           ,table_indexes[2]+((i>>2)%2)
                           );

    //ltrace(140) << "  table_indexes : " << getarraystring(_template->getDimension(),table_indexes) << endl;
    //ltrace(140) << "  coord : " << getarraystring(3*_template->getDimension(),coord) << endl;
    //ltrace(140) << "  values (init) : " << getarraystring(1<<_template->getDimension(),values) << endl;


    /*
     * Pour chaque dimension, on effectue une coupe de notre espace en x0, y0 ou z0, en calculant les valeurs
     * projetée sur la coupe, les autres valeurs restants constantes.
     * Par exemple pour x on trouve 'value(x0,y*,z*)' à partir de 'value(x1,y*,z*)' et 'value(x2,y*,z*)' :
     *
     *                            value(x2,..)-value(x1,..)
     * value(x0,..)=value(x1,..)+ ------------------------- (x1-x)
     *                                      x2-x1
     *
     * Au premier tour on a   values = [ v(x1,y1,z1), v(x2,y1,z1), v(x1,y2,z1), v(x2,y2,z1),
     *                                   v(x1,y1,z2), v(x2,y1,z2), v(x1,y2,z2), v(x2,y2,z2) ]
     * Au deusieme tour on a  values = [ v(x1,y1,z0), v(x2,y1,z0), v(x1,y2,z0), v(x2,y2,z0) ]
     * Au troisieme tour on a values = [ v(x1,y0,z0), v(x2,y0,z0) ]
     * Au dernier tour on a   values = [ v(x0,y0,z0) ]
     *
     * Si la table fait moins de 3 dimensions, on saute les premiers tours.
     */
    while(index--) {

        for (unsigned short i=0; i< (1<<index); i++ )
            values[i]=values[i]+(values[i+(1<<index)]-values[i])/(coord[index*3+2]-coord[index*3+1])*(coord[index*3]-coord[index*3+1]);
  //FAUX    values[i]=values[2*i]+(values[2*i+1]-values[2*i])/(coord[index*3+2]-coord[index*3+1])*(coord[index*3]-coord[index*3+1]);
        //ltrace(140) << "  values (" << (int)index <<") : " << getarraystring(1<<index,values) << endl;
    }
    
    //ltrace(141) << " = " << values[0] * _template->getTechnology()->getUnit(CLibertyTechnology::Unit::TIME) << endl;
    //ltraceout(141);
    //FIXME [HARDCODED] I assume all tables contain Time values
    return values[0] * _template->getTechnology()->getUnit(CLibertyTechnology::Unit::TIME);
}

CLuTable* CLuTable::getCut(unsigned short var, double value)
// ***********************************************************
{
    //ltrace(143) << "getting Cut of " << this << " for variable " << (int)var << " at value " << value << endl;ltracein(143);
    //ltrace(142) << "Original template : " << _template << endl;
    //for (unsigned short ii=1;ii <= _template->getDimension();ii++)
    //    ltrace(142) << "  " << (int)ii << " : " << getarraystring(_template->getVariableIndexes(ii)) << endl;
    
    
    CLuTableTemplate* newtemplate = _template->RemoveVariable(var);
    //ltrace(142) << "New template : " << newtemplate << endl;
    //for (unsigned short ii=1;ii <= newtemplate->getDimension();ii++)
    //    ltrace(142) << "  " << (int)ii << " : " << getarraystring(newtemplate->getVariableIndexes(ii)) << endl;

    CLuTable* newtable = create(newtemplate);

    vector<double> newindexes[3];

    unsigned short index;
    for (index=0;index<_template->getDimension();index++)
    {
        if (index == var-1)
            newindexes[index] = vector<double>(1,value);
        else
        {
            newindexes[index] = _template->getVariableIndexes(index+1);
            for (unsigned short i=0;i<newindexes[index].size();i++)
                newindexes[index][i]*=_template->getVariableUnit(index+1);
        }
    }
    for (;index<3;index++)
        newindexes[index] = vector<double>(1,0.0);

    //ltrace(142) << "New indexes : " << endl;
    //for (unsigned short ii=1;ii<=3;ii++)
    //    ltrace(142) << "  " << (int)ii << " : " << getarraystring(newindexes[ii-1]) << endl;

    for (unsigned short x=0;x < newindexes[0].size();x++)
        for (unsigned short y=0;y < newindexes[1].size();y++)
            for (unsigned short z=0;z < newindexes[2].size();z++)
                newtable->_values.push_back(getValue(newindexes[0][x],newindexes[1][y],newindexes[2][z])/_template->getTechnology()->getUnit(CLibertyTechnology::Unit::TIME));

    //ltrace(142) << "New Table : " << newtable << endl;
    //ltraceout(143);
    return newtable;
}

double CLuTable::Linearize()
// *************************
{
    //ltrace(143) << "Linearizing " << this <<  endl;ltracein(143);

    if (getDimension() != 1)
        throw Error("LuTable must have exactly one dimension to by linearized");

    vector<double> indexes = _template->getVariableIndexes(1);
    ///*dbg*/for (unsigned short i=1;i<_template->getVariableIndexSize(1);i++) {
    ///*dbg*/    double b=_getValue(i);double a=_getValue(i-1);
    ///*dbg*/    ltrace(141) << " . (" << b << "-" << a << ")/(" << indexes[i] << "-" << indexes[i-1] << ") =" << (b-a)/(indexes[i]-indexes[i-1]) << endl;
    ///*dbg*/}
    ///*dbg*/double imax=_getValue(indexes.size()-1);double i0=_getValue(0);
    ///*dbg*/ltrace(142) << " = " << (imax-i0)/(indexes[indexes.size()-1]-indexes[0]) << endl;
    //ltraceout(143);

    return (_getValue(indexes.size()-1)-_getValue(0))/(indexes[indexes.size()-1]-indexes[0]);
}


} //namespace CRL
