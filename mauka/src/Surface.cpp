
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

#include "hurricane/Warning.h"
#include "hurricane/Cell.h"
#include "nimbus/NimbusEngine.h"

#include "mauka/MaukaEngine.h"
#include "mauka/SimAnnealingPlacer.h"
#include "mauka/SubRow.h"
#include "mauka/Row.h"
#include "mauka/Surface.h"

namespace {

using namespace std;
using namespace Hurricane;
using namespace Mauka;

void VerifyInstanceQuadtreeInsertion(Cell *cell)
{
    cerr << "VerifyInstanceQuadtreeInsertion" << endl;
    for_each_instance(instance, cell->getInstances())
    {
        if (!instance->isMaterialized())
        {
            Cell* masterCell = instance->getMasterCell();
            cerr << tab << instance << " of " <<  masterCell << " is not materialized..."  << endl;
            tab++;
            VerifyInstanceQuadtreeInsertion(masterCell);
            tab--;
        }
        end_for;
    }
}

void DisplayInstanceOccurrence(Occurrence& instanceOccurrence)
{
    cerr << instanceOccurrence << " " <<  instanceOccurrence.getBoundingBox() << endl;
    Instance* instance = static_cast<Instance*>(instanceOccurrence.getEntity());
    Path path = instanceOccurrence.getPath();
    cerr << path.getTransformation() << endl;
    cerr << instance->getTransformation() << endl;
    while (!path.isEmpty())
    {
        cerr << path.getTailInstance() << " " << path.getTailInstance()->getTransformation() << endl;
        cerr << path.getTailInstance()->getMasterCell() << endl;
        cerr << path.getTailInstance()->getMasterCell()->getAbutmentBox() << endl;
        cerr << path.getTailInstance()->getMasterCell()->getBoundingBox() << endl;
        cerr << path.getTailInstance()->getAbutmentBox() << endl;
        cerr << path.getTailInstance()->getBoundingBox() << endl;
        path = path.getHeadPath();
    }
}

void DisplayNonLeafInstances(Cell* cell, Box area)
{
    ltrace(100) << "display of "  << cell << " in " << area <<  endl;
    for_each_instance(instance, cell->getNonLeafInstancesUnder(area))
    {
        ltrace(100) << instance <<  " " << instance->getBoundingBox() << endl;
        Cell* masterCell = instance->getMasterCell();
        ltracein(20);
        DisplayNonLeafInstances(masterCell, area);
        ltraceout(20);
        end_for;
    }
}

} // End of anonymous namespace.

namespace Mauka {

using Nimbus::NimbusEngine;
using Nimbus::GCellLocator;
    
Surface::Surface(MaukaEngine* mauka, const Box& placementbox) :
    Inherit(mauka->getCell(), placementbox),
    _mauka(mauka),
    _rowVector(),
    _rowYMax(),
    _rowYMinInv(),
    _rowZeroOrientation(false),
    _margin(0.0),
    _binWidthMax(0),
    _binWidthMin(0),
    _searchWidth(0),
    _searchHeight(0)
{}

Surface* Surface::create(MaukaEngine* mauka, const Box& placementbox) {
    if (!mauka) {
        throw Error("Can't create " + _TName("Surface") + " : empty mauka");
    }

    if (!mauka->getCell()) {
        throw Error("Can't create " + _TName("Surface") + " : empty cell on mauka");
    }
    
    Surface* surface = new Surface(mauka, placementbox);
    
    surface->_postCreate();

    return surface;
}

namespace {

void PlacementVerification(Cell* cell, Box& box) {
    set<Occurrence> occurrenceSet;
    for_each_occurrence(occurrence, cell->getLeafInstanceOccurrencesUnder(box)) {
        occurrenceSet.insert(occurrence);
        end_for;
    }
    // for_each_occurrence(occurrence, cell->getLeafInstanceOccurrences()) {
    //     if (occurrenceSet.find(occurrence) == occurrenceSet.end()) {
    //         cerr << occurrence << ":" << occurrence.getBoundingBox() << endl;
    //         throw Error("occurrence is badly placed");
    //     }
    //     end_for;
    // }
}

typedef list<Occurrence> InstanceOccurrencesList;

class PlacementProblem {
    public: GCell* _gcell;
    public: InstanceOccurrencesList _fixedInstanceOccurrenceList;
    public: MaukaEngine::UVector _toPlaceInstanceOccurrencesUVector;
    public: Surface::SubRowList _subRowList;
    PlacementProblem(GCell* gcell)
        : _gcell(gcell)
        , _fixedInstanceOccurrenceList()
        , _toPlaceInstanceOccurrencesUVector()
        , _subRowList()
        {}
};

typedef list<PlacementProblem*> PlacementProblemList;

}


  void Surface::_postCreate ()
  {
    typedef set<Occurrence> OccurrenceSet;

    Inherit::_postCreate();

    bool          partitionned = false;
    NimbusEngine* nimbus       = NULL;
    if (_box.isEmpty()) {
      nimbus = NimbusEngine::get ( getCell() );
      if (nimbus == NULL) {
        throw Error("Can't create Mauka::Surface, no Box and no Nimbus");
      }
      _box = nimbus->getGrid()->getRoot()->getBox();
      partitionned = (nimbus->getDepth() > 1);

      if ( partitionned )
        cmess2 << "     - Design is partionned (depth:" << nimbus->getDepth() << ")" << endl;
    }

    PlacementVerification ( getCell(), _box );

    DbU::Unit sliceHeight = _mauka->getSliceHeight();
    DbU::Unit pitch       = _mauka->getPitch(); 

    if ( _box.isEmpty() or _box.isPonctual() or _box.isFlat())
      throw Error("Wrong Box for Area: %s",getString(_box).c_str());

    if ( getHeight() % sliceHeight )
        throw Error("Box Height %s must be a multiple of Slice Height %s"
                   ,DbU::getValueString(getHeight()).c_str()
                   ,DbU::getValueString(sliceHeight).c_str());
    
    DbU::Unit instanceToPlaceWidthMax = 0;
    double    instanceToPlaceWidthSum = 0.0;
    
    for ( unsigned int id = 0; id < _mauka->_instanceWidths.size(); ++id ) {
      DbU::Unit instanceWidth = _mauka->_instanceWidths[id];
      instanceToPlaceWidthSum += (double)instanceWidth;

      if ( instanceWidth % pitch )
        throw Error("Width of %s (%s) is not a multiple of pitch (%s)."
                   ,getString(_mauka->_instanceOccurrencesVector[id]).c_str()
                   ,DbU::getValueString(instanceWidth).c_str()
                   ,DbU::getValueString(pitch).c_str());

      if (instanceWidth > instanceToPlaceWidthMax)
        instanceToPlaceWidthMax = instanceWidth;
    }
    
    // _binWidthMax = DbU::lambda
    //   ((unsigned)( 3.0 * DbU::getLambda(instanceToPlaceWidthMax) / DbU::getLambda(pitch))
    //                    * DbU::getLambda(pitch)); 
    // _binWidthMin = DbU::lambda
    //   ((unsigned)(DbU::getLambda(_binWidthMax) / (DbU::getLambda(pitch) * 2)) * DbU::getLambda(pitch));

    _binWidthMax = 2 * (instanceToPlaceWidthMax / pitch) * pitch; 
    _binWidthMin = (_binWidthMax / (pitch * 2)) * pitch;

    // cerr << "_binWidthMax:" << DbU::getValueString(_binWidthMax) << endl;
    // cerr << "_binWidthMin:" << DbU::getValueString(_binWidthMin) << endl;

    double               surfaceTotalWidth = 0.0;
    PlacementProblemList placementProblemList;
    OccurrenceSet        verifyInstanceOccurrencesSet;

    if ( nimbus ) {
      forEach ( GCell*, igcell, nimbus->getPlacementLeaves() ) {
        PlacementProblem* placementProblem = new PlacementProblem(*igcell);
        placementProblemList.push_back(placementProblem);
            
        InstanceOccurrencesList toPlaceInstanceOccurrencesList;
      // Search for preplaced leaf instances
        forEach ( Occurrence
                , ioccurrence, _mauka->getCell()->getLeafInstanceOccurrencesUnder(igcell->getBox()) ) {
          Instance* instance = static_cast<Instance*>((*ioccurrence).getEntity());

          if ( instance->isFixed() ) {
            placementProblem->_fixedInstanceOccurrenceList.push_back(*ioccurrence);
          } else {
            MaukaEngine::InstanceOccurrencesMap::const_iterator 
              iomit = _mauka->_instanceOccurrencesMap.find(*ioccurrence);

            if ( iomit == _mauka->_instanceOccurrencesMap.end() )
              throw Error("Instance occurrence unexpectedly appeared:\n"
                          "        %s",getString(*ioccurrence).c_str());

            placementProblem->_toPlaceInstanceOccurrencesUVector.push_back(iomit->second);
            verifyInstanceOccurrencesSet.insert(*ioccurrence);
          }
        }

      // Special case: no Nimbus run, Instances are *not* in the quadtree yet.
        if ( not partitionned ) {
          forEach(Occurrence, ioccurrence, _mauka->getCell()->getLeafInstanceOccurrences() ) {
            Instance* instance = static_cast<Instance*>((*ioccurrence).getEntity());
            if ( instance->isFixed() ) continue;

            MaukaEngine::InstanceOccurrencesMap::const_iterator iomit
              = _mauka->_instanceOccurrencesMap.find(*ioccurrence);

            if (iomit == _mauka->_instanceOccurrencesMap.end())
              throw Error("Instance occurrence unexpectedly appeared:\n"
                          "        %s",getString(*ioccurrence).c_str());

            placementProblem->_toPlaceInstanceOccurrencesUVector.push_back(iomit->second);
            verifyInstanceOccurrencesSet.insert(*ioccurrence);
          }
        }

        DbU::Unit searchWidth = DbU::lambda(_mauka->getSearchRatio() * DbU::getLambda(igcell->getWidth()));
        if (_searchWidth < searchWidth) _searchWidth = searchWidth;

        DbU::Unit searchHeight = DbU::lambda(_mauka->getSearchRatio() * DbU::getLambda(igcell->getHeight()));
        if (_searchHeight < searchHeight) _searchHeight = searchHeight;
      }

      for ( PlacementProblemList::iterator pplit = placementProblemList.begin();
            pplit != placementProblemList.end(); pplit++) {
        PlacementProblem* placementProblem = *pplit;
        GCell*            gcell            = placementProblem->_gcell;

        if ( placementProblem->_fixedInstanceOccurrenceList.size() != 0 ) {
          bool rowZeroOrientation = false;
          int  width              = (unsigned)(gcell->getWidth () / pitch);       // Number of X pitch
          int  height             = (unsigned)(gcell->getHeight() / sliceHeight); // Number of Y slices


          MaukaEngine::PrePlaceTab prePlaceTab(height, MaukaEngine::PrePlaceRow(width, false));

          InstanceOccurrencesList::const_iterator iolit
            = placementProblem->_fixedInstanceOccurrenceList.begin();

        // Find the orientation of the first row.
          Instance*      refInstance       = static_cast<Instance*>(iolit->getEntity());
          Box            refAbutmentBox    = refInstance->getAbutmentBox();
          Transformation refTransformation = refInstance->getTransformation();

          iolit->getPath().getTransformation().applyOn(refAbutmentBox);
          iolit->getPath().getTransformation().applyOn(refTransformation);
                
          Box preplacedBox = gcell->getIntersection(refAbutmentBox);

        // Position en y ramene au slice
          int refInsYPos = (int)((preplacedBox.getYMin() - gcell->getYMin()) / sliceHeight); 
        // Hauteur ramene a la hauteur du slice
          int                         refInsSliceHeight = (int)(preplacedBox.getHeight() / sliceHeight); 
          Transformation::Orientation orientation       = refTransformation.getOrientation();

          if ( refInsYPos % 2 == 0 ) {
            if (  (refInsSliceHeight % 2 == 0)
               or (orientation == Transformation::Orientation::ID)
               or (orientation == Transformation::Orientation::MX))
              rowZeroOrientation = false;
            else
              rowZeroOrientation = true;
          } else {
            if (  (refInsSliceHeight % 2 == 0)
               or (orientation == Transformation::Orientation::ID)
               or (orientation == Transformation::Orientation::MX))
              rowZeroOrientation = true;
            else
              rowZeroOrientation = false;
          }
                        
        // Tests for each Instance.
          for ( InstanceOccurrencesList::iterator iolit = placementProblem->_fixedInstanceOccurrenceList.begin();
                iolit != placementProblem->_fixedInstanceOccurrenceList.end(); iolit++ ) {
            Instance* instance            = static_cast<Instance*>(iolit->getEntity());
            Box       instanceAbutmentBox = instance->getAbutmentBox();

            iolit->getPath().getTransformation().applyOn(instanceAbutmentBox);
                    
            Box       preplacedBox   = gcell->getIntersection(instanceAbutmentBox);
            DbU::Unit insWidth       = preplacedBox.getWidth();
            DbU::Unit insHeight      = preplacedBox.getHeight();

            int insPitchWidth  = (int)(insWidth  / pitch);       // Largeur ramene au pitch
            int insSliceHeight = (int)(insHeight / sliceHeight); // Hauteur ramene a la hauteur du slice

            int ypos = (int)((preplacedBox.getYMin() - gcell->getYMin()) / sliceHeight); // Position en Y ramene au slice
            int xpos = (int)((preplacedBox.getXMin() - gcell->getXMin()) / pitch);       // Position en X ramene au pitch

            for ( int yit = ypos; yit < ypos + insSliceHeight; ++yit ) {
              for ( int xit = xpos; xit < xpos + insPitchWidth; ++xit ) {
                if ( (xit > width - 1) or (yit > height - 1) or (xit < 0) or (yit < 0) )
                  throw Error("%s is out of the abutment box"
                             ,getString(*iolit).c_str());
                if ( prePlaceTab[yit][xit] == false )
                  prePlaceTab[yit][xit] = true;
                else
                  throw Error("%s is badly placed. There is another instance at it's position"
                             ,getString(*iolit).c_str());
              }
            }
          }

          bool rowOrientation = rowZeroOrientation;
          for ( int y = 0; y < (int)prePlaceTab.size(); ++y ) {
            int x = 0;

            while ( x < (int)prePlaceTab[y].size() ) {
              while ( (x < (int)prePlaceTab[y].size()) and (prePlaceTab[y][x] == true) ) ++x;

              DbU::Unit subRowXMin = gcell->getXMin() + x * pitch;

              if (x >= (int)prePlaceTab[y].size()) break;

              while ( (x < (int)prePlaceTab[y].size()) and (prePlaceTab[y][x] == false) ) ++x;

              DbU::Unit subRowXMax = gcell->getXMin() + x * pitch;
              if ( subRowXMax - subRowXMin > _binWidthMin ) {
                SubRow* subRow = SubRow::create ( getCell()
                                                , this
                                                , Box(subRowXMin, gcell->getYMin() +  y    * sliceHeight
                                                     ,subRowXMax, gcell->getYMin() + (y+1) * sliceHeight)
                                                , rowOrientation );
                placementProblem->_subRowList.push_back(subRow);
              }
            }
            rowOrientation = not rowOrientation;
          }
        } else {
          bool rowOrientation = false;
          for ( DbU::Unit ymin = gcell->getYMin()
              ; ymin <= gcell->getYMax() - sliceHeight
              ; ymin += sliceHeight ) {
            SubRow* subRow = SubRow::create ( getCell()
                                            , this
                                            , Box(gcell->getXMin(), ymin
                                                 ,gcell->getXMax(), ymin + sliceHeight)
                                            , rowOrientation );
            placementProblem->_subRowList.push_back(subRow);
            rowOrientation = not rowOrientation;
          }
        }
      }
    } else {
      PlacementProblem* placementProblem = new PlacementProblem(NULL);
      _searchHeight = getHeight();
      _searchWidth  = getWidth();

      bool         rowOrientation = false;
      unsigned int nRows          = getHeight() / sliceHeight;

      surfaceTotalWidth = ((double)getWidth()) * nRows;
        
      for ( DbU::Unit ymin = getYMin(); ymin <= getYMax() - sliceHeight; ymin += sliceHeight ) {
        SubRow* subRow = SubRow::create ( getCell()
                                        , this
                                        , Box(getXMin(), ymin,getXMax(), ymin + sliceHeight)
                                        , rowOrientation );
        placementProblem->_subRowList.push_back(subRow);
        rowOrientation = not rowOrientation;
      }

      for ( unsigned int id=0; id < _mauka->_instanceOccurrencesVector.size(); ++id ) {
        placementProblem->_toPlaceInstanceOccurrencesUVector.push_back(id);
      }
      throw Error("MaukaEngine needs NimbusEngine");
    }

    _computeCapacity();

    linefill output ("       ",cmess2);
    for (PlacementProblemList::iterator pplit = placementProblemList.begin();
         pplit != placementProblemList.end(); pplit++) {
    //cmess2 << "     - Initial placement of " << (*pplit)->_gcell->getBox() << endl;
      output << (*pplit)->_gcell->getBox();
      _DisplayInstances((*pplit)->_toPlaceInstanceOccurrencesUVector, (*pplit)->_subRowList);
      delete *pplit;
    }
    output << endl;

    placementProblemList.clear ();
    
    if ( verifyInstanceOccurrencesSet.size() != _mauka->_instanceOccurrencesVector.size() ) {
      cerr << Error("There are %d Instances not taken into account"
                   ,(_mauka->_instanceOccurrencesVector.size() - verifyInstanceOccurrencesSet.size())) << endl;

      VerifyInstanceQuadtreeInsertion(_mauka->getCell());

      for ( MaukaEngine::InstanceOccurrencesVector::iterator iovit = _mauka->_instanceOccurrencesVector.begin();
            iovit != _mauka->_instanceOccurrencesVector.end(); iovit++ ) {
        OccurrenceSet::iterator osit = verifyInstanceOccurrencesSet.find(*iovit);

        if ( osit == verifyInstanceOccurrencesSet.end() ) {
          cerr << "  o  Problem with " << *iovit << endl;
          DisplayInstanceOccurrence ( *iovit );
          DisplayNonLeafInstances ( _mauka->getCell(), iovit->getBoundingBox() );
          return;
        }
      }
      throw Error("There are %d Instances not taken into account"
                 ,(_mauka->_instanceOccurrencesVector.size() - verifyInstanceOccurrencesSet.size()));
    }

    for ( RowVector::const_iterator rvit = _rowVector.begin(); rvit != _rowVector.end(); rvit++ ) {
      surfaceTotalWidth += (double)((*rvit)->getSubRowsWidth());
    }

    _computeRowsAndSubRows();
    
    _margin = 1.0 - instanceToPlaceWidthSum / surfaceTotalWidth;
    if ( _margin < 0.0 ) {
      throw Error("There is not enough free space to place the circuit %s < %s"
                 ,DbU::getValueString(surfaceTotalWidth).c_str()
                 ,DbU::getValueString(instanceToPlaceWidthSum).c_str()
                 );
    }
    if (_margin < 0.15)
      cerr << Warning("Low margin %f, may not be able to complete successfully",_margin) << endl;
    
    _computeCapacity();
  }


void Surface::_preDestroy()
{
    Inherit::_preDestroy();
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
        (*rvit)->destroy();
    _rowVector.clear();
}

void Surface::_computeCapacity()
{
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
        (*rvit)->_computeCapacity(_margin);
}

namespace {

    struct RowSortByY
    {
        bool operator()(const Container* row1, const Container* row2) const
        {
            return row1->getYMin() < row2->getYMin();
        }
    };
}
    
Row* Surface::InsertSubRowInRow(SubRow* subrow, bool orientation)
{
    Row* row = NULL;
    if (_rowVector.size() == 0)
    {
        row = Row::create(getCell(), this, subrow->getBox(), orientation);
        _rowVector.push_back(row);
    }
    else
    {
        RowVector::iterator rvit = lower_bound(_rowVector.begin(), _rowVector.end(), subrow, RowSortByY());
        if (rvit == _rowVector.end())
        {
            row = Row::create(getCell(), this, subrow->getBox(), orientation);
            _rowVector.push_back(row);
        }
        else if ((*rvit)->getYMin() == subrow->getYMin())
        {
            row = *rvit;
            if (row->_orientation != orientation)
            {
                cerr << "PROBLEM in " << row << " orientation with " << subrow << endl;
            }
            //assert(row->_orientation == orientation);
        }
        else
        {
            row = Row::create(getCell(), this, subrow->getBox(), orientation);
            _rowVector.insert(rvit, row);
        }
    }
    row->_InsertSubRow(subrow);
    return row;
}

void Surface::_computeRowsAndSubRows()
{
    _rowYMax.clear();
    _rowYMinInv.clear();
    for (unsigned i = 0; i < _rowVector.size(); i++)
    {
        Row* row = _rowVector[i];
        _rowYMax[row->getYMax()] = i;
        _rowYMinInv[row->getYMin()] = i;
        row->_computeSubRows();
    }
}

Bin* Surface::getBinInSurface(Bin* srcbin, double dist)
{
    Point srcPos = srcbin->getCenter();

    DbU::Unit lowerY = srcPos.getY() - DbU::lambda((dist * DbU::getLambda(_searchHeight)));  
    if (lowerY < getYMin())
        lowerY = getYMin();
    DbU::Unit upperY = srcPos.getY() + DbU::lambda((dist * DbU::getLambda(_searchHeight)));  
    if (upperY > getYMax())
        upperY = getYMax();

    RowYMax::iterator rinf = _rowYMax.upper_bound(lowerY);
    RowYMax::iterator rsup = _rowYMinInv.upper_bound(upperY);


    unsigned randidx = rinf->second +
        (unsigned)((rsup->second - rinf->second + 1) * (rand() / (RAND_MAX+1.0)));

    Row* searchRow = _rowVector[randidx];

    DbU::Unit lowerX = srcPos.getX() - DbU::lambda((dist * DbU::getLambda(_searchWidth)));
    if ((lowerX < searchRow->getXMin()) || (lowerX > searchRow->getXMax()))
        lowerX = searchRow->getXMin();
    DbU::Unit upperX = srcPos.getX() + DbU::lambda((dist * DbU::getLambda(_searchWidth)));
    if ((upperX > searchRow->getXMax()) || (upperX < searchRow->getXMin()))
        upperX = searchRow->getXMax();

    SubRow* subRow = searchRow->getSubRowBetween(lowerX, upperX);
    
    if ((lowerX < subRow->getXMin()) || (lowerX > subRow->getXMax()))
        lowerX = subRow->getXMin();
    if ((upperX > subRow->getXMax()) || (upperX < subRow->getXMin()))
        upperX = subRow->getXMax();
    
    Bin* dstBin = subRow->getBinBetween(lowerX, upperX, srcbin);
    return dstBin;
}

double Surface::getBinCost() const
{
    double binCost = 0.0;
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
        binCost += (*rvit)->getBinCost();
    return binCost;
}

double Surface::getRowCost() const
{
    double rowCost = 0.0;
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
        rowCost += (*rvit)->getCost();
    return rowCost;
}

double Surface::getBinsSize() const
{
    double totalBinsSize = 0.0;
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
      totalBinsSize += (double)(*rvit)->getBinsSize();
    return totalBinsSize;
}

double Surface::getBinsCapa() const
{
    double totalBinsCapa = 0.0;
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
      totalBinsCapa += (double)(*rvit)->getBinsCapa();
    return totalBinsCapa;
}

double Surface::getSubRowsCapa() const
{
    double totalSubRowsCapa = 0.0;
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
    {
      totalSubRowsCapa += (double)(*rvit)->getSubRowsCapa();
    }
    return totalSubRowsCapa;
}

namespace {
    
class sortInstanceOccurrencesIdsByWidth
{
    private: MaukaEngine* _mauka;
    public: sortInstanceOccurrencesIdsByWidth(MaukaEngine* mauka): _mauka(mauka) {}
    public: 
    bool operator()(unsigned instanceoccurrenceid1, unsigned instanceoccurrenceid2) const
    {
        DbU::Unit width1 = _mauka->getInstanceIdWidth(instanceoccurrenceid1);
        DbU::Unit width2 = _mauka->getInstanceIdWidth(instanceoccurrenceid2);
        return width1 > width2; 
    }
};

}


void Surface::_DisplayInstances(MaukaEngine::UVector& instanceids, SubRowList& subrowlist)
{
    // Insert instances in subrows
    sort(instanceids.begin(), instanceids.end(), sortInstanceOccurrencesIdsByWidth(_mauka));
    
    SubRowList::iterator srlit = subrowlist.begin();
    MaukaEngine::UVector::const_iterator insIterator = instanceids.begin(); 
    MaukaEngine::UVector::const_iterator lastLoopInsertedInsIterator = insIterator; 
    size_t nbInstancesPlaced = 0;

  // First instance.
  // if ( insIterator != instanceids.end() ) {
  //   Cell* master = (static_cast<Instance*>(_mauka->_instanceOccurrencesVector[*insIterator].getEntity()))->getMasterCell();
  //   DbU::Unit instanceWidth = _mauka->_instanceWidths[*insIterator];
  //   cerr << "  Trying to add #0 id:" << *insIterator
  //        << " w:" << DbU::getValueString(instanceWidth)<< " " << master << endl;
  // }

    while (true)
    {
        if (insIterator == instanceids.end())
        {
          break;
        // end of insertion
        }

        if (srlit == subrowlist.end())
        {
          //if ( cmess2.enabled() ) cerr << "\n";
            cerr << Warning("Mauka::_DisplayInstances(): Cannot honor Bin margin, bypassing.") << endl;

            srlit = subrowlist.begin();
            if (lastLoopInsertedInsIterator != insIterator)
                lastLoopInsertedInsIterator = insIterator;
            else
            {
                // insertion of instances with respect of Bins margin
                // did not succeed, inserting what's left.
                while (insIterator != instanceids.end()) 
                {
                    for (SubRow::BinVector::iterator bvit = (*srlit)->_binVector.begin();
                            bvit != (*srlit)->_binVector.end();
                            bvit++)
                    {
                        Bin* bin = *bvit;
                        unsigned instanceId = *insIterator;
                        DbU::Unit instanceWidth = _mauka->_instanceWidths[instanceId];
                        if (instanceWidth > (*srlit)->getWidthVsSize())
                            break;
                        bin->addInstance(instanceId);
                        ++insIterator;
                        if (insIterator == instanceids.end())
                            break;
                    }
                    if (insIterator == instanceids.end())
                        break;
                    ++srlit;
                    if (srlit == subrowlist.end())
                    {
                        if (lastLoopInsertedInsIterator == insIterator)
                        {
                            //impossible to succeed
                            // try to merge all bins in subrows
                            bool merged = false;
                            for (srlit = subrowlist.begin();
                                    srlit != subrowlist.end();
                                    srlit++)
                            {
                                merged = (*srlit)->_MergeBins();
                            }
                            
                            if (merged)
                            {
                                insIterator = instanceids.begin();
                            }
                            else
                            {
                                //count instances to place
                                unsigned nbInstancesToPlace = 0;
                                while (insIterator++ != instanceids.end())
                                    ++nbInstancesToPlace;
                              //cerr << "Box: " << getBoundingBox() << endl;
                                throw Error("Not enough free space to place all the instances.<br>\n"
                                            "Please increase the abutment box: %s %d placeds (%d instances remains to place)"
                                           ,getString(getBoundingBox()).c_str()
                                           ,nbInstancesPlaced
                                           ,(nbInstancesToPlace - nbInstancesPlaced)
                                           );
                            }
                        }
                        srlit = subrowlist.begin();
                        lastLoopInsertedInsIterator = insIterator;
                    }
                }
            }
        }
        
        if (insIterator != instanceids.end())
        {
            SubRow* subRow = *srlit;
            for (SubRow::BinVector::iterator bvit = (*srlit)->_binVector.begin();
                    bvit != (*srlit)->_binVector.end();
                    bvit++)
            {
                Bin* bin = *bvit;
                // cerr << "      Bin: " << bin->getBoundingBox()
                //      << " capa:" << DbU::getValueString(bin->getCapa())
                //      << " size:" << DbU::getValueString(bin->getSize())
                //      << " SubRow:" 
                //      << " capa:" << DbU::getValueString(subRow->getCapa())
                //      << " size:" << DbU::getValueString(subRow->getSize())
                //      << endl;

                if (insIterator == instanceids.end()) break;

                unsigned instanceId = *insIterator;
                DbU::Unit instanceWidth = _mauka->_instanceWidths[instanceId];
                if (instanceWidth > subRow->getCapaVsSize()) {
                  // cerr << "    SubRow capacity exceeded" << endl;
                  break;
                }
                if (bin->TryAddInstance(instanceId)) {
                  srlit = subrowlist.begin();
                  ++nbInstancesPlaced;
                  ++insIterator;

                  // if ( insIterator != instanceids.end() ) {
                  //   Instance* instance = static_cast<Instance*>(_mauka->_instanceOccurrencesVector[instanceId].getEntity());
                  //   instanceWidth = _mauka->_instanceWidths[instanceId];
                  //   cerr << "  Trying to add #" << nbInstancesPlaced
                  //        << " id:" << *insIterator
                  //        << " w:" << DbU::getValueString(instanceWidth)<< " " << instance << endl;
                  //   cerr << "    " << _mauka->_instanceOccurrencesVector[instanceId]<< endl;
                  // }

                  break;
                }
            }
            ++srlit;
            // if ( srlit != subrowlist.end() ) {
            //   cerr << "    SubRow capaVsSize: " << DbU::getValueString((*srlit)->getCapaVsSize()) << endl;
            // }
        }
    }
}

Record* Surface::_getRecord() const
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("MaukaEngine", _mauka));
    }
    return record;
}

void Surface::DisplayBinHits() const
{
    for (RowVector::const_iterator rvit = _rowVector.begin();
            rvit != _rowVector.end();
            rvit++)
        (*rvit)->DisplayBinHits();
}

void Surface::PlotBinsStats(ofstream& out) const
{
    out << "plot [:] '-' w l 2" << endl ;
}

void Surface::Plot(ofstream& out) const
{
    out << getXMin() << " " << getYMin() << endl
        << getXMin() << " " << getYMax() << endl
        << getXMax() << " " << getYMax() << endl
        << getXMax() << " " << getYMin() << endl
        << getXMin() << " " << getYMin() << endl << endl;
    out << "EOF" << endl;
}

}
