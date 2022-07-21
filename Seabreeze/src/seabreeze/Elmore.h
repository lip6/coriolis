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


//----------------------------------------------------------
// Class : Seabreeze::Elmore

  class Elmore {
    public:
      typedef  std::set<Contact*,DBo::CompareById>  ContactSet;
    public:
                                   Elmore           ( Net* );
                                  ~Elmore           ();
      inline  SeabreezeEngine*     getSeabreeze     () const;
              const Configuration* getConfiguration () const;
              void                 contFromNet      ( Net* );
              void                 buildTree        ( RoutingPad* ); 
              void                 buildFromNode    ( Node* source, Segment* );
              Contact*             buildBranch      ( double* R, double* C, Contact* contact );
              void                 setRC            ( double* R, double* C, Contact* , Segment* );
              void                 clearTree        ();
      inline  Tree*                getTree          ();
      inline  const ContactSet&    getContacts      () const;
              double               delayElmore      ( RoutingPad* );
              void                 toTree           ( std::ostream& ) const;
      inline  void                 setSeabreeze     ( SeabreezeEngine* );
      virtual Record*              _getRecord       () const;
      virtual std::string          _getString       () const;
      virtual std::string          _getTypeName     () const;
    private:
      SeabreezeEngine* _seabreeze;
      ContactSet       _contacts;
      ContactSet       _checker;
      Tree*            _tree;
  };


  inline SeabreezeEngine*          Elmore::getSeabreeze () const { return _seabreeze; }
  inline const Elmore::ContactSet& Elmore::getContacts  () const { return _contacts; }
  inline       Tree*               Elmore::getTree      () { return _tree; }
  inline       void                Elmore::setSeabreeze ( SeabreezeEngine* seabreeze ) { _seabreeze = seabreeze; }


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
