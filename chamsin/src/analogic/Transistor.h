#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "AnalogComponent.h"

class Transistor : public AnalogComponent {
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

        static const Name DrainName;
        static const Name SourceName;
        static const Name GridName;
        static const Name BulkName;
        static const Name AnonymousName;

        static Transistor* create(Library* library, const Name& name);
        void updateLayout();

        void setType(Type type);
        void setW(DbU::Unit value) { _w = value; updateLayout(); }
        void setL(DbU::Unit value) { _l = value; updateLayout(); }

        virtual Record* _getRecord() const;

    protected:
        void _postCreate();

    private:
        Net* _drain;
        Net* _source;
        Net* _grid;
        Net* _bulk;
        Net* _anonymous;
        Type _type;
        DbU::Unit _l;
        DbU::Unit _w;
        Pad *_source22;
        Pad *_drain42;
        Pad *_grid00, *_grid01, *_grid30, *_grid31;
        Pad *_anonymous10, *_anonymous11, *_anonymous12, *_anonymous50;

        Transistor(Library* library, const Name& name);
};

//INSPECTOR_P_SUPPORT(Transistor);

#endif // TRANSISTOR_H
