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

    private: const Layer* _layer;
    private: Box _boundingBox;

// Constructors
// ************

    protected: Pad(Net* net, const Layer* layer, const Box& boundingBox);

    public: static Pad* create(Net* net, const Layer* layer, const Box& boundingBox);

// Accessors
// *********

    public: virtual DbU::Unit getX() const;
    public: virtual DbU::Unit getY() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual const Layer* getLayer() const {return _layer;};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: void setBoundingBox(const Box& boundingBox);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Pad");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Pad);


#endif // HURRICANE_PAD

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
