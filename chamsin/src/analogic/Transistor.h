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
        static const Name AnonymousName;
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
        Net* _anonymous;
        Polarity _polarity;
        AbutmentType _abutmentType;
        DbU::Unit _l;
        DbU::Unit _w;
        Pad *_source20, *_source22;
        Pad *_drain40, *_drain42;
        Pad *_grid00, *_grid01, *_grid30, *_grid31;
        Pad *_anonymous10, *_anonymous11, *_anonymous12;

        Transistor(Library* library, const Name& name, const Polarity& polarity);
};

}

#endif // TRANSISTOR_H
