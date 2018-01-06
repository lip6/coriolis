// ****************************************************************************************************
// File: ./hurricane/Pad.h
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

    public: void setLayer(const Layer* layer);
    public: virtual void translate(const DbU::Unit& dx, const DbU::Unit& dy);
    public: void setBoundingBox(const Box& boundingBox);

// Others
// ******

    public: virtual void _toJson(JsonWriter*) const;
    public: static JsonObject* getJsonObject(unsigned long flags);
    public: virtual string _getTypeName() const {return _TName("Pad");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

};


class JsonPad : public JsonComponent {
// ***********************************

  public: static void initialize();
  public: JsonPad(unsigned long flags);
  public: virtual string getTypeName() const;
  public: virtual JsonPad* clone(unsigned long) const;
  public: virtual void toData(JsonStack&); 
};

} // End of Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Pad);


#endif // HURRICANE_PAD


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
