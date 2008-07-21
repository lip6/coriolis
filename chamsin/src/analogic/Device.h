#ifndef DEVICE_H
#define DEVICE_H

#include "AnalogComponent.h"

class Device : public AnalogComponent {
    public:
        static Device* create(Library* library, const Name& name);

    protected:
        void _postCreate();

    protected:

        Device(Library* library, const Name& name);
};


#endif // DEVICE_H
