// ****************************************************************************************************
// File: GenTrans.h
// Authors: Wu YiFei
// Date   : 04/04/2007 
// ****************************************************************************************************


#ifndef HURRICANE_GENTRANS
#define HURRICANE_GENTRANS


#include "Transistor.h"
#include "Box.h"


namespace Hurricane {

class GenTrans {
// *************

// Attributes
// **********
  protected : map<string, Box> _mapString2Box;


// Constructors
// *************
  public : GenTrans() {};

// Destructors
// ***********
  public : virtual ~GenTrans() {};

// Operators
// **********
  public : virtual void Calculate(Transistor*) = 0; 
  public : virtual void Generate(Transistor*) = 0;

};


class GenV1Trans : public GenTrans {
    public:
        typedef GenTrans Inherit;

        const Transistor::MaskV1Info* _masqueV1Info;
        static const int maxNbContacts = 8;

        GenV1Trans(Transistor::MaskV1Info*);

        virtual ~GenV1Trans() {};

        virtual void Calculate(Transistor*) ;
        virtual void Generate(Transistor*) ;
  
};

}

#endif
