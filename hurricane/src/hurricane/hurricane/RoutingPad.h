// ****************************************************************************************************
// File: ./hurricane/RoutingPad.h
// Authors: H.Clement, M.Sroka
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#ifndef HURRICANE_ROUTINGPAD
#define HURRICANE_ROUTINGPAD

#include "hurricane/Component.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Pin.h"

namespace Hurricane {


class Segment;


// ****************************************************************************************************
// RoutingPad declaration
// ****************************************************************************************************

class RoutingPad : public Component {
// *****************************

// Types
// *****

    public: typedef Component Inherit;


// Attributes
// **********

    private:   DbU::Unit _x;
    private:   DbU::Unit _y;
    private:   Occurrence _occurrence;



// Constructors
// ************

    protected: RoutingPad(Net* net, const Point& p, Occurrence occurrence = Occurrence());
    public: static RoutingPad* create(Net* net, Occurrence occurrence);

// Accessors
// *********

    //public: virtual Hooks getHooks() const;
    public: virtual DbU::Unit getX() const;
    public: virtual DbU::Unit getY() const;
    public: virtual Box getBoundingBox() const;
    public: virtual const Layer* getLayer() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual Point getCenter() const;
    public: Occurrence getOccurrence() const { return _occurrence; };
    public: Occurrence getPlugOccurrence();
    public: Point getSourcePosition() const;
    public: Point getTargetPosition() const;
    public: DbU::Unit getSourceX() const;
    public: DbU::Unit getSourceY() const;
    public: DbU::Unit getTargetX() const;
    public: DbU::Unit getTargetY() const;

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setX(const DbU::Unit& x);
    public: void setY(const DbU::Unit& y);
    public: void setPosition(const DbU::Unit& x, const DbU::Unit& y);
    public: void setPosition(const Point& position);
    public: void setOffset(const DbU::Unit& x, const DbU::Unit& y);
    public: void setExternalComponent(Component* component);
    public: void restorePlugOccurrence();

// Others
// ******

    protected: virtual void _postCreate();
    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("RoutingPad");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: Component* _getEntityAsComponent () const;
    public: Segment* _getEntityAsSegment () const;

};


RoutingPad* createRoutingPad ( Net* net, Occurrence plugOccurrence );
RoutingPad* createRoutingPad ( Pin* pin );


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::RoutingPad);

#endif // HURRICANE_ROUTINGPAD

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
