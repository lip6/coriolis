// -*- mode: C++; explicit-buffer-name: "Configuration.h<etesian>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/Configuration.h"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}
#include "crlcore/RoutingGauge.h"
#include "crlcore/CellGauge.h"


namespace Etesian {

  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;
  using  CRL::CellGauge;
  using  CRL::RoutingGauge;


// -------------------------------------------------------------------
// Class  :  "Etesian::Configuration".

  enum Effort        { Fast    =1
                     , Standard=3
                     , High    =6
                     , Extreme =9
                     };
  enum GraphicUpdate { UpdateAll =1 
                     , LowerBound=2 
                     , FinalOnly =3 
                     };

  class Configuration {
    public:
    // Constructor & Destructor.
                              Configuration             ( const RoutingGauge* rg=NULL, const CellGauge* cg=NULL );
                             ~Configuration             ();
             Configuration*   clone                     () const;
    // Methods.                                         
      inline RoutingGauge*    getGauge                  () const;
      inline CellGauge*       getCellGauge              () const;
      inline Effort           getPlaceEffort            () const;
      inline GraphicUpdate    getUpdateConf             () const;
      inline bool             getRoutingDriven          () const;
      inline double           getSpaceMargin            () const;
      inline double           getDensityVariation       () const;
      inline double           getAspectRatio            () const;
      inline string           getTieName                () const;
      inline string           getFeedNames              () const;
      inline string           getDiodeName              () const;
      inline string           getSpareBufferName        () const;
      inline string           getBloat                  () const;
      inline DbU::Unit        getLatchUpDistance        () const;
      inline DbU::Unit        getAntennaGateMaxWL       () const;
      inline DbU::Unit        getAntennaDiodeMaxWL      () const;
      inline void             setSpaceMargin            ( double );
      inline void             setDensityVariation       ( double );
      inline void             setAspectRatio            ( double );
             void             print                     ( Cell* ) const;
             Record*          _getRecord                () const;
             string           _getString                () const;
             string           _getTypeName              () const;
    protected:
    // Attributes.
      RoutingGauge*  _rg;
      CellGauge*     _cg;
      Effort         _placeEffort;
      GraphicUpdate  _updateConf;
      bool           _routingDriven;
      double         _spaceMargin;
      double         _densityVariation;
      double         _aspectRatio;
      string         _tieName;
      string         _feedNames;
      string         _diodeName;
      string         _spareBufferName;
      string         _bloat;
      DbU::Unit      _latchUpDistance;
      DbU::Unit      _antennaGateMaxWL;
      DbU::Unit      _antennaDiodeMaxWL;
    private:
                             Configuration ( const Configuration& );
      Configuration& operator=             ( const Configuration& );
  };


  inline RoutingGauge* Configuration::getGauge                  () const { return _rg; }
  inline CellGauge*    Configuration::getCellGauge              () const { return _cg; }
  inline Effort        Configuration::getPlaceEffort            () const { return _placeEffort; }
  inline GraphicUpdate Configuration::getUpdateConf             () const { return _updateConf; }
  inline bool          Configuration::getRoutingDriven          () const { return _routingDriven; }
  inline double        Configuration::getSpaceMargin            () const { return _spaceMargin; }
  inline double        Configuration::getDensityVariation       () const { return _densityVariation; }
  inline double        Configuration::getAspectRatio            () const { return _aspectRatio; }
  inline string        Configuration::getTieName                () const { return _tieName; }
  inline string        Configuration::getFeedNames              () const { return _feedNames; }
  inline string        Configuration::getDiodeName              () const { return _diodeName; }
  inline string        Configuration::getSpareBufferName        () const { return _spareBufferName; }
  inline string        Configuration::getBloat                  () const { return _bloat; }
  inline DbU::Unit     Configuration::getLatchUpDistance        () const { return _latchUpDistance; }
  inline DbU::Unit     Configuration::getAntennaGateMaxWL       () const { return _antennaGateMaxWL; }
  inline DbU::Unit     Configuration::getAntennaDiodeMaxWL      () const { return _antennaDiodeMaxWL; }
  inline void          Configuration::setSpaceMargin            ( double margin ) { _spaceMargin = margin; }
  inline void          Configuration::setDensityVariation       ( double margin ) { _densityVariation = margin; }
  inline void          Configuration::setAspectRatio            ( double ratio  ) { _aspectRatio = ratio; }


} // Etesian namespace.

INSPECTOR_P_SUPPORT(Etesian::Configuration);
