// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./seabreeze/Elmore.cpp"                        |
// +-----------------------------------------------------------------+

#include <string>
#include <vector>
#include <set>
#include "hurricane/Property.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Segment.h"
#include "Configuration.h"
#include "Tree.h"
#include "Delay.h"

namespace Hurricane {
  class Net;
  class Instance;
}

namespace Seabreeze {

  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::RoutingPad;
  using Hurricane::Contact;
  using Hurricane::Instance;
  using Hurricane::Segment;
  using Hurricane::PrivateProperty;
  class SeabreezeEngine;
  class ElmoreProperty;


//----------------------------------------------------------
// Class : Seabreeze::Elmore

  class Elmore {
      friend class ElmoreProperty;
    public:
                                   Elmore           ( Net* );
                                  ~Elmore           ();
      inline  SeabreezeEngine*     getSeabreeze     () const;
              const Configuration* getConfiguration () const;
      inline  Net*                 getNet           () const;
      inline  RoutingPad*          getDriver        () const;
              Delay*               getDelay         ( RoutingPad* ) const;
      inline  const std::vector<Delay*>&
                                   getDelays        () const;
              void                 buildTree        (); 
              void                 buildFromNode    ( Node* source, Segment* );
              Contact*             buildBranch      ( double* R, double* C, Contact* contact );
              void                 setRC            ( double* R, double* C, Contact* , Segment* );
      inline  Tree*                getTree          ();
              void                 setup            ();
              Delay*               delayElmore      ( RoutingPad* );
              void                 toTree           ( std::ostream& ) const;
      inline  void                 setSeabreeze     ( SeabreezeEngine* );
              Record*              _getRecord       () const;
              std::string          _getString       () const;
              std::string          _getTypeName     () const;
    private:
    private:
      SeabreezeEngine*     _seabreeze;
      Net*                 _net;
      RoutingPad*          _driver;
      Tree*                _tree;
      std::vector<Delay*>  _delays;
  };


  inline       SeabreezeEngine*     Elmore::getSeabreeze () const { return _seabreeze; }
  inline       Net*                 Elmore::getNet       () const { return _net; }
  inline       RoutingPad*          Elmore::getDriver    () const { return _driver; }
  inline       Tree*                Elmore::getTree      () { return _tree; }
  inline       void                 Elmore::setSeabreeze ( SeabreezeEngine* seabreeze ) { _seabreeze = seabreeze; }
  inline const std::vector<Delay*>& Elmore::getDelays    () const { return _delays; }


//---------------------------------------------------------
// Class : Seabreeze::ElmoreProperty

  class ElmoreProperty : public Hurricane::PrivateProperty {
      friend class ElmoreExtension;
    private:
      static Name _name;
    public:
      static  ElmoreProperty* create        ( Net* net );
      static  Name            staticGetName ();
              Name            getName       () const;
      inline  Elmore*         getElmore     ();  
      virtual string          _getTypeName  () const;  
      virtual Record*         _getRecord    () const;
      virtual std::string     _getString    () const;
    protected:
      Elmore  _elmore;
    protected:
      ElmoreProperty ( Net* );
  };


  inline Elmore* ElmoreProperty::getElmore ()
  { return &_elmore; }


//---------------------------------------------------------
// Class : Seabreeze::ElmoreExtension

  class ElmoreExtension {
    public:
      static        Elmore*     create     ( Net* );
      static        Elmore*     get        ( Net* );
      static inline Tree*       getTree    ( Net* );
      static inline void        toTree     ( Net*, std::ostream& );
      static        void        destroy    ( Net* );
  };


  inline Tree* ElmoreExtension::getTree ( Net* net )
  {
    Elmore* elmore = get( net );
    return (elmore) ? elmore->getTree() : nullptr;
  }


  inline void ElmoreExtension::toTree ( Net* net, std::ostream& os )
  {
    Elmore* elmore = get( net );
    if (elmore) elmore->toTree( os );
  }

  
}  // Seabreeze Namespace


INSPECTOR_P_SUPPORT(Seabreeze::Elmore);
INSPECTOR_P_SUPPORT(Seabreeze::ElmoreProperty);
