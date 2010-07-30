// ****************************************************************************************************
// File: CSpiceDriver.h
// Authors: Wu YiFei
// Date   : 26/11/2006 
// ****************************************************************************************************

#ifndef CSPICEDRIVER_H
#define CSPICEDRIVER_H

#include  "hurricane/Cell.h"

namespace CRL {

using Hurricane::Cell;
using Hurricane::Net;
using std::map;
using std::ofstream;
using std::string;

class CSpiceDriver {
// ****************

// Types
// *****

// Attributes
// *******************
    private :  Cell * _cell; 
    private :  map<Net*, unsigned int> * _netToIndexMap;

// Constructors
// ************
    public : CSpiceDriver(Cell*);

// Destructors
// ***********
    public : ~CSpiceDriver();

// Accessors
// *********	  
    public : 

// Predicats
// *********
    public : 

// Updators
// ********	    
    public :

// Others
// ******
    public : void _DriveLogicalViewOfDevice(ofstream&);
    public : void _DumpSubckt(ofstream& );
    public : void _DumpNetDefinition(ofstream& );
    public : void _DumpInstanciation(ofstream& );
    public : void _DumpEnds(ofstream& );


// Operators
// *********
    public : void Drive(const string);


};


}  // END OF NAMESPACE CRL


#endif
