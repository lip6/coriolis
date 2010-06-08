
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
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __MAUKA_CONFIGURATION__
#define  __MAUKA_CONFIGURATION__

#include  <string>

namespace Hurricane {
  class Record;
  class Cell;
}


namespace Mauka {

  using Hurricane::Cell;
  using Hurricane::Record;


// -------------------------------------------------------------------
// Class  :  "Katabatic::Configuration".


  class Configuration {
    public:
      static  Configuration* getDefault                    ();
    public:
    // Constructor & Destructor.
                             Configuration                 ();
                            ~Configuration                 ();
              Configuration* clone                         () const;
    // Methods.
      inline  bool           useStandardSimulatedAnnealing () const;
      inline  bool           doIgnorePins                  () const;
      inline  bool           doPlotBins                    () const;
      inline  double         getSearchRatio                () const;
      inline  double         getAnnealingNetMult           () const;
      inline  double         getAnnealingBinMult           () const;
      inline  double         getAnnealingRowMult           () const;
              void           print                         ( Cell* ) const;
      inline  void           setStandardSimulatedAnnealing ( bool );
      inline  void           setIgnorePins                 ( bool );
      inline  void           setPlotBins                   ( bool );
      inline  void           setSearchRatio                ( double );
      inline  void           setAnnealingNetMult           ( double );
      inline  void           setAnnealingBinMult           ( double );
      inline  void           setAnnealingRowMult           ( double );
      inline  static double  _normPercentage               ( double ratio, double min=0.0, double max=1.0 );
              Record*        _getRecord                    () const;
              std::string    _getString                    () const;
              std::string    _getTypeName                  () const;
    private:
    // Attributes.
      static Configuration* _default;
             bool           _standardSimulatedAnnealing;
             bool           _ignorePins;
             bool           _plotBins;                   // Plot bins utilisation.
             double         _searchRatio;
             double         _annealingNetMult;
             double         _annealingBinMult;
             double         _annealingRowMult;
    private:
                             Configuration ( const Configuration& );
      Configuration& operator=             ( const Configuration& );
  };


// Inline Methods.
  inline  bool    Configuration::useStandardSimulatedAnnealing () const { return _standardSimulatedAnnealing; }
  inline  bool    Configuration::doIgnorePins                  () const { return _ignorePins; }
  inline  bool    Configuration::doPlotBins                    () const { return _plotBins; }
  inline  double  Configuration::getSearchRatio                () const { return _searchRatio; }
  inline  double  Configuration::getAnnealingNetMult           () const { return _annealingNetMult; } 
  inline  double  Configuration::getAnnealingBinMult           () const { return _annealingBinMult; }
  inline  double  Configuration::getAnnealingRowMult           () const { return _annealingRowMult; }
  inline  void    Configuration::setStandardSimulatedAnnealing ( bool state ) { _standardSimulatedAnnealing=state; }
  inline  void    Configuration::setIgnorePins                 ( bool state ) { _ignorePins=state; }
  inline  void    Configuration::setPlotBins                   ( bool state ) { _plotBins=state; }
  inline  void    Configuration::setSearchRatio                ( double ratio ) { _searchRatio=_normPercentage(ratio,0.1); }
  inline  void    Configuration::setAnnealingNetMult           ( double mult ) { _annealingNetMult=_normPercentage(mult); } 
  inline  void    Configuration::setAnnealingBinMult           ( double mult ) { _annealingBinMult=_normPercentage(mult); }
  inline  void    Configuration::setAnnealingRowMult           ( double mult ) { _annealingRowMult=_normPercentage(mult); }

  inline double  Configuration::_normPercentage ( double ratio, double min, double max ) {
    if ( ratio < min ) return min;
    if ( ratio > max ) return max;
    return ratio;
  }


}  // End of Mauka namespace.


INSPECTOR_P_SUPPORT(Mauka::Configuration);


#endif  // __MAUKA_CONFIGURATION__
