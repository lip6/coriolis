#ifndef METACAPACITOR_H
#define METACAPACITOR_H

#include "Device.h"

class Capacitor;

class MetaCapacitor : public Device {
    public:
        static const Name BottomPlateName;
        static const Name TopPlateName;
        static MetaCapacitor* create(Library* library, const Name& name);
        void setMatrixSize(unsigned rows, unsigned columns);

    protected:
        void _postCreate();

    private:
        MetaCapacitor(Library* library, const Name& name);

        typedef vector<Capacitor*> CapacitorVector;
        typedef vector<CapacitorVector> CapacitorMatrix;
        CapacitorMatrix _capacitorMatrix;
        unsigned _rows;
        unsigned _columns;
        Net* _bottomPlate;
        Net* _topPlate;
};

#endif // METACAPACITOR_H
