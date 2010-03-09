

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./CellGauge.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include <sstream>

#include "crlcore/CellGauge.h"




namespace {


  const char *missingMandatoryAttr =
    "CRoutingGauge::createFromXml () :\n\n"
    "    Can't found mandatory attribute \"%s\".\n";

  const char *badAttrValue =
    "CRoutingGauge::createFromXml () :\n\n"
    "    Invalid value \"%s\" for attribute \"%s\".\n";

// const char *missingCellGaugeTag =
//   "::createCellGaugeFromXml () :\n\n"
//   "    Can't found \"cellgauge\" tag.\n"
//   "    (XML file := \"%s\").\n";

// const char *missingCellGaugeNameAttr =
//   "::createCellGaugeFromXml () :\n\n"
//   "    Can't found cellgauge name tag attribute.\n";

// const char *emptyCellGaugeNameAttr =
//   "::createCellGaugeFromXml () :\n\n"
//   "    cellgauge name tag attribute is empty.\n";


}  // End of Anonymous namespace.




namespace CRL {


// -------------------------------------------------------------------
// Class  :  "CRL::CellGauge".


  CellGauge::CellGauge ( const char*      name
                       , const char*      pinLayerName
                       , const DbU::Unit  pitch
                       , const DbU::Unit  sliceHeight
                       , const DbU::Unit  sliceStep
                       ) : _name(name)
                         , _pinLayerName(pinLayerName)
                         , _pitch(pitch)
                         , _sliceHeight(sliceHeight)
                         , _sliceStep(sliceStep)
  { }


  CellGauge* CellGauge::create ( const char*      name
                               , const char*      pinLayerName
                               , const DbU::Unit  pitch
                               , const DbU::Unit  sliceHeight
                               , const DbU::Unit  sliceStep
                               )
  {
    if ( !name || (name[0] == '\0') )
      throw Error ( "CellGauge::create() : Empty name." );
    
    CellGauge* gauge = new CellGauge ( name, pinLayerName, pitch, sliceHeight, sliceStep );

    if ( gauge == NULL )
      throw Error ( BadCreate, "CRL::CellGauge::create()" );

    return gauge;
  }


  CellGauge::~CellGauge()
  { }


  void  CellGauge::destroy ()
  { delete this; }


  string  CellGauge::_getTypeName () const
  { return _TName("CellGauge"); }


  string  CellGauge::_getString () const
  {
    return ( "<" + _TName("CellGauge ")
           + getString(_name)
         //+ " pinLayerName=" + getString(_pinLayerName)
         //+ " pitch="  + getValueString(_pitch)
         //+ " height=" + getValueString(_sliceHeight)
         //+ " step="   + getValueString(_sliceStep)
           + ">" );
  }


  Record* CellGauge::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "Name"            , &_name        ) );
    record->add ( getSlot ( "PinLayerName"    , &_pinLayerName) );
    record->add ( getSlot ( "pitch"           ,  _pitch       ) );
    record->add ( getSlot ( "sliceHeight"     , _sliceHeight  ) );
    record->add ( getSlot ( "sliceStep"       , _sliceStep    ) );
    return ( record );
  }


} // End of CRL namespace.
