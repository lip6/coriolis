
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
#ifndef __SURFACE_H
#define __SURFACE_H

#include "mauka/Container.h"
#include "mauka/MaukaEngine.h"

namespace Mauka {

using Hurricane::DbU;
using Hurricane::Box;
using Hurricane::Cell;

class Bin;
class Row;
class SubRow;
class MaukaEngine;
    
class Surface: public Container {
// ****************************
    friend class MaukaEngine;
    friend class SimAnnealingPlacer;
    friend class Row;
    friend class SubRow;
    friend class BBPlacer;

// Types
// *****
    public: typedef Container Inherit;
    public: typedef std::vector<Row*> RowVector;
    public: typedef std::list<SubRow*> SubRowList;
    public: typedef std::map<DbU::Unit, unsigned, std::less<DbU::Unit> > RowYMax;
    public: typedef std::map<DbU::Unit, unsigned, std::greater<DbU::Unit> > RowYMinInv;
    //public: typedef map<DbU::Unit, Row*> RowMap;
            
// Attributes
// **********
    private: MaukaEngine*       _mauka;
  //private: RowMap             _rowMap;
    private: RowVector          _rowVector;
    private: RowYMax            _rowYMax;
    private: RowYMinInv         _rowYMinInv;
    private: bool               _rowZeroOrientation;
    private: double             _margin;
    private: DbU::Unit          _binWidthMax;
    private: DbU::Unit          _binWidthMin;
    private: DbU::Unit          _searchWidth;
    private: DbU::Unit          _searchHeight;

// Constructors
// ************
    protected: Surface(MaukaEngine* mauka, const Box& placementbox);
    public: static Surface* create(MaukaEngine* mauka, const Box& placementbox);
               
// Accessors
// *********
    public: virtual Cell* getCell() const { return _mauka->getCell(); }
    public: Bin* getBinInSurface(Bin* srcbin, double dist);
    public: double getBinCost() const;
    public: double getRowCost() const;
    public: DbU::Unit getBinsSize() const;
    public: DbU::Unit getBinsCapa() const;
    public: DbU::Unit getSubRowsCapa() const;
    public: double getMargin() const { return _margin; };
    public: MaukaEngine* getMauka() { return _mauka; };

// Others
// ******
    public: void DisplayBinHits() const;
    protected: virtual void _preDestroy();
    private: void InsertRow(Row* row);
    private: void removeRow(Row* row);
    private: Row* InsertSubRow(SubRow* subrow, bool orientation);
    private: void removeSubRow(SubRow* subrow);
    private: void PlotBinsStats(std::ofstream& out) const;
    private: void Plot(std::ofstream& out) const;
    public: virtual std::string _getTypeName() const {return "Surface";};
    public: virtual Record* _getRecord() const;
    private: Row* InsertSubRowInRow(SubRow* subrow, bool orientation);
    private: void _computeRowsAndSubRows();
    private: void _DisplayInstances(MaukaEngine::UVector& instanceids, SubRowList& subrowlist);
    private: void _computeCapacity();
    private: void _postCreate();
};

}
#endif /* __SURFACE_H */
