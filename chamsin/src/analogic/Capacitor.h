#ifndef CAPACITOR_H
#define CAPACITOR_H

#include "AnalogComponent.h"

class Capacitor : public AnalogComponent {
    public:
        static const Name BottomPlateName;
        static const Name TopPlateName;

        static Capacitor* create(Library* library, const Name& name);
        void updateLayout();

//      void setType(Type type);
//      void setType(Type type); 
        
        void setW(DbU::Unit value) { _w = value; updateLayout(); }
        void setL(DbU::Unit value) { _l = value; updateLayout(); }

        virtual Record* _getRecord() const;
    
    protected:
        void _postCreate();

    private:
        Net* _bottomPlate;
        Net* _topPlate;
        Net* _anonymous;
        DbU::Unit _l;
        DbU::Unit _w;
        Pad* _topPlate10;
        Pad* _topPlate20;
        Pad* _topPlate30;
        Pad* _bottomPlate00;
        Pad *_anonymous01;
        Pad *_anonymous11;
        Pad *_anonymous12;
        Pad *_anonymous13;
        Pad *_anonymous21;
        Pad *_anonymous14;
       
        

        Capacitor(Library* library, const Name& name);
};


#endif // CAPACITOR_H
