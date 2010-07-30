// ****************************************************************************************************
// File: Spice.h
// Authors: Wu YiFei
// Date   : 26/11/2006 
// ****************************************************************************************************

# ifndef  __SPICE_H__
# define  __SPICE_H__

#include <string>

namespace Hurricane {
  class Cell;
}

namespace CRL {

using Hurricane::Cell;
using std::string;

// ------------------------------------------------------------------
// functions.


// This function create the logical view for cell, if the 
// file spice is correct in the level of syntax. else
// this function print those errors.
// Please note that this function is recursive. 
// That means this function create the logical view of
// those cells who's syntax is correct in the hierarchy.
// ******************************************************
  void  spiceParser ( const string cellPath, Cell* cell );

  void  spiceDriver ( const string cellPath, Cell* cell, unsigned int& saveState);

}

# endif
