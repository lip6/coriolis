// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/ModelDescriptor.h"          |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_MODEL_DESCRIPTOR_H
#define HURRICANE_MODEL_DESCRIPTOR_H

#include "hurricane/Name.h"


namespace Hurricane {

  class ModelDescriptor {
    public:
      struct ModelDescriptorComp:
        public std::binary_function<const ModelDescriptor*, const ModelDescriptor*, bool> {
          bool operator () ( const ModelDescriptor* lmd, const ModelDescriptor* rmd ) const
          { return lmd->getName() < rmd->getName(); }
      };
    public:
      inline              ModelDescriptor ( const Name&        name
                                          , const Name&        simul
                                          , const Name&        model
                                          ,       std::string  netlist
                                          , const Name&        name_n
                                          , const Name&        name_p
                                          ,       bool         precise );
      inline Name         getName         () const;
      inline Name         getSimulator    () const;
      inline Name         getModel        () const;
      inline std::string  getNetlist      () const;
      inline Name         getName_N       () const;
      inline Name         getName_P       () const;
      inline bool         isPrecise       () const;
    private:
      Name         _name;
      Name         _simulator;
      Name         _model;
      std::string  _netlist;
      Name         _name_N;
      Name         _name_P;
      bool         _precise;
  };

  
  inline  ModelDescriptor::ModelDescriptor( const Name&       name
                                          , const Name&       simul
                                          , const Name&       model
                                          ,       std::string netlist
                                          , const Name&       name_n
                                          , const Name&       name_p
                                          ,       bool        precise )
        : _name     (name)
        , _simulator(simul)
        , _model    (model)
        , _netlist  (netlist)
        , _name_N   (name_n)
        , _name_P   (name_p)
        , _precise  (precise)
  { }


  inline Name         ModelDescriptor::getName      () const { return _name; }
  inline Name         ModelDescriptor::getSimulator () const { return _simulator; }
  inline Name         ModelDescriptor::getModel     () const { return _model; }
  inline std::string  ModelDescriptor::getNetlist   () const { return _netlist; }
  inline Name         ModelDescriptor::getName_N    () const { return _name_N; }
  inline Name         ModelDescriptor::getName_P    () const { return _name_P; }
  inline bool         ModelDescriptor::isPrecise    () const { return _precise; }


}  // Hurricane namespace.

#endif  // HURRICANE_MODEL_DESCRIPTOR_H
