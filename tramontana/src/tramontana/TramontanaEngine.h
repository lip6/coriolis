// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                    Christian MASSON             |
// |  First impl.  :                            Yifei WU             |
// |  Second impl. :                    Jean-Paul CHAPUT             |
// |  E-mail       :            Jean-Paul.Chaput@lip6.fr             |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/TramontanaEngine.h"              |
// +-----------------------------------------------------------------+


#pragma  once
#include <iostream>

#include "hurricane/Name.h"
namespace Hurricane {
  class Layer;
  class Net;
  class Cell;
  class CellViewer;
}
#include "crlcore/ToolEngine.h"
#include "tramontana/Configuration.h"
#include "tramontana/Equipotential.h"


namespace Tramontana {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::CellViewer;
  using CRL::ToolEngine;


// -------------------------------------------------------------------
// Class  :  "Tramontana::TramontanaEngine".

  class TramontanaEngine : public ToolEngine {
    public:
      const    uint32_t    DestroyStage = (1 << 0);
      typedef  ToolEngine  Super;
      typedef  std::set<const BasicLayer*,           DBo::CompareById>  LayerSet;
      typedef  std::map<const BasicLayer*, LayerSet, DBo::CompareById>  ConnexityMap;
      typedef  std::map<Net*, EquipotentialSet, DBo::CompareById>       OpenSet;
      typedef  std::set<Equipotential*,DBo::CompareById>                ShortedSet;
      typedef  OpenSet::value_type                                      OpenCircuit;
    public:
      static  const Name&              staticGetName      ();
      static  TramontanaEngine*        create             ( Cell*, uint32_t depth=0 );
      static  TramontanaEngine*        get                ( const Cell* );
    public:                                               
      inline        bool               inDestroyStage     () const;
      inline        bool               doMergeSupplies    () const;
      inline        Configuration*     getConfiguration   () const;
              const Name&              getName            () const;
      inline        uint32_t           getDepth           () const;
      inline  const EquipotentialSet   getEquipotentials  () const;
      inline  const ShortedSet&        getShortedNets     () const;
      inline  const OpenSet&           getOpenNets        () const;
                    bool               getSuccessState    () const;
      inline        void               setViewer          ( CellViewer* );
      inline        CellViewer*        getViewer          ();
      inline  const std::vector<const BasicLayer*>&
                                       getExtracteds      () const;
      inline        Layer::Mask        getExtractedMask   () const;
              const LayerSet&          getCutConnexLayers ( const BasicLayer* ) const;
                    bool               isExtractable      ( const Layer* ) const;
                    bool               isExtractable      ( const Net* ) const;
                    void               extract            ( bool isTopLevel=true );
                    void               _extract           ();
                    void               consolidate        ();
                    void               showEquipotentials () const;
                    void               printSummary       () const;
      inline        void               printConfiguration () const;
                    void               add                ( Equipotential* );
                    void               remove             ( Equipotential* );
      virtual       Record*            _getRecord         () const;
      virtual       std::string        _getString         () const;
      virtual       std::string        _getTypeName       () const;
    private:                          
                    void               _buildCutConnexMap ();
    private:                          
    // Attributes.                    
      static  Name                     _toolName;
    private:                         
              Configuration*                  _configuration;
              std::vector<const BasicLayer*>  _extracteds;
              Layer::Mask                     _extractedsMask;
              ConnexityMap                    _connexityMap;
              CellViewer*                     _viewer;
              uint32_t                        _depth;
              uint32_t                        _flags;
              std::set<Equipotential*,DBo::CompareById>  _equipotentials;
              OpenSet                         _openNets;
              ShortedSet                      _shortedNets;
              std::vector<Equipotential*>     _powerNets;
              std::vector<Equipotential*>     _groundNets;
    protected:
    // Constructors & Destructors.
                                TramontanaEngine ( Cell*, uint32_t depth );
      virtual                  ~TramontanaEngine ();
      virtual void              _postCreate      ();
      virtual void              _preDestroy      ();
    private:
                                TramontanaEngine ( const TramontanaEngine& ) = delete;
              TramontanaEngine& operator=        ( const TramontanaEngine& ) = delete;
  };

  
  inline bool           TramontanaEngine::inDestroyStage     () const { return (_flags & DestroyStage); }
  inline bool           TramontanaEngine::doMergeSupplies    () const { return _configuration->doMergeSupplies(); }
  inline Configuration* TramontanaEngine::getConfiguration   () const { return _configuration; }
  inline void           TramontanaEngine::setViewer          ( CellViewer* viewer ) { _viewer=viewer; }
  inline CellViewer*    TramontanaEngine::getViewer          () { return _viewer; }
  inline  const std::vector<const BasicLayer*>&
                        TramontanaEngine::getExtracteds      () const { return _extracteds; }
  inline  Layer::Mask   TramontanaEngine::getExtractedMask   () const { return _extractedsMask; }
  inline uint32_t       TramontanaEngine::getDepth           () const { return _depth; }
  inline const std::set<Equipotential*,DBo::CompareById>        
                        TramontanaEngine::getEquipotentials  () const { return _equipotentials; }
  inline const TramontanaEngine::ShortedSet&
                        TramontanaEngine::getShortedNets     () const { return _shortedNets; }
  inline  const TramontanaEngine::OpenSet&
                        TramontanaEngine::getOpenNets        () const { return _openNets; }
  inline void           TramontanaEngine::printConfiguration () const { _configuration->print( getCell() ); }


}  // Tramontana namespace.


INSPECTOR_P_SUPPORT(Tramontana::TramontanaEngine);
