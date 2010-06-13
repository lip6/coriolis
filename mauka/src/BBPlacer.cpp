
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
// inspired by Andrew Caldwell's BranchSmallPlacer  
// Authors-Tag 

/**************************************************************************
***    
*** Copyright (c) 1995-2000 Regents of the University of California,
***               Andrew E. Caldwell, Andrew B. Kahng and Igor L. Markov
*** Copyright (c) 2000-2002 Regents of the University of Michigan,
***               Saurabh N. Adya and Igor L. Markov
***
***  Contact author(s): abk@cs.ucsd.edu, imarkov@umich.edu
***  Original Affiliation:   UCLA, computer Science Department,
***                          Los Angeles, CA 90095-1596 USA
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/

#include "hurricane/UpdateSession.h"
#include "hurricane/Net.h"
#include "hurricane/Cell.h"
#include "crlcore/ToolBox.h"

#include "mauka/MaukaEngine.h"
#include "mauka/Surface.h"
#include "mauka/Row.h"
#include "mauka/SubRow.h"
#include "mauka/Bin.h"
#include "mauka/BBPlacer.h"

namespace {

using namespace Mauka;

class CompareInstancePosition
{
    private: BBPlacer* _bbPlacer;
             
    public: CompareInstancePosition(BBPlacer* bbplacer)
            : _bbPlacer(bbplacer)
            {}
    public: bool operator()(unsigned id1, unsigned id2) const
            {
                return _bbPlacer->getInstanceIdX(id1) < _bbPlacer->getInstanceIdX(id2);
            }
};

} // End of anonymous namespace.

namespace Mauka {

using namespace std;
using namespace Hurricane;
using namespace CRL;

//#define PLACE_DEBUG 1

BBPlacer::BBPlacer(MaukaEngine* mauka)
    : _mauka(mauka)
    , _subRowVector()
    , _subRowInstances()
    , _instanceX()
    , _instanceY()
    , _instancePlaced()
    , _netVector()
    , _netBBoxes()
    , _netCosts()
    , _netCurrCostMark()
    , _netFlags()
    , _instanceToOptimize()
    , _leftEdge(0)
    , _rightEdge(0)
    , _oldCost(0.0)
    , _cost(0.0)
    , _bestCost(0.0)
    , _costFlag(0)
    , _queue()
    , _stack()
    , _loopVect()
    , _bestSolution()
    , _idx(0)
{
    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        _instanceMarginWidth.push_back(_mauka->_instanceWidths[i]);
        _instanceX.push_back(0);
        _instanceY.push_back(0);
        _instancePlaced.push_back(true);
    }

    for (unsigned netid = 0; netid < _mauka->_nets.size(); netid++)
    {
        vector<Box> bboxes(2);
        _netBBoxes.push_back(bboxes);
        vector<double> costs(2);
        _netCosts.push_back(costs);
        _netFlags.push_back(0);
        _netCurrCostMark.push_back(0);
    }


    Surface::RowVector& rowVector = _mauka->_surface->_rowVector;

    for (Surface::RowVector::iterator rvit = rowVector.begin();
            rvit != rowVector.end();
            rvit++)
    {
        Row::SubRowVector& subRowVector = (*rvit)->_subRowVector;
        for (Row::SubRowVector::iterator srvit = subRowVector.begin();
                srvit != subRowVector.end();
                srvit++)
        {
            SubRow* subRow = *srvit;
            if (subRow->getSize() > 0)
            {
                _subRowVector.push_back(subRow);
                _subRowInstances.push_back(MaukaEngine::UVector());
                DbU::Unit totalInstanceSizeInRow = 0;
                for (SubRow::BinVector::iterator bvit = subRow->_binVector.begin();
                        bvit != subRow->_binVector.end();
                        bvit++)
                {
                    Bin* bin = *bvit;
                    totalInstanceSizeInRow += bin->getSize();
                    for (MaukaEngine::UList::const_iterator ulit = bin->InstanceOccurrenceIdsBegin();
                            ulit != bin->InstanceOccurrenceIdsEnd();
                            ulit++)
                    {
                        unsigned instanceId = *ulit;
                        _subRowInstances.back().push_back(instanceId);
                    }
                }
                
                DbU::Unit pitch = _mauka->getPitch(); 
                DbU::Unit whiteSpace = subRow->getWidth() - totalInstanceSizeInRow;
                DbU::Unit instanceWhiteSpacePitch = (whiteSpace / _subRowInstances.back().size()) / pitch;
                DbU::Unit whiteSpaceRemain = whiteSpace
                    - (instanceWhiteSpacePitch * _subRowInstances.back().size() * pitch);
                assert(!(whiteSpaceRemain%pitch));
                
                if (instanceWhiteSpacePitch > 0)
                {
                    for (MaukaEngine::UVector::const_iterator uvit = _subRowInstances.back().begin();
                            uvit != _subRowInstances.back().end();
                            uvit++)
                    {
                        _instanceMarginWidth[*uvit] += instanceWhiteSpacePitch * pitch;
                    }
                }
                
                MaukaEngine::UVector::const_iterator uvit = _subRowInstances.back().begin();
                while (whiteSpaceRemain > 0)
                {
                    _instanceMarginWidth[*uvit++] += pitch;
                    whiteSpaceRemain -= pitch;
                    assert(uvit != _subRowInstances.back().end());
                }
    
                DbU::Unit xPos = subRow->getXMin();
                DbU::Unit yPos = subRow->getYMin();
                for (MaukaEngine::UVector::const_iterator uvit = _subRowInstances.back().begin();
                        uvit != _subRowInstances.back().end();
                        uvit++)
                {
                    unsigned instanceId = *uvit;
                    _instanceX[instanceId] = xPos;
                    _instanceY[instanceId] = yPos;
                    xPos += _instanceMarginWidth[instanceId];
                }
            }
        }
    }
}

void BBPlacer::Save()
{
    UpdateSession::open();
    for (unsigned i = 0; i < _subRowInstances.size(); i++)
    {
        SubRow* subRow = _subRowVector[i];
        bool rowOrientation = subRow->getRow()->getOrientation();

        for (unsigned j = 0; j < _subRowInstances[i].size(); j++)
        {
            unsigned instanceId = _subRowInstances[i][j];
            DbU::Unit x = _instanceX[instanceId];
            DbU::Unit y = _instanceY[instanceId];
            Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[instanceId];
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            Transformation::Orientation orientation;
            if (rowOrientation)
                orientation = Transformation::Orientation::ID;
            else
                orientation = Transformation::Orientation::MY;
            Box masterABox = instance->getMasterCell()->getAbutmentBox();
            Transformation instanceTransformation = getTransformation(masterABox
                    , x
                    , y
                    , orientation);
#if 0
            cerr << masterABox.getXMin() << "," << masterABox.getYMin()
                << masterABox.getXMax() << "," << masterABox.getYMax() << endl;
            cerr << x << "," << y << endl;
            cerr << instanceTransformation << endl;
            cerr << "occ transfo : " << instanceOccurrence.getPath().getTransformation() << endl;
#endif
            instanceOccurrence.getPath().getTransformation().invert().applyOn(instanceTransformation);
            instance->setTransformation(instanceTransformation);
            instance->setPlacementStatus(Instance::PlacementStatus::PLACED);
            //setPlacementStatusRecursivelyToPlaced(instance);
        }
        
    }
    UpdateSession::close();
}

void BBPlacer::Plot(ofstream& out) const
{
    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        DbU::Unit x = _instanceX[i];
        DbU::Unit y = _instanceY[i];
        Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[i];
        Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
        Box masterABox = instance->getMasterCell()->getAbutmentBox();
        Box instanceBox = Box(
                x,
                y,
                x + masterABox.getWidth(),
                y + masterABox.getHeight());
        out << instanceBox.getXMin()+0.4 << " " << instanceBox.getYMin()+0.4 << endl
            << instanceBox.getXMin()+0.4 << " " << instanceBox.getYMax()-0.4 << endl
            << instanceBox.getXMax()-0.4 << " " << instanceBox.getYMax()-0.4 << endl
            << instanceBox.getXMax()-0.4 << " " << instanceBox.getYMin()+0.4 << endl
            << instanceBox.getXMin()+0.4 << " " << instanceBox.getYMin()+0.4 << endl << endl;
    }
    
    out << "EOF" << endl << endl;

    out << "#nets" << endl;
    for (unsigned i = 0; i < _mauka->_netInstances.size(); i++)
    {
        if (_mauka->_hasInitX[i])
            continue;
        unsigned nbInstances = 0;
        DbU::Unit baryX = 0;
        DbU::Unit baryY = 0;
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[instanceId];
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            Box masterABox = instance->getMasterCell()->getAbutmentBox();
            ++nbInstances;
            baryX += _instanceX[instanceId] + masterABox.getWidth() / 2;
            baryY += _instanceY[instanceId] + masterABox.getHeight() / 2;
        }
        
        baryX = baryX / nbInstances;
        baryY = baryY / nbInstances;
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[instanceId];
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            Box masterABox = instance->getMasterCell()->getAbutmentBox();
            out << baryX << " " << baryY << endl
                << _instanceX[instanceId] + masterABox.getWidth() / 2 << " "
                << _instanceY[instanceId] + masterABox.getHeight() / 2 << endl << endl;
        }
    }
    out << "EOF" << endl << endl;
    
    out << "#nets with fixed point" << endl;
    for (unsigned i = 0; i < _mauka->_netInstances.size(); i++)
    {
        if (!_mauka->_hasInitX[i])
            continue;
        unsigned nbInstances = 1;
        DbU::Unit baryX = 0;
        DbU::Unit baryY = 0;
        baryX += _mauka->_netInitX[i];
        baryY += _mauka->_netInitY[i];
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[instanceId];
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            Box masterABox = instance->getMasterCell()->getAbutmentBox();
            ++nbInstances;
            baryX += _instanceX[instanceId] + masterABox.getWidth() / 2;
            baryY += _instanceY[instanceId] + masterABox.getHeight() / 2;
        }
        
        baryX = baryX / nbInstances;
        baryY = baryY / nbInstances;
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            Occurrence instanceOccurrence = _mauka->_instanceOccurrencesVector[instanceId];
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            Box masterABox = instance->getMasterCell()->getAbutmentBox();
            out << baryX << " " << baryY << endl
                << _instanceX[instanceId] + masterABox.getWidth() / 2 << " "
                << _instanceY[instanceId] + masterABox.getHeight() / 2 << endl << endl;
        }
        out << baryX << " " << baryY << endl
            << _mauka->_netInitX[i] << " "
            << _mauka->_netInitY[i] << endl << endl;
    }
    out << "EOF" << endl << endl;
}

void BBPlacer::Run()
{
    for (unsigned i = 0; i < _subRowInstances.size(); i++)
    {
        unsigned decal = 2;
        unsigned nInstancesToOptimize = 0;
        MaukaEngine::UVector::iterator ifirst = _subRowInstances[i].begin();
        MaukaEngine::UVector::iterator ilast = _subRowInstances[i].end();
        while (1)
        {
            _instanceToOptimize.clear();
            while (1)
            {
                if (ifirst == ilast)
                    break;
                _instanceToOptimize.push_back(*ifirst++);
                ++nInstancesToOptimize;
                if (nInstancesToOptimize >= 6)
                {
                    nInstancesToOptimize = 0;
                    break;
                }
            }
            Optimize();

            if (ifirst == ilast)
                break;

            sort(_subRowInstances[i].begin(), _subRowInstances[i].end(), CompareInstancePosition(this));

            if ((6 + decal) <= nInstancesToOptimize)
            {
                ifirst = _subRowInstances[i].begin() + decal;
            }
            else
            {
                ifirst = _subRowInstances[i].begin() + decal - 1;
            }
            
            decal += 2;
        }
    }
}

bool BBPlacer::Optimize()
{
    bool optimizationResult = false;
    const int numberInstances = _instanceToOptimize.size();

    // Save initial Solution
    _bestSolution.reserve(numberInstances);

    for (unsigned i=0; i<_instanceToOptimize.size(); i++)
    {
        _queue.push_back(_instanceToOptimize[i]);
    }

    MaukaEngine::UVector::iterator ifirst = _instanceToOptimize.begin();
    MaukaEngine::UVector::iterator ilast = _instanceToOptimize.end();
    vector<DbU::Unit>::iterator it = _bestSolution.begin();
    while (ifirst != ilast)
        *it++ = _instanceX[*ifirst++];
    
    // init Best Cost
    _bestCost = initCost();
#ifdef PLACE_DEBUG
    cout<<" Orig Cost: "<< _bestCost << endl;
    cout<<" Actual Cost: " << CurrentCost() << endl;
#endif

    // init initial Cost
    // init Edges
    _leftEdge = _instanceX[*_instanceToOptimize.begin()];
    _rightEdge = _instanceX[*_instanceToOptimize.rbegin()]
        + _instanceMarginWidth[*_instanceToOptimize.rbegin()];

    UnplaceAll();
    _cost = initCost();
    

#ifdef PLACE_DEBUG
    cout << " init Cost: " << _cost << endl;
#endif
    
    _loopVect.reserve(numberInstances + 2);
    for (int id = 0; id < numberInstances + 2; id++)
	_loopVect.push_back(0);
    _loopVect[numberInstances] = numberInstances;
    _loopVect[numberInstances + 1] = numberInstances + 1;

    _stack.reserve(numberInstances);

    _idx = numberInstances - 1;

    unsigned numAdds = 0;


    while(_idx < numberInstances)
    {
	addIns();
	numAdds++;

        if(_loopVect[_idx] == 0 || _cost >= _bestCost)
      	{

	    _loopVect[_idx] = 0;	//force a bound

	    if(_cost < _bestCost) //got here if:
				// new best complete soln (curWL < best)
				// bounded partial soln (curWL > best)
				//  so there is no need to additionally
				//  check to ensure this is a complete soln
	    {
		optimizationResult = true;
		_bestCost = _cost;
#ifdef PLACE_DEBUG
		cout<<" New Best: "<< _cost <<" found after "<< numAdds << endl;
		cout << "Cost recalculated: " << initCost() << endl;
		cout << "Actual Cost: " << CurrentCost() << endl;
#endif	

		ifirst = _instanceToOptimize.begin();
		it = _bestSolution.begin();
		while (ifirst != ilast)
		{
		    *it++ = _instanceX[*ifirst++];
		}
#if 0
                Save();
                for_each_view(view, _mauka->getCell()->getViews())
                {
                    if (CEditor* editor = dynamic_cast<CEditor*>(view))
                    {
                        editor->Stop("coucou");
                        break;
                    }
                    end_for;
                }
#endif
	    }

            while(_loopVect[_idx] == 0)
            {
	   	if(_idx < numberInstances)
		    removeIns();
    		_loopVect[++_idx]--; 
            }
      	}
      	--_idx;
    }
    ifirst = _instanceToOptimize.begin();
    it = _bestSolution.begin();
    while (ifirst != ilast)
        _instanceX[*ifirst++] = *it++;

    PlaceAll();
    _bestSolution.clear();
#ifdef PLACE_DEBUG
    cout<<" Total add Operations: "<< numAdds<< endl;
    cout<<" Final solution has cost: "<< _bestCost << endl << endl;
#endif
    _loopVect.clear();
    _queue.clear();
    _stack.clear();
    return optimizationResult;
}

void BBPlacer::addIns()
{
    _stack.push_back(_queue.front());
    _queue.pop_front();
    _loopVect[_idx] = _idx;

    unsigned instanceId = _stack.back();
    //cerr << "adding: "  << _instanceOccurrenceVector[instanceId] << " " ;

    if(_idx%2)
    {
        _instanceX[instanceId] = _rightEdge - _instanceMarginWidth[instanceId];
	_rightEdge -= _instanceMarginWidth[instanceId];
    }
    else
    {
        _instanceX[instanceId] = _leftEdge;
	_leftEdge += _instanceMarginWidth[instanceId];
    }
    //cerr << _instanceX[instanceId] << " ";
    _instancePlaced[instanceId] = true;
    _cost += UpdateInstanceCost(instanceId);
    //cerr << _cost << endl;
    //cerr << "verify " <<  CurrentCost() << endl;
}

void BBPlacer::removeIns()
{
    unsigned instanceId = _stack.back();
    //cerr << "Removing: "  << _instanceOccurrenceVector[instanceId] << " " ;
    
    _queue.push_back(instanceId);
    _stack.pop_back();
    if(_idx%2)
	_rightEdge += _instanceMarginWidth[instanceId]; 
    else
	_leftEdge -= _instanceMarginWidth[instanceId];
    _instancePlaced[instanceId] = false;
    _cost += UpdateInstanceCost(instanceId);
    //cerr << _cost << endl;
}

void BBPlacer::PlaceAll()
{
    for (MaukaEngine::UVector::iterator uvit = _instanceToOptimize.begin();
            uvit != _instanceToOptimize.end();
            uvit++)
    {
        _instancePlaced[*uvit] = true;
    }
}

void BBPlacer::UnplaceAll()
{
    for (MaukaEngine::UVector::iterator uvit = _instanceToOptimize.begin();
            uvit != _instanceToOptimize.end();
            uvit++)
    {
        _instancePlaced[*uvit] = false;
    }
}

double BBPlacer::initCost()
{
    ++_costFlag;
    double cost = 0.0;
    for (MaukaEngine::UVector::iterator uvit = _instanceToOptimize.begin();
            uvit != _instanceToOptimize.end();
            uvit++)
    {
        cost += initInstanceCost(*uvit);
    }
    return cost;
}

double BBPlacer::CurrentCost()
{
    ++_costFlag;
    double cost = 0.0;
    for (MaukaEngine::UVector::iterator uvit = _instanceToOptimize.begin();
            uvit != _instanceToOptimize.end();
            uvit++)
        cost += CurrentInstanceCost(*uvit);
    return cost;
}

double BBPlacer::UpdateInstanceCost(unsigned instanceid)
{
    double deltaCost = 0.0;
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_instanceNets[instanceid].begin();
            uvit != _mauka->_instanceNets[instanceid].end();
            uvit++)
    {
        deltaCost += UpdateNetCost(*uvit);
    }
    return deltaCost;
}

double BBPlacer::UpdateNetCost(unsigned netid)
{
    Box& netBBox = _netBBoxes[netid][_netFlags[netid]];
    double& netCost = _netCosts[netid][_netFlags[netid]];

    double initCost = netCost;
    netBBox.makeEmpty();
    if (_mauka->_hasInitX[netid])
        netBBox.merge(_mauka->_netInitX[netid], netBBox.getYMin());
    if (_mauka->_hasInitY[netid])
        netBBox.merge(netBBox.getXMin(), _mauka->_netInitY[netid]);
    
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netid].begin();
            uvit != _mauka->_netInstances[netid].end();
            uvit++)
    {
        if (_instancePlaced[*uvit])
        {
            //FIXME
            netBBox.merge(Point(_instanceX[*uvit], _instanceY[*uvit]));
        }
    }
    netCost = computeCost(netBBox);
    return (netCost - initCost);
}

double BBPlacer::initInstanceCost(unsigned instanceid)
{
    double cost = 0.0;
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_instanceNets[instanceid].begin();
            uvit != _mauka->_instanceNets[instanceid].end();
            uvit++)
    {
        computeNetBBox(*uvit);
        //SaveNetTempValue(*uvit);
	cost += CurrentNetCost(*uvit);
    }
    return cost;
}

double BBPlacer::CurrentInstanceCost(unsigned instanceid)
{
    double cost = 0.0;
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_instanceNets[instanceid].begin();
            uvit != _mauka->_instanceNets[instanceid].end();
            uvit++)
    {
        cost += CurrentNetCost(*uvit);
    }
    return cost;
}

void BBPlacer::SaveNetTempValue(unsigned netid)
{
    _netFlags[netid] = !_netFlags[netid];
}

void BBPlacer::computeNetBBox(unsigned netid)
{
    //cerr << _netVector[netid] << endl;
    //Box& netTmpBBox = _netBBoxes[netid][!_netFlags[netid]];
    Box& netTmpBBox = _netBBoxes[netid][_netFlags[netid]];
    //FIXME
    netTmpBBox.makeEmpty();
    if (_mauka->_hasInitX[netid])
        netTmpBBox.merge(_mauka->_netInitX[netid], netTmpBBox.getYMin());
    if (_mauka->_hasInitY[netid])
        netTmpBBox.merge(netTmpBBox.getXMin(), _mauka->_netInitY[netid]);
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netid].begin();
            uvit != _mauka->_netInstances[netid].end();
            uvit++)
    {
        if (_instancePlaced[*uvit])
        {
            //FIXME
            netTmpBBox.merge(Point(_instanceX[*uvit], _instanceY[*uvit]));
            //cerr << netTmpBBox << endl;
        }
    }
    _netCosts[netid][_netFlags[netid]] = computeCost(netTmpBBox);
    //_netCosts[netid][!_netFlags[netid]] = computeCost(netTmpBBox);
    //cerr << _netCosts[netid][!_netFlags[netid]] << endl;
}

double BBPlacer::computeCost(const Box& box) const
{
    if (!box.isEmpty())
      return DbU::getLambda(box.getYMax() - box.getYMin() + box.getXMax() - box.getXMin());
    else
      return 0.0;
}

double BBPlacer::CurrentNetCost(unsigned netid)
{
    if (_netCurrCostMark[netid] != _costFlag)
    {
        _netCurrCostMark[netid] = _costFlag;
        return _netCosts[netid][_netFlags[netid]];
    }
    return 0;
}


}
