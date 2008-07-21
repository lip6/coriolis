#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "AnalogComponent.h"

class Capacitor : public AnalogComponent {
    public:
        static const Name BottomPlateName;
        static const Name TopPlateName;

        static Capacitor* create(Library* library, const Name& name);

    protected:
        void _postCreate();

    private:
        Net* _bottomPlate;
        Net* _topPlate;
        DbU::Unit _l;
        DbU::Unit _w;
        Pad *_topPlate20;

        Capacitor(Library* library, const Name& name);
};


#endif // CAPACITOR_H
