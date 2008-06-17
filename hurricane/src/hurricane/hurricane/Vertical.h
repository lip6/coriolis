// ****************************************************************************************************
// File: Vertical.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_VERTICAL
#define HURRICANE_VERTICAL

#include "hurricane/Segment.h"
#include "hurricane/Verticals.h"

namespace Hurricane {


  class Layer;


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

    private: DbU::Unit _x;
    private: DbU::Unit _dySource;
    private: DbU::Unit _dyTarget;

// Constructors
// ************

    protected: Vertical ( Net* net
                        , Component* source
                        , Component* target
                        , const Layer* layer
                        , const DbU::Unit& x
                        , const DbU::Unit& width    = 0
                        , const DbU::Unit& dySource = 0
                        , const DbU::Unit& dyTarget = 0
                        );

    public: static Vertical* create ( Net* net
                                    , const Layer* layer
                                    , const DbU::Unit& x
                                    , const DbU::Unit& width    = 0
                                    , const DbU::Unit& dySource = 0
                                    , const DbU::Unit& dyTarget = 0
                                    );
    public: static Vertical* create ( Component* source
                                    , Component* target
                                    , const Layer* layer
                                    , const DbU::Unit& x
                                    , const DbU::Unit& width    = 0
                                    , const DbU::Unit& dySource = 0
                                    , const DbU::Unit& dyTarget = 0
                                    );

// Accessors
// *********

    public: virtual DbU::Unit getX() const {return _x;};
    public: virtual Point getCenter() const;
    public: virtual Box getBoundingBox() const;
    public: virtual Box getBoundingBox(const BasicLayer* basicLayer) const;
    public: virtual DbU::Unit getSourceX() const {return getX();};
    public: virtual DbU::Unit getSourceY() const;
    public: virtual DbU::Unit getTargetX() const {return getX();};
    public: virtual DbU::Unit getTargetY() const;
    public: virtual DbU::Unit getLength() const;
    public: const DbU::Unit& getDySource() const {return _dySource;};
    public: const DbU::Unit& getDyTarget() const {return _dyTarget;};

// Updators
// ********

    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);

    public: void setX(const DbU::Unit& x);
    public: void setDySource(const DbU::Unit& dySource);
    public: void setDyTarget(const DbU::Unit& dyTarget);
    public: void translate(const DbU::Unit& dx);

// Others
// ******

    public: virtual string _getTypeName() const {return _TName("Vertical");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Vertical);


#endif // HURRICANE_VERTICAL

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
