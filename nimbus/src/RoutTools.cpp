
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Contact.h"
#include "nimbus/Splitter.h"
#include "nimbus/GCell.h"
#include "nimbus/Fence.h"
#include "nimbus/RoutTools.h"

namespace Nimbus {

  using namespace Hurricane;

Layer* getGRLayer ()
{
  return DataBase::getDB()->getTechnology()->getLayer (Name ("SPL1"));
}

#if 0
Contact* getContact (Net& net, Point& position)
{
    Contact* contact;

    //cout << "Searching for " << position << endl;

    for_each_contact (contact, net.getContacts()) {

        if (contact->getPosition() == position) {
            return contact;
        } else {
        }

        end_for;
    }

    return NULL;
}
#endif

#if 0
Contacts* getContact (Net& net, Box& box)
{

    return (net.getContacts().getSubSet (getContactIsUnderFilter (box)));
}
#endif

Contact* CheckGetContact (Component* compo) {
    Contact* c = NULL;

    if (dynamic_cast<Contact*> (compo)) {
        c = dynamic_cast<Contact*> (compo);
    } else {
        // empty
    }

    return c;
}


// ********************************************************************
// Rattachements des Splitters à leur Fence
//

#if 0
void StickSplittersToFences (Cell* cell) {

    for (unsigned step = 1; true ; step ++) {
        
        DisplaySlot* displaySlot = getDisplaySlot (cell, getFenceDisplayName(step));

        if (!displaySlot)
            return;

        for_each_net (net, cell->getNets()) {

            for_each_component (compo, net->getComponents()) {

                if ( ! (is_a<Splitter*> (compo)) )
                    continue;

                Splitter* splitter = dynamic_cast<Splitter*> (compo);

                if (splitter->getFence())
                    continue;

                unsigned sstep = splitter->getStep();
                if (sstep != step) continue;
                Point p (splitter->getPosition());
                Box b = Box(p); b.Inflate(getUnit(10));
                for_each_go (go, displaySlot->_getQuadTree().getGosUnder (b)) {
                    if ( ! (is_a<Fence*>(go)) )
                        continue;

                    Fence* fence = dynamic_cast<Fence*> (go);

                    if (
                            (fence->getStep() == sstep)
                       )
                    {
                        fence->attachSplitter (splitter);
                        splitter->setFence (fence);
                        break;
                    }

                    end_for;
                }

                end_for;
            }

            end_for;
        }
    }

    return;
}
#endif

// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Contact_IsUnderFilter : public Filter<Contact*> {
// ******************************************************

    public: Box _area;

    public: Contact_IsUnderFilter(const Box& area)
    // *********************************************
    :       _area(area)
    {
    };

    public: Contact_IsUnderFilter(const Contact_IsUnderFilter& filter)
    // *******************************************************************
    :       _area(filter._area)
    {
    };

    public: Contact_IsUnderFilter& operator=(const Contact_IsUnderFilter& filter)
    // ******************************************************************************
    {
        _area = filter._area;
        return *this;
    };

    public: virtual Filter<Contact*>* getClone() const
    // *************************************************
    {
        return new Contact_IsUnderFilter(*this);
    };

    public: virtual bool accept(Contact* contact) const
    // ****************************************************
    {
        return (_area.intersect(contact->getBoundingBox())) && (getGRLayer() == contact->getLayer());
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Contact::isUnderFilter") + " " + getString(_area) + ">";
    };

};


ContactFilter getIsUnderFilter(const Box& area)
    // *********************************************************
{
    return Contact_IsUnderFilter(area);
}


ContactFilter getContactIsUnderFilter (const Box& area) {

    return Contact_IsUnderFilter (area);
}

class GCell_FenceFilter : public Filter<GCell*> {

    public: Fence* _fence;

    public: GCell_FenceFilter(Fence* fence)
            : _fence (fence)
            {
            };
    public: GCell_FenceFilter(const GCell_FenceFilter& filter)
            : _fence (filter._fence)
            {
            }
    public: GCell_FenceFilter& operator=(const GCell_FenceFilter& filter)
            {
                _fence = filter._fence;
                return *this;
            };
    public: virtual Filter<GCell*>* getClone() const
            {
                return new GCell_FenceFilter(*this);
            };
    public: virtual bool accept(GCell* gcell) const
            {
                for_each_fence(fence, gcell->getSurroundingFences())
                {
                    if (fence->getParentFence() == _fence)
                        return true;
                    end_for;
                }
                return false;
            };
    public: virtual string _getString() const
            {
                return "<" + _TName("GCell::FenceFilter") + ">";
            };
};

GCellFilter getGCellFenceFilter(Fence* fence)
{
    return GCell_FenceFilter(fence);
}
                

class GCell_StepFilter : public Filter<GCell*> {
// ******************************************************

    public: unsigned _step;

    public: GCell_StepFilter(unsigned step)
    // *********************************************
    :       _step(step)
    {
    };

    public: GCell_StepFilter(const GCell_StepFilter& filter)
    // *******************************************************************
    :       _step(filter._step)
    {
    };

    public: GCell_StepFilter& operator=(const GCell_StepFilter& filter)
    // ******************************************************************************
    {
        _step = filter._step;
        return *this;
    };

    public: virtual Filter<GCell*>* getClone() const
    // *************************************************
    {
        return new GCell_StepFilter(*this);
    };

    public: virtual bool accept(GCell* gcell) const
    // ****************************************************
    {
        return (gcell->getStep() == _step);
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("GCell::StepFilter") + " " + getString(_step) + ">";
    };

};


GCellFilter getStepFilter(unsigned step)
    // *********************************************************
{
    return GCell_StepFilter(step);
}


GCellFilter getGCellStepFilter (unsigned step) {

    return GCell_StepFilter (step);
}




// Fence filtering


class Fence_IsInnerOfFilter : public Filter<Fence*> {
// ******************************************************

    public: const GCell* _gc;

    public: Fence_IsInnerOfFilter(const GCell* gcell)
    // *********************************************
    :       _gc(gcell)
    {
    };

    public: Fence_IsInnerOfFilter(const Fence_IsInnerOfFilter& filter)
    // *******************************************************************
    :       _gc(filter._gc)
    {
    };

    public: Fence_IsInnerOfFilter& operator=(const Fence_IsInnerOfFilter& filter)
    // ******************************************************************************
    {
        _gc = filter._gc;
        return *this;
    };

    public: virtual Filter<Fence*>* getClone() const
    // *************************************************
    {
        return new Fence_IsInnerOfFilter(*this);
    };

    public: virtual bool accept(Fence* fence) const
    // ****************************************************
    {
        GCell* gc1 = fence->getGCell1();
        GCell* gc2 = fence->getGCell2();
        return ((gc1 == gc2) && (gc1 == _gc));
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Fence::isInnerOfFilter") + " " + getString(_gc) + ">";
    };

};


FenceFilter getIsInnerOfFilter(const GCell* gcell)
    // *********************************************************
{
    return Fence_IsInnerOfFilter(gcell);
}


FenceFilter getFenceIsInnerOfFilter (const GCell* gcell) {

    return Fence_IsInnerOfFilter (gcell);
}





class Fence_IsSubFenceOfFilter : public Filter<Fence*> {
// ******************************************************

    public: const Fence* _fence;

    public: Fence_IsSubFenceOfFilter(const Fence* fence)
    // *********************************************
    :       _fence(fence)
    {
    };

    public: Fence_IsSubFenceOfFilter(const Fence_IsSubFenceOfFilter& filter)
    // *******************************************************************
    :       _fence(filter._fence)
    {
    };

    public: Fence_IsSubFenceOfFilter& operator=(const Fence_IsSubFenceOfFilter& filter)
    // ******************************************************************************
    {
        _fence = filter._fence;
        return *this;
    };

    public: virtual Filter<Fence*>* getClone() const
    // *************************************************
    {
        return new Fence_IsSubFenceOfFilter(*this);
    };

    public: virtual bool accept(Fence* fence) const
    // ****************************************************
    {
        const GCell* gc1 = fence->getGCell1()->getContainer();
        const GCell* gc2 = fence->getGCell2()->getContainer();
        return (
                ( gc1 != gc2 )
                &&
                ( (gc1 == _fence->getGCell1()) || (gc1 == _fence->getGCell2()) )
                &&
                ( (gc2 == _fence->getGCell1()) || (gc2 == _fence->getGCell2()) )
               );
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Fence::isSubFenceOfFilter") + " " + getString(_fence) + ">";
    };

}; // class FenceIsSubFenceOfFilter


FenceFilter getIsSubFenceOfFilter(const Fence* fence)
    // *********************************************************
{
    return Fence_IsSubFenceOfFilter(fence);
}


FenceFilter getFenceIsSubFenceOfFilter (const Fence* fence) {

    return Fence_IsSubFenceOfFilter (fence);
}


} // namespace Nimbus
