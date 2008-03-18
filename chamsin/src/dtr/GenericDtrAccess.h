// ****************************************************************************************************
// File: GenericDtrAccess.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#ifndef HURRICANE_GENERICDTRACCESS 
#define HURRICANE_GENERICDTRACCESS


//BEGIN_NAMESPACE_HURRICANE

namespace Hurricane {

class DtrAccess;

class GenericDtrAccess {
// *********************

// Types
// *****

#   if !defined(__DOXYGEN_PROCESSOR__)
// Attributes
// **********
    private : static GenericDtrAccess instance;

    private : DtrAccess * _dtrAccess;

 
// Constructors
// ************
    protected : GenericDtrAccess(DtrAccess*);
    private : GenericDtrAccess(const GenericDtrAccess&);		
    private : GenericDtrAccess& operator=(const GenericDtrAccess&);
      
# endif 

    public : static void Instance(DtrAccess*);

// Destructors
// ***********
    protected : virtual ~GenericDtrAccess();

// Accessors
// *********	    


// Updators
// ********

// Operations
// **********	     

#   if !defined(__DOXYGEN_PROCESSOR__)
// Others
// ******
    public: virtual string _getTypeName() const {return _TName("GenericDtrAccess");};
    public: virtual string _getString() const;
# endif

};


//END_NAMESPACE_HURRICANE

}

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const H::GenericDtrAccess&);


#endif // HURRICANE_GENERICDTRACCESS
