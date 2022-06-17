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
        
      if ( cont ) {
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
  }

  void Elmore::build_from_Node ( Node* s, Segment* seg ) {
    if ( s->_contact == nullptr ) {
      cerr << "No contact found" << s->_contact << endl;
      return;
    }
    _tree->add_node(s);
//----------------------------------------------------------------------------------------
    cerr << endl << endl << "Build from contact : " << s->_contact << endl;
    cerr << "With segment : " << seg << endl;
//----------------------------------------------------------------------------------------    
    Contact* ccont = dynamic_cast<Contact*>(seg->getOppositeAnchor(s->_contact));
    if ( not ccont || (s->Np && ccont == (s->Np)->_contact) )
      return;
//----------------------------------------------------------------------------------------
    cerr << "Target contact : " << ccont << endl;
//----------------------------------------------------------------------------------------
    ccont = build_branch(ccont);
//----------------------------------------------------------------------------------------
    cerr << "Found a node : " << ccont << endl;
//----------------------------------------------------------------------------------------
    if ( not ccont ) {
      cerr << "This branch leads to a NULL contact ?" << endl;
      return;
    }

    Node* node = new Node(s, ccont);
    int count = 0;
    for ( Component* comp : ccont->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(comp)) ? 1 : 0;
    }
//-----------------------------------------------------------------------------------------
    cerr << "This contact has : " << count << " segments" << endl;
//-----------------------------------------------------------------------------------------
    if ( count == 1 ){
      _tree->add_node(node);
    }
    else if ( count > 2 ) {
      for ( Component* comp : ccont->getSlaveComponents() ) {
        Segment* segmt = dynamic_cast<Segment*>(comp);
        if ( not segmt ) 
          continue;
//----------------------------------------------------------------------
        cerr << "Segment : " << segmt << endl;
//----------------------------------------------------------------------
        Contact* target = dynamic_cast<Contact*>(segmt->getOppositeAnchor(ccont));
        if ( not target ) {
          cerr << "Wait... How can this happen ?" << endl;
          continue;
        }
//----------------------------------------------------------------------
        cerr << "Target is : " << target << endl;
//----------------------------------------------------------------------
        if ( checker.count(target) == 0 ){
          build_from_Node(node, segmt);
          cerr << endl;
        }
      }
    }
  }

  Contact* Elmore::build_branch ( Contact* ct ) {
    Contact* tmp = ct;
//------------------------------------------------------
    cerr << endl << "Start building branch with contact : " << ct << endl;
//------------------------------------------------------
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
        Contact* cct = nullptr;
        for ( Component* cp : tmp->getSlaveComponents() ) {
          sm = dynamic_cast<Segment*>(cp);
          if ( not sm ) 
            continue;
//-----------------------------------------------------------------------------------------------
        cerr << "Sm : " << sm << endl;
//----------------------------------------------------------------------------------------------- 
          Contact* tar = dynamic_cast<Contact*>(sm->getOppositeAnchor(tmp));
//-----------------------------------------------------------------------------------------------
        cerr << "tar : " << tar << endl;
//----------------------------------------------------------------------------------------------- 
          if ( tar && checker.count(tar) == 0 )
            cct = tar;
        }

//-----------------------------------------------------------------------------------------------
        cerr << "cct : " << cct << endl;
//-----------------------------------------------------------------------------------------------
        if ( not cct || checker.count(cct) != 0 ) {
          cerr << "This branch leads to no where ?" << endl;
          tmp = nullptr;
          break;
        }
        else
          tmp = cct;
      }
    } while ( count == 2 );
//----------------------------------------------------------------------------------------------
    cerr << "Branch done !" << endl;
//----------------------------------------------------------------------------------------------
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
