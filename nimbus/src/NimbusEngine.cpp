//
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>
//
// Authors-Tag 

#include "math.h"
#include "hurricane/Pin.h"
#include "hurricane/Library.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/LefDefExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/CellGauge.h"
#include "crlcore/RoutingGauge.h"
#include "nimbus/Grid.h"
#include "nimbus/Fence.h"
#include "nimbus/GCell.h"
#include "nimbus/GCells.h"
#include "nimbus/RoutTools.h"
#include "nimbus/SplitterContact.h"
#include "nimbus/NimbusEngine.h"

namespace Nimbus {

/*
 * ********************************************************************
 * Nimbus_GCells declaration
 * ********************************************************************
 */
class Nimbus_GCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: const GCellSet* _nbset;
                private: GCellSet::iterator _nbit;

                public: Locator(const GCellSet* nbset, GCellSet::iterator nbit);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const GCellSet* _nbset;
    private: GCellSet::iterator _nbit;

    public: Nimbus_GCells(const GCellSet* nbset);
    public: Nimbus_GCells(const Nimbus_GCells& gcelles);

    public: Nimbus_GCells& operator=(const Nimbus_GCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
    public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};



/*
 * ********************************************************************
 * Nimbus_ColumnGCells declaration
 * ********************************************************************
 */
class Nimbus_ColumnGCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: const GCell* _gcell;

                public: Locator(const GCell* gcell);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const GCell* _upperGCell;

    public: Nimbus_ColumnGCells(const GCell* upperGCell);
    public: Nimbus_ColumnGCells(const Nimbus_ColumnGCells& gcelles);

    public: Nimbus_ColumnGCells& operator=(const Nimbus_ColumnGCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
  public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};



/*
 * ********************************************************************
 * Nimbus_RowGCells declaration
 * ********************************************************************
 */
class Nimbus_RowGCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: const GCell* _gcell;

                public: Locator(const GCell* gcell);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const GCell* _leftGCell;

    public: Nimbus_RowGCells(const GCell* left);
    public: Nimbus_RowGCells(const Nimbus_RowGCells& gcelles);

    public: Nimbus_RowGCells& operator=(const Nimbus_RowGCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
    public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};



/*
 * ********************************************************************
 * Nimbus_Fences declaration
 * ********************************************************************
 */
class Nimbus_Fences : public Collection<Fence*> {
    
    public: typedef Collection<Fence*> Inherit;

    public: class Locator : public Hurricane::Locator<Fence*> {

                public: typedef Hurricane::Locator<Fence*> Inherit;

                private: const FenceSet* _fcset;
                private: FenceSet::iterator _fcit;

                public: Locator(const FenceSet* fcset, FenceSet::iterator fcit);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual Fence* getElement() const;
                public: virtual Hurricane::Locator<Fence*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const FenceSet* _fcset;
    private: FenceSet::iterator _fcit;

    public: Nimbus_Fences(const FenceSet* fcset);
    public: Nimbus_Fences(const Nimbus_Fences& fences);

    public: Nimbus_Fences& operator=(const Nimbus_Fences& fences);
            
    public: virtual Collection<Fence*>* getClone() const;
    public: virtual Hurricane::Locator<Fence*>* getLocator() const;

    public: virtual string _getString() const;
};


/*
 * ********************************************************************
 * Nimbus_Layers declaration
 * ********************************************************************
 */
class Nimbus_Layers : public Collection<Layer*> {
    
    public: typedef Collection<Layer*> Inherit;

    public: class Locator : public Hurricane::Locator<Layer*> {

                public: typedef Hurricane::Locator<Layer*> Inherit;

                private: const set<Layer*>* _lset;
                private: set<Layer*>::iterator _lit;

                public: Locator(const set<Layer*>* lset, set<Layer*>::iterator lit);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual Layer* getElement() const;
                public: virtual Hurricane::Locator<Layer*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const set<Layer*>* _lset;
    private: set<Layer*>::iterator _lit;

    public: Nimbus_Layers(const set<Layer*>* lset);
    public: Nimbus_Layers(const Nimbus_Layers& layers);

    public: Nimbus_Layers& operator=(const Nimbus_Layers& layers);
            
    public: virtual Collection<Layer*>* getClone() const;
    public: virtual Hurricane::Locator<Layer*>* getLocator() const;

    public: virtual string _getString() const;
};


void NimbusEngine::_preCreate(Cell* cell)
// **************************************
{
  if ( cell->isTerminal() )
    throw Error("Won't create Nimbus on a standard cell");

#if CORIOLIS_2_DEPRECATED
    if (cell->getLibrary() == library) throw Error("Won't create Nimbus on a standard cell");
    string libname (getString(library->getName()));

    // ********************************************************************************************
    //CellGauge* stdCellGauge(NULL);

    // Check for CellGauge of library
    CellGauge* cellGauge = AllianceFramework::get()->getCellGauge(library->getName());
    if (cellGauge)
    {
        //stdCellGauge = cellGauges[libname];
        cmess1 << "  o  Using existing cell gauge for library: <" << libname << ">" << endl;
    }
    else
    {
        //cerr << error << endl;
        cmess1 << "  o  Creating cell gauge for library " << libname << endl;

        // read std datas
        Cells cells = library->getCells();
        CellLocator cellLocator = cells.getLocator();
        if (! cellLocator.isValid())
        {
            string message = "Invalid standard cell library ";
            message += getString(library->getName());
            throw Error ( message );
        }
        Cell* gaugeCell = cellLocator.getElement();

        DbU::Unit sliceHeight = gaugeCell->getAbutmentBox().getHeight();
        DbU::Unit pitch = gaugeCell->getAbutmentBox().getWidth();

        cellLocator.progress();
        while (cellLocator.isValid())
        {
            gaugeCell = cellLocator.getElement();

            if (gaugeCell->getAbutmentBox().getHeight() != sliceHeight)
                throw Error ("Inconsistent standard cell library");

            if (gaugeCell->getAbutmentBox().getWidth() < pitch)
                pitch = gaugeCell->getAbutmentBox().getWidth();

            cellLocator.progress();
        }
        pitch = DbU::lambda (DbU::getLambda(pitch) / 2);
        DbU::Unit sliceStep = pitch;

        // Creating CellGauge in the CDataBase
        cmess1 << "  o  adding cell gauge for " << libname << endl;
        if (libname == "ibm01") sliceHeight = DbU::lambda(5.04);
        AllianceFramework::get()->addCellGauge (CellGauge::create(libname.c_str(), "METAL2", pitch, sliceHeight, sliceStep));
        //stdCellGauge = cellGauges[libname];
        cmess1 << "  o  added cell gauge for " << libname << endl;
    }
  //getCDataBase()->setDefaultCellGauge(library->getName());

    // *****************************************************************************************
    // Check for routing layer gauge
    RoutingGauge* routingGauge = AllianceFramework::get()->getRoutingGauge(library->getName());
    if (!routingGauge) throw Error ("Nimbus : unable to find RoutingGauge associated with library : " +
            getString(library->getName()));
    Fence::setRoutingGauge(Configuration::getDefault()->getCellGauge());
#endif  // CORIOLIS_2_DEPRECATED

}


const Name  NimbusEngine::_toolName = "Nimbus";

NimbusEngine::NimbusEngine (Cell* cell, const Box& workZone)
// *********************************************************
  : Inherit         (cell)
  , _configuration  (Configuration::getDefault()->clone())
  , _depth          (0)
  , _placementBoxSet()
  , _routingBoxSet  ()
  , _routingLayerSet()
{
  if ( not workZone.isEmpty() ) _configuration->setWorkZone(workZone);
}

NimbusEngine* NimbusEngine::create (Cell* cell, const Box& workZone)
// *****************************************************************
{
    _preCreate(cell);
    
    NimbusEngine* nimbus = new NimbusEngine (cell,workZone);
    
    nimbus->_postCreate();

    if ( nimbus->doPinsPlacement() ) nimbus->placePins();

    return nimbus;
}

void NimbusEngine::_postCreate ()
// ******************************
{
    Inherit::_postCreate();

    // ********************************************************************************************
    // fence display slot creation
  //_displaySlot = DisplaySlot::create (getCell(), "Nimbus::Fences", 255, 0, 0, "FFFFFFFFFFFFFFFF", 1, 0, 255, 0, "CC33CC33CC33CC33", 1);

    _grid = new Grid ( this );

    if ( _grid == NULL )
        throw Error ("NimbusEngine was unable to create the grid on <%s>"
                    ,getString(getCell()->getName()).c_str());

    getGrid()->getRoot()->setAsPlacementLeaf();
    getGrid()->getRoot()->setAsRoutingLeaf();
}

void NimbusEngine::_preDestroy ()
// ******************************
{
#if 0
    if (_grid)
        delete _grid;
    delete _configuration;
#endif
    Inherit::_preDestroy();
}

const Name& NimbusEngine::staticGetName()
// *************************************
{
    return _toolName;
}

void NimbusEngine::computeXSplit ( GCell* gcell, DbU::Unit* XSplit, unsigned nbSplits )
// ************************************************************************************
{
    DbU::Unit     X     = gcell->getXMin();
    DbU::Unit     width = gcell->getWidth();
    unsigned int  numColsInf = (unsigned)rint( width / (getPitch() * nbSplits) );

    // cerr << "computeXSplit(): width:" << DbU::getValueString(width)
    //      << " nbsplits:" << nbSplits
    //      << " " << (width / getPitch())
    //      << " numColsInf:" << numColsInf << endl;

    if (numColsInf == 0)
      throw Error ("NimbusEngine::computeXSplit(): %s too narrow to be splitted."
                  ,DbU::getValueString(width).c_str());

    DbU::Unit splitWidth = numColsInf * getPitch();
    for ( unsigned i = 0 ; i < (nbSplits - 1) ; i++ ) {
        XSplit[i] = X + (i+1) * splitWidth;
    }
}

void NimbusEngine::computeYSplit ( GCell* gcell, DbU::Unit* YSplit, unsigned nbSplits )
// ************************************************************************************
{
    DbU::Unit     Y          = gcell->getYMin();
    DbU::Unit     height     = gcell->getHeight();
    unsigned int  numRowsInf = (unsigned)rint( height / (getSliceHeight() * nbSplits) );

    if (numRowsInf == 0)
      throw Error ("NimbusEngine::computeYSplit(): %s too narrow to be splitted."
                  ,DbU::getValueString(height).c_str());

    DbU::Unit splitHeight = numRowsInf * getSliceHeight();
    for ( unsigned i = 0 ; i < (nbSplits - 1) ; i++ ) {
        YSplit[i] = Y + (i+1) * splitHeight;
    }
}

void NimbusEngine::horizontalLongSplit ( unsigned step, DbU::Unit& Y ) const
// **************************************************************
{
    if ( !_grid )
        throw Error ("NimbusEngine ERROR : Can't horizontalLongSplit because no grid exists!");

    _grid->horizontalLongSplit ( step, Y );
    return;
}

void NimbusEngine::verticalLongSplit ( unsigned step, DbU::Unit& X ) const
// ************************************************************
{
    if ( !_grid )
        throw Error ("NimbusEngine ERROR : Can't verticalLongSplit because no grid exists!");

    _grid->verticalLongSplit ( step, X );
    return;
}

void NimbusEngine::createFences ( unsigned depth )
// *****************************************
{
    if ( !_grid )
        throw Error ("Nimbus ERROR : Can't createFences because no grid exists!");
    _grid->createFences ( depth );
    return;
}

void NimbusEngine::placePins()
// *********************
{
    UpdateSession::open();
    DbU::Unit width = getCell()->getAbutmentBox().getWidth();
    DbU::Unit height = getCell()->getAbutmentBox().getHeight();
    DbU::Unit xMin = getCell()->getAbutmentBox().getXMin();
    DbU::Unit yMin = getCell()->getAbutmentBox().getYMin();

    PinIocOrder& northPins     = LefDefExtension::getNorthPinIocOrder(getCell());
    PinIocOrder& southPins     = LefDefExtension::getSouthPinIocOrder(getCell());
    PinIocOrder& eastPins      = LefDefExtension::getEastPinIocOrder(getCell());
    PinIocOrder& westPins      = LefDefExtension::getWestPinIocOrder(getCell());
    //PinIocOrder& undefinedPins = LefDefExtension::getUndefinedPinIoc(getCell());
              
    unsigned northCount = northPins.size();
    DbU::Unit pos = xMin;
    if (northCount)
    {
        if (width - northCount <= 0)
            throw Error("not enough place on north side");
        
        DbU::Unit conSpace = width/northCount;
        pos += conSpace / 2;

        for (PinIocOrder::iterator pit = northPins.begin();
                pit != northPins.end();
                pit++)
        {
            Pin* pin = getCell()->getPin(*pit);
            if (!pin)
                throw Error("Unknown Pin named : " + getString(*pit));
            pin->setPosition(pos, getCell()->getAbutmentBox().getYMax());
            pos += conSpace;
        }
    }
    
    unsigned southCount = southPins.size();
    pos = xMin;
    if (southCount)
    {
        if (width - southCount <= 0)
            throw Error("not enough place on south side");
        
        DbU::Unit conSpace = width/southCount;
        pos += conSpace / 2;

        for (PinIocOrder::iterator pit = southPins.begin();
                pit != southPins.end();
                pit++)
        {
            Pin* pin = getCell()->getPin(*pit);
            if (!pin)
                throw Error("Unknown Pin named : " + getString(*pit));
            pin->setPosition(pos, getCell()->getAbutmentBox().getYMin());
            pos += conSpace;
        }
    }
    
    unsigned eastCount = eastPins.size();
    pos = yMin;
    if (eastCount)
    {
        if (height - eastCount <= 0)
            throw Error("not enough place on east side");
        
        DbU::Unit conSpace = width/eastCount;
        pos += conSpace / 2;

        for (PinIocOrder::iterator pit = eastPins.begin();
                pit != eastPins.end();
                pit++)
        {
            Pin* pin = getCell()->getPin(*pit);
            if (!pin)
                throw Error("Unknown Pin named : " + getString(*pit));
            pin->setPosition(getCell()->getAbutmentBox().getXMax(), pos);
            pos += conSpace;
        }
    }
    
    unsigned westCount = westPins.size();
    pos = yMin;
    if (westCount)
    {
        if (height - westCount <= 0)
            throw Error("not enough place on west side");
        
        DbU::Unit conSpace = width/westCount;
        pos += conSpace / 2;
        for (PinIocOrder::iterator pit = westPins.begin();
                pit != westPins.end();
                pit++)
        {
            Pin* pin = getCell()->getPin(*pit);
            if (!pin)
                throw Error("Unknown Pin named : " + getString(*pit));
            pin->setPosition(getCell()->getAbutmentBox().getYMin(), pos);
            pos += conSpace;
        }
    }
    UpdateSession::close();
}

void NimbusEngine::progress(int nbSplits)
// ********************************
{
    progress ( nbSplits, nbSplits );
}

void NimbusEngine::progress(int nbXSplits, int nbYSplits)
// ************************************************
{
    if ( ! (nbXSplits > 1) ) throw Error ( "NimbusEngine.progress error must divide into at least 2 gcells on X coordinate" );
    if ( ! (nbYSplits > 1) ) throw Error ( "NimbusEngine.progress error must divide into at least 2 gcells on Y coordinate" );

    DbU::Unit sliceHeight = getConfiguration()->getSliceHeight();
    for_each_gcell (nb, _grid->getGCells(_depth))
    {
        if (nb->getHeight() == sliceHeight)
            return;
        end_for;
    }

    GCell* gcell = _grid->getRoot();
    GCell* r = NULL;

    for (unsigned step = 0 ; step < _depth ; step++) {
        r = gcell->getSubUpperLeft();
        if (!r) {
            gcell->makeSub();
            r = gcell->getSubUpperLeft();
        }
        gcell = r;
    }

    assert(gcell);
    assert(gcell->getStep() == _depth);

    _depth++;

    try {

    for ( GCell* nby = gcell ; nby != NULL ; nby = _grid->getRightOf (nby) )
    {
        DbU::Unit XSplit [nbXSplits-1];
        computeXSplit ( nby, XSplit, nbXSplits );

        for ( int i = 0 ; i < (nbXSplits-1) ; i++ ) {
            _grid->verticalLongSplit (_depth, XSplit[i] );
        }
    }
    for ( GCell* nbx = gcell ; nbx != NULL ; nbx = _grid->getDownOf (nbx) )
    {
        DbU::Unit YSplit [nbYSplits-1];
        computeYSplit ( nbx, YSplit, nbYSplits );

        for ( int i = 0 ; i < (nbYSplits-1) ; i++ ) {
            _grid->horizontalLongSplit ( _depth, YSplit[i] );
        }
    }


    _grid->createFences(_depth);

    } catch (Hurricane::Error &e) {
        if (e.getCode() != 1) throw e;
        cmess1 << "[WARNING] Finest grid level reached" << endl;
        destroyLevel(_depth);
    }
}

void NimbusEngine::recDestroy(unsigned depth, GCell* gcell)
{
    if (gcell->getDepth() == depth) gcell->destroy();
    else for_each_gcell(subgcell, gcell->getSubGCells())
    {
        recDestroy(depth, subgcell);
        end_for;
    }
}

void NimbusEngine::destroyLevel(unsigned depth)
{
    recDestroy (depth, getGrid()->getRoot());
    if (depth > 0) _depth = depth - 1;
}

#if 0
void NimbusEngine::balanceLeaves()
{
    GCell* CornerRB = _grid->getUpperLeftCorner(_step);


    // Balancing columns
    // *****************
    {
        GCell* col1RB = CornerRB;
        GCell* col2RB = NULL;

        while (col1RB)
        {
            col2RB = col1RB->getRightOfMe();
            assert (col2RB);
            GCell* nb1 = col1RB;
            GCell* nb2 = col2RB;

            // Cell surface & width summation
            // ******************************
            while (nb1 != NULL)
            {
                assert (nb2 != NULL);
                cumulatedSurface1 += getCellsSurface (nb1);
                cumulatedSurface2 += getCellsSurface (nb2);
                cumulatedWidth1 += getCellsWidth (nb1);
                cumulatedWidth2 += getCellsWidth (nb2);
                nb1 = nb1->getDownOfMe();
                nb2 = nb2->getDownOfMe();
            }

            // Tuning the vertical cut line
            // ****************************


            // Onto the next column pair
            // *************************
            col1RB = col2RB->getRightOfMe();
        }
    }

    // Balancing rows
    // **************
    {
        GCell* row1RB = CornerRB;
        GCell* row2RB = NULL;

        while (row1RB)
        {
            row2RB = row1RB->getDownOfMe();
            assert (row2RB);
            GCell* nb1 = row1RB;
            GCell* nb2 = row2RB;

            // Cell surface & height summation
            // *******************************
            while (nb1 != NULL)
            {
                assert (nb2 != NULL);
                cumulatedSurface1 += getCellsSurface (nb1);
                cumulatedSurface2 += getCellsSurface (nb2);
                cumulatedHeight1 += getCellsHeight (nb1);
                cumulatedHeight2 += getCellsHeight (nb2);
                nb1 = nb1->getRightOfMe();
                nb2 = nb2->getRightOfMe();
            }

            // Tuning the horizontal cut line
            // ******************************


            // Onto the next row pair
            // **********************
            row1RB = row2RB->getDownOfMe();
        }
    }

    // Balancing finished

    return;
} // balanceLeaves
#endif

#if 0
DbU::Unit NimbusEngine::getStandardCellHeight () const
// ****************************************
{
    return _stdCellHeight;
}
#endif

GCell* NimbusEngine::getPlacementLeafContaining(Point& p)
// *************************************************
{
    GCell* gcell = getGrid()->getRoot();

    while (! (gcell->isAPlacementLeaf()) )
    {
        bool deeper = false;
        for_each_gcell (nb, gcell->getSubGCells())
        {
            if (nb->strictContains(p))
            {
                gcell = nb;
                deeper = true;
            }
            end_for;
        }
        if (!deeper) return NULL;
    }
    return gcell;
}

bool NimbusEngine::testMargin ( double margin )
// **************************************
{
    bool testOK = true;
    for_each_gcell ( gcell, getPlacementLeaves() )
    {
        string message = "     - GCell " + getString( gcell ) + " :\t";
        double NimbMargin = gcell->testMargin();
        if ( NimbMargin < 0 )
        {
            message += "negative margin = " + getString ( NimbMargin );
            testOK = false;
        }
        else if ( NimbMargin < margin )
        {
            message += "margin = " + getString ( NimbMargin ) + " < " + getString ( margin );
            testOK = false;
        }
        else
        {
            message += "margin = " + getString ( NimbMargin ) + " : OK";
        }
        cmess1 << message << endl;
        end_for;
    }

    return testOK;
}

bool NimbusEngine::testMargin ()
// *****************************
{
    bool testOK = true;
    for_each_gcell ( gcell, getPlacementLeaves() )
    {
        string message = "     - GCell " + getString( gcell ) + " :\t";
        double NimbMargin = gcell->testMargin();
        if ( NimbMargin < 0 )
        {
            message += "negative margin = " + getString ( NimbMargin );
            testOK = false;
        }
        else if ( NimbMargin < getMargin() )
        {
          message += "margin = " + getString ( NimbMargin ) + " < " + getString ( getMargin() );
            testOK = false;
        }
        else
        {
            message += "margin = " + getString ( NimbMargin ) + " : OK";
        }
        cmess1 << message << endl;
        end_for;
    }

    return testOK;
}

GCell* NimbusEngine::getRoutingLeafContaining(Point& p)
// ***********************************************
{
    GCell* gcell = getGrid()->getRoot();

    while (! (gcell->isARoutingLeaf()) )
    {
        bool deeper = false;
        for_each_gcell (nb, gcell->getSubGCells())
        {
            if (nb->strictContains(p))
            {
                gcell = nb;
                deeper = true;
            }
            end_for;
        }
        if (!deeper) return NULL;
    }
    return gcell;
}


GCell* NimbusEngine::getCommonAncester(GCell* nb1, GCell* nb2) const
// **************************************************************
{
    if (nb1 == nb2) return nb1;

    if (!nb1) return nb2;
    if (!nb2) return nb1;

    if (nb1->getStep() > nb2->getStep())
        return getCommonAncester(nb1->getContainer(), nb2);
    else
        return getCommonAncester(nb1, nb2->getContainer());

    return NULL;
}


GCells NimbusEngine::getLeaves() const
// *************************
{
    return _grid->getGCells(_depth);
}
GCells NimbusEngine::getLeaves(unsigned step) const
// **************************************
{
    if (step > _depth) throw ("[ERROR] Request for an unreached refinement level");

    return _grid->getFastGCells(step);
}

Fences NimbusEngine::getFences(unsigned step)
// ************************************
{
    if (step > _depth) throw ("[ERROR] Request for an unreached refinement level");

    return (_grid->getFences(step));
}

GCells NimbusEngine::getPlacementLeaves() const
// ****************************************
{
    return Nimbus_GCells(&_placementBoxSet);
}

GCells NimbusEngine::getRoutingLeaves() const
// **************************************
{
    return Nimbus_GCells(&_routingBoxSet);
}

Fences NimbusEngine::getRoutingFences() const
// ************************************
{
    return Nimbus_Fences(&_routingFenceSet);
}

GCells NimbusEngine::getColumnGCells( unsigned indexColumn ) const
// *************************************************************
{
    GCell* _upperGCell = ((NimbusEngine*)this)->getGrid()->getUpperLeftCorner(((NimbusEngine*)this)->getDepth());   //(Nimbus*)this to avoid const problem
    for ( unsigned i = indexColumn ; i > 0 ; i-- ) {
        _upperGCell = _upperGCell->getRightOfMe();
        if (!_upperGCell)
            throw Error ( "NimbusEngine: Invalid Column Index" );
    }
    return Nimbus_ColumnGCells(_upperGCell);
}

GCells NimbusEngine::getRowGCells( unsigned indexRow ) const
// *******************************************************
{
    GCell* _leftGCell = ((NimbusEngine*)this)->getGrid()->getUpperLeftCorner(((NimbusEngine*)this)->getDepth());   //(Nimbus*)this to avoid const problem
    for ( unsigned i = indexRow ; i > 0 ; i-- ) {
        _leftGCell = _leftGCell->getDownOfMe();
        if (!_leftGCell)
            throw Error ( "NimbusEngine: Invalid Row Index" );
    }

    return Nimbus_RowGCells(_leftGCell);
}

Layers NimbusEngine::getRoutingLayers() const
// ************************************
{
    return Nimbus_Layers(&_routingLayerSet);
}

void NimbusEngine::flushRoutingFences()
// ******************************
{
    for_each_fence(fence, getRoutingFences())
    {
        fence->_NotRoutingFence();
        end_for;
    }
    _routingFenceSet.clear();
    return;
}

void NimbusEngine::addToRoutingFences(Fence* fence)
// ******************************************
{
    _routingFenceSet.insert(fence);
    return;
}

void NimbusEngine::addToRoutingLayers(Layer* layer)
// ******************************************
{
    _routingLayerSet.insert(layer);
    return;
}

void NimbusEngine::flushPlacementLeaves()
// ******************************
{
    for_each_gcell(gcell, getPlacementLeaves())
    {
        gcell->_setNotPlacementLeaf();
        end_for;
    }
    _placementBoxSet.clear();

    return;
}

void NimbusEngine::flushRoutingLeaves()
// ******************************
{
    for_each_gcell(gcell, getRoutingLeaves())
    {
        gcell->_setNotRoutingLeaf();
        end_for;
    }
    _routingBoxSet.clear();

    return;
}

void NimbusEngine::setRoutingLeaf(GCell* gcell)
// ****************************************
{
    setLeaf(gcell, _routingBoxSet);
    return;
}
void NimbusEngine::setPlacementLeaf(GCell* gcell)
// ******************************************
{
    setLeaf(gcell, _placementBoxSet);
    return;
}

void NimbusEngine::setSubGCellsAsPlacementLeaves (GCell* gcell)
// **********************************************************
{
    gcell->setSubGCellsAsPlacementLeaves();
    return;
}
void NimbusEngine::setSubGCellsAsRoutingLeaves (GCell* gcell)
// ********************************************************
{
    gcell->setSubGCellsAsRoutingLeaves();
    return;
}

void NimbusEngine::setLeaf(GCell* gcell, GCellSet& nbset)
// ***************************************************
{
    nbset.insert(gcell);
#if 0
    GCell* container = gcell->getContainer();
    while (container)
    {
        GCellSet::iterator nbit = nbset.find(container);
        if (nbit != nbset.end())
            nbset.erase(nbit);
        container = container->getContainer();
    }
#endif
    return;
}

void NimbusEngine::_removePlacementLeaf(GCell* gcell)
// **********************************************
{
    GCellSet::iterator nbit = _placementBoxSet.find(gcell);
    if (nbit != _placementBoxSet.end())
        _placementBoxSet.erase(nbit);
    return;
}
 
void NimbusEngine::_removeRoutingLeaf(GCell* gcell)
// **********************************************
{
    GCellSet::iterator nbit = _routingBoxSet.find(gcell);
    if (nbit != _routingBoxSet.end())
        _routingBoxSet.erase(nbit);
    return;
}

void NimbusEngine::regroup(bool placed, bool fixed)
// *****************
{
    grabPlacementModificationFlag();
    UpdateSession::open();
    for_each_instance(ins, getCell()->getInstances())
    {
        if ( (ins->getPlacementStatus() == Instance::PlacementStatus::PLACED) && !placed ) continue;
        if ( (ins->getPlacementStatus() == Instance::PlacementStatus::FIXED ) && !fixed  ) continue;

        Box instanceABox = ins->getAbutmentBox();
        Point insCenter = ins->getBoundingBox().getCenter();
        GCell* nb = getPlacementLeafContaining(insCenter);
        if ( nb )
        {
            Point center = nb->getCenter();
            DbU::Unit xPos = center.getX();
            DbU::Unit yPos = center.getY();
            Box masterABox = ins->getMasterCell()->getAbutmentBox();
            ins->setTransformation(
                    getTransformation(
                        masterABox,
                        xPos - instanceABox.getHalfWidth(),
                        yPos - instanceABox.getHalfHeight(),
                        Transformation::Orientation::ID)
                    );
        }
        else
        {
            cerr << "Didn't find PlacementLeaf for instance " << ins << endl;
        }
        end_for;
    }
    UpdateSession::close();
    return;
}

void NimbusEngine::placementLeavesUp ()
// ******************************
{
    UpdateSession::open();
    grabPlacementModificationFlag();
    set<GCell*> gcelles;
    set<GCell*> newleaves;
    getPlacementLeaves().fill(gcelles);

    for_each_gcell (gcell, getPlacementLeaves())
    {
        GCell* container = gcell->getContainer();
        if (container) gcelles.insert (container);
        end_for;
    }

    for(set<GCell*>::iterator nbit = gcelles.begin() ; nbit != gcelles.end() ; nbit++)
    {
        GCell* nb1 = *nbit;
        GCell* nb2 = *nbit;
        bool found (false);
        while ( (nb2 = nb2->getContainer()) )
        {
            if ( gcelles.find (nb2) != gcelles.end() )
                found = true;
        }
        if (!found) newleaves.insert (nb1);
    }

    for(set<GCell*>::iterator nbit = newleaves.begin() ; nbit != newleaves.end() ; nbit++)
    {
        GCell* nb = *nbit;
        nb->setAsPlacementLeaf();
    }

    hideFences();
    showPlacementLeaves();
    UpdateSession::close();

    return;
}

void NimbusEngine::placementLeavesDown ()
// ******************************
{
    UpdateSession::open();
    grabPlacementModificationFlag();
    set<GCell*> gcelles;
    getPlacementLeaves().fill(gcelles);
    for(set<GCell*>::iterator nbit = gcelles.begin() ; nbit != gcelles.end() ; nbit++)
    {
        (*nbit)->setSubGCellsAsPlacementLeaves();
    }
    hideFences();
    showPlacementLeaves();
    UpdateSession::close();

    return;
}
    
void NimbusEngine::hideFences()
// **********************
{
    for (unsigned step = 1; step <= _depth ; step++)
    {
        for_each_fence (fence, getFences(step))
        {
            fence->setInvisible();
            end_for;
        }
    }
    return;
}

void NimbusEngine::showPlacementLeaves()
// *******************************
{
    for_each_gcell(gcell, getPlacementLeaves())
    {
        for_each_fence(fence, gcell->getSurroundingFences())
        {
            fence->setVisible();
            end_for;
        }
        end_for;
    }
    for_each_go(go, getCell()->getExtensionSlice("Nimbus::Grid")->_getQuadTree()->getGos())
    {
        Fence* fence (NULL);
        if (! (fence = (dynamic_cast<Fence*> (go)) ) ) continue;
        if (! fence->isVisible() ) continue;
        Fence* pfence (fence);
        while ( (pfence = pfence->getParentFence()) ) pfence->setInvisible();
        end_for;
    }

    return;
}
void NimbusEngine::showRoutingLeaves()
// *******************************
{
    for_each_gcell(gcell, getRoutingLeaves())
    {
        for_each_fence(fence, gcell->getSurroundingFences())
        {
            fence->setVisible();
            end_for;
        }
        end_for;
    }
    return;
}

void NimbusEngine::invalidateSplitterContactsOppositeCounts(Net* net)
// ************************************************************
{
    assert(net);
    for_each_component(compo, net->getComponents())
    {
        if (! (dynamic_cast<SplitterContact*> (compo)) )
            continue;
        SplitterContact* sc = dynamic_cast<SplitterContact*> (compo);
        sc->invalidateOppositeCounts();
        end_for;
    }

    return;
}

void NimbusEngine::invalidateSplitterContactsOppositeCounts()
// ****************************************************
{
    for_each_net(net, getCell()->getNets())
    {
        invalidateSplitterContactsOppositeCounts(net);
        end_for;
    }
    return;
}

string NimbusEngine::_getString() const
// ******************************
{
    return "<" + _TName("Nimbus") + ">";
}

Record* NimbusEngine::_getRecord() const
// *************************
{
    Record* record = Inherit::_getRecord();

    if (record) {
        record->add(getSlot("Depth", _depth));
        record->add(getSlot("Root Box", _grid->getRoot()));
    }
    return record;
}

const Name& NimbusEngine::getName () const
{
    return _toolName;
}



NimbusEngine* NimbusEngine::get ( Cell* cell )
{
  return static_cast<NimbusEngine*>(ToolEngine::get(cell, staticGetName()));
}

/*
 * ********************************************************************
 * Nimbus_GCells implementation
 *
 */
 
Nimbus_GCells::Nimbus_GCells(const GCellSet* nbset)
// **************************************************************************************
:       Inherit()
        , _nbset(nbset)
{
    assert(nbset);
    _nbit = nbset->begin();
}
 
Nimbus_GCells::Nimbus_GCells(const Nimbus_GCells& gcelles)
// ************************************************************************************
:       Inherit()
        , _nbset(gcelles._nbset)
        , _nbit(gcelles._nbit)
{
}
 
Nimbus_GCells& Nimbus_GCells::operator=(const Nimbus_GCells& gcelles)
// ****************************************************************
{
        _nbset    = gcelles._nbset;
        _nbit     = gcelles._nbit;
        return *this;
}
 
Collection<GCell*>* Nimbus_GCells::getClone() const
// ***********************************************
{
        return new Nimbus_GCells(*this);
}
 
Locator<GCell*>* Nimbus_GCells::getLocator() const
// **********************************************
{
        return new Locator(_nbset, _nbit);
}
 
string Nimbus_GCells::_getString() const
// *************************************
{
        string s = "<" + _TName("Nimbus::GCells");
        //if (_gcellIterator) s += " " + getString((*_gcellIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Nimbus_GCells::Locator implementation
 */
Nimbus_GCells::Locator::Locator(const GCellSet* nbset, GCellSet::iterator nbit)
// ************************************************************************************
:       Inherit(),
        _nbset(nbset),
        _nbit(nbit)
{
}
 
Nimbus_GCells::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _nbset(locator._nbset),
        _nbit(locator._nbit)
{
}
 
Nimbus_GCells::Locator& Nimbus_GCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _nbset = locator._nbset;
        _nbit = locator._nbit;
        return *this;
}
 
GCell* Nimbus_GCells::Locator::getElement() const
// **************************************************
{
        return (*_nbit);
}
 
Locator<GCell*>* Nimbus_GCells::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Nimbus_GCells::Locator::isValid() const
// **************************************************
{
        return (_nbit != _nbset->end());
}

void Nimbus_GCells::Locator::progress()
// *********************************************
{
    _nbit++;

    return;
}
 
string Nimbus_GCells::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Nimbus::GCells::Locator");
        if (_nbit != _nbset->end()) s += " " + getString(*_nbit);
        s += ">";
        return s;
}










/*
 * ********************************************************************
 * Nimbus_ColumnGCells implementation
 *
 */
 
Nimbus_ColumnGCells::Nimbus_ColumnGCells(const GCell* upperGCell)
// *********************************************************************
:       Inherit(),
        _upperGCell ( upperGCell )
{
}
 
Nimbus_ColumnGCells::Nimbus_ColumnGCells(const Nimbus_ColumnGCells& gcelles)
// ********************************************************************************
:       Inherit()
{
    _upperGCell = gcelles._upperGCell;
}
 
Nimbus_ColumnGCells& Nimbus_ColumnGCells::operator=(const Nimbus_ColumnGCells& gcelles)
// *******************************************************************************************
{
        _upperGCell = gcelles._upperGCell;
        return *this;
}
 
Collection<GCell*>* Nimbus_ColumnGCells::getClone() const
// *********************************************************
{
        return new Nimbus_ColumnGCells(*this);
}
 
Locator<GCell*>* Nimbus_ColumnGCells::getLocator() const
// **********************************************
{
        return new Locator(_upperGCell);
}
 
string Nimbus_ColumnGCells::_getString() const
// *********************************************
{
        string s = "<" + _TName("Nimbus::ColumnGCells");
        //if (_gcellIterator) s += " " + getString((*_gcellIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Nimbus_ColumnGCells::Locator implementation
 */
Nimbus_ColumnGCells::Locator::Locator(const GCell* gcell)
// **********************************************************
:       Inherit(),
        _gcell(gcell)
{
}
 
Nimbus_ColumnGCells::Locator::Locator(const Locator& locator)
// ************************************************************
:       Inherit(),
        _gcell(locator._gcell)
{
}
 
Nimbus_ColumnGCells::Locator& Nimbus_ColumnGCells::Locator::operator=(const Locator& locator)
// **********************************************************************************************
{
        _gcell = locator._gcell;
        return *this;
}
 
GCell* Nimbus_ColumnGCells::Locator::getElement() const
// *******************************************************
{
        return (GCell*)_gcell;
}
 
Locator<GCell*>* Nimbus_ColumnGCells::Locator::getClone() const
// ***************************************************************
{
        return new Locator(*this);
}
 
bool Nimbus_ColumnGCells::Locator::isValid() const
// **************************************************
{
        return (_gcell != NULL);
}

void Nimbus_ColumnGCells::Locator::progress()
// *********************************************
{
    _gcell = _gcell->getDownOfMe();
    return;
}
 
string Nimbus_ColumnGCells::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Nimbus::ColumnGCells::Locator");
        s += " " + getString(_gcell);
        s += ">";
        return s;
}










/*
 * ********************************************************************
 * Nimbus_RowGCells implementation
 *
 */
 
Nimbus_RowGCells::Nimbus_RowGCells(const GCell* leftGCell)
// *************************************************************
:       Inherit(),
        _leftGCell ( leftGCell )
{
}
 
Nimbus_RowGCells::Nimbus_RowGCells(const Nimbus_RowGCells& gcelles)
// ***********************************************************************
:       Inherit()
{
    _leftGCell = gcelles._leftGCell;
}
 
Nimbus_RowGCells& Nimbus_RowGCells::operator=(const Nimbus_RowGCells& gcelles)
// *******************************************************************************************
{
        _leftGCell = gcelles._leftGCell;
        return *this;
}
 
Collection<GCell*>* Nimbus_RowGCells::getClone() const
// ******************************************************
{
        return new Nimbus_RowGCells(*this);
}
 
Locator<GCell*>* Nimbus_RowGCells::getLocator() const
// *****************************************************
{
        return new Locator(_leftGCell);
}
 
string Nimbus_RowGCells::_getString() const
// ******************************************
{
        string s = "<" + _TName("Nimbus::RowGCells");
        //if (_gcellIterator) s += " " + getString((*_gcellIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Nimbus_RowGCells::Locator implementation
 */
Nimbus_RowGCells::Locator::Locator(const GCell* gcell)
// *******************************************************
:       Inherit(),
        _gcell(gcell)
{
}
 
Nimbus_RowGCells::Locator::Locator(const Locator& locator)
// *********************************************************
:       Inherit(),
        _gcell(locator._gcell)
{
}
 
Nimbus_RowGCells::Locator& Nimbus_RowGCells::Locator::operator=(const Locator& locator)
// ****************************************************************************************
{
        _gcell = locator._gcell;
        return *this;
}
 
GCell* Nimbus_RowGCells::Locator::getElement() const
// ****************************************************
{
        return (GCell*)_gcell;
}
 
Locator<GCell*>* Nimbus_RowGCells::Locator::getClone() const
// ************************************************************
{
        return new Locator(*this);
}
 
bool Nimbus_RowGCells::Locator::isValid() const
// **********************************************
{
        return (_gcell != NULL);
}

void Nimbus_RowGCells::Locator::progress()
// *****************************************
{
    _gcell = _gcell->getRightOfMe();
    return;
}
 
string Nimbus_RowGCells::Locator::_getString() const
// ***************************************************
{
        string s = "<" + _TName("Nimbus::RowGCells::Locator");
        s += " " + getString(_gcell);
        s += ">";
        return s;
}










/*
 * ********************************************************************
 * Nimbus_Fences implementation
 *
 */
 
Nimbus_Fences::Nimbus_Fences(const FenceSet* fcset)
// **************************************************************************************
:       Inherit()
        , _fcset(fcset)
{
    assert(fcset);
    _fcit = fcset->begin();
}
 
Nimbus_Fences::Nimbus_Fences(const Nimbus_Fences& fences)
// ************************************************************************************
:       Inherit()
        , _fcset(fences._fcset)
        , _fcit(fences._fcit)
{
}
 
Nimbus_Fences& Nimbus_Fences::operator=(const Nimbus_Fences& fences)
// ****************************************************************
{
        _fcset    = fences._fcset;
        _fcit     = fences._fcit;
        return *this;
}
 
Collection<Fence*>* Nimbus_Fences::getClone() const
// ***********************************************
{
        return new Nimbus_Fences(*this);
}
 
Locator<Fence*>* Nimbus_Fences::getLocator() const
// **********************************************
{
        return new Locator(_fcset, _fcit);
}
 
string Nimbus_Fences::_getString() const
// *************************************
{
        string s = "<" + _TName("Nimbus::Fences");
        //if (_fenceIterator) s += " " + getString((*_fenceIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Nimbus_Fences::Locator implementation
 */
Nimbus_Fences::Locator::Locator(const FenceSet* fcset, FenceSet::iterator fcit)
// ************************************************************************************
:       Inherit(),
        _fcset(fcset),
        _fcit(fcit)
{
}
 
Nimbus_Fences::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _fcset(locator._fcset),
        _fcit(locator._fcit)
{
}
 
Nimbus_Fences::Locator& Nimbus_Fences::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _fcset = locator._fcset;
        _fcit = locator._fcit;
        return *this;
}
 
Fence* Nimbus_Fences::Locator::getElement() const
// **************************************************
{
        return (*_fcit);
}
 
Locator<Fence*>* Nimbus_Fences::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Nimbus_Fences::Locator::isValid() const
// **************************************************
{
        return (_fcit != _fcset->end());
}

void Nimbus_Fences::Locator::progress()
// *********************************************
{
    _fcit++;

    return;
}
 
string Nimbus_Fences::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Nimbus::Fences::Locator");
        if (_fcit != _fcset->end()) s += " " + getString(*_fcit);
        s += ">";
        return s;
}








/*
 * ********************************************************************
 * Nimbus_Layers implementation
 *
 */
 
Nimbus_Layers::Nimbus_Layers(const set<Layer*>* lset)
// **************************************************************************************
:       Inherit()
        , _lset(lset)
{
    assert(lset);
    _lit = lset->begin();
}
 
Nimbus_Layers::Nimbus_Layers(const Nimbus_Layers& layers)
// ************************************************************************************
:       Inherit()
        , _lset(layers._lset)
        , _lit(layers._lit)
{
}
 
Nimbus_Layers& Nimbus_Layers::operator=(const Nimbus_Layers& layers)
// ****************************************************************
{
        _lset    = layers._lset;
        _lit     = layers._lit;
        return *this;
}
 
Collection<Layer*>* Nimbus_Layers::getClone() const
// ***********************************************
{
        return new Nimbus_Layers(*this);
}
 
Locator<Layer*>* Nimbus_Layers::getLocator() const
// **********************************************
{
        return new Locator(_lset, _lit);
}
 
string Nimbus_Layers::_getString() const
// *************************************
{
        string s = "<" + _TName("Nimbus::Layers");
        //if (_layerIterator) s += " " + getString((*_layerIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Nimbus_Layers::Locator implementation
 */
Nimbus_Layers::Locator::Locator(const set<Layer*>* lset, set<Layer*>::iterator lit)
// ************************************************************************************
:       Inherit(),
        _lset(lset),
        _lit(lit)
{
}
 
Nimbus_Layers::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _lset(locator._lset),
        _lit(locator._lit)
{
}
 
Nimbus_Layers::Locator& Nimbus_Layers::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _lset = locator._lset;
        _lit = locator._lit;
        return *this;
}
 
Layer* Nimbus_Layers::Locator::getElement() const
// **************************************************
{
        return (*_lit);
}
 
Locator<Layer*>* Nimbus_Layers::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Nimbus_Layers::Locator::isValid() const
// **************************************************
{
        return (_lit != _lset->end());
}

void Nimbus_Layers::Locator::progress()
// *********************************************
{
    _lit++;

    return;
}
 
string Nimbus_Layers::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Nimbus::Layers::Locator");
        if (_lit != _lset->end()) s += " " + getString(*_lit);
        s += ">";
        return s;
}












} // namespace Nimbus
