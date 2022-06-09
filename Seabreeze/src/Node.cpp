#include "Seabreeze/Node.h"

#include <iostream>

Node::Node ()
  : R(0)
  , C(0)
  , label(-1)
  , ap(0)
{ _contact = nullptr; }

Node::Node ( Node* p, Contact* ct )
  : R(0)
  , C(0)
  , Np(p)
  , _contact(ct)
  , label(-1)
  , ap(0)
{ 
  if( p != nullptr) 
    (p->Ne).push_back(this); 
}

Node::~Node ()
{}
