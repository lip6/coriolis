#ifndef METATRANSISTOR_H
#define METATRANSISTOR_H

#include "AnalogComponent.h"

class MetaTransistor: public AnalogComponent {
     private : char _type;
     private : unsigned _m;              
     private : Micro _le, _we;           // length and width expected
     private : Micro _lr, _wr;           // real length and real width
     private : unsigned _nSex, _nDex, nSin, _nDin, _nSsh, _nDsh;
     private : Micro _dgg, _de;
     private : MicroPower2 _as, _ad;
     private : Micro _ps, _pd;
     private : double _capaDrain, _capaGate, _capaSource;
     private : double _cgb, _cgs, _cdb, _cds, _csb, _cgd;


    protected : MetaTransistor(Library* library, const Name& name, char type);

    public : static MetaTransistor* create(Library* library, const Name& name, char type);		
   
    protected : virtual void _postCreate();


    // ********************************************
    public : void createConnection();
		
    // Create the layout of all motifs in this metatransistor. 
    // *******************************************************	     
    public : void createLayout();


// Accessors
// *********	    
    public : const Micro&  getLe() const { return _le; };
    public : const Micro&  getWe()  const  { return _we; };
    public : const char  getType() const { return _type; };
    public : const unsigned  getM() const { return _m; };


// Updators
// ********
    public : void setLe  (const Micro le)   { _le=le; };
    public : void setWe  (const Micro we)  { _we=we; };
    public : void setType(const char type)  { _type=type; };
    public : void setM   (const unsigned m)  { _m=m; };


};


}

#endif // HURRICANE_METATRANSISTOR
