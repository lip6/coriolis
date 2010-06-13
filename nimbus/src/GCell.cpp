
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>

#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "crlcore/CellGauge.h"
#include "nimbus/VFence.h"
#include "nimbus/HFence.h"
#include "nimbus/GCell.h"
#include "nimbus/NimbusEngine.h"

namespace Nimbus {


/*
 * ********************************************************************
 * GCell_Fences declaration
 * ********************************************************************
 */
class GCell_Fences : public Collection<Fence*> {
    
    public: typedef Collection<Fence*> Inherit;

    public: class Locator : public Hurricane::Locator<Fence*> {

                public: typedef Hurricane::Locator<Fence*> Inherit;

                private: const GCell* _nb;
                private: Fence* _fence;

                public: Locator(const GCell* gcell = NULL, Fence* fence = NULL);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual Fence* getElement() const;
                public: virtual Hurricane::Locator<Fence*>* getClone() const;

                public: virtual bool isValid() const;

                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const GCell* _nb;
    private: Fence* _fence;

    public: GCell_Fences(const GCell* gcell);
    public: GCell_Fences(const GCell_Fences& fences);

    public: GCell_Fences& operator=(const GCell_Fences& fences);
            
    public: virtual Collection<Fence*>* getClone() const;
    public: virtual Hurricane::Locator<Fence*>* getLocator() const;

    public: virtual string _getString() const;
};


    

/*
 * ********************************************************************
 * Implementation of GCell
 *
 */

GCell::GCell (NimbusEngine* nimbus, unsigned step, const Box& box, GCell* container)
// ***************************************************************************
    : Inherit ()
    , _nimbus (nimbus)
    , _box(box)
    , _step (step)
    , _container (container)
    , _nextOfGCellGCellSet (NULL)
    , _subGCells()
    , _subFences()
    , _upfence (NULL)
    , _downfence (NULL)
    , _leftfence (NULL)
    , _rightfence (NULL)
    , _isAPlacementLeaf(false)
    , _isARoutingLeaf(false)
{}


GCell* GCell::create(NimbusEngine* nimbus, unsigned step, const Box& box, GCell* container)
{
    if (!nimbus)
        throw Error("Can't create GCell : empty nimbus");

    GCell* gcell = new GCell(nimbus, step, box, container);
    gcell->_postCreate();
    return gcell;
}

void GCell::_preDestroy ()
// ******************
{
    if (_upfence)
        _upfence->destroy();
    if (_downfence)
        _downfence->destroy();
    if (_leftfence)
        _leftfence->destroy();
    if (_rightfence)
        _rightfence->destroy();

    if (_container) {
        _container->removeSubGCell (this);
    }

    for_each_gcell (gcell, getSubGCells())
    {
        gcell->destroy();
        end_for;
    }

    for (RBPavement::iterator rbpit = _pavement.begin();
            rbpit != _pavement.end();
            rbpit++)
    {
        delete rbpit->second;
    }
}

  Cell* GCell::getCell () const
  // **************************
  {
    return _nimbus->getCell();
  }

void GCell::setXMax (DbU::Unit x)
//**************************
{
    DbU::Unit diff = x - getXMax();
    _box.inflate (0, 0, diff, 0);
    DbU::Unit width = getXMax() - getXMin();
    if (_upfence)   { _upfence   ->setSize(width); _upfence   ->computeCapacity(); }
    if (_downfence) { _downfence ->setSize(width); _downfence ->computeCapacity(); }
    return;
}

void GCell::setYMax (DbU::Unit y)
//**************************
{
    DbU::Unit diff = y - getYMax();
    _box.inflate (0, 0, 0, diff);
    DbU::Unit height = getYMax() - getYMin();
    if (_leftfence)  { _leftfence  ->setSize(height); _leftfence  ->computeCapacity(); }
    if (_rightfence) { _rightfence ->setSize(height); _rightfence ->computeCapacity(); }
    return;
}

void GCell::setXMin (DbU::Unit x)
//**************************
{
    DbU::Unit diff = getXMin() - x;
    _box.inflate (diff, 0, 0, 0);
    DbU::Unit width = getXMax() - getXMin();
    if (_upfence)   { _upfence   ->setSize(width); _upfence   ->computeCapacity(); }
    if (_downfence) { _downfence ->setSize(width); _downfence ->computeCapacity(); }
    return;
}

void GCell::setYMin (DbU::Unit y)
// **************************
{
    DbU::Unit diff = getYMin() - y;
    _box.inflate (0, diff, 0, 0);
    DbU::Unit height = getYMax() - getYMin();
    if (_leftfence)  { _leftfence  ->setSize(height); _leftfence  ->computeCapacity(); }
    if (_rightfence) { _rightfence ->setSize(height); _rightfence ->computeCapacity(); }
    return;
}

bool GCell::hasSubGCells () const
// ******************************
{
    return (!(getSubGCells().isEmpty()));
}

bool GCell::horizontalIsCrossed (DbU::Unit& Y) const
//*********************************
{
    return (Interval (getYMin(), getYMax())).contains (Y);
}

bool GCell::verticalIsCrossed (DbU::Unit& X) const
//*********************************
{
    return (Interval (getXMin(), getXMax())).contains (X);
}

bool GCell::strictContains(const Point& point) const
// *************************************************
{
	return strictContains(point.getX(), point.getY());
}

bool GCell::strictContains(const DbU::Unit& x, const DbU::Unit& y) const
//* **********************************************************
{
    bool res = (!isEmpty()) && (getXMin() <= x) && (getYMin() <= y);
    if (getRightFence()) res &= (x < getXMax()); else res &= (x <= getXMax());
    if (getUpFence())    res &= (y < getYMax()); else res &= (y <= getYMax());
    return res;
}

void GCell::makeSub ()
// *******************
{
    assert (!hasSubGCells());
    Box box (getXMin(), getYMin(), getXMax(), getYMax());
    GCell* gcell = new GCell (_nimbus, _step + 1, box, this);
    addSubGCell (gcell);
    return;
}

void GCell::subVSplit (DbU::Unit& X)
// ****************************
{
    RBList tmp;
    getSubGCells().fill(tmp);

    for (
            RBList::iterator lrbit = tmp.begin() ;
            lrbit != tmp.end() ;
            lrbit++
        )
    {
        if ( (*lrbit)->hasSubGCells() ) {
            throw Error("Nimbus: unable to split a gcell");
            cerr << "Don't know how to split" << endl;
        }

        if ( (*lrbit)->verticalIsCrossed(X) ) {

            RBPair subs = (*lrbit)->vSplitMe (X);
            //removeSubGCell ( (*lrbit) );
            (*lrbit)->destroy();
            addSubGCell (subs.first);
            addSubGCell (subs.second);
        }
    }

    return;
}

void GCell::subHSplit (DbU::Unit& Y)
// ****************************
{
    RBList tmp;
    getSubGCells().fill(tmp);

    for (
            RBList::iterator lrbit = tmp.begin() ;
            lrbit != tmp.end() ;
            lrbit++
        )
    {
        if ( (*lrbit)->hasSubGCells() ) {
            throw Error("Nimbus: unable to split a gcell");
            cerr << "Don't know how to split" << endl;
        }

        if ( (*lrbit)->horizontalIsCrossed(Y) ) {

            RBPair subs = (*lrbit)->hSplitMe (Y);
            //removeSubGCell ( (*lrbit) );
            (*lrbit)->destroy();
            addSubGCell (subs.first);
            addSubGCell (subs.second);
        }
    }

    return;
}

pair <GCell*, GCell*> GCell::vSplitMe (DbU::Unit& coord)
// ************************************************
{
    Box b1 = Box
        (  getXMin()
         , getYMin()
         , coord
         , getYMax()
        );
    Box b2 = Box
        (  coord //+ getUnit (5)
         , getYMin()
         , getXMax()
         , getYMax()
        );

    GCell* rb1 = new GCell (_nimbus, _step, b1, _container);
    GCell* rb2 = new GCell (_nimbus, _step, b2, _container);

    assert ( rb1 && rb2 ); 

    return pair<GCell*, GCell*> (rb1, rb2);
}

pair <GCell*, GCell*> GCell::hSplitMe (DbU::Unit& coord)
// ************************************************
{
    Box b1 = Box
        (  getXMin()
         , getYMin()
         , getXMax()
         , coord
        );
    Box b2 = Box
        (  getXMin()
         , coord
         , getXMax()
         , getYMax()
        );

    GCell* rb1 = new GCell (_nimbus, _step, b1, _container);
    GCell* rb2 = new GCell (_nimbus, _step, b2, _container);

    assert ( rb1 && rb2 ); 

    return pair<GCell*, GCell*> (rb1, rb2);
}


void GCell::addSubGCell (GCell* gcell)
// ***********************************
{
    _subGCells._insert (gcell);
    addToPavement (gcell);
    
    return;
}

void GCell::addInnerFence (Fence* fence)
//* ************************************
{
    if (
            fence->getGCell1()->getContainer()
            ==
            fence->getGCell2()->getContainer()
       )
    {
        _subFences._insert (fence);
    }

    return;
}

bool GCell::isAStrictSubGCellOf (GCell* gcell) const
// *************************************************
{
    if ( (gcell == this) || (gcell == NULL)) return false;
    if ( ! getContainer() ) return false;
    return getContainer()->isASubGCellOf(gcell);
}

bool GCell::isASubGCellOf (GCell* gcell) const
// *******************************************
{
    if (gcell == this) return true;
    GCell* container = getContainer();
    if (!container) return false;
    return container->isASubGCellOf (gcell);
}

void GCell::setAsPlacementLeaf()
// *****************************
{
    recFlushSubPlacementLeaves (this);
    _nimbus->setPlacementLeaf(this);
    _isAPlacementLeaf = true;
    GCell* nb = getContainer();
    while (nb)
    {
        _nimbus->_removePlacementLeaf(nb);
        nb->_setNotPlacementLeaf();
        nb = nb->getContainer();
    }
#if 0
    if (getContainer())
        getContainer()->_setNotPlacementLeaf();
#endif
    for_each_fence(fence, getSurroundingFences())
    {
        fence->setVisible();
        if (fence->getParentFence())
            fence->getParentFence()->setInvisible();
        end_for;
    }
    return;
}

void GCell::recFlushSubPlacementLeaves(GCell* gcell)
// *************************************************
{
    for_each_gcell(nb, gcell->getSubGCells())
    {
        nb->_setNotPlacementLeaf();
        _nimbus->_removePlacementLeaf(nb);
        recFlushSubPlacementLeaves(nb);
        end_for;
    }
    return;
}

void GCell::setSubGCellsAsPlacementLeaves()
// ****************************************
{
    for_each_gcell(gcell, getSubGCells())
    {
        gcell->setAsPlacementLeaf();
        end_for;
    }
    return;
}

void GCell::setAsRoutingLeaf()
// ***************************
{
    recFlushSubRoutingLeaves (this);
    _nimbus->setRoutingLeaf(this);
    _isARoutingLeaf = true;
    GCell* nb = getContainer();
    while (nb)
    {
        _nimbus->_removeRoutingLeaf(nb);
        nb->_setNotRoutingLeaf();
        nb = nb->getContainer();
    }
    return;
}

void GCell::recFlushSubRoutingLeaves(GCell* gcell)
// **********************************************
{
    for_each_gcell(nb, gcell->getSubGCells())
    {
        nb->_setNotRoutingLeaf();
        _nimbus->_removeRoutingLeaf(nb);
        recFlushSubRoutingLeaves(nb);
        end_for;
    }
    return;
}

double GCell::testMargin()
//* **********************
{
  DbU::Unit sliceHeight = _nimbus->getConfiguration()->getSliceHeight();
    if ( isEmpty() || _box.isPonctual() || _box.isFlat() )
        throw Error ( "Very very strange GCell" );
    if ( getHeight() % sliceHeight )
        throw Error("GCell Height must be a multiple of Slice Height");
    unsigned nRows = getHeight() / sliceHeight;
    DbU::Unit areaTotalWidth = getWidth() * nRows;
    DbU::Unit instanceWidthSum = 0;
    for_each_instance ( instance, _nimbus->getCell()->getInstancesUnder ( this->_box ) ) {
        instanceWidthSum += instance->getMasterCell()->getAbutmentBox().getWidth();
        end_for;
    }
    double margin = 1.0 - DbU::getLambda(instanceWidthSum) / DbU::getLambda(areaTotalWidth);
    return margin;
}

void GCell::setSubGCellsAsRoutingLeaves()
// **************************************
{
    for_each_gcell(gcell, getSubGCells())
    {
        gcell->setAsRoutingLeaf();
        end_for;
    }
    return;
}

GCell* GCell::getRoutingLeaf()
// ***************************
{
    if (isARoutingLeaf()) return this;
    if (getContainer()) return getContainer()->getRoutingLeaf();
    return NULL;
}

GCell* GCell::getPlacementLeaf()
// *****************************
{
    if (isAPlacementLeaf()) return this;
    if (getContainer()) return getContainer()->getPlacementLeaf();
    return NULL;
}

void GCell::removeSubGCell (GCell* gcell)
// **************************************
{
    _subGCells._remove (gcell);
    removeFromPavement (gcell);

    return;
}

// Gestion du pavage

void GCell::addToPavement (GCell* gcell) {

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    if (_pavement.find(X) == _pavement.end()) {
        _pavement[X] = new RBMap;
    }

    (*(_pavement[X]))[Y] = gcell;

    return;
}

void GCell::removeFromPavement (GCell* gcell) {

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    RBMap* col = (_pavement[X]);
    assert (col);

    RBMap::iterator rbrm = col->find(Y);
    assert (rbrm != col->end());

    col->erase (rbrm);

    if (col->empty())
        _pavement.erase (_pavement.find(X));

    return;
}

void GCell::dumpPavement () const
// ********************************
{
    for (
            RBPavement::const_iterator rbpit = _pavement.begin() ;
            rbpit != _pavement.end()                       ;
            rbpit++
        )
    {
        cout << "     - Line " << (*rbpit).first << " :";

        for (
                RBMap::iterator rbmit = ((*rbpit).second)->begin() ;
                rbmit != ((*rbpit).second)->end()                  ;
                rbmit++
            )
        {
            cout << " (" << (*rbmit).first << ", " << (*rbmit).second << ")";
        }

        cout << endl;
    }

    return;
} // dumpPavement

///////////////////////////////////////////////////////////////////////
// a droite
//
GCell* GCell::_computeSubRightOf (const GCell* gcell) const
{
    // "X + 1"
    assert (gcell->getContainer() == this);

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    RBPavement::const_iterator col = _pavement.find(X);
    assert (col != _pavement.end());

    col++;

    if (col == _pavement.end())
        return NULL;

    RBMap::const_iterator rbit = ((*col).second)->find(Y);
    assert (rbit != ((*col).second)->end());

    return (*rbit).second;
}

GCell* GCell::getSubRightOf (GCell* gcell) const
{
    // "X + 1"
    assert (gcell->getContainer() == this);

    if (gcell->getRightFence())
        return (gcell->getRightFence()->getRightGCell());

    GCell* nb = _computeSubRightOf (gcell);
#if 0
    if (gcell) {
        VFence* vfence = VFence::create (gcell, gcell);
        assert (vfence);
        gcell->setRightFence(vfence);
        gcell->setLeftFence(vfence);
    }
#endif

    return nb;
}

///////////////////////////////////////////////////////////////////////
// a gauche
//
GCell* GCell::_computeSubLeftOf (const GCell* gcell) const
{
    // "X - 1"

    assert (gcell->getContainer() == this);

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    RBPavement::const_iterator col = _pavement.find(X);
    assert (col != _pavement.end());

    if (col == _pavement.begin())
        return NULL;

    col--;

    RBMap::const_iterator rbit = ((*col).second)->find(Y);
    assert (rbit != ((*col).second)->end());

    return (*rbit).second;
}

GCell* GCell::getSubLeftOf (GCell* gcell) const
{
    // "X - 1"

    assert (gcell->getContainer() == this);

    if (gcell->getLeftFence())
        return (gcell->getLeftFence()->getLeftGCell());

    GCell* nb = _computeSubLeftOf (gcell);
#if 0
    if (gcell) {
        VFence* vfence = new VFence (gcell, gcell);
        assert (vfence);
        gcell->setLeftFence(vfence);
        gcell->setRightFence(vfence);
    }
#endif

    return nb;
}

///////////////////////////////////////////////////////////////////////
// en bas
//
GCell* GCell::_computeSubDownOf (const GCell* gcell) const
{
    // "Y - 1"

    assert (gcell->getContainer() == this);

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    RBPavement::const_iterator col = _pavement.find(X);
    assert (col != _pavement.end());

    RBMap::const_iterator rbit = ((*col).second)->find(Y);
    assert (rbit != ((*col).second)->end());

    if (rbit == ((*col).second)->begin())
        return NULL;

    rbit--;

    return (*rbit).second;
}

GCell* GCell::getSubDownOf (GCell* gcell) const
{
    // "Y - 1"

    assert (gcell->getContainer() == this);

    if (gcell->getDownFence())
        return (gcell->getDownFence()->getDownGCell());

    GCell* nb = _computeSubDownOf (gcell);
#if 0
    if (gcell) {
        HFence* hfence = new HFence (gcell, gcell);
        assert (hfence);
        gcell->setDownFence(hfence);
        gcell->setUpFence(hfence);
    }
#endif

    return nb;
}

///////////////////////////////////////////////////////////////////////
// en haut
//
GCell* GCell::_computeSubUpOf (const GCell* gcell) const
{
    // "Y + 1"

    assert (gcell->getContainer() == this);

    DbU::Unit X = gcell->getXCenter();
    DbU::Unit Y = gcell->getYCenter();

    RBPavement::const_iterator col = _pavement.find(X);
    assert (col != _pavement.end());

    RBMap::const_iterator rbit = ((*col).second)->find(Y);
    assert (rbit != ((*col).second)->end());

    rbit++;

    if (rbit == ((*col).second)->end())
        return NULL;

    return (*rbit).second;
}
GCell* GCell::getSubUpOf (GCell* gcell) const
{
    // "Y + 1"

    assert (gcell->getContainer() == this);

    if (gcell->getUpFence())
        return (gcell->getUpFence()->getUpGCell());

    GCell* nb = _computeSubUpOf (gcell);
#if 0
    if (gcell) {
        HFence* hfence = new HFence (gcell, gcell);
        assert (hfence);
        gcell->setUpFence(hfence);
        gcell->setDownFence(hfence);
    }
#endif

    return nb;
}

GCell* GCell::getSubUpperLeft () const
{
    if (!hasSubGCells())
        return NULL;

    RBPavement::const_iterator col = _pavement.begin();
    assert (col != _pavement.end());

    RBMap::const_reverse_iterator rbit = ((*col).second)->rbegin();
    RBMap::const_reverse_iterator rbend = ((*col).second)->rend();
    assert (rbit != rbend);

    return (*rbit).second;
}

GCell* GCell::getSubUpperRight () const
{
    if (!hasSubGCells())
        return NULL;

    RBPavement::const_reverse_iterator col = _pavement.rbegin();
    assert (col != _pavement.rend());

    RBMap::const_reverse_iterator rbit = ((*col).second)->rbegin();
    RBMap::const_reverse_iterator rbend = ((*col).second)->rend();
    assert (rbit != rbend);

    return (*rbit).second;
}

GCell* GCell::getSubBottomRight () const
{
    if (!hasSubGCells())
        return NULL;

    RBPavement::const_reverse_iterator col = _pavement.rbegin();
    assert (col != _pavement.rend());

    RBMap::const_iterator rbit = ((*col).second)->begin();
    assert (rbit != ((*col).second)->end());

    return (*rbit).second;
}

GCell* GCell::getSubBottomLeft () const
{
    if (!hasSubGCells())
        return NULL;

    RBPavement::const_iterator col = _pavement.begin();
    assert (col != _pavement.end());

    RBMap::const_iterator rbit = ((*col).second)->begin();
    assert (rbit != ((*col).second)->end());

    return (*rbit).second;
}

/*
 * ********************************************************************
 * Up Down Left Right of Me !!!!
 * ********************************************************************
 *
 * ********************************************************************
 */

/*
 * ********************************************************************
 *
 * Voisin de gauche
 */

/*
 * montee/descente recursive
 */
GCell* GCell::_recGetLeftOfMe () const
{
    GCell* gcell = NULL;

    if (_container) {
        // essai de raccource par les Fence
        gcell = _container->getLeftOfMe();

        if (!gcell) {
            // essai par calcul
            gcell = _container->computeLeftOfMe();
        }
    }

    if (!gcell) return NULL;

    // on verifie au'on tient la boite a droite de notre container
    assert (gcell->getStep() == _container->getStep());
    GCell* resnb = gcell->getSubUpperRight();
    DbU::Unit Y = getYCenter();

    while ( !
            ( resnb->horizontalIsCrossed(Y) )
          )
    {
        resnb = gcell->getSubDownOf(resnb);
        if (!resnb) break;
    }

    return resnb;
}
/*
 * ********************************************************************
 */

/*
 * ********************************************************************
 *
 * Voisin de droite
 */

/*
 * rec -> remonter jusqu'a trouver une droite sinon renvoi NULL
 */
GCell* GCell::_recGetRightOfMe () const
{
    GCell* gcell = NULL;

    if (_container) {
        // essai de raccource par les Fence
        gcell = _container->getRightOfMe();

        if (!gcell) {
            // essai par calcul
            gcell = _container->computeRightOfMe();
        }
    }
    if (!gcell) return NULL;

    // on verifie au'on tient la boite a droite de notre container
    assert (gcell->getStep() == _container->getStep());
    GCell* resnb = gcell->getSubUpperLeft();
    DbU::Unit Y = getYCenter();

    while ( !
            ( resnb->horizontalIsCrossed(Y) )
          )
    {
        resnb = gcell->getSubDownOf(resnb);
        if (!resnb) break;
    }

    return resnb;
}
/*
 * ********************************************************************
 */

/*
 * ********************************************************************
 *
 * Voisin du haut
 */

/*
 * rec -> 
 */
GCell* GCell::_recGetUpOfMe () const
{
    GCell* gcell = NULL;

    if (_container) {
        // essai de raccource par les Fence
        gcell = _container->getUpOfMe();

        if (!gcell) {
            // essai par calcul
            gcell = _container->computeUpOfMe();
        }
    }
    if (!gcell) return NULL;

    // on verifie au'on tient la boite a droite de notre container
    assert (gcell->getStep() == _container->getStep());
    GCell* resnb = gcell->getSubBottomLeft();
    DbU::Unit X = getXCenter();

    while ( !
            ( resnb->verticalIsCrossed(X) )
          )
    {
        resnb = gcell->getSubRightOf(resnb);
        if (!resnb) break;
    }

    return resnb;
}
/*
 * ********************************************************************
 */

/*
 * ********************************************************************
 *
 * Voisin du bas
 */

/*
 * rec -> 
 */
GCell* GCell::_recGetDownOfMe () const
{
    GCell* gcell = NULL;

    if (_container) {
        // essai de raccource par les Fence
        gcell = _container->getDownOfMe();

        if (!gcell) {
            // essai par calcul
            gcell = _container->computeDownOfMe();
        }
    }
    if (!gcell) return NULL;

    // on verifie au'on tient la boite a droite de notre container
    assert (gcell->getStep() == _container->getStep());
    GCell* resnb = gcell->getSubUpperLeft();
    DbU::Unit X = getXCenter();

    while ( !
            ( resnb->verticalIsCrossed(X) )
          )
    {
        resnb = gcell->getSubRightOf(resnb);
        if (!resnb) break;
    }

    return resnb;
}
/*
 * ********************************************************************
 */

/*
 * ********************************************************************
 * haut bas droite gauche par calcul
 */
GCell* GCell::computeUpOfMe () const
{
    if (!_container) return NULL;

    GCell* gcell = _container->_computeSubUpOf (this);
    if (!gcell) gcell = _recGetUpOfMe();

    return gcell;
}

GCell* GCell::computeDownOfMe () const
{
    if (!_container) return NULL;

    GCell* gcell = _container->_computeSubDownOf (this);
    if (!gcell) gcell = _recGetDownOfMe();

    return gcell;
}

GCell* GCell::computeRightOfMe () const
{
    if (!_container) return NULL;

    GCell* gcell = _container->_computeSubRightOf (this);
    if (!gcell) gcell = _recGetRightOfMe();

    return gcell;
}

GCell* GCell::computeLeftOfMe () const
{
    if (!_container) return NULL;

    GCell* gcell = _container->_computeSubLeftOf (this);
    if (!gcell) gcell = _recGetLeftOfMe();

    return gcell;
}

/*
 * ********************************************************************
 *  haut bas droite gauche par les frontières
 */
GCell* GCell::getRightOfMe () const
{
    if (!_rightfence) return NULL;
    return _rightfence->getRightGCell();
}
GCell* GCell::getLeftOfMe () const
{
    if (!_leftfence) return NULL;
    return _leftfence->getLeftGCell();
}
GCell* GCell::getUpOfMe () const
{
    if (!_upfence) return NULL;
    return _upfence->getUpGCell();
}
GCell* GCell::getDownOfMe () const
{
    if (!_downfence) return NULL;
    return _downfence->getDownGCell();
}

/*
 * ********************************************************************
 * traversée de boite
 *
 */
Fence* GCell::getOppositeFence (const Fence* fence) const
{
    if (fence == _upfence)
        return _downfence;
    else if (fence == _downfence)
        return _upfence;
    else if (fence == _leftfence)
        return _rightfence;
    else if (fence == _rightfence)
        return _leftfence;
    else
        throw Error ("getOppositeFence : not a fence of mine");

    return NULL;
}













/*
 * RBList implementation
 *
 */
GCell::GCellSet::GCellSet()
// ************************
    : Inherit()
{
}

unsigned GCell::GCellSet::_getHashValue(GCell* gcell) const
{
    return ( (unsigned int)( (unsigned long)gcell ) ) / 2;
}

GCell* GCell::GCellSet::_getNextElement(GCell* gcell) const
{
    return gcell->_getNextOfGCellGCellSet();
}

void GCell::GCellSet::_setNextElement(GCell* gcell, GCell* nextGCell) const
{
    gcell->_setNextOfGCellGCellSet(nextGCell);
    return;
}


/*
 * SubFenceSet implementation
 *
 */
GCell::SubFenceSet::SubFenceSet()
// ******************************
    : Inherit()
{
}

unsigned GCell::SubFenceSet::_getHashValue(Fence* fence) const
{
    return ( (unsigned int)( (unsigned long)fence ) ) / 2;
}

Fence* GCell::SubFenceSet::_getNextElement(Fence* fence) const
{
    return fence->_getNextOfGCellSubFenceSet();
}

void GCell::SubFenceSet::_setNextElement(Fence* fence, Fence* nextFence) const
{
    fence->_setNextOfGCellSubFenceSet(nextFence);
    return;
}


string GCell::_getString() const
// ******************************
{
    if (isEmpty())
        return "<" + _TName("GCell") + " empty>";
    else
        return
          "<" + _TName("GCell") + " " + getString(_step) + " " +
          DbU::getValueString(getXMin()) + " " + DbU::getValueString(getYMin()) + " " +
          DbU::getValueString(getXMax()) + " " + DbU::getValueString(getYMax()) +
          ">";
}

Record* GCell::_getRecord() const
// ************************
{
    Record* record = Inherit::_getRecord();

    if (!record)
        record = new Record(getString(this));

    record->add(getSlot("Container", _container));
    record->add(getSlot("UpFence", _upfence));
    record->add(getSlot("DownFence", _downfence));
    record->add(getSlot("LeftFence", _leftfence));
    record->add(getSlot("RightFence", _rightfence));
    record->add(getSlot("Step", _step));
    record->add(getSlot("isAplacementLeaf", _isAPlacementLeaf));
    record->add(getSlot("isARoutingLeaf", _isARoutingLeaf));
    //record->add(getSlot("Fences", &_surroundingFences));
    record->add(getSlot("SubGCells", &_subGCells));

    return record;
}



/*
 * ********************************************************************
 * Collection des fences
 */
Fences GCell::getSurroundingFences () const
{
    return GCell_Fences(this);
}


/*
 * ********************************************************************
 *
 * GCell_Fences implementation
 */

GCell_Fences::GCell_Fences(const GCell* gcell)
// *******************************************
:       Inherit()
	, _nb(gcell)
	, _fence(NULL)
{
        if (_nb->getRightFence()) {
            _fence = _nb->getRightFence();
            return;
        }
        if (_nb->getLeftFence()) {
            _fence = _nb->getLeftFence();
            return;
        }
        if (_nb->getUpFence()) {
            _fence = _nb->getUpFence();
            return;
        }
        if (_nb->getDownFence()) {
            _fence = _nb->getDownFence();
            return;
        }
        _fence = NULL;

        return;
}
 
GCell_Fences::GCell_Fences(const GCell_Fences& fences)
// ***************************************************
:       Inherit()
        , _nb(fences._nb)
        , _fence(fences._fence)
{
}
 
GCell_Fences& GCell_Fences::operator=(const GCell_Fences& fences)
// **************************************************************
{
        _nb    = fences._nb;
        _fence = fences._fence;
        return *this;
}
 
Collection<Fence*>* GCell_Fences::getClone() const
// ***********************************************
{
        return new GCell_Fences(*this);
}
 
Locator<Fence*>* GCell_Fences::getLocator() const
// **********************************************
{
        return new Locator(_nb, _fence);
}
 
string GCell_Fences::_getString() const
// ************************************
{
        string s = "<" + _TName("GCell::Fences");
        if (_nb) s += " " + getString(_nb);
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * GCell_Fences::Locator implementation
 */
GCell_Fences::Locator::Locator(const GCell* gcell, Fence* fence)
// *************************************************************
:       Inherit(),
        _nb(gcell),
        _fence(fence)
    {
    }
     
    GCell_Fences::Locator::Locator(const Locator& locator)
    // ***************************************************
    :       Inherit(),
            _nb(locator._nb),
            _fence(locator._fence)
    {
    }
     
    GCell_Fences::Locator& GCell_Fences::Locator::operator=(const Locator& locator)
    // ****************************************************************************
    {
            _nb = locator._nb;
            _fence = locator._fence;
            return *this;
    }
     
    Fence* GCell_Fences::Locator::getElement() const
    // *********************************************
    {
            return _fence;
    }
     
    Locator<Fence*>* GCell_Fences::Locator::getClone() const
    // *****************************************************
    {
            return new Locator(*this);
    }
     
    bool GCell_Fences::Locator::isValid() const
    // ****************************************
    {
            return (_fence != NULL);
    }

    void GCell_Fences::Locator::progress()
    // ***********************************
    {
        bool found = false;

        if ( _nb->getRightFence() ) {

            if (_fence == _nb->getRightFence())
                found = true;

        }

        if ( _nb->getLeftFence() ) {

            if (found) {
                _fence = _nb->getLeftFence();
                return;
            }

            if (_fence == _nb->getLeftFence())
                found = true;
        }

        if ( _nb->getUpFence() ) {

            if (found) {
                _fence = _nb->getUpFence();
                return;
            }

            if (_fence == _nb->getUpFence())
                found = true;
        }

        if ( _nb->getDownFence() ) {

            if (found) {
                _fence = _nb->getDownFence();
                return;
            }

            if (_fence == _nb->getDownFence())
                found = true;
        }

        if (found) {
            _fence = NULL;
            return;
        }

        throw Error ("Fence collection strangeness");
        
        return;
    }
     
    string GCell_Fences::Locator::_getString() const
    // *********************************************
    {
            string s = "<" + _TName("GCells::Fences::Locator");
            if (_nb) s += " " + getString(_nb);
            s += ">";
            return s;
    }




} // namespace Nimbus
