#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "AnalogComponent.h"

class Capacitor : public AnalogComponent {
    public:
        static Capacitor* create(Library* library, const Name& name);

    protected:
        void _postCreate();

    private:

        Capacitor(Library* library, const Name& name);
};


#endif // CAPACITOR_H
