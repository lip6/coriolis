// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universié 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./foehn/Dag.h"                                 |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <vector>
namespace Hurricane {
  class Instance;
}
#include "hurricane/Plug.h"
#include "foehn/Configuration.h"


namespace Foehn {

  using std::string;
  using std::vector;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::DBo;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::Plug;
  class FoehnEngine;


// -------------------------------------------------------------------
// Class  :  "Foehn::Dag".


  class Dag  {
    public:
                                          Dag                   ( FoehnEngine*, std::string label  );
                                          Dag                   ( const Dag& ) = delete;
                                         ~Dag                   ();
                    Dag&                  operator=             ( const Dag& ) = delete;
                    FoehnEngine*          getFoehn              ();
      inline        Configuration&        getConfiguration      ();
      inline        std::string           getLabel              () const;
                    Cell*                 getCell               () const;
      inline        bool                  isDff                 ( std::string ) const;
      inline        bool                  isDff                 ( Name ) const;
      inline        bool                  isIgnoredNet          ( std::string ) const;
      inline        bool                  isIgnoredNet          ( Name ) const;
      inline        bool                  isIgnoredMasterNet    ( std::string ) const;
      inline        bool                  isIgnoredMasterNet    ( Name ) const;
      inline        bool                  isIgnoredPlug         ( const Plug* ) const;
      inline        void                  setDffRe              ( std::string );        
      inline        void                  setIgnoredNetRe       ( std::string );        
      inline        void                  setIgnoredMasterNetRe ( std::string );        
                    void                  addDStart             ( Instance* );
                    void                  addDStart             ( Net* );
                    void                  addToDOrder           ( Instance* );
                    void                  dpropagate            ();
                    void                  resetDepths           ();
                    void                  _dpropagateOn         ( Net* );
                    void                  _dpropagateOn         ( Instance* );
      inline  const std::vector<Entity*>& getDOrder () const;   
    // Inspector support.                                       
                    Record*               _getRecord            () const;
                    string                _getString            () const;
                    string                _getTypeName          () const;
    private:
             FoehnEngine*            _foehn;
             Configuration           _configuration;
             std::string             _label;
             std::vector<Entity*>    _dorder;
             std::vector<Net*>       _inputs;
             std::vector<Instance*>  _reacheds;
  };

  
  inline       FoehnEngine*          Dag::getFoehn              () { return _foehn; }
  inline       Configuration&        Dag::getConfiguration      () { return _configuration; }
  inline       std::string           Dag::getLabel              () const { return _label; }
  inline       bool                  Dag::isDff                 ( std::string name ) const { return _configuration.isDff(name); }
  inline       bool                  Dag::isDff                 ( Name        name ) const { return _configuration.isDff(getString(name)); }
  inline       bool                  Dag::isIgnoredNet          ( std::string name ) const { return _configuration.isIgnoredNet(name); }
  inline       bool                  Dag::isIgnoredNet          ( Name        name ) const { return _configuration.isIgnoredNet(getString(name)); }
  inline       bool                  Dag::isIgnoredMasterNet    ( std::string name ) const { return _configuration.isIgnoredMasterNet(name); }
  inline       bool                  Dag::isIgnoredMasterNet    ( Name        name ) const { return _configuration.isIgnoredMasterNet(getString(name)); }
  inline       void                  Dag::setDffRe              ( std::string name ) { _configuration.setDffRe(name); }       
  inline       void                  Dag::setIgnoredNetRe       ( std::string name ) { _configuration.setIgnoredNetRe(name); }       
  inline       void                  Dag::setIgnoredMasterNetRe ( std::string name ) { _configuration.setIgnoredMasterNetRe(name); }       
  inline const std::vector<Entity*>& Dag::getDOrder             () const { return _dorder; }

  inline bool  Dag::isIgnoredPlug ( const Plug* plug ) const
  {
    if (not plug->getNet()) return true;
    if (isIgnoredNet(plug->getNet()->getName())) return true;
    if (isIgnoredMasterNet(plug->getMasterNet()->getName())) return true;
    return false;
  }


}  // Foehn namespace.


INSPECTOR_P_SUPPORT(Foehn::Dag);
