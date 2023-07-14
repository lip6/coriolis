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
// |  C++ Header  :  "./hurricane/analog/Parameter.h"                |
// +-----------------------------------------------------------------+


#ifndef ANALOG_PARAMETER_H
#define ANALOG_PARAMETER_H

#include <string>
#include "hurricane/Commons.h"


namespace Analog {

  using Hurricane::Record;


  class Parameter {
    public:
      friend class Device;
      enum  Type  { STEP=0, CHOICE=1, SPIN=2, MCHECK=3, CAPACITOR=4 };
    public:
      virtual                   ~Parameter    ();
      inline  const std::string& getName      () const;
      inline        int          getIndex     ();
      inline        void         setIndex     ( int );
      virtual       std::string  _getTypeName () const = 0;
      virtual       std::string  _getString   () const;
      virtual       Record*      _getRecord   () const;
    protected:
      inline                     Parameter    ( const std::string& name );
    private:
                                 Parameter    ();
                                 Parameter    ( const Parameter& parameter );
    private:
      const std::string  _name;
            int          _index;
  };


  inline                    Parameter::Parameter ( const std::string& name ) : _name(name), _index(0) {}
  inline const std::string& Parameter::getName   () const { return _name; }
  inline       int          Parameter::getIndex  () { return _index; }
  inline       void         Parameter::setIndex  ( int index ) { _index = index; }


} // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::Parameter);

#endif  // ANALOG_PARAMETER_H
