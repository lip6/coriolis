// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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


#pragma  once
#include <regex.h>
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
  void            restoreNetsDirection        ( Cell* , Cell::Flags );


// -------------------------------------------------------------------
// Class  :  "CRL::NamingScheme".

  class NamingScheme {
    public:
      enum Flag { NoFlags           = 0x0000
                , Recursive         = 0x0001
                , FromVerilog       = 0x0002
                , NoLowerCase       = 0x0004
                , UniquifyUpperCase = 0x0008
                };
    public:
      typedef std::function< Name(const Name&,uint32_t) >  converter_t;
    public:
      static Name  vhdlToVlog           ( const Name& vhdlName );
      static Name  vlogToVhdl           ( const Name& vlogName, uint32_t flags );
      static void  toVhdl               ( Cell* topCell, uint32_t flags );
                   NamingScheme         ( uint32_t flags );
      inline void  setNoLowerCase       ( bool state );
      inline void  setUniquifyUpperCase ( bool state );
             Name  convert              ( const Name& ) const;
    private:
      uint32_t     _flags;
      converter_t  _converter;
  };


  inline void  NamingScheme::setNoLowerCase ( bool state )
  {
    if (state) _flags |=  NoLowerCase;
    else       _flags &= ~NoLowerCase;
  }


  inline void  NamingScheme::setUniquifyUpperCase ( bool state )
  {
    if (state) _flags |=  UniquifyUpperCase;
    else       _flags &= ~UniquifyUpperCase;
  }


// -------------------------------------------------------------------
// Class  :  "CRL::SubNetNames".

  class  SubNetNames {
    public:
                           SubNetNames   ();
             bool          match         ( std::string );
      inline int32_t       getIndex      () const;
      inline std::string   getBase       () const;
      inline uint32_t      nextSubNet    ();
             std::string   getSubNetName () const;
    private:
      static bool         _compiled;
      static regex_t      _pattern;
             std::string  _base;
             int32_t      _index;
             uint32_t     _count;
  };


  inline int32_t      SubNetNames::getIndex   () const { return _index; }
  inline std::string  SubNetNames::getBase    () const { return _base; }
  inline uint32_t     SubNetNames::nextSubNet () { return ++_count; }


} // CRL namespace.
