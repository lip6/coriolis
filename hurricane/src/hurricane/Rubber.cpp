// ****************************************************************************************************
// File: Rubber.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6 : Rubbers end at component center

#include "Rubber.h"
#include "Net.h"
#include "Plug.h"
#include "Instance.h"
#include "Cell.h"
#include "Hook.h"
#include "Component.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Rubber implementation
// ****************************************************************************************************

Rubber::Rubber(Net* net, Hook* hook)
// *********************************
:  Inherit(),
        _net(net),
        _hook(hook),
        _count(0),
        _boundingBox(),
        _nextOfNetRubberSet(NULL)
{
        if (!_net)
                throw Error("Can't create " + _TName("Rubber") + " : null net");

        if (!_hook)
                throw Error("Can't create " + _TName("Rubber") + " : null hook");

        if (_hook->GetComponent()->GetNet() != _net)
                throw Error("Can't create " + _TName("Rubber") + " : invalid hook");
}

void Rubber::Delete()
// ******************
{
        throw Error("Abnormal deletion of " + _TName("Rubber"));
}

Cell* Rubber::GetCell() const
// **************************
{
        return _net->GetCell();
}

Point Rubber::GetCenter() const
// ****************************
{
        return GetBoundingBox().GetCenter();
}

Point Rubber::GetBarycenter() const
// ********************************
{
        int n = GetHooks().GetSize();
        Unit x = 0;
        Unit y = 0;
        for_each_hook(hook, GetHooks()) {
                Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
                x += position.GetX() / n;
                y += position.GetY() / n;
                end_for;
        }
        return Point(x,y);
}

Box Rubber::GetBoundingBox() const
// *******************************
{
        if (_boundingBox.IsEmpty())
        {
                Rubber* rubber = const_cast<Rubber*>(this);
                Box& boundingBox = rubber->_boundingBox;
                for_each_hook(hook, GetHooks()) {
                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
                        boundingBox.Merge(position);
                        end_for;
                }
        }
        return _boundingBox;
}

Hooks Rubber::GetHooks() const
// ***************************
{
        return (_hook) ? _hook->GetHooks().GetSubSet(Hook::GetIsMasterFilter()) : Hooks();
}

void Rubber::Materialize()
// ***********************
{
        if (!IsMaterialized()) {
                Cell* cell = GetCell();
                QuadTree* quadTree = cell->_GetQuadTree();
                quadTree->Insert(this);
                cell->_Fit(quadTree->GetBoundingBox());
        }
}

void Rubber::Unmaterialize()
// *************************
{
        if (IsMaterialized()) {
                Cell* cell = GetCell();
                cell->_Unfit(GetBoundingBox());
                cell->_GetQuadTree()->Remove(this);
        }
}

void Rubber::Translate(const Unit& dx, const Unit& dy)
// ***************************************************
{
}

Rubber* Rubber::_Create(Hook* hook)
// ********************************
{
        if (!hook)
                throw Error("Can't create " + _TName("Rubber") + " : null hook");

        if (!hook->IsMaster())
                throw Error("Can't create " + _TName("Rubber") + " : not a master hook");

        Net* net = hook->GetComponent()->GetNet();

        if (!net)
                throw Error("Can't create " + _TName("Rubber") + " : unconnected component");

        Rubber* rubber = new Rubber(net, hook);

        rubber->_PostCreate();

        return rubber;
}

void Rubber::_PostCreate()
// ***********************
{
        _net->_GetRubberSet()._Insert(this);

        for_each_hook(hook, GetHooks()) {
                hook->GetComponent()->_SetRubber(this);
                end_for;
        }

        Inherit::_PostCreate();
}

void Rubber::_Delete()
// *******************
{
        _PreDelete();

        delete this;
}

void Rubber::_PreDelete()
// **********************
{
// trace << "entering Rubber::_PreDelete: " << this << endl;
// trace_in();

        Inherit::_PreDelete();

        _count = (unsigned)-1; // to avoid a new destruction

        for_each_hook(hook, GetHooks()) {
                hook->GetComponent()->_SetRubber(NULL);
                end_for;
        }

        _net->_GetRubberSet()._Remove(this);

// trace << "exiting Rubber::_PreDelete:" << endl;
// trace_out();
}

string Rubber::_GetString() const
// ******************************
{
        string s = Inherit::_GetString();
        s.insert(s.length() - 1, " " + GetString(_net->GetName()));
        s.insert(s.length() - 1, " " + GetString(_count));
        return s;
}

Record* Rubber::_GetRecord() const
// *************************
{
        Record* record = Inherit::_GetRecord();
        if (record) {
                record->Add(GetSlot("Net", _net));
                record->Add(GetSlot("Hook", _hook));
                record->Add(GetSlot("Count", _count));
                record->Add(GetSlot("BoundingBox", _boundingBox));
        }
        return record;
}

void Rubber::_SetNet(Net* net)
// ***************************
{
        if (net != _net) {
                if (_net) _net->_GetRubberSet()._Remove(this);
                _net = net;
                if (_net) _net->_GetRubberSet()._Insert(this);
        }
}

void Rubber::_SetHook(Hook* hook)
// ******************************
{
        assert(hook->IsMaster());
        assert(hook->GetComponent()->GetNet() == GetNet());
        _hook = hook;
}

void Rubber::_Capture()
// ********************
{
        Invalidate();
        _count++;
}

void Rubber::_Release()
// ********************
{
        if (_count != ((unsigned)-1)) { // not in deletion
                Invalidate();
                if ((--_count) == 1) _Delete();
        }
}

void Rubber::Invalidate(bool propagateFlag)
// ****************************************
{
        Inherit::Invalidate(false);
        _boundingBox.MakeEmpty();
}

//bool Rubber::_IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const
//// **************************************************************************************
//{
//        double x = GetValue(point.GetX());
//        double y = GetValue(point.GetY());
//        double a = GetValue(aperture);
//        Point origin;
//
//        switch (view->GetRubberDisplayType())
//        {
//            case View::RubberDisplayType::GEOMETRIC:
//                {
//                    origin = GetCenter();
//                    break;
//                }
//            case View::RubberDisplayType::BARYCENTRIC:
//                {
//                    origin = GetBarycenter();
//                    break;
//                }
//            case View::RubberDisplayType::STEINER:
//                {
//                    // XXX
//                    return false;
//                    break;
//                }
//            default:
//                throw Error("Unknown RubberDisplayType");
//        }
//        double xo = GetValue(origin.GetX());
//        double yo = GetValue(origin.GetY());
//        for_each_hook(hook, GetHooks()) {
//                Point extremity = extremity = hook->GetComponent()->GetBoundingBox().GetCenter();
//                double xe = GetValue(extremity.GetX());
//                double ye = GetValue(extremity.GetY());
//                double xp = xo;
//                double yp = yo;
//                if (xo != xe) xp = (((xe - xo) / (ye - yo)) * (y - yo)) + xo;
//                if (yo != ye) yp = (((ye - yo) / (xe - xo)) * (x - xo)) + yo;
//                if ((abs(xp - x) <= a) || (abs(yp - y) <= a)) return true;
//                end_for;
//        }
//        return false;
//}
//
//
//void Rubber::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************************
//{
//        assert(!basicLayer);
//
//        Point center;
//
//        switch (view->GetRubberDisplayType())
//        {
//            case View::RubberDisplayType::GEOMETRIC:
//                {
//                    center = transformation.GetPoint(GetCenter());
//                    for_each_hook(hook, GetHooks()) {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        view->DrawLine(center, transformation.GetPoint(position));
//                        end_for;
//                    }
//                    break;
//                }
//            case View::RubberDisplayType::BARYCENTRIC:
//                {
//                    center = transformation.GetPoint(GetBarycenter());
//                    for_each_hook(hook, GetHooks()) {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        view->DrawLine(center, transformation.GetPoint(position));
//                        end_for;
//                    }
//                    break;
//                }
//            case View::RubberDisplayType::STEINER:
//                {
//                    center = transformation.GetPoint(GetBarycenter());
//                    for_each_hook(hook, GetHooks()) {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        Point crosspoint (position.GetX(), center.GetY());
//                        view->DrawLine(position, crosspoint);
//                        view->DrawLine(crosspoint, center);
//                        end_for;
//                    }
//
//                    break;
//                }
//            default:
//                throw Error("Unknown RubberDisplayType");
//        }
//
//}
//
typedef struct pcmp_s {
    bool operator() (const Point& p1, const Point& p2) const {
        return (p1.GetX() < p2.GetX()) || ( (p1.GetX() == p2.GetX()) && (p1.GetY() < p2.GetY()) );
    }
} pcmp_t;

//void Rubber::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// **********************************************************************************************
//{
//        Point center;
//
//        switch (view->GetRubberDisplayType())
//        {
//            case View::RubberDisplayType::GEOMETRIC:
//                {
//                    center = transformation.GetPoint(GetCenter());
//                    for_each_hook(hook, GetHooks()) {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        view->DrawLine(center, transformation.GetPoint(position));
//                        end_for;
//                    }
//                    break;
//                }
//            case View::RubberDisplayType::BARYCENTRIC:
//                {
//                    center = transformation.GetPoint(GetBarycenter());
//                    for_each_hook(hook, GetHooks()) {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        view->DrawLine(center, transformation.GetPoint(position));
//                        end_for;
//                    }
//                    break;
//                }
//            case View::RubberDisplayType::STEINER:
//                {
//                    set <Point, pcmp_t> pset;
//                    for_each_hook (hook, GetHooks())
//                    {
//                        Point position = hook->GetComponent()->GetBoundingBox().GetCenter();
//                        pset.insert (position);
//                        end_for;
//                    }
//                    center = transformation.GetPoint(GetBarycenter());
//                    Unit lastXup = center.GetX();
//                    Unit lastXlo = center.GetX();
//                    for (
//                            set<Point, pcmp_t>::iterator pit = pset.begin();
//                            pit != pset.end();
//                            pit++
//                        )
//                    {
//                        Point position (*pit);
//                        Point crosspoint (position.GetX(), center.GetY());
//                        Point connxpoint (center);
//                        if (position.GetY() > center.GetY())
//                        {
//                            // en haut
//                            if ( (position.GetX() - lastXup) < (position.GetY() - center.GetY()) )
//                            {
//                                crosspoint.SetX (lastXup);
//                                crosspoint.SetY (position.GetY());
//                                connxpoint.SetX (lastXup);
//                            }
//                            else
//                                lastXup = position.GetX();
//                        } else {
//                            // en bas
//                            if ( (position.GetX() - lastXlo) < (center.GetY() - position.GetY()) )
//                            {
//                                crosspoint.SetX (lastXlo);
//                                crosspoint.SetY (position.GetY());
//                                connxpoint.SetX (lastXlo);
//                            }
//                            else
//                                lastXlo = position.GetX();
//                        }
//
//
//                        view->DrawLine(position, crosspoint);
//                        view->DrawLine(crosspoint, connxpoint);
//                    }
//                    break;
//                }
//            default:
//                throw Error("Unknown RubberDisplayType");
//        }
//
//}
//


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
