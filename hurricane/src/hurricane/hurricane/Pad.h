// ****************************************************************************************************
// File: Pad.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_PAD
#define HURRICANE_PAD

#include "hurricane/Component.h"
#include "hurricane/Pads.h"

namespace Hurricane {

class Layer;



// ****************************************************************************************************
// Pad declaration
// ****************************************************************************************************

class Pad : public Component {
// *************************

// Types
// *****

    public: typedef Component Inherit;

// Attributes
// **********

    private: Layer* _layer;
    private: Box _boundingBox;

// Constructors
// ************

    protected: Pad(Net* net, Layer* layer, const Box& boundingBox);

    public: static Pad* create(Net* net, Layer* layer, const Box& boundingBox);

// Accessors
// *********

    public: virtual Unit getX() const;
    public: virtual Unit getY() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual Layer* getLayer() const {return _layer;};

// Updators
// ********

    public: virtual void translate(const Unit& dx, const Unit& dy);
    public: void setBoundingBox(const Box& boundingBox);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Pad");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::Pad)

#endif // HURRICANE_PAD

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
