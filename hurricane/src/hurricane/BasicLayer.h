// ****************************************************************************************************
// File: BasicLayer.h
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#ifndef HURRICANE_BASIC_LAYER
#define HURRICANE_BASIC_LAYER

#include "Layer.h"
#include "BasicLayers.h"
#include "Box.h"

namespace Hurricane {

class View;



// ****************************************************************************************************
// BasicLayer declaration
// ****************************************************************************************************

class BasicLayer : public Layer {
// ****************************

// Types
// *****

	public: typedef Layer Inherit;

    public: class Type {
	// ***************

		public: enum Code {UNDEFINED=0, CONDUCTING=1, CONTACT=2};

		private: Code _code;

		public: Type(const Code& code = UNDEFINED);
		public: Type(const Type& type);

		public: Type& operator=(const Type& type);

		public: operator const Code&() const {return _code;};

		public: const Code& GetCode() const {return _code;};

        public: string _GetTypeName() const { return _TName("BasicLayer::Type"); };
		public: string _GetString() const;
		public: Record* _GetRecord() const;

	};

// Attributes
// **********

	private: Type _type;
	private: unsigned _extractNumber;
	private: unsigned short _redValue;
	private: unsigned short _greenValue;
	private: unsigned short _blueValue;
	private: string _fillPattern;
	private: double _displayThreshold;
//	private: GdkGC* _drawGC;
//	private: GdkGC* _fillGC;

// Constructors
// ************

	protected: BasicLayer(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

	public: static BasicLayer* Create(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

// Accessors
// *********

	public: const Type& GetType() const {return _type;};
	public: unsigned GetExtractNumber() const {return _extractNumber;};
	public: const unsigned short& GetRedValue() const {return _redValue;};
	public: const unsigned short& GetGreenValue() const {return _greenValue;};
	public: const unsigned short& GetBlueValue() const {return _blueValue;};
	public: const string& GetFillPattern() const {return _fillPattern;};
	public: double GetDisplayThreshold() const {return _displayThreshold;};
	public: virtual BasicLayers GetBasicLayers() const;

// Updators
// ********

	public: void SetColor(unsigned short redValue, unsigned short greenValue, unsigned short blueValue);
	public: void SetFillPattern(const string& fillPattern);
	public: void SetDisplayThreshold(double threshold) {_displayThreshold = threshold;};

// Others
// ******

	protected: virtual void _PostCreate();

	protected: virtual void _PreDelete();

	public: virtual string _GetTypeName() const {return _TName("BasicLayer");};
	public: virtual string _GetString() const;
	public: virtual Record* _GetRecord() const;
	public: virtual BasicLayer* _GetSymbolicBasicLayer() {return this;};
	//public: GdkGC* _GetDrawGC();
	//public: GdkGC* _GetFillGC();

	public: void _Fill(View* view, const Box& box) const;

};




// -------------------------------------------------------------------
// Class  :  "Proxy...<const BasicLayer::Type::Code*>".

template<>
  inline string  ProxyTypeName<BasicLayer::Type::Code> ( const BasicLayer::Type::Code* object )
                                                       { return "<PointerSlotAdapter<BasicLayer::Type::Code>>"; }

template<>
  inline string  ProxyString<BasicLayer::Type::Code>   ( const BasicLayer::Type::Code* object )
                                                       {
                                                         switch ( *object ) {
                                                           case BasicLayer::Type::UNDEFINED:  return "UNDEFINED";
                                                           case BasicLayer::Type::CONDUCTING: return "CONDUCTING";
                                                           case BasicLayer::Type::CONTACT:    return "CONTACT";
                                                         }
                                                         return "ABNORMAL";
                                                       }

template<>
  inline Record* ProxyRecord<BasicLayer::Type::Code>   ( const BasicLayer::Type::Code* object )
                                                       {
                                                         Record* record = new Record(GetString(object));
                                                         record->Add(GetSlot("Code", (unsigned int*)object));
                                                         return record;
                                                       }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::BasicLayer)


bool Scan(const string& s, H::BasicLayer::Type& type);


#endif // HURRICANE_BASIC_LAYER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
