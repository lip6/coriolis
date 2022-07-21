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
#include "hurricane/Contact.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Component.h"
#include "Node.h"


namespace Seabreeze {

  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  using Hurricane::Component;


//---------------------------------------------------------
// Class : Seabreeze::Tree.
  
  class Tree {
    public:
                                       Tree               ();
                                      ~Tree               ();
      inline       size_t              getN               ();
                   Node*               getNode            ( Contact* );
      inline const std::vector<Node*>& getNodeList        () const;
                   void                newNode            ();
                   void                addNode            ( Node* );
                   void                markNodeAfter      ( Node* );
                   void                getBranch          ( Contact* );
                   double              computeElmoreDelay ( RoutingPad* );
                   void                printNode          ( std::ostream& , Node* , size_t depth );
                   void                print              ( std::ostream& );
                   void                clear              ();
    private:	
      std::vector<Node*>  _nodes;
  };


  inline       size_t              Tree::getN        ()       { return _nodes.size(); }
  inline const std::vector<Node*>& Tree::getNodeList () const { return _nodes; }


}  // Seabreeze namespace.
