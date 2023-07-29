// ****************************************************************************************************
// File: ./Pad.cpp
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

#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Pad implementation
// ****************************************************************************************************

Pad::Pad(Net* net, const Layer* layer, const Box& boundingBox)
// ***********************************************************
:  Inherit(net),
    _layer(layer),
    _boundingBox(boundingBox)
{
    if (!_layer)
        throw Error("Can't create " + _TName("Pad") + " : null layer");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("Pad") + " : empty bounding box");
}

Pad* Pad::create(Net* net, const Layer* layer, const Box& boundingBox)
// *******************************************************************
{
    Pad* pad = new Pad(net, layer, boundingBox);

    pad->_postCreate();

    return pad;
}

DbU::Unit Pad::getX() const
// *******************
{
  return _boundingBox.getXCenter();
}

DbU::Unit Pad::getY() const
// *******************
{
  return _boundingBox.getYCenter();
}

Box Pad::getBoundingBox() const
// ****************************
{
    Box boundingBox = _boundingBox;

    return boundingBox;
}

Box Pad::getBoundingBox(const BasicLayer* basicLayer) const
// **************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    Box boundingBox = _boundingBox;

    return boundingBox;
}

void Pad::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _boundingBox.translate(dx, dy);
    }
}

void Pad::setLayer(const Layer* layer)
// ***********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Pad::setBoundingBox(const Box& boundingBox)
// *********************************************
{
    if (_boundingBox.isEmpty())
        throw Error("Can't set bounding box : empty bounding box");

    if (boundingBox != _boundingBox) {
        invalidate(true);
        _boundingBox = boundingBox;
    }
}

void Pad::_toJson(JsonWriter* writer) const
// ****************************************
{
  Inherit::_toJson( writer );

  jsonWrite( writer, "_layer"      , _layer->getName() );
  jsonWrite( writer, "_boundingBox", &_boundingBox     );
}

string Pad::_getString() const
// ***************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " " + getString(_boundingBox));
    return s;
}

Record* Pad::_getRecord() const
// **********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Layer", _layer));
        record->add(getSlot("BoundingBox", &_boundingBox));
    }
    return record;
}


// ****************************************************************************************************
// JsonPad implementation
// ****************************************************************************************************

Initializer<JsonPad>  jsonPadInit ( 0 );


void  JsonPad::initialize()
// ************************
{ JsonTypes::registerType( new JsonPad (JsonWriter::RegisterMode) ); }

JsonPad::JsonPad(unsigned long flags)
// **********************************
  : JsonComponent(flags)
{
  add( "_layer"      , typeid(string) );
  add( "_boundingBox", typeid(Box)    );
}

string JsonPad::getTypeName() const
// ********************************
{ return "Pad"; }

JsonPad* JsonPad::clone(unsigned long flags) const
// ***********************************************
{ return new JsonPad ( flags ); }

void JsonPad::toData(JsonStack& stack)
// ***********************************
{
  check( stack, "JsonPad::toData" );
  unsigned int jsonId = presetId( stack );

  Pad* pad = Pad::create
    ( get<Net*>(stack,".Net")
    , DataBase::getDB()->getTechnology()->getLayer( get<const char*>(stack,"_layer") )
    , get<Box>(stack,".Box")
    );
  
  JsonNet* jnet = jget<JsonNet>( stack );
  if (jnet) {
    jnet->addHookLink( pad->getBodyHook  (), jsonId, get<string>(stack,"_bodyHook"  ) );
  } else {
    cerr << Error( "JsonPad::toData(): Missing (Json)Net in stack context." ) << endl;
  }

// Hook/Ring rebuild are done as a post-process.
  update( stack, pad );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
