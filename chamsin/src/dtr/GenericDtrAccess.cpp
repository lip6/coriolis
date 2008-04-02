// ****************************************************************************************************
// File: GenericDtrAccess.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "DtrAccess.h"
#include "GenericDtrAccess.h"

namespace Hurricane {

// ****************************************************************************************************
// GenericDtrAccess implementation
// ****************************************************************************************************

GenericDtrAccess GenericDtrAccess::instance(NULL);

GenericDtrAccess::GenericDtrAccess(DtrAccess* dtraccess):_dtrAccess(dtraccess)
// **************************************************************************
{
}

void GenericDtrAccess::Instance(DtrAccess* dtraccess)
// **************************************************
{
   if(!(instance._dtrAccess))
      instance._dtrAccess = dtraccess;
}

string GenericDtrAccess::_getString() const
// ****************************************
{
  string s("Singleton GenericDtrAccess");
  return s;
  
}

}

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const Hurricane::GenericDtrAccess& access)
// ************************************************
{
  return access._getString();
}


