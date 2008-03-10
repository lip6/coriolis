// ****************************************************************************************************
// File: AnalogicalCommons.h
// Authors: Wu YiFei 
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_ANALOGICALCOMMONS
#define HURRICANE_ANALOGICALCOMMONS



// *********************************************************************
// Macros Declaration.
// *********************************************************************

#  define TRANSN 'N'
#  define TRANSP 'P'


#  define MAXNBCONTACT 8 

#  define IF_DEBUG_HUR_ANALOG \
   if(getenv("DEBUG_HUR_ANALOG")) {

#  ifndef END_IF
#  define END_IF \
   }
# endif


// *********************************************************************
// Analogical Unit declaration.
// *********************************************************************
#   if !defined(__DOXYGEN_PROCESSOR__)

typedef double Micro;
typedef double MicroPower2;
typedef long   Nano;

# endif

#include "TwoSpaces.h"


#endif // HURRICANE_ANALOGICALCOMMONS
