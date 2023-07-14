// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/ToolEngine.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/Commons.h"
#include "hurricane/Timer.h"
#include "hurricane/DBo.h"
#include "hurricane/Slot.h"

namespace Hurricane {
  class Name;
  class Cell;
}

#include  "crlcore/Measures.h"
#include  "crlcore/ToolEngines.h"


namespace CRL {

  using Hurricane::Timer;
  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Cell;
    

// -------------------------------------------------------------------
// Class  :  "CRL::ToolEngine".

  class ToolEngine : public DBo {
    public:
      typedef  DBo  Super;
    public:
      static        ToolEngines  get                                 ( const Cell* cell );
      static        ToolEngine*  get                                 ( const Cell* cell, const Name& name );
      static        void         destroyAll                          ();
      static        bool         inDestroyAll                        ();
    public:
      virtual const Name&        getName                             () const = 0;
      inline        Cell*        getCell                             () const;
      inline        uint32_t     getPassNumber                       () const;
                    bool         placementModificationFlagHasChanged ();
                    bool         routingModificationFlagHasChanged   ();
      inline        void         setInRelationDestroy                ( bool );
      inline  const Timer&       getTimer                            () const;
      inline        void         setPassNumber                       ( uint32_t );
      inline        std::string  getMeasureLabel                     ( std::string ) const;
                    void         startMeasures                       ();
                    void         stopMeasures                        ();
                    void         suspendMeasures                     ();
                    void         resumeMeasures                      ();
                    void         printMeasures                       () const;
      template<typename Data>
      inline        void         addMeasure                          ( std::string, const Data&, unsigned int width ) const;
      template<typename Data>
      inline        void         addMeasure                          ( std::string, const Data& ) const;
      template<typename Data>
      inline        void         addMeasure                          ( std::string, Data* ) const;
      template<typename Data>
      inline        const Data*  getMeasure                          ( std::string ) const;
      virtual       std::string  _getTypeName                        () const;
      virtual       std::string  _getString                          () const;
      virtual       Record*      _getRecord                          () const;
    private:
      static        bool         _inDestroyAll;
    protected:
                    Cell*        _cell;
    private:
                    bool         _verbose;
                    unsigned int _placementModificationFlag;
                    unsigned int _routingModificationFlag;
                    bool         _inRelationDestroy;
                    Timer        _timer;
                    uint32_t     _passNumber;
    protected:
                                 ToolEngine                          ( Cell* cell, bool verbose=true );
      virtual       void         _postCreate                         ();
      virtual       void         _preDestroy                         ();
    protected:      
                    void         grabPlacementModificationFlag       ();
                    void         getPlacementModificationFlag        ();
                    void         grabRoutingModificationFlag         ();
                    void         getRoutingModificationFlag          ();
  };


// -------------------------------------------------------------------
// Inline Functions.


  inline       Cell*     ToolEngine::getCell              () const { return _cell; }
  inline       void      ToolEngine::setInRelationDestroy ( bool state ) { _inRelationDestroy = state; }
  inline const Timer&    ToolEngine::getTimer             () const { return _timer; }
  inline       uint32_t  ToolEngine::getPassNumber        () const { return _passNumber; }
  inline       void      ToolEngine::setPassNumber        ( uint32_t n ) { _passNumber = n; }

  inline std::string  ToolEngine::getMeasureLabel ( std::string label ) const
  { return _getTypeName() + "." + label; }
      
  template<typename Data>
  inline void  ToolEngine::addMeasure ( std::string name, const Data& data, unsigned int width ) const
  { ::CRL::addMeasure( getCell(), getMeasureLabel(name), getPassNumber(), data, width ); }

  template<typename Data>
  inline void  ToolEngine::addMeasure ( std::string name, const Data& data ) const
  { ::CRL::addMeasure( getCell(), getMeasureLabel(name), getPassNumber(), data ); }

  template<typename Data>
  inline void  ToolEngine::addMeasure ( std::string name, Data* data ) const
  { ::CRL::addMeasure( getCell(), getMeasureLabel(name), getPassNumber(), data ); }

  template<typename Data>
  inline const Data* ToolEngine::getMeasure ( std::string name ) const
  {
    const Measure<Data>* measure = ::CRL::getMeasure<Data>( getCell(), getMeasureLabel(name) );
    if (not measure) return NULL;
    return &(measure->getData( getPassNumber() ));
  }


} // CRL namespace.
