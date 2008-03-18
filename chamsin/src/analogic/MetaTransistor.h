// ****************************************************************************************************
// File: MetaTransistor.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_METATRANSISTOR
#define HURRICANE_METATRANSISTOR

#include "Cell.h"
#include "AnalogicalCommons.h"


//BEGIN_NAMESPACE_HURRICANE

namespace Hurricane {

class Library; 
class Name;
class Symbol;
class Record;

class MetaTransistor: public Cell {
// ********************************


// Types
// *****
     public : typedef Cell Inherit; 

#   if !defined(__DOXYGEN_PROCESSOR__)

// Logicals Attributes
// *******************
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


// Behaviorals attributes
// **********************	       
     private : double _temp, _vds, _vgs;  // DC 
     private : double _vg, _vd, _vs, _vb; 
     private : char _region; 
     private : double _ids; 
     private : double _vth, _vdsat;       // AC

# endif 

// Constructors
// ************
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : MetaTransistor(Library* library, const Name& name, char type);
# endif 

    public : static MetaTransistor* create(Library* library, const Name& name, char type);		
   
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : virtual void _postCreate();


// Destructors
// ***********
    protected : ~MetaTransistor() {};
    protected : virtual void _preDestroy();
# endif 

// Operations
// **********

    // Create the connection between all instances.
    // ********************************************
    public : void createConnection();
		
    // Create the layout of all motifs in this metatransistor. 
    // *******************************************************	     
    public : void createLayout();


#   if !defined(__DOXYGEN_PROCESSOR__)
    // get all paramters after generation of Layout (capa..). 
    // *****************************************************
    public : void getParameterOfGeneration() { /* to do */};

    // Delete all instances and all motifs in this metatransistor.
    // ***********************************************************	     
    public : void Flush();
#   endif


// Accessors
// *********	    
    public : const Micro&  getLe() const { return _le; };
    public : const Micro&  getWe()  const  { return _we; };
    public : const char  getType() const { return _type; };
    public : const unsigned  getM() const { return _m; };


// Updators
// ********
    public : void SetLe  (const Micro le)   { _le=le; };
    public : void SetWe  (const Micro we)  { _we=we; };
    public : void SetType(const char type)  { _type=type; };
    public : void SetM   (const unsigned m)  { _m=m; };


//#endif 

#   if !defined(__DOXYGEN_PROCESSOR__)
// Others
// ******
    public: virtual string _getTypeName() const {return _TName("MetaTransistor");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;
# endif

};


}

//END_NAMESPACE_HURRICANE

#endif // HURRICANE_METATRANSISTOR


