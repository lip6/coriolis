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
// |  C++ Header  :  "./seabreeze/Tree.h"                            |
// +-----------------------------------------------------------------+


#pragma once
#include <set>
#include <vector>
#include <iostream>
#include "hurricane/RoutingPad.h"


namespace Seabreeze {

  using Hurricane::Record;
  using Hurricane::DBo;
  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  class Node;
  class Delay;
  class Elmore;


//---------------------------------------------------------
// Class : Seabreeze::Tree.
  
  class Tree {
    public:
      typedef  std::set<Contact*,DBo::CompareById>  ContactSet;
    public:
                                       Tree               ( Elmore* );
                                      ~Tree               ();
      inline       bool                isReached          ( Contact* ) const;
      inline       Elmore*             getElmore          () const;
      inline       size_t              getN               ();
                   Node*               getNode            ( Contact* );
      inline const std::vector<Node*>& getNodeList        () const;
      inline       void                setReached         ( Contact* );
                   void                addNode            ( Node* );
                   void                markNodeAfter      ( Node* );
                   void                getBranch          ( Contact* );
                   Delay*              computeElmoreDelay ( RoutingPad* );
                   void                printNode          ( std::ostream& , Node* , size_t depth );
                   void                print              ( std::ostream& );
                   Record*             _getRecord         () const;
                   std::string         _getString         () const;
                   std::string         _getTypeName       () const;
    private:
      Elmore*             _elmore;
      std::vector<Node*>  _nodes;
      ContactSet          _reacheds;
  };


  inline       Elmore*             Tree::getElmore   () const { return _elmore; }
  inline       size_t              Tree::getN        ()       { return _nodes.size(); }
  inline const std::vector<Node*>& Tree::getNodeList () const { return _nodes; }

  inline  void  Tree::setReached ( Contact* contact ) { _reacheds.insert( contact ); }
  inline  bool  Tree::isReached  ( Contact* contact ) const { return _reacheds.count( contact ); }


}  // Seabreeze namespace.


INSPECTOR_P_SUPPORT(Seabreeze::Tree);
