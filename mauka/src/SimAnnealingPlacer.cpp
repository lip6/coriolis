
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

#include <cmath>

#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"

#include "mauka/Surface.h"
#include "mauka/Bin.h"
#include "mauka/Row.h"
#include "mauka/MaukaEngine.h"
#include "mauka/Move.h"
#include "mauka/SimAnnealingPlacer.h"

namespace {

using Hurricane::Error;
using Hurricane::Warning;
using Hurricane::Transformation;
using Hurricane::UpdateSession;
using CRL::getTransformation;
    
double computeStdDev(double sumcost, double sumsquare, unsigned accepted)
{
    if (accepted <= 1)
	return 0.0;
    
    double stdDev = (sumsquare - sumcost * sumcost / (double)accepted) / ((double)accepted - 1.0);
    if (stdDev > 0.0)
	stdDev = sqrt(stdDev);
    else
	stdDev = 0.0;

    return stdDev;
}

}

namespace Mauka {

using namespace std;
    
SimAnnealingPlacer::SimAnnealingPlacer(MaukaEngine* mauka)
// *******************************************************
    : _mauka(mauka)
    , _instanceBins()
    , _netBBoxes()
    , _netCosts()
    , _netFlags()
    , _netCost(0.0)
    , _binCost(0.0)
    , _rowCost(0.0)
    , _initNetCost(0.0)
    , _initBinCost(0.0)
    , _initRowCost(0.0)
    , _netMult(_mauka->getAnnealingNetMult())
    , _binMult(_mauka->getAnnealingBinMult())
    , _rowMult(_mauka->getAnnealingRowMult())
    , _temperature(0.0)
    , _distance(0.0)
    , _loop(0)
    , _iterationsFactor(0)
    , _iterations(0)
    , _moves(0)
    , _sourceEqualTargetMovementNumber(0)
    , _surOccupationTargetMovementNumber(0)
    , _impossibleExchangeMovementNumber(0)
{
    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        _instanceBins.push_back(NULL);
    }

    for (unsigned netid = 0; netid < _mauka->_nets.size(); netid++)
    {
        vector<Box> bboxes(2);
        _netBBoxes.push_back(bboxes);
        vector<double> costs(2);
        _netCosts.push_back(costs);
        _netFlags.push_back(0);
    }
    
}

void SimAnnealingPlacer::init()
{
    Surface* surface = _mauka->_surface;
    if (!surface)
        throw Error("Cannot init with no surface ...");
    
    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        if (_instanceBins[i] == NULL)
            cerr << "no bin for " << _mauka->_instanceOccurrencesVector[i] << endl;
    }
    
    _initNetCost = _netCost = getNetCost();
    _initBinCost = _binCost = surface->getBinCost();
    _initRowCost = _rowCost = surface->getRowCost();

    double   stdDev    = 0.0;
    double   sumCost   = 0.0;
    double   sumSquare = 0.0;
    unsigned accepted  = 0;

    _temperature = 1e30;
    _distance    = 1.0;

    cmess1 << "  o  Beginning global placement" << endl;
    cmess2 << Dots::asSizet     ("     - Number of nodes to place",_mauka->_instanceOccurrencesVector.size()) << endl;
    cmess2 << Dots::asPercentage("     - Margin"        ,surface->getMargin()) << endl; 

    cmess2 << Dots::asDouble("     - Initial RowCost"   ,_rowCost) << endl;
    cmess2 << Dots::asDouble("     - Initial BinCost"   ,_binCost) << endl;
    cmess2 << Dots::asDouble("     - Initial NetCost"   ,_netCost) << endl;
    cmess2 << Dots::asDouble("     - Initial Cost"      ,getCost()) << endl;
    cmess2 << Dots::asLambda("     - Total bins capa"   ,surface->getBinsCapa()) << endl;
    cmess2 << Dots::asLambda("     - Total bins size"   ,surface->getBinsSize()) << endl;
    cmess2 << Dots::asLambda("     - Total subrows capa",surface->getSubRowsCapa()) << endl;
    cmess1 << "  o  Computing initial Temperature ...";

    Move move(this);

    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        if (!move.Next(_distance))
        {
          cmess2 << "  o  No more mouvement possible..." << endl;
          return;
        }

        double deltaRowCost = move.getDeltaRowCost();
        double deltaBinCost = move.getDeltaBinCost();
        double deltaNetCost = move.getDeltaNetCost();

        double deltaCost = computeCost(deltaRowCost, deltaBinCost, deltaNetCost);
        
        
        if (accept(deltaCost))
        {
            move.accept();
            accepted += 1;
            _rowCost += deltaRowCost;
            _netCost += deltaNetCost;
            _binCost += deltaBinCost;
            double cost = getCost();
            sumCost += cost; sumSquare += cost * cost;
            //sumCost += deltaCost; sumSquare += deltaCost * deltaCost;
        } 
        else
        {
            move.Reject();
        }
        ++_moves;
    }
    
    stdDev = computeStdDev(sumCost, sumSquare, accepted);
    _temperature = 20.0 * stdDev;
    _iterationsFactor = 15;
    _iterations  = (unsigned)(_iterationsFactor * pow(_mauka->_instanceOccurrencesVector.size(), 1.33));

    if (false)
    {
        double debug = DebugNetCost();
        cout << "debug = " << debug << endl;
        cout << "NetCost = " << _netCost << endl << endl;
        //	assert ((netCost - 1.0 <= debug) && (debug <= netCost + 1.0));
    }

    cmess1 << " done" << endl;
}


bool SimAnnealingPlacer::Iterate()
{
    unsigned accepted = 0;
    double sumCost = 0.0;
    double sumSquare = 0.0;
    double stdDev = 0.0, sucRatio = 0.0;
    double oldTemperature = 0.0;

    Move move(this);

    for (unsigned i = 0; i < _iterations; ++i)
    {
        if (!move.Next(_distance))
        {
          cmess1 << "  o  No More Mouvement Possible ....." << endl;
          return false;
        }
	    
        double deltaRowCost = move.getDeltaRowCost();
        double deltaBinCost = move.getDeltaBinCost();
        double deltaNetCost = move.getDeltaNetCost();

        double deltaCost = computeCost(deltaRowCost, deltaBinCost, deltaNetCost);

#if 0
        cerr << deltaRowCost << endl;
        cerr << deltaBinCost << endl;
        cerr << deltaNetCost << endl;
        cerr << deltaCost << endl << endl; 
#endif

        if (accept(deltaCost)) {
            move.accept();

            accepted += 1;
            _rowCost += deltaRowCost;
            _binCost += deltaBinCost;
            _netCost += deltaNetCost;
            double cost = getCost();
            sumCost += cost; sumSquare += cost * cost;
            //sumCost += deltaCost; sumSquare += deltaCost * deltaCost;
        }
        else
        {
            move.Reject();
        }
        ++_moves;
    }
    ++_loop;
    
    oldTemperature = _temperature;
    stdDev = computeStdDev(sumCost, sumSquare, accepted);

    if (stdDev == 0.0)
        _temperature = 0.0;
    else
        _temperature = _temperature * max(0.5, exp(-0.7 * _temperature / stdDev));
    
    sucRatio = accepted / (double)_iterations;
    _distance = max(0.1, min(_distance * (1.0 - 0.44 + sucRatio), 1.0));

    cmess2 << "     - Loop:"                    << setw(4) << setfill('0')<< _loop
           << " NIns:"                          << _mauka->_instanceOccurrencesVector.size() 
           << " Temperature:"                   << _temperature
           << " Cost:"                          << getCost() << endl;
    cmess2 << "                 RowCost:"       << _rowCost
           << " BinCost:"                       <<  _binCost
           << " NetCost:"                       << _netCost << endl;
    cmess2 << "                 Success Ratio:" << sucRatio * 100.0
           << "% Dist:"                         << _distance
           << " Delta:"                         << _temperature / oldTemperature << endl;

    if (false)
    {
        double debug = DebugNetCost();
        cout << "debug = " << debug << endl;
        cout << "NetCost = " << _netCost << endl << endl;
        //	assert ((netCost - 1.0 <= debug) && (debug <= netCost + 1.0));
    }

    if ( _mauka->getRefreshCb() != NULL ) _mauka->getRefreshCb() ();

    return ((_temperature != 0.0)
            && ((sucRatio > 0.15) || (stdDev > (0.0001 / getCost()))));
}

double SimAnnealingPlacer::getNetCost()
// ************************************
{
    double totalNetCost = 0.0;
    for (unsigned netid = 0; netid < _mauka->_netInstances.size(); netid++)
    {
        unsigned lastInstanceId = 0;
        unsigned insCount = 0;
        Box& netBBox = _netBBoxes[netid][_netFlags[netid]];

        double& netCost = _netCosts[netid][_netFlags[netid]];
        netCost = 0.0;
        for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netid].begin();
                uvit != _mauka->_netInstances[netid].end();
                uvit++)
        {
            unsigned instanceId = *uvit;
            lastInstanceId = instanceId;
            Bin* bin = _instanceBins[instanceId];
            netBBox.merge(bin->getCenter().getX(), bin->getCenter().getY());
            ++insCount;
        }
        if (_mauka->_hasInitX[netid])
            netBBox.merge(_mauka->_netInitX[netid], netBBox.getYMin()); 
        if (_mauka->_hasInitY[netid])
            netBBox.merge(netBBox.getXMin(), _mauka->_netInitY[netid]); 
        
        if ( insCount == 0 )
        {
          cerr << Warning("Net <%s> is not connected.",getString(_mauka->_nets[netid]).c_str()) << endl;
        }
        else
        {
            DbU::Unit width = 0;
#if 0
            if (_mauka->_params.takeSplittersIntoAccountOptimization)
            {
                Instance* instance = _instanceOccurrences[lastInstanceId]; 
                Net* net = _nets[netid]; 
                Plug* netPlug = NULL;
                for_each_plug(plug, instance->getConnectedPlugs())
                {
                    Net* plugNet = plug->getNet();
                    if (plugNet == net)
                    {
                        netPlug = plug;
                        break;
                    }
                    end_for;
                }
                if (!netPlug)
                    throw Error("MaukaEngine internal error: no plug");
                Hook* bodyHook = netPlug->getBodyHook();
                for_each_hook(hook, bodyHook->getHooks())
                {
                    Component* component = hook->getComponent();
                    if (SplitterContact* splitterContact = dynamic_cast<SplitterContact*>(component))
                    {
                        if (VFence* vfence = dynamic_cast<VFence*>(splitterContact->getSplitter()->getFence()))
                        {
                            if (_hasInitX[netid])
                                _hasInitX[netid] = false;
                            else
                            {
                                _hasInitX[netid] = true;
                                _netInitX[netid] = vfence->getX();
                            }
                        }
                        else if (HFence* hfence = dynamic_cast<HFence*>(splitterContact->getSplitter()->getFence()))
                        {
                            if (_hasInitY[netid])
                                _hasInitY[netid] = false;
                            else
                                {
                                    _hasInitY[netid] = true;
                                    _netInitY[netid] = hfence->getY();
                                }
                        }
                        else throw Error("MaukaEngine internal error");
                    }
                    else if (Pin* pin = dynamic_cast<Pin*>(component))
                    {
                      if (not _mauka->odIgnorePins())
                        {
                            netBBox.merge(pin->getX(), pin->getY());
                        }
                    }
                    end_for;
                }
                if (_hasInitX[netid])
                    netBBox.merge(_netInitX[netid], netBBox.getYMin()); 
                if (_hasInitY[netid])
                    netBBox.merge(netBBox.getXMin(), _netInitY[netid]); 
            }
#endif
            width = netBBox.getWidth();
            if (width == 0)
            {
                //all instances in the same bin...
                //take for width half of the bin
                Bin* bin = _instanceBins[lastInstanceId];
                width = bin->getWidth() / 2;
            }
            double cost = DbU::getLambda(netBBox.getHeight() + width);
            netCost = cost;
            totalNetCost += cost;
        }
    }
    return totalNetCost;
}

double SimAnnealingPlacer::DebugNetCost()
// **************************************
{
    double totalNetCost = 0.0;
    for (unsigned netid = 0; netid < _mauka->_netInstances.size(); netid++)
    {
        unsigned lastInstanceId = 0;
        unsigned insCount = 0;
        Box box;
        for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netid].begin();
                uvit != _mauka->_netInstances[netid].end();
                uvit++)
        {
            unsigned instanceId = *uvit;
            lastInstanceId = instanceId;
            Bin* bin = _instanceBins[instanceId];
            box.merge(bin->getCenter().getX(), bin->getCenter().getY());
            ++insCount;
        }
            
        if (insCount)
        {
#if 0
          if (_mauka->_params.takeSplittersIntoAccountOptimization)
            {
                Occurrence instanceOccurrence = _instanceOccurrences[lastInstanceId];
                Instance* instance = dynamic_cast<Instance*>(instanceOccurrence.getEntity());
                assert(instance);
                Net* net = _nets[netid]; 
                Plug* netPlug = NULL;
                for_each_plug(plug, instance->getConnectedPlugs())
                {
                    Net* plugNet = plug->getNet();
                    if (plugNet == net)
                    {
                        netPlug = plug;
                        break;
                    }
                    end_for;
                }
                if (!netPlug)
                    throw Error("MaukaEngine internal error: no plug");
                Hook* bodyHook = netPlug->getBodyHook();
                bool hasInitX = false;
                bool hasInitY = false;
                DbU::Unit netInitX = 0;
                DbU::Unit netInitY = 0;
                for_each_hook(hook, bodyHook->getHooks())
                {
                    if (SplitterContact* splitterContact = dynamic_cast<SplitterContact*>(hook->getComponent()))
                    {
                        if (VFence* vfence = dynamic_cast<VFence*>(splitterContact->getSplitter()->getFence()))
                        {
                            if (hasInitX)
                                hasInitX = false;
                            else
                            {
                                hasInitX = true;
                                netInitX = vfence->getX();
                            }
                        }
                        else if (HFence* hfence = dynamic_cast<HFence*>(splitterContact->getSplitter()->getFence()))
                        {
                            if (hasInitY)
                                hasInitY = false;
                            else
                                {
                                    hasInitY = true;
                                    netInitY = hfence->getY();
                                }
                        }
                        else throw Error("MaukaEngine internal error");
                    }
                    end_for;
                }
                if (hasInitX)
                    box.merge(netInitX, box.getYMin()); 
                if (hasInitY)
                    box.merge(box.getXMin(), netInitY); 
            }
#endif
            DbU::Unit width = box.getWidth();
            if (width == 0)
            {
                //all instances in the same bin...
                //take for width half of the bin
                Bin* bin = _instanceBins[lastInstanceId];
                width = bin->getWidth() / 2;
            }
            totalNetCost += DbU::getLambda(box.getHeight() + width);
        }
    }
    return totalNetCost;
}

void SimAnnealingPlacer::DisplayResults() const
{
  unsigned totalImpossibleMovements
    = _impossibleExchangeMovementNumber
    + _sourceEqualTargetMovementNumber
    + _surOccupationTargetMovementNumber; 

  cmess2 << Dots::asUInt      ("     - Total impossible movements",totalImpossibleMovements) << endl;
  cmess2 << Dots::asPercentage("     - Suroccupied target"        ,100.0 * _surOccupationTargetMovementNumber / totalImpossibleMovements) << endl;
  cmess2 << Dots::asPercentage("     - Source equal target"       ,100.0 * _sourceEqualTargetMovementNumber   / totalImpossibleMovements) << endl;
  cmess2 << Dots::asPercentage("     - Impossible exchange"       ,100.0 * _impossibleExchangeMovementNumber  / totalImpossibleMovements) << endl; 
  cmess1 <<                    "  o  Global Placement finished" << endl;
  cmess2 << Dots::asPercentage("     - Gain for RowCost" ,100.0 * (_initRowCost - _rowCost) / _initRowCost) << endl;
  cmess2 << Dots::asPercentage("     - Gain for BinCost" ,100.0 * (_initBinCost - _binCost) / _initBinCost) << endl;
  cmess2 << Dots::asPercentage("     - Gain for NetCost" ,100.0 * (_initNetCost - _netCost) / _initNetCost) << endl;
  cmess2 << Dots::asDouble    ("     - NetCost Estimated",_netCost) << endl;
}

double SimAnnealingPlacer::getCost() const
{
    return computeCost(_rowCost, _binCost, _netCost);
}

double SimAnnealingPlacer::computeCost(double rowcost, double bincost, double netcost) const
{
    return rowcost  /  _initRowCost * _rowMult
        +  bincost  /  _initBinCost * _binMult
        +  netcost  /  _initNetCost * _netMult;
}

bool SimAnnealingPlacer::accept(double deltacost) const
{
  if (_mauka->useStandardSimulatedAnnealing())
    {
        double doubleRand = (double) (rand() / (RAND_MAX + 1.0));
        return ((deltacost <= 0.0)
                || ((_temperature != 0.0)
                    && (exp(-deltacost / _temperature) > doubleRand)));
    }
    else
        return deltacost <= 0.0;
}


void SimAnnealingPlacer::Save() const
{
    UpdateSession::open();
    for(unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        Occurrence instanceOccurrence =  _mauka->_instanceOccurrencesVector[i];
        Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
        Bin* bin = _instanceBins[i];
        bool rowOrientation = bin->getSubRow()->getRow()->getOrientation();
        if (!bin)
            throw Error("No bin for instance");
        DbU::Unit xPos = bin->getCenter().getX();
        DbU::Unit yPos = bin->getCenter().getY();
        Box masterABox = instance->getMasterCell()->getAbutmentBox();
        Transformation::Orientation orientation;
        if (rowOrientation)
            orientation = Transformation::Orientation::ID;
        else
            orientation = Transformation::Orientation::MY;
        Transformation instanceTransformation = getTransformation(masterABox
                , xPos - masterABox.getHalfWidth()
                , yPos - masterABox.getHalfHeight()
                , orientation);
        instanceOccurrence.getPath().getTransformation().invert().applyOn(instanceTransformation);
        instance->setTransformation(instanceTransformation);
        instance->setPlacementStatus(Instance::PlacementStatus::PLACED);
        //setPlacementStatusRecursivelyToPlaced(instance);
    }
    UpdateSession::close();
}

void SimAnnealingPlacer::Plot(ofstream& out) const
{
    out << "#instances" << endl;
    for (unsigned i = 0; i < _mauka->_instanceOccurrencesVector.size(); i++)
    {
        Instance* instance = static_cast<Instance*>(_mauka->_instanceOccurrencesVector[i].getEntity());
        const Bin* bin = _instanceBins[i];
        if (!bin)
            throw Error("No bin for instance");
        DbU::Unit xPos = bin->getCenter().getX();
        DbU::Unit yPos = bin->getCenter().getY();
        Box masterABox = instance->getMasterCell()->getAbutmentBox();
        Box instanceBox = Box(
                xPos - masterABox.getWidth() / 2,
                yPos - masterABox.getHeight() / 2,
                xPos + masterABox.getWidth() / 2,
                yPos + masterABox.getHeight() / 2);
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
            ++nbInstances;
            const Bin* bin = _instanceBins[instanceId];
            baryX += bin->getCenter().getX();
            baryY += bin->getCenter().getY();
        }
        
        baryX = baryX / nbInstances;
        baryY = baryY / nbInstances;
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            const Bin* bin = _instanceBins[instanceId];
            out << baryX << " " << baryY << endl
                << bin->getCenter().getX() << " "
                << bin->getCenter().getY() << endl << endl;
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
            ++nbInstances;
            const Bin* bin = _instanceBins[instanceId];
            baryX += bin->getCenter().getX();
            baryY += bin->getCenter().getY();
        }
        
        baryX = baryX / nbInstances;
        baryY = baryY / nbInstances;
        for (unsigned j = 0; j < _mauka->_netInstances[i].size(); j++)
        {
            unsigned instanceId = _mauka->_netInstances[i][j];
            const Bin* bin = _instanceBins[instanceId];
            out << baryX << " " << baryY << endl
                << bin->getCenter().getX() << " "
                << bin->getCenter().getY() << endl << endl;
        }
        out << baryX << " " << baryY << endl
            << _mauka->_netInitX[i] << " "
            << _mauka->_netInitY[i] << endl << endl;
    }
    out << "EOF" << endl << endl;
}

}
