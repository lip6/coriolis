// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/CellGauge.h"                         |
// +-----------------------------------------------------------------+


#ifndef  CRL_CELL_GAUGE_H
#define  CRL_CELL_GAUGE_H

#include <string>
#include <map>
#include "hurricane/Commons.h"
#include "hurricane/DbU.h"
#include "hurricane/Name.h"
#include "crlcore/Utilities.h"


namespace CRL {

  using std::string;
  using std::map;
  using Hurricane::JsonObject;
  using Hurricane::JsonStack;
  using Hurricane::Initializer;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Name;


// -------------------------------------------------------------------
// Class  :  "CRL::CellGauge".

  class CellGauge {
    public:
      enum Flags { NoFlags = 0, Pad = ( 1<<0) };
    public:
      static  CellGauge*       create           ( const char*      name
                                                , const char*      pinLayerName
                                                , const DbU::Unit  pitch       =0
                                                , const DbU::Unit  sliceHeight =0
                                                , const DbU::Unit  sliceStep   =0 );
      virtual void             destroy          ();
      inline  bool             isPad            () const;
      inline  const Name&      getName          () const;
      inline  unsigned long    getFlags         () const;
      inline  const Name&      getPinLayerName  () const;
      inline  const DbU::Unit  getPitch         () const;
      inline  const DbU::Unit  getSliceHeight   () const;
      inline  const DbU::Unit  getSliceStep     () const;
              CellGauge*       getClone         () const;
      inline  void             setFlags         ( unsigned long flags );
      inline  void             resetFlags       ( unsigned long flags );
      inline  void             setPitch         ( DbU::Unit );
      inline  void             setSliceHeight   ( DbU::Unit );
      inline  void             setSliceStep     ( DbU::Unit );
              void             toJson           ( JsonWriter* ) const;
      virtual string           _getTypeName     () const;
      virtual string           _getString       () const;
      virtual Record*          _getRecord       () const;
    protected:
              Name             _name;
              Name             _pinLayerName;
              DbU::Unit        _pitch;
              DbU::Unit        _sliceHeight;
              DbU::Unit        _sliceStep;
              unsigned long    _flags;
    protected:
                               CellGauge        ( const char*      name
                                                , const char*      pinLayerName
                                                , const DbU::Unit  pitch
                                                , const DbU::Unit  sliceHeight
                                                , const DbU::Unit  sliceStep );
    protected:
      virtual                 ~CellGauge        ();
    private:
                               CellGauge        ( const CellGauge& );
              CellGauge&       operator=        ( const CellGauge& );
  };


// New Types.
  typedef  map<Name,CellGauge*>  CellGaugeMap;


// Inline Functions.
  inline        bool           CellGauge::isPad           () const { return _flags & Flags::Pad; }
  inline  const Name&          CellGauge::getName         () const { return _name; }
  inline  const Name&          CellGauge::getPinLayerName () const { return _pinLayerName; }
  inline  const DbU::Unit      CellGauge::getPitch        () const { return _pitch; }
  inline  const DbU::Unit      CellGauge::getSliceHeight  () const { return _sliceHeight; }
  inline  const DbU::Unit      CellGauge::getSliceStep    () const { return _sliceStep; }
  inline        unsigned long  CellGauge::getFlags        () const { return _flags; }
  inline        void           CellGauge::setFlags        ( unsigned long flags ) { _flags |=  flags; }
  inline        void           CellGauge::resetFlags      ( unsigned long flags ) { _flags &= ~flags; }
  inline        void           CellGauge::setPitch        ( DbU::Unit pitch  ) { _pitch       = pitch; } 
  inline        void           CellGauge::setSliceHeight  ( DbU::Unit height ) { _sliceHeight = height; }
  inline        void           CellGauge::setSliceStep    ( DbU::Unit step   ) { _sliceStep   = step; }


// -------------------------------------------------------------------
// Class  :  "JsonCellGauge".

  class JsonCellGauge : public JsonObject {
    public:
      static  void           initialize    ();
                             JsonCellGauge ( unsigned long flags );
      virtual string         getTypeName   () const;
      virtual JsonCellGauge* clone         ( unsigned long flags ) const;
      virtual void           toData        ( JsonStack& );
  };


}  // namespace CRL.


INSPECTOR_P_SUPPORT(CRL::CellGauge);


#endif  // CRL_CELL_GAUGE_H
