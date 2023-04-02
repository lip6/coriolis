// ****************************************************************************************************
// File: ./Commons.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************


#include  <cxxabi.h>
#include  "hurricane/Commons.h"


tstream  cdebug ( std::cerr );


namespace Hurricane {

// -------------------------------------------------------------------
// Function  :  "demangle ()".


#define HAVE_CXA_DEMANGLE

#ifdef  HAVE_CXA_DEMANGLE

string  demangle ( const char* symbol )
{
  int          status;
  size_t       length = 4096;
  char         demangled[length];

  abi::__cxa_demangle ( symbol, demangled, &length, &status );

  return (status == 0) ? demangled : "";
}

#else

string  demangle ( const char* symbol )
{ return symbol; }

#endif


  string& split ( string& s )
  {
    size_t i = s.find( "<" );
    while ( i != string::npos ) {
      if (i+3 > s.size()) break;
    //if (s[i+2] != '>') {
        s.insert( i, "\\n" );
    //}
      i = s.find( "<", i+3 );
    }
    return s;
  }


} // End of Hurricane namespace.



// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

# if 0
bool Scan(const string& s, unsigned& u)
// ************************************
{
	unsigned v, n;
	sscanf(s.c_str(), "%u%n", &v, &n);
	if (n != s.size()) return false;
	u = v;
	return true;
}

bool Scan(const string& s, unsigned short& u)
// ******************************************
{
	unsigned short v;
	unsigned n;
	sscanf(s.c_str(), "%hu%n", &v, &n);
	if (n != s.size()) return false;
	u = v;
	return true;
}

bool Scan(const string& s, double& d)
// **********************************
{
	unsigned n;
	double v;
	sscanf(s.c_str(), "%lf%n", &v, &n);
	if (n != s.size()) return false;
	d = v;
	return true;
}

bool Scan(const string& s, int& i)
// *******************************
{
	int v;
	unsigned n;
	sscanf(s.c_str(), "%d%n", &v, &n);
	if (n != s.size()) return false;
	i = v;
	return true;
}


// -------------------------------------------------------------------
// Function  :  "Scan()".

  /*! \overload bool Scan ( const string &s, string &pattern )
   *  \param   s       string to interpret.
   *  \param   pattern a stipple pattern for a layer.
   *  \return     true if the conversion was correct.
   *
   *  Look for a stipple patern. Checks the string length (must be
   *  equal to 16) and only made of hexadecimal characters.
   */

bool Scan ( const string &s, string &pattern )
{
  if ( s.size() != 16 ) return ( false );

  string  validChars = "0123456789ABCDEFabcdef";
  for ( unsigned i=0 ; i<16 ; i++ ) {
    if ( validChars.find(s[i]) == string::npos ) return ( false );
  }
  pattern = s;

  return ( true );
}


// -------------------------------------------------------------------
// Function  :  "Scan()".

  /*! \overload bool Scan ( const string &s, unsigned short &redValue, unsigned short &greenValue, unsigned short &blueValue )
   *  \param   s          string to interpret.
   *  \param   redValue   the red color's component.
   *  \param   greenValue the green color's component.
   *  \param   blueValue  the blue color's component.
   *  \return             true if the conversion was correct.
   *
   *  Split a string of the form \c "RRR,GGG,BBB" into its numerical
   *  components (between 0 and 255).
   */

bool Scan ( const string         &s
          ,       unsigned short &redValue
          ,       unsigned short &greenValue
          ,       unsigned short &blueValue )
{
  unsigned       n;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
  
  sscanf ( s.c_str(), "%hu,%hu,%hu%n", &red, &green, &blue, &n );
  if ( n != s.size() ) return ( false );

  redValue   = red;
  greenValue = green;
  blueValue  = blue;

  return ( true );
}
# endif


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
