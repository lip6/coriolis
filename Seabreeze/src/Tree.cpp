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
// |  C++ Module  :  "./Tree.cpp"                                    |
// +-----------------------------------------------------------------+


#include <string>
#include <iostream>
#include <algorithm>
#include "hurricane/Error.h"
#include "seabreeze/Tree.h"


namespace Seabreeze {

  using std::string;
  using std::find;
  using std::set;
  using std::cerr;
  using std::endl;
  using std::ostream;
  using Hurricane::Error;


  Tree::Tree ()
    : _nodes()
  {}


  Tree::~Tree ()
  {
    for( Node* n : _nodes) delete n;
  }


  Node* Tree::get_node ( Contact* contact )
  {
    for ( Node* n : _nodes ) {
      if (n->contact() == contact) return n;
    }
    return nullptr;
  }


  void  Tree::new_node ()
  { _nodes.push_back( new Node() ); }


  void Tree::add_node ( Node* node )
  {
    node->setLabel( _nodes.size() );
    if (find(_nodes.begin(), _nodes.end(), node) == _nodes.end())
      _nodes.push_back( node );
  }


  void  Tree::After_i ( Node *ni )
  {
    if (not ni) return;
    ni->setAp( 1 );
    for ( Node* child : ni->childs() ) {
      After_i( child );
    }
  }


  set<Node*> Tree::Branch_i ( Contact* contact )
  {
    set<Node*> parents;
    Node *ni = get_node( contact );
    while ( ni->parent() ) {
      parents.insert( ni->parent() );
      ni = ni->parent();
    }
    return parents;
  }


  double  Tree::Delay_Elmore ( RoutingPad* rp )
  {
    if (not rp) {
      cerr << Error( "Tree::computeDelay(): Sink RoutingPad argument is NULL." ) << endl;
      return -1.0;
    }
  
    Contact* sink = nullptr;
    for ( Component* component : rp->getSlaveComponents() ) {
      Contact* contact = dynamic_cast<Contact*>( component );
      if (contact) {
        sink = contact;
        break;
      }
    }
    if (not sink) {
      cerr << Error( "Tree::computeDelay(): No Contact anchored on RoutingPad, aborting.\n"
                     "        (on %s)"
                   , getString(rp).c_str()
                   ) << endl;
      return -1.0;
    }
    
    cdebug_log(199,1) << "Tree::computeDelay()" << endl;
    cdebug_log(199,0) << "  rp=" << rp << endl;
    cdebug_log(199,0) << "  sink=" << sink << endl;
  
    set<Node*> br = Branch_i( sink );
    Node*      ni = get_node( sink );
    After_i( ni );
    ni->setAp( 0 );

  // Compute Rt of all nodes
    for ( size_t k = 0; k < _nodes.size(); k++ ) {
      if (k == 0)
        _nodes[k]->setRt( _nodes[k]->R() );
      else {
        if (_nodes[k]->ap() == 0) {
          if (br.count(_nodes[k]) > 0) {
            _nodes[k]->setRt( _nodes[k]->parent()->Rt() + _nodes[k]->R() );
          } else {
            _nodes[k]->setRt( _nodes[k]->parent()->Rt() );
          }
        } else {
          _nodes[k]->setRt( ni->Rt() );
        }
      }
    }
  
  // Compute Elmore delay time 
    double delay = 0.0;
    for ( size_t k = 0; k < _nodes.size(); k++ ) {
      delay += (_nodes[k]->Rt()) * (_nodes[k]->C());
    }

    return delay;
  }


  void  Tree::printNode ( ostream& os, Node* node, size_t depth )
  {
    string indent ( depth*2, ' ');
    os << indent << node->contact() << " R=" << node->R() << " C=" << node->C() << endl;
    for ( Node* child : node->childs() ) {
      printNode( os, child, depth+1 );
    }
  }


  void  Tree::print ( ostream& os )
  {
    os << "Elmore Tree of " << _nodes[0]->contact()->getNet() << endl;
    os << "  Tree has " << _nodes.size() << " nodes :" << endl;
    printNode( os, _nodes[0], 0 );
  }


  void  Tree::clear ()
  {
    _nodes.clear();
  }


}  // Seabreeze namespace.
