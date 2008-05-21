// ****************************************************************************************************
// File: Horizontal.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_HORIZONTAL
#define HURRICANE_HORIZONTAL

#include "hurricane/Segment.h"
#include "hurricane/Horizontals.h"

namespace Hurricane {



// ****************************************************************************************************
// Horizontal declaration
// ****************************************************************************************************

class Horizontal : public Segment {
// ******************************

// Types
// *****

    public: typedef Segment Inherit;

// Attributes
// **********

    private: Unit _y;
    private: Unit _dxSource;
    private: Unit _dxTarget;

// Constructors
// ************

    protected: Horizontal(Net* net, Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);

    public: static Horizontal* create(Net* net, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);
    public: static Horizontal* create(Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width = 0, const Unit& dxSource = 0, const Unit& dxTarget = 0);

// Accessors
// *********

    public: virtual Unit getY() const {return _y;};
    public: virtual Point getCenter() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual Unit getSourceX() const;
    public: virtual Unit getSourceY() const {return getY();};
    public: virtual Unit getTargetX() const;
    public: virtual Unit getTargetY() const {return getY();};
    public: virtual Unit getLength() const;
    public: const Unit& getDxSource() const {return _dxSource;};
    public: const Unit& getDxTarget() const {return _dxTarget;};

// Updators
// ********

    public: virtual void translate(const Unit& dx, const Unit& dy);

    public: void setY(const Unit& y);
    public: void setDxSource(const Unit& dxSource);
    public: void setDxTarget(const Unit& dxSource);
    public: void translate(const Unit& dy);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Horizontal");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Horizontal)

#endif // HURRICANE_HORIZONTAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
