
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
#ifndef __ROW_H
#define __ROW_H

#include "mauka/Container.h"
#include "mauka/SubRow.h"

namespace Mauka {

using Hurricane::DbU;
using Hurricane::Box;

class Surface;
    
// ****************************************************************************************************
// Row declaration
// ****************************************************************************************************

class Row : public Container {
// *************************
    friend class Surface;
    friend class SubRow;
    friend class BBPlacer;

// Types
// *****
    public: typedef Container Inherit;
    public: typedef std::vector<SubRow*> SubRowVector;
    public: typedef std::map< DbU::Unit, unsigned, std::less<DbU::Unit> > SubRowXMax;
    public: typedef std::map< DbU::Unit, unsigned, std::greater<DbU::Unit> > SubRowXMinInv;
 
// Attributes
// **********
    private: Surface*           _surface;
    private: SubRowVector       _subRowVector;
    private: SubRowXMax         _subRowXMax;
    private: SubRowXMinInv      _subRowXMinInv;
    private: bool               _orientation;
    private: DbU::Unit          _size; // sum of the subrows size
    private: DbU::Unit          _capa;

// Constructors
// ************
    protected: Row(Cell* cell, Surface* surface, const Box& box, bool orientation);
    public: static Row* create(Cell* cell, Surface* surface, const Box& box, bool orientation);

// Accessors
// *********
    public: SubRow* getSubRowBetween(DbU::Unit x1, DbU::Unit x2);
    public: double getCost() const;
    public: bool getOrientation() const { return _orientation; }
    public: DbU::Unit getSubRowsWidth() const;
    public: DbU::Unit getSubRowsCapa() const;
    public: double getBinCost() const;
    public: DbU::Unit getBinsSize() const;
    public: DbU::Unit getBinsCapa() const;
            
// Updators
// ********
    private: void _computeCapacity(double margin);
    private: void _AddSize(DbU::Unit value);

// Others
// ******
    public: virtual Cell* getCell() const;
    protected: virtual void _preDestroy();
    private: void _InsertSubRow(SubRow* subrow);
    private: void _computeSubRows();
    public: virtual std::string _getTypeName() const {return "Row";};
    public: void DisplayBinHits() const; 
};

}

#endif // __ROW_H
