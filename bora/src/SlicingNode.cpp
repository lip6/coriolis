// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./SlicingNode.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
#include "hurricane/DeepNet.h"
#include "hurricane/RoutingPad.h"
#include "crlcore/RoutingGauge.h"
#include "bora/SlicingNode.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Point;
  using Hurricane::Box;
  using Hurricane::DeepNet;
  using Hurricane::Component;
  using Hurricane::Plug;
  using Hurricane::RoutingPad;
  using Analog::AnalogCellExtension;


// -------------------------------------------------------------------
// Class  :  "Bora::SlicingNode".


  CRL::RoutingGauge*    SlicingNode::_rg           = NULL;
  Cell*                 SlicingNode::_cell         = NULL;
  Katana::KatanaEngine* SlicingNode::_katana       = NULL;
  vector<Segment*>      SlicingNode::_railSegments = vector<Segment*>();
  size_t                SlicingNode::_idCounter    = 0;

 
  SlicingNode::SlicingNode ( unsigned int type
                           , NodeSets*    nodeSets
                           , unsigned int alignment    
                           , BoxSet*      boxSet
                           )
    : Super()
    , _id      (_idCounter++)
    , _parent  (NULL)
    , _flags   (0)
    , _nodeSets(nodeSets)
    , _x       (0)
    , _y       (0)
    , _boxSet  (NULL)
    , _gcell   (NULL)
    , _master  (NULL)
  {

    setType     (type);
    setAlignment(alignment);
    
    if (boxSet != NULL) { 
      setBoxSet(boxSet);
      setPreset(Preset); 
      setSet   (Set);
    }

  //cdebug_log(536,0) << "SlicingNode::SlicingNode(unsigned,NodeSet*,unsigned,BoxSet*) - " << this << endl;
  } 

 
  SlicingNode::SlicingNode ( unsigned int type )
    : Super()
    , _parent  (NULL)
    , _flags   (0)
    , _nodeSets(NodeSets::create( NULL, NULL))
    , _x       (0)
    , _y       (0)
    , _boxSet  (NULL)
    , _gcell   (NULL)
    , _master  (NULL)
  {
    setType     (type);
    setAlignment(UnknownAlignment);

  //cdebug_log(536,1) << "SlicingNode::SlicingNode(unsigned) - " << this << endl;
  } 


  SlicingNode::~SlicingNode ()
  { }

  
  bool  SlicingNode::isSameSize ( BoxSet* boxSet ) const 
  {
    if ( _boxSet and boxSet ) {
      if (   (_boxSet->getHeight() == boxSet->getHeight())
         and (_boxSet->getWidth () == boxSet->getWidth ()) )
        return true;
    }
    return false;
  }

  
  bool  SlicingNode::isSameSize ( DbU::Unit height, DbU::Unit width ) const 
  {
    if (_boxSet) {
      if ( (_boxSet->getHeight() == height) and (_boxSet->getWidth() == width) )
        return true;
    }
    return false;
  }
         

  bool  SlicingNode::isEmpty () const
  { return _nodeSets->empty(); }


  bool  SlicingNode::isSame ( SlicingNode* , unsigned int ) const
  {
    cerr << Error( "SlicingNode::isSame(): Base class method must never be called." ) << endl;
    return false;
  }


  bool  SlicingNode::isSymmetric ( SlicingNode* , unsigned int , unsigned int ) const
  {
    cerr << Error( "SlicingNode::isSymmetric(): Base class method must never be called." ) << endl;
    return false;
  }


  DbU::Unit  SlicingNode::getHeight () const 
  { return (_boxSet) ? _boxSet->getHeight() : 0; }


  DbU::Unit  SlicingNode::getWidth () const 
  { return (_boxSet) ? _boxSet->getWidth() : 0; }


  DbU::Unit  SlicingNode::getRootHeight () const 
  { return getRoot()->getHeight(); }


  DbU::Unit  SlicingNode::getRootWidth () const 
  { return getRoot()->getWidth(); }


  DbU::Unit  SlicingNode::getRootX () const 
  { return getRoot()->getX(); }


  DbU::Unit  SlicingNode::getRootY () const 
  { return getRoot()->getY(); }


  int  SlicingNode::getCpt ()
  { return (_boxSet) ? _boxSet->getCpt() : 0 ; }


  double  SlicingNode::getRatio ()
  { return (_boxSet) ? _boxSet->getRatio() : 0 ; }


  double  SlicingNode::getArea ()
  { return (_boxSet) ? _boxSet->getArea() : 0 ; }


  DbU::Unit  SlicingNode::getVPitch () 
  {
    if (_rg) return _rg->getVerticalPitch();

    cerr << Error( "SlicingNode::getVPitch(): RoutingGauge has not been defined." )  << endl;
    return 0;
  }
     

  DbU::Unit  SlicingNode::getHPitch () 
  {
    if (_rg) return _rg->getHorizontalPitch();

    cerr << Error( "SlicingNode::getHPitch(): RoutingGauge has not been defined." )  << endl;
    return 0;
  }


  void  SlicingNode::setBoxSet( BoxSet* boxSet )
  {
    if (not isPreset()) {
      if (_nodeSets->find(boxSet) != NULL) { 
        setSet( Set );
        _boxSet = boxSet; 
      } else {
        cerr << Warning( "SlicingNode::setBoxSet(BoxSet*): Boxset not found." ) << endl;
      }
    }
  }


  void  SlicingNode::setBoxSet ( DbU::Unit height, DbU::Unit width )
  {
    if (not isPreset()) {
      BoxSet* match = _nodeSets->find( height, width );
      if (match) { 
        setSet( Set );
        _boxSet = match;
      } else {
        cerr << Warning( "SlicingNode::setBoxSet(DbU::Unit w,DbU::Unit h): Boxset not found." ) << endl; 
      }
    } 
  }


  void  SlicingNode::setBoxSet ( size_t index )
  {
    if (index+1 > _nodeSets->size())
      cerr << Warning( "SlicingNode::setBoxSet(size_t): Out of bound index." ) << endl;
    else
      _boxSet = _nodeSets->at( index );
  }


  void  SlicingNode::_setBoxSet ( BoxSet* boxSet )
  {
    if (isPreset()) return;

    setSet( Set );
    _boxSet = boxSet; 
  }


  void  SlicingNode::setX ( DbU::Unit x )
  {
    float pitch  = _rg->getVerticalPitch();
  
    if (fmod(x,pitch) > 1e-06) {
      cerr << Warning( "SlicingNode::setX(): On %s, X position %s is not pitched on %s (will be adjusted)."
                     , getString(this).c_str()
                     , DbU::getValueString(x    ).c_str()
                     , DbU::getValueString(pitch).c_str()
                     ) << endl;
    }

    _x = x;
  }


  void  SlicingNode::setY ( DbU::Unit y )
  {     
    float pitch = _rg->getHorizontalPitch();

    if (fmod(y,pitch) > 1e-06) {
      cerr << Warning( "SlicingNode::setY(): On %s, Y position %s is not pitched on %s (will be adjusted)."
                     , getString(this).c_str()
                     , DbU::getValueString(y    ).c_str()
                     , DbU::getValueString(pitch).c_str()
                     ) << endl;
    }

    _y = y;
  }

  
  void  SlicingNode::clearParentsNodeSets ()
  {
    if (_parent)     _parent->clearParentsNodeSets();
    if (getMaster()) _nodeSets->clear();
  }


  void  SlicingNode::resetSlicingTree()
  {
  // Notes: Reset a slicing tree means:
  // - Empty NodeSets of every parent
  // - Unset every nodes that are not preset
  // - Unplace every nodes

    clearParentsNodeSets();
    getRoot()->_resetSlicingTree();
  }


  void  SlicingNode::_resetSlicingTree ()
  {
    _x = 0;
    _y = 0;
    setPlaced( false );
    
    if (not isPreset()){
      _boxSet = NULL;
      setSet( false );
    }
  }


  void  SlicingNode::print () const
  {
    printParent();
    if   (isPreset()) cerr << "Preset     : True"   << endl;
    else              cerr << "Preset     : False " << endl;
    if   (isSet())    cerr << "Set        : True"   << endl;
    else              cerr << "Set        : False " << endl;
    if   (isPlaced()) cerr << "Placed     : True"   << endl;
    else              cerr << "Placed     : False " << endl;
    if (getMaster()) {
      cerr << "Master     : "; 
      getMaster()->printLine();
    }

    unsigned int type = getType();
    if      (type == HorizontalSNode) cerr << "Type       : HorizontalSNode"<< endl;
    else if (type == VerticalSNode  ) cerr << "Type       : VerticalSNode"  << endl;
    else if (type == DeviceSNode    ) cerr << "Type       : DeviceSNode"    << endl;
    else if (type == RoutingSNode   ) cerr << "Type       : RoutingSNode"   << endl;
    else                              cerr << "Type       : UnknownType"    << endl;

    cerr << "Height     : " << DbU::getPhysical(getHeight(),DbU::Micro) << endl;
    cerr << "Width      : " << DbU::getPhysical(getWidth(),DbU::Micro) << endl;
    cerr << "X          : " << DbU::getPhysical(_x,DbU::Micro) << endl;
    cerr << "Y          : " << DbU::getPhysical(_y,DbU::Micro) << endl;
    if (isRoutingEstimated()) cerr << "REstimated : True"  << endl;
    else                      cerr << "REstimated : False" << endl;

  //_nodeSets->print();
    cerr << "GCell      : " << _gcell << endl;
    if (not ((getType() == HorizontalSNode) or (getType() == VerticalSNode))) {
      cerr << "GCell      : " << _gcell << endl;
      if (_gcell) {
        cerr << "Edges : " << endl;
        vector<Anabatic::Edge*> ne = _gcell->getNorthEdges();
        vector<Anabatic::Edge*> se = _gcell->getSouthEdges();
        vector<Anabatic::Edge*> ee = _gcell->getEastEdges();
        vector<Anabatic::Edge*> we = _gcell->getWestEdges();

        cerr << "--- North : " << endl;
        for ( Anabatic::Edge* edge : ne ) cerr << edge->getOpposite(_gcell) << endl;
        cerr << "--- South : " << endl;
        for ( Anabatic::Edge* edge : se ) cerr << edge->getOpposite(_gcell) << endl;
        cerr << "--- East : " << endl;
        for ( Anabatic::Edge* edge : ee ) cerr << edge->getOpposite(_gcell) << endl;
        cerr << "--- West : " << endl;
        for ( Anabatic::Edge* edge : we ) cerr << edge->getOpposite(_gcell) << endl;
      }
    }
    cerr << endl;
  }


  void  SlicingNode::printLine() const 
  {
    unsigned int type = getType();
    if (_master) {
      cerr << " [MASTER]: ";
      _master->printLine(); 
    }

    if      (type == HorizontalSNode) cerr << "Type       : HorizontalSNode";
    else if (type == VerticalSNode  ) cerr << "Type       : VerticalSNode"  ;
    else if (type == DeviceSNode    ) cerr << "Type       : DeviceSNode"    ;
    else if (type == RoutingSNode   ) cerr << "Type       : RoutingSNode"   ;
    else                              cerr << "Type       : UnknownType"    ;
    cerr << ", X: " << DbU::getPhysical(getX(),DbU::Micro);
    cerr << ", Y: " << DbU::getPhysical(getY(),DbU::Micro);
    cerr << ", W: " << setprecision(4) << DbU::getPhysical(getWidth() ,DbU::Micro);
    cerr << ", H: " << setprecision(4) << DbU::getPhysical(getHeight(),DbU::Micro) << endl;
  }


  void  SlicingNode::printParent() const 
  {
    if (_parent) {
      unsigned int type = _parent->getType();
      if      (type == HorizontalSNode) cerr << "Parent     : Type: HorizontalSNode";
      else if (type == VerticalSNode  ) cerr << "Parent     : Type: VerticalSNode";
      else                              cerr << "Parent     : Type: UnknownType";
      cerr << ", H: " << setprecision(4) << DbU::getPhysical(_parent->getHeight(),DbU::Micro);
      cerr << ", W: " << setprecision(4) << DbU::getPhysical(_parent->getWidth(),DbU::Micro);
      cerr << ", X: " << DbU::getPhysical(_parent->getX(),DbU::Micro);
      cerr << ", Y: " << DbU::getPhysical(_parent->getY(),DbU::Micro) <<  endl;
    } else {
      cerr << "Parent     : IsROOT " << endl;
    }
}


  string  SlicingNode::_getString () const
  {
    ostringstream os;
    os << "<" << _getTypeName();
    os << " id:" << getId() << " gid:";
    if (getGCell()) os << getGCell()->getId();
    else            os << "-";
    os << " choices:" << getNodeSets()->getBoxSets().size();
    os << ">";
    return os.str();
  }


  string  SlicingNode::_getTypeName () const
  { return "SlicingNode"; }


  void  SlicingNode::printBoxSet () const
  { if (_boxSet) _boxSet->print(); }


  void  SlicingNode::place ( DbU::Unit x, DbU::Unit y )
  {
    cdebug_log(536,1) << "SlicingNode::place(DbU::Unit x, DbU::Unit y)" << endl;

    if (isSet()) {
      _x = x;
      _y = y;
      setPlaced( Placed );

      DbU::Unit hpitch = _rg->getHorizontalPitch();
      DbU::Unit vpitch = _rg->getVerticalPitch();

      if (_x % vpitch)
        cerr << Warning( "SlicingNode::place(): On %s, _x is not pitched (%s, pitch:%s)."
                       , getString(this).c_str()
                       , DbU::getValueString(_x).c_str()
                       , DbU::getValueString(vpitch).c_str()
                       ) << endl;

      if (_y % hpitch)
        cerr << Warning( "SlicingNode::place(): On %s, _y is not pitched (%s, pitch:%s)."
                       , getString(this).c_str()
                       , DbU::getValueString(_y).c_str()
                       , DbU::getValueString(hpitch).c_str()
                       ) << endl;
    } else
      cerr << Error( "SlicingNode::place(DbU::Unit x, DbU::Unit y): Device has not been set." ) << endl;

    cdebug_tabw(536,-1);
  }


  void  SlicingNode::replace ( DbU::Unit x, DbU::Unit y )
  {
    cdebug_log(536,1) << "SlicingNode::replace(DbU::Unit x, DbU::Unit y)" << endl;

    if (isSet()) {
      _x = x;
      _y = y;
      setPlaced( Placed );
    } else
      cerr << Error( "SlicingNode::replace(DbU::Unit x, DbU::Unit y): Device has not been set." ) << endl;
    
    cdebug_tabw(536,-1);
  }


  void  SlicingNode::_place ( DbU::Unit x, DbU::Unit y, bool replace )
  {
    cdebug_log(536,1) << "SlicingNode::place(DbU::Unit x, DbU::Unit y)" << endl;
    SlicingNode::place(x,y);
    cdebug_tabw(536,-1);
  }


  void  SlicingNode::setGlobalSize ( DbU::Unit height, DbU::Unit width )
  {
    cmess2 << "  o  Selecting box set (" << DbU::getValueString(height)
           << "," << DbU::getValueString(width) << ")" << endl;
    setBoxSet( height, width ); 
  }


  void  SlicingNode::setGlobalSize ( size_t index )
  {
    cmess2 << "  o  Selecting box set at index " << index << endl;
    setBoxSet( index ); 
  }


  void  SlicingNode::_setGlobalSize ( BoxSet* boxSet ) 
  {
    cdebug_log(535,1) << "SlicingNode::_setGlobalSize( BoxSet* boxSet ) " << endl;
    _setBoxSet( boxSet );
    cdebug_log(535,-1);
  }

  
  void  SlicingNode::_setGlobalSize ( DbU::Unit, DbU::Unit )
  {
    cerr << Error( "SlicingNode::_setGlobalSize(): Must not be called on \"%s\"." , _getTypeName().c_str() ) << endl;
  }


  void  SlicingNode::preDestroy ()
  {
    if (_parent) _parent->removeNode(this);
  //else {
  //  if (_railCell) _railCell->destroy();
  //}
    if (getMaster()) _nodeSets->clear();
  }


  void  SlicingNode::destroy()
  {
    SlicingNode::preDestroy();
    delete this;
  }


  void  SlicingNode::preRecursiveDestroy()
  {
    SlicingNode::preDestroy();
  }


  void  SlicingNode::recursiveDestroy ()
  {
    SlicingNode::preRecursiveDestroy();
    delete this;
  }


  LSlicingNodes  SlicingNode::getLeaves ()
  {
    return LSlicingNodes();
  }


  bool  SlicingNode::checkInitialPlacement ( int& cpt ) const 
  {
  // Notes:: Initial placement criteria is having at least 2 devices placed at (x, y) = (0, 0)

    bool initialPlacement = false;
    if (cpt < 2) initialPlacement = true;
    return initialPlacement;
  }


  void  SlicingNode::updateCellAbutmentBox ()
  {
    Point pmin = Point( getX(), getY() );
    Point pmax = Point( getX()+getWidth(), getY()+getHeight() );
    _cell->setAbutmentBox( Box(pmin,pmax) );
  }


  void  SlicingNode::setGCell ( Anabatic::GCell* gcell )
  {
    _gcell = gcell;
    cdebug_log(535,0) << "SlicingNode::setGCell() " << _gcell << endl;
  }


  void  SlicingNode::setCell ( Cell* cell )
  {
    if (not _parent) {
      if (checkCellInstances(cell)) {
        _cell = cell;
        AnalogCellExtension::set( cell, this );
      } 
    } else
      cerr << Warning( "SlicingNode::setCell(Cell*): Slicing tree's cell can only be set at root node." ) << endl;
  }


  void  SlicingNode::setKatana ( Katana::KatanaEngine* katana )
  { _katana = katana; }


  bool SlicingNode::checkCellInstances ( Cell* cell )
  { return true; }


  SlicingNode* SlicingNode::findInstance ( Instance* instance )
  { return NULL; }


  SlicingNode* SlicingNode::findSlicingNode ( Anabatic::GCell* gcell )
  { return NULL; }


  SlicingNode* SlicingNode::findHorizontalParent () const
  {
    SlicingNode* node = _parent;
    while ( node ) {
      if (node->getType() == HorizontalSNode) break;
      node = node->getParent();
    }
    return node;
  }


  SlicingNode* SlicingNode::findVerticalParent () const
  {
    SlicingNode* node = _parent;
    while ( node ) {
      if (node->getType() == VerticalSNode) break;
      node = node->getParent();
    }
    return node;
  }


  size_t  SlicingNode::getIndexInHorizontalParent () const
  {
    if (getParent()) { 
      if (getParent()->getType() == HorizontalSNode) {
        const vector<SlicingNode*>& children = getParent()->getChildren();

        size_t i = 0;
        for ( ; i<children.size() ; ++i ) {
          if (   (children[i]->getType()   == getType()  )
             and (children[i]->getHeight() == getHeight()) 
             and (children[i]->getWidth () == getWidth ()) 
             and (children[i]->getX()      == getX()     ) 
             and (children[i]->getY()      == getY()     ) )
            return i;
        }
      } else {
        SlicingNode* node = getParent();

        while ( node->getParent() ) {
          if (node->getParent()->getType() == HorizontalSNode)
            return node->getParent()->getChildIndex(node);

          node = node->getParent();
        }
      }
    }
    return NodeSets::NotFound;
  }


  size_t  SlicingNode::getIndexInVerticalParent () const
  {
    if (getParent()) { 
      if (getParent()->getType() == VerticalSNode) {
        const vector<SlicingNode*>& children = getParent()->getChildren();

        size_t i = 0;
        for ( ; i<children.size() ; ++i ) {
          if (   (children[i]->getType()   == getType()  )
             and (children[i]->getHeight() == getHeight()) 
             and (children[i]->getWidth () == getWidth ()) 
             and (children[i]->getX()      == getX()     ) 
             and (children[i]->getY()      == getY()     ) )
            return i;
        }
      } else {
        SlicingNode* node = getParent();

        while ( node->getParent() ) {
          if (node->getParent()->getType() == VerticalSNode)
            return node->getParent()->getChildIndex(node);
          
          node = node->getParent();
        }
      }
    }
    return NodeSets::NotFound;
  }


  void  SlicingNode::createRoutingPads ()
  {
    cdebug_log(536,1) << "SlicingNode::createRoutingPads()" << endl;

    if (not _cell) {
      cdebug_tabw(536,-1);
      return;
    }

  //_cell->flattenNets( Cell::Flags::StayOnPlugs );
    _cell->flattenNets();

    cdebug_tabw(536,-1);
  }


  void  SlicingNode::clearRoutingPads ()
  {
    vector<DeepNet*> nets;
  
    for ( Net* net : _cell->getNets() ) {
      DeepNet* n = dynamic_cast<DeepNet*>(net);
      if (n) nets.push_back(n);
    }
    for ( DeepNet* net : nets ){
      net->destroy();
    }
    for ( Net* net : _cell->getNets() ) {
      vector<RoutingPad*> rps;
      for ( RoutingPad* rp : net->getRoutingPads() ) rps.push_back( rp );
      for ( RoutingPad* rp : rps ) rp->destroy();
    }

    _cell->getFlags().reset( Cell::Flags::FlattenedNets|Cell::Flags::Routed );
    _cell->setTerminalNetlist( false );
  } 


  void  SlicingNode::restrictDevices ()
  {
    if (_gcell) {
      Anabatic::Vertex* vertex
        = _gcell->getObserver<Anabatic::Vertex>( Anabatic::GCell::Observable::Vertex );
      vertex->clearRestriction();
    }
  }


  void SlicingNode::setVertexRestriction ( Net* , Katana::KatanaEngine* )
  { }


  void  SlicingNode::estimateChannelsSize ()
  { setRoutingEstimated( RoutingEstimated ); }


  void  SlicingNode::expandRoutingChannel () 
  { setRoutingEstimated( RoutingEstimated ); }


  void  SlicingNode::_expandRoutingChannel () 
  { setRoutingEstimated( RoutingEstimated ); }


  void  SlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width ) 
  { setRoutingEstimated( RoutingEstimated ); }


  void  SlicingNode::clearGlobalRouting ()
  {
    vector<Component*> segments;
    vector<Component*> contacts;
    
    for ( Net* net: _cell->getNets() ) {
      for ( Component* component : net->getComponents() ) {
        if (not SlicingNode::isRailSegments(dynamic_cast<Segment*>(component))) {
          Hurricane::Horizontal* h = dynamic_cast<Hurricane::Horizontal*>(component);
          Hurricane::Vertical*   v = dynamic_cast<Hurricane::Vertical*>  (component);
          Hurricane::Contact*    c = dynamic_cast<Hurricane::Contact*>   (component);
        
          if (h or v) segments.push_back(component);
          if (c)      contacts.push_back(component);
        }
      }
    }

    for ( Component* component : segments ) component->destroy();
    for ( Component* component : contacts ) component->destroy();
    clearGCells();
  } 


  void  SlicingNode::clearGCells ()
  { _gcell = NULL; }


  void  SlicingNode::updateGCellPosition ()
  {
    if (_gcell) _gcell->setSouthWestCorner( getX(), getY() );

    cdebug_log(535,0) << "SlicingNode::updateGCellPosition(): " << _gcell << endl;
  }


  void  SlicingNode::updateGContacts()
  {
    if (_gcell) _gcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );

    cdebug_log(535,0) << "SlicingNode::updateGContacts(): " << _gcell << endl;
  }

  
  int  SlicingNode::getNbDevices()
  { return 1; } 


  bool  SlicingNode::containAxis ( DbU::Unit axis, unsigned int symmetryType )
  {
    if      (symmetryType == HSymmetry) return ( (axis > getX()) and (axis < getX()+getWidth ()));
    else if (symmetryType == VSymmetry) return ( (axis > getY()) and (axis < getY()+getHeight()));
    return false;
  }


  void  SlicingNode::setRoutingGauge ( CRL::RoutingGauge* rg )
  { _rg = rg; }


  void  SlicingNode::addRailSegments ( Segment* segment )
  { _railSegments.push_back(segment); }


  bool  SlicingNode::isRailSegments ( Segment* segment )
  {
    for ( Segment* rail : _railSegments ) {
      if (segment == rail) return true;
    }
    return false;
  }


  bool  SlicingNode::isRailSegments ( Plug* plug )
  {
    Instance* instance = plug->getInstance();

    if (instance->getMasterCell()->getNets().getSize() != 1) return false;
    Net* net = instance->getMasterCell()->getNets().getFirst();

    if (net->getComponents().getSize() != 1) return false;
    Segment* segment = dynamic_cast<Segment*>( net->getComponents().getFirst() );

    return segment and isRailSegments( segment );
  }

  
  void  SlicingNode::setBoxSetIndex ( size_t )
  { cerr << Error( "SlicingNode::setBoxSetIndex(): Base class method must never be called." ) << endl; }

  
  size_t  SlicingNode::getBoxSetIndex () const
  {
    cerr << Error( "SlicingNode::getBoxSetIndex(): Base class method must never be called." ) << endl;
    return 0;
  }

  
  Instance* SlicingNode::getInstance () const
  {
    cerr << Error( "SlicingNode::getInstance(): Base class method must never be called." ) << endl;
    return NULL;
  }

  
  void  SlicingNode::setHeight ( DbU::Unit )
  { cerr << Error( "SlicingNode::setHeight(): Base class method must never be called." ) << endl; }

  
  void  SlicingNode::setWidth ( DbU::Unit )
  { cerr << Error( "SlicingNode::setWidth(): Base class method must never be called." ) << endl; }

  
  void  SlicingNode::setInstance ( Instance* )
  { cerr << Error( "SlicingNode::setInstance(): Base class method must never be called." ) << endl; }


  void  SlicingNode::updateMatrixGCellPosition ()
  { cerr << Error( "SlicingNode::updateMatrixGCellPosition(): Base class method must never be called." ) << endl; }


  void  SlicingNode::updateMatrixGContacts ()
  { cerr << Error( "SlicingNode::updateMatrixGContacts(): Base class method must never be called." ) << endl; }


  void  SlicingNode::addWireOccupation ( DbU::Unit , DbU::Unit, Net* )
  { cerr << Error( "SlicingNode::addWireOccupation(): Base class method must never be called." ) << endl; }


  int SlicingNode::getMaxWireOccupation ()
  {
    cerr << Error( "SlicingNode::getMaxWireOccupation(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  DbU::Unit  SlicingNode::getToleranceRatioH () const
  {
    cerr << Error( "SlicingNode::getMaxWireOccupation(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  DbU::Unit  SlicingNode::getToleranceRatioW () const
  {
    cerr << Error( "SlicingNode::getMaxWireOccupation(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  void  SlicingNode::setToleranceRatioH ( DbU::Unit )
  { cerr << Error( "SlicingNode::setToleranceRatioH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::setToleranceRatioW ( DbU::Unit )
  { cerr << Error( "SlicingNode::setToleranceRatioW(): Base class method must never be called." ) << endl; }


  DbU::Unit  SlicingNode::getToleranceBandH () const
  {
    cerr << Error( "SlicingNode::getToleranceBandH(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  DbU::Unit  SlicingNode::getToleranceBandW () const
  {
    cerr << Error( "SlicingNode::getToleranceBandW(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  void  SlicingNode::setToleranceBandH ( DbU::Unit )
  { cerr << Error( "SlicingNode::setToleranceBandH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::setToleranceBandW ( DbU::Unit )
  { cerr << Error( "SlicingNode::setToleranceBandW(): Base class method must never be called." ) << endl; }


  void  SlicingNode::recursiveSetToleranceRatioH ( DbU::Unit )
  { cerr << Error( "SlicingNode::recursiveSetToleranceRatioH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::recursiveSetToleranceRatioW ( DbU::Unit )
  { cerr << Error( "SlicingNode::recursiveSetToleranceRatioW(): Base class method must never be called." ) << endl; }


  void  SlicingNode::recursiveSetToleranceBandH ( DbU::Unit )
  { cerr << Error( "SlicingNode::recursiveSetToleranceBandH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::recursiveSetToleranceBandW ( DbU::Unit )
  { cerr << Error( "SlicingNode::recursiveSetToleranceBandW(): Base class method must never be called." ) << endl; }


  void  SlicingNode::_recursiveSetToleranceRatioH ( DbU::Unit )
  { cerr << Error( "SlicingNode::_recursiveSetToleranceRatioH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::_recursiveSetToleranceRatioW ( DbU::Unit )
  { cerr << Error( "SlicingNode::_recursiveSetToleranceRatioW(): Base class method must never be called." ) << endl; }


  void  SlicingNode::_recursiveSetToleranceBandH ( DbU::Unit )
  { cerr << Error( "SlicingNode::_recursiveSetToleranceBandH(): Base class method must never be called." ) << endl; }


  void  SlicingNode::_recursiveSetToleranceBandW ( DbU::Unit )
  { cerr << Error( "SlicingNode::_recursiveSetToleranceBandW(): Base class method must never be called." ) << endl; }


  void  SlicingNode::_recursiveSetTolerances ( DbU::Unit , DbU::Unit , DbU::Unit , DbU::Unit )
  { cerr << Error( "SlicingNode::_recursiveSetTolerances(): Base class method must never be called." ) << endl; }


  void  SlicingNode::setTolerances ( DbU::Unit , DbU::Unit , DbU::Unit , DbU::Unit )
  { cerr << Error( "SlicingNode::setTolerances(): Base class method must never be called." ) << endl; }


  void  SlicingNode::recursiveSetTolerances ( DbU::Unit , DbU::Unit , DbU::Unit , DbU::Unit )
  { cerr << Error( "SlicingNode::recursiveSetTolerances(): Base class method must never be called." ) << endl; }

  
  bool  SlicingNode::hasEmptyChildrenNodeSets () const              
  {
    cerr << Error( "SlicingNode::(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  const  VSlicingNodes& SlicingNode::getChildren () const              
  {
    static VSlicingNodes empty;
    cerr << Error( "SlicingNode::getChildren(): Base class method must never be called." ) << endl;
    return empty;
  }
  

  SlicingNode* SlicingNode::getChild ( size_t ) const      
  {
    cerr << Error( "SlicingNode::getChild(): Base class method must never be called." ) << endl;
    return NULL;
  }
  

  size_t  SlicingNode::getChildIndex ( SlicingNode* ) const
  {
    cerr << Error( "SlicingNode::getChildIndex(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  size_t  SlicingNode::getNbChild () const              
  {
    cerr << Error( "SlicingNode::getNbChild(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  void  SlicingNode::createChild ( unsigned int type, unsigned int alignment )
  { cerr << Error( "SlicingNode::createChild(): Base class method must never be called." ) << endl; }


  void  SlicingNode::createChild ( NodeSets*, unsigned int, Instance*, BoxSet* )
  { cerr << Error( "SlicingNode::createChild(): Base class method must never be called." ) << endl; }


  void  SlicingNode::createChild ( size_t childIndex, size_t copyIndex, unsigned int tr ) 
  { cerr << Error( "SlicingNode::createChild(): Base class method must never be called." ) << endl; }


  void  SlicingNode::createRouting ( DbU::Unit )
  { cerr << Error( "SlicingNode::createRouting(): Base class method must never be called." ) << endl; }


  void  SlicingNode::insertNode ( SlicingNode* , size_t index ) 
  { cerr << Error( "SlicingNode::insertNode(): Base class method must never be called." ) << endl; }


  void  SlicingNode::push_back ( SlicingNode* , bool reset ) 
  { cerr << Error( "SlicingNode::push_back(): Base class method must never be called." ) << endl; }


  void  SlicingNode::push_front ( SlicingNode* ) 
  { cerr << Error( "SlicingNode::push_front(): Base class method must never be called." ) << endl; }


  void  SlicingNode::removeNode ( SlicingNode* )
  { cerr << Error( "SlicingNode::removeNode(): Base class method must never be called." ) << endl; }


  void  SlicingNode::removeAllNodes ()     
  { cerr << Error( "SlicingNode::removeAllNodes(): Base class method must never be called." ) << endl; }


  const  LSymmetries& SlicingNode::getSymmetries () const
  {
    static LSymmetries empty;
    cerr << Error( "SlicingNode::getSymmetries(): Base class method must never be called." ) << endl;
    return empty;
  }


  void  SlicingNode::setSymmetries ( const LSymmetries& )
  { cerr << Error( "SlicingNode::setSymmetries(): Base class method must never be called." ) << endl; }


  bool  SlicingNode::isSymmetry ( size_t index, Symmetry& )
  {
    cerr << Error( "SlicingNode::isSymmetry(): Base class method must never be called." ) << endl;
    return false;
  }
  

  bool  SlicingNode::isSymmetry ( size_t index )
  {
    cerr << Error( "SlicingNode::isSymmetry(): Base class method must never be called." ) << endl;
    return false;
  }
  

  void  SlicingNode::addSymmetry ( size_t childIndex, size_t copyIndex, bool reset )
  { cerr << Error( "SlicingNode::addSymmetry(): Base class method must never be called." ) << endl; }


  void  SlicingNode::removeSymmetry ( size_t index )
  { cerr << Error( "SlicingNode::removeSymmetry(): Base class method must never be called." ) << endl; }


  void  SlicingNode::normalizeSymmetries ()
  { cerr << Error( "SlicingNode::normalizeSymmetries(): Base class method must never be called." ) << endl; }


  void  SlicingNode::printChildren () const
  { cerr << Error( "SlicingNode::printChildren(): Base class method must never be called." ) << endl; }


  void  SlicingNode::printChildrenLine () const
  { cerr << Error( "SlicingNode::printChildrenLine(): Base class method must never be called." ) << endl; }


  SlicingNode* SlicingNode::clone ( unsigned int )
  {
    cerr << Error( "SlicingNode::clone(): Base class method must never be called." ) << endl;
    return NULL;
  }
  

  double  SlicingNode::getDevicesArea () const
  {
    cerr << Error( "SlicingNode::getDevicesArea(): Base class method must never be called." ) << endl;
    return 0.0;
  }
  

  double  SlicingNode::getOccupationArea () const
  {
    cerr << Error( "SlicingNode::getOccupationArea(): Base class method must never be called." ) << endl;
    return 0.0;
  }
  

  void  SlicingNode::createSlicingRouting ()
  { cerr << Error( "SlicingNode::createSlicingRouting(): Base class method must never be called." ) << endl; }


  void  SlicingNode::resetSlicingRouting ()
  { cerr << Error( "SlicingNode::resetSlicingRouting(): Base class method must never be called." ) << endl; }


  void  SlicingNode::destroySlicingRouting ()
  { cerr << Error( "SlicingNode::destroySlicingRouting(): Base class method must never be called." ) << endl; }


  size_t  SlicingNode::getRoutingIndex ( SlicingNode* ) const
  {
    cerr << Error( "SlicingNode::getRoutingIndex(): Base class method must never be called." ) << endl;
    return 0;
  }
  

  SlicingNode* SlicingNode::getSlicingRouting ( size_t ) const
  {
    cerr << Error( "SlicingNode::getSlicingRouting(): Base class method must never be called." ) << endl;
    return NULL;
  }
  

  bool  SlicingNode::checkSymmetryNet ( unsigned int , Net* , Net* ) const
  {
    cerr << Error( "SlicingNode::checkSymmetryNet(): Base class method must never be called." ) << endl;
    return false;
  }
  

  void  SlicingNode::addSymmetryNet ( unsigned int , Net* , Net* )
  { cerr << Error( "SlicingNode::addSymmetryNet(): Base class method must never be called." ) << endl; }


  void  SlicingNode::updateNetConstraints ()
  { cerr << Error( "SlicingNode::updateNetConstraints(): Base class method must never be called." ) << endl; }


  void  SlicingNode::updateSymNetAxis ()
  { cerr << Error( "SlicingNode::updateSymNetAxis(): Base class method must never be called." ) << endl; }


  void  SlicingNode::flattenDigitalNets ()
  { cerr << Error( "SlicingNode::flattenDigitalNets(): Base class method must never be called." ) << endl; }


  void  SlicingNode::updateWireOccupation ( Anabatic::Dijkstra* )
  { cerr << Error( "SlicingNode::updateWireOccupation(): Base class method must never be called." ) << endl; }


  void  SlicingNode::resetWireOccupation ()
  {
    cerr << Error( "SlicingNode::resetWireOccupation(): Base class method must never be called.\n"
                   "        (on %s)"
                 , getString(this).c_str() ) << endl;
  }


}  // Bora namespace.
