#include "Seabreeze/Seabreeze.h"
#include "hurricane/Net.h"

namespace Seabreeze {

  using namespace std;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::PrivateProperty;
  using Hurricane::Component;

//---------------------------------------------------------
// Class : "Elmore"

  Elmore::Elmore ( Net* net )
    : _conts()
    , checker()
    , _tree()
  {}

  Elmore::~Elmore ()
  {
    delete _tree;
  }

  void Elmore::contFromNet ( Net* net ) {
    for ( RoutingPad* rp : net->getRoutingPads() ) {

      for ( Component* c : rp->getSlaveComponents() ) {
        Contact* ct = dynamic_cast<Contact*>(c);
        
        if ( not ct )   continue;

        _conts.insert(ct);
      }
    }
  }

  void Elmore::buildTree ( RoutingPad* rp )
  {
    if ( rp == nullptr ) {
      cerr << "Input RoutingPad is NULL. Please select a RoutingPad !" << endl;
      return;
    }
    Contact* ct = dynamic_cast<Contact*>(rp);
    if ( not ct ) {
      cerr << "No contact found" << endl;
      return;
    }
    
    Node* s = new Node(nullptr, ct);
    build_from_node(s);
  }

  void Elmore::build_from_node ( Node* s  )
  {
    if ( s->_contact == nullptr || find(_conts.begin(), _conts.end(), s->_contact) == _conts.end() ) {
      return;
    }

    _tree->add_node(s);
  
    // To check for circle
    checker.insert(s->_contact);

    for ( Component* c : (s->_contact)->getSlaveComponents() ) {

      Contact* ccont = dynamic_cast<Contact*>(c);

      if ( not ccont )
        continue;
      else{
        if ( find( _conts.begin(), _conts.end(), ccont) != _conts.end() ) {

          if ( find( checker.begin(), checker.end(), ccont) != checker.end() ) {
            cerr << "Net contains a circle. Cannot apply Elmore's delay here !" << endl;
            return;
          }
          else {
            if ( ccont != (s->Np)->_contact ) {
              Node* node = new Node( s, ccont );
              build_from_node(node);
            }
          }
        }
      }
    }
  }

  void Elmore::clearTree ()
  {
    _tree->clear();
    checker.clear();
  }

  Tree* Elmore::getTree ()
  {
    return _tree;
  }

  int Elmore::delayElmore ( RoutingPad* rp )
  {
    return _tree->Delay_Elmore(rp);
  }
  
  void Elmore::toTREE ( ostream& out ) const
  {
    _tree->print( out );
  }

//---------------------------------------------------------
// Class : "ElmoreProperty"

  Name ElmoreProperty::_name = "Seabreeze::Elmore";
  
  ElmoreProperty* ElmoreProperty::create ( Net* net )
  {
    ElmoreProperty* property = new ElmoreProperty(net);
    //ElmoreProperty* property(net);
 
    property->_postCreate();
    return property;
  }

  Name ElmoreProperty::getName () const
  {
    return _name;
  }

  string ElmoreProperty::_getTypeName () const
  {
    return "ElmoreProperty";
  }

//---------------------------------------------------------
// Class : "ElmoreExtension"

  Elmore* ElmoreExtension::create ( Net* net )
  {
    return ElmoreProperty::create(net)->getElmore();
  }

  void ElmoreExtension::destroyAll ()
  {
    
  }

  void ElmoreExtension::destroy ( Net* net )
  {

  }
}
