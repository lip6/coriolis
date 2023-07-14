// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/Rule.h"                            |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include "hurricane/Name.h"


namespace Hurricane {


  class Rule {
    public:
      inline               Rule         ( const Name& name, const std::string& reference );
      inline               Rule         ( const Rule& rule );
      virtual             ~Rule         ();
      inline Name          getName      () const;
      inline std::string   getReference () const;
    public:               
    // Inspector support. 
      virtual std::string  _getTypeName () const;
      virtual std::string  _getString   () const;
      virtual Record*      _getRecord   () const;
    private:
      const Name         _name;
      const std::string  _reference;
  };
  
  
  inline                 Rule::Rule         ( const Name& name, const std::string& reference ): _name(name), _reference(reference) { }
  inline                 Rule::Rule         ( const Rule& rule ): _name(rule._name), _reference(rule._reference) { }
  inline Hurricane::Name Rule::getName      () const { return _name; }
  inline std::string     Rule::getReference () const { return _reference; }


}  // Hurricane namespace.


INSPECTOR_P_SUPPORT(Hurricane::Rule);
