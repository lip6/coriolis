#ifndef METATRANSISTOR_H
#define METATRANSISTOR_H

#include "Device.h"

class Transistor;

class MetaTransistor : public Device {
    public:
        class Type {
            public:
                enum Code {UNDEFINED=0, NMOS=1, PMOS=2};

                Type(const Code& code = UNDEFINED);
                Type(const Type& type);

                Type& operator=(const Type& type);
                operator const Code&() const {return _code;};

                const Code& getCode() const {return _code;};

            private:
                Code _code;
        };

        typedef deque<Transistor*> Transistors;

        static const Name DrainName;
        static const Name SourceName;
        static const Name GridName;
        static const Name BulkName;
        static const Name AnonymousName;

        static MetaTransistor* create(Library* library, const Name& name);
        void updateLayout();

        void setType(Type type);
        void setM(unsigned m);
        void setW(DbU::Unit value);
        void setL(DbU::Unit value);

    protected:
        void _postCreate();

    private:
        Net* _drain;
        Net* _source;
        Net* _grid;
        Net* _bulk;
        Net* _anonymous;
        Type _type;
        unsigned _m;
        DbU::Unit _l;
        DbU::Unit _w;
        Transistors _transistors;

        MetaTransistor(Library* library, const Name& name);
};


#endif // METATRANSISTOR_H
