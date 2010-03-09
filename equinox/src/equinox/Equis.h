
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./equinox/Equis.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __EQUINOX_EQUIS__
#define  __EQUINOX_EQUIS__


namespace Hurricane {

  template<class Type> class GenericLocator;
  template<class Type> class GenericCollection;
  template<class Type> class GenericFilter;

}

namespace Equinox {
  
  
  using Hurricane::GenericCollection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericFilter;
  
  
  class Equi;


  typedef GenericCollection<Equi*> Equis;
  typedef GenericLocator<Equi*>    EquiLocator;
  typedef GenericFilter<Equi*>     EquiFilter;

  
} // End of Equinox namespace.



#endif //  __EQUINOX_EQUIS__
