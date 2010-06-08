
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
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 
#include "hurricane/Error.h"
#include "mauka/SubRow.h"
#include "mauka/Bin.h"
#include "mauka/Surface.h"
#include "mauka/SimAnnealingPlacer.h"

namespace Mauka {

using namespace std;
using Hurricane::Error;

const Name Bin::_goName = "Mauka::Bin";
    
Bin::Bin(Cell* cell, SubRow* subrow, const Box& box)
    : Inherit(cell, box)
    , _subRow(subrow)
    , _mauka(NULL)
    , _instanceOccurrenceIds()
    , _size(0)
    , _capa(0)
    , _sourceHits(0)
    , _targetHits(0)
{}

Bin* Bin::create(Cell* cell, SubRow* subrow, const Box& box)
{
    Bin* bin = new Bin(cell, subrow, box);
    bin->_postCreate();
    return bin;
}

void Bin::_postCreate()
{
    if (!_subRow)
        throw Error("Empty SubRow for Bin"); 
    _mauka = _subRow->getSurface()->getMauka();
    if (!_mauka)
        throw Error("Empty SimAnnealingPlacer for Bin"); 
    _capa = DbU::lambda(DbU::getLambda(getWidth()) * (1.0 - _subRow->getSurface()->getMargin()));
    Inherit::_postCreate();
}

void Bin::_computeCapacity(double margin)
{
    _capa = DbU::lambda(DbU::getLambda(getWidth()) * (1.0 - margin));
}

const Name& Bin::getStaticName ()
{
  return _goName;
}

const Name& Bin::getName () const
{
  return _goName;
}

Cell* Bin::getCell() const
{
    return _subRow->getCell();
}

void Bin::addInstance(unsigned instanceid)
{
    _instanceOccurrenceIds.push_back(instanceid);
    DbU::Unit insWidth = _mauka->_instanceWidths[instanceid];
    _AddSize(insWidth);
    _mauka->_simAnnealingPlacer->_instanceBins[instanceid] = this;
}

void Bin::_AddSize(DbU::Unit value)
{
    _size += value;
    _subRow->_AddSize(value);
}

void Bin::removeInstance(unsigned instanceid)
{
    _instanceOccurrenceIds.remove(instanceid);
    DbU::Unit insWidth = _mauka->_instanceWidths[instanceid];
    _AddSize(-insWidth);
    _mauka->_simAnnealingPlacer->_instanceBins[instanceid] = NULL;
}

void Bin::removeBackInstance(unsigned instanceid)
{
    assert(_instanceOccurrenceIds.back() == instanceid);
    _instanceOccurrenceIds.pop_back();
    DbU::Unit insWidth = _mauka->_instanceWidths[instanceid];
    _AddSize(-insWidth);
    _mauka->_simAnnealingPlacer->_instanceBins[instanceid] = NULL;
}

void Bin::removeFrontInstance(unsigned instanceid)
{
    assert(_instanceOccurrenceIds.front() == instanceid);
    _instanceOccurrenceIds.pop_front();
    DbU::Unit insWidth = _mauka->_instanceWidths[instanceid];
    _AddSize(-insWidth);
    _mauka->_simAnnealingPlacer->_instanceBins[instanceid] = NULL;
}

bool Bin::TryAddInstance(unsigned instanceid)
{
    DbU::Unit insWidth = _mauka->_instanceWidths[instanceid];
    if (getCapaVsSize() < insWidth)
        return false;
    addInstance(instanceid);
    return true;
}

bool Bin::UnderOccupied() const
{
    // On Considère pour l'instant qu'un Bin est sous-occupé
    // et donc qu'il est intéressant d'y déplacer une instance
    // si il a 20% de vide par rapport à son remplissage idéal

    return ((_instanceOccurrenceIds.size() == 0)
        || (_size <= (1.0 - _subRow->getSurface()->getMargin() - 0.2) * _capa));
}

double Bin::getCost() const
{
    return abs(DbU::getLambda(_size - _capa));
}

void Bin::DisplayHits() const
{
    cout << this << " nb source hits: " << _sourceHits << endl;
    cout << this << " nb target hits: " << _targetHits << endl;
}

void Bin::incrementSourceHits()
{
	 ++_sourceHits;
}

void Bin::incrementTargetHits()
{
	 ++_targetHits;
}

void Bin::PlotStats(ofstream& out) const
{
    unsigned x = (unsigned)(DbU::getLambda(getXMin())) + (unsigned)(DbU::getLambda(getXMax() - getXMin())) / 2;
    //unsigned decal = (unsigned)(DbU::getLambda(getYMax() - getYMin())) / 4;
    //unsigned y = (unsigned)(DbU::getLambda(getYMin()));
    unsigned y = (unsigned)(DbU::getLambda(getYMin())) + (unsigned)(DbU::getLambda(getYMax() - getYMin())) / 2;
    unsigned totalMoves = _mauka->_simAnnealingPlacer->getMoves();
    if (totalMoves)
    {
        //double sourcePercent = (_sourceHits * 100.0) / totalMoves;
        double targetPercent = (_targetHits * 100.0) / totalMoves;
        out << "set label \""  
            << targetPercent << "%\" at " << x << "," << y << " center"
            << endl;
        //out << "set label \""  
        //    << targetPercent << "%\" at " << x << "," << y + decal << " center"
        //    << endl;
    }
}

void Bin::Plot(ofstream& out) const
{
    out << DbU::getLambda(getXMin()) + 0.3 << " " << DbU::getLambda(getYMin()) + 0.3 << endl
        << DbU::getLambda(getXMin()) + 0.3 << " " << DbU::getLambda(getYMax()) - 0.3 << endl
        << DbU::getLambda(getXMax()) - 0.3 << " " << DbU::getLambda(getYMax()) - 0.3 << endl
        << DbU::getLambda(getXMax()) - 0.3 << " " << DbU::getLambda(getYMin()) + 0.3 << endl
        << DbU::getLambda(getXMin()) + 0.3 << " " << DbU::getLambda(getYMin()) + 0.3 << endl << endl;
}

}  // End of Mauka namespace.
