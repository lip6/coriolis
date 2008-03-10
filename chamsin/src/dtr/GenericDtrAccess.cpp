// ****************************************************************************************************
// File: GenericDtrAccess.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "DtrAccess.h"
#include "GenericDtrAccess.h"
//#include "DtrAccess.h"

BEGIN_NAMESPACE_HURRICANE


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

GenericDtrAccess::~GenericDtrAccess()
// *********************************
{
  if(_dtrAccess)  {  cout<<GetString(_dtrAccess)<<" is delete "<<endl; _dtrAccess->Delete();}
}


string GenericDtrAccess::_GetString() const
// ****************************************
{
  string s("Singleton GenericDtrAccess");
  return s;
  
}

END_NAMESPACE_HURRICANE



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string GetString(const H::GenericDtrAccess& access)
// ************************************************
{
  return access._GetString();
}


