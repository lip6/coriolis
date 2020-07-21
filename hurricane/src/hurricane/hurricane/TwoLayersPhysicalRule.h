// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  : hurricane/TwoLayersPhysicalRule.h                |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/PhysicalRule.h"


namespace Hurricane {

  class TwoLayersPhysicalRule : public PhysicalRule {
    public:
      typedef PhysicalRule Super;
    public:
      inline               TwoLayersPhysicalRule ( const Name&        name
                                                 ,       DbU::Unit    value
                                                 , const std::string& reference
                                                 ,       bool         symetric
                                                 );
      virtual             ~TwoLayersPhysicalRule ();
      inline  bool         isSymetric            () const;
    public:
      virtual std::string  _getTypeName          () const;
      virtual std::string  _getString            () const;
      virtual Record*      _getRecord            () const;
    private:
      const bool  _symetric; 
  };
  
  
  TwoLayersPhysicalRule::TwoLayersPhysicalRule ( const Name&        name
                                               ,       DbU::Unit    value
                                               , const std::string& reference
                                               ,       bool         symetric
                                               )
    : PhysicalRule(name,reference)
    , _symetric(symetric)
  {
    addValue( value, 0 );
  }
  

  bool TwoLayersPhysicalRule::isSymetric () const { return _symetric; } 


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::TwoLayersPhysicalRule);
