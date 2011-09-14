/*
 *  Node.cpp
 *  openChams
 *
 *  Created by damien dupuis on 23/08/11.
 *  Copyright 2010-2011 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "vlsisapd/openChams/Node.h"

namespace OpenChams {
Node::Node(Name nodeName, Position pos, Node* parent) 
    : _name(nodeName)
    , _position(pos)
    , _parent(parent)
    , _right(NULL)
    , _top(NULL)
{}
    
Bloc::Bloc(Name blocName, Position pos, Node* parent)
    : Node(blocName, pos, parent)
{}

Group::Group(Name groupName, Position pos, Node* parent)
     : Node(groupName, pos, parent)
     , _isolated(false)
     , _paired(false)
     , _align(Group::NONE)
{}

} // namespace
