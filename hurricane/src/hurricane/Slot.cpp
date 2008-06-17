
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
//
// ===================================================================
//
// $Id: Slot.cpp,v 1.9 2007/07/29 15:25:00 jpc Exp $
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Slot.cpp"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/Commons.h"




namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::Slot".


  size_t  Slot::_allocateds = 0;


  size_t  Slot::getAllocateds ()
  {
    return _allocateds;
  }


  Slot::~Slot ()
  {
  //cerr << "Slot::~Slot() - " << _name << ": " << hex << (void*)this << dec << endl;
    _allocateds--;
  }


} // End of Hurricane namespace.
