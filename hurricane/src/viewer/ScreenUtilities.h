
// -*- C++ -*-


# ifndef  __SCREENUTILITIES_H__
#   define  __SCREENUTILITIES_H__

# include  <string>
# include  <QBrush>

# include  "Commons.h"


namespace Hurricane {


  class BasicLayer;


  // Functions.

  QBrush  getBrush ( const string& pattern, int red, int green, int blue );
  QBrush  getBrush ( const BasicLayer* layer );




} // End of Hurricane namespace.


# endif
