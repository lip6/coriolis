
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
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

#ifdef HAVE_HMETIS_LIB

#include <climits>

#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/Plug.h"
#include "hurricane/Pin.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
using namespace CRL;

#include "nimbus/Splitter.h"
#include "nimbus/GCell.h"
#include "nimbus/SplitterContact.h"
#include "nimbus/StepProperty.h"
#include "nimbus/NimbusEngine.h"
using namespace Nimbus;

#include "metis/hmetis.h"
#include "metis/MetisGraph.h"

namespace {

using namespace Metis;
    
typedef map<unsigned, Occurrence> Id2OccurrencesMap;
    
struct removeEmptyPartResult
{
    removeEmptyPartResult() {}
    bool operator()(MetisGraph::PartResult* partresult)
    {
        if (partresult->second.size() == 0)
        {
            delete partresult;
            return true;
        }
        return false;
    }
};


void VerifyHGraph(int nvtxs, int nhedges, int* vwgts, int* eptr
        , int* eind, int* hewgts, int nparts, int* part
        , Id2OccurrencesMap& hypernetidmap)
{
    cerr << "nparts = " << nparts << endl;
    cerr << "nvtxs = " << nvtxs << endl;

    cerr << "vwgts" << endl;
    for (int i=0; i < nvtxs; i++)
    {
        cerr << vwgts[i] << " ";
    }
    cerr << endl;

    cerr << "nhedges = " << nhedges << endl;

    cerr << "eptr" << endl;
    for (int i=0; i <= nhedges; i++)
    {
        cerr << eptr[i] << " ";
    }
    cerr << endl;

    cerr << "eind" << endl;
    for (int i=0; i < eptr[nhedges]; i++)
    {
        cerr << eind[i] << " ";
    }
    cerr << endl;

    if (hewgts)
    {
        cerr << "hewgts" << endl;
        for (int i=0; i < nhedges; i++)
        {
            cerr << hewgts[i] << " ";
        }
        cerr << endl;
    }

    cerr << "part" << endl;
    for (int i=0; i < nvtxs; i++)
    {
        cerr << part[i] << " ";
    }
    cerr << endl;

    cerr << "nets" << endl;
    for (int i=0; i < nhedges; i++)
    {
        cerr << hypernetidmap[i] << " ";
        if (hewgts)
        {
            cerr << "(" << hewgts[i] << ") : ";
        }
        else
        {
            cerr << ": " << endl;
        }
        for (int j=eptr[i]; j < eptr[i+1]; j++)
        {
            cerr << "(" << eind[j] << "," << vwgts[eind[j]] << ") " ;
        }
        cerr << endl;
    }
}


} // End of anonymous namespace.


namespace Metis {

MetisGraph::MetisGraph ( MetisEngine* metis, GCell* gcell )
    : _metis(metis)
    , _cell(_metis->getCell())
    , _gcell(gcell)
    , _toPlaceInstanceOccurrencesSet()
    , _rootNetOccurrencesSet()
    , _partResultVector()
    , _edgeCut(INT_MAX)
{
  typedef map<Instance*,Occurrence> OccurrencesLUT;
  OccurrencesLUT occurrencesLUT;

  for_each_occurrence(occurrence, _cell->getLeafInstanceOccurrences())
  {
    Instance* instance = static_cast<Instance*>(occurrence.getEntity());
    if (!instance->isFixed()) {
      OccurrencesLUT::iterator duplicated = occurrencesLUT.find(instance);
      if (duplicated != occurrencesLUT.end()) {
        throw Error("MetisEngine limitation: Each unplaced instance must have one occurrence only.\n"
                    "Model %s is intanciated as:<b>\n.    %s\n.    %s</b>\n  (at least)."
                   ,getString(instance->getMasterCell()->getName()).c_str()
                   ,occurrence.getCompactString().c_str()
                   ,(*duplicated).second.getCompactString().c_str()
                   );
      }
      occurrencesLUT.insert(make_pair(instance,occurrence));
      _toPlaceInstanceOccurrencesSet.insert(occurrence); //treat this later
    }
    end_for;
  }

    for_each_occurrence(occurrence, _cell->getHyperNetRootNetOccurrences())
    {
        Net* net = static_cast<Net*>(occurrence.getEntity());
        if (net->isGlobal() || net->isPower() || net->isGround())
            continue;
        if (net->getCell()->isLeaf())
            continue;
        _rootNetOccurrencesSet.insert(occurrence);
        end_for;
    }
}

MetisGraph::MetisGraph(MetisEngine* metis, MetisGraph* previous, GCell* gcell, OccurrenceSet& toplaceinstanceoccurrences)
    : _metis(metis)
    , _cell(_metis->getCell())
    , _gcell(gcell)
    , _toPlaceInstanceOccurrencesSet(toplaceinstanceoccurrences)
    , _rootNetOccurrencesSet()
    , _partResultVector()
{
    for (OccurrenceSet::iterator osit = previous->_rootNetOccurrencesSet.begin();
            osit != previous->_rootNetOccurrencesSet.end();
            osit++)
    {
        HyperNet hyperNet(*osit);
        for_each_occurrence(leafPlugOccurrence, hyperNet.getLeafPlugOccurrences())
        {
            Path path = leafPlugOccurrence.getPath();
            Instance* instance = (static_cast<Plug*>(leafPlugOccurrence.getEntity()))->getInstance();
            Occurrence instanceOccurrence(instance, path);
            OccurrenceSet::const_iterator iosit = _toPlaceInstanceOccurrencesSet.find(instanceOccurrence);
            if (iosit != _toPlaceInstanceOccurrencesSet.end())
            {
                _rootNetOccurrencesSet.insert(*osit); //treat later fixed points.
                break;
            }
            end_for;
        }
    }
    //treat fixed points in Part method
}

MetisGraph::~MetisGraph()
{
    for (PartResultVector::iterator prvit = _partResultVector.begin();
            prvit != _partResultVector.end();
            prvit++)
    {
        delete (*prvit);
    }
            
}

int MetisGraph::part ( linefill& output )
{
    typedef vector<GCell*> GCellVector;
    GCellVector subGCells;
    
    if (not _gcell->hasSubGCells())
      throw Error("Metis: GCell doesn't have any sub-GCells");
    for_each_gcell(gcell, _gcell->getSubGCells())
    {
        subGCells.push_back(gcell);
        end_for;
    }
    
    unsigned subGCellsCount = subGCells.size();

    if (subGCellsCount < 2)
      throw Error("Metis, Less than 2 sub-GCells (%d)",subGCellsCount);

    vector<double> subGCellsCountOccupation(subGCellsCount, 0.0);
    typedef list<Occurrence> InstanceOccurrencesList;
    typedef vector<InstanceOccurrencesList> BoxesInstanceOccurrencesVector;
    typedef vector<list<double> > BoxesInstanceOccurrencesWeights;
    BoxesInstanceOccurrencesVector subGCellsFixedInstanceOccurrences(subGCellsCount, InstanceOccurrencesList());
    BoxesInstanceOccurrencesWeights subGCellsFixedInstanceOccurrenceWeights(subGCellsCount, list<double>());

    vector<int> vwgts_vector;
    vector<int> hewgts_vector;
    vector<int> part_vector;
    
    typedef map<Occurrence, unsigned> Occurrences2IdMap;
    Occurrences2IdMap instanceOccurrencesMap;

    Id2OccurrencesMap toPlaceInstanceOccurrencesMap;

    for (unsigned gcellCount = 0; gcellCount != subGCells.size(); gcellCount++)
    {
        GCell* gcell = subGCells[gcellCount];
        for_each_occurrence(instanceOccurrence, _cell->getLeafInstanceOccurrencesUnder(gcell->getBox()))
        {
            Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
            if (instance->isFixed())
            {
                Box instanceOccurrenceABox = instance->getAbutmentBox();
                instanceOccurrence.getPath().getTransformation().applyOn(instanceOccurrenceABox);
                if (gcell->contains(instanceOccurrenceABox))
                {
                    double instanceWeight =
                        DbU::getLambda(instance->getMasterCell()->getAbutmentBox().getWidth())
                        * DbU::getLambda(instance->getMasterCell()->getAbutmentBox().getHeight());
                    subGCellsCountOccupation[gcellCount] += instanceWeight;
                    subGCellsFixedInstanceOccurrenceWeights[gcellCount].push_back(instanceWeight);
                    subGCellsFixedInstanceOccurrences[gcellCount].push_back(instanceOccurrence);
                    continue;
                }
                else
                {
                    Box intersection = gcell->getIntersection(instanceOccurrenceABox);
                    if (!intersection.isEmpty())
                    {
                        double intersectionWeight =
                            DbU::getLambda(intersection.getWidth()) * DbU::getLambda(intersection.getHeight()); 

                        subGCellsCountOccupation[gcellCount] += intersectionWeight;
                        subGCellsFixedInstanceOccurrenceWeights[gcellCount].push_back(intersectionWeight);
                        subGCellsFixedInstanceOccurrences[gcellCount].push_back(instanceOccurrence);
                        //FIXME only the last part of the fixed point will be taken into account
                    }
                }
            }
            end_for;
        }
    }


    unsigned nodeId = 0;
    
    //treat fixed instances occurrences
    for (unsigned gcellCount = 0; gcellCount != subGCells.size(); gcellCount++)
    {
        GCell* gcell = subGCells[gcellCount];
        double gcellArea = DbU::getLambda(gcell->getWidth()) * DbU::getLambda(gcell->getHeight());
        if (((gcellArea - subGCellsCountOccupation[gcellCount])/gcellArea) < 0.10)
        {
            cerr << "surOccupied gcell : " << gcell << endl;
        }
        else
        {
            _partResultVector.push_back(new PartResult(gcell, OccurrenceSet()));
            unsigned gcellId = _partResultVector.size() - 1;
            unsigned fixedNodesCount = subGCellsFixedInstanceOccurrences[gcellCount].size();
            list<double>::const_iterator dlit =
                subGCellsFixedInstanceOccurrenceWeights[gcellCount].begin();
            InstanceOccurrencesList::const_iterator iolit =
                subGCellsFixedInstanceOccurrences[gcellCount].begin();
            for (unsigned id = 0; id != fixedNodesCount; id++)
            {
                vwgts_vector.push_back((int)*dlit);
                instanceOccurrencesMap[*iolit] = nodeId++;
                ++dlit; ++iolit;
            }
            part_vector.insert(part_vector.end(), fixedNodesCount, gcellId);
        }
    }

    //now treat instance occurrences to place

    typedef vector<Occurrence> OccurrenceVector;
    OccurrenceVector instanceOccurrenceVector(_toPlaceInstanceOccurrencesSet.begin(), _toPlaceInstanceOccurrencesSet.end());
    random_shuffle(instanceOccurrenceVector.begin(), instanceOccurrenceVector.end());
    
    for (OccurrenceVector::const_iterator ovit = instanceOccurrenceVector.begin();
            ovit != instanceOccurrenceVector.end();
            ovit++)
    {
        Instance* instance = static_cast<Instance*>(ovit->getEntity());
        double instanceWeight =
            DbU::getLambda(instance->getMasterCell()->getAbutmentBox().getWidth())
            * DbU::getLambda(instance->getMasterCell()->getAbutmentBox().getHeight());
        vwgts_vector.push_back((int)instanceWeight);
        part_vector.push_back(-1);
        toPlaceInstanceOccurrencesMap[nodeId] = *ovit;
        instanceOccurrencesMap[*ovit] = nodeId++;
    }

    output << _toPlaceInstanceOccurrencesSet.size();

    // constructing edges from hypernets
    vector<int> eptr_vector;
    vector<int> eind_vector;
    eptr_vector.push_back(0);
    int hyperEdgesCount = 0;

#ifdef METISSE_DEBUG
    Id2OccurrencesMap hyperNetIdMap;
    Id2OccurrencesMap nodesIdMap;
#endif

    OccurrenceVector netOccurrenceVector(_rootNetOccurrencesSet.begin(), _rootNetOccurrencesSet.end());
    
#if 0
    for (OccurrenceSet::iterator osit = _rootNetOccurrencesSet.begin();
            osit != _rootNetOccurrencesSet.end();
            osit++)
#endif
    for (OccurrenceVector::iterator ovit = netOccurrenceVector.begin();
            ovit != netOccurrenceVector.end();
            ovit++)
    {
        HyperNet hyperNet(*ovit);
        list<unsigned> hyperNetNodes;
        typedef list<Occurrence> OccurrenceList;
        OccurrenceList terminals;
        unsigned nodesToPlace = 0;
        
        //look for pins, pins are on the root net
        Net* rootNet = static_cast<Net*>(ovit->getEntity());
        for_each_pin(pin, rootNet->getPins())
        {
            Path path = ovit->getPath();
            Occurrence pinOccurrence(pin, path);
            terminals.push_back(pinOccurrence);
            end_for;
        }
        
        OccurrenceSet instanceOccurrencesSet; //to detect multi connection of a single instance
        for_each_occurrence(leafPlugOccurrence, hyperNet.getLeafPlugOccurrences())
        {
            Path path = leafPlugOccurrence.getPath();
            Instance* instance = (static_cast<Plug*>(leafPlugOccurrence.getEntity()))->getInstance();
            Occurrence instanceOccurrence(instance, path);
            OccurrenceSet::const_iterator iosit = instanceOccurrencesSet.find(instanceOccurrence);
            if (iosit != instanceOccurrencesSet.end())
                continue;
            instanceOccurrencesSet.insert(instanceOccurrence);
            Occurrences2IdMap::const_iterator imit = instanceOccurrencesMap.find(instanceOccurrence);
            if (imit == instanceOccurrencesMap.end())
            {
                terminals.push_back(instanceOccurrence);
            }
            else
            {
                unsigned nodeId = imit->second;
                hyperNetNodes.push_back(nodeId);
                if (part_vector[nodeId] == -1)
                    ++nodesToPlace;
            }
            end_for;
        }

        unsigned terminalsCount = terminals.size();
        
        if (nodesToPlace > 1 || ((nodesToPlace > 0) && (terminalsCount > 0)))
        {
#ifdef METISSE_DEBUG
            hyperNetIdMap[hyperEdgesCount] = *ovit;
#endif
            ++hyperEdgesCount;
            if (terminalsCount > 0)
            {
                DbU::Unit x = 0;
                DbU::Unit y = 0;
                for (OccurrenceList::iterator olit = terminals.begin();
                        olit != terminals.end();
                        olit++)
                {
                    Point center = olit->getBoundingBox().getCenter();
                    x += center.getX() / terminalsCount;
                    y += center.getY() / terminalsCount;
                }
                Point barycenter(x,y);
                //recherche brute force de la meilleure gcell
                GCell* targetGCell = NULL;
                unsigned targetGCellId = 0;
                DbU::Unit bestDistance = LONG_MAX;
                for (unsigned gcellid = 0; gcellid < _partResultVector.size(); gcellid++)
                {
                    GCell* gcell = _partResultVector[gcellid]->first;
                    DbU::Unit distance = gcell->manhattanDistance(barycenter);
                    if (distance < bestDistance)
                    {
                        bestDistance = distance;
                        targetGCell = gcell;
                        targetGCellId = gcellid;
                    }
                }
                assert(targetGCell);
                //insert fixed point
                hyperNetNodes.push_back(nodeId++);
                vwgts_vector.push_back(0);
                part_vector.push_back(targetGCellId);
                assert(_metis->getGlobalConnectionsWeightRatio());
                if (_metis->getGlobalConnectionsWeightRatio() > 0)
                  hewgts_vector.push_back(_metis->getGlobalConnectionsWeightRatio());
                else
                  hewgts_vector.push_back(1);
            }
            else
            {
                assert(_metis->getGlobalConnectionsWeightRatio());
                if (_metis->getGlobalConnectionsWeightRatio() < 0)
                  hewgts_vector.push_back(-_metis->getGlobalConnectionsWeightRatio());
                else
                  hewgts_vector.push_back(1);
            }
            assert(hyperNetNodes.size() > 1);
            eind_vector.insert(eind_vector.end(), hyperNetNodes.begin(), hyperNetNodes.end());
            eptr_vector.push_back(eptr_vector.back() + hyperNetNodes.size());
        }
    }


    //OK now the real thing ... call hmetis
    
    size_t nvtxs = part_vector.size();
    if (nvtxs < _metis->getNumberOfInstancesStopCriterion())
        throw TooLowNVTXSException(nvtxs);

    size_t nhedges = eptr_vector.size() - 1;
    size_t nparts = _partResultVector.size();

    assert(part_vector.size() == vwgts_vector.size());
    assert (nhedges == (size_t)hyperEdgesCount);
    assert (nhedges == static_cast<size_t>(hewgts_vector.size()));
    
    int* eind = new int[eind_vector.size()];
    for (unsigned id = 0; id < eind_vector.size(); id++)
    {
        eind[id] = eind_vector[id]; 
    }
    
    int* eptr = new int[eptr_vector.size()];
    for (unsigned id = 0; id < eptr_vector.size(); id++)
    {
        eptr[id] = eptr_vector[id]; 
    }

    int* vwgts = new int[vwgts_vector.size()];  
    for (unsigned id = 0; id < vwgts_vector.size(); id++)
    {
        vwgts[id] = vwgts_vector[id];
    }
    
    int* hewgts = new int[hewgts_vector.size()];
    for (unsigned id = 0; id < hewgts_vector.size(); id++)
    {
        hewgts[id] = hewgts_vector[id];
    }

    bool preAssignment = false;    
    int* part = new int[nvtxs];
    for (unsigned id = 0; id < part_vector.size(); id++)
    {
        part[id] = part_vector[id];
        if (part_vector[id] != -1)
            preAssignment = true;
    }
    
    //verification

    for (int id = 0; id < eptr[nhedges] ; id++)
    {
      assert(eind[id] < (int)nvtxs);
    }
    
    for (size_t id = 0; id < nvtxs ; id++)
    {
      assert((part[id] == -1) || (part[id] < (int)nparts));
    }

    _metis->setHMetisOption ( Configuration::HMetisRandom, -1 ); //use random


    if (preAssignment)
      _metis->setHMetisOption ( Configuration::HMetisPreAssign, 1 );
    else
      _metis->setHMetisOption ( Configuration::HMetisPreAssign, 0 );
    
#ifdef METISSE_DEBUG
    VerifyHGraph(nvtxs, nhedges, vwgts, eptr, eind, hewgts, nparts, part, hyperNetIdMap);
#endif
    
    if (_metis->getPartOrKWayHMetis())
    {
      int ubFactor = _metis->getUbFactor();
        if (!ubFactor)
            ubFactor = 2; // the minimal value is 1, but let's try a bit of amplitude.
        HMETIS_PartRecursive(nvtxs, nhedges, vwgts
                            , eptr, eind, hewgts, nparts
                            , ubFactor
                            , _metis->getHMetisOptions()
                            , part, &_edgeCut);
    }
    else 
    {
      int ubFactor = _metis->getUbFactor();
        if (!ubFactor)
            ubFactor = 5; //minimal value
        HMETIS_PartKway(nvtxs, nhedges, vwgts
                       , eptr, eind, hewgts, nparts
                       , ubFactor, _metis->getHMetisOptions(), part, &_edgeCut);
    }

    UpdateSession::open();
    for (Id2OccurrencesMap::const_iterator omit = toPlaceInstanceOccurrencesMap.begin();
            omit != toPlaceInstanceOccurrencesMap.end();
            omit++)
    {
        unsigned instanceId = omit->first;
        unsigned gcellId      = part[instanceId];
        Occurrence instanceOccurrence = omit->second;
        Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
        GCell* gcell = _partResultVector[gcellId]->first;

        _partResultVector[gcellId]->second.insert(instanceOccurrence);
        
        DbU::Unit xPos = gcell->getCenter().getX();
        DbU::Unit yPos = gcell->getCenter().getY();
        Box masterABox = instance->getMasterCell()->getAbutmentBox();
        Transformation instanceTransformation = getTransformation(masterABox
                , xPos - masterABox.getHalfWidth()
                , yPos - masterABox.getHalfHeight()
                , Transformation::Orientation::ID);
        instanceOccurrence.getPath().getTransformation().invert().applyOn(instanceTransformation);
        instance->setTransformation(instanceTransformation);
        instance->setPlacementStatus(Instance::PlacementStatus::PLACED);
    }

    _partResultVector.erase(
            remove_if(_partResultVector.begin(), _partResultVector.end(), removeEmptyPartResult())
            , _partResultVector.end()
            );

    for (PartResultVector::iterator prvit = _partResultVector.begin();
            prvit != _partResultVector.end();
            prvit++)
    {
        (*prvit)->first->setAsPlacementLeaf();
    }
    
    UpdateSession::close();

    delete[] eind;
    delete[] eptr;
    delete[] hewgts;
    delete[] vwgts;
    delete[] part;

    return _edgeCut;
}

}

#endif /* HAVE_HMETIS_LIB */
