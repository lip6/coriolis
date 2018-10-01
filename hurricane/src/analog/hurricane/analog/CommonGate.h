#ifndef __COMMON_GATE_H
#define __COMMON_GATE__PAIR_H

#include "hurricane/DbU.h"

#include "hurricaneAMS/devices/TransistorFamily.h"

namespace Hurricane {
    class Library;
    class Name;
}

class MetaTransistor;
class CommonGate : public TransistorFamily {

    public:
        typedef TransistorFamily Inherit;

        static CommonGate*
        create(Hurricane::Library* library, const Hurricane::Name& name, const Type& type, bool bulkConnected);
        Hurricane::DbU::Unit getW() const;
        Hurricane::DbU::Unit getL() const;

        virtual Hurricane::Name getDeviceName() const;
        virtual Arguments* getArguments();

        void setExternalDummy(const std::string& externalDummy );
        std::string getExternalDummy() const;


    protected:
        CommonGate(Hurricane::Library* library, const Hurricane::Name& name, const Type& type);
        virtual void _postCreate(const Hurricane::Name& deviceName);
        virtual void createConnections(bool bulkConnected);

        static const Hurricane::Name       _cgBulkConnectedName;
        static const Hurricane::Name       _cgBulkUnconnectedName;
        MetaTransistor*         _m1;
        MetaTransistor*         _m2;
        StepParameter*          _w;
        StepParameter*          _l;
        FormFactorParameter*    _m;
        SpinBoxParameter*       _mInt;
        SpinBoxParameter*       _externalDummy;

        MCheckBoxParameter*     _sourceFirst;
        MCheckBoxParameter*     _bulkType;

};

#endif /* __COMMON_GATE__PAIR_H */
