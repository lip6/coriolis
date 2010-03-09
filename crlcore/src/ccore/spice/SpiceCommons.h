// ****************************************************************************************************
// File: CSpiceCommons.h
// Authors: Wu YiFei 
// Date   : 25/11/2007 
// ****************************************************************************************************

#ifndef CSPICECOMMONS_H
#define CSPICECOMMONS_H


// *********************************************************************
// Macros Declaration.
// *********************************************************************

#  define IF_DEBUG_CSPICEPARSER \
   if(getenv("DEBUG_CSPICEPARSER")) {

#  ifndef END_IF
#  define END_IF \
   }
# endif

#endif // CSPICECOMMONS
