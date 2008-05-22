#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "hurricane/Name.h"
#include "hurricane/Cell.h"
using namespace Hurricane;

namespace Hurricane {

class Transistor : public Cell {
    public:
        static const Name DrainName;
        static const Name SourceName;
        static const Name GridName;
        static const Name BulkName;
        enum Polarity {N=0, P=1};
        enum AbutmentType { INTERNAL=0, LEFT=1, RIGHT=2, SINGLE=3};

        static Transistor* create(Library* library, const Name& name, const Polarity& polarity);		
        void createLayout();

        bool isNmos() const { return _polarity == N; };
        bool isPmos() const { return _polarity == P; };
        bool isInternal() const { return _abutmentType == INTERNAL; };
        bool isLeft() const     { return _abutmentType == LEFT; };
        bool isRight() const    { return _abutmentType == RIGHT; };
        bool isSingle() const   { return _abutmentType == SINGLE; };
    protected:
        void _postCreate();

    private:
        Net* _drain;
        Net* _source;
        Net* _grid;
        Net* _bulk;
        Polarity _polarity;
        AbutmentType _abutmentType;
        double _l;
        double _w;
        Contact* _source20;
        Contact* _source22;
        Contact* _drain40;
        Contact* _drain42;

        Transistor(Library* library, const Name& name, const Polarity& polarity);
};

}

#endif // TRANSISTOR_H
