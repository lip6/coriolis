// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
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


#ifndef  CRL_TOOLBOX_H
#define  CRL_TOOLBOX_H

#include <functional>
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Component.h"


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


  class NamingScheme {
    public:
      enum Flag { NoFlags     = 0x0000
                , Recursive   = 0x0001
                , FromVerilog = 0x0002
                };
    public:
      typedef std::function< Name(const Name&) >  converter_t;
    public:
      static Name  vlogToVhdl ( const Name& vlogName );
      static void  toVhdl     ( Cell* topCell, unsigned int flags );
  };


} // CRL namespace.

#endif // CRL_TOOLBOX_H
