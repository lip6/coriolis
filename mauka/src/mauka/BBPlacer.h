
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
#ifndef __BBPLACER_H
#define __BBPLACER_H

#include "hurricane/Instance.h"
#include "mauka/SubRow.h"
#include "mauka/MaukaEngine.h"

namespace Mauka {

    using Hurricane::DbU;

// ****************************************************************************************************
// BBPlacer declaration
// ****************************************************************************************************

class BBPlacer {
// *************

    friend class MaukaEngine;
    
// Types
// *****
    public: typedef std::vector<SubRow*> SubRowVector;
// Attributes
// **********
    private: MaukaEngine*                     _mauka;
    private: SubRowVector                     _subRowVector;
    private: MaukaEngine::UTable              _subRowInstances;
    private: MaukaEngine::UnitVector          _instanceMarginWidth;
    private: MaukaEngine::UnitVector          _instanceX;           // Left Corner X position
    private: MaukaEngine::UnitVector          _instanceY;
    private: MaukaEngine::BVector             _instancePlaced;
    private: MaukaEngine::NetVector           _netVector;
    private: MaukaEngine::BBoxes              _netBBoxes;
    private: MaukaEngine::Costs               _netCosts;
    private: MaukaEngine::UVector             _netCurrCostMark;
    private: MaukaEngine::UVector             _netFlags;
    // For the optimization engine itself
    private: MaukaEngine::UVector             _instanceToOptimize; 
    private: DbU::Unit                        _leftEdge;
    private: DbU::Unit                        _rightEdge;
    private: double                           _oldCost;
    private: double                           _cost;
    private: double                           _bestCost;
    // _costFlag is for computing nets costs 
    // Each net must be visited only one time 
    private: unsigned                         _costFlag;
    private: unsigned                         _saveFlag;
    private: std::deque<unsigned>             _queue;    // ordering 
    private: MaukaEngine::UVector             _stack;
    private: MaukaEngine::UVector             _loopVect; // use for loop
    private: MaukaEngine::UnitVector          _bestSolution;
    private: int                              _idx;
            
// Constructors
// ************
    public: BBPlacer(MaukaEngine* mauka);

// Accessors
// *********
    public: DbU::Unit getInstanceIdX(unsigned id) const { return _instanceX[id]; }

// Updators
// ********

// Others
// ******

    public: void Run();
    public: void Save();
    private: bool Optimize();
    private: double initCost();
    private: double CurrentCost();
    private: void PlaceAll();
    private: void UnplaceAll();
    private: void addIns();
    private: void removeIns();
    private: double UpdateInstanceCost(unsigned instanceid);
    private: double UpdateNetCost(unsigned netid);
    private: double computeCost(const Box& box) const;
    private: double initInstanceCost(unsigned instanceid);
    private: double CurrentInstanceCost(unsigned instanceid);
    private: void computeNetBBox(unsigned netid);
    private: void SaveNetTempValue(unsigned netid);
    private: double CurrentNetCost(unsigned netid);
    private: void Plot(std::ofstream& out) const;
};

}

#endif // __BBPLACER_H
