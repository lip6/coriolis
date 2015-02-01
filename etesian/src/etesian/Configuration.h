// -*- mode: C++; explicit-buffer-name: "Configuration.h<etesian>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
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


#ifndef  ETESIAN_CONFIGURATION_H
#define  ETESIAN_CONFIGURATION_H

#include  <string>

#include  "hurricane/DbU.h"
namespace Hurricane {
  class Layer;
  class Cell;
}

#include  "crlcore/CellGauge.h"


namespace Etesian {


  using  std::string;
  using  Hurricane::Record;
  using  Hurricane::Layer;
  using  Hurricane::DbU;
  using  Hurricane::Cell;
  using  CRL::CellGauge;


// -------------------------------------------------------------------
// Class  :  "Etesian::Configuration" (decorator).


  class Configuration {
    public:
    // Constructor & Destructor.
      virtual                   ~Configuration      ();
      virtual Configuration*     clone              () const = 0;
    // Methods.                                     
      virtual CellGauge*         getCellGauge       () const = 0;
      virtual bool               isSlowMotion       () const = 0;
      virtual void               setFlags           ( unsigned int ) = 0;
      virtual void               unsetFlags         ( unsigned int ) = 0;
      virtual void               print              ( Cell* ) const = 0;
      virtual Record*            _getRecord         () const = 0;
      virtual string             _getString         () const = 0;
      virtual string             _getTypeName       () const = 0;
    protected:
                                 Configuration      ();
    private:
                                 Configuration      ( const Configuration& );
              Configuration&     operator=          ( const Configuration& );
    private:
      static  Configuration*     _default;
  };


// -------------------------------------------------------------------
// Class  :  "Etesian::ConfigurationConcrete".


  class ConfigurationConcrete : public Configuration {
      friend class Configuration;
    public:
    // Constructor & Destructor.
                                     ConfigurationConcrete ( const CellGauge* cg=NULL );
      virtual                       ~ConfigurationConcrete ();
      virtual ConfigurationConcrete* clone                 () const;
    // Methods.
      virtual CellGauge*             getCellGauge          () const;
      virtual bool                   isSlowMotion          () const;
      virtual void                   setFlags              ( unsigned int );
      virtual void                   unsetFlags            ( unsigned int );
      virtual void                   print                 ( Cell* ) const;
      virtual Record*                _getRecord            () const;
      virtual string                 _getString            () const;
      virtual string                 _getTypeName          () const;
    protected:
    // Attributes.
      CellGauge*    _cg;
      unsigned int  _flags;
    private:
                             ConfigurationConcrete ( const ConfigurationConcrete& );
      ConfigurationConcrete& operator=             ( const ConfigurationConcrete& );
  };


} // Etesian namespace.


INSPECTOR_P_SUPPORT(Etesian::Configuration);
INSPECTOR_P_SUPPORT(Etesian::ConfigurationConcrete);


#endif  // ETESIAN_CONFIGURATION_H
