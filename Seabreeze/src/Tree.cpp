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
#include "hurricane/RoutingPad.h"
#include "seabreeze/Tree.h"
#include "seabreeze/Node.h"


namespace Seabreeze {

  using std::string;
  using std::find;
  using std::set;
  using std::cerr;
  using std::endl;
  using std::ostream;
  using Hurricane::Error;
  using Hurricane::Component;


  Tree::Tree ()
    : _nodes()
  {}


  Tree::~Tree ()
  {
    for( Node* n : _nodes) delete n;
  }


  Node* Tree::getNode ( Contact* contact )
  {
    for ( Node* n : _nodes ) {
      if (n->contact() == contact) return n;
    }
    return nullptr;
  }


  void  Tree::newNode ()
  { _nodes.push_back( new Node() ); }


  void Tree::addNode ( Node* node )
  {
    if (find(_nodes.begin(), _nodes.end(), node) == _nodes.end())
      _nodes.push_back( node );
  }


  void  Tree::markNodeAfter ( Node *ni )
  {
    if (not ni) return;
    ni->setAp( 1 );
    for ( Node* child : ni->childs() ) {
      markNodeAfter( child );
    }
  }


  void Tree::getBranch ( Contact* contact )
  {
    for ( Node* n : _nodes ) {
      n->setLabel(0);
    }

    Node *ni = getNode( contact );
    ni->setLabel(1);
    while ( ni->parent() ) {
      ni->parent()->setLabel(1);
      ni = ni->parent();
    }
  }


  double  Tree::computeElmoreDelay ( RoutingPad* rp )
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
  
    getBranch( sink );
    Node*      ni = getNode( sink );
    markNodeAfter( ni );
    ni->setAp( 0 );
  // Compute Rt of all nodes
    for ( size_t k = 0; k < _nodes.size(); k++ ) {
      if (k == 0)
        _nodes[k]->setRt( _nodes[k]->R() );
      else {
        if (_nodes[k]->ap() == 0) {
          if (_nodes[k]->label() == 1) {
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


  string Tree::_getTypeName () const
  { return "Seabreeze::Tree"; }


  string  Tree::_getString () const
  {
    string  s = "<" + _getTypeName() + ">";
    return s;
  }


  Record* Tree::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record != nullptr) {
      record->add( getSlot("_nodes", &_nodes) );
    }
    return record;
  }


}  // Seabreeze namespace.
