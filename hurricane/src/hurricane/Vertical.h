// ****************************************************************************************************
// File: Vertical.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_VERTICAL
#define HURRICANE_VERTICAL

#include "Segment.h"
#include "Verticals.h"

namespace Hurricane {



// ****************************************************************************************************
// Vertical declaration
// ****************************************************************************************************

class Vertical : public Segment {
// ****************************

// Types
// *****

    public: typedef Segment Inherit;

// Attributes
// **********

    private: Unit _x;
    private: Unit _dySource;
    private: Unit _dyTarget;

// Constructors
// ************

    protected: Vertical(Net* net, Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);

    public: static Vertical* create(Net* net, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);
    public: static Vertical* create(Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width = 0, const Unit& dySource = 0, const Unit& dyTarget = 0);

// Accessors
// *********

    public: virtual Unit getX() const {return _x;};
    public: virtual Point getCenter() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual Unit getSourceX() const {return getX();};
    public: virtual Unit getSourceY() const;
    public: virtual Unit getTargetX() const {return getX();};
    public: virtual Unit getTargetY() const;
    public: virtual Unit getLength() const;
    public: const Unit& getDySource() const {return _dySource;};
    public: const Unit& getDyTarget() const {return _dyTarget;};

// Updators
// ********

    public: virtual void translate(const Unit& dx, const Unit& dy);

    public: void setX(const Unit& x);
    public: void setDySource(const Unit& dySource);
    public: void setDyTarget(const Unit& dyTarget);
    public: void translate(const Unit& dx);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Vertical");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Vertical)

#endif // HURRICANE_VERTICAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
