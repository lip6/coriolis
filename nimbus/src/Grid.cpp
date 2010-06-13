
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#include <math.h>

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/UpdateSession.h"
#include "crlcore/Utilities.h"
#include "crlcore/ToolBox.h"
#include "crlcore/CellGauge.h"
#include "crlcore/LefDefExtension.h"
#include "nimbus/GCell.h"
#include "nimbus/Fence.h"
#include "nimbus/VFence.h"
#include "nimbus/HFence.h"
#include "nimbus/StepProperty.h"
#include "nimbus/Grid.h"
#include "nimbus/NimbusEngine.h"

namespace Nimbus {

#define RG_HORIZONTAL   0
#define RG_VERTICAL     1



/*
 * ********************************************************************
 * GCell_GCells declaration
 * ********************************************************************
 */
class Grid_GCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: GCell* _nb;
                private: unsigned _direction;

                public: Locator(GCell* gcell = NULL);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;

                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: GCell* _nb;
    private: unsigned _step;

    public: Grid_GCells(const Grid* grid, unsigned step);
    public: Grid_GCells(const Grid_GCells& gcelles);

    public: Grid_GCells& operator=(const Grid_GCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
    public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};

/*
 * ********************************************************************
 * GCell_GCells declaration
 * ********************************************************************
 */
class Grid_FastGCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: GCell* _nb;
                private: unsigned _direction;

                public: Locator(GCell* gcell = NULL);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;

                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: GCell* _nb;
    private: unsigned _step;

    public: Grid_FastGCells(const Grid* grid, unsigned step);
    public: Grid_FastGCells(const Grid_FastGCells& gcelles);

    public: Grid_FastGCells& operator=(const Grid_FastGCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
    public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};



/*
 * ********************************************************************
 * Grid
 *
 * ********************************************************************
 */
Grid::Grid (NimbusEngine* nimbus)
//*******************************
    : _nimbus   (nimbus)
    , _cell     (nimbus->getCell())
    , _rootGCell(NULL)
    , _layer    (NULL)
    , _fences   ()
{
  _layer = DataBase::getDB()->getTechnology()->getLayer (Name ("SPL1"));
  if ( _layer == NULL )
    throw Error("NimbusEngine needs <SPL1> layer to be defined");

  Box defBox = LefDefExtension::getRowsBox(_cell);
    
  if ( not _nimbus->getWorkZone().isEmpty() ) {
    _rootGCell = GCell::create(_nimbus, 0, _nimbus->getWorkZone());
  } else if ( not defBox.isEmpty() ) {
    UpdateSession::open();

    if ( _cell->getAbutmentBox().isEmpty() )
      _cell->setAbutmentBox ( defBox );

    cmess1 << "     - DEF abutment box found: " << defBox << endl;
    _rootGCell = GCell::create(_nimbus, 0, defBox);

    UpdateSession::close();
  } else if ( _cell->getAbutmentBox().isEmpty() ) {
    if ( isNoInstancePlacedOrFixed(_cell) ) {
      DbU::Unit minWidth = DbU::Max;
      double    sumWidth = 0.0;
      forEach ( Instance*, iinstance, _cell->getInstances() ) {
        Cell*     model = iinstance->getMasterCell();
        DbU::Unit width = model->getAbutmentBox().getWidth();

        if ( width < minWidth ) minWidth = width;
        sumWidth += DbU::getLambda(width);
      }
      rectangularShape(_nimbus->getMargin(), nimbus->getAspectRatio(), sumWidth, minWidth );

      UpdateSession::open();

      _cell->setAbutmentBox( Box ( _rootGCell->getXMin(), _rootGCell->getYMin()
                                 , _rootGCell->getXMax(), _rootGCell->getYMax()) );
#if 0
      Point center = box.getCenter();
      forEach ( Instance*, iinstance, cell->getInstances() ) {
        iinstance->setPlacementStatus ( Instance::PlacementStatus::PLACED );
        setStep(**iinstance, 0);

        Box       insABox    = iinstance->getAbutmentBox();
        Box       masterABox = iinstance->getMasterCell()->getAbutmentBox();
        DbU::Unit xPos       = center.getX();
        DbU::Unit yPos       = center.getY();

        instance->setTransformation
          ( getTransformation ( masterABox
                              , xPos - insABox.getHalfWidth()
                              , yPos - insABox.getHalfHeight()
                              , Transformation::Orientation::ID
                              ) );
      }
#endif
      UpdateSession::close();
    } else {
      Box box = _cell->getBoundingBox();
            
      UpdateSession::open();
      _cell->setAbutmentBox ( box );
      _rootGCell = GCell::create(_nimbus, 0, box);
      UpdateSession::close();
    }
  } else {
    cmess1 << "     - Design abutment box found: " << _cell->getAbutmentBox() << endl;
    _rootGCell = GCell::create(_nimbus, 0, _cell->getAbutmentBox());
  }
  _rootGCell->setAsPlacementLeaf();
  _rootGCell->setAsRoutingLeaf();
}

Grid::~Grid ()
//************
{
    _rootGCell->destroy();
}

#if 0
void Grid::Clear()
// **************
{
    _oldBoxes.clear();
    _newBoxes.clear();

    return;
}
#endif

  void Grid::rectangularShape(double margin, double aspectRatio, double sumWidth, DbU::Unit minWidth)
  // ************************************************************************************************
  {
    cmess1 << "  o  Creating abutment box: (margin: " << margin
           << ", aspect ratio:" << aspectRatio << ")" << endl;

    double     dMinWidth   = DbU::getLambda(minWidth);
    double     rowHeight   = DbU::getLambda(_nimbus->getSliceHeight());
    DbU::Unit  pitch       = _nimbus->getPitch();
    double     marginWidth = (1.0+margin) * sumWidth;

    // cerr << "sumWidth:"   << sumWidth
    //      << " rowHeight:" << rowHeight
    //      << " pitch:"     << DbU::getValueString(pitch)
    //      << " minWidth:"  << minWidth
    //      << endl;

  // AR = x/y    S = x*y = marginWidth*SH    x=S/y    AR = S/y^2
  // y = sqrt(S/AR)

    double       y    = sqrt ( marginWidth*rowHeight ) / aspectRatio;
    unsigned int rows = (unsigned int)(y / rowHeight);

    if ( rows == 0 ) ++rows;
    double rowWidth = marginWidth / rows;

  //cerr << "y:" << y << " rows:" << rows << endl;

    if ( rowWidth < dMinWidth ) {
      rowWidth = dMinWidth;
      rows     = (unsigned int)(marginWidth / rowWidth);

      if ( rows == 0 ) ++rows;
    }

    DbU::Unit unitRowWidth = DbU::lambda(rowWidth);
    DbU::Unit adjustWidth  = unitRowWidth % pitch;
    if ( adjustWidth != 0 )
      unitRowWidth += pitch - adjustWidth;

    Box ab = Box ( 0, 0, unitRowWidth, rows * _nimbus->getSliceHeight() );

#if DEPRECATED
    if ( nrows == 0 ) {
      nrows = (int)(sqrt((1.0 + margin) * sumwidth / rowHeight) + 0.5);
    }

    DbU::Unit rowWidth
      = DbU::lambda
      ( (int)((double)((int)((1.0 + margin) * DbU::getLambda(sumwidth)
                            / ((double)nrows * DbU::getLambda(pitch)) ))
             * DbU::getLambda(pitch) + 0.5)
      );

    Box ab = Box(0, 0, rowWidth, nrows * rowHeight);
#endif

    cmess1 << "     - Abutment box: " << ab << endl;

    _rootGCell = GCell::create ( _nimbus, (unsigned)0, ab );
  }

void Grid::horizontalLongSplit (unsigned step, DbU::Unit& Y) const
//***************************************************************
{
    //Clear();

    recLS (step, Y, RG_HORIZONTAL, _rootGCell);
    return;
}

void Grid::verticalLongSplit (unsigned step, DbU::Unit& X) const
//*************************************************************
{
    //Clear();

    recLS (step, X, RG_VERTICAL, _rootGCell);
    return;
}


void Grid::recLS (unsigned step, DbU::Unit& coord, unsigned direction, GCell* gcell) const
//*************************************************************************************
{
    //cout << " recLS : " << step << "-" << gcell->getStep() << endl;

    if (gcell->getStep() >= step) {

        return;
    }
    
    if (gcell->getStep() == step - 1) {

        if (!(gcell->hasSubGCells())) {
            gcell->makeSub();
        }

        if (direction == RG_HORIZONTAL) {
            //cout << "subHSplit" << endl;
            gcell->subHSplit (coord);
            //cout << "subHSplitted" << endl;
        } else if (direction == RG_VERTICAL) {
            //cout << "subVSplit" << endl;
            gcell->subVSplit (coord);
            //cout << "subVSplitted" << endl;
        } else {
            cerr << "Nothing to do here" << endl;
            abort();
        }

        return;

    } else { // exploration récursive

        if (!(gcell->hasSubGCells())) {
            gcell->makeSub();
        }

        for_each_gcell (nb, gcell->getSubGCells()) {
            if (direction == RG_HORIZONTAL) {
                if (
                        nb->horizontalIsCrossed (coord)
                   )
                {
                    recLS (step, coord, direction, nb);
                }
            } else if (direction == RG_VERTICAL) {
                if (
                        nb->verticalIsCrossed (coord)
                   )
                {
                    recLS (step, coord, direction, nb);
                }
            } else {
                cerr << "Nothing to do here" << endl;
                abort();
            }

            end_for;
        }
    }

    return;
}

Layer* Grid::getLayer () const {

    return _layer;
}

GCell* Grid::recGetGCell (unsigned step, Point& p, GCell* gcell) const {
    GCell* res = NULL;

    if (! (gcell->contains(p))) {
        return NULL;
    }
    
    if (gcell->getStep() == step) {
        return gcell;
        
    } else {
        for_each_gcell (subBox, gcell->getSubGCells()) {

            res = recGetGCell (step, p, subBox);

            if (res)
                break;
                //return res;
            end_for;
        }
    }

    assert (res);

    return res;
} // recGetGCell

GCell* Grid::getGCell (unsigned step, Point& p) const
{

    return recGetGCell (step, p, _rootGCell);
}

const Box Grid::getBox (unsigned step, Point& p) const
{

    GCell* gcell = getGCell (step, p);
    return Box (gcell->getXMin(), gcell->getYMin(), gcell->getXMax(), gcell->getYMax());
}

Point Grid::getPosition (unsigned step, Point& pos) const
{
    
    return (getGCell(step,pos)->getCenter());
}

DbU::Unit Grid::getX (unsigned step, DbU::Unit& X) const
{
    Point p = Point (X, DbU::lambda (0));
    return getGCell (step, p)->getXCenter();
}

DbU::Unit Grid::getY (unsigned step, DbU::Unit& Y) const
{
    Point p = Point (DbU::lambda (0), Y);
    return getGCell (step, p)->getYCenter();
}


void Grid::recDumpGrid (GCell* gcell) const{

    cout << "     Step : " << gcell->getStep() << endl;
    gcell->dumpPavement();

    for_each_gcell (subBox, gcell->getSubGCells()) {
        recDumpGrid(subBox);
        end_for;
    }

    return;
}

void Grid::dumpGrid () const{

    cout << "  o  dumping grid..." << endl;
    recDumpGrid (_rootGCell);
    cout << "     - grid dumped." << endl;

    return;
}

/*
 * ********************************************************************
 * Collection des boites du niveau step
 */
GCells Grid::getGCells (unsigned step) const
{
    return Grid_GCells(this, step);
}
GCells Grid::getFastGCells (unsigned step) const
{
    return Grid_FastGCells(this, step);
}

/*
 * ********************************************************************
 *
 * Création des Frontières
 */
void Grid::createFences (unsigned step)
{
    _fences[step] = new FenceSet();

    for_each_gcell (gcell, getGCells(step)) {

        //cmess1 << gcell << endl;

        if (!gcell->getLeftFence()) {
            GCell* nbleft = gcell->computeLeftOfMe();
            if (nbleft) {
                Fence* fence = static_cast<Fence*> (VFence::create (this, nbleft, gcell));
                gcell->getContainer()->addInnerFence(fence);
                fence->setStep(step);
                _fences[step]->_insert(fence);
            }
                
        }
        if (!gcell->getRightFence()) {
            GCell* nbright = gcell->computeRightOfMe();
            if (nbright) {
                Fence* fence = static_cast<Fence*> (VFence::create (this, gcell, nbright));
                gcell->getContainer()->addInnerFence(fence);
                fence->setStep(step);
                _fences[step]->_insert(fence);
            }
        }
        if (!gcell->getUpFence()) {
            GCell* nbup = gcell->computeUpOfMe();
            if (nbup) {
                Fence* fence = static_cast<Fence*> (HFence::create (this, nbup, gcell));
                gcell->getContainer()->addInnerFence(fence);
                fence->setStep(step);
                _fences[step]->_insert(fence);
            }
        }
        if (!gcell->getDownFence()) {
            GCell* nbdown = gcell->computeDownOfMe();
            if (nbdown) {
                Fence* fence = static_cast<Fence*> (HFence::create (this, gcell, nbdown));
                gcell->getContainer()->addInnerFence(fence);
                fence->setStep(step);
                _fences[step]->_insert(fence);
            }
        }
        end_for;
    }
    return;
}

/*
 * ********************************************************************
 *
 * Collection des frontieres pour un niveau
 */
Fences Grid::getFences (unsigned step)
{
    return _fences[step]->getElements();
}

Fence* Grid::getFenceBetween (GCell* nb1, GCell* nb2) const
{
    Fence* fence = testFenceBetween (nb1, nb2);
    if (!fence)
    {
        cerr << "no fence between " << nb1 << " and " << nb2 << "." << endl;
        abort();
        throw Error ("getFenceBetween : gcelles are not linked together");
    }

    return fence;
}

Fence* Grid::testFenceBetween (GCell* nb1, GCell* nb2) const
{
    if (nb1 == nb2)
    {
        return NULL;
#if 0
        abort();
        throw Error ("getFenceBetween : same gcelles");
#endif
    }
    if (nb1->getStep() != nb2->getStep())
    {
        return NULL;
#if 0
        abort();
        throw Error ("getFenceBetween : gcelles from different grid levels");
#endif
    }
    for_each_fence (fence, nb1->getSurroundingFences())
    {
        if (fence->getOppositeGCell (nb1) == nb2)
            return fence;

        end_for;
    }

    return NULL;
}


// Compat

Grid* Grid::create (unsigned n, DbU::Unit& width, DbU::Unit& height) {

    throw Error ("Nimbus: Grid: entering an outdated and deactivated function");
    return NULL;
}


Box Grid::getGRBox (unsigned step, Point& p) {

    return (getBox (step, p));
}

Contact* Grid::getContact (Net& net, Point& position) {

    for_each_contact (contact, net.getContacts()) {

        if (contact->getPosition() == position) {
            return contact;
        } else {
            // vide
        }
   
        end_for;
    }

    return NULL;
}

Contact* Grid::getOrCreateContact (Net& net, Point& position, DbU::Unit width, DbU::Unit height) {

    Contact* c = NULL;

    c = getContact (net, position);

    if (!c)
        c = Contact::create (&net, getLayer(), position.getX(), position.getY(), width, height);

    return c;
}
#if 0
Contact* Grid::getOrCreateContact (Net& net, Point& position) {

    Contact* c = NULL;

    c = getContact (net, position);

    if (!c)
        c = Contact::create (&net, getLayer(), position.getX(), position.getY(), DbU::lambda(5), DbU::lambda(5));

    return c;
}
#endif

DbU::Unit Grid::getVSplitterHalfLength (unsigned step) {

    return DbU::lambda (2);
}

DbU::Unit Grid::getHSplitterHalfLength (unsigned step) {

    return DbU::lambda (2);
}

Point Grid::getSubUpLeft (unsigned step, Point& pos) {

    GCell* gcell = getGCell (step, pos);
    return gcell->getSubUpperLeft()->getCenter();
}
Point Grid::getSubUpRight (unsigned step, Point& pos) {

    GCell* gcell = getGCell (step, pos);
    assert (gcell);
    assert (gcell->getSubUpperRight());
    return gcell->getSubUpperRight()->getCenter();
}
Point Grid::getSubDownLeft (unsigned step, Point& pos) {

    GCell* gcell = getGCell (step, pos);
    return gcell->getSubBottomLeft()->getCenter();
}
Point Grid::getSubDownRight (unsigned step, Point& pos) {

    GCell* gcell = getGCell (step, pos);
    return gcell->getSubBottomRight()->getCenter();
}

GCell* Grid::getLeftOf (const GCell* gcell) const
{
    GCell* nbl = gcell->getLeftOfMe();
    if (!nbl) nbl = gcell->computeLeftOfMe();

    return nbl;
}
GCell* Grid::getRightOf (const GCell* gcell) const
{
    GCell* nbr = gcell->getRightOfMe();
    if (!nbr) nbr = gcell->computeRightOfMe();

    return nbr;
}
GCell* Grid::getUpOf (const GCell* gcell) const
{
    GCell* nbu = gcell->getUpOfMe();
    if (!nbu) nbu = gcell->computeUpOfMe();

    return nbu;
}
GCell* Grid::getDownOf (const GCell* gcell) const
{
    GCell* nbd = gcell->getDownOfMe();
    if (!nbd) nbd = gcell->computeDownOfMe();

    return nbd;
}

GCell* Grid::getUpperLeftCorner (unsigned step) const
{
    GCell* gcell = getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        gcell = gcell->getSubUpperLeft();
        if (!gcell) throw Error ("Nimbus crushed when diving too deep.");
    }
    return gcell;
}
GCell* Grid::getUpperRightCorner (unsigned step) const
{
    GCell* gcell = getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        gcell = gcell->getSubUpperRight();
        if (!gcell) throw Error ("Nimbus crushed when diving too deep.");
    }
    return gcell;
}
GCell* Grid::getLowerLeftCorner (unsigned step) const
{
    GCell* gcell = getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        gcell = gcell->getSubLowerLeft();
        if (!gcell) throw Error ("Nimbus crushed when diving too deep.");
    }
    return gcell;
}
GCell* Grid::getLowerRightCorner (unsigned step) const
{
    GCell* gcell = getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        gcell = gcell->getSubLowerRight();
        if (!gcell) throw Error ("Nimbus crushed when diving too deep.");
    }
    return gcell;
}







/*
 * ********************************************************************
 *
 * Grid_GCells implementation
 */

Grid_GCells::Grid_GCells(const Grid* grid, unsigned step)
// ************************************************************************
:       Inherit()
	, _nb(NULL)
	, _step(step)
{
    _nb = grid->getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        _nb = _nb->getSubUpperLeft();
        if (!_nb) throw Error ("Nimbus crushed when diving too deep.");
    }
}
 
Grid_GCells::Grid_GCells(const Grid_GCells& gcelles)
// ************************************************************************
:       Inherit()
        , _nb(gcelles._nb)
        , _step(gcelles._step)
{
}
 
Grid_GCells& Grid_GCells::operator=(const Grid_GCells& gcelles)
// ***********************************************************************************
{
        _nb   = gcelles._nb;
        _step = gcelles._step;
        return *this;
}
 
Collection<GCell*>* Grid_GCells::getClone() const
// ***********************************************
{
        return new Grid_GCells(*this);
}
 
Locator<GCell*>* Grid_GCells::getLocator() const
// ******************************************************
{
        return new Locator(_nb);
}
 
string Grid_GCells::_getString() const
// *************************************
{
        string s = "<" + _TName("GCell::GCells");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Grid_GCells::Locator implementation
 */
Grid_GCells::Locator::Locator(GCell* gcell)
// ********************************************************
:       Inherit(),
        _nb(gcell),
        _direction(0)
{
}
 
Grid_GCells::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _nb(locator._nb),
        _direction(locator._direction)
{
}
 
Grid_GCells::Locator& Grid_GCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _nb = locator._nb;
        _direction = locator._direction;
        return *this;
}
 
GCell* Grid_GCells::Locator::getElement() const
// *********************************************
{
        return _nb;
}
 
Locator<GCell*>* Grid_GCells::Locator::getClone() const
// *****************************************************
{
        return new Locator(*this);
}
 
bool Grid_GCells::Locator::isValid() const
// *****************************************
{
        return (_nb != NULL);
}

void Grid_GCells::Locator::progress()
// ************************************
{
    GCell* gcell = NULL;

    switch (_direction) {

        case 0:
            gcell = _nb->computeRightOfMe();
            if (!gcell) {
                gcell = _nb->computeDownOfMe();
                _direction = 1;
            }
            break;
        case 1:
            gcell = _nb->computeLeftOfMe();
            if (!gcell) {
                gcell = _nb->computeDownOfMe();
                _direction = 0;
            }
            break;
        default:
            gcell = NULL;
            break;
    }

    _nb = gcell;
}
 
string Grid_GCells::Locator::_getString() const
// **********************************************
{
        string s = "<" + _TName("GCells::GCells::Locator");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}



/*
 * ********************************************************************
 * Grid_FastGCells implementation
 */
Grid_FastGCells::Grid_FastGCells(const Grid* grid, unsigned step)
// **************************************************************************************
:       Inherit()
        , _nb(NULL)
        , _step(step)
{
    _nb = grid->getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        _nb = _nb->getSubUpperLeft();
        if (!_nb) throw Error ("Nimbus crushed when diving too deep.");
    }
}
 
Grid_FastGCells::Grid_FastGCells(const Grid_FastGCells& gcelles)
// ************************************************************************************
:       Inherit()
        , _nb(gcelles._nb)
        , _step(gcelles._step)
{
}
 
Grid_FastGCells& Grid_FastGCells::operator=(const Grid_FastGCells& gcelles)
// ****************************************************************
{
        _nb = gcelles._nb;
        _step = gcelles._step;
        return *this;
}
 
Collection<GCell*>* Grid_FastGCells::getClone() const
// ***********************************************
{
        return new Grid_FastGCells(*this);
}
 
Locator<GCell*>* Grid_FastGCells::getLocator() const
// **********************************************
{
        return new Locator(_nb);
}
 
string Grid_FastGCells::_getString() const
// *************************************
{
        string s = "<" + _TName("GCell::GCells");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Grid_FastGCells::Locator implementation
 */
Grid_FastGCells::Locator::Locator(GCell* gcell)
// ********************************************************
:       Inherit(),
        _nb(gcell),
        _direction(0)
{
}
 
Grid_FastGCells::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _nb(locator._nb),
        _direction(locator._direction)
{
}
 
Grid_FastGCells::Locator& Grid_FastGCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _nb = locator._nb;
        _direction = locator._direction;
        return *this;
}
 
GCell* Grid_FastGCells::Locator::getElement() const
// *********************************************************
{
        return _nb;
}
 
Locator<GCell*>* Grid_FastGCells::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Grid_FastGCells::Locator::isValid() const
// **************************************************
{
        return (_nb != NULL);
}

void Grid_FastGCells::Locator::progress()
// *********************************************
{
    GCell* gcell = NULL;

    switch (_direction) {

        case 0:
            gcell = _nb->getRightOfMe();
            if (!gcell) {
                gcell = _nb->getDownOfMe();
                _direction = 1;
            }
            break;
        case 1:
            gcell = _nb->getLeftOfMe();
            if (!gcell) {
                gcell = _nb->getDownOfMe();
                _direction = 0;
            }
            break;
        default:
            gcell = NULL;
            break;
    }

    _nb = gcell;
}
 
string Grid_FastGCells::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Grid::FastGCells::Locator");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}


#if 0




/*
 * ********************************************************************
 * Grid_Fences implementation
 */
Grid_Fences::Grid_Fences(const Grid* grid, unsigned step)
// **************************************************************************************
:       Inherit()
        , _nb(NULL)
        , _fence(NULL)
        , _step(step)
{
    _nb = grid->getRoot();

    for (unsigned level = 0 ; level < step ; level++) {
        _nb = _nb->getSubUpperLeft();
        if (!_nb) throw Error ("Nimbus crushed while diving too deep.");
    }
}
 
Grid_Fences::Grid_Fences(const Grid_Fences& fences)
// ************************************************************************************
:       Inherit()
        , _nb(fences._nb)
        , _fence(fences._nb)
        , _step(fences._step)
{
}
 
Grid_Fences& Grid_Fences::operator=(const Grid_Fences& fences)
// ****************************************************************
{
        _nb = ._nb;
        _step = ._step;
        return *this;
}
 
Collection<Fence*>* Grid_Fences::getClone() const
// ***********************************************
{
        return new Grid_Fences(*this);
}
 
Locator<Fence*>* Grid_Fences::getLocator() const
// **********************************************
{
        return new Locator(_nb);
}
 
string Grid_Fences::_getString() const
// *************************************
{
        string s = "<" + _TName("Fence::Fencees");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Grid_Fences::Locator implementation
 */
Grid_Fences::Locator::Locator(Fence* gcell)
// ********************************************************
:       Inherit(),
        _nb(gcell),
        _direction(0)
{
}
 
Grid_Fences::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _nb(locator._nb),
        _direction(locator._direction)
{
}
 
Grid_Fences::Locator& Grid_Fences::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _nb = locator._nb;
        _direction = locator._direction;
        return *this;
}
 
Fence* Grid_Fences::Locator::getElement() const
// *********************************************************
{
        return _nb;
}
 
Locator<Fence*>* Grid_Fences::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Grid_Fences::Locator::isValid() const
// **************************************************
{
        return (_nb != NULL);
}

void Grid_Fences::Locator::progress()
// *********************************************
{
    Fence* gcell = NULL;

    switch (_direction) {

        case 0:
            gcell = _nb->getLeftOfMe();
            if (!gcell) {
                gcell = _nb->getDownOfMe();
                _direction = 1;
            }
            break;
        case 1:
            gcell = _nb->getRightOfMe();
            if (!gcell) {
                gcell = _nb->getDownOfMe();
                _direction = 0;
            }
            break;
        default:
            gcell = NULL;
            break;
    }

    _nb = gcell;
}
 
string Grid_Fences::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Grid::FastFencees::Locator");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}

#endif



/*
 * FenceSet implementation
 *
 */
Grid::FenceSet::FenceSet()
// **********************
    : Inherit()
{
}

unsigned Grid::FenceSet::_getHashValue(Fence* fence) const
{
    return ( (unsigned int)( (unsigned long)fence ) ) / 2;
}

Fence* Grid::FenceSet::_getNextElement(Fence* fence) const
{
    return fence->_getNextOfGridFenceSet();
}

void Grid::FenceSet::_setNextElement(Fence* fence, Fence* nextFence) const
{
    fence->_setNextOfGridFenceSet(nextFence);
    return;
}


}
