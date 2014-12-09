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

#include "mauka/Bin.h"
#include "mauka/Row.h"
#include "mauka/Surface.h"
#include "mauka/SimAnnealingPlacer.h"
#include "mauka/SubRow.h"

namespace Mauka {

using namespace std;
using Hurricane::Error;

SubRow::SubRow(Cell* cell, Surface* surface, const Box& box)
    :Inherit(cell, box)
    , _surface(surface)
    , _row(NULL)
    , _binVector()
    , _size(0)
    , _capa(0)
{}

SubRow* SubRow::create(Cell* cell, Surface* surface, const Box& box, bool orientation)
{
    SubRow* subRow = new SubRow(cell, surface, box);
    subRow->_subRowPostCreate(orientation);
    return subRow;
}

  void SubRow::_subRowPostCreate(bool orientation)
  {
    _row = _surface->InsertSubRowInRow(this, orientation); 

#if BINS_WITH_BALANCED_WIDTH
    unsigned nBins
      = nBins = getWidth() / _surface->_binWidthMax + ((getWidth() % _surface->_binWidthMax) ? 1 : 0);

    if ( nBins == 0 ) {
      assert (getWidth() >= _surface->_binWidthMin);
      nBins = 1;
    }

    DbU::Unit pitch     = _surface->_mauka->getPitch(); 
    DbU::Unit binsWidth = ((getWidth() / pitch) / nBins) * pitch;
    
    if ( getWidth() % _surface->_mauka->getPitch() )
      throw Error("Subrow::_postCreate(): SubRow width %s is not a multiple of pitch."
                 ,DbU::getValueString(getWidth()).c_str());

    if ( binsWidth % _surface->_mauka->getPitch() )
      throw Error("Subrow::_postCreate(): SubRow Bin width %s is not a multiple of pitch."
                 ,DbU::getValueString(binsWidth).c_str());

    DbU::Unit totalBinsWidth  = binsWidth * nBins;
    DbU::Unit binsWidthRemain = getWidth() - totalBinsWidth;
    
    if (binsWidthRemain % pitch)
      throw Error("Subrow::_postCreate(): SubRow Bin width remainder %s is not a multiple of pitch."
                 ,DbU::getValueString(binsWidthRemain).c_str());

    unsigned binsWidthRemainPitch = binsWidthRemain / pitch; 

    DbU::Unit xMin = getXMin();
    DbU::Unit xMax = xMin;
        
    for ( unsigned binId = 0; binId < nBins; ++binId ) {
      if ( binsWidthRemainPitch > 0 ) {
        xMax += binsWidth + pitch;
        --binsWidthRemainPitch;
      } else
        xMax += binsWidth;

      Bin* bin = Bin::create(getCell(), this, Box(xMin, getYMin(), xMax, getYMax()));
      _binVector.push_back(bin);
      _binXMax[bin->getXMax()] = _binVector.size() - 1;

      xMin = xMax;
    }
#else  // BINS_WITH_BALANCED_WIDTH
  // All bins are 2*biggest cell width, the lastest also include the remainder.

    unsigned nBins = getWidth() / _surface->_binWidthMax;
    if ( nBins == 0 ) {
      assert ( getWidth() >= _surface->_binWidthMin );
      nBins = 1;
    }

    DbU::Unit binsWidth       = _surface->_binWidthMax;
    DbU::Unit binsWidthRemain = getWidth() - nBins*binsWidth; // Could be negative.
    
    if ( getWidth() % _surface->_mauka->getPitch() )
      throw Error("Subrow::_postCreate(): SubRow width %s is not a multiple of pitch."
                 ,DbU::getValueString(getWidth()).c_str());

    if ( binsWidth % _surface->_mauka->getPitch() )
        throw Error("Subrow::_postCreate(): SubRow Bin width %s is not a multiple of pitch."
                   ,DbU::getValueString(binsWidth).c_str());

    DbU::Unit xMin = getXMin();
    DbU::Unit xMax = xMin;
        
    for ( unsigned int binId=0 ; binId < nBins ; ++binId ) {
      xMax += binsWidth;
      if ( binId == 0 ) xMax += binsWidthRemain;

      Bin* bin = Bin::create ( getCell(), this, Box(xMin,getYMin(),xMax,getYMax()) );
      _binVector.push_back(bin);
      _binXMax[bin->getXMax()] = _binVector.size() - 1;

      xMin = xMax;
    }
#endif  // BINS_WITH_BALANCED_WIDTH

    Inherit::_postCreate();
}

bool SubRow::_MergeBins()
{
    if (_binVector.size() > 1)
    {
        cerr << "merging " << this << endl;
        _size = 0;
        Bin* bin = Bin::create(getCell(), this, Box(getXMin(), getYMin(), getXMax(), getYMax()));
        for (BinVector::iterator bvit = _binVector.begin();
                bvit != _binVector.end();
                bvit++)
        {
            for (MaukaEngine::UList::const_iterator ulit = (*bvit)->_instanceOccurrenceIds.begin();
                    ulit != (*bvit)->_instanceOccurrenceIds.end();
                    ulit++)
            {
                _surface->_mauka->_simAnnealingPlacer->_instanceBins[*ulit] = NULL;
            }
            (*bvit)->destroy();
        }
        _binVector.clear();
        _binVector.push_back(bin);
        return true;
    }
    return false;
}

void SubRow::_computeCapacity(double margin)
{
    for (BinVector::const_iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
    {
        (*bvit)->_computeCapacity(margin);
        _capa += (*bvit)->_capa;
    }
}

void SubRow::_preDestroy()
{
    Inherit::_preDestroy();
    for (BinVector::iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
    {
        (*bvit)->destroy();
    }
}

void SubRow::DisplayBinHits() const
{
    for (BinVector::const_iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
    {
        (*bvit)->DisplayHits();
    }
}

Bin* SubRow::getBinBetween(DbU::Unit lowerX, DbU::Unit upperX, const Bin* srcbin)
{
    assert(lowerX <= upperX);
    assert(lowerX >= getXMin());
    assert(upperX <= getXMax());
    
    
    if (_binVector.size() == 1)
        return _binVector[0];
    
    DbU::Unit searchPosition = lowerX +
        DbU::lambda((int)(DbU::getLambda(upperX-lowerX) * rand() / (RAND_MAX + 1.0)));

    unsigned binId = _binXMax.upper_bound(searchPosition)->second;
    
    if (_binVector[binId] == srcbin)
    {
        if (binId == 0)
            return _binVector[1];
        if (binId == _binVector.size() - 1)
            return _binVector[_binVector.size() - 2];
        static bool altern = true;
        if (altern)
        {
            altern = false;
            return _binVector[binId + 1];
        }
        else
        {
            altern = true;
            return _binVector[binId - 1];
        }
    }

    return _binVector[binId];
}

void SubRow::_AddSize(DbU::Unit value)
{
    _size += value;
    _row->_AddSize(value);
}


double SubRow::getCost() const
{
    return abs(DbU::getLambda(_size - _capa));
}

double SubRow::getBinCost() const
{
    double binCost = 0.0;
    for (BinVector::const_iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
        binCost += (*bvit)->getCost();
    return binCost;
}

DbU::Unit SubRow::getBinsSize() const
{
    DbU::Unit totalBinsSize = 0;
    for (BinVector::const_iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
    {
        totalBinsSize += (*bvit)->getSize();
    }
    return totalBinsSize;
}

DbU::Unit SubRow::getBinsCapa() const
{
    DbU::Unit totalBinsCapa = 0;
    for (BinVector::const_iterator bvit = _binVector.begin();
            bvit != _binVector.end();
            bvit++)
    {
        totalBinsCapa += (*bvit)->getCapa();
    }
    return totalBinsCapa;
}

Cell* SubRow::getCell() const
{
    return _row->getCell();
}

}
