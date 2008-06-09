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


  class Layer;


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

    private: DbU::Unit _y;
    private: DbU::Unit _dxSource;
    private: DbU::Unit _dxTarget;

// Constructors
// ************

    protected: Horizontal ( Net* net
                          , Component* source
                          , Component* target
                          , const Layer* layer
                          , const DbU::Unit& y
                          , const DbU::Unit& width    = 0
                          , const DbU::Unit& dxSource = 0
                          , const DbU::Unit& dxTarget = 0
                          );
    public: static Horizontal* create ( Net* net
                                      , const Layer* layer
                                      , const DbU::Unit& y
                                      , const DbU::Unit& width    = 0
                                      , const DbU::Unit& dxSource = 0
                                      , const DbU::Unit& dxTarget = 0
                                      );
    public: static Horizontal* create ( Component* source
                                      , Component* target
                                      , const Layer* layer
                                      , const DbU::Unit& y
                                      , const DbU::Unit& width    = 0
                                      , const DbU::Unit& dxSource = 0
                                      , const DbU::Unit& dxTarget = 0
                                      );

// Accessors
// *********

    public: virtual DbU::Unit getY() const {return _y;};
    public: virtual Point getCenter() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual DbU::Unit getSourceX() const;
    public: virtual DbU::Unit getSourceY() const {return getY();};
    public: virtual DbU::Unit getTargetX() const;
    public: virtual DbU::Unit getTargetY() const {return getY();};
    public: virtual DbU::Unit getLength() const;
    public: const DbU::Unit& getDxSource() const {return _dxSource;};
    public: const DbU::Unit& getDxTarget() const {return _dxTarget;};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setY(const DbU::Unit& y);
    public: void setDxSource(const DbU::Unit& dxSource);
    public: void setDxTarget(const DbU::Unit& dxSource);
    public: void translate(const DbU::Unit& dy);

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
