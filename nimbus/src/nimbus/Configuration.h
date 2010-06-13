
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |     N i m b u s  -  Partionnement Infrastructure                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __NIMBUS_CONFIGURATION__
#define  __NIMBUS_CONFIGURATION__

#include  <string>
#include  <vector>
#include  "hurricane/Box.h"
#include  "crlcore/CellGauge.h"
#include  "crlcore/RoutingGauge.h"

namespace Hurricane {
  class Record;
  class Cell;
}

namespace CRL {
  class RoutingLayerGauge;
}


namespace Nimbus {

  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Cell;
  using Hurricane::Record;
  using CRL::CellGauge;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Nimbus::Configuration".


  class Configuration {
    public:
      static  Configuration*      getDefault                    ();
    public:                    
    // Constructor & Destruct  or.
                                  Configuration                 ( CellGauge*, RoutingGauge* );
                                 ~Configuration                 ();
              Configuration*      clone                         () const;
    // Methods.
      inline  const CellGauge*    getCellGauge                  () const;
      inline  const RoutingGauge* getRoutingGauge               () const;
      inline  DbU::Unit           getPitch                      () const;
      inline  DbU::Unit           getSliceHeight                () const;
      inline  DbU::Unit           getSliceStep                  () const;
      inline  const std::vector<RoutingLayerGauge*>&     
                                  getLayerGauges                () const;
      inline  bool                doPinsPlacement               () const;
      inline  double              getAspectRatio                () const;
      inline  double              getMargin                     () const;
      inline  const Box&          getWorkZone                   () const;
              void                print                         ( Cell* ) const;
      inline  void                setPinsPlacement              ( bool );
      inline  void                setAspectRatio                ( double );
      inline  void                setMargin                     ( double );
      inline  void                setWorkZone                   ( const Box& );
      inline  static double       _normPercentage               ( double ratio, double min=0.0, double max=1.0 );
              Record*             _getRecord                    () const;
              std::string         _getString                    () const;
              std::string         _getTypeName                  () const;
    private:                   
    // Attributes.             
      static Configuration* _default;
             CellGauge*     _cellGauge;
             RoutingGauge*  _routingGauge;
             bool           _pinsPlacement;
             double         _aspectRatio;
             double         _margin;
             Box            _workZone;
    private:                   
                     Configuration ( const Configuration& );
      Configuration& operator=     ( const Configuration& );
  };


// Inline Methods.
  inline  DbU::Unit   Configuration::getPitch         () const { return _cellGauge->getPitch(); }
  inline  DbU::Unit   Configuration::getSliceHeight   () const { return _cellGauge->getSliceHeight(); }
  inline  DbU::Unit   Configuration::getSliceStep     () const { return _cellGauge->getSliceStep(); }
  inline  const std::vector<RoutingLayerGauge*>&     
                      Configuration::getLayerGauges   () const { return _routingGauge->getLayerGauges(); }
  inline  bool        Configuration::doPinsPlacement  () const { return _pinsPlacement; }
  inline  double      Configuration::getAspectRatio   () const { return _aspectRatio; }
  inline  double      Configuration::getMargin        () const { return _margin; }
  inline  const Box&  Configuration::getWorkZone      () const { return _workZone; }
  inline  void        Configuration::setPinsPlacement ( bool state ) { _pinsPlacement=state; }
  inline  void        Configuration::setAspectRatio   ( double ratio ) { _aspectRatio=_normPercentage(ratio,0.1,10.0); }
  inline  void        Configuration::setMargin        ( double margin ) { _margin=_normPercentage(margin,0.0,10.0); }
  inline  void        Configuration::setWorkZone      ( const Box& workZone ) { _workZone=workZone; } 

  inline double  Configuration::_normPercentage ( double ratio, double min, double max ) {
    if ( ratio < min ) return min;
    if ( ratio > max ) return max;
    return ratio;
  }


}  // End of Nimbus namespace.


INSPECTOR_P_SUPPORT(Nimbus::Configuration);


#endif  // __NIMBUS_CONFIGURATION__
