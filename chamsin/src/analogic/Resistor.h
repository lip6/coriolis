#ifndef RESISTOR_H
#define RESISTOR_H

#include "AnalogComponent.h"

class Resistor : public AnalogComponent {
    public:
        static Resistor* create(Library* library, const Name& name);

    protected:
        void _postCreate();

    private:

        Resistor(Library* library, const Name& name);
};


#endif // RESISTOR_H
