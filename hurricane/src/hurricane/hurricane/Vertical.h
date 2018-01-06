// ****************************************************************************************************
// File: ./hurricane/Vertical.h
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

    public: virtual void _toJson(JsonWriter*) const;
    public: static JsonObject* getJsonObject(unsigned long flags);
    public: virtual string _getTypeName() const {return _TName("Vertical");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


class JsonVertical : public JsonSegment {
// **************************************

  public: static void initialize();
  public: JsonVertical(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonVertical* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};

} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Vertical);


#endif // HURRICANE_VERTICAL


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
