// ****************************************************************************************************
// File: Commons.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************


#ifdef  HAVE_LIBIBERTY
#include "hurricane/demangle.h"
#include "hurricane/libiberty.h"
#endif

#include "hurricane/Commons.h"


namespace Hurricane {



// ****************************************************************************************************
// Tracing tools
// ****************************************************************************************************

static long         TRACE_LEVEL      = 0;
static unsigned int TRACE_SHOW_LEVEL = (unsigned int)-1;

bool in_trace()
// ************
{
	return (0 < TRACE_LEVEL);
}

void trace_on()
// ************
{
	TRACE_LEVEL++;
}

void trace_off()
// *************
{
	if (0 < TRACE_LEVEL) TRACE_LEVEL--;
}

void trace_in()
// ************
{
	if (0 < TRACE_LEVEL) tab++;
}

void trace_out()
// *************
{
	if (0 < TRACE_LEVEL) tab--;
}

bool  inltrace ( unsigned int level )
//***********************************
{
  return TRACE_SHOW_LEVEL <= level;
}

unsigned int  ltracelevel ( unsigned int level )
//**********************************************
{
  unsigned int  previousLevel = TRACE_SHOW_LEVEL;
  TRACE_SHOW_LEVEL = level;

  return previousLevel;
}

void  ltracein (unsigned int level, unsigned int count )
//******************************************************
{
  if ( inltrace(level) ) while ( count-- ) tab++;
}

void  ltraceout (unsigned int level, unsigned int count )
//*******************************************************
{
  if ( inltrace(level) ) while ( count-- ) tab--;
}

// -------------------------------------------------------------------
// Function  :  "Demangle ()".

string  demangle ( const char* symbol )
{
  string  mangled   = "_Z";
  mangled += symbol;
  
# ifdef  HAVE_LIBIBERTY
  char* result = cplus_demangle ( mangled.c_str(), DMGL_PARAMS|DMGL_ANSI|DMGL_TYPES );

  if ( result ) {
    mangled = result;
    free ( result );
    return mangled;
  }
# endif

  return mangled;
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
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
