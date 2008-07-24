#ifndef METACAPACITOR_H
#define METACAPACITOR_H

#include "Device.h"

class Capacitor;

class MetaCapacitor : public Device {
    public:
        static MetaCapacitor* create(Library* library, const Name& name);

    protected:
        void _postCreate();

    private:
        MetaCapacitor(Library* library, const Name& name);
};


#endif // METACAPACITOR_H
