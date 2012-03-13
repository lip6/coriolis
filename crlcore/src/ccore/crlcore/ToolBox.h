

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           Alliance / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/ToolBox.h"                      |
// +-----------------------------------------------------------------+


#ifndef  __CRL_TOOLBOX_H__
#define  __CRL_TOOLBOX_H__

#include  "hurricane/Cell.h"
#include  "hurricane/Net.h"
#include  "hurricane/Component.h"


namespace CRL {

  using Hurricane::Name;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Transformation;
  using Hurricane::Component;
  using Hurricane::Plug;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Library;
  using Hurricane::Occurrence;

  Component*      getBestExternalComponent    ( Net* );
  bool            placeNet                    ( Net* );
  void            placeNets                   ( Cell* );
  void            placeMasterNets             ( const Library* );
  void            placePlug                   ( Plug* );
  void            placePlugs                  ( Cell* );
  void            createPartRing2             ( Net& );
  void            createPartRing              ( Cell*, Name nom );
  void            createPlugsRing             ( Net& );
  void            createPlugsRing             ( Cell* );
  void            createPlugsAndPinsRing      ( Cell* );
  void            createContactsRing          ( Cell* );
  void            deleteEmptyNets             ( Cell* );
  Transformation  getTransformation           ( const Box& abox, const DbU::Unit& x, const DbU::Unit& y, const Transformation::Orientation& orientation );
  bool            isNoInstancePlacedOrFixed   ( Cell* );
  Occurrence      getRootNetOccurrence        ( const Occurrence& netoccurrence );
  void            connectPlugHooks            ( Cell* );
  size_t          getInstancesCount           ( const Cell* cell );
  void            setNetsPosition             ( Cell* );

} // End of CRL namespace.

#endif // __CRL_TOOLBOX_H__
