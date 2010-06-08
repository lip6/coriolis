
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
#include <cstdlib>
#include <climits>

#include "hurricane/Error.h"
#include "hurricane/Plug.h"

#include "mauka/MaukaEngine.h"
#include "mauka/Bin.h"
#include "mauka/SubRow.h"
#include "mauka/Row.h"
#include "mauka/MaukaBox.h"
#include "mauka/SimAnnealingPlacer.h"
#include "mauka/Surface.h"
#include "mauka/Move.h"

namespace {

using Hurricane::DbU;
    
DbU::Unit PositionRand(const DbU::Unit position, const double distance, const DbU::Unit min, const DbU::Unit max)
{
    DbU::Unit borneInf, borneSup;

    if ((borneSup = position + DbU::lambda((int)(distance * DbU::getLambda(max) + 0.5)) ) > max )
        borneSup = max;

    if ((borneInf = position - DbU::lambda((int)(distance * DbU::getLambda(max) + 0.5)) ) < min )
        borneInf = min;
    
    return borneInf + DbU::lambda((int)(DbU::getLambda(borneSup - borneInf) * rand() / (RAND_MAX+1.0)));
}

} // End of anonymous namespace.


namespace Mauka {

using namespace std;

Move::Move(SimAnnealingPlacer* simannealingplacer)
    : _simAnnealingPlacer(simannealingplacer)
    , _mauka(_simAnnealingPlacer->_mauka)
    , _surface(_mauka->_surface)
    , _exchange(false)
    , _srcIns(0)
    , _srcBin(NULL)
    , _srcBinInitCost(0.0)
    , _srcSubRow(NULL)
    , _srcRow(NULL)
    , _srcRowInitCost(0.0)
    , _srcWidth(0)
    , _dstBin(NULL)
    , _dstBinInitCost(0.0)
    , _dstSubRow(NULL)
    , _dstRow(NULL)
    , _dstRowInitCost(0.0)
    , _dstIns(0)
    , _dstWidth(0)
    , _affectedNets()
{}

double Move::getDeltaRowCost() const
{
    double deltaRowCost = -_srcRowInitCost;
    deltaRowCost -= _dstRowInitCost;
    deltaRowCost += _srcRow->getCost();
    deltaRowCost += _dstRow->getCost();
    return deltaRowCost;
}

double Move::getDeltaBinCost() const
{
    double deltaBinCost = -_srcBinInitCost;
    deltaBinCost -= _dstBinInitCost;
    deltaBinCost += _srcBin->getCost();
    deltaBinCost += _dstBin->getCost();
#if 0
    cerr << "src init cost " << _srcBinInitCost << endl;
    cerr << "dst init cost " << _dstBinInitCost << endl;
    cerr << "src after cost " << _srcBin->getCost() << endl;
    cerr << "dst after cost " << _dstBin->getCost() << endl;
    cerr << deltaBinCost << endl;
    if (_exchange)
    {
        cerr << "exchange" << endl;
        cerr << "srcWidth " << _srcWidth << endl;
        cerr << "dstWidth " << _dstWidth << endl;
        cerr << "after .... src " << endl;
        cerr << "srcsize " << _srcBin->getSize() << endl;
        cerr << "srccapa " << _srcBin->getCapa() << endl;
    }
    else
    {
        cerr << "move" << endl;
        cerr << _srcWidth << endl;
        cerr << "after .... src " << endl;
        cerr << _srcBin->getSize() << endl;
        cerr << _srcBin->getCapa() << endl;
    }
#endif
    return deltaBinCost;
}

static const unsigned	NetSrc		= 1;
static const unsigned	NetDst		= 2;
static const unsigned	NetSrcDst	= 3;

double Move::getDeltaNetCost()
{
    // Find affected nets
    // ==================
    _affectedNets.clear();
    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_instanceNets[_srcIns].begin();
            uvit != _mauka->_instanceNets[_srcIns].end();
            uvit++)
    {
        _affectedNets[*uvit] = NetSrc;
    }

    if (_exchange)
    {
        for (MaukaEngine::UVector::const_iterator uvit = _mauka->_instanceNets[_dstIns].begin();
                uvit != _mauka->_instanceNets[_dstIns].end();
                uvit++)
        {
            unsigned netId = *uvit;
            if (_affectedNets.find(netId) == _affectedNets.end())
                _affectedNets[netId] = NetDst;
            else
                if (_affectedNets[netId] != NetDst)
                _affectedNets[netId] = NetSrcDst;
        }
    }
    
    // compute delta
    // =============

    double delta = 0.0;
    for (AffectedNets::iterator anit = _affectedNets.begin();
	    anit != _affectedNets.end();
	    anit++) 
    {
        unsigned netId = anit->first;
        unsigned flag = anit->second;
        double& netCost = _simAnnealingPlacer->_getNetIdCost(netId);
        //cerr << "netCost " << netCost << endl;
        double& netTmpCost = _simAnnealingPlacer->_getNetIdTmpCost(netId);
        //cerr << "netTmpCost " << netTmpCost << endl; 
        Box& currBox = _simAnnealingPlacer->_getNetIdBBox(netId);
        Box& tmpBox  = _simAnnealingPlacer->_getNetTmpBBox(netId);
        //cerr << "before" << endl;
        //cerr << "currBox " << currBox << endl;
        //cerr << "tmpBox " << tmpBox << endl;
	
        if (flag == NetSrc)
        {
            tmpBox = currBox;
            if ((tmpBox = Update(tmpBox , _srcBin->getCenter(), _dstBin->getCenter())).isEmpty())
            {
                for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netId].begin();
                        uvit != _mauka->_netInstances[netId].end();
                        uvit++)
                {
                    unsigned instanceId = *uvit;
                    Bin* bin = _simAnnealingPlacer->_instanceBins[instanceId];
                    tmpBox.merge(bin->getCenter().getX(), bin->getCenter().getY());
                }
            }

            if (_mauka->_hasInitX[netId])
                tmpBox.merge(_mauka->_netInitX[netId], tmpBox.getYMin());
            if (_mauka->_hasInitY[netId])
                tmpBox.merge(tmpBox.getXMin(), _mauka->_netInitY[netId]);
 
            DbU::Unit width = tmpBox.getWidth();
            if (width == 0)
            {
                width = _srcBin->getWidth() / 2;
            }
            netTmpCost = DbU::getLambda(tmpBox.getHeight() + width);
#if 0
            cerr << "tmpBox " << tmpBox <<endl;
            cerr << "  SrcPos     = " << _srcBin->getCenter() << endl;
            cerr << "  DstPos     = " << _dstBin->getCenter() << endl;
            cerr << "netTmpCost(netSrc) " << netTmpCost << endl << endl;
#endif
            delta += netTmpCost - netCost;
            
#if 0 // code pour debug .... 
	    Box checkBox;
            for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netId].begin();
                    uvit != _mauka->_netInstances[netId].end();
                    uvit++)
	    {
                unsigned instanceId = *uvit;
                Bin* bin = _mauka->_instanceBins[instanceId];
                checkBox.merge(bin->getCenter().getX(), bin->getCenter().getY());
	    }

	    if (checkBox != tmpBox) {
            cout << "error: mauvaise bbox : NetSrc" << endl;
            cout << "  checkBox = " << checkBox << endl;
            cout << "  tmpBox   = " << tmpBox << endl;
            cout << "  CurrBBox   = " << currBox  << endl;
            cout << "  SrcPos     = " << _srcBin->getCenter() << endl;
            cout << "  DstPos     = " << _dstBin->getCenter() << endl;
            exit(1);
	    }
#endif
        }
        else
            if (flag == NetDst)
            {
                tmpBox = currBox;
                if ((tmpBox = Update(tmpBox , _dstBin->getCenter(), _srcBin->getCenter())).isEmpty())
                {
                    for (MaukaEngine::UVector::const_iterator uvit = _mauka->_netInstances[netId].begin();
                            uvit != _mauka->_netInstances[netId].end();
                            uvit++)
                    {
                        Bin* bin = _simAnnealingPlacer->_instanceBins[*uvit];
                        tmpBox.merge(bin->getCenter().getX(), bin->getCenter().getY());
                    }
                }
                if (_mauka->_hasInitX[netId])
                    tmpBox.merge(_mauka->_netInitX[netId], tmpBox.getYMin());
                if (_mauka->_hasInitY[netId])
                    tmpBox.merge(tmpBox.getXMin(), _mauka->_netInitY[netId]);
                DbU::Unit width = tmpBox.getWidth();
                if (width == 0)
                {
                    width = _dstBin->getWidth() / 2;
                }
                netTmpCost = DbU::getLambda(tmpBox.getHeight() + width);
#if 0
                cerr << "netDst" << endl;
                cerr << "width " << tmpBox.getWidth() << endl;
                cerr << "height " << tmpBox.getHeight() << endl;
                cerr << "netTmpCost(netDst) " << netTmpCost << endl << endl;
#endif
                delta += netTmpCost - netCost;
#ifdef MOVE_DEBUG
	    cout << "end" << endl;
	    cout << "check" << endl;
	    Box checkBox;
	    for_each_plug(plug, net);
	    {
		Instance* ins = plug->getInstance(); 
		SurfContainer* container = PGetContainer(*ins);
		cout << container->getXCenter() << " " << container->getYCenter() << endl;
		checkBox.merge(container->getXCenter(), container->getYCenter());
		end_for;
	    }

	    if (checkBox != *PTmpBBox(*net)) {
		cout << "error: mauvaise bbox : NetDst" << endl;
		cout << "  check_bbox = " << checkBox << endl;
		cout << "  TmpBBox   = " << PTmpBBox(*net) << endl;
		cout << "  CurrentBBox   = " <<  PCurrentBBox(*net) << endl;
		cout << "  SrcPos     = " << _dstBin->getPos() << endl;
		cout << "  DstPos     = " << _srcBin->getPos() << endl;
		exit(1);
	    }
#endif
            }
    }
    return delta;
}

void Move::TryMove()
{
    if (!_exchange)
    {
        _srcBin->removeInstance(_srcIns);
        _dstBin->addInstance(_srcIns);
    }
    else
    {
        _srcBin->removeInstance(_srcIns);
        _dstBin->removeFrontInstance(_dstIns);
        _dstBin->addInstance(_srcIns);
        _srcBin->addInstance(_dstIns);
    }
}
    
bool Move::Next(double dist)
{
    bool moveCondition;
    unsigned nbrefused = 0;

    // Choisi un mouvement
    // ===================

    do {
#if 0
        if (1)
        {
            if (dist < 0.4)
                for_each_view(view, _mauka->getCell()->getViews())
                {
                    if (CEditor* editor = dynamic_cast<CEditor*>(view))
                    {
                        if (_srcBin)
                            editor->Unselect(_srcBin);
                        if (_dstBin && (_dstBin != _srcBin))
                            editor->Unselect(_dstBin);
                        break;
                    }
                    end_for;
                }
        }
#endif


        moveCondition = true;

        _srcIns = _mauka->getRandomInstanceId();
        assert ( _srcIns < _simAnnealingPlacer->_instanceBins.size() );  // d2 11/02/05
        _srcBin = _simAnnealingPlacer->_instanceBins[_srcIns];
        _srcSubRow = _srcBin->getSubRow();
        _srcRow = _srcSubRow->getRow();
        assert ( _srcIns < _mauka->_instanceWidths.size() );  // d2 11/02/05
        _srcWidth = _mauka->_instanceWidths[_srcIns];
        _srcBinInitCost = _srcBin->getCost();
#if 0
        cerr << "_srcBin " << endl;
        cerr << "srcbinsize " << _srcBin->getSize() << endl;
        cerr << "srcbincapa " << _srcBin->getCapa() << endl;
#endif
            
        _srcRowInitCost = _srcRow->getCost();
        
        _dstBin = _surface->getBinInSurface(_srcBin, dist);
        _dstSubRow = _dstBin->getSubRow();
        _dstRow = _dstSubRow->getRow();

#if 0
        if (1)
        {
            if (dist < 0.4)
            {
                for_each_view(view, _mauka->getCell()->getViews())
                {
                    if (CEditor* editor = dynamic_cast<CEditor*>(view))
                    {
                        cerr << _srcBin << endl;
                        cerr << _srcRow << endl;
                        cerr << _srcSubRow << endl;
                        cerr << _dstBin << endl;
                        cerr << _dstRow << endl;
                        cerr << _dstSubRow << endl;
                        cerr << endl;
                        editor->Select(_srcBin);
                        if (_dstBin != _srcBin)
                            editor->Select(_dstBin);
                        editor->Stop("gli");
                        break;
                    }
                    end_for;
                }
            }
        }
#endif

        _dstBinInitCost = _dstBin->getCost();
#if 0
        cerr << "initially .... dst" << endl;
        cerr << "dstbinsize " << _dstBin->getSize() << endl;
        cerr << "dstbincapa " << _dstBin->getCapa() << endl;
#endif
        _dstRowInitCost = _dstRow->getCost(); 

        if (_dstBin == _srcBin)
        {
            _simAnnealingPlacer->incrSourceEqualTargetMovementNumber();
            moveCondition = false;
        }
        if (moveCondition && _dstBin->UnderOccupied())
        {
            _exchange = false;
            // Le bin destination est sous-occupé
            // On tente de déplacer l'instance
            if (_dstSubRow->getWidth() - _dstSubRow->getSize() < _srcWidth)
            {
                moveCondition = false;
                _simAnnealingPlacer->incrSurOccupationTargetMovementNumber();
            }
        }
        else if (moveCondition)
        {
            _exchange = true;
            _dstIns = _dstBin->getFirstInstanceOccurrenceId();
            assert ( _dstIns < _mauka->_instanceWidths.size() );  // d2 11/02/05
            _dstWidth = _mauka->_instanceWidths[_dstIns];

            if (_srcSubRow->getWidth() - _srcSubRow->getSize() < _dstWidth - _srcWidth)
            {
                //Try to move the src ins to dst bin
                if (_dstSubRow->getWidth() - _dstSubRow->getSize() < _srcWidth)
                {
                    moveCondition = false;
                    _simAnnealingPlacer->incrSurOccupationTargetMovementNumber();
                }
                _exchange = false;
            }
            if (_exchange
                    && (_dstSubRow->getWidth() - _dstSubRow->getSize() < _srcWidth - _dstWidth))
            {
                //Try to move the dst ins to src bin
                if (_srcSubRow->getWidth() - _srcSubRow->getSize() < _dstWidth)
                {
                    moveCondition = false;
                    _simAnnealingPlacer->incrSurOccupationTargetMovementNumber();
                }
                else
                {
                    _exchange = false;
                    _srcIns = _dstIns;
                    _srcWidth = _dstWidth;
                    Bin* tmpBin = _dstBin;
                    _dstBin = _srcBin;
                    _srcBin = tmpBin;
                    SubRow* tmpSubRow = _dstSubRow;
                    _dstSubRow = _srcSubRow;
                    _srcSubRow = tmpSubRow;
                    Row* tmpRow = _dstRow;
                    _dstRow = _srcRow;
                    _srcRow = tmpRow;
                    double tmp2 = _dstRowInitCost;
                    _dstRowInitCost = _srcRowInitCost;
                    _srcRowInitCost = tmp2;
                    tmp2 = _dstBinInitCost;
                    _dstBinInitCost = _srcBinInitCost;
                    _srcBinInitCost = tmp2;
                }
            }
        }
        if (!moveCondition)
        {
            ++nbrefused;
        }
        if (nbrefused > (unsigned)(1.5 * _mauka->_instanceOccurrencesVector.size()))
            return false;
    } while (!moveCondition);
   
    // Deplace les instances
    // =====================
    _srcBin->incrementSourceHits();
    _dstBin->incrementTargetHits();
    
    TryMove();

#if TO_BE_PORTED_UNDER_CORIOLIS_2
    if (_mauka->getRunMode().actionIsOn())
    {
        if (dist < 0.4)
            for_each_view(view, _mauka->getCell()->getViews())
            {
                if (CEditor* editor = dynamic_cast<CEditor*>(view))
                {
                    editor->Refresh();
                    editor->Stop("coucou");
                    break;
                }
                end_for;
            }
    }
#endif
    return true;
}

void
Move::accept()
{
    // Sauvegarde des cout des nets
    for (AffectedNets::iterator anit = _affectedNets.begin();
            anit != _affectedNets.end();
            anit++)
    {
        unsigned netId = anit->first;
        unsigned flag = anit->second;
        if (flag == NetSrc || flag == NetDst)
        {
            _simAnnealingPlacer->_InvertNetIdFlag(netId);
        }
    }
}

void
Move::Reject()
{
    if (!_exchange)
    {
        _dstBin->removeBackInstance(_srcIns);
        _srcBin->addInstance(_srcIns);
    }
    else
    {
        _srcBin->removeBackInstance(_dstIns);
        _dstBin->removeBackInstance(_srcIns);
        _dstBin->addInstance(_dstIns);
        _srcBin->addInstance(_srcIns);
    }
}

}
