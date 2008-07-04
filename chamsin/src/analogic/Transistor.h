#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "hurricane/Name.h"
using namespace Hurricane;

#include "Device.h"

namespace Hurricane {

class Transistor : public Device {
    public:
        class Polarity {
            public:
                enum Code {N=0, P=1};
                
                Polarity(const Code& code): _code(code) {}
                operator const Code&() const { return _code; };
                string _getString() const;
                Record* _getRecord() const;

            private:
                Polarity() {}
                Code _code;
        };

        class AbutmentType {
            public:
                enum Code {INTERNAL=0, LEFT=1, RIGHT=2, SINGLE=3};

                AbutmentType(const Code& code): _code(code) {}
                operator const Code&() const { return _code; };
                string _getString() const;
                Record* _getRecord() const;

            private:
                AbutmentType() {}
                Code _code;
        };

        static const Name DrainName;
        static const Name SourceName;
        static const Name GridName;
        static const Name BulkName;
        static const Name AnonymousName;


        static Transistor* create(Library* library, const Name& name,
                const Polarity& polarity,
                DbU::Unit l, DbU::Unit w,
                const AbutmentType& abutmentType=AbutmentType::SINGLE);
        void createLayout();

        bool isNmos() const { return _polarity == Polarity::N; };
        bool isPmos() const { return _polarity == Polarity::P; };
        bool isInternal() const { return _abutmentType == AbutmentType::INTERNAL; };
        bool isLeft() const     { return _abutmentType == AbutmentType::LEFT; };
        bool isRight() const    { return _abutmentType == AbutmentType::RIGHT; };
        bool isSingle() const   { return _abutmentType == AbutmentType::SINGLE; };

        void setW(DbU::Unit value) { _w = value; createLayout(); }
        void setL(DbU::Unit value) { _l = value; createLayout(); }
        void setPolarity(const Polarity& polarity);

        virtual Record* _getRecord() const;

    protected:
        void _postCreate();

    private:
        Net* _drain;
        Net* _source;
        Net* _grid;
        Net* _bulk;
        Net* _anonymous;
        Polarity _polarity;
        AbutmentType _abutmentType;
        DbU::Unit _l;
        DbU::Unit _w;
        Pad *_source20, *_source22;
        Pad *_drain40, *_drain42;
        Pad *_grid00, *_grid01, *_grid30, *_grid31;
        Pad *_anonymous10, *_anonymous11, *_anonymous12, *_anonymous50;

        Transistor(Library* library, const Name& name,
                const Polarity& polarity,
                DbU::Unit l, DbU::Unit w,
                const AbutmentType& abutmentType);
};

}

template<> inline std::string getString<const Hurricane::Transistor::Polarity::Code*>
(const Hurricane::Transistor::Polarity::Code* object ) {
    switch (*object) {
        case Hurricane::Transistor::Polarity::N: return "N";
        case Hurricane::Transistor::Polarity::P: return "P";
        default: return "ABNORMAL";
    }
}

template<> inline Hurricane::Record* getRecord<const Hurricane::Transistor::Polarity::Code*>
(const Hurricane::Transistor::Polarity::Code* object) {
    Hurricane::Record* record = new Hurricane::Record(getString(object));
    record->add(getSlot("Code", (unsigned int*)object));
    return record;
}

template<> inline std::string getString<const Hurricane::Transistor::AbutmentType::Code*>
(const Hurricane::Transistor::AbutmentType::Code* object ) {
    switch (*object) {
        case Hurricane::Transistor::AbutmentType::INTERNAL: return "INTERNAL";
        case Hurricane::Transistor::AbutmentType::LEFT: return "LEFT";
        case Hurricane::Transistor::AbutmentType::RIGHT: return "RIGHT";
        case Hurricane::Transistor::AbutmentType::SINGLE: return "SINGLE";
        default: return "ABNORMAL";
    }
}

template<> inline Hurricane::Record* getRecord<const Hurricane::Transistor::AbutmentType::Code*>
(const Hurricane::Transistor::AbutmentType::Code* object) {
    Hurricane::Record* record = new Hurricane::Record(getString(object));
    record->add(getSlot("Code", (unsigned int*)object));
    return record;
}

INSPECTOR_P_SUPPORT(Hurricane::Transistor);
INSPECTOR_P_SUPPORT(Hurricane::Transistor::Polarity);
INSPECTOR_P_SUPPORT(Hurricane::Transistor::AbutmentType);

#endif // TRANSISTOR_H
