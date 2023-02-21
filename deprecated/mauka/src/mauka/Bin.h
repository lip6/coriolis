
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
#ifndef __BIN_H
#define __BIN_H

#include "hurricane/Occurrences.h"
#include "mauka/Container.h"
#include "mauka/MaukaEngine.h"

namespace Mauka {

using Hurricane::DbU;
using Hurricane::Box;
using Hurricane::Cell;
using Hurricane::Occurrence;
using Hurricane::Occurrences;
    
class SubRow;
class SimAnnealingPlacer;
    
// ****************************************************************************************************
// Bin declaration
// ****************************************************************************************************

class Bin : public Container {
// *************************
    
// Friends
// *******
    friend class BBPlacer;
    friend class SubRow;

// Types
// *****
    public: typedef Container Inherit;

// Attributes
// **********
    private: static const Name   _goName;
    private: SubRow*             _subRow;
    private: MaukaEngine*        _mauka;
    private: MaukaEngine::UList  _instanceOccurrenceIds; 
    private: DbU::Unit           _size; // sum of the contained instances width
    private: DbU::Unit           _capa; // ideal occupation of the bin
    private: unsigned            _sourceHits;
    private: unsigned            _targetHits;

// Constructors
// ************
    protected: Bin(Cell* cell, SubRow* subrow, const Box& box);
    public: static Bin* create(Cell* cell, SubRow* subrow, const Box& box);

// Accessors
// *********
    public: static const Name& getStaticName();
    public: virtual const Name& getName() const;
    public: virtual Cell* getCell() const;
    public: DbU::Unit getCapaVsSize() const { return (_capa - _size);}
    public: DbU::Unit getSize() const { return _size; }
    public: DbU::Unit getCapa() const { return _capa; }
    public: SubRow* getSubRow() { return _subRow; }
    public: unsigned getFirstInstanceOccurrenceId() const { return _instanceOccurrenceIds.front(); }
    public: double getCost() const;
    public: unsigned getInstanceOccurrenceIdSize() const { return _instanceOccurrenceIds.size(); }
    public: const MaukaEngine::UList::const_iterator InstanceOccurrenceIdsBegin() const { return _instanceOccurrenceIds.begin(); }
    public: const MaukaEngine::UList::const_iterator InstanceOccurrenceIdsEnd() const { return _instanceOccurrenceIds.end(); }
    public: Occurrences getInstanceOccurrences() const;

// Updators
// ********
    public: void removeInstance(unsigned instanceid);
    public: void removeFrontInstance(unsigned instanceid);
    public: void removeBackInstance(unsigned instanceid);
    public: void addInstance(unsigned instanceid);
    private:void _AddSize(DbU::Unit value);

// Others
// ******
    private: void _computeCapacity(double margin);
    public: void DisplayHits() const;
    public: void incrementSourceHits();
    public: void incrementTargetHits();
    public: bool TryAddInstance(unsigned instanceid);
    public: bool UnderOccupied() const;
    protected: virtual void _postCreate();
    public: virtual std::string _getTypeName() const {return "Bin";};
    public: void PlotStats(std::ofstream& out) const;
    public: void Plot(std::ofstream& out) const;
};

}

#endif // __BIN_H
