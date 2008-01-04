// ****************************************************************************************************
// File: Pad.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6 (to be seen : PROVISOIREMENT in _Draw & _Highlight)

#include "Pad.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Pad implementation
// ****************************************************************************************************

Pad::Pad(Net* net, Layer* layer, const Box& boundingBox)
// *****************************************************
:  Inherit(net),
    _layer(layer),
    _boundingBox(boundingBox)
{
    if (!_layer)
        throw Error("Can't create " + _TName("Pad") + " : null layer");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("Pad") + " : empty bounding box");
}

Pad* Pad::Create(Net* net, Layer* layer, const Box& boundingBox)
// *************************************************************
{
    Pad* pad = new Pad(net, layer, boundingBox);

    pad->_PostCreate();

    return pad;
}

Unit Pad::GetX() const
// *******************
{
    return 0;
}

Unit Pad::GetY() const
// *******************
{
    return 0;
}

Box Pad::GetBoundingBox() const
// ****************************
{
    Box boundingBox = _boundingBox;

    if (is_a<CompositeLayer*>(_layer))
        boundingBox.inflate(((CompositeLayer*)_layer)->GetMaximalPadSize());

    return boundingBox;
}

Box Pad::GetBoundingBox(BasicLayer* basicLayer) const
// **************************************************
{
    if (!_layer->Contains(basicLayer)) return Box();

    Box boundingBox = _boundingBox;

    if (is_a<CompositeLayer*>(_layer))
        boundingBox.inflate(((CompositeLayer*)_layer)->GetPadSize(basicLayer));

    return boundingBox;
}

void Pad::Translate(const Unit& dx, const Unit& dy)
// ************************************************
{
    if ((dx != 0) || (dy != 0)) {
        Invalidate(true);
        _boundingBox.translate(dx, dy);
    }
}

void Pad::SetBoundingBox(const Box& boundingBox)
// *********************************************
{
    if (_boundingBox.isEmpty())
        throw Error("Can't set bounding box : empty bounding box");

    if (boundingBox != _boundingBox) {
        Invalidate(true);
        _boundingBox = boundingBox;
    }
}

string Pad::_GetString() const
// ***************************
{
    string s = Inherit::_GetString();
    s.insert(s.length() - 1, " " + GetString(_layer->GetName()));
    s.insert(s.length() - 1, " " + GetString(_boundingBox));
    return s;
}

Record* Pad::_GetRecord() const
// **********************
{
    Record* record = Inherit::_GetRecord();
    if (record) {
        record->Add(GetSlot("Layer", _layer));
        record->Add(GetSlot("BoundingBox", &_boundingBox));
    }
    return record;
}

//void Pad::_Draw(View* view, BasicLayer* basicLayer, const Box& updateArea, const Transformation& transformation)
//// ****************************************************************************************************
//{
//    Unit width = _boundingBox.GetWidth();
//    Unit height = _boundingBox.GetHeight();
//    if (1 < view->GetScreenSize(max(width, height))) {
//        basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//        view->DrawRectangle(transformation.GetBox(GetBoundingBox(basicLayer))); // PROVISOIREMENT
//    }
//}
//
//void Pad::_Highlight(View* view, const Box& updateArea, const Transformation& transformation)
//// ******************************************************************************************
//{
//    for_each_basic_layer(basicLayer, GetLayer()->GetBasicLayers()) {
//        basicLayer->_Fill(view, transformation.GetBox(GetBoundingBox(basicLayer)));
//        view->DrawRectangle(transformation.GetBox(GetBoundingBox(basicLayer))); // PROVISOIREMENT
//        end_for;
//    }
//}
//

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
