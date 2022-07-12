#include "Seabreeze/Seabreeze.h"
#include "hurricane/Net.h"
#include "hurricane/Segment.h"
#include "hurricane/DebugSession.h"

namespace Seabreeze {

  using namespace std;
  using Hurricane::DBo;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Cell;
  using Hurricane::Instance;
  using Hurricane::PrivateProperty;
  using Hurricane::Component;
  using Hurricane::Segment;
  using Hurricane::DebugSession;

//---------------------------------------------------------
// Class : "Elmore"

  Elmore::Elmore ( Net* net )
    : _config (new Configuration())
    , _conts  ()
    , checker ()
    , _tree   (new Tree())
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

    cerr << "Root contact : " << ct->getId() << endl;
    cerr << "Start building tree..." << endl;

    checker.insert(ct);
    Node* s = new Node(nullptr, ct);
//---------------------------------------------------------
    double R = 0;
    double C = 0;
    Set_RC(&R, &C, ct, nullptr);
    s->R = R;
    if ( C == 0 )
      s->C = 0;
    else 
      s->C = 1/C;
//---------------------------------------------------------   
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

  void Elmore::build_from_Node ( Node* s, Segment* seg ) 
  {
    if ( s->_contact == nullptr ) {
      cerr << "No contact found" << s->_contact << endl;
      return;
    }
    _tree->add_node(s);

    cdebug_log(199,0) << endl 
                      << endl 
                      << "Build from contact : " << s->_contact->getId() << endl
                      << "With segment : " << seg->getId() << endl;
    
    Contact* ccont = dynamic_cast<Contact*>(seg->getOppositeAnchor(s->_contact));
    if ( not ccont || (s->Np && ccont == (s->Np)->_contact) )
      return;

    cdebug_log(199,0) << "Target contact : " << ccont->getId() << endl;
//-----------------------------------------------------------------------
    double Rb = 0;
    double Cb = 0;
//-----------------------------------------------------------------------
    ccont = build_branch(&Rb, &Cb, ccont);

    cdebug_log(199, 0) << "Found a node : " << ccont->getId() << endl;

    if ( not ccont ) {
      cerr << "This branch leads to a NULL contact ?" << endl;
      return;
    }

    Node* node = new Node(s, ccont);
//-----------------------------------------------------------------------
    node->R = Rb;
    if ( Cb == 0 )
      node->C = 0;
    else 
      node->C = 1/Cb;

    cdebug_log(199,0) << "R = " << Rb << "; C = " << Cb << endl;
//-----------------------------------------------------------------------
    int count = 0;
    for ( Component* comp : ccont->getSlaveComponents() ) {
      count += (dynamic_cast<Segment*>(comp)) ? 1 : 0;
    }

    cdebug_log(199,0) << "This node's contact has : " << count << " segments" << endl;

    if ( count == 1 ){
      _tree->add_node(node);
    }
    else if ( count > 2 ) {
      for ( Component* comp : ccont->getSlaveComponents() ) {
        Segment* segmt = dynamic_cast<Segment*>(comp);
        if ( not segmt ) 
          continue;

        cdebug_log(199,1) << "Segment : " << segmt->getId() << endl;
        cdebug_tabw(199,-1);

        Contact* target = dynamic_cast<Contact*>(segmt->getOppositeAnchor(ccont));
        if ( not target ) {
          cerr << "Wait... How can this happen ?" << endl;
          continue;
        }

        cdebug_log(199,0) << "Target is : " << target->getId() << endl;
        cdebug_tabw(199,-1);

        if ( checker.count(target) == 0 ){
          build_from_Node(node, segmt);
          cdebug_log(199,0) << endl;
        }
      }
    }
  }

  Contact* Elmore::build_branch ( double* R, double* C, Contact* ct ) 
  {
    Contact* tmp = ct;

    cdebug_log(199,1) << endl << "Start building branch with contact : " << ct->getId() << endl;

    int count;
    do {
      checker.insert(tmp);
      count = 0;
      Segment* sm = nullptr; 
      for ( Component* cp : tmp->getSlaveComponents() ) {
        sm = dynamic_cast<Segment*>(cp);
        if ( not sm ) 
          continue;

        Contact* tar = dynamic_cast<Contact*>(sm->getOppositeAnchor(tmp));
        if ( tar && checker.count(tar) != 0 )   {
          Set_RC(R, C, tmp, sm);
          cdebug_log(199,0) << "RC from build_branch :" << endl;
          cdebug_log(199,0) << "tmp = " << tmp->getId() << "; sm = " << sm->getId() << "; R = " << *R << "; C = " << *C << endl;
        }
        count += 1;
      }
      if ( count == 0 ) {
        cerr << "Something is not right here : Contact " << tmp << " is isolated ?" << endl;
        break;
      }
      else if ( count != 2 ) {
        break;
      }
      else {
        Contact* cct = nullptr;
        for ( Component* cp : tmp->getSlaveComponents() ) {
          sm = dynamic_cast<Segment*>(cp);
          if ( not sm ) 
            continue;

          cdebug_log(199,1) << "Sm  : " << sm->getId() << endl;
          cdebug_tabw(199,-1);

          Contact* tar = dynamic_cast<Contact*>(sm->getOppositeAnchor(tmp));

          cdebug_log(199,0) << "tar : " << tar->getId() << endl << endl;
 
          if ( tar && checker.count(tar) == 0 )
            cct = tar;
        }

        cdebug_tabw(199,-1);
        cdebug_log(199,0) << "cct : " << cct->getId() << endl;

        if ( not cct || checker.count(cct) != 0 ) {
          cerr << "This branch leads to no where ?" << endl;
          tmp = nullptr;
          break;
        }
        else
          tmp = cct;
      }
    } while ( count == 2 );

    cdebug_tabw(199,-1);
    cdebug_log(199,0) << "Branch done !" << endl;

    return tmp;
  }

  void Elmore::Set_RC ( double* R, double* C, Contact* ct, Segment* sm ) {
    double Rct = getConfig()->getRct();
    //double h_ct = DbU::toPhysical(ct->getHeight(), DbU::UnitPower::Nano);
    double w_ct = DbU::toPhysical(ct->getWidth(), DbU::UnitPower::Nano);

    double S_ct = w_ct*w_ct;
    (*R) += Rct*S_ct;

    if ( sm == nullptr ) {
      *C = 0;
    }
    else {
      double Rsm = getConfig()->getRsm();
      double Csm = getConfig()->getCsm();
      double l_sm = DbU::toPhysical(sm->getLength(), DbU::UnitPower::Nano);
      double w_sm = DbU::toPhysical(sm->getWidth(), DbU::UnitPower::Nano);
      double S_sm = l_sm*w_sm;
//---------------------------------------------------------------------------------------
      cdebug_log(199,0) << "sm = " << sm->getId() <<  "; l_sm = " << l_sm << "; w_sm = " << w_sm << "; S_sm = " << S_sm << endl;
//---------------------------------------------------------------------------------------
      (*R) += Rsm*S_sm;
      if ( S_sm == 0 )
        (*C) += 0;
      else
        (*C) += 1/(Csm*S_sm);
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
