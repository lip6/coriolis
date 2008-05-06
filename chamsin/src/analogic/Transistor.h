#ifndef TRANSISTOR_H
#define TRANSISTOR_H

#include "Cell.h"
using namespace Hurricane;

namespace Hurricane {

class Transistor : public Cell {
    public:
        enum Polarity {N=0, P=1};
        enum AbutmentType { INTERNAL=0, LEFT=1, RIGHT=2, SINGLE=3};

        static Transistor* create(Library* library, const Name& name, const Polarity& polarity);		
        void createLayout();

        bool isNmos() const { return polarity == N; };
        bool isPmos() const { return polarity == P; };
        bool isInternal() const { return abutmentType == INTERNAL; };
        bool isLeft() const     { return abutmentType == LEFT; };
        bool isRight() const    { return abutmentType == RIGHT; };
        bool isSingle() const   { return abutmentType == SINGLE; };

    private:
        Polarity polarity;
        AbutmentType abutmentType;
        double l;
        double w;
};

}

#endif // TRANSISTOR_H
