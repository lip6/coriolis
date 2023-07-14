// ****************************************************************************************************
// File: ./hurricane/Transformation.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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
        public: Orientation(const string& );

        public: Orientation& operator=(const Orientation& orientation);

        public: operator const Code&() const {return _code;};

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("Transformation::Orientation"); };
        public: string _getString() const;
        public: Record* _getRecord() const;
    };

// Attributes
// **********

    private: DbU::Unit _tx;
    private: DbU::Unit _ty;
    private: Orientation _orientation;

// Constructors
// ************

    public: Transformation();

    public: Transformation(const DbU::Unit& tx, const DbU::Unit& ty, const Orientation& orientation = Orientation::ID);
    public: Transformation(const Point& translation, const Orientation& orientation = Orientation::ID);

    public: Transformation(const Transformation& transformation);

// Operators
// *********

    public: Transformation& operator=(const Transformation& transformation);

    public: bool operator==(const Transformation& transformation) const;
    public: bool operator!=(const Transformation& transformation) const;

// Accessors
// *********

    public: const DbU::Unit& getTx() const {return _tx;};
    public: const DbU::Unit& getTy() const {return _ty;};
    public: Point getTranslation() const {return Point(_tx, _ty);};
    public: const Orientation& getOrientation() const {return _orientation;};

    public: DbU::Unit getX(const DbU::Unit& x, const DbU::Unit& y) const;
    public: DbU::Unit getY(const DbU::Unit& x, const DbU::Unit& y) const;

    public: DbU::Unit getX(const Point& point) const;
    public: DbU::Unit getY(const Point& point) const;

    public: DbU::Unit getDx(const DbU::Unit& dx, const DbU::Unit& dy) const;
    public: DbU::Unit getDy(const DbU::Unit& dx, const DbU::Unit& dy) const;

    public: Point getPoint(const DbU::Unit& x, const DbU::Unit& y) const;
    public: Point getPoint(const Point& point) const;

    public: Box getBox(const DbU::Unit& x1, const DbU::Unit& y1, const DbU::Unit& x2, const DbU::Unit& y2) const;
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

    public: void applyOn(DbU::Unit& x, DbU::Unit& y) const;
    public: void applyOn(Point& point) const;
    public: void applyOn(Box& box) const;
    public: void applyOn(Transformation& transformation) const;

// Others
// ******

    public: string _getTypeName() const { return _TName("Transformation"); };
    public: string _getString() const;
    public: Record* _getRecord() const;
    public:  void toJson(JsonWriter*) const;

};

class JsonTransformation : public JsonObject {
// *******************************************

  public: static void initialize();
  public: JsonTransformation(unsigned long);
  public: virtual string getTypeName() const;
  public: virtual JsonTransformation* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};


} // End of Hurricane namespace.


// -------------------------------------------------------------------
// Inspector Support for  :  "Transformation::Orientation::Code*".

template<>
inline std::string  getString<const Hurricane::Transformation::Orientation::Code*>
                             ( const Hurricane::Transformation::Orientation::Code* object )
                             {
                               switch ( *object ) {
                                 case Hurricane::Transformation::Orientation::ID: return "ID";
                                 case Hurricane::Transformation::Orientation::R1: return "R1";
                                 case Hurricane::Transformation::Orientation::R2: return "R2";
                                 case Hurricane::Transformation::Orientation::R3: return "R3";
                                 case Hurricane::Transformation::Orientation::MX: return "MX";
                                 case Hurricane::Transformation::Orientation::XR: return "XR";
                                 case Hurricane::Transformation::Orientation::MY: return "MY";
                                 case Hurricane::Transformation::Orientation::YR: return "YR";
                               }
                               return "ABNORMAL";
                             }

template<>
inline Hurricane::Record* getRecord<const Hurricane::Transformation::Orientation::Code*>
                                   ( const Hurricane::Transformation::Orientation::Code* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Code", (unsigned int*)object));
                                     return record;
                                   }

template<>
inline std::string  getString<Hurricane::Transformation::Orientation::Code*>
                             ( Hurricane::Transformation::Orientation::Code* object )
                             {
                               switch ( *object ) {
                                 case Hurricane::Transformation::Orientation::ID: return "ID";
                                 case Hurricane::Transformation::Orientation::R1: return "R1";
                                 case Hurricane::Transformation::Orientation::R2: return "R2";
                                 case Hurricane::Transformation::Orientation::R3: return "R3";
                                 case Hurricane::Transformation::Orientation::MX: return "MX";
                                 case Hurricane::Transformation::Orientation::XR: return "XR";
                                 case Hurricane::Transformation::Orientation::MY: return "MY";
                                 case Hurricane::Transformation::Orientation::YR: return "YR";
                               }
                               return "ABNORMAL";
                             }

template<>
inline Hurricane::Record* getRecord<Hurricane::Transformation::Orientation::Code*>
                                   ( Hurricane::Transformation::Orientation::Code* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Code", (unsigned int*)object));
                                     return record;
                                   }


INSPECTOR_PR_SUPPORT(Hurricane::Transformation);
INSPECTOR_PR_SUPPORT(Hurricane::Transformation::Orientation);
IOSTREAM_POINTER_SUPPORT(Hurricane::Transformation::Orientation::Code);

#endif // HURRICANE_TRANSFORMATION


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
