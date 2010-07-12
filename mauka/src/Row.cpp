
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
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
// Foundation, inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 19/07/2006
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 

#include "mauka/SubRow.h"
#include "mauka/Surface.h"
#include "mauka/Row.h"

namespace Mauka {

using std::cerr;
using std::endl;
using Hurricane::Error;

Row::Row(Cell* cell, Surface* surface, const Box& box, bool orientation)
    :Inherit(cell, box)
    , _surface(surface)
    , _subRowVector()
    , _subRowXMax()
    , _subRowXMinInv()
    , _orientation(orientation)
    , _size(0)
    , _capa(0)
{}

Row* Row::create(Cell* cell, Surface* surface, const Box& box, bool orientation)
{
    if (!surface)
        throw Error("Can't create Mauka::Row : empty surface");
    Row* row = new Row(cell, surface, box, orientation);
    row->_postCreate();

    return row;
}

void Row::_preDestroy()
{
    Inherit::_preDestroy();
    for (SubRowVector::iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        (*srit)->destroy();
    }
    _subRowVector.clear();
}

namespace {

  using namespace Hurricane;

    struct SubRowSortByX
    {
        bool operator()(const SubRow* subrow1, const SubRow* subrow2) const
        {
            return subrow1->getXMin() < subrow2->getXMin();
        }
    };
}

void Row::_InsertSubRow(SubRow* subrow)
{
    if (_subRowVector.size() == 0)
        _subRowVector.push_back(subrow);
    else
    {
        SubRowVector::iterator srvit = lower_bound(_subRowVector.begin(), _subRowVector.end(), subrow, SubRowSortByX());
        if (srvit == _subRowVector.end())
        {
            _subRowVector.push_back(subrow);
        }
        else if ((*srvit)->getXMin() == subrow->getXMin())
        {
            throw Error("Bug in SubRow insertion");
        }
        else
        {
            _subRowVector.insert(srvit, subrow);
        }
    }
    _box.merge(subrow->getBox());
}

void Row::_computeSubRows()
{
    _subRowXMax.clear();
    _subRowXMinInv.clear();
    for (unsigned i = 0; i < _subRowVector.size(); i++)
    {
        SubRow* subRow = _subRowVector[i];
        _subRowXMax[subRow->getXMax()] = i;
        _subRowXMinInv[subRow->getXMin()] = i;
    }
}

SubRow* Row::getSubRowBetween(DbU::Unit x1, DbU::Unit x2)
{
    assert(x1 <= x2);
    assert(x1 >= getXMin());
    assert(x2 <= getXMax());
    
    if (_subRowVector.size() == 1)
        return _subRowVector[0];
    
    SubRowXMax::iterator rinf = _subRowXMax.upper_bound(x1);
    SubRowXMax::iterator rsup = _subRowXMinInv.upper_bound(x2);
    
    
    unsigned randidx = rinf->second +
        (unsigned)((rsup->second - rinf->second + 1) * (rand() / (RAND_MAX+1.0)));
#if 0
    //cerr << x1 << endl;
    //cerr << x2 << endl;
    for (SubRowXMax::iterator srit = _subRowXMax.begin();
            srit != _subRowXMax.end();
            srit++)
    {
        cerr << srit->first << "," << srit->second << endl;
    }
    cerr << randidx << endl << endl;;
#endif

    return _subRowVector[randidx];
}

void Row::_AddSize(DbU::Unit value)
{
    _size += value;
}

double Row::getCost() const
{
  return abs(DbU::getLambda(_size - _capa));
}

double Row::getBinCost() const
{
    double binCost = 0.0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        binCost += (*srit)->getBinCost();
    }
    return binCost;
}

DbU::Unit Row::getBinsSize() const
{
    DbU::Unit totalBinsSize = 0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        totalBinsSize += (*srit)->getBinsSize();
    }
    return totalBinsSize;
}

DbU::Unit Row::getBinsCapa() const
{
    DbU::Unit totalBinsCapa = 0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        totalBinsCapa += (*srit)->getBinsCapa();
    }
    return totalBinsCapa;
}

DbU::Unit Row::getSubRowsWidth() const
{
    DbU::Unit subRowsWidth = 0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        subRowsWidth += (*srit)->getWidth();
    }
    return subRowsWidth;
}

DbU::Unit Row::getSubRowsCapa() const
{
    DbU::Unit totalSubRowsCapa = 0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        totalSubRowsCapa += (*srit)->getCapa();
    }
    return totalSubRowsCapa;
}

void Row::_computeCapacity(double margin)
{
    _capa = 0;
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        (*srit)->_computeCapacity(margin);
        _capa += (*srit)->_capa;
    }
}

void Row::DisplayBinHits() const
{
    for (SubRowVector::const_iterator srit = _subRowVector.begin();
            srit != _subRowVector.end();
            srit++)
    {
        (*srit)->DisplayBinHits();
    }
}

Cell* Row::getCell() const
{
    return _surface->getCell();
}

}
