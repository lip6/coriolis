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

  using std::string;


  Node::Node ()
    : _R      (0.0)
    , _Rt     (0.0)
    , _C      (0.0)
    , _parent (nullptr)
    , _childs ()
    , _contact(nullptr)
    , _label  (0)
    , _ap     (0)
  { }


  Node::Node ( Node* parent, Contact* contact )
    : _R      (0.0)
    , _Rt     (0.0)
    , _C      (0.0)
    , _parent (parent)
    , _childs ()
    , _contact(contact)
    , _label  (0)
    , _ap     (0)
  { 
    if (parent) parent->addChild( this ); 
  }


  Node::~Node ()
  { }


  string  Node::_getTypeName () const
  { return "Seabreeze::Node"; }


  string  Node::_getString () const
  {
    string  s = "<Node ";
    s += getString( _contact );
    s += " R=" + getString( _R );
    s += " C=" + getString( _C );
    s += ">";
    return s;
  }


  Record* Node::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record != nullptr) {
      record->add( getSlot("_R"      ,  _R      ) );
      record->add( getSlot("_Rt"     ,  _Rt     ) );
      record->add( getSlot("_C"      ,  _C      ) );
      record->add( getSlot("_parent" ,  _parent ) );
      record->add( getSlot("_childs" , &_childs ) );
      record->add( getSlot("_contact",  _contact) );
      record->add( getSlot("_label"  ,  _label  ) );
      record->add( getSlot("_ap"     ,  _ap     ) );
    }
    return record;
  }

}  // Seabreeze namespace.
