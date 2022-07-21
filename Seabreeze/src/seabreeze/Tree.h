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
namespace Hurricane {
  class Contact;
  class RoutingPad;
}


namespace Seabreeze {

  using Hurricane::Record;
  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  class Node;


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
                   Record*             _getRecord         () const;
                   std::string         _getString         () const;
                   std::string         _getTypeName       () const;
    private:	
      std::vector<Node*>  _nodes;
  };


  inline       size_t              Tree::getN        ()       { return _nodes.size(); }
  inline const std::vector<Node*>& Tree::getNodeList () const { return _nodes; }


}  // Seabreeze namespace.


INSPECTOR_P_SUPPORT(Seabreeze::Tree);
