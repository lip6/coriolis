
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2011-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Node.cpp"                                    |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/Node.h"
using namespace std;


namespace OpenChams {


  Node::Node(const string& nodeName, Position pos, Node* parent) 
    : _name    (nodeName)
    , _position(pos)
    , _parent  (parent)
    , _right   (NULL)
    , _top     (NULL)
  { }
    

  Bloc::Bloc(const string& blocName, Position pos, Node* parent)
    : Node(blocName,pos,parent)
  { }


  Group::Group(const string& groupName, Position pos, Node* parent)
    : Node(groupName,pos,parent)
    , _isolated(false)
    , _paired(false)
    , _align(Group::NONE)
  { }


} // OpenChams namespace.
