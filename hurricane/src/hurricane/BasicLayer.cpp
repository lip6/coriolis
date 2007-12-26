// ****************************************************************************************************
// File: BasicLayer.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "BasicLayer.h"
#include "Technology.h"
#include "CompositeLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// BasicLayer_BasicLayers declaration
// ****************************************************************************************************

class BasicLayer_BasicLayers : public Collection<BasicLayer*> {
// **********************************************************

// Types
// *****

	public: typedef Collection<BasicLayer*> Inherit;

	public: class Locator : public Hurricane::Locator<BasicLayer*> {
	// ***********************************************************

		public: typedef Hurricane::Locator<BasicLayer*> Inherit;

		private: const BasicLayer* _basicLayer;

		public: Locator(const BasicLayer* basicLayer = NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual BasicLayer* GetElement() const;
		public: virtual Hurricane::Locator<BasicLayer*>* GetClone() const;

		public: virtual bool IsValid() const;

		public: virtual void Progress();

		public: virtual string _GetString() const;

	};

// Atributes
// *********

	private: const BasicLayer* _basicLayer;

// Constructors
// ************

	public: BasicLayer_BasicLayers(const BasicLayer* basicLayer = NULL);
	public: BasicLayer_BasicLayers(const BasicLayer_BasicLayers& basicLayers);

// Operators
// *********

	public: BasicLayer_BasicLayers& operator=(const BasicLayer_BasicLayers& basicLayers);

// Accessors
// *********

	public: virtual Collection<BasicLayer*>* GetClone() const;
	public: virtual Hurricane::Locator<BasicLayer*>* GetLocator() const;

// Others
// ******

	public: virtual string _GetString() const;

};



// ****************************************************************************************************
// BasicLayer implementation
// ****************************************************************************************************

BasicLayer::BasicLayer(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
:	Inherit(technology, name, minimalSize, minimalSpacing),
	_type(type),
	_extractNumber(extractNumber),
	_redValue(255),
	_greenValue(255),
	_blueValue(255),
	_fillPattern("FFFFFFFFFFFFFFFF"),
	_displayThreshold(0.0)
{
}

BasicLayer* BasicLayer::Create(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize, const Unit& minimalSpacing)
// ****************************************************************************************************
{
	BasicLayer* basicLayer =
		new BasicLayer(technology, name, type, extractNumber, minimalSize, minimalSpacing);

	basicLayer->_PostCreate();

	return basicLayer;
}

BasicLayers BasicLayer::GetBasicLayers() const
// *******************************************
{
	return BasicLayer_BasicLayers(this);
}

void BasicLayer::SetColor(unsigned short redValue, unsigned short greenValue, unsigned short blueValue)
// ****************************************************************************************************
{
	if ((redValue != _redValue) || (greenValue != _greenValue) || (blueValue != _blueValue)) {
		_redValue = redValue;
		_greenValue = greenValue;
		_blueValue = blueValue;
		//if (_drawGC) {
		//	gdk_gc_destroy(_drawGC);
		//	_drawGC = NULL;
		//}
		//if (_fillGC) {
		//	gdk_gc_destroy(_fillGC);
		//	_fillGC = NULL;
		//}
	}
}

void BasicLayer::SetFillPattern(const string& fillPattern)
// *******************************************************
{
	if (fillPattern != _fillPattern) {
		if (fillPattern.size() != 16)
			throw Error("Can't set fill pattern (bad value)");

		string validChars = "0123456789ABCDEFabcdef";
		for (unsigned i = 0; i < 16; i++) {
			if (validChars.find(fillPattern[i]) == string::npos)
				throw Error("Can't set fill pattern (bad value)");
		}
		_fillPattern = fillPattern;
		//if (_drawGC) {
		//	gdk_gc_destroy(_drawGC);
		//	_drawGC = NULL;
		//}
		//if (_fillGC) {
		//	gdk_gc_destroy(_fillGC);
		//	_fillGC = NULL;
		//}
	}
}

void BasicLayer::_PostCreate()
// ***************************
{
	Mask basicLayersMask = 0;
	for_each_basic_layer(basicLayer, GetTechnology()->GetBasicLayers()) {
		basicLayersMask |= basicLayer->GetMask();
		end_for;
	}

	Mask mask = 1;
	while (mask && (mask & basicLayersMask)) mask = mask<<1;

	if (!mask)
		throw Error("Can't create " + _TName("BasicLayer") + " : mask capacity overflow");

	_SetMask(mask);

	if (_extractNumber) {
		Mask extractMask = (1 << _extractNumber);

		if (!extractMask)
			throw Error("Can't create " + _TName("BasicLayer") + " : extract mask capacity overflow");

		_SetExtractMask(extractMask);
	}

	Inherit::_PostCreate();
}

void BasicLayer::_PreDelete()
// **************************
{
	Inherit::_PreDelete();

	//if (_drawGC) gdk_gc_destroy(_drawGC);
	//if (_fillGC) gdk_gc_destroy(_fillGC);

	CompositeLayers compositeLayers = GetTechnology()->GetCompositeLayers();
	for_each_composite_layer(compositeLayer, compositeLayers) {
		if (compositeLayer->Contains(this)) compositeLayer->Remove(this);
		end_for;
	}
}

string BasicLayer::_GetString() const
// **********************************
{
	string s = Inherit::_GetString();
	// s.insert(s.length() - 1, " " + GetString(_type));
	return s;
}

Record* BasicLayer::_GetRecord() const
// *****************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Type", &_type));
		record->Add(GetSlot("RedValue", &_redValue));
		record->Add(GetSlot("GreenValue", &_greenValue));
		record->Add(GetSlot("BlueValue", &_blueValue));
		record->Add(GetSlot("FillPattern", &_fillPattern));
		record->Add(GetSlot("DisplayThreshold", &_displayThreshold));
	}
	return record;
}

//GdkGC* BasicLayer::_GetDrawGC()
//// ****************************
//{
//	if (!_drawGC) _drawGC = gtk_gc_new(_redValue, _greenValue, _blueValue);
//
//	return _drawGC;
//}
//
//GdkGC* BasicLayer::_GetFillGC()
//// ****************************
//{
//	if (!_fillGC) _fillGC = gtk_gc_new(_redValue, _greenValue, _blueValue, _fillPattern);
//
//	return _fillGC;
//}
//

//void BasicLayer::_Fill(View* view, const Box& box) const
//// *****************************************************
//{
//	switch (_type) {
//		case Type::CONTACT : {
//
//			Unit minimalSize = GetMinimalSize();
//			Unit minimalSpacing = GetMinimalSpacing();
//
//			if ((minimalSize <= 0) || (minimalSpacing <= 0))
//				view->FillRectangle(box);
//			else {
//				view->DrawRectangle(box);
//
//				Unit width = box.GetWidth();
//				Unit height = box.GetHeight();
//				Unit offset = minimalSize + minimalSpacing;
//
//				int nx = (int)(GetValue(width) / GetValue(offset));
//				int ny = (int)(GetValue(height) / GetValue(offset));
//
//				Unit dx = (width - (minimalSize + (offset * nx))) / 2;
//				Unit dy = (height - (minimalSize + (offset * ny))) / 2;
//
//				if (dx < 0) dx = (width - (minimalSize + (offset * (--nx)))) / 2;
//				if (dy < 0) dy = (height - (minimalSize + (offset * (--ny)))) / 2;
//
//				Unit xmin = box.GetXMin() + dx;
//				Unit ymin = box.GetYMin() + dy;
//				Unit xmax = box.GetXMax() - dx;
//				Unit ymax = box.GetYMax() - dy;
//
//				if ((xmin < xmax) && (ymin < ymax)) {
//					Unit y = ymin;
//					do {
//						Unit x = xmin;
//						do {
//							view->FillRectangle(x, y, x + minimalSize, y + minimalSize, true);
//							x += offset;
//						} while (x < xmax);
//						y += offset;
//					} while (y < ymax);
//				}
//			}
//			break;
//		}
//		default : {
//			view->FillRectangle(box);
//			break;
//		}
//	}
//}
//
// ****************************************************************************************************
// BasicLayer_BasicLayers implementation
// ****************************************************************************************************

BasicLayer_BasicLayers::BasicLayer_BasicLayers(const BasicLayer* basicLayer)
// *************************************************************************
: 	Inherit(),
	_basicLayer(basicLayer)
{
}

BasicLayer_BasicLayers::BasicLayer_BasicLayers(const BasicLayer_BasicLayers& basicLayers)
// **************************************************************************************
: 	Inherit(),
	_basicLayer(basicLayers._basicLayer)
{
}

BasicLayer_BasicLayers& BasicLayer_BasicLayers::operator=(const BasicLayer_BasicLayers& basicLayers)
// *************************************************************************************************
{
	_basicLayer = basicLayers._basicLayer;
	return *this;
}

Collection<BasicLayer*>* BasicLayer_BasicLayers::GetClone() const
// **************************************************************
{
	return new BasicLayer_BasicLayers(*this);
}

Locator<BasicLayer*>* BasicLayer_BasicLayers::GetLocator() const
// *************************************************************
{
	return new Locator(_basicLayer);
}

string BasicLayer_BasicLayers::_GetString() const
// **********************************************
{
	string s = "<" + _TName("BasicLayer::BasicLayers");
	if (_basicLayer) s += " " + GetString(_basicLayer);
	s += ">";
	return s;
}



// ****************************************************************************************************
// BasicLayer_BasicLayers::Locator implementation
// ****************************************************************************************************

BasicLayer_BasicLayers::Locator::Locator(const BasicLayer* basicLayer)
// *******************************************************************
:	Inherit(),
	_basicLayer(basicLayer)
{
}

BasicLayer_BasicLayers::Locator::Locator(const Locator& locator)
// *************************************************************
:	Inherit(),
	_basicLayer(locator._basicLayer)
{
}

BasicLayer_BasicLayers::Locator& BasicLayer_BasicLayers::Locator::operator=(const Locator& locator)
// ************************************************************************************************
{
	_basicLayer = locator._basicLayer;
	return *this;
}

BasicLayer* BasicLayer_BasicLayers::Locator::GetElement() const
// ************************************************************
{
	return (BasicLayer*)_basicLayer;
}

Locator<BasicLayer*>* BasicLayer_BasicLayers::Locator::GetClone() const
// ********************************************************************
{
	return new Locator(*this);
}

bool BasicLayer_BasicLayers::Locator::IsValid() const
// **************************************************
{
	return (_basicLayer != NULL);
}

void BasicLayer_BasicLayers::Locator::Progress()
// *********************************************
{
	_basicLayer = NULL;
}

string BasicLayer_BasicLayers::Locator::_GetString() const
// *******************************************************
{
	string s = "<" + _TName("BasicLayer::BasicLayers::Locator");
	if (_basicLayer) s += " " + GetString(_basicLayer);
	s += ">";
	return s;
}



// ****************************************************************************************************
// BasicLayer::Type implementation
// ****************************************************************************************************

BasicLayer::Type::Type(const Code& code)
// *************************************
:	_code(code)
{
}

BasicLayer::Type::Type(const Type& type)
// *************************************
:	_code(type._code)
{
}

BasicLayer::Type& BasicLayer::Type::operator=(const Type& type)
// ************************************************************
{
	_code = type._code;
	return *this;
}

string BasicLayer::Type::_GetString() const
// ****************************************
{
  return GetString(_code);
}

Record* BasicLayer::Type::_GetRecord() const
// ***********************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Code", &_code));
	return record;
}

} // End of Hurricane namespace.

bool Scan(const string& s, H::BasicLayer::Type& type)
// **************************************************
{
	if (s == "UNDEFINED") {
		type = H::BasicLayer::Type::UNDEFINED;
		return true;
	}
	if (s == "CONDUCTING") {
		type = H::BasicLayer::Type::CONDUCTING;
		return true;
	}
	if (s == "CONTACT") {
		type = H::BasicLayer::Type::CONTACT;
		return true;
	}
	return false;
}

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
