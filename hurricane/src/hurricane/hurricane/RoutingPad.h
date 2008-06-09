// ****************************************************************************************************
// 
// This file is part of the Coriolis Project.
// Copyright (c) 2001-2006  Laboratoire LIP6 - Departement ASIM
//                          Universite Pierre et Marie Curie.
// 
// File: RoutingPad.h
// Authors: H.Clement, M.Sroka
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

#endif // HURRICANE_ROUTINGPAD
