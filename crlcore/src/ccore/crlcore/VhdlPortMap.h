// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/VhdlPortMap.h"                      |
// +-----------------------------------------------------------------+


#ifndef  VHDL_PORTMAP_H
#define  VHDL_PORTMAP_H

#include <set>
#include <iostream>
#include "hurricane/Net.h"
#include "crlcore/VhdlBit.h"
#include "crlcore/VhdlSignal.h"


namespace Vhdl {

  using Hurricane::Net;
  using Hurricane::Instance;


// -------------------------------------------------------------------
// Class  :  "Vhdl::PortMap".

  class PortMap {
    public:
      static  PortMap*      create        ( const Signal*, unsigned int flags );
      virtual const Signal* getSignal     () const = 0;
      virtual void          doMapping     ( Instance* ) = 0;
      virtual void          toVhdlPortMap ( std::ostream&, size_t ) const = 0;
                            PortMap       ( unsigned int flags );
      virtual              ~PortMap       ();
    protected:
      static  const Bit*    _lookup       ( const Bit* masterBit, Instance* );
    protected:
      unsigned int  _flags;
  };


// -------------------------------------------------------------------
// Class  :  "Vhdl::ScalarPortMap".

  class ScalarPortMap : public PortMap {
    public:
                                  ScalarPortMap ( const ScalarSignal*, unsigned int flags );
      virtual                    ~ScalarPortMap ();
      virtual const ScalarSignal* getSignal     () const;
      virtual void                doMapping     ( Instance* );
      virtual void                toVhdlPortMap ( std::ostream&, size_t ) const;
    private:
      const ScalarSignal*    _signal;
      const Bit*             _mapping;
  };


// -------------------------------------------------------------------
// Class  :  "Vhdl::VectorPortMap".

  class VectorPortMap : public PortMap {
    public:
                                  VectorPortMap ( const VectorSignal*, unsigned int flags );
      virtual                    ~VectorPortMap ();
      virtual const VectorSignal* getSignal     () const;
      virtual void                doMapping     ( Instance* );
      virtual void                toVhdlPortMap ( std::ostream&, size_t ) const;
    private:
      const VectorSignal*          _signal;
      std::map<size_t,const Bit*>  _mapping;
  };



}  // Vhdl namespace.

#endif  // VHDL_PORTMAP_H
