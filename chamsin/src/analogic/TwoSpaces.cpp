// **************************************************************
// TwoSpaces.cpp
// Author : Wu YiFei
// Date  : 12/04/2007 
// ***************************************************************

#include "TwoSpaces.h"

namespace Hurricane {


// ****************************************************************************************************
// Two Spaces definition.
// ****************************************************************************************************
TwoSpaces::TwoSpaces(const string& s)
// *********************************
:  _s(s)
  ,_n(1)
{
}


TwoSpaces::TwoSpaces(const TwoSpaces& ts)
// **************************************
: _s(ts._s)
  ,_n(ts._n)
{
}
  

TwoSpaces& TwoSpaces::operator=(const TwoSpaces& ts)
// ************************************************
{  
   _s = ts._s;
   _n = ts._n;
   return *this;
}


TwoSpaces& TwoSpaces::operator++()
// *******************************
{
   if(_n<100) _n++;
   return *this; 
}  

TwoSpaces TwoSpaces::operator++(int)
// *********************************
{
   TwoSpaces  ts = *this;
   if(_n<100) _n++;
   return ts;
}   


TwoSpaces& TwoSpaces::operator--()
// *******************************
{ 
   if(_n>1) _n--;
   return *this;
}


TwoSpaces TwoSpaces::operator--(int)
// *********************************
{
   TwoSpaces ts = *this;
   if(_n>1) _n--;
   return ts;
}  


TwoSpaces TwoSpaces::operator+(int count)
// **************************************
{ 
   TwoSpaces ts = *this;
   if( (_n+count) <= 100 )  ts._n = _n + count;
   return ts;
}


TwoSpaces TwoSpaces::operator-(int count)
// **************************************
{
  TwoSpaces ts = *this;
  if ( (_n - count) >= 1 ) ts._n = _n - count;
  return ts;
}  


string TwoSpaces::_GetString() const
// *********************************
{
  string s;
  unsigned n = _n;

  while(n--) {
    s += _s;
  }

  return s;
}



// ****************************************************************************************************
// Variables definition.
// ****************************************************************************************************
TwoSpaces ts("  ");

}


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************
string GetString(const Hurricane::TwoSpaces& ts)
// ***********************************
{
  return ts._GetString();
}  

