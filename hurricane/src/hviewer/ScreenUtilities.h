
// -*- C++ -*-


# ifndef  __SCREENUTILITIES_H__
#   define  __SCREENUTILITIES_H__

# include  <string>
# include  <QBrush>

# include  "Commons.h"


namespace Hurricane {


  class BasicLayer;


  // Constants.
  const size_t  InvalidIndex = (size_t)-1;


  // Functions.

  QBrush  getBrush ( const string& pattern, int red, int green, int blue );




} // End of Hurricane namespace.


# endif
