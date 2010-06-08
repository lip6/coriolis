
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#include "nimbus/NimbusEngine.h"


namespace Nimbus {


  using namespace std;

  const Name Fence::_extensionName = "Nimbus::Grid";


/*
 * ********************************************************************
 * Fence_Splitters declaration
 * ********************************************************************
 */
class Fence_Splitters : public Collection<Splitter*> {
    
    public: typedef Collection<Splitter*> Inherit;

    public: class Locator : public Hurricane::Locator<Splitter*> {

                public: typedef Hurricane::Locator<Splitter*> Inherit;

                private: const SplitterSet* _spset;
                private: SplitterSet::iterator _spit;

                public: Locator(const SplitterSet* spset, SplitterSet::iterator spit);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual Splitter* getElement() const;
                public: virtual Hurricane::Locator<Splitter*>* getClone() const;

                public: virtual bool isValid() const;
                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const SplitterSet* _spset;
    private: SplitterSet::iterator _spit;

    public: Fence_Splitters(const SplitterSet* spset);
    public: Fence_Splitters(const Fence_Splitters& splitters);

    public: Fence_Splitters& operator=(const Fence_Splitters& splitters);
            
    public: virtual Collection<Splitter*>* getClone() const;
    public: virtual Hurricane::Locator<Splitter*>* getLocator() const;

    public: virtual string _getString() const;
};


/*
 * ********************************************************************
 * Fence_GCells declaration
 * ********************************************************************
 */
class Fence_GCells : public Collection<GCell*> {
    
    public: typedef Collection<GCell*> Inherit;

    public: class Locator : public Hurricane::Locator<GCell*> {

                public: typedef Hurricane::Locator<GCell*> Inherit;

                private: const Fence* _fence;
                private: GCell* _gc;

                public: Locator(const Fence* fence, GCell* gcell = NULL);
                public: Locator(const Locator& locator);

                public: Locator& operator=(const Locator& locator);

                public: virtual GCell* getElement() const;
                public: virtual Hurricane::Locator<GCell*>* getClone() const;

                public: virtual bool isValid() const;

                public: virtual void progress();

                public: virtual string _getString() const;
            };


    private: const Fence* _fence;
    private: GCell* _gc;

    public: Fence_GCells(const Fence* fence);
    public: Fence_GCells(const Fence_GCells& gcelles);

    public: Fence_GCells& operator=(const Fence_GCells& gcelles);
            
    public: virtual Collection<GCell*>* getClone() const;
    public: virtual Hurricane::Locator<GCell*>* getLocator() const;

    public: virtual string _getString() const;
};


RoutingGauge* Fence::_routingGauge = NULL;


/*
 * ********************************************************************
 * Fence implementation
 *
 */


Fence::Fence ( Grid* grid, DbU::Unit size, unsigned capacity, unsigned occupancy, bool visible)
  : Inherit (grid->getCell())
    , _splitters ()
    , _components ()
    , _occupancy (occupancy)
    , _capacity (capacity)
    , _frontLine (NULL)
    , _nextOfFenceSubFenceSet(NULL)
    , _nextOfGCellSubFenceSet(NULL)
    , _nextOfGridFenceSet(NULL)
    , _isARoutingFence(false)
    , _visible(visible)
    , _step (0)
    , _size (size)
    , _grid (grid)
    , _subFences()
{
    return;
}

Fence::~Fence ()
{
    return;
}

void Fence::_preDestroy() {

    //throw Error ("[ERROR] Fence deletion temporarily disabled (to be fixed...)");
    
    Inherit::_preDestroy();

    Fence* parentFence = getParentFence();
    if (parentFence) parentFence->removeSubFence(this);

    for (
            SplitterSet::iterator spit = _splitters.begin() ;
            spit != _splitters.end() ;
            spit++
        )
    {
        (*spit)->onDeletedFence();
    }
#if 0
    // remove Fence property ?
    for (
            ComponentSet::iterator cit = _components.begin() ;
            cit != _components.end() ;
            cit++
        )
    {
        (*cit)->onDeletedFence();
    }
#endif

    return;
}

void Fence::_postCreate ()
{
    Inherit::_postCreate();

    //getGCell1()->addSurroundingFence (this);
    //getGCell2()->addSurroundingFence (this);
    Fence* parentFence = getParentFence();
    if (parentFence) parentFence->addSubFence(this);

    computeCapacity();
    
    return;
};

void Fence::setAsRoutingFence()
{
    if (_isARoutingFence) return;
    _isARoutingFence = true;

    _grid->getNimbus()->addToRoutingFences(this);

    return;
}

void Fence::destroy()
{
    _preDestroy();
    delete this;
}

void Fence::computeCapacity()
{

#if 0
    unsigned pitch = static_cast<unsigned>(getValue (getCDataBase()->getPitch()));
    unsigned nlayers = getGrid()->getNimbus()->getNumberOfRoutingLayers();

    //Name alu1 ("alu1");
    double capa (0);
    for_each_layergauge (routingLayerGauge, _routingGauge)
    {
        capa += routingLayerGauge->getTrackNumber(fence->getMin(), fence->getMax());
        end_for;
    }
    
    
    setCapacity(static_cast<unsigned>(capa/2));

#endif
    return;
}

void Fence::_moveTo (DbU::Unit target)
{
    for_each_fence (subfence, getSubFences())
    {
        subfence->_moveTo (target);
        end_for;
    }
    return;
}

DbU::Unit Fence::getDistance() const {

    Point p1 = getGCell1()->getCenter();
    Point p2 = getGCell2()->getCenter();

    return p1.manhattanDistance(p2);
}

void Fence::attachSplitter (Splitter* splitter)
{
    _splitters.insert(splitter);
    _occupancy ++;
    
    return;
}

void Fence::detachSplitter (Splitter* splitter)
{
    SplitterSet::iterator spit = _splitters.find (splitter);

    if (spit != _splitters.end()) {
        _splitters.erase (spit);
        _occupancy --;
    } else {
        throw Error ("DetachSplitter: the splitter is not attached to this fence");
    }

    return;
}

void Fence::attachComponent (Component* component)
{
    _components.insert(component);
    _occupancy ++;
    
    return;
}

void Fence::detachComponent (Component* component)
{
    ComponentSet::iterator cit = _components.find (component);

    if (cit != _components.end()) {
        _components.erase (cit);
        _occupancy --;
    } else {
        throw Error ("DetachComponent: the component is not attached to this fence");
    }

    return;
}

string Fence::_getString() const {
    return "<" + _TName ( "Fence" )
         + " " + getString ( getX() )
         + " " + getString ( getY() ) + ">";
}

Record* Fence::_getRecord() const {
    Record* record = Inherit::_getRecord();

    if (record) {
        record->add(getSlot("Capacity", _capacity));
        record->add(getSlot("Occupancy", _occupancy));
        record->add(getSlot("Size", _size));
        record->add(getSlot("Step", _step));
        record->add(getSlot("Visible", _visible));
        record->add(getSlot("RoutingFence", _isARoutingFence));
        record->add(getSlot("X", getX()));
        record->add(getSlot("Y", getY()));
    }

    return record;
}

// void Fence::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
// {
//     if (!isVisible()) return;
//     Point p1 = transformation.getPoint(getP1());
//     Point p2 = transformation.getPoint(getP2());

//     DbU::Unit halfWidth;
//     DbU::Unit pitch = getCDataBase()->getDefaultCGPitch();
    
//     if (_capacity != 0)
//         halfWidth = DbU::lambda( (((getValue(_size)/10) * ((double)_occupancy)) / ((double)_capacity))/2 );
//     else
//         halfWidth = DbU::lambda( (((getValue(_size)/10) * ((double)_occupancy)) / 1                 )/2 );

//     if (p1.getX() == p2.getX())
//     {
//         Point pp1 = Point (p1.getX() + halfWidth, p1.getY() + pitch);
//         Point pp2 = Point (p2.getX() - halfWidth, p2.getY() - pitch);
//         view->FillRectangle (pp1, pp2);

//         Point pp1f = Point (p1.getX() + DbU::lambda(getValue(_size)/20), p1.getY() + pitch);
//         Point pp2f = Point (p2.getX() - DbU::lambda(getValue(_size)/20), p2.getY() - pitch);
//         view->DrawRectangle (pp1f, pp2f);
//     }
//     else if (p1.getY() == p2.getY())
//     {
//         Point pp1 = Point (p1.getX() + pitch, p1.getY() + halfWidth);
//         Point pp2 = Point (p2.getX() - pitch, p2.getY() - halfWidth );
//         view->FillRectangle (pp1, pp2);

//         Point pp1f = Point (p1.getX() + pitch, p1.getY() + DbU::lambda(getValue(_size)/20));
//         Point pp2f = Point (p2.getX() - pitch, p2.getY() - DbU::lambda(getValue(_size)/20));
//         view->DrawRectangle (pp1f, pp2f); 
//     }
//     else
//     {
//         view->DrawLine(p1, p2);
//     }

//     return;
// }

// void Fence::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
// {
//     Point p1 = transformation.getPoint(getP1());
//     Point p2 = transformation.getPoint(getP2());

//     view->DrawLine(p1, p2);

//     return;
// }

Box Fence::getBoundingBox () const
{
    Box box;
    
    Point p1 = getP1();
    Point p2 = getP2();

    if (p1.getX() == p2.getX())
    {
        Point pp1f = Point (p1.getX() + (_size/20), p1.getY() + DbU::lambda(1));
        Point pp2f = Point (p2.getX() - (_size/20), p2.getY() - DbU::lambda(1));

        box = Box(pp1f);
        box.merge(pp2f);
    }
    else if (p1.getY() == p2.getY())
    {
        Point pp1f = Point (p1.getX() + DbU::lambda(1), p1.getY() + (_size/20));
        Point pp2f = Point (p2.getX() - DbU::lambda(1), p2.getY() - (_size/20));

        box = Box(pp1f);
        box.merge(pp2f);
    }
    else
    {
        box = Box(p1);
        box.merge(p2);
    }
    box.inflate(DbU::lambda(1), DbU::lambda(2));

    return box;
}

void Fence::setFrontLine(FrontLine* frontLine)
{
    if (getFrontLine() != frontLine) throw Error ("Fence is in another frontline already");
    _frontLine = frontLine;
    return;
}

FrontLine* Fence::getFrontLine()
{
    if (!_frontLine)
    {
        Fence* parentFence = getParentFence();
	if (parentFence) _frontLine = parentFence->getFrontLine();
    }
    return _frontLine;
}

Splitters Fence::getSplitters () const
{
    return Fence_Splitters(&_splitters);
}

GCells Fence::getGCells () const
{
    return Fence_GCells(this);
}

void Fence::addSubFence (Fence* fence)
{
    _subFences._insert(fence);
    return;
}

void Fence::removeSubFence (Fence* fence)
{
    _subFences._remove(fence);
    return;
}

GCell* Fence::getOppositeGCell (GCell* gcell) const {

    if (getGCell1() == gcell) {
        return getGCell2();
    } else if (getGCell2() == gcell) {
        return getGCell1();
    }

    throw Error ("[ERROR] fence: not a gcell of mine");
}

Fence* Fence::getParentFence() const {

    GCell* gc1 = getGCell1()->getContainer();
    GCell* gc2 = getGCell2()->getContainer();

    if (gc1 == gc2) return NULL;

    return _grid->getFenceBetween(gc1,gc2);
}

Splitter* Fence::getNetSplitter(Net* net) const
{
    for (
            SplitterSet::iterator spit = _splitters.begin() ;
            spit != _splitters.end();
            spit++
        )
    {
        if ( (*spit)->getNet() == net )
            return (*spit);
    }
    return NULL;
}

Cell* Fence::getCell() const {
    return _grid->getCell();
}

// bool Fence::_IsInterceptedBy(View* view, const Point& point, const DbU::Unit& aperture) const
// {
//     if (!isVisible()) return false;

//     Box box(point);
//     box.inflate(aperture);
//     return getBoundingBox().intersect(box);
// }


/*
 * ********************************************************************
 * Fence_Splitters implementation
 *
 */
 
Fence_Splitters::Fence_Splitters(const SplitterSet* spset)
// **************************************************************************************
:       Inherit()
        , _spset(spset)
{
    assert(spset);
    _spit = spset->begin();
}
 
Fence_Splitters::Fence_Splitters(const Fence_Splitters& splitters)
// ************************************************************************************
:       Inherit()
        , _spset(splitters._spset)
        , _spit(splitters._spit)
{
}
 
Fence_Splitters& Fence_Splitters::operator=(const Fence_Splitters& splitters)
// ****************************************************************
{
        _spset    = splitters._spset;
        _spit     = splitters._spit;
        return *this;
}
 
Collection<Splitter*>* Fence_Splitters::getClone() const
// ***********************************************
{
        return new Fence_Splitters(*this);
}
 
Locator<Splitter*>* Fence_Splitters::getLocator() const
// **********************************************
{
        return new Locator(_spset, _spit);
}
 
string Fence_Splitters::_getString() const
// *************************************
{
        string s = "<" + _TName("Fence::Splitters");
        //if (_splitterIterator) s += " " + getString((*_splitterIterator));
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Fence_Splitters::Locator implementation
 */
Fence_Splitters::Locator::Locator(const SplitterSet* spset, SplitterSet::iterator spit)
// ************************************************************************************
:       Inherit(),
        _spset(spset),
        _spit(spit)
{
}
 
Fence_Splitters::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _spset(locator._spset),
        _spit(locator._spit)
{
}
 
Fence_Splitters::Locator& Fence_Splitters::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _spset = locator._spset;
        _spit = locator._spit;
        return *this;
}
 
Splitter* Fence_Splitters::Locator::getElement() const
// **************************************************
{
        return (*_spit);
}
 
Locator<Splitter*>* Fence_Splitters::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Fence_Splitters::Locator::isValid() const
// **************************************************
{
        return (_spit != _spset->end());
}

void Fence_Splitters::Locator::progress()
// *********************************************
{
    _spit++;

    return;
}
 
string Fence_Splitters::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Fence::Splitters::Locator");
        if (_spit != _spset->end()) s += " " + getString(*_spit);
        s += ">";
        return s;
}









/*
 * ********************************************************************
 * Fence_GCells implementation
 */
Fence_GCells::Fence_GCells(const Fence* fence)
// **************************************************************************************
:       Inherit()
        , _fence(fence)
        , _gc(fence->getGCell1())
{
}
 
Fence_GCells::Fence_GCells(const Fence_GCells& gcelles)
// ************************************************************************************
:       Inherit()
        , _fence(gcelles._fence)
        , _gc(gcelles._gc)
{
}
 
Fence_GCells& Fence_GCells::operator=(const Fence_GCells& gcelles)
// ****************************************************************
{
        _fence = gcelles._fence;
        _gc = gcelles._gc;
        return *this;
}
 
Collection<GCell*>* Fence_GCells::getClone() const
// ***********************************************
{
        return new Fence_GCells(*this);
}
 
Locator<GCell*>* Fence_GCells::getLocator() const
// **********************************************
{
        return new Locator(_fence, _gc);
}
 
string Fence_GCells::_getString() const
// *************************************
{
        string s = "<" + _TName("Fence::GCells");
        if (_gc) s += " " + getString(_gc);
        s += ">";
        return s;
}

/*
 * ********************************************************************
 * Fence_GCells::Locator implementation
 */
Fence_GCells::Locator::Locator(const Fence* fence, GCell* gcell)
// ********************************************************
:       Inherit(),
        _fence(fence),
        _gc(gcell)
{
}
 
Fence_GCells::Locator::Locator(const Locator& locator)
// ****************************************************
:       Inherit(),
        _fence(locator._fence),
        _gc(locator._gc)
{
}
 
Fence_GCells::Locator& Fence_GCells::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
        _fence = locator._fence;
        _gc = locator._gc;
        return *this;
}
 
GCell* Fence_GCells::Locator::getElement() const
// **************************************************
{
        return _gc;
}
 
Locator<GCell*>* Fence_GCells::Locator::getClone() const
// *****************************************************************
{
        return new Locator(*this);
}
 
bool Fence_GCells::Locator::isValid() const
// **************************************************
{
        return (_gc != NULL);
}

void Fence_GCells::Locator::progress()
// *********************************************
{
    if (_gc == _fence->getGCell1()) {
        _gc = _fence->getGCell2();
    } else {
        _gc = NULL;
    }

    return;
}
 
string Fence_GCells::Locator::_getString() const
// *******************************************************
{
        string s = "<" + _TName("Fence::GCells::Locator");
        if (_gc) s += " " + getString(_gc);
        s += ">";
        return s;
}


/*
 * ********************************************************************
 * SubFenceSet implementation
 *
 */
Fence::SubFenceSet::SubFenceSet()
// *******************************
    : Inherit()
{
}

unsigned Fence::SubFenceSet::_getHashValue(Fence* fence) const
{
    return ( (unsigned int)( (unsigned long)fence ) ) / 2;
}

Fence* Fence::SubFenceSet::_getNextElement(Fence* fence) const
{
    return fence->_getNextOfFenceSubFenceSet();
}

void Fence::SubFenceSet::_setNextElement(Fence* fence, Fence* nextFence) const
{
    fence->_setNextOfFenceSubFenceSet(nextFence);
    return;
}



} // namespace Nimbus
