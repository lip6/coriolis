// ****************************************************************************************************
// File: Transformation.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_TRANSFORMATION
#define HURRICANE_TRANSFORMATION

#include "hurricane/Box.h"

namespace Hurricane {



// ****************************************************************************************************
// Transformation declaration
// ****************************************************************************************************

class Transformation {
// *****************

// Types
// *****

    public: class Orientation {
    // **********************

        public: enum Code {ID=0, R1=1, R2=2, R3=3, MX=4, XR=5, MY=6, YR=7};

        private: Code _code;

        public: Orientation(const Code& code = ID);
        public: Orientation(const Orientation& orientation);

        public: Orientation& operator=(const Orientation& orientation);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Transformation::Orientation"); };
        public: string _getString() const;
        public: Record* _getRecord() const;
    };

// Attributes
// **********

    private: Unit _tx;
    private: Unit _ty;
    private: Orientation _orientation;

// Constructors
// ************

    public: Transformation();

    public: Transformation(const Unit& tx, const Unit& ty, const Orientation& orientation = Orientation::ID);
    public: Transformation(const Point& translation, const Orientation& orientation = Orientation::ID);

    public: Transformation(const Transformation& transformation);

// Operators
// *********

    public: Transformation& operator=(const Transformation& transformation);

    public: bool operator==(const Transformation& transformation) const;
    public: bool operator!=(const Transformation& transformation) const;

// Accessors
// *********

    public: const Unit& getTx() const {return _tx;};
    public: const Unit& getTy() const {return _ty;};
    public: Point getTranslation() const {return Point(_tx, _ty);};
    public: const Orientation& getOrientation() const {return _orientation;};

    public: Unit getX(const Unit& x, const Unit& y) const;
    public: Unit getY(const Unit& x, const Unit& y) const;

    public: Unit getX(const Point& point) const;
    public: Unit getY(const Point& point) const;

    public: Unit getDx(const Unit& dx, const Unit& dy) const;
    public: Unit getDy(const Unit& dx, const Unit& dy) const;

    public: Point getPoint(const Unit& x, const Unit& y) const;
    public: Point getPoint(const Point& point) const;

    public: Box getBox(const Unit& x1, const Unit& y1, const Unit& x2, const Unit& y2) const;
    public: Box getBox(const Point& point1, const Point& point2) const;
    public: Box getBox(const Box& box) const;

    public: Transformation getTransformation(const Transformation& transformation) const;

    public: Transformation getInvert() const;

// Predicates
// **********

    public: bool isEven() const {return !(_orientation.getCode() & 1);};
    public: bool isOdd() const {return (_orientation.getCode() & 1);};

// Updators
// ********

    public: Transformation& invert();

// Manipulators
// ************

    public: void applyOn(Unit& x, Unit& y) const;
    public: void applyOn(Point& point) const;
    public: void applyOn(Box& box) const;
    public: void applyOn(Transformation& transformation) const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Transformation"); };
    public: string _getString() const;
    public: Record* _getRecord() const;

};




// -------------------------------------------------------------------
// Class  :  "Proxy...<const Transformation::Orientation::Code*>".

template<>
  inline string  ProxyTypeName<Transformation::Orientation::Code>
                              ( const Transformation::Orientation::Code* object )
                              { return "<PointerSlotAdapter<Transformation::Orientation::Code>>"; }

template<>
  inline string  ProxyString  <Transformation::Orientation::Code>
                              ( const Transformation::Orientation::Code* object )
                              {
                                switch ( *object ) {
                                  case Transformation::Orientation::ID: return "ID";
                                  case Transformation::Orientation::R1: return "R1";
                                  case Transformation::Orientation::R2: return "R2";
                                  case Transformation::Orientation::R3: return "R3";
                                  case Transformation::Orientation::MX: return "MX";
                                  case Transformation::Orientation::XR: return "XR";
                                  case Transformation::Orientation::MY: return "MY";
                                  case Transformation::Orientation::YR: return "YR";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Transformation::Orientation::Code>
                              ( const Transformation::Orientation::Code* object )
                              {
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
                                return record;
                              }



} // End of Hurricane namespace.


ValueIOStreamSupport(Hurricane::Transformation)


#endif // HURRICANE_TRANSFORMATION

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
