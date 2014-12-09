
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

#include <memory>
#include <queue>
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "hurricane/HyperNet.h"
#include "hurricane/Timer.h"
#include "hurricane/DataBase.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/AllianceFramework.h"
#include "nimbus/GCell.h"
#include "nimbus/NimbusEngine.h"

#include "mauka/Surface.h"
#include "mauka/Row.h"
#include "mauka/SimAnnealingPlacer.h"
#include "mauka/BBPlacer.h"
#include "mauka/MaukaEngine.h"

namespace Mauka {

using std::auto_ptr;
using Hurricane::ForEachIterator;
using Hurricane::Warning;
using Hurricane::Plug;
using Hurricane::Path;
using Hurricane::OccurrenceLocator;
using Hurricane::PlugLocator;
using Hurricane::HyperNet;
using Hurricane::Timer;
using Hurricane::UpdateSession;
using namespace CRL;
using namespace Nimbus;

Name  MaukaEngine::_toolName = "Mauka";

MaukaEngine::MaukaEngine(Cell* cell)
// *********************************
    : Inherit(cell)
    , _configuration(new Configuration())
    , _feedCells(this)
    , _instanceOccurrencesVector()
    , _instanceOccurrencesMap()
    , _instanceWidths()
    , _instanceNets()
    , _nets()
    , _netInstances()
    , _netInitX()
    , _netInitY()
    , _hasInitX()
    , _hasInitY()
    , _surface(NULL)
    , _simAnnealingPlacer(NULL)
    , _bbPlacer(NULL)
{}

MaukaEngine* MaukaEngine::create(Cell* cell, Box placementbox)
// ***********************************************************
{
    MaukaEngine* mauka = new MaukaEngine(cell);

    mauka->_maukaPostCreate(placementbox);

    return mauka;
}

void MaukaEngine::_maukaPostCreate(Box& placementbox)
// **************************************************
{
    Inherit::_postCreate();

  // Ugly. Direct uses of Alliance Framework.
    addFeed ( AllianceFramework::get()->getCell("tie_x0"   ,Catalog::State::Views) );
    addFeed ( AllianceFramework::get()->getCell("rowend_x0",Catalog::State::Views) );

    Construct();
    //_simAnnealingPlacer = new SimAnnealingPlacer(this, 1, 0.0, 0.0);
    _simAnnealingPlacer = new SimAnnealingPlacer(this);
    //_displaySlot = DisplaySlot::create(getCell(),,139,0,139);
    _surface = Surface::create(this, placementbox);
    _simAnnealingPlacer->init();
    //Plot();
}

#if 0
void MaukaEngine::ReInit()
// *******************
{
    Box placementBox = _surface->getBox();
    _surface->destroy();
    _simAnnealingPlacer->destroy();
    _surface = Surface::create(this, placementBox);
    _simAnnealingPlacer = SimAnnealingPlacer::create(this);
}
#endif

void MaukaEngine::Run()
{

    Timer timer;
    timer.start();
    while ( _simAnnealingPlacer->Iterate() );

    timer.stop();
    _simAnnealingPlacer->DisplayResults();

  //if ( doPlotBins() ) PlotBinsStats();

    cmess1 << "  o  Simulated annealing took " << Timer::getStringTime(timer.getCombTime()) 
           << " [+" << Timer::getStringMemory(timer.getIncrease()) << "]." << endl;
    cmess1 << "     (raw measurements : " << timer.getCombTime()
           << "s [+" << (timer.getIncrease()>>10) <<  "Ko/"
           << (timer.getMemorySize()>>10) << "Ko])" << endl;

    //Plot();
    _bbPlacer = new BBPlacer(this);
    _bbPlacer->Run();
    _bbPlacer->Save();
    if ( doInsertFeeds() ) insertFeeds ();
    //Plot();
}

namespace {
// void VerifyPathCellBox(const Occurrence& occurrence)
// //On occurrence Path set all cells Abox to (0,0,0,0) if Box is empty
// //This avoids bad Transfos on Path
// {
//     Path path = occurrence.getPath();
//     while (!path.isEmpty())
//     {
//         Instance* instance = path.getHeadInstance();
//         Cell* model = instance->getMasterCell();
//         if (model->getAbutmentBox().isEmpty())
//         {
//             model->setAbutmentBox(Box(Point(0,0), Point(0,0)));
//         }
//         path = path.getTailPath();
//     }
// }
}

void MaukaEngine::Construct()
// **************************
{
    typedef map<Occurrence, unsigned> InstanceOccurrenceMap;
    typedef map<Instance*,Occurrence> OccurrencesLUT;
    typedef map<Net*, unsigned> NetMap;
    OccurrencesLUT occurencesLUT;
    NetMap netMap;
    unsigned instanceId = 0;
    unsigned netId = 0;
    
    DbU::DbU::Unit standardCellHeight = 0;
    
    UpdateSession::open ();

    forEach ( Occurrence, ioccurrence, getCell()->getNonLeafInstanceOccurrences() )
    {
      Box topLevelAbutmentBox = getCell()->getAbutmentBox();

      Instance* instance = static_cast<Instance*>((*ioccurrence).getEntity());
      if ( instance->isUnplaced() ) {
        Cell* masterCell = instance->getMasterCell();
        if ( masterCell->getAbutmentBox().isEmpty() )
          masterCell->setAbutmentBox ( topLevelAbutmentBox );

        instance->setTransformation  ( Transformation() );
        instance->setPlacementStatus ( Instance::PlacementStatus::PLACED );
      }
    }

    UpdateSession::close ();
    
    forEach ( Occurrence, ioccurrence, getCell()->getLeafInstanceOccurrences() )
    {
        Instance* instance = static_cast<Instance*>((*ioccurrence).getEntity());
      //cerr << (*ioccurrence).getPath() << ":"
      //     << instance << " " << (int)instance->getPlacementStatus().getCode() << endl;

        if ( not instance->isFixed() and instance->isTerminal() )
        {
            //cerr << "unplaced " << (*ioccurrence) << (*ioccurrence).getBoundingBox() << endl;
            Cell* model = instance->getMasterCell();
            if (model->getAbutmentBox().isEmpty()) {
              throw Error("Unplaced terminal instance <%s> of <%s> with an empty model abutment box.\n"
                          "(hint: the vst model may be missing)" 
                         ,getString(instance->getName()).c_str()
                         ,getString(model->getName()).c_str()
                         );
            }

            DbU::Unit insWidth = model->getAbutmentBox().getWidth();
            DbU::Unit insHeight = model->getAbutmentBox().getHeight();
            if (standardCellHeight == 0) {
              cmess1 << "     - Slice height deduced from: <" << model << ">." << endl;
              standardCellHeight = insHeight;
            }
            else if (insHeight != standardCellHeight)
            {
                throw Error("All non-standard instances : "
                        + getString(instance->getName())
                        + " must be placed "
                        + getString(standardCellHeight) + " "
                        + DbU::getValueString(standardCellHeight) );
            }
            _instanceOccurrencesVector.push_back(*ioccurrence);
            //VerifyPathCellBox(*ioccurrence);
            _instanceWidths.push_back(insWidth);
            OccurrencesLUT::iterator duplicate = occurencesLUT.find(instance);
            if (duplicate != occurencesLUT.end())
            {
            //cerr << "Unplaced Instance : "   << *duplicate << endl;
            //cerr << "Unplaced Occurrence : " << (*ioccurrence) << endl;
            //cerr << (*duplicate)->getPlacementStatus() << endl;
                throw Error("Each unplaced instance must have one occurrence only.\n"
                            "Model <%s> is intanciated as:\n<b>.  %s\n.  %s</b>\n(at least)."
                           ,getString(instance->getMasterCell()->getName()).c_str()
                           ,(*ioccurrence).getCompactString().c_str()
                           ,((*duplicate).second).getCompactString().c_str()
                           );
            }
            _instanceOccurrencesMap[*ioccurrence] = instanceId++;
            occurencesLUT.insert(make_pair(instance,*ioccurrence));
            _instanceNets.push_back(UVector());
        }
    }

    if (_instanceOccurrencesVector.size() == 0)
        throw Error("No Instance to place...");

    typedef set<Net*> NetSet;
    NetSet treatedNets;

    for (InstanceOccurrencesVector::const_iterator iovit = _instanceOccurrencesVector.begin();
            iovit != _instanceOccurrencesVector.end();
            iovit++)
    {
        Instance* instance = static_cast<Instance*>(iovit->getEntity());
        for_each_plug(plug, instance->getConnectedPlugs())
        {
            Net* net = plug->getNet();
            if (net->isGlobal())
                continue;
            typedef list<Occurrence> InstanceOccurrenceList;
            InstanceOccurrenceList instanceOccurrenceList;
            Occurrence rootNetOccurrence = getHyperNetRootNetOccurrence(Occurrence(net, iovit->getPath()));
            Net* rootNet = static_cast<Net*>(rootNetOccurrence.getEntity());
            NetSet::iterator snit = treatedNets.find(rootNet);
            if (snit != treatedNets.end())
                continue;
            treatedNets.insert(rootNet);
            HyperNet hyperNet(rootNetOccurrence);
            typedef list<Point> PointList;
            PointList pointList;
            for_each_occurrence(occurrence, hyperNet.getNetOccurrences())
            {
                Instance* instance = occurrence.getPath().getTailInstance();
                if (instance && instance->isLeaf())
                {
                    Occurrence instanceOccurrence = Occurrence(instance, occurrence.getPath().getHeadPath());
                    if (instance->isFixed())
                        pointList.push_back(instanceOccurrence.getBoundingBox().getCenter());
                    else
                        instanceOccurrenceList.push_back(instanceOccurrence);
                }
                end_for;
            }
            unsigned pointListSize = pointList.size();
            Point fixedPoint(0,0);
            if (pointListSize > 0)
            {
                for (PointList::iterator plit = pointList.begin();
                        plit != pointList.end();
                        plit++)
                {
                    fixedPoint.setX(fixedPoint.getX() + plit->getX() / pointListSize);
                    fixedPoint.setY(fixedPoint.getY() + plit->getY() / pointListSize); 
                }
            }
            if ((instanceOccurrenceList.size() < 2) && pointListSize == 0)
                continue;
            _netInstances.push_back(UVector());
            _nets.push_back(net);
            if (pointListSize > 0)
            {
                _netInitX.push_back(fixedPoint.getX());
                _netInitY.push_back(fixedPoint.getY());
                _hasInitX.push_back(true);
                _hasInitY.push_back(true);
            }
            else
            {
                _netInitX.push_back(0);
                _netInitY.push_back(0);
                _hasInitX.push_back(false);
                _hasInitY.push_back(false);
            }

            netMap[net] = netId;
            for (InstanceOccurrenceList::iterator iolit = instanceOccurrenceList.begin();
                    iolit != instanceOccurrenceList.end();
                    iolit++)
            {
                InstanceOccurrenceMap::const_iterator iomit = _instanceOccurrencesMap.find(*iolit);
                if (iomit == _instanceOccurrencesMap.end())
                {
                    cerr << "cannot find " << iomit->first << endl;
                    throw Error("Error in netsInstances construction in SimAnnealingPlacer");
                }
                _netInstances.back().push_back(iomit->second);
                _instanceNets[iomit->second].push_back(netId);
            }
            ++netId;
            end_for;
        }
        
    }

#if DEBUG

    //debug ... display netlist
    for (unsigned instanceId = 0; instanceId < _instanceOccurrencesVector.size(); instanceId++)
    {
        cerr << "instance  " << _instanceOccurrencesVector[instanceId] << endl;
        cerr << "is connected to " << endl;
        for (UVector::const_iterator uvit = _instanceNets[instanceId].begin();
                uvit != _instanceNets[instanceId].end();
                uvit++)
        {
            unsigned netid = *uvit;
            cerr << _nets[netid] << endl;
            for (UVector::const_iterator nuvit = _netInstances[netid].begin();
                    nuvit != _netInstances[netid].end();
                    nuvit++)
            {
                cerr << _instanceOccurrencesVector[*nuvit] << endl;
            }
            cerr << endl;
        }
        cerr << endl;
    }
#endif


}

bool MaukaEngine::Iterate()
// ******************
{
    bool canContinue = _simAnnealingPlacer->Iterate();
    Save();
    return canContinue;
}

void MaukaEngine::_preDestroy()
// *********************
{
    _surface->destroy();
    if (_simAnnealingPlacer)
        delete _simAnnealingPlacer;
    if (_bbPlacer)
        delete _bbPlacer;
    delete _configuration;
    Inherit::_preDestroy();
}

Record* MaukaEngine::_getRecord() const
// ************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Surface", _surface));
    }
    return record;
}

const Name& MaukaEngine::staticGetName()
// *************************************
{
    return _toolName;
}

const Name& MaukaEngine::getName() const
// *******************************
{
    return _toolName;
}

void MaukaEngine::Save() const
// *********************
{
    if (_bbPlacer)
        _bbPlacer->Save();
    else if (_simAnnealingPlacer)
        _simAnnealingPlacer->Save();
}

MaukaEngine* MaukaEngine::get(Cell* cell)
// ********************************************
{
  return static_cast<MaukaEngine*>(ToolEngine::get(cell,staticGetName()));
}

namespace {
    class TestSubRow : public Box {
        public: DbU::DbU::Unit _size; // sum of the contained instances width
        public: TestSubRow(const Box& box): Box(box), _size(0) {}
    };

    typedef list<TestSubRow*> TestSubRowsList;
    typedef vector<Occurrence> InstanceOccurrencesVector;
    typedef list<Occurrence> InstanceOccurrencesList;

    void DestroyTestSubRows(TestSubRowsList& testsubrowslist)
    {
        for (TestSubRowsList::iterator tsrlit = testsubrowslist.begin();
                tsrlit != testsubrowslist.end();
                tsrlit++)
        {
            delete *tsrlit;
        }
        testsubrowslist.clear();
    }

    struct sortInstanceOccurrencesByWidth
    {
        bool operator()(Occurrence occurrence1, Occurrence occurrence2)
        {
            Instance* instance1 = static_cast<Instance*>(occurrence1.getEntity());
            Instance* instance2 = static_cast<Instance*>(occurrence2.getEntity());
            return instance1->getAbutmentBox().getWidth() > instance2->getAbutmentBox().getWidth();
        }
    };

    bool TryDisplayInstancesInSubRows(
            InstanceOccurrencesVector& instanceoccurrencesvector, TestSubRowsList& testsubrowslist)
    {
        // Try Insert instances in subrows
        sort(instanceoccurrencesvector.begin(), instanceoccurrencesvector.end(), sortInstanceOccurrencesByWidth());
        
        TestSubRowsList::iterator tsrlit = testsubrowslist.begin();
        InstanceOccurrencesVector::const_iterator insIterator = instanceoccurrencesvector.begin(); 
        InstanceOccurrencesVector::const_iterator lastLoopInsertedInsIterator = insIterator; 

        while(insIterator != instanceoccurrencesvector.end())
        {
            if (tsrlit == testsubrowslist.end())
            {
                tsrlit = testsubrowslist.begin();
                if (lastLoopInsertedInsIterator != insIterator)
                    lastLoopInsertedInsIterator = insIterator;
                else
                {
                    return false;
                }
            }
            TestSubRow* testSubRow = *tsrlit;
            Instance* instance = static_cast<Instance*>(insIterator->getEntity());
            Cell* model = instance->getMasterCell();
            DbU::DbU::Unit insWidth = model->getAbutmentBox().getWidth();
            if (insWidth <= testSubRow->getWidth() - testSubRow->_size)
            {
                testSubRow->_size += insWidth;
                ++insIterator;
            }
            ++tsrlit;
        }
        return true;
    }
}

bool TestMaukaConstruction(Cell* cell, GCell* gcell)
// *************************************************
{
    auto_ptr<Configuration> configuration ( new Configuration() );

    DbU::Unit pitch       = configuration->getPitch(); 
    DbU::Unit sliceHeight = configuration->getSliceHeight();

    const Box& box = gcell->getBox();

    if (box.isEmpty() || box.isPonctual() ||box.isFlat())
        throw Error("Wrong Box for GCell");
    if (box.getHeight() % sliceHeight)
        throw Error("Box Height must be a multiple of Slice Height");

    InstanceOccurrencesList fixedInstanceOccurrenceList;
    InstanceOccurrencesVector toPlaceInstanceOccurrencesVector;
    //search for preplaced leaf instances
    DbU::DbU::Unit instanceToPlaceWidthMax = 0;
    for_each_occurrence(occurrence, cell->getLeafInstanceOccurrencesUnder(gcell->getBox()))
    {
        Instance* instance = static_cast<Instance*>(occurrence.getEntity());
        if (instance->isFixed())
            fixedInstanceOccurrenceList.push_back(occurrence);
        else
        {
            Cell* model = instance->getMasterCell();
            DbU::DbU::Unit insWidth = model->getAbutmentBox().getWidth();
            toPlaceInstanceOccurrencesVector.push_back(occurrence);
            if (instanceToPlaceWidthMax > insWidth)
                instanceToPlaceWidthMax = insWidth;
        }
        end_for;
    }
    
    DbU::DbU::Unit binWidthMax = DbU::lambda((unsigned)(
                2.0 * DbU::getLambda(instanceToPlaceWidthMax) / DbU::getLambda(pitch)) * DbU::getLambda(pitch)); 
    DbU::DbU::Unit binWidthMin = DbU::lambda((unsigned)(
                DbU::getLambda(binWidthMax) / (DbU::getLambda(pitch) * 2)) * DbU::getLambda(pitch));

    TestSubRowsList testSubRowsList;
    
    if (fixedInstanceOccurrenceList.size() != 0)
    {
        int width  = (unsigned)(gcell->getWidth() / pitch); //number of x pitch
        int height = (unsigned)(gcell->getHeight() / sliceHeight); //number of y slices


        MaukaEngine::PrePlaceTab prePlaceTab(height, MaukaEngine::PrePlaceRow(width, false));

        for (InstanceOccurrencesList::iterator iolit = fixedInstanceOccurrenceList.begin();
                iolit != fixedInstanceOccurrenceList.end();
                iolit++)
        {
            Instance* instance = static_cast<Instance*>(iolit->getEntity());
            Box instanceAbutmentBox = instance->getAbutmentBox();
            iolit->getPath().getTransformation().applyOn(instanceAbutmentBox);
            
            Box preplacedBox = gcell->getIntersection(instanceAbutmentBox);
            
            DbU::DbU::Unit insWidth  = preplacedBox.getWidth();
            DbU::DbU::Unit insHeight = preplacedBox.getHeight();

            int insPitchWidth  = (int)(insWidth / pitch);         // largeur ramene au pitch
            int insSliceHeight = (int)(insHeight / sliceHeight); // hauteur ramene a la hauteur du slice
            
            int ypos = (int)((preplacedBox.getYMin() - gcell->getYMin()) / sliceHeight); // position en y ramene au slice
            int xpos = (int)((preplacedBox.getXMin() - gcell->getXMin()) / pitch);	// position en x ramene au pitch
            for (int yit = ypos; yit < ypos + insSliceHeight; yit++)
            {
                for (int xit = xpos; xit < xpos + insPitchWidth; xit++)
                {
                    if (   (xit > width - 1) || (yit > height - 1)
                            || (xit < 0    ) || (yit < 0     ) )
                    {
                        cerr << " o ERROR : " << *iolit
                            << " out of the abutment box" << endl;
                        exit(1);
                    }
                    if (prePlaceTab[yit][xit] == false)
                    {
                        prePlaceTab[yit][xit] = true;
                    }
                    else
                    {
                        cerr << " o ERROR : " << *iolit
                            << " badly placed .... There is already an instance at its position ...."
                            << endl;
                        exit (1);
                    }
                }
            }
        }
        for (int y = 0; y < (int)prePlaceTab.size(); y++)
        {
            int x = 0;
            while (x < (int)prePlaceTab[y].size())
            {
                while ((x < (int)prePlaceTab[y].size()) && (prePlaceTab[y][x] == true))
                    ++x;
                DbU::DbU::Unit subRowXMin = gcell->getXMin() + x * pitch;

                if (x >= (int)prePlaceTab[y].size())
                    break;

                while ((x < (int)prePlaceTab[y].size()) && (prePlaceTab[y][x] == false))
                    ++x;
                DbU::DbU::Unit subRowXMax = gcell->getXMin() + x * pitch;
                if (subRowXMax - subRowXMin > binWidthMin)
                {
                    testSubRowsList.push_back(new TestSubRow(
                                Box(subRowXMin, gcell->getYMin() + y * sliceHeight
                                    , subRowXMax, gcell->getYMin() + (y+1) * sliceHeight)
                                ));
                }
                
            }
        }
    }
    else
    {
        for (DbU::DbU::Unit ymin = gcell->getYMin(); ymin <= gcell->getYMax() - sliceHeight; ymin += sliceHeight)
        {
            testSubRowsList.push_back(new TestSubRow(
                    Box(gcell->getXMin(), ymin, gcell->getXMax(), ymin + sliceHeight)
                    ));
        }
    }
    
    bool tryInstanceInsertion = TryDisplayInstancesInSubRows(toPlaceInstanceOccurrencesVector, testSubRowsList);
    DestroyTestSubRows(testSubRowsList);
    return tryInstanceInsertion;
}


void MaukaEngine::PlotBinsStats() const
// ******************************
{
    ofstream out("binsstats.gpl");

    out << "set noxtics" << endl << "set noytics" << endl
	<< "set noborder" << endl << "set nokey" << endl
        << "set title '" << getCell()->getName() << "'" << endl
	<< "#set terminal postscript eps color solid" << endl
        << "#set output 'binsstats.ps'" << endl;

    _surface->PlotBinsStats(out);
}

void MaukaEngine::Plot() const
// *********************
{
    static unsigned count = 0;
    string cellNameString = getString(getCell()->getName())
        + "_" + getString(count) +  ".gpl";
    ++count;
    ofstream out(cellNameString.c_str());
    
    out << "set noxtics" << endl << "set noytics" << endl
	<< "set noborder" << endl << "set nokey" << endl
        << "set title '" << cellNameString << "'" << endl
	<< "#set terminal postscript eps color solid" << endl << "#set output '"
	<< cellNameString << ".ps'" << endl;

    Box boundingBox = _surface->getBox();
    boundingBox.merge(PlotFixedPointsLabels(out));
    
    out << "set xrange[" << boundingBox.getXMin()
        << ":" << boundingBox.getXMax() << "]" << endl
	<< "set yrange[" << boundingBox.getYMin()
        << ":" << boundingBox.getYMax() << "]" << endl;
    
    out << "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3, '-' w l 4" << endl;
//	<< "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3, '-' w l 4" << endl;

    _surface->Plot(out);
    if (_bbPlacer)
        _bbPlacer->Plot(out);
    else
        _simAnnealingPlacer->Plot(out);

    out << "pause -1 'press any key'" << endl;
}

Box MaukaEngine::PlotFixedPointsLabels(ofstream& out) const
// ****************************************************
{
    Box boundingBox;

    out << "#FixedPoints" << endl;
    for (unsigned i = 0; i < _netInstances.size(); i++)
    {
        if (_hasInitX[i])
        {
            
            out << "set label \""  
                << getString(_nets[i]->getName()) << "\" at "
                << _netInitX[i] << "," << _netInitY[i] << " center"
                << endl;
        }
        boundingBox.merge(_netInitX[i], _netInitY[i]);
    }
    return boundingBox;
}

unsigned MaukaEngine::getRandomInstanceId() const {
    unsigned instanceId = (unsigned)((double)_instanceOccurrencesVector.size() * rand() / (RAND_MAX + 1.0));
    return instanceId; 
}


  void  MaukaEngine::regroupOverloadedGCells ( Cell* cell )
  {
    NimbusEngine* nimbus = NimbusEngine::get ( cell );
    if ( nimbus == NULL )
      throw Error ("Mauka::regroupOverloadedGCells(): Nimbus doesn't exists on <%s>"
                  ,getString(cell->getName()).c_str());

    queue<GCell*>  toProcess;
    set  <GCell*>  containers;

    forEach ( GCell*, igcell, nimbus->getPlacementLeaves() ) {
      toProcess.push ( *igcell );
    }

    while ( not toProcess.empty() ) {
      GCell* container = toProcess.front();
      GCell* parent    = container->getContainer();
      toProcess.pop ();

      if ( containers.find(parent) != containers.end() ) continue;
      containers.insert ( container );

      if ( not TestMaukaConstruction(cell,container) ) {
        cerr << Warning("Not enough margin on %s",getString(container).c_str()) << endl;

        if ( parent == NULL )
          throw Error("Not enough margin on the whole Cell");

        parent->setAsPlacementLeaf ();
        toProcess.push ( parent );

        cmess2 << "     - Sets as placement leaf " << parent << endl;
      }
    }

    nimbus->regroup ();
  }
    

}  // End of Mauka namespace.
