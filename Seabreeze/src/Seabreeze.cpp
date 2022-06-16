#include "Seabreeze/Seabreeze.h"
#include "hurricane/Net.h"
#include "hurricane/Segment.h"

namespace Seabreeze {

  using namespace std;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::PrivateProperty;
  using Hurricane::Component;
  using Hurricane::Segment;

//---------------------------------------------------------
// Class : "Elmore"

  Elmore::Elmore ( Net* net )
    : _conts()
    , checker()
    , _tree(new Tree())
  {}

  Elmore::~Elmore ()
  {
    delete _tree;
  }

  void Elmore::contFromNet ( Net* net ) {
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      for ( Component* comp : rp->getSlaveComponents() ) {
        Contact* ct = dynamic_cast<Contact*>(comp);
        if ( not ct )   continue;
        _conts.insert(ct);
      }
    }
  }

  void Elmore::buildTree ( RoutingPad* rp )
  {
    if ( rp == nullptr ) {
      cerr << "Input RoutingPad is NULL. Please select a RoutingPad !" << endl;
      cerr << "Cannot build tree" << endl;
      return;
    }

    Contact* ct = nullptr;
    for ( Component* c : rp->getSlaveComponents() ) {
      Contact* cont = dynamic_cast<Contact*>(c);
        
      if ( cont ) {//&& find(_conts.begin(), _conts.end(), cont) != _conts.end() ) {
        ct = cont;
        break;
      }
    }
    if ( ct == nullptr ) {
      cerr << "No contact found" << endl;
      cerr << "Cannot build tree" << endl;
      return;
    }

    cerr << "Root contact : " << ct << endl;
    cerr << "Start building tree..." << endl;

    Node* s = new Node(nullptr, ct);
//    build_from_node(s);
    Segment* seg = nullptr;
    int c = 0;
    for ( Component* comp : ct->getSlaveComponents() ) {
      seg = dynamic_cast<Segment*>(comp);
      if ( not seg ) continue;
      c++;
    }
    if ( c != 1 ) {
      cerr << "Not begin with a RoutingPad ? Something doesn't seem right : " << c << " linked segments" << endl;
      cerr << "Tree build failed" << endl;
      return;
    }
    build_from_Node(s, seg);

    cerr << endl << "Finished building tree !" << endl << endl;
    _tree->print(cerr);
//    cerr << "Tree built succesfully !" << endl;
  }

  void Elmore::build_from_node ( Node* s  )
  {
    if ( s->_contact == nullptr ) {
      cerr << "No contact found" << s->_contact << endl;
      cerr << "Tree build failed" << endl;
      return;
    }

    _tree->add_node(s);
  
    // To check for circle
    checker.insert(s->_contact);

    for ( Component* comp : (s->_contact)->getSlaveComponents() ) {

      Segment* seg = dynamic_cast<Segment*>(comp);

      if ( not seg ) continue;

      Contact* ccont = dynamic_cast<Contact*>(seg->getOppositeAnchor(s->_contact));
      if ( not ccont || (s->Np && ccont == (s->Np)->_contact) )
        continue;
      else{               
        if ( find( checker.begin(), checker.end(), ccont) != checker.end() ) {
          cerr << "Net contains a circle. Cannot apply Elmore's delay here !" << endl;
          return;
        }
        else {
          if ( not s->Np || ccont != (s->Np)->_contact ) {
            Node* node = new Node(s, ccont);
            build_from_node(node);
          }
        }
      } 
    }
  }

  void Elmore::build_from_Node ( Node* s, Segment* seg ) {
    if ( s->_contact == nullptr ) {
      cerr << "No contact found" << s->_contact << endl;
      cerr << "Tree build failed" << endl;
      return;
    }

    _tree->add_node(s);
//----------------------------------------------------------------------------------------
    cerr << "Build from contact : " << s->_contact << endl;
//----------------------------------------------------------------------------------------    
    Contact* ccont = dynamic_cast<Contact*>(seg->getOppositeAnchor(s->_contact));
    if ( not ccont || (s->Np && ccont == (s->Np)->_contact) )
      return;
//----------------------------------------------------------------------------------------
    cerr << "Target contact : " << ccont << endl;
//----------------------------------------------------------------------------------------
//    ccont = build_branch(ccont);
    Contact* ctr = nullptr;
    build_branch(ctr, ccont);
//----------------------------------------------------------------------------------------
    cerr << "Found a node : " << ctr << endl;
//----------------------------------------------------------------------------------------
//    if ( not ccont ) {
    if ( not ctr ) {
      cerr << "This branch leads to a NULL contact ?" << endl;
      cerr << "Tree build failed" << endl;
      return;
    }

    Node* node = new Node(s, ccont);
    int count = 1;
//    for ( Component* comp : ccont->getSlaveComponents() ) {
    for ( Component* comp : ctr->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(comp)) ? 1 : 0;
    }
    if ( count == 1 ){
      _tree->add_node(node);
    }
    else if ( count > 2 ) {
//      for ( Component* comp : ccont->getSlaveComponents() ) {
      for ( Component* comp : ctr->getSlaveComponents() ) {
        Segment* segmt = dynamic_cast<Segment*>(comp);
        if ( not segmt ) continue;
        build_from_Node(node, segmt);
      }
    }
  }

  void Elmore::build_branch ( Contact* ctr, Contact* ct ) {
    int count = 0;
    checker.insert(ct);
//-----------------------------------------------------------------------------------
    cerr << "Build branch from contact : " << ct << endl;
//-----------------------------------------------------------------------------------
    for ( Component* cp : ct->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(cp)) ? 1 : 0;
    }

    if ( count == 0 ) {
      cerr << "Something is not right here : Contact " << ct << " is isolated ?" << endl;
    }
    else if ( count == 2 ) {
      Segment* sm = nullptr;
      for ( Component* cp : ct->getSlaveComponents() ) {
        sm = dynamic_cast<Segment*>(cp);      
        if ( not sm ) continue;
      }

      Contact* cct = dynamic_cast<Contact*>(sm->getOppositeAnchor(ct));
//------------------------------------------------------------------------------------
      cerr << "To contact : " << cct << endl;
//------------------------------------------------------------------------------------
      if ( not cct || find(checker.begin(), checker.end(), cct) != checker.end() ) {
        cerr << "This branch leads to no where ?" << endl;
      }
      else {
        build_branch(ctr, cct);
      }
    }
    else {
      ctr = ct;
//-------------------------------------------------------------------
      cerr << "Return contact : " << ctr << endl;
//-------------------------------------------------------------------
    }
  }

/*
  Contact* Elmore::build_branch ( Contact* ct ) {
    int count = 0;
    checker.insert(ct);
//-----------------------------------------------------------------------------------
    cerr << "Build branch from contact : " << ct << endl;
//-----------------------------------------------------------------------------------
    for ( Component* cp : ct->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(cp)) ? 1 : 0;
    }

    Contact* cont = nullptr;
    if ( count == 0 ) {
      cerr << "Something is not right here : Contact " << ct << " is isolated ?" << endl;
    }
    else if ( count == 2 ) {
      Segment* sm = nullptr;
      for ( Component* cp : ct->getSlaveComponents() ) {
        sm = dynamic_cast<Segment*>(cp);      
        if ( not sm ) continue;
      }

      Contact* cct = dynamic_cast<Contact*>(sm->getOppositeAnchor(ct));
//------------------------------------------------------------------------------------
      cerr << "To contact : " << cct << endl;
//------------------------------------------------------------------------------------
      if ( not cct || find(checker.begin(), checker.end(), cct) != checker.end() ) {
        cerr << "This branch leads to no where ?" << endl;
      }
      else {
        build_branch(cct);
      }
    }
    else {
      cont = ct;
//-------------------------------------------------------------------
      cerr << "Return contact : " << cont << endl;
//-------------------------------------------------------------------
    }
//-------------------------------------------------------------------
    cerr << "After the else : " << cont << endl;
    cerr << "Is this line printed twice ?" << endl;
//-------------------------------------------------------------------
    return cont;
  }
*/

  Contact* Elmore::build_branch ( Contact* ct ) {
    Contact* tmp = ct;

    int count;
    do {
      checker.insert(tmp);
      count = 0;
      for ( Component* cp : tmp->getSlaveComponents() ) {
        count += (dynamic_cast<Segment*>(cp)) ? 1 : 0;
      }
      if ( count == 0 ) {
        cerr << "Something is not right here : Contact " << tmp << " is isolated ?" << endl;
        break;
      }
      else if ( count != 2 ) {
        break;
      }
      else {
        Segment* sm = nullptr;
        for ( Component* cp : tmp->getSlaveComponents() ) {
          sm = dynamic_cast<Segment*>(cp);
          if ( not sm ) continue;
        }
        
        Contact* cct = dynamic_cast<Contact*>(sm->getOppositeAnchor(ct));
        if ( not cct || checker.count(cct) != 0 ) {
          cerr << "This branch leads to no where ?" << endl;
          tmp = nullptr;
          break;
        }
        else
          tmp = cct;
      }
    } while ( count == 2 );

    return tmp;
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
