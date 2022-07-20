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
// |  C++ Module  :  "./Node.cpp"                                    |
// +-----------------------------------------------------------------+


#include <iostream>
#include "seabreeze/Node.h"

namespace Seabreeze {


  Node::Node ()
    : _R      (0.0)
    , _Rt     (0.0)
    , _C      (0.0)
    , _parent (nullptr)
    , _childs ()
    , _contact(nullptr)
    , _label  (-1)
    , _ap     (0)
  { }


  Node::Node ( Node* parent, Contact* contact )
    : _R      (0.0)
    , _Rt     (0.0)
    , _C      (0.0)
    , _parent (parent)
    , _childs ()
    , _contact(contact)
    , _label  (-1)
    , _ap     (0)
  { 
    if (parent) parent->addChild( this ); 
  }


  Node::~Node ()
  { }


}  // Seabreeze namespace.
