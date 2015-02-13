// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
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

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Name;


// -------------------------------------------------------------------
// Class  :  "CRL::CellGauge".


  class CellGauge {

    public:
    // Constructors & Destructor.
      static  CellGauge*       create           ( const char*      name
                                                , const char*      pinLayerName
                                                , const DbU::Unit  pitch       =0
                                                , const DbU::Unit  sliceHeight =0
                                                , const DbU::Unit  sliceStep   =0 );
      virtual void             destroy          ();
    // Accessors
      inline  const Name&      getName          () const;
      inline  const Name&      getPinLayerName  () const;
      inline  const DbU::Unit  getPitch         () const;
      inline  const DbU::Unit  getSliceHeight   () const;
      inline  const DbU::Unit  getSliceStep     () const;
              CellGauge*       getClone         () const;
    // Hurricane management.
      virtual string           _getTypeName     () const;
      virtual string           _getString       () const;
      virtual Record*          _getRecord       () const;

    protected:
    // Internal: Attributes.
              Name             _name;
              Name             _pinLayerName;
              DbU::Unit        _pitch;
              DbU::Unit        _sliceHeight;
              DbU::Unit        _sliceStep;

    protected:
    // Internal: Constructors & Destructors.
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
  inline  const Name&      CellGauge::getName         () const { return _name; }
  inline  const Name&      CellGauge::getPinLayerName () const { return _pinLayerName; }
  inline  const DbU::Unit  CellGauge::getPitch        () const { return _pitch; }
  inline  const DbU::Unit  CellGauge::getSliceHeight  () const { return _sliceHeight; }
  inline  const DbU::Unit  CellGauge::getSliceStep    () const { return _sliceStep; }


}  // namespace CRL.


INSPECTOR_P_SUPPORT(CRL::CellGauge);


#endif  // CRL_CELL_GAUGE_H
