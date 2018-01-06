// ****************************************************************************************************
// File: ./Horizontal.cpp
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
#include "hurricane/Horizontal.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Plug.h"
#include "hurricane/Error.h"


namespace Hurricane {



// ****************************************************************************************************
// Horizontal implementation
// ****************************************************************************************************

Horizontal::Horizontal(Net* net, Component* source, Component* target, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
    _y(y),
    _dxSource(dxSource),
    _dxTarget(dxTarget)
{
}

Horizontal* Horizontal::create(Net* net, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Horizontal") + " : null net");

    Horizontal* horizontal = new Horizontal(net, NULL, NULL, layer, y, width, dxSource, dxTarget);

    horizontal->_postCreate();

    return horizontal;
}

Horizontal* Horizontal::create(Component* source, Component* target, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
{
    if (!source)
        throw Error("Can't create " + _TName("Horizontal") + " : null source");

    if (!target)
        throw Error("Can't create " + _TName("Horizontal") + " : null target");

    Horizontal* horizontal =
        new Horizontal(source->getNet(), source, target, layer, y, width, dxSource, dxTarget);

    horizontal->_postCreate();

    return horizontal;
}

Box Horizontal::getBoundingBox() const
// ***********************************
{
  DbU::Unit size      = getLayer()->getExtentionWidth() + getHalfWidth();
  DbU::Unit extention = getLayer()->getExtentionCap  ();

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

Box Horizontal::getBoundingBox(const BasicLayer* basicLayer) const
// *********************************************************
{
    if (!getLayer()->contains(basicLayer)) return Box();

    DbU::Unit size      = getLayer()->getExtentionWidth(basicLayer) + getHalfWidth();
    DbU::Unit extention = getLayer()->getExtentionCap  (basicLayer);

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

DbU::Unit Horizontal::getSourceX() const
// ********************************
{
    Component* source = getSource();
    return (source) ? source->getX() + _dxSource : _dxSource;
}

DbU::Unit Horizontal::getTargetX() const
// ********************************
{
    Component* target = getTarget();
    return (target) ? target->getX() + _dxTarget : _dxTarget;
}

DbU::Unit Horizontal::getLength() const
// *******************************
{
    return abs(getSourceX() - getTargetX());
}

Point Horizontal::getCenter() const
// *******************************
{
    return Point ( (getSourceX()+getTargetX())/2, getY() );
}

void Horizontal::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *******************************************************
{
    if (dy != 0) {
        invalidate(true);
        _y += dy;
    }
    if (dx != 0) {
        if (!getSourceHook()->isAttached()) {
            invalidate(true);
            _dxSource += dx;
        }
        if (!getTargetHook()->isAttached()) {
            invalidate(true);
            _dxTarget += dx;
        }
    }
}

void Horizontal::setY(const DbU::Unit& y)
// *********************************
{
    if (y != _y) {
        invalidate(true);
        _y = y;
    }
}

void Horizontal::setDxSource(const DbU::Unit& dxSource)
// ***********************************************
{
    if (dxSource != _dxSource) {
        invalidate(false);
        _dxSource = dxSource;
    }
}

void Horizontal::setDxTarget(const DbU::Unit& dxTarget)
// ***********************************************
{
    if (dxTarget != _dxTarget) {
        invalidate(false);
        _dxTarget = dxTarget;
    }
}

void Horizontal::translate(const DbU::Unit& dy)
// ***************************************
{
    if (dy != 0) {
        invalidate(true);
        _y += dy;
    }
}

void Horizontal::_toJson(JsonWriter* writer) const
// ***********************************************
{
  Inherit::_toJson( writer );

  jsonWrite( writer, "_y"       , _y );
  jsonWrite( writer, "_dxSource", _dxSource );
  jsonWrite( writer, "_dxTarget", _dxTarget );
}

string Horizontal::_getString() const
// **********************************
{
    return Inherit::_getString();
}

Record* Horizontal::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(DbU::getValueSlot("Y", &_y));
        record->add(DbU::getValueSlot("DxSource", &_dxSource));
        record->add(DbU::getValueSlot("DxTarget", &_dxTarget));
    }
    return record;
}


// ****************************************************************************************************
// JsonHorizontal implementation
// ****************************************************************************************************

Initializer<JsonHorizontal>  jsonHorizontalInit ( 0 );


void  JsonHorizontal::initialize()
// *******************************
{ JsonTypes::registerType( new JsonHorizontal (JsonWriter::RegisterMode) ); }

JsonHorizontal::JsonHorizontal(unsigned long flags)
// ************************************************
  : JsonSegment(flags)
{
  add( "_y"       , typeid(uint64_t) );
  add( "_dxSource", typeid(uint64_t) );
  add( "_dxTarget", typeid(uint64_t) );
}

string JsonHorizontal::getTypeName() const
// ***************************************
{ return "Horizontal"; }

JsonHorizontal* JsonHorizontal::clone(unsigned long flags) const
// *************************************************************
{ return new JsonHorizontal ( flags ); }

void JsonHorizontal::toData(JsonStack& stack)
// ******************************************
{
  check( stack, "JsonHorizontal::toData" );
  unsigned int jsonId = presetId( stack );

  Horizontal* horizontal = Horizontal::create
    ( get<Net*>(stack,".Net")
    , DataBase::getDB()->getTechnology()->getLayer( get<string>(stack,"_layer") )
    , DbU::fromDb( get<int64_t>(stack,"_y"       ) )
    , DbU::fromDb( get<int64_t>(stack,"_width"   ) )
    , DbU::fromDb( get<int64_t>(stack,"_dxSource") )
    , DbU::fromDb( get<int64_t>(stack,"_dxTarget") )
    );
  
  JsonNet* jnet = jget<JsonNet>( stack );
  if (jnet) {
    jnet->addHookLink( horizontal->getBodyHook  (), jsonId, get<string>(stack,"_bodyHook"  ) );
    jnet->addHookLink( horizontal->getSourceHook(), jsonId, get<string>(stack,"_sourceHook") );
    jnet->addHookLink( horizontal->getTargetHook(), jsonId, get<string>(stack,"_targetHook") );
  } else {
    cerr << Error( "JsonHorizontal::toData(): Missing (Json)Net in stack context." ) << endl;
  }

// Hook/Ring rebuild are done as a post-process.
  update( stack, horizontal );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
