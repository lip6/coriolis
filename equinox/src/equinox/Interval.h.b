
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
// |  C++ Header  :       "./equinox/Interval.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __EQUINOX_INTERVAL__
#define __EQUINOX_INTERVAL__

namespace Equinox {

  // -------------------------------------------------------------------
  // Class  :  "EQUINOX::Interval".
  
  
  class Interval {
     
  public: 
    /**/                              Interval           ();
    /**/   virtual                   ~Interval           ();
    /**/   virtual  long              GetLowPoint        () const = 0;
    /**/   virtual  long              GetHighPoint       () const = 0;
    /**/   virtual  void              Print              () const;

  };
  
  
  
} // end of namespace


#endif  //__EQUINOX_INTERVAL__
