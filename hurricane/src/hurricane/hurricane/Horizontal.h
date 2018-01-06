// ****************************************************************************************************
// File: ./hurricane/Horizontal.h
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

    public: virtual void _toJson(JsonWriter*) const;
    public: static JsonObject* getJsonObject(unsigned long flags);
    public: virtual string _getTypeName() const {return _TName("Horizontal");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


class JsonHorizontal : public JsonSegment {
// ****************************************

  public: static void initialize();
  public: JsonHorizontal(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonHorizontal* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};

} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Horizontal);


#endif // HURRICANE_HORIZONTAL


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
