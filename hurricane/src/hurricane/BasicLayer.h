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

        public: const Code& getCode() const {return _code;};

        public: string _getTypeName() const { return _TName("BasicLayer::Type"); };
        public: string _getString() const;
        public: Record* _getRecord() const;

    };

// Attributes
// **********

    private: Type _type;
    private: unsigned _extractNumber;
    private: BasicLayer* _connectorLayer;
    private: BasicLayer* _obstructionLayer;
    private: Name _realName;

// Constructors
// ************

    protected: BasicLayer(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

    public: static BasicLayer* create(Technology* technology, const Name& name, const Type& type, unsigned extractNumber, const Unit& minimalSize = 0, const Unit& minimalSpacing = 0);

// Accessors
// *********

    public: const Type& getType() const {return _type;};
    public: unsigned getExtractNumber() const {return _extractNumber;};
    public: virtual BasicLayers getBasicLayers() const;
    public: virtual BasicLayer* getConnectorLayer() const {return _connectorLayer;}; 
    public: virtual BasicLayer* getObstructionLayer() const {return _obstructionLayer;}; 
    public: const Name& getRealName () const { return _realName; };

// Updators
// ********

    public: void setConnectorLayer(BasicLayer* layer) {_connectorLayer = layer;};
    public: void setObstructionLayer(BasicLayer* layer) {_obstructionLayer = layer;};
    public: void setRealName(const char* realName) { _realName = realName; };

// Others
// ******

    protected: virtual void _postCreate();

    protected: virtual void _preDestroy();

    public: virtual string _getTypeName() const {return _TName("BasicLayer");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
    public: virtual BasicLayer* _getSymbolicBasicLayer() {return this;};

    public: void _fill(View* view, const Box& box) const;

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
                                                         Record* record = new Record(getString(object));
                                                         record->add(getSlot("Code", (unsigned int*)object));
                                                         return record;
                                                       }


} // End of Hurricane namespace.


SetNestedSlotAdapter(Hurricane::BasicLayer)


bool Scan(const string& s, Hurricane::BasicLayer::Type& type);


#endif // HURRICANE_BASIC_LAYER

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
