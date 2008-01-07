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

#include "Component.h"
#include "Occurrence.h"
#include "Pin.h"

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

	private:   Unit _x;
	private:   Unit _y;
	private:   Occurrence _occurrence;



// Constructors
// ************

	protected: RoutingPad(Net* net, const Point& p, Occurrence occurrence = Occurrence());
	public: static RoutingPad* Create(Net* net, Occurrence occurrence);

// Accessors
// *********

	//public: virtual Hooks GetHooks() const;
	public: virtual Unit GetX() const;
	public: virtual Unit GetY() const;
	public: virtual Box GetBoundingBox() const;
	public: virtual Layer* GetLayer() const;
	public: virtual Box GetBoundingBox(const BasicLayer* basicLayer) const;
        public: virtual Point GetCenter() const;
	public: Occurrence GetOccurrence() const { return _occurrence; };
    public: Occurrence GetPlugOccurrence();
    public: Point GetSourcePosition() const;
    public: Point GetTargetPosition() const;
    public: Unit GetSourceX() const;
    public: Unit GetSourceY() const;
    public: Unit GetTargetX() const;
    public: Unit GetTargetY() const;

// Updators
// ********

	public: virtual void Translate(const Unit& dx, const Unit& dy);

	public: void SetX(const Unit& x);
	public: void SetY(const Unit& y);
	public: void SetPosition(const Unit& x, const Unit& y);
	public: void SetPosition(const Point& position);
	public: void SetOffset(const Unit& x, const Unit& y);
    public: void SetExternalComponent(Component* component);
    public: void RestorePlugOccurrence();

// Others
// ******

	protected: virtual void _PostCreate();
	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("RoutingPad");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;

    public: Component* _GetEntityAsComponent () const;
    public: Segment* _GetEntityAsSegment () const;
//    public: bool _IsInterceptedBy(View* view, const Point& point, const Unit& aperture) const;
//	public: virtual void _Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation);
//	public: virtual void _Highlight(View* view, const Box& updateArea, const Transformation& transformation);

};


RoutingPad* CreateRoutingPad ( Net* net, Occurrence plugOccurrence );
RoutingPad* CreateRoutingPad ( Pin* pin );


} // End of Hurricane namespace.

#endif // HURRICANE_ROUTINGPAD
