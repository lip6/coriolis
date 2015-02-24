// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/EtesianEngine.h"                     |
// +-----------------------------------------------------------------+


#ifndef  ETESIAN_ETESIAN_ENGINE_H
#define  ETESIAN_ETESIAN_ENGINE_H

#include <iostream>
#include <unordered_map>
#include "coloquinte/circuit.hxx"

#include "hurricane/Timer.h"
#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellWidget;
  class Instance;
}

#include "crlcore/ToolEngine.h"
#include "etesian/Configuration.h"
#include "etesian/FeedCells.h"


namespace Etesian {

  using Hurricane::Timer;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Record;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Etesian::EtesianEngine".

  class EtesianEngine : public CRL::ToolEngine {
    public:
      enum Flag { NoPlacement=0x0001
                , FlatDesign =0x0002
                , ForceUpdate=0x0004
                , YSpinSet   =0x0008
                , SlowMotion =0x0010
                };
    public:
      static  const Name&          staticGetName    ();
      static  EtesianEngine*       create           ( Cell* );
      static  EtesianEngine*       get              ( const Cell* );
    public:                                         
      inline  bool                 isSlowMotion     () const;
      virtual Configuration*       getConfiguration ();
      virtual const Configuration* getConfiguration () const;
      virtual const Name&          getName          () const;
      inline  CellGauge*           getCellGauge     () const;
      inline  DbU::Unit            getPitch         () const;
      inline  DbU::Unit            getSliceHeight   () const;
      inline  const FeedCells&     getFeedCells     () const;
      inline  void                 setCellWidget    ( Hurricane::CellWidget* );
              void                 startMeasures    ();
              void                 stopMeasures     ();
              void                 printMeasures    ( std::string ) const;
              void                 setDefaultAb     ();
              void                 resetPlacement   ();
              void                 toColoquinte     ();
              void                 place            ( unsigned int flags=SlowMotion );
      inline  void                 useFeed          ( Cell* );
              size_t               findYSpin        ();
              void                 addFeeds         ();
      virtual Record*              _getRecord       () const;
      virtual std::string          _getString       () const;
      virtual std::string          _getTypeName     () const;

    private:
    // Attributes.
      static Name                                     _toolName;
    protected:
             Configuration*                           _configuration;
             unsigned int                             _flags;
             Timer                                    _timer;
             coloquinte::box<coloquinte::int_t>       _surface;
             coloquinte::netlist                      _circuit;
             coloquinte::placement_t                  _placementLB;
             coloquinte::placement_t                  _placementUB;
             std::unordered_map<string,unsigned int>  _cellsToIds;
             std::vector<Instance*>                   _idsToInsts;
             Hurricane::CellWidget*                   _cellWidget;
             FeedCells                                _feedCells;
             size_t                                   _yspinSlice0;

    protected:
    // Constructors & Destructors.
                             EtesianEngine    ( Cell* );
      virtual               ~EtesianEngine    ();
      virtual void           _postCreate      ();
      virtual void           _preDestroy      ();
    private:                                  
                             EtesianEngine    ( const EtesianEngine& );
              EtesianEngine& operator=        ( const EtesianEngine& );
    private:
              void           _updatePlacement ( const coloquinte::placement_t&, unsigned int flags=0 );
              void           _progressReport1 ( time_t startTime, string label ) const;
              void           _progressReport2 ( time_t startTime, string label ) const;
  };


// Inline Functions.
  inline  bool             EtesianEngine::isSlowMotion   () const { return getConfiguration()->isSlowMotion(); }
  inline  void             EtesianEngine::setCellWidget  ( Hurricane::CellWidget* cw ) { _cellWidget = cw; }
  inline  CellGauge*       EtesianEngine::getCellGauge   () const { return getConfiguration()->getCellGauge(); }
  inline  DbU::Unit        EtesianEngine::getPitch       () const { return getCellGauge()->getPitch(); }
  inline  DbU::Unit        EtesianEngine::getSliceHeight () const { return getCellGauge()->getSliceHeight(); }
  inline  void             EtesianEngine::useFeed        ( Cell* cell ) { _feedCells.useFeed(cell); }
  inline  const FeedCells& EtesianEngine::getFeedCells   () const { return _feedCells; }


// Variables.
  extern const char* missingEtesian;


}  // Etesian namespace.


INSPECTOR_P_SUPPORT(Etesian::EtesianEngine);


#endif  // ETESIAN_ETESIAN_ENGINE_H
