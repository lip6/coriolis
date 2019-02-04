// -*- C++ -*-
//
// This file is part of the Chams Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                      C H A M S                                  |
// |        H o r u s  -  S l i c i n g  T r e e                     |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricaneAMS/layout/SlicingNode.cpp"    |
// +-----------------------------------------------------------------+

#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricaneAMS/analogic/AnalogCellExtension.h"
#include "hurricaneAMS/layout/SlicingNode.h"
#include "anabatic/AnabaticEngine.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Property.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/RoutingLayerGauge.h"

using namespace std;
using Hurricane::Warning;
using Hurricane::Breakpoint;
using Hurricane::Error;
using Hurricane::NetRoutingState;
using Hurricane::NetRoutingExtension;
using Hurricane::NetExternalComponents;
using Hurricane::Net;
using Hurricane::Cell;
using Hurricane::DeepNet;


// -----------------------------------------------------------------------------------------------//
// Class : BoxSet
// -----------------------------------------------------------------------------------------------//


BoxSet::BoxSet( DbU::Unit height, DbU::Unit width ): _height( height )
                                                   , _width ( width  )
                                                   , _cpt   ( 1      ){}


BoxSet::BoxSet( BoxSet* boxSet )
{
  if ( boxSet != NULL ){
    _height = boxSet->getHeight();
    _width  = boxSet->getWidth();
    _cpt    = boxSet->getCpt();
  } else {
    _height = 0;
    _width  = 0;
    _cpt    = 1;
  }
}


BoxSet::~BoxSet(){}


void BoxSet::print() const 
{
  cerr << "area: "            << setprecision(4) << this->getOccupationArea() ;
  cerr << "%, \t cpt: "       << this->getCpt();
  if ((this->getWidth() == 0) && (this->getHeight() == 0)) cerr << ", \t ratio (W/H):inf";
  else                                                     cerr << ", \t ratio (W/H):" << setprecision(2) << this->getWidth()/this->getHeight();
  cerr << ", \t ratio (W*H):" << setprecision(4) << DbU::getPhysical(this->getWidth(),DbU::Micro) * DbU::getPhysical(this->getHeight(),DbU::Micro) << "um²";
  if (this->getType() == DeviceSNode){ cerr << ", \t Nfing: " << this->getNFing(); }
  cerr << ", \t H: "          << setprecision(4) << DbU::getPhysical(this->getHeight(),DbU::Micro) << "um" ;
  cerr << ", W: "             << DbU::getPhysical(this->getWidth(),DbU::Micro)<< "um"; 
  cerr << endl; 
}


const vector<BoxSet*>& BoxSet::getSet() const
{
  cerr << "Error(vector< BoxSet*> getSet () const): DBoxSet and RHVBoxSet do not have vector of BoxSets." << endl;
  static const vector<BoxSet*> boxSet = vector<BoxSet*> ();
  return boxSet;
}


int BoxSet::getNFing() const
{
  cerr << "Error(int BoxSet::getNFing() const): Only DBoxSet has fingers." << endl;
  return 0;
}


void BoxSet::destroy()
{
  delete(this);
}


// -----------------------------------------------------------------------------------------------//
// Class : HVBoxSet
// -----------------------------------------------------------------------------------------------//
                                       

HVBoxSet::HVBoxSet( vector<BoxSet*> dimensionSet, DbU::Unit height, DbU::Unit width ): BoxSet       ( height, width )
                                                                                     ,_dimensionSet ( dimensionSet  ){}
                                       

HVBoxSet::HVBoxSet( HVBoxSet* boxSet ): BoxSet        ( boxSet           )
                                      , _dimensionSet ( boxSet->getSet() ){}


HVBoxSet::~HVBoxSet(){}


double HVBoxSet::getDevicesArea() const
{
  double area = 0;
  for (vector<BoxSet*>::const_iterator it = _dimensionSet.begin(); it != _dimensionSet.end(); it++){ 
    area += (*it)->getDevicesArea();
  }
  return area;
}


// -----------------------------------------------------------------------------------------------//
// Class : HBoxSet
// -----------------------------------------------------------------------------------------------//


int HBoxSet::_count    = 0;
int HBoxSet::_countAll = 0;


HBoxSet::HBoxSet( vector< BoxSet*> dimensionSet, DbU::Unit height, DbU::Unit width  ): HVBoxSet( dimensionSet, height, width )
{
  if ( (_height == 0) && (_width == 0) ){
    calculateHeight();
    calculateWidth();
  }
}


HBoxSet::HBoxSet( HBoxSet* boxSet ): HVBoxSet( boxSet ){}
  

HBoxSet::~HBoxSet(){}


HBoxSet* HBoxSet::create( vector< BoxSet*> dimensionSet, DbU::Unit height, DbU::Unit width  )
{
  _count++;
  _countAll++;
  return new HBoxSet( dimensionSet, height, width ); 
}


HBoxSet* HBoxSet::clone()
{
  return HBoxSet::create( getSet(), getHeight(), getWidth() );
}


void HBoxSet::calculateHeight() 
{
  DbU::Unit currentH = 0;
  for (vector<BoxSet*>::const_iterator it = _dimensionSet.begin(); it != _dimensionSet.end(); it++){
    currentH += (*it)->getHeight();
  }
  _height = currentH;
}


void HBoxSet::calculateWidth()
{
  DbU::Unit currentW = 0;
  for (vector<BoxSet*>::const_iterator it = _dimensionSet.begin(); it != _dimensionSet.end(); it++){
    if (currentW < (*it)->getWidth()){ currentW = (*it)->getWidth(); }
  }
  _width = currentW;
}


void HBoxSet::destroy()
{
  _count--;
  BoxSet::destroy();
}

  
// -----------------------------------------------------------------------------------------------//
// Class : VBoxSet
// -----------------------------------------------------------------------------------------------//


int VBoxSet::_count    = 0;
int VBoxSet::_countAll = 0;


VBoxSet::VBoxSet( vector<BoxSet*> dimensionSet, DbU::Unit height, DbU::Unit width ): HVBoxSet( dimensionSet, height, width )
{
  if ( (_height == 0) && (_width == 0) ){
    calculateHeight();
    calculateWidth ();
  }
}


VBoxSet::VBoxSet( VBoxSet* boxSet ): HVBoxSet( boxSet ){}


VBoxSet::~VBoxSet(){}


VBoxSet* VBoxSet::create( vector<BoxSet*> dimensionSet, DbU::Unit height, DbU::Unit width  )
{
  _count++;
  _countAll++;
  return new VBoxSet( dimensionSet, height, width ); 
}


VBoxSet* VBoxSet::clone()
{
  return VBoxSet::create( getSet(), getHeight(), getWidth() );
}


void VBoxSet::calculateHeight()
{
  DbU::Unit currentH = 0;
  for (vector<BoxSet*>::const_iterator it = _dimensionSet.begin(); it != _dimensionSet.end(); it++){
    if (currentH < (*it)->getHeight()){ currentH = (*it)->getHeight(); }
  }
  _height = currentH;
}


void VBoxSet::calculateWidth()
{
  DbU::Unit currentW = 0;
  for (vector<BoxSet*>::const_iterator it = _dimensionSet.begin(); it != _dimensionSet.end(); it++){
    currentW += (*it)->getWidth();
  }
  _width = currentW;
}


void VBoxSet::destroy()
{
  _count--;
  BoxSet::destroy();
}


// -----------------------------------------------------------------------------------------------//
// Class : DBoxSet
// -----------------------------------------------------------------------------------------------//


int DBoxSet::_count    = 0;
int DBoxSet::_countAll = 0;


DBoxSet::DBoxSet( DbU::Unit height, DbU::Unit width, int nfing ): BoxSet( height, width ), _nfing(nfing){}


DBoxSet::DBoxSet( DBoxSet* boxSet ): BoxSet( boxSet ), _nfing(boxSet->getNFing()){}


DBoxSet::~DBoxSet(){}


DBoxSet* DBoxSet::create( DbU::Unit height, DbU::Unit width, int nfing )
{
  _count++;
  _countAll++;
  return new DBoxSet( height, width, nfing ); 
}


DBoxSet* DBoxSet::clone()
{
  return DBoxSet::create( getHeight(), getWidth(), getNFing() ); 
}


void DBoxSet::destroy()
{
  _count--;
  BoxSet::destroy();
}
  

// -----------------------------------------------------------------------------------------------//
// Class : RHVBoxSet
// -----------------------------------------------------------------------------------------------//


int RHVBoxSet::_count    = 0;
int RHVBoxSet::_countAll = 0;


RHVBoxSet::RHVBoxSet( DbU::Unit height, DbU::Unit width ): BoxSet( height, width )
{
  _width  = width;
  _height = height;
}


RHVBoxSet::RHVBoxSet( RHVBoxSet* boxSet ): BoxSet( boxSet ){}


RHVBoxSet::~RHVBoxSet(){}


void RHVBoxSet::destroy()
{
  _count--;
  delete(this);
}


void RHVBoxSet::print() const 
{
  cerr << "area: "            << setprecision(4) << this->getOccupationArea() ;
  cerr << "%, \t cpt: "       << this->getCpt();
//  if ( (_width == 0)&&(_height == 0) ) cerr << ", \t ratio (W/H):inf";
//  else                                 cerr << ", \t ratio (W/H):" << _width/_height;
//  cerr << ", \t ratio (W*H):" << setprecision(4) << DbU::getPhysical(_width,DbU::Micro) * DbU::getPhysical(_height,DbU::Micro) << "um²";

//  if (this->getType() == DeviceSNode){ cerr << ", \t Nfing: " << this->getNFing(); }
  cerr << ", \t H: "          << setprecision(4) << DbU::getPhysical(_height,DbU::Micro) << "um" ;
  cerr << ", W: "             << DbU::getPhysical(_width,DbU::Micro)<< "um"; 
  cerr << endl; 
}


// -----------------------------------------------------------------------------------------------//
// Class : RHBoxSet
// -----------------------------------------------------------------------------------------------//
  
  
RHBoxSet::RHBoxSet( DbU::Unit height ): RHVBoxSet( height, 0 ){}


RHBoxSet::RHBoxSet( RHBoxSet* boxSet ): RHVBoxSet( boxSet ){}

  
RHBoxSet::~RHBoxSet(){}


RHBoxSet* RHBoxSet::create( DbU::Unit height )
{
  _count++;
  _countAll++;
  return new RHBoxSet ( height );
}


RHBoxSet* RHBoxSet::clone()
{
  return RHBoxSet::create( getHeight() ); 
}


// -----------------------------------------------------------------------------------------------//
// Class : RVBoxSet
// -----------------------------------------------------------------------------------------------//
  
  
RVBoxSet::RVBoxSet( DbU::Unit width ): RHVBoxSet( 0, width ){}


RVBoxSet::RVBoxSet( RVBoxSet* boxSet ): RHVBoxSet( boxSet ){}

  
RVBoxSet::~RVBoxSet(){}


RVBoxSet* RVBoxSet::create( DbU::Unit width )
{
  _count++;
  _countAll++;
  return new RVBoxSet( width );
}


RVBoxSet* RVBoxSet::clone()
{
  return RVBoxSet::create( getWidth() ); 
}


// -----------------------------------------------------------------------------------------------//
// Class : NodeSets
// -----------------------------------------------------------------------------------------------//


NodeSets::NodeSets( double start, double step, double count )
{
  _nodeSets = std::vector<BoxSet*>();
  _nodeSets.clear();
  _start = start;
  _step  = step;
  _count = count;
}
  

NodeSets::NodeSets( const NodeSets* other )
{
  _nodeSets = other->getNodeSets();
  _start = other->getStartParameter();
  _step  = other->getStepParameter();
  _count = other->getCountParameter();
}


NodeSets::~NodeSets(){}

  
NodeSets* NodeSets::create( Cell* cell, double start, double step, double count, CRL::RoutingGauge* rg )
{
  NodeSets* nodeset = new NodeSets(start, step, count);

  Device* dev = dynamic_cast<Device*>( cell );
  if (dev) {
  //cdebug_log(536,0) << "createNodeSets for an Analog Device" << endl;
    TransistorFamily* tf  = dynamic_cast<TransistorFamily*>(dev);
    
    for (int i = 0; i < count; i++){
      tf->setNfing( start + i*step ); 
      
      FormFactorParameter* pff = NULL;
      if ( (pff = dynamic_cast<FormFactorParameter*>(tf->getParameter("M"))) != NULL ) 
        pff->setValue( tf->getNfing() );
      
      auto_ptr<LayoutGenerator> layoutGenerator ( new LayoutGenerator() );
      layoutGenerator->setDevice(dev);
      layoutGenerator->drawLayout(); 
      
      if (rg){
        float h2pitch = rg->getHorizontalPitch()*2;
        float v2pitch = rg->getVerticalPitch  ()*2;
                                       
        float h = dev->getAbutmentBox().getHeight();
        float w = dev->getAbutmentBox().getWidth();

        if (fmod(h,h2pitch) > 1e-06) {
          cerr << Warning( "NodeSets::create(): The height of device \"%s\" (%s) is not pitched on 2*%s (adjusted)."
                         , getString(dev->getName()).c_str()
                         , DbU::getValueString(dev->getAbutmentBox().getWidth()).c_str()
                         , DbU::getValueString(rg->getHorizontalPitch()*2).c_str()
                         ) << endl;
        }
        if (fmod(w,v2pitch) > 1e-06) {
          cerr << Warning( "NodeSets::create(): The width of device \"%s\" (%s) is not pitched on 2*%s (adjusted)."
                         , getString(dev->getName()).c_str()
                         , DbU::getValueString(dev->getAbutmentBox().getWidth()).c_str()
                         , DbU::getValueString(rg->getVerticalPitch()*2).c_str()
                         ) << endl;
        }

        nodeset->push_back(DBoxSet::create( ceil(h/h2pitch)*h2pitch
                                          , ceil(w/v2pitch)*v2pitch
                                          , start + i*step
                                          )
                          );
      } else {
        nodeset->push_back(DBoxSet::create( dev->getAbutmentBox().getHeight()
                                          , dev->getAbutmentBox().getWidth()
                                          , start + i*step
                                          )
                          );
      }
    }
  } else {
  //cdebug_log(536,0) << "createNodeSets for a Digital Device: " << cell << endl;
    if (rg) {
      DbU::Unit h2pitch = rg->getHorizontalPitch()*2;
      DbU::Unit v2pitch = rg->getVerticalPitch  ()*2;
      DbU::Unit h       = cell->getAbutmentBox().getHeight();
      DbU::Unit w       = cell->getAbutmentBox().getWidth(); 

      if (h % h2pitch) {
        cerr << Warning( "NodeSets::create(): The height of device \"%s\" (%s) is not pitched on %s*2 (adjusted)."
                       , getString(cell->getName()).c_str()
                       , DbU::getValueString(cell->getAbutmentBox().getHeight()).c_str()
                       , DbU::getValueString(rg->getHorizontalPitch()).c_str()
                       ) << endl;
      }
      if (w % v2pitch) {
        cerr << Warning( "NodeSets::create(): The width of device \"%s\" (%s) is not pitched on %s*2 (adjusted)."
                       , getString(cell->getName()).c_str()
                       , DbU::getValueString(cell->getAbutmentBox().getWidth()).c_str()
                       , DbU::getValueString(rg->getVerticalPitch()).c_str()
                       ) << endl;
      }

      nodeset->push_back(DBoxSet::create( ceil(h/h2pitch)*h2pitch
                                        , ceil(w/v2pitch)*v2pitch
                                        )
                        );
    } else {
      nodeset->push_back(DBoxSet::create( cell->getAbutmentBox().getHeight()
                                        , cell->getAbutmentBox().getWidth()
                                        )
                        );
    }
  }
  return nodeset;
}

NodeSets* NodeSets::create()
{
  return new NodeSets();
}


BoxSet* NodeSets::operator[] ( size_t i ) 
{
  if (i < _nodeSets.size()) return _nodeSets[i];
  return NULL;
}


BoxSet* NodeSets::at ( size_t i ) 
{
  if (i < _nodeSets.size()) return _nodeSets[i];
  return NULL;
}


void NodeSets::clear() 
{
  if ( _nodeSets.empty() != true ){
    for (vector<BoxSet*>::iterator it = _nodeSets.begin(); it != _nodeSets.end(); it++){
      (*it)->destroy();
    }
    _nodeSets.clear();
  }
}


BoxSet* NodeSets::getPairH( DbU::Unit height ) const 
{
  DbU::Unit w        = 0;
  DbU::Unit hclosest = 0;
  BoxSet* boxSet = (*_nodeSets.begin());

  for (vector<BoxSet*>::const_iterator itHW = _nodeSets.begin(); itHW != _nodeSets.end(); itHW++){ 

    if ( ((*itHW)->getHeight() > hclosest) && (height >= (*itHW)->getHeight()) ){
      hclosest = (*itHW)->getHeight();
      w        = (*itHW)->getWidth();
      boxSet   = (*itHW);
    }
  }
    
  if ( (w == 0) && (hclosest == 0) )
    { cerr << "Error(pair<DbU::Unit,DbU::Unit> NodeSets::getPairH( DbU::Unit height ) const): No solution for h = " << height << " has been found." << endl; }

  return boxSet;
}
 

BoxSet* NodeSets::getPairHW( DbU::Unit height, DbU::Unit width ) const 
{
  vector<BoxSet*>::const_iterator it = _nodeSets.begin();
  DbU::Unit bestH    = (*it)->getHeight();
  DbU::Unit bestW    = (*it)->getWidth();
  DbU::Unit currentH = 0;
  DbU::Unit currentW = 0;
  BoxSet*   boxSet   = (*it);

  while (it != _nodeSets.end()){
    currentH = (*it)->getHeight();
    currentW = (*it)->getWidth();

    if (currentH <= height){
      if ( currentH > bestH ){
        bestH = currentH;
        bestW = currentW;
        boxSet = (*it);
      } else if (currentH == bestH){
        if ( (currentW > bestW) && (currentW <= width) ){ 
          bestH = currentH;
          bestW = currentW;
          boxSet = (*it);
        }
        else if ( (currentW < bestW) && (bestW > width) ){ 
          bestH = currentH;
          bestW = currentW;
          boxSet = (*it);
        }
      }
      it++;
    } else {
      it = _nodeSets.end();
    }
  }

  return boxSet;
}
 

BoxSet* NodeSets::getPairHW( unsigned int index ) const 
{
  BoxSet* bs = NULL;
  if ( index < _nodeSets.size() ){
    bs = _nodeSets[index];
  }
  return bs;
}


int NodeSets::findIndex( DbU::Unit height, DbU::Unit width ) const
{
  bool end     = false;
  int indextbd = 0;
  int index    = 0;
  vector<BoxSet*>::const_iterator it = _nodeSets.begin();

  while(end == false){
    if ( ((*it)->getHeight() == height) && ((*it)->getWidth() == width) ){
      indextbd = index;
      end      = true;
    }
    index++;
    it++;
    if (it == _nodeSets.end()){ end = true; }
  }

  return indextbd;
}


vector<BoxSet* >::iterator NodeSets::find( DbU::Unit height, DbU::Unit width )
{
  vector<BoxSet* >::iterator it     = _nodeSets.begin();
  vector<BoxSet* >::iterator itFind = _nodeSets.end();
    
  if (_nodeSets.empty() != true){

    while(it != _nodeSets.end()){
      if ( ((*it)->getHeight() == height) && ((*it)->getWidth() == width) ){ 
        itFind = it;
        it     = _nodeSets.end(); 
      }
      else { it++; }
      if ( it == _nodeSets.end() ){ it = _nodeSets.end(); }
    }
  } else { it = _nodeSets.end(); }

  return itFind;
}


vector<BoxSet* >::iterator NodeSets::find( BoxSet* boxSet )
{
  vector<BoxSet* >::iterator it     = _nodeSets.begin();
  vector<BoxSet* >::iterator itFind = _nodeSets.end();

  if ( (_nodeSets.empty() != true) && (boxSet != NULL) ){

    while(it != _nodeSets.end()){
      if ( ((*it)->getHeight() == boxSet->getHeight()) && ((*it)->getWidth() == boxSet->getWidth()) ){ 
        itFind = it;
        it     = _nodeSets.end(); 
      }
      else { it++; }
    }
  } else { it = _nodeSets.end(); }

  return itFind;
}


vector<BoxSet* >::iterator NodeSets::find( int nfing )
{
  vector<BoxSet* >::iterator it     = _nodeSets.begin();
  vector<BoxSet* >::iterator itFind = _nodeSets.end();

  if ( (_nodeSets.empty() != true) ){

    while(it != _nodeSets.end()){
      if ( (*it)->getNFing() == nfing ){ 
        itFind = it;
        it     = _nodeSets.end(); 
      }
      else { it++; }
    }
  } else { it = _nodeSets.end(); }

  return itFind;
}


void NodeSets::print() const 
{
  cerr << "Print - NodeSets:" << endl;
  int index = 0;
  if ( this->size() == 0){ cerr << "--- EMPTY ---" << endl; }
  else {
    for (vector<BoxSet*>::const_iterator itPrint = _nodeSets.begin(); itPrint != _nodeSets.end(); itPrint++)
      { 
        cerr << index << ": \t";
        (*itPrint)->print();
        index++;
      } 
    cerr << endl;
  }
}


bool NodeSets::compare ( NodeSets nodeSets2, unsigned int flags ) const
{
  if (_nodeSets.size() != nodeSets2.size()) {
    if (flags & ShowDiff)
      cerr << "NodeSets::compare() size differs: " << _nodeSets.size()
           << " vs. " << nodeSets2.size() << endl;
    return false;
  }

  bool isSame = true;
  for ( size_t iset = 0 ; iset<_nodeSets.size() ; ++iset ) {
    if (  (_nodeSets[iset]->getHeight() != nodeSets2[iset]->getHeight())
       or (_nodeSets[iset]->getWidth () != nodeSets2[iset]->getWidth ()) ) {
      isSame = false;
      if (flags & ShowDiff)
        cerr << "NodeSets::compare() Box [" << iset << "] differs "
             << " (" << DbU::getValueString(_nodeSets[iset]->getWidth()) << "," << DbU::getValueString(_nodeSets[iset]->getHeight()) << ") vs."
             << " (" << DbU::getValueString(nodeSets2[iset]->getWidth()) << "," << DbU::getValueString(nodeSets2[iset]->getHeight()) << ")"
             << endl;
    }
  }

  return isSame;
}


void NodeSets::push_back( BoxSet* boxSet )
{
  if( this->find(boxSet) == _nodeSets.end() ){ _nodeSets.push_back(boxSet); }
  else { 
    (*find(boxSet))->incrementCpt(); 
    boxSet->destroy();
  }
}


void NodeSets::push_back( vector<BoxSet*> vect, DbU::Unit height, DbU::Unit width, unsigned int type )
{
  if( this->find(height, width) == _nodeSets.end() ){ 
    if (type == HorizontalSNode){
      _nodeSets.push_back(HBoxSet::create(vect, height, width));     
    } 
    else if (type == VerticalSNode){
      _nodeSets.push_back(VBoxSet::create(vect, height, width));   
    }    
  }
  else { 
    (*find(height, width))->incrementCpt(); 
  }
}


NodeSets* NodeSets::clone() 
{
  NodeSets* nodesets = new NodeSets( _start, _step, _count );
  for (vector<BoxSet*>::iterator it = _nodeSets.begin(); it != _nodeSets.end(); it++)
    { 
      nodesets->push_back((*it)->clone()); 
    }
  return nodesets;
}


// -----------------------------------------------------------------------------------------------//
// Class : SlicingNode
// -----------------------------------------------------------------------------------------------//

CRL::RoutingGauge*    SlicingNode::_rg            = NULL;
Cell*                 SlicingNode::_cell          = NULL;
Katana::KatanaEngine* SlicingNode::_katana        = NULL;
vector<Segment*>      SlicingNode::_railSegments = vector<Segment*>();
 
SlicingNode::SlicingNode( unsigned int type
                        , NodeSets*    nodeSets
                        , unsigned int alignment    
                        , BoxSet*      boxSet
                        ):_nodeSets(nodeSets)
                         ,_x(0), _y(0)
{
  _flags        = 0;
  _parent       = NULL;
  _master       = NULL;
  _gcell        = NULL;
  _nodeSets     = nodeSets;
  _x            = 0;
  _y            = 0;
  this->setType(type);
  this->setAlignment(alignment);

  if ( boxSet != NULL ){ 
    this->setBoxSet(boxSet);
    this->setPreset(Preset); 
    this->setSet(Set);
  }
} 


SlicingNode::~SlicingNode(){}

  
bool SlicingNode::isSameSize( BoxSet* boxSet ) const 
{
  bool same = false;
  if ( (_boxSet != NULL)&&(boxSet != NULL) ){
    if ((_boxSet->getHeight() == boxSet->getHeight()) && (_boxSet->getWidth() == boxSet->getWidth())){
      same = true;
    }
  }
  return same;
}

  
bool SlicingNode::isSameSize( DbU::Unit height, DbU::Unit width ) const 
{
  bool same = false;
  if ( _boxSet != NULL ){
    if ( (_boxSet->getHeight() == height) && (_boxSet->getWidth() == width) ){
      same = true;
    }
  }
  return same;
}
         

bool SlicingNode::isEmpty() const
{
  return _nodeSets->empty();
}


DbU::Unit SlicingNode::getHeight () const 
{
  if (_boxSet != NULL){ 
    return _boxSet->getHeight(); 
  } else { return 0; }
}


DbU::Unit SlicingNode::getWidth () const 
{
  if (_boxSet != NULL){ 
    return _boxSet->getWidth(); 
  } else { return 0; }
}



DbU::Unit SlicingNode::getRootHeight () const 
{
  if (_parent){
    SlicingNode* p = _parent;  
    while(p->getParent()){
      p = p->getParent();
    }
    return p->getHeight();
  } else {
    return getHeight();
  }
}


DbU::Unit SlicingNode::getRootWidth () const 
{
  if (_parent){
    SlicingNode* p = _parent;  
    while(p->getParent()){
      p = p->getParent();
    }
    return p->getWidth();
  } else {
    return getWidth();
  }
}


DbU::Unit SlicingNode::getRootX () const 
{
  if (_parent){
    SlicingNode* p = _parent;  
    while(p->getParent()){
      p = p->getParent();
    }
    return p->getX();
  } else {
    return getX();
  }
}


DbU::Unit SlicingNode::getRootY () const 
{
  if (_parent){
    SlicingNode* p = _parent;  
    while(p->getParent()){
      p = p->getParent();
    }
    return p->getY();
  } else {
    return getY();
  }
}


SlicingNode* SlicingNode::getRoot() 
{ 
  SlicingNode* parent = this;
  if (_parent != NULL){ parent = _parent->getRoot(); }
  return parent; 
}


int SlicingNode::getCpt()
{
  if (_boxSet != NULL){ return _boxSet->getCpt(); }
  else                { return 0;                 }
}


double SlicingNode::getRatio()
{
  if (_boxSet != NULL){ return _boxSet->getRatio(); }
  else                { return 0;                   }
}


double SlicingNode::getArea()
{
  if (_boxSet != NULL){ return _boxSet->getArea(); }
  else                { return 0;                   }
}


DbU::Unit SlicingNode::getVPitch () 
{
  if (_rg) return _rg->getVerticalPitch();

  cerr << Error( "DbU::Unit getVPitch(): RoutingGauge has not been defined." )  << endl;
  return 0;
}
     
DbU::Unit SlicingNode::getHPitch () 
{
  if (_rg) return _rg->getHorizontalPitch();

  cerr << Error( "DbU::Unit getHPitch(): RoutingGauge has not been defined." )  << endl;
  return 0;
}

void SlicingNode::setBoxSet( BoxSet* boxSet )
{
  if ( (!this->isPreset()) ){
    if (_nodeSets->find(boxSet) != _nodeSets->end()){ 
      this->setSet(Set);
      _boxSet = boxSet; 
    } else {
      cerr << "Error(void SlicingNode::setBoxSet( BoxSet* boxSet )): Boxset not found.";
    }
  }
}


void SlicingNode::setBoxSet( DbU::Unit height, DbU::Unit width )
{
  if (!this->isPreset()){
    if (_nodeSets->find(height, width) != _nodeSets->end()){ 
      this->setSet(Set);
      _boxSet = (*_nodeSets->find(height, width)); 
    } else {
      cerr << "Error(void SlicingNode::setBoxSet( BoxSet* boxSet )): Boxset not found.";
    }
  } 
}



void SlicingNode::setBoxSet( size_t index )
{
  if (index > (_nodeSets->size()-1)){ cerr << "Error(void SlicingNode::setBoxSet( int index )): Out of bound index." << endl; }
  else {
    this->_setBoxSet((*(_nodeSets->begin() + index)));
  }
}


void SlicingNode::_setBoxSet( BoxSet* boxSet )
{
  if (isPreset()) return;

  setSet( Set );
  _boxSet = boxSet; 
}


void SlicingNode::setX ( DbU::Unit x )
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


void SlicingNode::setY ( DbU::Unit y )
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

  
void SlicingNode::clearParentsNodeSets()
{
  if ( _parent != NULL ){ _parent->clearParentsNodeSets(); }
  if (getMaster() == NULL) _nodeSets->clear();
}


void SlicingNode::resetSlicingTree()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Reset a slicing tree means:
// - Empty NodeSets of every parent
// - Unset every nodes that are not preset
// - Unplace every nodes
// -----------------------------------------------------------------------------------------------//

  clearParentsNodeSets();
  getRoot()->_resetSlicingTree();
}


void SlicingNode::_resetSlicingTree()
{
  _x = 0;
  _y = 0;
  setPlaced(false);
    
  if (!this->isPreset()){
    _boxSet = NULL;
    setSet(false);
  }
}


void SlicingNode::print() const
{
  printParent();
  if   ( this->isPreset() ){ cerr << "Preset     : True"   << endl; }
  else                     { cerr << "Preset     : False " << endl; }
  if   ( this->isSet() )   { cerr << "Set        : True"   << endl; }
  else                     { cerr << "Set        : False " << endl; }
  if   ( this->isPlaced() ){ cerr << "Placed     : True"   << endl; }
  else                     { cerr << "Placed     : False " << endl; }
  if   ( this->getMaster()){
    cerr << "Master     : "; 
    this->getMaster()->printLine();
  }
  unsigned int type = getType();
  if      ( type == HorizontalSNode ){ cerr << "Type       : HorizontalSNode"<< endl; }
  else if ( type == VerticalSNode   ){ cerr << "Type       : VerticalSNode"  << endl; }
  else if ( type == DeviceSNode     ){ cerr << "Type       : DeviceSNode"    << endl; }
  else if ( type == RoutingSNode    ){ cerr << "Type       : RoutingSNode"   << endl; }
  else                               { cerr << "Type       : UnknownType"    << endl; }
  cerr << "Height     : " << DbU::getPhysical(getHeight(),DbU::Micro) << endl;
  cerr << "Width      : " << DbU::getPhysical(getWidth(),DbU::Micro) << endl;
  cerr << "X          : " << DbU::getPhysical(_x,DbU::Micro) << endl;
  cerr << "Y          : " << DbU::getPhysical(_y,DbU::Micro) << endl;
  if (isRoutingEstimated()) cerr << "REstimated : True"  << endl;
  else                      cerr << "REstimated : False" << endl;

//_nodeSets->print();
  cerr << "GCell      : " << _gcell << endl;
  if (!((getType() == HorizontalSNode)||(getType() == VerticalSNode))){
    cerr << "GCell      : " << _gcell << endl;
    if(_gcell){
      cerr << "Edges : " << endl;
      vector<Anabatic::Edge*> ne = _gcell->getNorthEdges();
      vector<Anabatic::Edge*> se = _gcell->getSouthEdges();
      vector<Anabatic::Edge*> ee = _gcell->getEastEdges();
      vector<Anabatic::Edge*> we = _gcell->getWestEdges();

      cerr << "--- North : " << endl;
      for (vector<Anabatic::Edge*>::const_iterator itn = ne.begin(); itn != ne.end(); itn++){
        cerr << (*itn)->getOpposite(_gcell) << endl;
      }
      cerr << "--- South : " << endl;
      for (vector<Anabatic::Edge*>::const_iterator its = se.begin(); its != se.end(); its++){
        cerr << (*its)->getOpposite(_gcell) << endl;
      }
      cerr << "--- East : " << endl;
      for (vector<Anabatic::Edge*>::const_iterator ite = ee.begin(); ite != ee.end(); ite++){
        cerr << (*ite)->getOpposite(_gcell) << endl;
      }
      cerr << "--- West : " << endl;
      for (vector<Anabatic::Edge*>::const_iterator itw = we.begin(); itw != we.end(); itw++){
        cerr << (*itw)->getOpposite(_gcell) << endl;
      }
    }
  }
  cerr << endl;
    
}


void SlicingNode::printLine() const 
{
  unsigned int type = getType();
  if (_master) {
    cerr << " [MASTER]: ";
    _master->printLine(); 
  }
  if      ( type == HorizontalSNode ){ cerr << "Type       : HorizontalSNode"; }
  else if ( type == VerticalSNode   ){ cerr << "Type       : VerticalSNode"  ; }
  else if ( type == DeviceSNode     ){ cerr << "Type       : DeviceSNode"    ; }
  else if ( type == RoutingSNode    ){ cerr << "Type       : RoutingSNode"   ; }
  else                               { cerr << "Type       : UnknownType"    ; }
  cerr << ", X: " << DbU::getPhysical(getX(),DbU::Micro);
  cerr << ", Y: " << DbU::getPhysical(getY(),DbU::Micro);
  cerr << ", W: " << setprecision(4) << DbU::getPhysical(getWidth() ,DbU::Micro);
  cerr << ", H: " << setprecision(4) << DbU::getPhysical(getHeight(),DbU::Micro) << endl;
}


void SlicingNode::printParent() const 
{
  if (_parent != NULL){
    unsigned int type = _parent->getType();
    if      ( type == HorizontalSNode ){ cerr << "Parent     : Type: HorizontalSNode" ; }
    else if ( type == VerticalSNode   ){ cerr << "Parent     : Type: VerticalSNode"   ; }
    else                               { cerr << "Parent     : Type: UnknownType"; }
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
  if (getGCell()) os << " id:" << getGCell()->getId();
  os << " " << (void*)this << ">";
  return os.str();
}


string  SlicingNode::_getTypeName () const
{ return "SlicingNode"; }


void SlicingNode::printBoxSet() const
{
  if (_boxSet != NULL){
    _boxSet->print();
  }
}


void SlicingNode::place( DbU::Unit x, DbU::Unit y )
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


void SlicingNode::replace( DbU::Unit x, DbU::Unit y )
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


void SlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
{
  cdebug_log(536,1) << "SlicingNode::place(DbU::Unit x, DbU::Unit y)" << endl;
  SlicingNode::place(x,y);
  cdebug_tabw(536,-1);
}


void SlicingNode::setGlobalSize( DbU::Unit height, DbU::Unit width )
{
  cmess2 << "  o  Selecting box set (" << DbU::getValueString(height)
         << "," << DbU::getValueString(width) << ")" << endl;

  this->setBoxSet( height, width ); 
}


void SlicingNode::setGlobalSize( size_t index )
{
  cmess2 << "  o  Selecting box set at index " << index << endl;

  this->setBoxSet( index ); 
}


void SlicingNode::_setGlobalSize ( BoxSet* boxSet ) 
{
  cdebug_log(535,1) << "SlicingNode::_setGlobalSize( BoxSet* boxSet ) " << endl;
  _setBoxSet( boxSet );
  cdebug_log(535,-1);
}


void SlicingNode::preDestroy()
{
  if ( _parent != NULL ){
    _parent->removeNode(this);
  }/* else {
    if (_railCell) _railCell->destroy();
    }*/
  if (this->getMaster() == NULL) _nodeSets->clear();
}


void SlicingNode::destroy()
{
  SlicingNode::preDestroy();
  delete(this);
}


void SlicingNode::preRecursiveDestroy()
{
  SlicingNode::preDestroy();
}


void SlicingNode::recursiveDestroy()
{
  SlicingNode::preRecursiveDestroy();
  delete(this);
}


list<SlicingNode*> SlicingNode::getLeaves() const
{
  return list<SlicingNode*>();
}


bool SlicingNode::checkInitialPlacement( int& cpt ) const 
{
// -----------------------------------------------------------------------------------------------//
// Notes:: Initial placement criteria is having at least 2 devices placed at (x, y) = (0, 0)
// -----------------------------------------------------------------------------------------------//
  bool initialPlacement = false;
  if ( cpt < 2 ){ initialPlacement = true; }
  return initialPlacement;
}

void SlicingNode::updateCellAbutmentBox()
{
  Point pmin = Point(getX(), getY());  
  Point pmax = Point(getX()+getWidth(), getY()+getHeight());  
  Box   box  = Box(pmin, pmax);
  _cell->setAbutmentBox(box);
}


void SlicingNode::setGCell( Anabatic::GCell* gcell )
{
  _gcell = gcell;
  cdebug_log(535,0) << "SlicingNode::setGCell() " << _gcell << endl;
}

// Error Methods
DbU::Unit SlicingNode::getToleranceRatioH() const
{
  cerr << "Error(DbU::Unit SlicingNode::getToleranceH()): Device and Routing do not have tolerance parameters." << endl;
  return 0;
}


DbU::Unit SlicingNode::getToleranceRatioW() const
{
  cerr << "Error(DbU::Unit SlicingNode::getToleranceW()): Device and Routing do not have tolerance parameters." << endl;
  return 0;
}


void SlicingNode::setToleranceRatioH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::setToleranceH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::setToleranceRatioW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::setToleranceW( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


DbU::Unit SlicingNode::getToleranceBandH() const
{
  cerr << "Error(DbU::Unit SlicingNode::getToleranceBandH()): Device and Routing do not have tolerance parameters." << endl;
  return 0;
}


DbU::Unit SlicingNode::getToleranceBandW() const
{
  cerr << "Error(DbU::Unit SlicingNode::getToleranceBandW()): Device and Routing do not have tolerance parameters." << endl;
  return 0;
}


void SlicingNode::setToleranceBandH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::setToleranceBandH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::setToleranceBandW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::setToleranceBandW(DbU::Unit tolerance)): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::recursiveSetToleranceRatioH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::recursiveSetToleranceRatioH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::recursiveSetToleranceRatioW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::recursiveSetToleranceRatioW( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::recursiveSetToleranceBandH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::recursiveSetToleranceBandH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::recursiveSetToleranceBandW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::recursiveSetToleranceBandW( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::_recursiveSetToleranceRatioH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::_recursiveSetToleranceRatioH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::_recursiveSetToleranceRatioW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::_recursiveSetToleranceRatioW( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::_recursiveSetToleranceBandH( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::_recursiveSetToleranceBandH( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::_recursiveSetToleranceBandW( DbU::Unit tolerance )
{
  cerr << "Error(void SlicingNode::_recursiveSetToleranceBandW( DbU::Unit tolerance )): Device and Routing do not have tolerance parameters." << endl; 
}


void SlicingNode::setTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  cerr << "Error(void SlicingNode::setTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )): Device and Routing do not have tolerance parameters." << endl; 
}  


void SlicingNode::recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  cerr << "Error(void SlicingNode::recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )): Device and Routing do not have tolerance parameters." << endl; 
} 


void SlicingNode::_recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  cerr << "Error(void SlicingNode::_recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )): Device and Routing do not have tolerance parameters." << endl; 
}


bool SlicingNode::hasEmptyChildrenNodeSets() const
{
  cerr << "Error(bool SlicingNode::hasEmptyChildrenNodeSets()): Device and routing do not have child." << endl;
  return true;
} 


const vector<SlicingNode*>& SlicingNode::getChildren() const
{
  cerr << "Error(vector<SlicingNode*> SlicingNode::getChildren()): Device and Routing do not have child." << endl;
  static vector<SlicingNode*> dummyVectorSlicingNode;
  return dummyVectorSlicingNode;
}


SlicingNode* SlicingNode::getChild( int index ) const
{
  cerr << "Error(SlicingNode* SlicingNode::getChild(int index)): Device and Routing do not have child." << endl;
  return NULL;
}


int SlicingNode::getChildIndex( SlicingNode* node ) const
{
  cerr << "Error(int SlicingNode::getChildIndex( SlicingNode* node )): Device and Routing do not have child." << endl;
  return 0;
}


size_t SlicingNode::getNbChild() const
{
  cerr << "Error(int SlicingNode::getNbChild()): Device and Routing do not have child." << endl;
  return 0;
}


void SlicingNode::createChild( unsigned int type, unsigned int alignment )
{
  cerr << "Error(void createChild( SlicingType type, Alignment alignment = UnknownAlignment)): Device and Routing do not have child." << endl; 
}


void SlicingNode::createChild( NodeSets*    nodeSets
                             , unsigned int alignment
                             , Instance*    instance
                             , BoxSet*      boxSet
                             )
{
  cerr << "Error(void createChild( NodeSets* nodeSets, Alignment alignment, Instance* instance, BoxSet* boxset )): Device and Routing do not have child." << endl; 
}


void SlicingNode::createChild( int childIndex, int copyIndex, unsigned int tr )
{
  std::cerr << "Error(createChild( int childIndex, int copyIndex, unsigned int tr )): Device and Routing do not have child." << endl; 
}


void SlicingNode::createRouting( DbU::Unit space )
{
  std::cerr << "Error(void createChild( DbU::Unit space )): Device and Routing do not have child." << endl; 
}


void SlicingNode::insertNode( SlicingNode* node, int index )
{
  cerr << "Error(void SlicingNode::insertNode( SlicingNode* node, int index )): Device and Routing do not have child." << endl; 
}


void SlicingNode::push_back( SlicingNode* node, bool reset )
{
  cerr << "Error(void SlicingNode::push_back( SlicingNode* node, bool reset )): Device and Routing do not have child." << endl; 
}


void SlicingNode::push_front( SlicingNode* node )
{
  cerr << "Error(void SlicingNode::push_front( SlicingNode* node )): Device and Routing do not have child." << endl; 
}


void SlicingNode::removeNode( SlicingNode* node )
{
  cerr << "Error(void SlicingNode::removeNode( SlicingNode* node )): Device and Routing do not have child." << endl; 
}


void SlicingNode::removeAllNodes()
{
  cerr << "Error(void SlicingNode::removeAllNodes()): Device and Routing do not have child." << endl; 
}


list<pair<int,int> > SlicingNode::getSymmetries() const
{
  cerr << "Error(list<pair<int,int> > SlicingNode::getSymmetries() const) : Device and Routing do not have symmetries." << endl; 
  return list<pair<int,int> >();
}


void SlicingNode::setSymmetries( list<pair<int,int> > sym )
{
  cerr << "Error(void SlicingNode::setSymmetries( list<pair<int,int> > sym )) : Device and Routing do not have symmetries." << endl; 
}


bool SlicingNode::isSymmetry( int index, std::pair<int,int>& symmetry )
{
  cerr << "Error(bool SlicingNode::isSymmetry( int index, std::pair<int,int>& symmetry )) : Device and Routing do not have symmetries." << endl; 
  return false;
}


bool SlicingNode::isSymmetry( int index )
{
  cerr << "Error(bool SlicingNode::isSymmetry( int index )) : Device and Routing do not have symmetries." << endl; 
  return false;
}


void SlicingNode::addSymmetry( int childIndex, int copyIndex, bool reset )
{
  cerr << "Error(void SlicingNode::addSymmetry( int childIndex, int copyIndex, bool reset )) : Device and Routing do not have symmetries." << endl; 
}


void SlicingNode::removeSymmetry( int index )
{
  cerr << "Error(void SlicingNode::removeSymmetry( int index )) : Device and Routing do not have symmetries." << endl; 
}


void SlicingNode::normalizeSymmetries() 
{
  cerr << "Error(void SlicingNode::normalizeSymmetries() const) : Device and Routing do not have symmetries." << endl; 
}


void SlicingNode::printChildren() const
{
  cerr << "Error(void SlicingNode::printChildren() const): Device and Routing do not have child." << endl; 
}


void SlicingNode::printChildrenLine() const
{
  cerr << "Error(void SlicingNode::printChildrenLine() const): Device and Routing do not have child." << endl; 
}


double SlicingNode::getOccupationArea() const
{
  cerr << "Error(double SlicingNode::getOccupationArea() const) : Occupied Area is not relevant for Routing and Devices." << endl; 
  return 100;
}


void SlicingNode::setNFing( int nfing ) 
{
  cerr << "Error(void SlicingNode::setNFing( int nfing )): Only DlicingNode have NFing parameter." << endl;
}


int SlicingNode::getNFing() const
{
  cerr << "Error(int SlicingNode::getNFing() const): Only DSlicingNode have NFing parameter." << endl;
  return 0;
}


double SlicingNode::getStartParameter() const
{
  cerr << "Error(int SlicingNode::getStartParameter() const): Only DSlicingNode have Start parameter." << endl;
  return 0;
}


double SlicingNode::getStepParameter() const
{
  cerr << "Error(int SlicingNode::getStepParameter() const): Only DSlicingNode have Step parameter." << endl;
  return 0;
}


double SlicingNode::getCountParameter() const
{
  cerr << "Error(int SlicingNode::getCountParameter() const): Only DSlicingNode have Count parameter." << endl;
  return 0;
}


Instance* SlicingNode::getInstance() const
{
  cerr << "Error(Instance* getInstance() const): Only DSlicingNode have an Instance." << endl;
  return NULL;
}


void SlicingNode::setInstance( Instance* instance ) 
{
  cerr << "Error(Instance* setInstance( Instance* instance ) const): Only DSlicingNode have an Instance." << endl;
}


void SlicingNode::setCell( Cell* cell )
{
  if (_parent == NULL){
    if  (checkCellInstances(cell)){
      _cell = cell;
      AnalogCellExtension::setSlicingTree( cell, this );
    } 
  } else {
    cerr << "Error(void SlicingNode::setCell( Cell* cell )): SlicingTree's root only can set the Slicing tree's cell." << endl;
  }
}


void SlicingNode::setKatana( Katana::KatanaEngine* katana )
{
  _katana = katana;
}


bool SlicingNode::checkCellInstances( Cell* cell )
{
  return true;
}


SlicingNode* SlicingNode::findInstance( Instance* instance )
{
  return NULL;
}


SlicingNode* SlicingNode::findSlicingNode( Anabatic::GCell* gcell )
{
  return NULL;
}



void SlicingNode::createSlicingRouting()
{
  cerr << "Error(void SlicingNode::createSlicingRouting()): Used for HVSlicingNode only." << endl;
}


void SlicingNode::resetSlicingRouting()
{
  cerr << "Error(void SlicingNode::resetSlicingRouting()): Used for HVSlicingNode only." << endl;
}


void SlicingNode::destroySlicingRouting()
{
  cerr << "Error(void SlicingNode::destroySlicingRouting()): Used for HVSlicingNode only." << endl;
}


int SlicingNode::getRoutingIndex ( SlicingNode* node ) const 
{
  cerr << "Error(int SlicingNode::getRoutingIndex ( SlicingNode* node ) const ): Routing index is used for Horizontal/Vertical nodes." << endl;
  return 0;
}


SlicingNode* SlicingNode::getSlicingRouting ( int index ) const
{
  cerr << "Error(SlicingNode* SlicingNode::getSlicingRouting ( int index ) const): Used for Horizontal/Vertical Slicing node only." << endl;
  return NULL;
}


SlicingNode* SlicingNode::findHorizontalParent () const
{
  if (_parent) { 
    SlicingNode* node = _parent;
    while(node){
      if (node->getType() == HorizontalSNode){ break; }
      else { node = node->getParent(); }
    }
    return node;
  } else {
    return NULL;
  }
}


SlicingNode* SlicingNode::findVerticalParent () const
{
  if (_parent) { 
    SlicingNode* node = _parent;
    while(node){
      if (node->getType() == VerticalSNode){ break; }
      else { node = node->getParent(); }
    }
    return node;
  } else {
    return NULL;
  }
}


int SlicingNode::getIndexInHorizontalParent() const
{
  if (_parent) { 
    if (this->getParent()->getType() == HorizontalSNode){
      int i = 0;
      vector<SlicingNode*>::const_iterator it = this->getParent()->getChildren().begin();
      while (it != this->getParent()->getChildren().end()){
        if (  ((*it)->getType()   == this->getType()  )
           && ((*it)->getHeight() == this->getHeight()) 
           && ((*it)->getWidth () == this->getWidth ()) 
           && ((*it)->getX()      == this->getX()     ) 
           && ((*it)->getY()      == this->getY()     ) 
           )
          { it = this->getParent()->getChildren().end(); }
        else { 
          i++;
          it++;
        }
      }
      return i;
    } else {
      bool         found = false;
      SlicingNode* node  = this->getParent();

      while((node->getParent() != NULL) && (found == false)){
        if (node->getParent()->getType() == HorizontalSNode){ 
          found = true;
        } else { 
          node = node->getParent(); 
        }
      }

      if (found){
        return node->getParent()->getChildIndex(node);
      } else {
        return -1;
      }
    }
  } else {
    return -1;
  }
}


int SlicingNode::getIndexInVerticalParent() const
{
  if (_parent) { 
    if (this->getParent()->getType() == VerticalSNode){
      int i = 0;
      vector<SlicingNode*>::const_iterator it = this->getParent()->getChildren().begin();
      while (it != this->getParent()->getChildren().end()){
        if (  ((*it)->getType()   == this->getType()  )
           && ((*it)->getHeight() == this->getHeight()) 
           && ((*it)->getWidth () == this->getWidth ()) 
           && ((*it)->getX()      == this->getX()     ) 
           && ((*it)->getY()      == this->getY()     ) 
           )
          { it = this->getParent()->getChildren().end(); }
        else { 
          i++;
          it++;
        }
      }
      return i;
    } else {
      bool         found = false;
      SlicingNode* node  = this->getParent();

      while((node->getParent() != NULL) && (found == false)){
        if (node->getParent()->getType() == VerticalSNode){ 
          found = true;
        } else { 
          node = node->getParent(); 
        }
      }

      if (found){
        return node->getParent()->getChildIndex(node);
      } else {
        return -1;
      }
    }
  } else {
    return -1;
  }
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
  
#if THIS_IS_DISABLED
  for ( Net* net : _cell->getNets() ) {
    cdebug_log(536,0) << "Net: " << net << endl;

    if ( net->getPlugs().getSize() > 1 ) {
      cdebug_log(530,1) << "Not a rail supply and has " <<  net->getPlugs().getSize() << " plugs" << endl;

      vector<Plug*> plugs;
      for ( Plug* plug : net->getPlugs() ) {
        cdebug_log(530,0) << "| " << plug << endl;

        if (dynamic_cast<Device*>(plug->getMasterNet()->getCell())){
          cdebug_log(530,0) << "| Device: " << plug << endl;
          plugs.push_back( plug );
        } else if (isRailSegments(plug)){
          cdebug_log(530,0) << "| PR: " << plug << endl;
          plugs.push_back( plug );
        } else {
          if (not plug->getNet()->isSupply())
            plugs.push_back( plug );
        }
      }

      for ( Plug* plug : plugs ) {
        cdebug_log(536,0) << "Building RoutingPad for " << plug << endl;

        RoutingPad* rp = RoutingPad::create( net, Occurrence(plug) );
        cdebug_log(536,0) << "+ " << rp << ", instance: " << plug->getInstance()->getTransformation() <<  endl;
        cdebug_log(536,0) << "+ instance ab: " << plug->getInstance()->getMasterCell()->getAbutmentBox() <<  endl;

      // Select the Component with the highest layer.
        Net*        masterNet = static_cast<Plug*>( rp->getOccurrence().getEntity() )->getMasterNet();
        Horizontal* hbest     = NULL;
        Vertical*   vbest     = NULL;

        for ( Component* c : masterNet->getComponents() ) {
        //cdebug_log(530,0) << "| " <<  c << endl;

          Horizontal* h = dynamic_cast<Horizontal*>(c);
          Vertical*   v = dynamic_cast<Vertical*>(c);
          
          if (h) {
            if (not hbest) hbest = h;
            else {
              if (h->getLayer()->above(hbest->getLayer())){
                hbest = h;
              } else if (h->getLayer() == hbest->getLayer()){
                if ( hbest->getBoundingBox().getWidth() * hbest->getBoundingBox().getHeight() 
                   <  h->getBoundingBox().getWidth()    * h->getBoundingBox().getHeight() 
                   ) {
                  hbest = h;
                }
              }
            }
          } else if (v) {
            if (not vbest) vbest = v;
            else {
              if (v->getLayer()->above(vbest->getLayer())){
                vbest = v;
              } else if (v->getLayer() == vbest->getLayer()){
                if ( vbest->getBoundingBox().getWidth() * vbest->getBoundingBox().getHeight() 
                   <  v->getBoundingBox().getWidth()    * v->getBoundingBox().getHeight() 
                   ) {
                  vbest = v;
                }
              }
            }
          }
        }

        if (hbest) {
          cdebug_log(536,0) << "* " << hbest << endl;
          rp->setExternalComponent(hbest);
        } else if (vbest) {
          cdebug_log(536,0) << "* " << vbest << endl;
          rp->setExternalComponent(vbest);
        }
      }

      RoutingPad* previousRp = NULL;
      for ( RoutingPad* rp : net->getRoutingPads() ) {
        if ( previousRp
           and (  not rp        ->getBodyHook()->isAttached()
               or not previousRp->getBodyHook()->isAttached()) ) {
          rp->getBodyHook()->attach( previousRp->getBodyHook() );
        }
        previousRp = rp;
      }
      cdebug_tabw(530,-1);
    }
  }
#endif

  cdebug_tabw(536,-1);
}


void SlicingNode::clearRoutingPads()
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
} 


void SlicingNode::restrictDevices()
{
  if (_gcell){
    Anabatic::Vertex* vertex = _gcell->getObserver< Anabatic::Vertex>( Anabatic::GCell::Observable::Vertex);
    vertex->clearRestriction();
  }
}


void SlicingNode::setVertexRestriction ( Net* net, Katana::KatanaEngine* kanata ){}


void SlicingNode::estimateChannelsSize ()
{
  setRoutingEstimated (RoutingEstimated); 
}


void SlicingNode::expandRoutingChannel () 
{
  setRoutingEstimated (RoutingEstimated); 
}


void SlicingNode::_expandRoutingChannel () 
{
  setRoutingEstimated (RoutingEstimated); 
}


void SlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width ) 
{
  setRoutingEstimated (RoutingEstimated); 
}


void SlicingNode::clearGlobalRouting()
{
  vector<Component*> c = vector<Component*>();
  c.clear();
  vector<Component*> s = vector<Component*>();

  int hcpt = 0;
  int vcpt = 0;
  int ccpt = 0;

  for(Net* net: _cell->getNets()){
    for(Component* component: net->getComponents()){
      if (SlicingNode::isRailSegments(dynamic_cast<Segment*> (component)) == false){
        Hurricane::Horizontal* h  = dynamic_cast<Hurricane::Horizontal*>(component);
        Hurricane::Vertical*   v  = dynamic_cast<Hurricane::Vertical*>  (component);
        Hurricane::Contact*    cc = dynamic_cast<Hurricane::Contact*>   (component);
        
        if (h)  s.push_back(component);
        if (v)  s.push_back(component);
        if (cc) c.push_back(component);
        
        if (h)  hcpt++;
        if (v)  vcpt++;
        if (cc) ccpt++;
      }
    }
  }
  for (vector<Component*>::iterator it = s.begin(); it != s.end(); it++){
    (*it)->destroy();
  }
  for (vector<Component*>::iterator it = c.begin(); it != c.end(); it++){
    (*it)->destroy();
  }
  this->clearGCells();
} 


void SlicingNode::clearGCells()
{
  _gcell = NULL;
}


void SlicingNode::updateGCellPosition()
{
  if (_gcell) {
    _gcell->setSouthWestCorner( getX(), getY() );
  }
  cdebug_log(535,0) << "SlicingNode::updateGCellPosition(): " << _gcell << endl;
}


void SlicingNode::updateGContacts()
{
  if (_gcell) {
    _gcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
  }
  cdebug_log(535,0) << "SlicingNode::updateGContacts(): " << _gcell << endl;
}


/*<<<<<<< HEAD*/
void SlicingNode::updateMatrixGCellPosition()
{
  cerr << "Error(void SlicingNode::updateMatrixGCellPosition()): Only used by DSlicingNode" << endl;
}

void SlicingNode::updateMatrixGContacts()
{
  cerr << "Error(void SlicingNode::updateMatrixGContacts()): Only used by DSlicingNode" << endl;
}

/*void SlicingNode::updateMatrixGContactsPosition()
{
  cerr << "Error(void SlicingNode::updateMatrixGContactsPosition()): Only used by DSlicingNode" << endl;
}


void SlicingNode::updateGContactsPosition()
{
  if (_gcell)_gcell->updateContactsPosition();
}


=======
>>>>>>> ad94201dbefd0e6f98b1e28a032a3b2a6d57390d*/
int SlicingNode::getNbDevices()
{
  return 1;
}
    

bool SlicingNode::containAxis( DbU::Unit axis, unsigned int symmetryType )
{
  if        (symmetryType == HSymmetry){
    return ( (axis > getX()) && (axis < getX()+getWidth()));
  } else if (symmetryType == VSymmetry){
    return ( (axis > getY()) && (axis < getY()+getHeight()));
  } else return false;
}

    
bool SlicingNode::checkSymmetryNet ( unsigned int type, Net* net1, Net* net2 ) const
{
  cerr << "Error(bool checkSymmetryNet ( unsigned int type, Net* net1, Net* net2 ) const): Only used by HVSlicingNode" << endl;
  return false;
}


void SlicingNode::addSymmetryNet ( unsigned int type, Net* net1, Net* net2 )
{
  cerr << "Error(void addSymmetryNet ( unsigned int type,  Net* net1, Net* net2 )): Only used by HVSlicingNode" << endl;
}


void SlicingNode::updateNetConstraints ()
{
  cerr << "Error(void SlicingNode::updateNetConstraints ()): Only used by HVSlicingNode" << endl;
}



void SlicingNode::updateSymNetAxis()
{
  cerr << "Error(void SlicingNode::updateSymNetAxis ()): Only used by HVSlicingNode" << endl;
}

void SlicingNode::setRoutingGauge ( CRL::RoutingGauge* rg )
{
  _rg     = rg; 
}


void SlicingNode::addRailSegments( Hurricane::Segment* s )
{
  _railSegments.push_back(s);
}


bool SlicingNode::isRailSegments ( Hurricane::Segment* s )
{
  bool is = false;
  for (Segment* seg : _railSegments){
    if ( s == seg ) {
      is = true;
      break;
    }
  }
  return is;
}

bool SlicingNode::isRailSegments ( Hurricane::Plug* p )
{
  Instance* i = p->getInstance();
    Net* net = NULL;
    int t = 0;
    for (Net* n : i->getMasterCell()->getNets()) {
      net = n;
      t++;
    }
    if (t != 1) return false;
    Component* component = NULL;
    t = 0;
    for (Component* c : net->getComponents()){
      component = c;
      t++;
    }
    if (t != 1) return false;

    Horizontal* h = dynamic_cast<Horizontal*>(component);
    Vertical*   v = dynamic_cast<Vertical*>  (component);
    if (h)      return isRailSegments(h);
    else if (v) return isRailSegments(v);
    else        return false;
}


void  SlicingNode::flattenDigitalNets ()
{
  cerr << "Error(void SlicingNode::flattenDigitalNets ()): Only used by HVSlicingNode" << endl;
}

void SlicingNode::updateWireOccupation ( Anabatic::VertexSet )
{
  cerr << "Error(void SlicingNode::updateWireOccupation ( Anabatic::VertexSet )): Only used by HVSlicingNode" << endl;
}

void SlicingNode::addWireOccupation ( DbU::Unit min, DbU::Unit max )
{
  cerr << "Error(void SlicingNode::addWireOccupation ( DbU::Unit min, DbU::Unit max )): Only used by HVSlicingNode and RHVSlicingNode" << endl;
}

void SlicingNode::resetWireOccupation ()
{
  cerr << "Error(void SlicingNode::resetWireOccupation()): Only used by HVSlicingNode and RHVSlicingNode" << endl;
}


int SlicingNode::getMaxWireOccupation ()
{
  cerr << "Error(int SlicingNode::getMaxWireOccupation()): Only used by HVSlicingNode and RHVSlicingNode" << endl;
  return 0;
}


// -----------------------------------------------------------------------------------------------//
// Class : HVSlicingNode
// -----------------------------------------------------------------------------------------------//


HVSlicingNode::HVSlicingNode( unsigned int type, unsigned int alignment ): SlicingNode( type, NodeSets::create(), alignment, NULL )
{
  _toleranceRatioH = 0;
  _toleranceRatioW = 0;
  _toleranceBandH  = 0;
  _toleranceBandW  = 0;
  _children        = vector<SlicingNode*>();
  _symmetries      = list<pair <int,int> >();
  _slicingRouting  = vector<RHVSlicingNode*>();
  _netsymmetries   = vector<tuple<unsigned int, Net*, Net*> >();
}


HVSlicingNode::~HVSlicingNode(){}


  
void  HVSlicingNode::setToleranceRatioH ( DbU::Unit tolerance ) 
{
  _toleranceRatioH = tolerance; 
  getRoot()->_resetSlicingTree();
}


void  HVSlicingNode::setToleranceRatioW ( DbU::Unit tolerance ) 
{
  _toleranceRatioW = tolerance; 
  getRoot()->_resetSlicingTree();
}


void  HVSlicingNode::setToleranceBandH  ( DbU::Unit tolerance ) 
{
  _toleranceBandH = tolerance; 
  resetSlicingTree();
}


void  HVSlicingNode::setToleranceBandW  ( DbU::Unit tolerance ) 
{
  _toleranceBandW = tolerance; 
  resetSlicingTree();
}


void HVSlicingNode::recursiveSetToleranceRatioH( DbU::Unit tolerance )
{
  _recursiveSetToleranceRatioH(tolerance);
  getRoot()->_resetSlicingTree();
}


void HVSlicingNode::recursiveSetToleranceRatioW( DbU::Unit tolerance )
{
  _recursiveSetToleranceRatioW(tolerance);
  getRoot()->_resetSlicingTree();
}


void HVSlicingNode::recursiveSetToleranceBandH( DbU::Unit tolerance )
{
  resetSlicingTree();
  _recursiveSetToleranceBandH(tolerance);
}


void HVSlicingNode::recursiveSetToleranceBandW( DbU::Unit tolerance )
{
  resetSlicingTree();
  _recursiveSetToleranceBandW(tolerance);
}


void HVSlicingNode::_recursiveSetToleranceRatioH( DbU::Unit tolerance )
{
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){

    if ( (!(*it)->isDevice()) && (!(*it)->isRouting()) )
      { (*it)->recursiveSetToleranceRatioH(tolerance); }
  }
  _toleranceRatioH = tolerance; 
}


void HVSlicingNode::_recursiveSetToleranceRatioW( DbU::Unit tolerance )
{
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){

    if ( (!(*it)->isDevice()) && (!(*it)->isRouting()) )
      { (*it)->recursiveSetToleranceRatioW(tolerance); }
  }
  _toleranceRatioW = tolerance; 
}


void HVSlicingNode::_recursiveSetToleranceBandH( DbU::Unit tolerance )
{
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){

    if ( (!(*it)->isDevice()) && (!(*it)->isRouting()) )
      { (*it)->recursiveSetToleranceBandH(tolerance); }
  }
  _toleranceBandH = tolerance; 
}


void HVSlicingNode::_recursiveSetToleranceBandW( DbU::Unit tolerance )
{
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){

    if ( (!(*it)->isDevice()) && (!(*it)->isRouting()) )
      { (*it)->recursiveSetToleranceBandW(tolerance); }
  }
  _toleranceBandW = tolerance; 
}


void HVSlicingNode::setTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  resetSlicingTree();
  _toleranceRatioH = trh;
  _toleranceRatioW = trw;
  _toleranceBandH  = tbh;
  _toleranceBandW  = tbw;
}


void HVSlicingNode::recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  resetSlicingTree();
  _recursiveSetTolerances( trh, trw, tbh, tbw );
}


void HVSlicingNode::_recursiveSetTolerances( DbU::Unit trh, DbU::Unit trw, DbU::Unit tbh, DbU::Unit tbw )
{
  _recursiveSetToleranceRatioH(trh);
  _recursiveSetToleranceRatioW(trw);
  _recursiveSetToleranceBandH(tbh);
  _recursiveSetToleranceBandW(tbw);
}


bool HVSlicingNode::hasEmptyChildrenNodeSets() const
{
  bool flag = false;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    if ( (node->empty() != true) && (!(*it)->isRouting()) )
      { if (node->empty() == true) {flag = true;} }
  }
  return flag;
}


int HVSlicingNode::getChildIndex( SlicingNode* node ) const
{
  int i = 0;
  vector<SlicingNode*>::const_iterator it = _children.begin();
  while (it != _children.end()){
    if (  ((*it)->getType()   == node->getType()  )
       && ((*it)->getHeight() == node->getHeight()) 
       && ((*it)->getWidth () == node->getWidth ()) 
       && ((*it)->getX()      == node->getX()     ) 
       && ((*it)->getY()      == node->getY()     ) 
       )
      { it = _children.end(); }
    else { 
      i++;
      it++;
    }
  }
  return i;
}


void HVSlicingNode::createChild( unsigned int type, unsigned int alignment )
{
  if (type == HorizontalSNode) {
    HSlicingNode* hsn = HSlicingNode::create(alignment);
    hsn->setTolerances( getToleranceRatioH()
                      , getToleranceRatioW()
                      , getToleranceBandH()
                      , getToleranceBandW()
                      );
    push_back(hsn);
    resetSlicingTree();
  }

  else if (type == VerticalSNode) { 
    VSlicingNode* vsn = VSlicingNode::create(alignment);
    vsn->setTolerances( getToleranceRatioH()
                      , getToleranceRatioW()
                      , getToleranceBandH()
                      , getToleranceBandW()
                      );
    push_back(vsn); 
    resetSlicingTree();
  }
  else 
    { cerr << " Error(void HVSlicingNode::createChild(SlicingType type, Alignment alignment)): Unknown type." << endl; }
}   


void HVSlicingNode::createChild( NodeSets*    nodeSets
                               , unsigned int alignment
                               , Instance*    instance
                               , BoxSet*      boxSet
                               )
{
  DSlicingNode* node = DSlicingNode::create( nodeSets, alignment, instance, boxSet );
  node->setParent(this);
  this->push_back(node); 
  resetSlicingTree();
}
  

void HVSlicingNode::createChild( int childIndex, int copyIndex, unsigned int tr )
{
  if (childIndex != copyIndex){
      
    SlicingNode* node = this->getChild(childIndex)->clone(tr);
    this->insertNode( node, copyIndex ); 
    _symmetries.push_back( pair<int,int>(min(childIndex, copyIndex), max(childIndex, copyIndex)) );
    resetSlicingTree();
    normalizeSymmetries();
  }
  else { cerr << "Error(void HVSlicingNode::createChild( int childIndex, int copyIndex, unsigned int tr )): Indexes cannot be the same." << endl; }
}  


void HVSlicingNode::insertNode( SlicingNode* node, int index )
{
  vector<SlicingNode*>::iterator it = _children.begin();
  for (int i = 0; i < index; i++){ if (it != _children.end()){ it++; } }
  _children.insert(it,node);
  node->setParent(this);
  resetSlicingTree();
}


void HVSlicingNode::push_back( SlicingNode* node, bool reset ) 
{
  node->setParent(this);
  _children.push_back(node); 
  if (reset) { resetSlicingTree(); }
}


void HVSlicingNode::push_front( SlicingNode* node ) 
{
  node->setParent(this);
  _children.insert(_children.begin(), node); 
  resetSlicingTree();
}


void HVSlicingNode::removeNode( SlicingNode* node ) 
{
  int  index = 0;
  bool found = false;
  vector<SlicingNode*>::iterator it = _children.begin();
//(*it)->printLine();
  while( it != _children.end() ){
    if ((*it) == node) {  
        _children.erase(it);  
        found = true;
        it = _children.end();
      }
    else {
      it++;
      index++; 
    }
  }

  if (found == true){
    removeSymmetry(index);
    resetSlicingTree();
    node->removeParent();
    node->setX(0);
    node->setY(0);
    node->setPlaced(false);
  } /*else {
    cerr << "Didn't found" << endl;
    }*/
}


bool HVSlicingNode::isSymmetry( int index, pair<int,int>& symmetry )
{
  bool symmetryFound = false;
    
  if (_symmetries.empty() != true){
    for (list<pair<int,int> >::const_iterator it2 = _symmetries.begin(); it2 != _symmetries.end(); it2++){
        
      if ((*it2).second == index){ 
        symmetry = pair<int,int>((*it2).first,(*it2).second); 
        symmetryFound = true;
      }
    }
  }
  return symmetryFound;
}


bool HVSlicingNode::isSymmetry( int index )
{
  bool symmetryFound = false;
    
  if (_symmetries.empty() != true){
    for (list<pair<int,int> >::const_iterator it2 = _symmetries.begin(); it2 != _symmetries.end(); it2++){
        
      if ((*it2).second == index){ 
        symmetryFound = true;
      }
    }
  }
  return symmetryFound;
}


void HVSlicingNode::addSymmetry( int childIndex, int copyIndex, bool reset )
{
  if (childIndex >= (int)getChildren().size()) {
    cerr << Warning( "HVSlicingNode::addSymmetry(int,int)): Child index out of range:%d (> %d), symmetry is ignored." 
                   , childIndex, getChildren().size() ) << endl;
    return;
  }

  if (copyIndex >= (int)getChildren().size()) {
    cerr << Warning( "HVSlicingNode::addSymmetry(int,int)): Copy index out of range:%d (> %d), symmetry is ignored." 
                   , copyIndex, getChildren().size() ) << endl;
    return;
  }

  if (childIndex == copyIndex) {
    cerr << Warning( "HVSlicingNode::addSymmetry(int,int)): Indexes cannot be identical (%d), symmetry is ignored." 
                   , childIndex ) << endl;
    return;
  }
  unsigned int symmetryType = 0;
  if (this->getType() == HorizontalSNode) symmetryType = HSymmetry;
  if (this->getType() == VerticalSNode  ) symmetryType = VSymmetry;
  
  
  if (not getChild(childIndex)->isSymmetric(getChild(copyIndex), symmetryType, ShowDiff)) { 
    cerr << Warning( "HVSlicingNode::addSymmetry(int,int)): Children %d and %d are not the same, symmetry is ignored." 
                   , childIndex, copyIndex ) << endl;
    return;
  }
  
  _symmetries.push_back( pair<int,int>(min(childIndex, copyIndex), max(childIndex, copyIndex)) ); 
  if (reset) resetSlicingTree();
  unsigned int flag = 0;
  if (this->getType() == HorizontalSNode) flag = HSymmetry;
  if (this->getType() == VerticalSNode  ) flag = VSymmetry;
  getChild(copyIndex)->setSymmetryFlag(flag);

  getChild(copyIndex)->setMaster(getChild(childIndex));
  normalizeSymmetries();
}

  
void HVSlicingNode::removeSymmetry( int index )
{
  bool first        = true;
  int  erasedFirst  = 0;
  int  erasedSecond = 0;
  bool isReference  = true;

  for ( list<pair<int,int> >::iterator it = _symmetries.begin(); it != _symmetries.end(); it++ ){
    if ((((*it).first == index) || ((*it).second == index) ) && (first == true)){
      list<pair<int,int> >::iterator itToerase = it;
      it++;
      resetSlicingTree(); 
      first = false;
        
      if ((*it).first == index){
        erasedFirst  = (*itToerase).first;
        erasedSecond = (*itToerase).second;
      } else { isReference = false; }

      _symmetries.erase(itToerase);
      it--;
    } 
    else { 
      if ( (first == false) && (isReference) ){
        if ((*it).first == erasedFirst){ (*it).first = erasedSecond; }
      } 
      if ((*it).first  > index) { (*it).first--;  }
      if ((*it).second > index) { (*it).second--; }
    }
  }
}

void HVSlicingNode::normalizeSymmetries()
{
// -----------------------------------------------------------------------------------------------//
// Notes : Symmetries must be described in a specific way, in case it is not, this method correct it.
// Examples: 
// WRONG: Symmetries [< 1, 2 >, < 2, 3 >, < 3, 4 >]; RIGHT: [< 1, 2 >, < 1, 3 >, < 1, 4 >];
// WRONG: Symmetries [< 3, 4 >, < 5, 6 >, < 1, 2 >]; RIGHT: [< 1, 2 >, < 3, 4 >, < 5, 6 >];
// WRONG: Symmetries [< 1, 2 >, < 1, 2 >, < 3, 4 >]; RIGHT: [< 1, 2 >, < 3, 4 >];
// -----------------------------------------------------------------------------------------------//

  list<pair<int,int> > adjustedSymmetries = list<pair<int,int> >();
  for (list<pair<int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++){ 

    if ((*it).first > (*it).second ){ adjustedSymmetries.push_back(pair<int,int >((*it).second,(*it).first)); } 
    else { adjustedSymmetries.push_back(pair<int,int >((*it).first,(*it).second)); } 
  }
  adjustedSymmetries.sort();

  bool next = false;
  list<pair<int,int> >::iterator it2 = adjustedSymmetries.begin();
  it2++;

  for (list<pair<int,int> >::iterator it = adjustedSymmetries.begin(); it != adjustedSymmetries.end(); it++){
    it2 = it;
    it2++;

    while(next != true){
      if ( (*it).second == (*it2).first ){ (*it2).first = (*it).first; }
      if ( (*it).second <  (*it2).first ){ next = true; }
      it2++;
      if ( it2 == adjustedSymmetries.end() ) { next = true; }
    }
    next = false;
  }
  _symmetries = adjustedSymmetries;
}


void HVSlicingNode::_resetSlicingTree()
{
  _x = 0;
  _y = 0;
  setPlaced(false);
    
  if (!this->isPreset()){
    _boxSet = NULL;
    setSet(false);
      
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
      (*it)->_resetSlicingTree();
    }
  }
}


void HVSlicingNode::print() const
{
  SlicingNode::print();
  if (_symmetries.empty() == false){
    cerr << "Symmetries: " << endl;
    for (list<pair<int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++)
      { cerr << "Children: " << (*it).first << " and " << (*it).second << endl;  }
    cerr << endl;
  } else {
    cerr << "Symmetries: None" << endl;
  }
  if (_slicingRouting.empty() == false){
    cerr<< "Slicing Routing: " << endl;
    int index = 0;
    for (vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
      cerr << "---------------- " << setprecision(4) << index << " -----------------" << endl;
      cerr << "Print - Slicing Routing: ";
      cerr << "X: "      << DbU::getPhysical((*it)->getX(), DbU::Micro);
      cerr << ", Y: "      << DbU::getPhysical((*it)->getY(), DbU::Micro);
      cerr << ", \t height: " << DbU::getPhysical((*it)->getHeight(), DbU::Micro);
      cerr << ", width: "  << DbU::getPhysical((*it)->getWidth(), DbU::Micro) << endl;
      cerr << "GCell: " << (*it)->getGCell() << endl;
      if((*it)->getGCell()){
        cerr << "GCell      : " << (*it)->getGCell() << endl;
      
        cerr << "Edges : " << endl;
        vector<Anabatic::Edge*> ne = (*it)->getGCell()->getNorthEdges();
        vector<Anabatic::Edge*> se = (*it)->getGCell()->getSouthEdges();
        vector<Anabatic::Edge*> ee = (*it)->getGCell()->getEastEdges();
        vector<Anabatic::Edge*> we = (*it)->getGCell()->getWestEdges();

        cerr << "--- North : " << ne.empty() << endl;
        for (vector<Anabatic::Edge*>::const_iterator itn = ne.begin(); itn != ne.end(); itn++){
          cerr << (*itn)->getOpposite((*it)->getGCell()) << endl;
        }
        cerr << "--- South : " << se.empty()<< endl;
        for (vector<Anabatic::Edge*>::const_iterator its = se.begin(); its != se.end(); its++){
          cerr << (*its)->getOpposite((*it)->getGCell()) << endl;
        }
        cerr << "--- East : " << ee.empty()<< endl;
        for (vector<Anabatic::Edge*>::const_iterator ite = ee.begin(); ite != ee.end(); ite++){
          cerr << (*ite)->getOpposite((*it)->getGCell()) << endl;
        }
        cerr << "--- West : "<< we.empty() << endl;
        for (vector<Anabatic::Edge*>::const_iterator itw = we.begin(); itw != we.end(); itw++){
          cerr << (*itw)->getOpposite((*it)->getGCell()) << endl;
        }
      }
      cerr << "------------------------------------" << endl;
      index++;
    }
  } else {
    cerr<< "Slicing Routing: empty" << endl;
  }
  cerr << endl;
}


void HVSlicingNode::printChildren() const
{
  int count = 0;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    if ( ((*it)->isDevice()) || ((*it)->isRouting()) ){
      cerr << "-- Children: " << count << "/" << _children.size()-1 << " --" << endl;
      (*it)->print(); 
    }
    else {
      cerr << "-- Children: " << count << "/" << _children.size()-1 << " --" << endl;
      (*it)->print();
      (*it)->printChildren();
    }
    count++;
  }
}


void HVSlicingNode::printLine() const
{
  SlicingNode::printLine();

  if (_symmetries.empty() == false){
    cerr << "Symmetries: " << endl;
    for (list<pair<int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++)
      { cerr << "Children: " << (*it).first << " and " << (*it).second << endl;  }
    cerr << endl;
  }

  /*int index = 0;
    cerr << "Print - Slicing Routing: " << endl;
  for (vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    cerr << "Index: "    << index;
    cerr << ", X: "      << DbU::getPhysical((*it)->getX(), DbU::Micro);
    cerr << ", Y: "      << DbU::getPhysical((*it)->getY(), DbU::Micro);
    cerr << ", \t height: " << DbU::getPhysical((*it)->getHeight(), DbU::Micro);
    cerr << ", width: "  << DbU::getPhysical((*it)->getWidth(), DbU::Micro);
    if ((*it)->getMaster()){
      cerr << ", MASTER: ";
      (*it)->getMaster()->printLine();
    } else {
      cerr << endl;
    }
    index++;
    }*/
}


void HVSlicingNode::printChildrenLine() const
{
  int count = 0;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    if ( ((*it)->isDevice()) || ((*it)->isRouting()) ){
      cerr << "-- Children: " << count << "/" << _children.size()-1 << " --" << endl;
      (*it)->printLine(); 
    }
    else {
      cerr << endl;
      cerr << "-- Children: " << count << "/" << _children.size()-1 << " --" << endl;
      (*it)->printLine();
      (*it)->printChildrenLine();
    }
    count++;
  }
}


bool HVSlicingNode::recursiveCheckPreset() const
{
  bool isPreset = this->isPreset();
  if (isPreset){
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
      isPreset = (*it)->recursiveCheckPreset(); 
    }
  }
  return isPreset;
}


bool HVSlicingNode::recursiveCheckSet() const
{
  bool isSet = this->isSet();   
  if (isSet){
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
      isSet = (*it)->recursiveCheckSet(); 
    }
  }
  return isSet;
}


bool HVSlicingNode::recursiveCheckPlaced() const
{
  bool isPlaced = this->isPlaced();
  if (isPlaced){
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
      isPlaced = (*it)->recursiveCheckPlaced(); 
    }
  }
  return isPlaced;
}


int HVSlicingNode::getLeafNumber() const
{
  int cpt = 0;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++)
    { cpt += (*it)->getLeafNumber(); }
  return cpt;  
}


double HVSlicingNode::getDevicesArea() const
{
  double area = 0;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
    area += (*it)->getDevicesArea();
  }
  return area;
}


double HVSlicingNode::getOccupationArea() const
{
  double estimation = 0;
  if(recursiveCheckPlaced())
    {
      estimation = getDevicesArea()/(getHeight() * getWidth())*100;
    }
  else { cerr << "Error(double HVSlicingNode::getSpaceEstimation()): SlicingNodes dimensions need to be set first before being estimated." << endl; }
  return estimation;
}


void HVSlicingNode::setGlobalSize( DbU::Unit height, DbU::Unit width )
{
  if ( (_nodeSets->empty() != true) ){
    vector<BoxSet*>::const_iterator it = _nodeSets->begin();
    DbU::Unit  bestH    = 0;
    DbU::Unit  bestW    = 0;
    DbU::Unit  currentH = 0;
    DbU::Unit  currentW = 0;
    BoxSet* boxSet  = (*_nodeSets->begin());

    while (it != _nodeSets->end()){
      currentH = (*it)->getHeight();
      currentW = (*it)->getWidth();
      if ( (currentH <= height) && (currentW <= width) ){
        if ( ((height-currentH) <= _toleranceRatioH) && ((height-bestH) <= _toleranceRatioH) ) {
          if (currentW > bestW){ 
            bestH  = currentH;
            bestW  = currentW;
            boxSet = (*it);
          }
        } else if (currentH > bestH) {
          bestH  = currentH;
          bestW  = currentW;
          boxSet = (*it);
        }
      }
      it++;
    } 
    this->_setGlobalSize(boxSet);

  } else  { cerr << "Error(void HVSlicingNode::setGlobalSize( DbU::Unit height, DbU::Unit width )): NodeSets empty. UpdateGlobalSize needs to be used first or with higher tolerances." << endl; }
}


void HVSlicingNode::setGlobalSize( size_t index )
{
  if ( _nodeSets->empty() != true ){
    if (index > (_nodeSets->size()-1)){ cerr << "Error(void HVSlicingNode::setGlobalSize( size_t index )): Out of bound index." << endl; }
    else {
      this->_setGlobalSize((*(_nodeSets->begin() + index)));
    }
  } else  { cerr << "Error(void HVSlicingNode::setGlobalSize( int index )): NodeSets empty. UpdateGlobalSize needs to be used first or with higher tolerances." << endl; }
}


void HVSlicingNode::_setGlobalSize ( BoxSet* boxSet ) 
{
//cerr << "void HVSlicingNode::_setGlobalSize ( BoxSet* boxSet ) " << endl;
  if ( (!this->isPreset()) ){
    if (this->getMaster() == NULL) {
      this->_setBoxSet(boxSet);  
    } else {
      this->_setBoxSet(_master->getBoxSet());  
    }
    if (  ((getType() == HorizontalSNode) && (isHSymmetry()))
       || ((getType() == VerticalSNode  ) && (isVSymmetry()))
       ){
      vector<BoxSet*>::const_iterator itBoxSet = boxSet->getSet().begin();
      for (vector<SlicingNode*>::reverse_iterator it = _children.rbegin(); it != _children.rend(); ++it){
        (*it)->_setGlobalSize((*itBoxSet));
        itBoxSet++;
      }
    } else {
      vector<BoxSet*>::const_iterator itBoxSet = boxSet->getSet().begin();
      for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
        (*it)->_setGlobalSize((*itBoxSet));
        itBoxSet++;
      }
    }
  }
}
    


void HVSlicingNode::preDestroy()
{
  SlicingNode::preDestroy();
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->removeParent();
  }
}


void HVSlicingNode::destroy()
{
  HVSlicingNode::preDestroy();
  delete(this);
}


void HVSlicingNode::preRecursiveDestroy()
{
  SlicingNode::preRecursiveDestroy();
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    if ( ((*it)->getType() == HorizontalSNode) || ((*it)->getType() == VerticalSNode) ){
      (*it)->removeParent();
      (*it)->recursiveDestroy();
    } else {
      (*it)->removeParent();
      (*it)->destroy();
    }
  }
}


void HVSlicingNode::recursiveDestroy()
{
  HVSlicingNode::preRecursiveDestroy();
  delete(this);
}

void printList( list<SlicingNode*> liste )
{
  for ( list<SlicingNode*>::const_iterator it = liste.begin(); it != liste.end(); it++ ){
    (*it)->print(); 
  }
}

list<SlicingNode*> HVSlicingNode::getLeaves () const
{
  list<SlicingNode*> leaves = list<SlicingNode*>();
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    if ( ((*it)->getType() == DeviceSNode) || ((*it)->getType() == RoutingSNode) ){
      leaves.push_back((*it));
    } else {
      leaves.splice(leaves.end(), (*it)->getLeaves());
    }
  }
  return leaves;
}


bool HVSlicingNode::checkInitialPlacement( int& cpt ) const
{
// -----------------------------------------------------------------------------------------------//
// Notes:: Initial placement criteria is having at least 2 devices placed at (x, y) = (0, 0)
// -----------------------------------------------------------------------------------------------//

  bool initialPlacement = false;
  if (cpt < 2){
    initialPlacement = true;
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
      if (cpt < 2){ initialPlacement = (*it)->checkInitialPlacement(cpt); } 
    }
  }
  return initialPlacement;
}


bool HVSlicingNode::isSame( SlicingNode* node, unsigned int flags ) const
{
  if (   ( getType()           == node->getType()           )
     and ( getNbChild()        == node->getNbChild()        ) 
     and ( getToleranceBandH() == node->getToleranceBandH() )
     and ( getToleranceBandW() == node->getToleranceBandW() ) ) {
    bool isSame = true;
 
    for ( size_t ichild = 0 ; isSame and (ichild < getNbChild()) ; ++ichild ) {
      isSame = getChild(ichild)->isSame( node->getChild(ichild) );
    }

    if (not isSame and (flags & ShowDiff))
      cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs (childs)." << endl;

    return isSame;
  } else {
    if (flags & ShowDiff)
      cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs." << endl;
    return false;
  }
}


/*bool  HVSlicingNode::checkCellInstances ( Cell* cell )
{
  if (cell) {
    bool found = true;
    for ( Instance* instance : cell->getInstances() ) {
      if (not findInstance(instance)) {
        found = false;
        cerr << Warning( "HVSlicingNode::checkCellInstances() Instance \"%s\" is missing from the SlicingTree. "
                       , getString(instance->getName()).c_str()
                       ) << endl;
      }
    }
    return found;
  }

  return false;
}*/
bool HVSlicingNode::checkCellInstances ( Cell* cell )
{
  if (cell) {
    bool found = true;
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
      if (not (*it)->checkCellInstances(cell)) found = false;
    }
    return found;
  } else {
    cerr << "HVSlicingNode::checkCellInstances() Cell is NULL. " << endl;
    return false;
  }
}

SlicingNode* HVSlicingNode::findInstance( Instance* instance )
{
  SlicingNode* node = NULL;
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
    if( ((*it)->getType() == HorizontalSNode) || ((*it)->getType() == VerticalSNode) ){
      SlicingNode* inode = (*it)->findInstance(instance);
      if (inode != NULL){ node = inode; }
    } else if ((*it)->getType() == DeviceSNode){
      if ( (*it)->getInstance() == instance ){
        node = (*it);
      }
    }
  }
  return node;
}



SlicingNode* HVSlicingNode::findSlicingNode( Anabatic::GCell* gcell )
{
  SlicingNode* node = NULL;
  for (vector<RHVSlicingNode*>::iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); ++it){
    if ( (*it)->getGCell() == gcell ){
      node = (*it);
      break;
    }
  }
  
  if (node == NULL){
    for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); ++it){ 
      if( ((*it)->getType() == HorizontalSNode) || ((*it)->getType() == VerticalSNode) ){
        SlicingNode* inode = (*it)->findSlicingNode(gcell);
        if (inode != NULL){ node = inode; }
      } else if ((*it)->getType() == DeviceSNode){
        if ( (*it)->getGCell() == gcell ){
          node = (*it);
          break;
        }
      }
    }
  }
  return node;
}


void HVSlicingNode::resetSlicingRouting()
{
  for (vector<RHVSlicingNode*>::iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    (*it)->resetSize();
  }
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
    if ( ((*it)->getType() == HorizontalSNode)||((*it)->getType() == VerticalSNode) ){
      (*it)->resetSlicingRouting();
    }
  }
  this->setRoutingEstimated(0);
}


void HVSlicingNode::destroySlicingRouting()
{
  for (vector<RHVSlicingNode*>::iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    (*it)->destroy();
  }
  _slicingRouting.clear();
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
    if ( ((*it)->getType() == HorizontalSNode)||((*it)->getType() == VerticalSNode) ){
      (*it)->destroySlicingRouting();
    }
  }
  this->setRoutingEstimated(0);
}


int HVSlicingNode::getRoutingIndex( SlicingNode* node ) const
{
  int i = 0;
  vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin();
  while (it != _slicingRouting.end()){
    if (  ((*it)->getHeight() == node->getHeight()) 
       && ((*it)->getWidth () == node->getWidth ()) 
       && ((*it)->getX()      == node->getX()     ) 
       && ((*it)->getY()      == node->getY()     ) 
       )
      { it = _slicingRouting.end(); }
    else { 
      i++;
      it++;
    }
  }
  return i;
}


SlicingNode* HVSlicingNode::getSlicingRouting( int index ) const
{
  return (*(_slicingRouting.begin()+index));
}


void HVSlicingNode::restrictDevices()
{
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){ 
    (*it)->restrictDevices();
  }
  for (std::vector<RHVSlicingNode*>::iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    (*it)->restrictDevices();
  }
}


void HVSlicingNode::setVertexRestriction( Net* net, Katana::KatanaEngine* katana )
{ 
  cdebug_log(536,1) << "HVSlicingNode::setVertexRestriction(Net*,KatanaEngine*)" << endl;

  this->restrictDevices();
  vector<RoutingPad*> rps;
  for ( RoutingPad* rp : net->getRoutingPads() ) rps.push_back( rp );
  for ( RoutingPad* rp : rps ) {
    Box    rpBb   = rp->getBoundingBox();
    Point  center = rpBb.getCenter();
    Anabatic::GCell* gcell  = katana->getGCellUnder( center );
        
    if (not gcell) {
      cerr << Error( "Dijkstra::load(): %s of %s is not under any GCell.\n"
                   "        It will be ignored ans the routing will be incomplete."
                   , getString(rp  ).c_str()
                   , getString(net).c_str()
                   ) << endl;
      continue;
    }

    Anabatic::Vertex* vertex = gcell->getObserver<Anabatic::Vertex>(Anabatic::GCell::Observable::Vertex);
  // Analog Restrictions
    Plug*                plug   = dynamic_cast<Plug*>(rp->getPlugOccurrence().getEntity());
    Cell*                cell   = plug->getInstance()->getMasterCell();
    Device*              device = dynamic_cast<Device*          >(cell);

    if (device){
      cdebug_log(536,0) << "Underlying device: " << device << endl;

      unsigned int rule = device->getRestrictions2(plug->getMasterNet());
      vertex->clearRestriction();

      cdebug_log(536,0) << "Restrictions rule: " << rule << endl;
      if (rule & WestBlocked ) vertex->setWRestricted();
      if (rule & EastBlocked ) vertex->setERestricted();
      if (rule & SouthBlocked) vertex->setSRestricted();
      if (rule & NorthBlocked) vertex->setNRestricted();
      cdebug_log(536,0) << "Applied restrictions: " << vertex << endl;
    } else {
      vertex->clearRestriction();
    }
  }

  cdebug_tabw(536,-1);
}


void HVSlicingNode::estimateChannelsSize()
{
  for (std::vector<RHVSlicingNode*>::iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    (*it)->estimateChannelsSize();
  }
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->estimateChannelsSize();
  }
}


void HVSlicingNode::expandRoutingChannel()
{
  estimateChannelsSize();
  _expandRoutingChannel();
}


void HVSlicingNode::_expandRoutingChannel()
{
  for ( RHVSlicingNode* node : _slicingRouting ) node->_expandRoutingChannel();
  for ( SlicingNode*    node : _children ) {
    if (node->getType() != RoutingSNode) node->_expandRoutingChannel();
  }
  setRoutingEstimated(RoutingEstimated);
  if (_parent == NULL) {
    for ( SlicingNode* node : _children ) node->adjustBorderChannels();
  }
}


void HVSlicingNode::expandRoutingChannel( DbU::Unit height, DbU::Unit width )
{
  for ( RHVSlicingNode* node : _slicingRouting ) node->expandRoutingChannel( height, width );
  for ( SlicingNode*    node : _children ) {
    if (node->getType() != RoutingSNode) node->expandRoutingChannel( height, width );
  }
  setRoutingEstimated( RoutingEstimated );
  if (_parent == NULL)
    for ( SlicingNode* node : _children ) node->adjustBorderChannels();
}


bool HVSlicingNode::isRoutingEstimated() const
{
//cerr << "bool HVSlicingNode::isRoutingEstimated() const" << endl;
  bool estimated = true;
  if (_slicingRouting.empty()){
    estimated = false;
  } else {
    for (std::vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
      estimated &= (*it)->isRoutingEstimated();
    }
  }

  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    if ( ((*it)->getType() == HorizontalSNode ) || ((*it)->getType() == VerticalSNode) ){
      estimated &= (*it)->isRoutingEstimated();
    }
  }
  return estimated;
}


void HVSlicingNode::updateGCellPosition()
{
  cdebug_log(535,1) << "HVSlicingNode::updateGCellPosition()" << endl;

  for ( SlicingNode* node : _slicingRouting ) node->updateGCellPosition();
  for ( SlicingNode* node : _children       ) node->updateGCellPosition();

  cdebug_tabw(535,-1);
}


void HVSlicingNode::updateGContacts()
{
  cdebug_log(535,1) << "HVSlicingNode::updateGContacts()" << endl;

  for ( SlicingNode* node : _slicingRouting ) node->updateGContacts();
  for ( SlicingNode* node : _children       ) node->updateGContacts();

  cdebug_tabw(535,-1);
}


void HVSlicingNode::clearGCells()
{
  for (std::vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
   (*it)->clearGCells();
  }
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->clearGCells();
  }
  _gcell = NULL;
}


int HVSlicingNode::getNbDevices ()
{
  int nb = 0;
  for ( SlicingNode* node : _children ){
    nb += node->getNbDevices();
  }
  return nb;
}


string  HVSlicingNode::_getString () const
{
  string s = Super::_getString();
  s.insert( s.size()-1, " childs:" + getString(_children.size()) );
  return s;
}


string  HVSlicingNode::_getTypeName () const
{ return "HVSlicingNode"; }


void HVSlicingNode::setSymmetryFlag( unsigned int flag )
{
  SlicingNode::setSymmetryFlag(flag);
  for (std::vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); it++){
    (*it)->setSymmetryFlag(flag);
  }
  for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->setSymmetryFlag(flag);
  }
}


void HVSlicingNode::setMaster ( SlicingNode* master ) 
{
  _master = master;
  int index = 0;
  
  if (  ((getType() == HorizontalSNode) && (isHSymmetry()))
     || ((getType() == VerticalSNode  ) && (isVSymmetry()))
     ){
    for (vector<SlicingNode*>::reverse_iterator it = _children.rbegin(); it != _children.rend(); it++) {   
      (*it)->setMaster(master->getChild(index));
      index++;
    }
  } else {
    for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++) {   
      (*it)->setMaster(master->getChild(index));
      index++;
    }
  }
}


bool HVSlicingNode::isSymmetric( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
{
  if (   ( getType()           == node->getType()           )
     and ( getNbChild()        == node->getNbChild()        ) 
     and ( getToleranceBandH() == node->getToleranceBandH() )
     and ( getToleranceBandW() == node->getToleranceBandW() ) ) {
    bool isSame = true;

    if (  ((getType() == HorizontalSNode) && (symmetryType == HSymmetry))
       || ((getType() == VerticalSNode  ) && (symmetryType == VSymmetry))
       ){
      int i = 0;
      for ( int ichild = (int) getNbChild()-1 ; isSame and (ichild > -1) ; --ichild ) {
        isSame = getChild(i)->isSymmetric( node->getChild(ichild), symmetryType );
        i++;
      }
    } else {
      for ( int ichild = 0 ; isSame and (ichild < (int) getNbChild()) ; ++ichild ) {
        isSame = getChild(ichild)->isSymmetric( node->getChild(ichild), symmetryType );
      }
    }

    if (not isSame and (flags & ShowDiff))
      cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs (childs)." << endl;

    return isSame;
  } else {
    if (flags & ShowDiff)
      cerr << "HVSlicingNode::isSame() " << this << " and " << node << " differs." << endl;
    return false;
  }
}


bool HVSlicingNode::checkSymmetryNet( unsigned int type, Net* net1, Net* net2 ) const
{
//cerr << "bool HVSlicingNode::checkSymmetryNet( unsigned int type, Net* net1, Net* net2 ) const" << endl;
  bool b = false;
  for ( vector<tuple<unsigned int, Net*, Net*> >::const_iterator it = _netsymmetries.begin(); it != _netsymmetries.end(); ++it) {
    if (get<1>((*it)) == net1){
      if (get<2>((*it)) == net2){
        if (get<0>((*it)) == type) { 
          b = true;
          break;
        }
      }
    }
  }
  return b;
}


void HVSlicingNode::addSymmetryNet( unsigned int type, Net* net1, Net* net2 )
{
//cerr << "void HVSlicingNode::addSymmetryNet( unsigned int type, Net* net1, Net* net2 )" << endl;
  if (!checkSymmetryNet(type, net1, net2)){
    unsigned int ftype = type;
    if      (type == 1) ftype = NetRoutingState::Vertical;
    else if (type == 2) ftype = NetRoutingState::Horizontal;
    _netsymmetries.push_back(make_tuple(ftype, net1, net2));
  } else {
    cerr << "Warning(void HVSlicingNode::addSymmetryNet( unsigned int type, Net* net1, Net* net2 )): Net symmetry already set." << endl;
  }
}


void HVSlicingNode::updateNetConstraints()
{
//cerr << "void void HVSlicingNode::updateNetConstraints()" << endl;
  if (_cell != NULL){
    for ( vector<tuple<unsigned int, Net*, Net*> >::const_iterator it = _netsymmetries.begin(); it != _netsymmetries.end(); ++it) {
      if (get<2>((*it))){
        Net* masterNet = get<1>((*it));
        if (not masterNet) cerr << "void HVSlicingNode::updateNetConstraints() Net \"%s\" not found." << endl;

        NetRoutingState* masterState = NetRoutingExtension::get( masterNet );
        if (not masterState) masterState = NetRoutingExtension::create( masterNet );
      //cerr << "type: " << get<0>((*it)) << endl;
      //cerr << "net1: " << get<1>((*it)) << endl;
      //cerr << "net2: " << get<2>((*it)) << endl;

        masterState->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                               | NetRoutingState::Symmetric
                               | NetRoutingState::SymmetricMaster
                               | get<0>((*it)) );
      /*if      (get<0>((*it)) == NetRoutingState::Vertical  ) masterState->setSymAxis( getX() + getWidth ()/2 );
        else if (get<0>((*it)) == NetRoutingState::Horizontal) masterState->setSymAxis( getY() + getHeight()/2 );
        else  cerr << "void HVSlicingNode::updateNetConstraints(): Wrong axis for Net \"%s\"." << endl;*/
        
        Net* slaveNet = get<2>((*it));
        if (not slaveNet) cerr << "void HVSlicingNode::updateNetConstraints() Net \"%s\" not found." << endl;

        NetRoutingState* slaveState = NetRoutingExtension::get( slaveNet );
        if (not slaveState) slaveState = NetRoutingExtension::create( slaveNet );

        slaveState ->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                               | NetRoutingState::Symmetric
                               | get<0>((*it)) );
      /*if      (get<0>((*it)) == NetRoutingState::Vertical  ) slaveState->setSymAxis( getX() + getWidth ()/2 );
        else if (get<0>((*it)) == NetRoutingState::Horizontal) slaveState->setSymAxis( getY() + getHeight()/2 );
        else cerr << "void HVSlicingNode::updateNetConstraints(): Wrong axis for Net \"%s\"." << endl;*/
        slaveState ->setSymNet ( masterNet );
        masterState->setSymNet ( slaveNet  );

      //cerr << "masterState: " << masterState << endl;
      //cerr << "slaveState : " << slaveState  << endl;
      } else {
        Net* net = get<1>((*it));
        if (not net) cerr << "void HVSlicingNode::updateNetConstraints() Net \"%s\" not found." << endl;

        NetRoutingState* state = NetRoutingExtension::get( net );
        if (not state) state = NetRoutingExtension::create( net );

        state->setFlags  ( NetRoutingState::AutomaticGlobalRoute
                         | NetRoutingState::Symmetric
                         | get<0>((*it)) );
      //cerr << "state: " << state << endl;
      /*if      (get<0>((*it)) == NetRoutingState::Vertical  ) state->setSymAxis( getX() + getWidth ()/2 );
        else if (get<0>((*it)) == NetRoutingState::Horizontal) state->setSymAxis( getY() + getHeight()/2 );
        else cerr << "void HVSlicingNode::updateNetConstraints(): Wrong axis for Net \"%s\"." << endl;*/
      }
    }
    for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); ++it){
      if (((*it)->getType() == HorizontalSNode)||((*it)->getType() == VerticalSNode)) (*it)->updateNetConstraints();
    }
  } else {
    cerr << "Error(void HVSlicingNode::updateNetConstraints()): Cell undefined." << endl;
  }
}


void HVSlicingNode::updateSymNetAxis()
{
//cerr << "void HVSlicingNode::updateSymNetAxis()" << endl;
  if (_cell != NULL){
    for ( const tuple<unsigned int, Net*, Net*> triplet : _netsymmetries ) {

      
      SlicingNode* n1 = getChild(_symmetries.front().first);
      SlicingNode* n2 = getChild(_symmetries.front().second);
      DbU::Unit xCenter = (n1->getX() + n2->getX() + n2->getWidth())/2;
      DbU::Unit yCenter = (n1->getY() + n2->getY() + n2->getHeight())/2;
      if (get<2>(triplet)){
        Net* masterNet = get<1>(triplet);
        if (not masterNet) cerr << "void HVSlicingNode::updateSymNetAxis() Net \"%s\" not found." << endl;

        NetRoutingState* masterState = NetRoutingExtension::get( masterNet );
        if (not masterState) cerr << "void HVSlicingNode::updateSymNetAxis(): UpdateNetConstraint need to be used first." << endl;

        if      (get<0>(triplet) == NetRoutingState::Vertical  ) masterState->setSymAxis( xCenter );//getXCenter() );
        else if (get<0>(triplet) == NetRoutingState::Horizontal) masterState->setSymAxis( yCenter );//getYCenter() );
        else cerr << "void HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." << endl;
        
        Net* slaveNet = get<2>(triplet);
        if (not slaveNet) cerr << "void HVSlicingNode::updateSymNetAxis() Net \"%s\" not found." << endl;

        NetRoutingState* slaveState = NetRoutingExtension::get( slaveNet );
        if (not slaveState) cerr << "void HVSlicingNode::updateSymNetAxis(): updateSymNetAxis need to be used first." << endl;

        if      (get<0>(triplet) == NetRoutingState::Vertical  ) slaveState->setSymAxis( xCenter );//getXCenter() );
        else if (get<0>(triplet) == NetRoutingState::Horizontal) slaveState->setSymAxis( yCenter );//getYCenter() );
        else cerr << "void HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." << endl;

      //cerr << "Master net: " << masterNet << ", state: " << masterState << ", axis: " << DbU::getValueString(masterState->getSymAxis())<< endl;
      //cerr << "Slave net: " << slaveNet   << ", state: " << slaveState  << ", axis: " << DbU::getValueString(slaveState->getSymAxis())<< endl;
      } else {

        
      /*
        if        ( getType() == HorizontalSNode ) {
          if ( !checkSelfSymH (get<1>(triplet)) ) {
            cerr << Error( "SlicingNode::HVSlicingNode::updateSymNetAxis(): Net %s is not self symmetric.", getString(get<1>(triplet)).c_str() ) << endl;
          }
        } else if ( getType() == VerticalSNode   ) {
          if ( !checkSelfSymV (get<1>(triplet)) ) {
            cerr << Error( "SlicingNode::HVSlicingNode::updateSymNetAxis(): Net %s is not self symmetric.", getString(get<1>(triplet)).c_str() ) << endl;
          }
        } */

        Net* net = get<1>(triplet);
        if (not net) cerr << "void HVSlicingNode::updateSymNetAxis() Net not found." << endl;

        NetRoutingState* state = NetRoutingExtension::get( net );
        if (not state) cerr << "void HVSlicingNode::updateSymNetAxis(): UpdateNetConstraint need to be used first." << endl;

        if      (get<0>(triplet) == NetRoutingState::Vertical  ){
        //cerr << "case Vertical" << endl ;
          state->setSymAxis( xCenter );//getXCenter() );
        }
        else if (get<0>(triplet) == NetRoutingState::Horizontal) {
        //cerr << "case Horizontal" << endl ;
          state->setSymAxis( yCenter );//getYCenter() );
        }

        


        else cerr << "void HVSlicingNode::updateSymNetAxis(): Unknown NetRoutingState type." << endl;
      //cerr << "Master Only net: " << net << ", state: " << state  << ", axis: " << DbU::getValueString(state->getSymAxis())<< endl;
      }
    }
    for ( SlicingNode* child : _children ) {
      if ( (child->getType() == HorizontalSNode) or (child->getType() == VerticalSNode) )
        child->updateSymNetAxis();
    }
  } else {
    cerr << "Error(void HVSlicingNode::updateSymNetAxis()): Cell undefined." << endl;
  }
}


void HVSlicingNode::flattenDigitalNets ()
{
  if (_cell != NULL){
    for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); ++it){
      if (((*it)->getType() == HorizontalSNode)||((*it)->getType() == VerticalSNode)){
        (*it)->flattenDigitalNets();
      } else if ((*it)->getType() == DeviceSNode){
        if ( ((*it)->getGCell() != NULL) && ((*it)->getInstance() != NULL) ){
          if ( (*it)->getGCell()->isMatrix() ) {
          //cerr << "FLATTEN NETS: " << (*it)->getInstance()  << endl;
            _cell->flattenNets( (*it)->getInstance(), Cell::Flags::BuildRings|Cell::Flags::WarnOnUnplacedInstances );
          }
        }
      }
    }
  } else {
    cerr << "Error(void HVSlicingNode::flattenDigitalNets()): Cell not found." << endl;
  }
}


void HVSlicingNode::updateWireOccupation ( Anabatic::VertexSet sources )
{
//cerr << "void HVSlicingNode::updateWireOccupation ( Anabatic::VertexSet sources )" << endl;
  if (_parent == NULL){
  //Anabatic::Vertex* ivertex = NULL;
    for ( Anabatic::Vertex* ivertex : sources ) {
      Anabatic::GCell*       igcell = ivertex->getGCell();
      SlicingNode* snode = findSlicingNode(igcell);
      if (snode){
        if((snode->getType() == RoutingSNode)&& ivertex->hasAData()){
        //cerr << "ivertex:" << ivertex << ", min:" <<  DbU::getPhysical(ivertex->getIMin(), DbU::Micro) << ", max:" <<  DbU::getPhysical(ivertex->getIMax(), DbU::Micro) << endl;
          snode->addWireOccupation( ivertex->getIMin()
                                  , ivertex->getIMax()
                                  );
            }
      }
    }
  }
//cerr << "" << endl;
}

void HVSlicingNode::resetWireOccupation ()
{
  for (std::vector<RHVSlicingNode*>::const_iterator it = _slicingRouting.begin(); it != _slicingRouting.end(); ++it){
    (*it)->resetWireOccupation();
  }
  
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); ++it){
    if ((*it)->getType() != DeviceSNode) (*it)->resetWireOccupation();
  }
}
// -----------------------------------------------------------------------------------------------//
// Class : HSlicingNode
// -----------------------------------------------------------------------------------------------//


int HSlicingNode::_count    = 0;
int HSlicingNode::_countAll = 0;


HSlicingNode::HSlicingNode( unsigned int type, unsigned int alignment ): HVSlicingNode( type, alignment ){}


HSlicingNode::~HSlicingNode(){}


HSlicingNode* HSlicingNode::create( unsigned int alignment )
{
  _count++;
  _countAll++;
  return new HSlicingNode( HorizontalSNode, alignment );
}


void HSlicingNode::createRouting( DbU::Unit space )
{
  this->push_back(RHSlicingNode::create( space )); 
  resetSlicingTree();
} 


void HSlicingNode::print() const
{
  cerr << "- Print from Slicing Node - " << endl;
  cerr << "SlicingType: Horizontal Node" << endl; 
  if      (this->isAlignLeft  ()){ cerr << "Alignment  : Left"    << endl; }
  else if (this->isAlignCenter()){ cerr << "Alignment  : Middle"  << endl; }
  else if (this->isAlignRight ()){ cerr << "Alignment  : Right"   << endl; }
  else                           { cerr << "Alignment  : Unknown" << endl; }
  cerr << "Tolerances : RatioH: " << DbU::getPhysical(_toleranceRatioH,DbU::Micro) << ", RatioW: " << DbU::getPhysical(_toleranceRatioW,DbU::Micro) << ", BandH: " << DbU::getPhysical(_toleranceBandH,DbU::Micro) << ", BandW: " << DbU::getPhysical(_toleranceBandW,DbU::Micro) << endl; 
  HVSlicingNode::print();
}


HSlicingNode* HSlicingNode::clone( unsigned int tr )
{
  HSlicingNode* node = HSlicingNode::create( this->getAlignment() );
  node->setTolerances( getToleranceRatioH()
                     , getToleranceRatioW()
                     , getToleranceBandH()
                     , getToleranceBandW()
                     );
  node->setBoxSet    ( getBoxSet()           );
  node->setNodeSets  ( _nodeSets->clone()     );
  node->setPreset    ( this->getPreset()     );
  node->setSet       ( this->getSet()        );
  node->setPlaced    ( this->getPlaced()     );
  node->setSymmetries( this->getSymmetries() );

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    if (tr == MY){ node->push_front((*it)->clone(tr)); }
    else         { node->push_back ((*it)->clone(tr)); }
  }
  return node;
}


void HSlicingNode::place( DbU::Unit x, DbU::Unit y )
{
  if( recursiveCheckSet() ){ 
    if (!_slicingRouting.empty()) {
      this->destroySlicingRouting();
      this->resetSlicingRouting();
    }
    this->_place(x,y); 
    if (_slicingRouting.empty()) {
      this->createSlicingRouting();
    } 
    this->updateCellAbutmentBox();
  } else { cerr << "Error(void HSlicingNode::place( DbU::Unit x, DbU::Unit y )): The SlicingTree is not completely set." << endl; }
}


void HSlicingNode::replace( DbU::Unit x, DbU::Unit y )
{
// WARNING: This will change GCell edges.
  if (recursiveCheckSet()) { 
    _place( x, y, true ); 
    updateCellAbutmentBox();
    updateGCellPosition();
    updateGContacts();
  } else {
    cerr << Error( "void HSlicingNode::replace(DbU::Unit,DbU::Unit)): The SlicingTree is not completely set." ) << endl;
  }
}


void HSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
{
  cdebug_log(536,1) << "void HSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )" << endl;
  vector<RHVSlicingNode*>::iterator itspace = _slicingRouting.begin();
  DbU::Unit xref = x;
  DbU::Unit yref = y;
  
  if (isRoutingEstimated()){
    (*itspace)->_place(xref, yref, replace);
    yref += (*itspace)->getHeight();
    itspace++;
  }

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){ 
    if ( ((*it)->isHorizontal()) || ((*it)->isVertical()) ){ 
      if ( (*it)->isAlignLeft() ){
        (*it)->setX(xref);
        (*it)->setY(yref);
      }
      else if ( (*it)->isAlignCenter() ){
        (*it)->setX(xref + (this->getWidth()/2) - ((*it)->getWidth()/2));
        (*it)->setY(yref);
      }
      else if ( (*it)->isAlignRight()  ){
        (*it)->setX(xref + this->getWidth()     - (*it)->getWidth());
        (*it)->setY(yref);
      }
    }

    if      ( (*it)->isAlignLeft()   ) { (*it)->_place(xref                                               , yref, replace); }
    else if ( (*it)->isAlignCenter() ) { (*it)->_place(xref + (this->getWidth()/2) - ((*it)->getWidth()/2), yref, replace); }
    else if ( (*it)->isAlignRight()  ) { (*it)->_place(xref + this->getWidth()     -  (*it)->getWidth()   , yref, replace); }
    else if ( (*it)->isRouting()     ) { (*it)->_place(xref                                               , yref, replace); }
    else    { 
      cerr << "Error(_place( DbU::Unit x, DbU::Unit y )): Unknown Alignment in SlicingTree." << endl   ; 
      (*it)->print();
    }
      
    xref = x;
    yref += (*it)->getHeight();
    if (isRoutingEstimated()){
      (*itspace)->_place(xref, yref, replace);
      yref += (*itspace)->getHeight();
      itspace++;
    }
  }
  setPlaced(Placed);
  cdebug_tabw(536,-1);
}


void HSlicingNode::updateGlobalSize()
{
  cdebug_log(535,1) << "HSlicingNode::updateGlobalsize()" << endl;

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++) { 
    (*it)->updateGlobalSize(); 
  }

  if (this->getMaster() == NULL) {
    if (this->getNbChild() == 1){
      _nodeSets->clear();
      NodeSets* node = _children[0]->getNodeSets();
      for(vector<BoxSet*>::const_iterator it2 = node->begin(); it2 != node->end(); it2++){
        vector<BoxSet*> vect   = vector<BoxSet*>();
        vect.push_back((*it2));
        DbU::Unit height = (*it2)->getHeight();
        DbU::Unit width  = (*it2)->getWidth();
        _nodeSets->push_back(vect, height, width, HorizontalSNode);
      }
    }
    else if ( (this->hasEmptyChildrenNodeSets() != true) && (_nodeSets->empty() == true) ){
      
      HSetState state = HSetState(this);
      while( !state.end() ){ state.next(); }

      _nodeSets = state.getNodeSets();
    }
    if (_nodeSets->empty()){ cerr << "Error(void HSlicingNode::updateGlobalSize()): No solution has been found. Try to set larger tolerances." << endl; }
  } else {
    _nodeSets = _master->getNodeSets();
  }

  cdebug_tabw(535,-1);
}


void HSlicingNode::preDestroy()
{
  _count--;
  HVSlicingNode::preDestroy();
}


void HSlicingNode::destroy()
{
  HSlicingNode::preDestroy();
  delete(this);
}


void HSlicingNode::preRecursiveDestroy()
{
  _count--;
  HVSlicingNode::preRecursiveDestroy();
}


void HSlicingNode::recursiveDestroy()
{
  HSlicingNode::preRecursiveDestroy();
  delete(this);
}


OpenChams::SlicingNode* HSlicingNode::toOpenChams()
{
  OpenChams::SlicingNode* hnode = OpenChams::HSlicingNode::create();
  if      ( isAlignLeft()   ){ hnode->setAlignment( "AlignLeft"       ); }
  else if ( isAlignRight()  ){ hnode->setAlignment( "AlignRight"      ); }
  else if ( isAlignCenter() ){ hnode->setAlignment( "AlignCenter"     ); }
  else if ( isAlignTop()    ){ hnode->setAlignment( "AlignTop"        ); }
  else if ( isAlignBottom() ){ hnode->setAlignment( "AlignBottom"     ); }
  else                       { hnode->setAlignment( "UnknownAlignment"); }

  hnode->setToleranceRatioH(_toleranceRatioH);
  hnode->setToleranceRatioW(_toleranceRatioW);
  hnode->setToleranceBandH(_toleranceBandH);
  hnode->setToleranceBandW(_toleranceBandW);

  if (!_symmetries.empty()){
    for (list<pair <int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++){
      hnode->addSymmetry((*it).first, (*it).second);
    }
  }

  for (vector<SlicingNode*>::iterator it2 = _children.begin(); it2 != _children.end(); it2++){
    hnode->push_back( (*it2)->toOpenChams() );
  }

  return hnode;
}


void HSlicingNode::createSlicingRouting() //  Needs to be change in case of not alternative H/V
{
  if (_boxSet){
    int numberChildren = getNbChild();
    vector<SlicingNode*>::iterator it1 = _children.begin();
    DbU::Unit x     = this->getX();
    DbU::Unit y     = this->getY();

    DbU::Unit heightValue = 0;
    if ( _parent ){
      if        ( _parent->getType() == VerticalSNode   ){
        if        ( (this->getAlignment() == AlignBottom) || (this->getAlignment() == AlignTop) ){
          heightValue = _parent->getHeight() - this->getHeight(); 
        } else if ( this->getAlignment() == AlignCenter ){
          heightValue = (_parent->getHeight() - this->getHeight())/2; 
        }
      }
    }

    DbU::Unit hpitch = _rg->getHorizontalPitch();
    if (heightValue % hpitch)
      cerr << Warning( "HSlicingNode::createSlicingRouting(): On %s, height is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(heightValue).c_str()
                     , DbU::getValueString(hpitch).c_str()
                     ) << endl;

    for (int i = 0; i < numberChildren+1; ++i){
      RHSlicingNode* rnode = NULL;
      if ( i == 0 ){
        if ( (this->getAlignment() == AlignTop) || (this->getAlignment() == AlignCenter) ){
          rnode = RHSlicingNode::create( heightValue );
        } else {
          rnode = RHSlicingNode::create();
        }
      } else if ( i == numberChildren ){
        if ( (this->getAlignment() == AlignBottom) || (this->getAlignment() == AlignCenter) ){
          rnode = RHSlicingNode::create( heightValue );
        } else {
          rnode = RHSlicingNode::create();
        }
      } else {
        rnode = RHSlicingNode::create();
      }
      rnode->setParent(this);
        
      if (i == 0){
        if        ( this->getAlignment() == AlignBottom ){
          rnode->place(x, y);
        } else if ( (this->getAlignment() == AlignCenter) || (this->getAlignment() == AlignTop)) {
          rnode->place(x, y-heightValue);
        }
      } else {
        rnode->place(x, y);
      }

      if (_master){
        rnode->setMaster(_master->getSlicingRouting(i));
      }
      _slicingRouting.push_back(rnode);

      if (i < numberChildren){
        y += (*it1)->getHeight();
        it1++;
      }
    }
    
    if (_master){
      if (isHSymmetry()){
        for (size_t i = 0; i < _slicingRouting.size(); i++){
          getSlicingRouting(i)->setMaster(_master->getSlicingRouting(_slicingRouting.size()-1-i));
        }
      } else {
        for (size_t i = 0; i < _slicingRouting.size(); i++){
          getSlicingRouting(i)->setMaster(_master->getSlicingRouting(i));
        }
      }
    } else if ((_symmetries.empty() == false)&&(isAlignCenter())){
      for (size_t i = 0; i < (_slicingRouting.size()/2); i++){
        getSlicingRouting(_slicingRouting.size()-1-i)->setMaster(getSlicingRouting(i));
      }
    }

    for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){ 
      if ( ((*it)->isHorizontal()) || ((*it)->isVertical()) ){ 
        (*it)->createSlicingRouting();
      }
    }   

    this->setRoutingCreated(RoutingCreated); 
  } else {
    cerr << "Error(void HSlicingNode::createSlicingRouting()): SlicingTree needs to be placed first." << endl; 
  }
}


DbU::Unit HSlicingNode::getHeight () const 
{
  DbU::Unit hpitch = _rg->getHorizontalPitch();
  DbU::Unit height = 0;

  if (isRoutingEstimated()){
    for ( SlicingNode*    node : _children       ) height += node->getHeight();
    for ( RHVSlicingNode* node : _slicingRouting ) height += node->getHeight();
  } else {
    if (_boxSet != NULL) height = _boxSet->getHeight();
  }

  if (height % hpitch)
    cerr << Warning( "HSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(height).c_str()
                   , DbU::getValueString(hpitch).c_str()
                   ) << endl;
  return height;
}


DbU::Unit HSlicingNode::getWidth () const 
{
//cdebug_log(536,0) << "DbU::Unit HSlicingNode::getWidth () const " << endl;

  DbU::Unit vpitch = _rg->getVerticalPitch();
  DbU::Unit width  = 0;

  if (isRoutingEstimated()) {
    SlicingNode* m = NULL;
    for ( SlicingNode* node : _children ) {
      if ( (node->getType() != RoutingSNode) and (node->getWidth() > width) ) {
        width = node->getWidth();
        m = node;
      } 
    }
    if (m->isDevice()) width += vpitch*2;
  } else {
  //cdebug_log(536,0) << "case is routing not estimated" << endl;
    if (_boxSet != NULL) width = _boxSet->getWidth();
  }

  if (width % vpitch)
    cerr << Warning( "HSlicingNode::getWidth(): On %s, width is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(width).c_str()
                   , DbU::getValueString(vpitch).c_str()
                   ) << endl;
  return width;
}


void HSlicingNode::setGCell ( Anabatic::GCell* gcell )
{
  Anabatic::GCell* prevGCell = gcell;
  Anabatic::GCell* nextGCell = NULL;
  vector<SlicingNode*   >::const_iterator itc = _children.begin();
  vector<RHVSlicingNode*>::const_iterator its = _slicingRouting.begin();
  cdebug_log(535,1) << "HSlicingNode::setGCell(Anabatic::GCell*), start Y: "<< (*its)->getY() << ", GCell:" << gcell << endl; 
  int index = 0;
  DbU::Unit y = (*its)->getY();

  while ( not ((itc == _children.end()) and (its+1 == _slicingRouting.end())) ) {
    if (index%2 == 0) {
      y += (*its)->getHeight();
      nextGCell = prevGCell->hcut( y );
      (*its)->setGCell( prevGCell );
      its++;
    } else {
      y += (*itc)->getHeight();
      nextGCell = prevGCell->hcut( y );
      (*itc)->setGCell( prevGCell );
      itc++;
    } 
    prevGCell = nextGCell;
    index++;
  }
  (*its)->setGCell( prevGCell );

  cdebug_tabw(535,-1);
}


void HSlicingNode::adjustBorderChannels ()
{
  if (_parent){
    if (_parent->getHeight() > getHeight()){
      DbU::Unit space = _parent->getHeight() - getHeight();

      if        ( this->getAlignment() == AlignTop    ){
        RHVSlicingNode* ch = (*_slicingRouting.begin());
        ch->setHeight(ch->getHeight()+space);


      } else if ( this->getAlignment() == AlignCenter ){
        RHVSlicingNode* chf = (*_slicingRouting.begin());
        RHVSlicingNode* chl = (*(_slicingRouting.end()-1));
        chf->setHeight(chf->getHeight()+space/2);
        chl->setHeight(chl->getHeight()+space/2);

      } else if ( this->getAlignment() == AlignBottom ){
        RHVSlicingNode* ch = (*(_slicingRouting.end()-1));
        ch->setHeight(ch->getHeight()+space);

      }
    }
  }
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->adjustBorderChannels();
  }
}


string  HSlicingNode::_getString () const
{
  string s = Super::_getString();
  return s;
}


string  HSlicingNode::_getTypeName () const
{ return "HSlicingNode"; }


// -----------------------------------------------------------------------------------------------//
// Class : VSlicingNode
// -----------------------------------------------------------------------------------------------//


int VSlicingNode::_count    = 0;
int VSlicingNode::_countAll = 0;

  
VSlicingNode::VSlicingNode( unsigned int type, unsigned int alignment ): HVSlicingNode( type, alignment ){}


VSlicingNode::~VSlicingNode(){}


VSlicingNode* VSlicingNode::create( unsigned int alignment )
{
  _count++;
  _countAll++;
  return new VSlicingNode( VerticalSNode, alignment ); 
}


void VSlicingNode::createRouting( DbU::Unit space )
{
  this->push_back(RVSlicingNode::create( space )); 
  resetSlicingTree();
} 


void VSlicingNode::print() const
{
  cerr << "- Print from Slicing Node - " << endl;
  cerr << "SlicingType: Vertical Node"  << endl; 
  if      (this->isAlignBottom()){ cerr << "Alignment  : Bottom"  << endl; }
  else if (this->isAlignCenter()){ cerr << "Alignment  : Middle"  << endl; }
  else if (this->isAlignTop   ()){ cerr << "Alignment  : Top"     << endl; }
  else                           { cerr << "Alignment  : Unknown" << endl; }
  cerr << "Tolerances : RatioH: " << DbU::getPhysical(_toleranceRatioH,DbU::Micro) << ", RatioW: " << DbU::getPhysical(_toleranceRatioW,DbU::Micro) << ", BandH: " << DbU::getPhysical(_toleranceBandH,DbU::Micro) << ", BandW: " << DbU::getPhysical(_toleranceBandW,DbU::Micro) << endl; 
  HVSlicingNode::print();
}


VSlicingNode* VSlicingNode::clone( unsigned int tr )
{
  VSlicingNode* node = VSlicingNode::create( this->getAlignment() );
  node->setTolerances( getToleranceRatioH()
                     , getToleranceRatioW()
                     , getToleranceBandH()
                     , getToleranceBandW()
                     );
  node->setBoxSet    ( getBoxSet()           );
  node->setNodeSets  ( _nodeSets->clone()     );
  node->setPreset    ( this->getPreset()     );
  node->setSet       ( this->getSet()        );
  node->setPlaced    ( this->getPlaced()     );
  node->setSymmetries( this->getSymmetries() );

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    if (tr == MX){ node->push_front((*it)->clone(tr)); }
    else         { node->push_back ((*it)->clone(tr)); }
  }
  return node;
}

  
void VSlicingNode::place( DbU::Unit x, DbU::Unit y )
{
  if (recursiveCheckSet()){
    if (!_slicingRouting.empty()) {
      this->destroySlicingRouting();
      this->resetSlicingRouting();
    }
    this->_place(x,y); 
    if (_slicingRouting.empty()) {
      this->createSlicingRouting();
    } 
    this->updateCellAbutmentBox();
  } else { cerr << "Error(void VSlicingNode::place( DbU::Unit x, DbU::Unit y )): The SlicingTree is not completely set." << endl; }
}

  
void VSlicingNode::replace( DbU::Unit x, DbU::Unit y )
{
// WARNING: This will change GCell edges.
  if (recursiveCheckSet()) {
    _place( x, y, true ); 
    updateCellAbutmentBox();
    updateGCellPosition();
    updateGContacts();
  } else {
    cerr << Error( "HVlicingNode::place(DbU::Unit,DbU::Unit)): The SlicingTree is not completely set." ) << endl;
  }
}

  
void VSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
{
  cdebug_log(536,1) << "void VSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )" << endl;
//printLine();
  vector<RHVSlicingNode*>::iterator itspace = _slicingRouting.begin();
  DbU::Unit xref = x;
  DbU::Unit yref = y;
  
  if (isRoutingEstimated()){
    (*itspace)->_place(xref, yref, replace);
    xref += (*itspace)->getWidth();
    itspace++;
  }

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    if ( ( (*it)->isHorizontal()) || ((*it)->isVertical()) ){ 
      if ( (*it)->isAlignBottom() ){
        (*it)->setX(xref);
        (*it)->setY(yref);
      }
      else if ( (*it)->isAlignCenter() ){
        (*it)->setX(xref);
        (*it)->setY(yref + (this->getHeight()/2) - ((*it)->getHeight()/2));
      }
      else if ( (*it)->isAlignTop() ){
        (*it)->setX(xref);
        (*it)->setY(yref + this->getHeight() - (*it)->getHeight());
      }
    }
      
    if      ( (*it)->isAlignBottom() ) { (*it)->_place(xref, yref                                                 , replace); }
    else if ( (*it)->isAlignCenter() ) { (*it)->_place(xref, yref + (this->getHeight()/2) - ((*it)->getHeight()/2), replace); }
    else if ( (*it)->isAlignTop()    ) { (*it)->_place(xref, yref +  this->getHeight()    -  (*it)->getHeight()   , replace); }
    else if ( (*it)->isRouting()     ) { (*it)->_place(xref, yref                                                 , replace); }
    else    { 
      cerr << " Error(void place( DbU::Unit x, DbU::Unit y )): Unknown Alignment in SlicingTree." << endl ; 
      (*it)->print();
    }
    
    xref += (*it)->getWidth();
    yref = y;
    if (isRoutingEstimated()){
      (*itspace)->_place(xref, yref, replace);
      xref += (*itspace)->getWidth();
      itspace++;
    }
  }
  setPlaced(Placed);
  cdebug_tabw(536,-1);
}


void VSlicingNode::updateGlobalSize()
{
  cdebug_log(535,1) << "HSlicingNode::updateGlobalsize()" << endl;

  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++) { 
    (*it)->updateGlobalSize(); 
  }

  if (this->getMaster() == NULL) {
    if (this->getNbChild() == 1){   
      _nodeSets->clear();
      NodeSets* node = _children[0]->getNodeSets();
      for(vector<BoxSet*>::const_iterator it2 = node->begin(); it2 != node->end(); it2++){
        vector<BoxSet*> vect   = vector<BoxSet*>();
        vect.push_back((*it2));
        DbU::Unit height = (*it2)->getHeight();
        DbU::Unit width  = (*it2)->getWidth();
        _nodeSets->push_back(vect, height, width, VerticalSNode);
      }
    }
    else if ( (this->hasEmptyChildrenNodeSets() != true) && (_nodeSets->empty() == true) ){

      VSetState state = VSetState(this);
      while( !state.end() ){ state.next(); }

      _nodeSets = state.getNodeSets();
    }
    if (_nodeSets->empty()) { cerr << "Error(void VSlicingNode::updateGlobalSize()): No solution has been found. Try to set larger tolerances." << endl; }
  } else {
    _nodeSets = _master->getNodeSets();
  }

  cdebug_tabw(535,-1);
}


void VSlicingNode::preDestroy()
{
  _count--;
  HVSlicingNode::preDestroy();
}


void VSlicingNode::destroy()
{
  VSlicingNode::preDestroy();
  delete(this);
}


void VSlicingNode::preRecursiveDestroy()
{
  _count--;
  HVSlicingNode::preRecursiveDestroy();
}


void VSlicingNode::recursiveDestroy()
{
  VSlicingNode::preRecursiveDestroy();
  delete(this);
}


OpenChams::SlicingNode* VSlicingNode::toOpenChams()
{
  OpenChams::SlicingNode* vnode = OpenChams::VSlicingNode::create();
  if      ( isAlignLeft()   ){ vnode->setAlignment( "AlignLeft"       ); }
  else if ( isAlignRight()  ){ vnode->setAlignment( "AlignRight"      ); }
  else if ( isAlignCenter() ){ vnode->setAlignment( "AlignCenter"     ); }
  else if ( isAlignTop()    ){ vnode->setAlignment( "AlignTop"        ); }
  else if ( isAlignBottom() ){ vnode->setAlignment( "AlignBottom"     ); }
  else                       { vnode->setAlignment( "UnknownAlignment"); }

  vnode->setToleranceRatioH(_toleranceRatioH);
  vnode->setToleranceRatioW(_toleranceRatioW);
  vnode->setToleranceBandH(_toleranceBandH);
  vnode->setToleranceBandW(_toleranceBandW);

  if (!_symmetries.empty()){
    for (list<pair <int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++){
      vnode->addSymmetry((*it).first, (*it).second);
    }
  }

  for (vector<SlicingNode*>::iterator it2 = _children.begin(); it2 != _children.end(); it2++){
    vnode->push_back( (*it2)->toOpenChams() );
  }

  return vnode;
}


void VSlicingNode::createSlicingRouting() //  Needs to be change in case of not alternative H/V
{
  if ( isPlaced() ){
    int numberChildren = getNbChild();
    vector<SlicingNode*>::iterator it1 = _children.begin();
    DbU::Unit x     = this->getX();
    DbU::Unit y     = this->getY();

      
    DbU::Unit widthValue = 0;
    if (_parent){
      if        ( _parent->getType() == HorizontalSNode   ){
        if        ( (this->getAlignment() == AlignLeft) || (this->getAlignment() == AlignRight) ){
          widthValue = _parent->getWidth() - this->getWidth(); 
        } else if ( this->getAlignment() == AlignCenter ){
          widthValue = (_parent->getWidth() - this->getWidth())/2; 
        }
      }
    } 

    DbU::Unit vpitch = _rg->getVerticalPitch();
    if (widthValue % vpitch)
      cerr << Warning( "VSlicingNode::createSlicingRouting(): On %s, width is not pitched (%s, pitch:%s)."
                     , getString(this).c_str()
                     , DbU::getValueString(widthValue).c_str()
                     , DbU::getValueString(vpitch).c_str()
                     ) << endl;

    for (int i = 0; i < numberChildren+1; ++i){
      RVSlicingNode* rnode = NULL;
      if ( i == 0 ){
        if ( (this->getAlignment() == AlignRight) || (this->getAlignment() == AlignCenter) ){
          rnode = RVSlicingNode::create( widthValue );
        } else {
          rnode = RVSlicingNode::create();
        }
      } else if ( i == numberChildren ){
        if ( (this->getAlignment() == AlignLeft) || (this->getAlignment() == AlignCenter) ){
          rnode = RVSlicingNode::create( widthValue );
        } else {
          rnode = RVSlicingNode::create();
        }
      } else {
          rnode = RVSlicingNode::create();
      }
      rnode->setParent(this);

      if (i == 0){
        if        ( this->getAlignment() == AlignLeft ){
          rnode->place(x, y);
        } else if ( (this->getAlignment() == AlignCenter) || (this->getAlignment() == AlignRight)) {
          rnode->place(x-widthValue, y);
        }
      } else {
        rnode->place(x, y);
      }
      _slicingRouting.push_back(rnode);

      if (i < numberChildren){
        x += (*it1)->getWidth();
        it1++;
      }
    }
    
    if (_master){
      if (isVSymmetry()){
        for (size_t i = 0; i < _slicingRouting.size(); i++){
          getSlicingRouting(i)->setMaster(_master->getSlicingRouting(_slicingRouting.size()-1-i));
        }
      } else {
        for (size_t i = 0; i < _slicingRouting.size(); i++){
          getSlicingRouting(i)->setMaster(_master->getSlicingRouting(i));
        }
      }
    } else if ((_symmetries.empty() == false)&&(isAlignCenter())){
      for (size_t i = 0; i < (_slicingRouting.size()/2); i++){
        getSlicingRouting(_slicingRouting.size()-1-i)->setMaster(getSlicingRouting(i));
      }
    }

    for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){ 
      if ( ((*it)->isHorizontal()) || ((*it)->isVertical()) ){ 
        (*it)->createSlicingRouting();
      }
    }  
    this->setRoutingCreated(RoutingCreated);
  }
}


DbU::Unit VSlicingNode::getWidth () const 
{
  DbU::Unit vpitch = _rg->getVerticalPitch();
  DbU::Unit width  = 0;

  if (isRoutingEstimated()){
    for ( SlicingNode*    node : _children       ) width += node->getWidth();
    for ( RHVSlicingNode* node : _slicingRouting ) width += node->getWidth();
  } else {
    if (_boxSet != NULL) width = _boxSet->getWidth();
  }

  if (width % vpitch)
    cerr << Warning( "VSlicingNode::getWidth(): On %s, width is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(width).c_str()
                   , DbU::getValueString(vpitch).c_str()
                   ) << endl;
  return width;
}


DbU::Unit VSlicingNode::getHeight () const 
{
  DbU::Unit hpitch = _rg->getHorizontalPitch();
  DbU::Unit height = 0;
  
  if (isRoutingEstimated()) {
    SlicingNode* m = NULL;
    for ( SlicingNode* node : _children ) {
      if (node->getType() != RoutingSNode) {
        if (node->getHeight() > height) {
          height = node->getHeight();
          m = node;
        }
      }
    }
    if (m->isDevice()) height += hpitch*2;
  } else {
    if (_boxSet != NULL) height = _boxSet->getHeight();
  }

  if (height % hpitch)
    cerr << Warning( "VSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(height).c_str()
                   , DbU::getValueString(hpitch).c_str()
                   ) << endl;
  return height;
}


void VSlicingNode::setGCell ( Anabatic::GCell* gcell )
{
  cdebug_log(535,1) << "VSlicingNode::setGCell(Anabatic::GCell*) " << gcell << endl; 

  Anabatic::GCell* prevGCell = gcell;
  Anabatic::GCell* nextGCell = NULL;
  vector<SlicingNode*   >::const_iterator itc = _children.begin();
  vector<RHVSlicingNode*>::const_iterator its = _slicingRouting.begin();
  int index = 0;
  DbU::Unit x = (*its)->getX();

  while ( not ((itc == _children.end()) and (its+1 == _slicingRouting.end())) ) {
    if (index%2 == 0) {
      x += (*its)->getWidth();
      nextGCell = prevGCell->vcut( x );
      (*its)->setGCell( prevGCell );
      its++;
    } else {
      x += (*itc)->getWidth();
      nextGCell = prevGCell->vcut( x );
      (*itc)->setGCell( prevGCell );
      itc++;
    } 
    prevGCell = nextGCell;
    index++;
  } 
  (*its)->setGCell( prevGCell );

  cdebug_tabw(535,-1);
}



void VSlicingNode::adjustBorderChannels ()
{
  if (_parent){
    DbU::Unit space = _parent->getWidth() - getWidth();

    if        ( this->getAlignment() == AlignRight    ){
      RHVSlicingNode* ch = (*_slicingRouting.begin());
      ch->setWidth(ch->getWidth()+space);


    } else if ( this->getAlignment() == AlignCenter ){
      RHVSlicingNode* chf = (*_slicingRouting.begin());
      RHVSlicingNode* chl = (*(_slicingRouting.end()-1));
      chf->setWidth(chf->getWidth()+space/2);
      chl->setWidth(chl->getWidth()+space/2);

    } else if ( this->getAlignment() == AlignLeft ){
      RHVSlicingNode* ch = (*(_slicingRouting.end()-1));
      ch->setWidth(ch->getWidth()+space);

    }
  }
  for (vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
    (*it)->adjustBorderChannels();
  }
}


string  VSlicingNode::_getString () const
{
  string s = Super::_getString();
  return s;
}


string  VSlicingNode::_getTypeName () const
{ return "VSlicingNode"; }

// -----------------------------------------------------------------------------------------------//
// Class : DSlicingNode
// -----------------------------------------------------------------------------------------------//

int DSlicingNode::_count    = 0;
int DSlicingNode::_countAll = 0;


DSlicingNode::DSlicingNode( unsigned int type
                          , NodeSets*    nodeSets
                          , unsigned int alignment
                          , Instance*    instance
                          , BoxSet*      boxSet 
                          ): SlicingNode( type, nodeSets, alignment, boxSet )
                           , _instance(instance)
{}


DSlicingNode::~DSlicingNode(){}
  

DSlicingNode* DSlicingNode::create( NodeSets*    nodeSets
                                  , unsigned int alignment
                                  , Instance*    instance 
                                  , BoxSet*      boxSet 
                                  )
{
  _count++;
  _countAll++;
  return new DSlicingNode( DeviceSNode, nodeSets, alignment, instance, boxSet ); 
}


void DSlicingNode::print() const
{
  cerr << "- Print from Slicing Node - " << endl;
  cerr << "SlicingType: Device Node" << endl; 
  if      (this->isAlignLeft  ()){ cerr << "Alignment  : Left"    << endl; }
  else if (this->isAlignCenter()){ cerr << "Alignment  : Middle"  << endl; }
  else if (this->isAlignRight ()){ cerr << "Alignment  : Right"   << endl; }
  else if (this->isAlignTop   ()){ cerr << "Alignment  : Top"     << endl; }
  else if (this->isAlignBottom()){ cerr << "Alignment  : Bottom"  << endl; }
  else                           { cerr << "Alignment  : Unknown" << endl; }
  cerr <<   "NFingers   : " << getNFing() << endl;
  if (_instance != NULL){
    cerr << "Instance   : " << _instance << endl;
  } else { 
    cerr << "Instance   : None" <<  endl;
  }
  SlicingNode::print();
}


DSlicingNode* DSlicingNode::clone( unsigned int tr )
{
  DSlicingNode* node = DSlicingNode::create( _nodeSets->clone() 
                                           , this->getAlignment()
                                           , this->getInstance()
                                           , this->getBoxSet()
                                           );
  node->setPreset(this->getPreset());
  node->setSet   (this->getSet()   );
  node->setPlaced(this->getPlaced());
  return node;
}


void DSlicingNode::setNFing( int nfing )
{
  if (_nodeSets->find(nfing) != _nodeSets->end()){ 
    _boxSet = (*_nodeSets->find(nfing));
  } else {
    _boxSet = (*_nodeSets->begin());
  }
}


int DSlicingNode::getNFing() const
{
  if (_boxSet != NULL) { return _boxSet->getNFing(); }
  else                 { return 1;                   }
}


double DSlicingNode::getStartParameter() const
{
  return _nodeSets->getStartParameter();
}


double DSlicingNode::getStepParameter() const
{
  return _nodeSets->getStepParameter();
}


double DSlicingNode::getCountParameter() const
{
  return _nodeSets->getCountParameter();
}


void DSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
{
  cdebug_log(536,1) << "void DSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )" << endl;
  if (replace){
    SlicingNode::place(x,y);
    if (_instance != NULL){
      Cell*   model = _instance->getMasterCell();  
      _instance->setTransformation(Transformation( _x - model->getAbutmentBox().getXMin(), _y - model->getAbutmentBox().getYMin()));
      _instance->setPlacementStatus(Instance::PlacementStatus::PLACED); 
    }
  } else {
    if (isSet()){
      SlicingNode::place(x,y);

      if (_instance != NULL){
        Cell*   model = _instance->getMasterCell();   
        Device* dev   = dynamic_cast<Device*>(model);

        if (dev) {     
          TransistorFamily* tf = dynamic_cast<TransistorFamily*>(dev);
          if (tf) {
            tf->setNfing( this->getNFing() ); 
            FormFactorParameter* pff = NULL;
            if ( (pff = dynamic_cast<FormFactorParameter*>(tf->getParameter("M"))) != NULL ) {
              pff->setValue( tf->getNfing() );
            }
            shared_ptr<LayoutGenerator> layoutGenerator ( new LayoutGenerator() );
            layoutGenerator->setDevice(dev);
            layoutGenerator->drawLayout(); 
          }
        }
        _instance->setTransformation(Transformation( _x - model->getAbutmentBox().getXMin(), _y - model->getAbutmentBox().getYMin()));
        _instance->setPlacementStatus(Instance::PlacementStatus::PLACED);
    
      }
    }
  }
  cdebug_tabw(536,-1);
}


void DSlicingNode::place( DbU::Unit x, DbU::Unit y )
{
  DSlicingNode::_place(x, y);
}


void DSlicingNode::preDestroy()
{
  _count--;
  SlicingNode::preDestroy();
}


void DSlicingNode::destroy()
{
  DSlicingNode::preDestroy();
  delete(this);
}


void DSlicingNode::preRecursiveDestroy()
{
  _count--;
  SlicingNode::preRecursiveDestroy();
}


void DSlicingNode::recursiveDestroy()
{
  DSlicingNode::preRecursiveDestroy();
  delete(this);
}


OpenChams::SlicingNode* DSlicingNode::toOpenChams()
{
  OpenChams::SlicingNode* dnode = OpenChams::DSlicingNode::create(_instance->getName()._getString());
  if      ( isAlignLeft()   ){ dnode->setAlignment("AlignLeft"       ); }
  else if ( isAlignRight()  ){ dnode->setAlignment("AlignRight"      ); }
  else if ( isAlignCenter() ){ dnode->setAlignment("AlignCenter"     ); }
  else if ( isAlignTop()    ){ dnode->setAlignment("AlignTop"        ); }
  else if ( isAlignBottom() ){ dnode->setAlignment("AlignBottom"     ); }
  else                       { dnode->setAlignment("UnknownAlignment"); }
  
  string strue = "true";
  string sfalse = "false";
  if( isPreset() ){ dnode->setPreset( strue  ); }
  else            { dnode->setPreset( sfalse ); }
  dnode->setX     ( _x         );
  dnode->setY     ( _y         );
  dnode->setNFing ( getNFing() );
  dnode->setStart ( getStartParameter() );
  dnode->setStep  ( getStepParameter()  );
  dnode->setCount ( getCountParameter() );

  return dnode;
}


bool DSlicingNode::checkInitialPlacement ( int& cpt ) const
{
// -----------------------------------------------------------------------------------------------//
// Notes:: Initial placement criteria is having at least 2 devices placed at (x, y) = (0, 0)
// -----------------------------------------------------------------------------------------------//
  bool initialPlacement = false;
  if ( cpt < 2 ){
    if ( (_x == 0) && (_y == 0) ){
      cpt++;
      if ( cpt < 2 ) { initialPlacement = true; }
    }
    else { initialPlacement = true; } 
  } 
  return initialPlacement;
}


void DSlicingNode::setGCell( Anabatic::GCell* gcell )
{
  cdebug_log(535,0) << "DSlicingNode::setGCell(Anabatic::GCell*) " << gcell << endl;

  if (getParent() != NULL) {
    if (getParent()->getType() == HorizontalSNode) {
      if (getAlignment() == AlignLeft) {
        Anabatic::GCell* rightc = gcell->vcut( getX() + getWidth() );
        _gcell = gcell;
        rightc->setType( Anabatic::Flags::StrutGCell );
      } else if (getAlignment() == AlignCenter) {
        _gcell = gcell->vcut( getX() );
        Anabatic::GCell* rightc = _gcell->vcut( this->getX() + this->getWidth() );
        gcell->setType ( Anabatic::Flags::StrutGCell );
        rightc->setType( Anabatic::Flags::StrutGCell );
      } else if (getAlignment() == AlignRight ){
        _gcell = gcell->vcut( getX() );
        gcell->setType( Anabatic::Flags::StrutGCell );
      }
    } else if (getParent()->getType() == VerticalSNode) {
      if (getAlignment() == AlignBottom){
        Anabatic::GCell* topc = gcell->hcut(this->getY()+this->getHeight());
        _gcell = gcell;
        topc->setType( Anabatic::Flags::StrutGCell );
      } else if (getAlignment() == AlignCenter) {
        _gcell = gcell->hcut( getY() );
        Anabatic::GCell* topc = _gcell->hcut( getY() + getHeight() );
        gcell->setType( Anabatic::Flags::StrutGCell );
        topc->setType ( Anabatic::Flags::StrutGCell );
      } else if (getAlignment() == AlignTop) {
        _gcell = gcell->hcut( this->getY() );
        gcell->setType( Anabatic::Flags::StrutGCell );
      }
    } else {
      _gcell = gcell;
    }
  } else {
    _gcell = gcell;
  }
  if (_gcell) {
    if (dynamic_cast<Device*>(_instance->getMasterCell())){
      _gcell->setType( Anabatic::Flags::DeviceGCell );
    } else {
      _gcell->setType( Anabatic::Flags::MatrixGCell ); 
      if(  (_gcell->getWidth()  > Anabatic::Session::getSliceHeight())
        and(_gcell->getHeight() > Anabatic::Session::getSliceHeight()) ) {
        _gcell->doGrid();
      }
    }
  }

  cdebug_log(535,0) << "> Device:" << _instance << endl;
  cdebug_log(535,0) << "> GCell: " << _gcell << endl;
}


bool DSlicingNode::isSame ( SlicingNode* node, unsigned int flags ) const
{
  bool isSame = _nodeSets->compare( node->getNodeSets(), flags );
  if (not isSame and (flags & ShowDiff))
    cerr << "DSlicingNode::isSame() Check " << this << " vs. " << node << endl;
  return isSame;
}
  

bool DSlicingNode::isSymmetric( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
{
  bool isSame = _nodeSets->compare( node->getNodeSets(), flags );
  if (not isSame and (flags & ShowDiff))
    cerr << "[WARNING] DSlicingNode::isSymmetric(): Different nodesets Check " << this << " vs. " << node << endl;
  if (_instance != NULL){
    Cell*   model1 = _instance->getMasterCell();   
    Cell*   model2 = node->getInstance()->getMasterCell();  

    Device* dev1   = dynamic_cast<Device*>(model1);
    Device* dev2   = dynamic_cast<Device*>(model2);
    if ((dev1 != NULL) && (dev2 !=NULL)){
      CommonSourcePair* csp1 = dynamic_cast<CommonSourcePair*>(model1);
      DifferentialPair* dp1  = dynamic_cast<DifferentialPair*>(model1);
      Transistor*       ts1  = dynamic_cast<Transistor*>(model1);

      CommonSourcePair* csp2 = dynamic_cast<CommonSourcePair*>(model2);
      DifferentialPair* dp2  = dynamic_cast<DifferentialPair*>(model2);
      Transistor*       ts2  = dynamic_cast<Transistor*>(model2);

      if      ((csp1 != NULL) && (csp2 != NULL)) isSame = csp1->isSame(csp2);
      else if ((dp1  != NULL) && (dp2  != NULL)) isSame = dp1->isSame(dp2);
      else if ((ts1  != NULL) && (ts2  != NULL)) isSame = ts1->isSame(ts2);
      else                                       isSame = false;
    } 
    if (not(isSame)) cerr << "[WARNING] DSlicingNode::isSymmetric(): Different devices. Check " << this << " vs. " << node << endl;
  }
  return isSame;
}


void DSlicingNode::updateMatrixGCellPosition()
{
  DbU::Unit tx = getX()-_gcell->getXMin();
  DbU::Unit ty = getY()-_gcell->getYMin();
  Anabatic::GCell* ygcell = _gcell;
  Anabatic::GCell* xgcell = ygcell;
  while (ygcell){
    while(xgcell){
      xgcell->setSouthWestCorner( xgcell->getXMin()+tx, xgcell->getYMin()+ty ); 
      if (xgcell->getEast()->isMatrix()) xgcell = xgcell->getEast();
      else                               xgcell = NULL;
    }
    if (ygcell->getNorth()->isMatrix()) ygcell = ygcell->getNorth();
    else                                ygcell = NULL;
    xgcell = ygcell;
  }
}


void DSlicingNode::updateMatrixGContacts()
{
  Anabatic::GCell* ygcell = _gcell;
  Anabatic::GCell* xgcell = ygcell;
  while (ygcell){
    while(xgcell){
      xgcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
      if (xgcell->getEast()->isMatrix()) xgcell = xgcell->getEast();
      else                               xgcell = NULL;
    }
    if (ygcell->getNorth()->isMatrix()) ygcell = ygcell->getNorth();
    else                                ygcell = NULL;
    xgcell = ygcell;
  }
}


void DSlicingNode::updateGCellPosition()
{
  cdebug_log(535,0) << "DSlicingNode::updateGCellPosition() " << _gcell << endl;

  if (_gcell) {
    if (_parent) {
      if (_parent->getType() == HorizontalSNode) {
        DbU::Unit hpitch = _rg->getHorizontalPitch();

        if (getAlignment() == AlignLeft) {
          _gcell->getEastNMatrix()->setSouthWestCorner( getX()+getWidth(), getY() );
          _gcell->getEastNMatrix()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/hpitch)
                                                        , floor(getHeight()/hpitch)
                                                        );
        } else if (getAlignment() == AlignCenter) {
          _gcell->getWest()->setSouthWestCorner( _parent->getX(), getY() );
          _gcell->getWest()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/(2*hpitch))
                                                 , floor(getHeight()/(2*hpitch))
                                                 );

          _gcell->getEastNMatrix()->setSouthWestCorner( getX()+getWidth(), getY() );
          _gcell->getEastNMatrix()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/(2*hpitch))
                                                        , floor(getHeight()/(2*hpitch))
                                                        );
        } else if (getAlignment() == AlignRight) {
          _gcell->getWest()->setSouthWestCorner( _parent->getX(), getY() );
          _gcell->getWest()->forceEdgesCapacities( floor((_parent->getWidth()-getWidth())/hpitch)
                                                 , floor(getHeight()/hpitch)
                                                 );
          
        }
      } else if (_parent->getType() == VerticalSNode) {
        DbU::Unit vpitch = _rg->getVerticalPitch();

        if (getAlignment() == AlignBottom) {
          _gcell->getNorthNMatrix()->setSouthWestCorner( getX(), getY()+getHeight() );
          _gcell->getNorthNMatrix()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/vpitch)
                                                         , floor(getWidth()/vpitch)
                                                         );
        } else if (getAlignment() == AlignCenter) {
          _gcell->getSouth()->setSouthWestCorner( getX(), _parent->getY()    );
          _gcell->getSouth()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/(2*vpitch))
                                                  , floor(getWidth()/(2*vpitch))
                                                  );

          _gcell->getNorthNMatrix()->setSouthWestCorner( getX(), getY()+getHeight() );
          _gcell->getNorthNMatrix()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/(2*vpitch))
                                                         , floor(getWidth()/(2*vpitch))
                                                         );
        } else if (getAlignment() == AlignTop) {
          _gcell->getSouth()->setSouthWestCorner( getX(), _parent->getY()    );
          _gcell->getSouth()->forceEdgesCapacities( floor((_parent->getHeight()-getHeight())/vpitch)
                                                  , floor(getWidth()/vpitch)
                                                  );
        }
      }
    }
    if (_gcell->isMatrix()) this->updateMatrixGCellPosition();
    else                    _gcell->setSouthWestCorner( getX(), getY() );
    cdebug_log(535,0) << "| Updated:" << _gcell << endl;
  }
}



void DSlicingNode::updateGContacts()
{
  cdebug_log(535,0) << "DSlicingNode::updateGContacts() " << _gcell << endl;

  if (_gcell) {
    if (_parent) {
      if (_parent->getType() == HorizontalSNode) {

        if (getAlignment() == AlignLeft) {
          _gcell->getEastNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          
        } else if (getAlignment() == AlignCenter) {
          _gcell->getWest()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          _gcell->getEastNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );

        } else if (getAlignment() == AlignRight) {
          _gcell->getWest()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          
        }
      } else if (_parent->getType() == VerticalSNode) {

        if (getAlignment() == AlignBottom) {
          _gcell->getNorthNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
        } else if (getAlignment() == AlignCenter) {
          _gcell->getSouth()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
          _gcell->getNorthNMatrix()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );

        } else if (getAlignment() == AlignTop) {
          _gcell->getSouth()->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
        }
      }
    }
    if (_gcell->isMatrix()) this->updateMatrixGContacts();
    else                    _gcell->updateGContacts( Anabatic::Flags::Vertical|Anabatic::Flags::Horizontal );
    cdebug_log(535,0) << "| Updated:" << _gcell << endl;
  }
}


void DSlicingNode::restrictDevices()
{
  if (_gcell){
    Anabatic::Vertex* vertex = _gcell->getObserver< Anabatic::Vertex>( Anabatic::GCell::Observable::Vertex);
    vertex->setRestricted();
    
    if (!vertex->hasValidStamp()){
      if ((_gcell->getWidth() > _gcell->getHeight())||(_gcell->getWidth() == _gcell->getHeight())) {
        vertex->unsetFlags(Anabatic::Vertex::iVertical);
        vertex->setFlags(Anabatic::Vertex::iHorizontal);
      } else {
        vertex->unsetFlags(Anabatic::Vertex::iHorizontal);
        vertex->setFlags(Anabatic::Vertex::iVertical  );
      }
    }
  }
  
}


void DSlicingNode::printLine() const
{
  cerr << "Instance: " << _instance << ", " ;
  SlicingNode::printLine();
}


string  DSlicingNode::_getString () const
{
  string s = Super::_getString();
  s.insert( s.size()-1, " "+getString(_instance->getName()));
  return s;
}


string  DSlicingNode::_getTypeName () const
{ return "DSlicingNode"; }


bool DSlicingNode::isAnalog () const
{
  if (_instance){
    Cell*   model = _instance->getMasterCell();   
    Device* dev   = dynamic_cast<Device*>(model);
    if (dev) return true;     
    else     return false;
  } else return false;
}


bool DSlicingNode::isDigital () const
{
  return (not (isAnalog()));
}


bool DSlicingNode::checkCellInstances ( Cell* cell ){
  if (_instance){
    if (cell->getInstance(_instance->getName())) return true;
    else return false;
  } else return false;
}
// -----------------------------------------------------------------------------------------------//
// Class : RHVSlicingNode
// -----------------------------------------------------------------------------------------------//

int RHVSlicingNode::_count    = 0;
int RHVSlicingNode::_countAll = 0;


RHVSlicingNode::RHVSlicingNode(): SlicingNode( RoutingSNode, NodeSets::create(), UnknownAlignment, NULL )
                                , _railInstance(NULL)
{
  this->setPreset(Preset); 
  this->setSet   (Set   ); 
  this->setOverCell(OverCell);
  _wireOccupation = ChannelRouting::create();
}


RHVSlicingNode::~RHVSlicingNode(){}


void RHVSlicingNode::print() const
{
  cerr << "- Print from Slicing Node - " << endl;
  if (_railInstance) cerr << "RailInstace: " << _railInstance << endl; 
  SlicingNode::print();
}


void RHVSlicingNode::_setGlobalSize ( DbU::Unit height, DbU::Unit width ){}


void RHVSlicingNode::preDestroy()
{
  _count--;
  SlicingNode::preDestroy();
  if(_wireOccupation) _wireOccupation->destroy();
}


void RHVSlicingNode::destroy()
{
  RHVSlicingNode::preDestroy();
  delete(this);
}


void RHVSlicingNode::preRecursiveDestroy()
{
  _count--;
  SlicingNode::preRecursiveDestroy();
}


void RHVSlicingNode::recursiveDestroy()
{
  RHVSlicingNode::preRecursiveDestroy();
  delete(this);
}


void RHVSlicingNode::resetSize()
{
  _boxSet->setHeight( _minHeight );
  _boxSet->setWidth ( _minWidth  );
}


bool RHVSlicingNode::isSame( SlicingNode* node, unsigned int flags ) const
{
  return ( (this->getHeight() == node->getHeight())&& (this->getWidth() == node->getWidth()) );
}


bool RHVSlicingNode::isSymmetric( SlicingNode* node, unsigned int symmetryType, unsigned int flags ) const
{
  return ( (this->getHeight() == node->getHeight())&& (this->getWidth() == node->getWidth()) );
}


void RHVSlicingNode::setRailInstance(Hurricane::Instance* i) 
{
  _railInstance = i;
}


void RHVSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )
{
  SlicingNode::_place(x,y,replace);
  if (_railInstance){
    Net* net = NULL;
    int t = 0;
    for (Net* n : _railInstance->getMasterCell()->getNets()) {
      net = n;
      t++;
    }
    if (t != 1) cerr << "Error(void RHVSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )): Several Nets in Rail Instance Cell." << endl;
    Component* component = NULL;
    t = 0;
    for (Component* c : net->getComponents()){
      component = c;
      t++;
    }
    if (t != 1) cerr << "Error(void RHVSlicingNode::_place( DbU::Unit x, DbU::Unit y, bool replace )): Several Components in Rail Instance Cell." << endl;

    Horizontal* h = dynamic_cast<Horizontal*>(component);
    Vertical*   v = dynamic_cast<Vertical*>  (component);
    UpdateSession::open();
    if (h){
      h->setDxSource(0);
      h->setDxTarget(getRootWidth());
      _railInstance->getMasterCell()->setAbutmentBox(Box( 0, 0, getRootWidth(), getHeight()  ));
      _railInstance->setTransformation(Transformation( _cell->getAbutmentBox().getXMin(), getY() ) );
    } else if (v){
      v->setDySource(0);
      v->setDyTarget(getRootHeight());
      _railInstance->getMasterCell()->setAbutmentBox(Box( 0, 0, getWidth(), getRootHeight() ));
      _railInstance->setTransformation(Transformation( getX(), _cell->getAbutmentBox().getYMin() ) );
    } else {
      cerr << "Error(void  RHVSlicingNode::_place( DbU::Unit x = 0, DbU::Unit y = 0, bool replace = false )): Unknown segment type." << endl;
    }
    _railInstance->setPlacementStatus(Instance::PlacementStatus::PLACED);
    UpdateSession::close();
  }
}


void RHVSlicingNode::addWireOccupation( DbU::Unit min, DbU::Unit max )
{
//cerr << "addWireOccupation min:" << DbU::getPhysical(min, DbU::Micro) << ", max:" << DbU::getPhysical(max, DbU::Micro) << ", MaxCount: " << _wireOccupation->getMaxCount() << endl;
  if (_wireOccupation) {
    if (min == max){/*
      RHSlicingNode* h = dynamic_cast<RHSlicingNode*>(this);
      RVSlicingNode* v = dynamic_cast<RVSlicingNode*>(this);
      if (h) {
        _wireOccupation->insertChannel(getX(), getX()+getWidth());
      } else if (v) {
        _wireOccupation->insertChannel(getY(), getY()+getHeight());
        }*/
    } else {
      _wireOccupation->insertChannel(min, max);
    }
  }
//cerr << "NewMaxCount:" << _wireOccupation->getMaxCount() << endl;
//_wireOccupation->print();
//printLine();
}
  
  
void RHVSlicingNode::resetWireOccupation()
{
  if (_wireOccupation) _wireOccupation->reset();
}


int RHVSlicingNode::getMaxWireOccupation()
{
  if (_wireOccupation) return _wireOccupation->getMaxCount()+1;
  else return 0;
}


// Error Message Methods
unsigned int RHVSlicingNode::getAlignment() const
{
  cerr << " Error(unsigned int getAlignment () const): Routing does not have centering type." << endl;
  return UnknownAlignment;
}


NodeSets* RHVSlicingNode::getNodeSets() const
{
  cerr << " Error(NodeSets* getNodeSets() const): Routing does not have different dimensions." << endl;
  return NULL;
}


BoxSet* RHVSlicingNode::getPairH( DbU::Unit height ) const
{
  cerr << " Error(BoxSet* getPairH ( DbU::Unit height ) const): Routing does not have different dimensions." << endl;
  return _boxSet;
}


BoxSet* RHVSlicingNode::getPairHW( DbU::Unit height, DbU::Unit width ) const
{
  cerr << " Error(BoxSet* RHVSlicingNode::getPairHW( DbU::Unit height, DbU::Unit width ) const): Routing does not have different dimensions." << endl;
  return _boxSet;
}


void RHVSlicingNode::setPairH( DbU::Unit height )
{
  cerr << " Error(void setPairH ( DbU::Unit height )): Routing does not have different dimensions." << endl;
}


void RHVSlicingNode::setPairHW( DbU::Unit height, DbU::Unit width )
{
  cerr << " Error(void setPairHW ( DbU::Unit height, DbU::Unit width )): Routing does not have different dimensions." << endl;
}


// -----------------------------------------------------------------------------------------------//
// Class : RHSlicingNode
// -----------------------------------------------------------------------------------------------//


RHSlicingNode::RHSlicingNode( DbU::Unit height ): RHVSlicingNode()
{
  RHBoxSet* node = RHBoxSet::create(height);
  _nodeSets->push_back(node);
  _boxSet = node;
  _minHeight = height;
  if (_parent != NULL){
    _minWidth = _parent->getWidth();
  } else {
    _minWidth = 0;
  }

  DbU::Unit hpitch = _rg->getHorizontalPitch();
  if (height % hpitch)
    cerr << Warning( "RHSlicingNode::RHSlicingNode(): On %s, height is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(height).c_str()
                   , DbU::getValueString(hpitch).c_str()
                   ) << endl;
}


RHSlicingNode::RHSlicingNode( Hurricane::Instance* i ): RHVSlicingNode()
{
  RHBoxSet* node = RHBoxSet::create(i->getMasterCell()->getAbutmentBox().getHeight());
  _nodeSets->push_back(node);
  _boxSet = node;
  _minHeight = i->getMasterCell()->getAbutmentBox().getHeight();
  setRailInstance(i);
  if (_parent != NULL){
    _minWidth = _parent->getWidth();
  } else {
    _minWidth = 0;
  }
}


RHSlicingNode::~RHSlicingNode(){};


RHSlicingNode* RHSlicingNode::create( DbU::Unit height )
{
  _count++;
  _countAll++;
  
  return new RHSlicingNode(height); 
}


RHSlicingNode* RHSlicingNode::create( Hurricane::Net* net,Hurricane::Layer* layer, int npitch, string cname, string iname )
{
//cerr << "RHSlicingNode::Create (Layer and co)" << endl; 
  Cell* cell   = Hurricane::Cell::create(Hurricane::DataBase::getDB()->getRootLibrary(), cname);
  Net*  subnet = Hurricane::Net::create(cell, net->getName());
  DbU::Unit height = CRL::AllianceFramework::get()->getRoutingGauge()->getHorizontalPitch()* npitch;
  subnet->setExternal(true);
  Hurricane::Horizontal* h = Hurricane::Horizontal::create( subnet, layer, height/2, height );
  NetExternalComponents::setExternal( h );
  Hurricane::Instance* instance = Hurricane::Instance::create(_cell, iname, cell); 
  instance->getPlug(subnet)->setNet(net);
  cell->setAbutmentBox(Box(0, 0, 0, height));
  SlicingNode::addRailSegments(h);
  return new RHSlicingNode(instance); 
}


RHSlicingNode* RHSlicingNode::clone( unsigned int tr )
{
  RHSlicingNode* node = RHSlicingNode::create(this->getHeight());
  return node; 
}

  
DbU::Unit RHSlicingNode::getWidth () const
{
//cerr << "DbU::Unit RHSlicingNode::getWidth () const" << endl;
  if ( (_parent) ){
    return _parent->getWidth();
  } else {
    return 0;
  }
}


void RHSlicingNode::setHeight( DbU::Unit height )
{
  _boxSet->setHeight( height );

  DbU::Unit hpitch = _rg->getHorizontalPitch();
  if (height % hpitch)
    cerr << Warning( "RHSlicingNode::getHeight(): On %s, height is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(height).c_str()
                   , DbU::getValueString(hpitch).c_str()
                   ) << endl;
}


OpenChams::SlicingNode* RHSlicingNode::toOpenChams()
{
  OpenChams::SlicingNode* rnode = OpenChams::RSlicingNode::create(this->getHeight());
  return rnode;
}


void RHSlicingNode:: estimateChannelsSize()
{
  if (getRailInstance() == NULL){
    if (_gcell){
    /*ChannelRouting* c = ChannelRouting::create();
      for ( Contact* contact : _gcell->getGContacts() ) {
        DbU::Unit xmin = contact->getX();
        DbU::Unit xmax = contact->getX();
        NetRoutingState* state = NetRoutingExtension::get( contact->getNet() );
        DbU::Unit wpitch = 1;
        if (state) wpitch = state->getWPitch();
        for ( Component* comp: contact->getSlaveComponents() ){
          Vertical*   v = dynamic_cast<Vertical*>(comp);
          Horizontal* h = dynamic_cast<Horizontal*>(comp);
          if (v){
            if (xmin > v->getX()) xmin = v->getX();
            if (xmax < v->getX()) xmax = v->getX();

            if (xmin < getX())            xmin = getX();
            if (xmax > getX()+getWidth()) xmax = getX()+getWidth();

          } else if (h){
            xmin = getX();
            xmax = getX()+getWidth();
          }
        }
        c->insertChannel(xmin, xmax, wpitch);
        }*/

    /*if (_rg){
        DbU::Unit hpitch = _rg->getHorizontalPitch();

        if ( c->getMaxCount() > 0 ){
          int occupancyH = c->getMaxCount()+1;  //+1 for having enough space
          if (occupancyH%2 != 0) occupancyH++;
          this->updateMasterSize( occupancyH*hpitch );
        } else this->updateMasterSize( 2*hpitch );
        this->setRoutingEstimated(RoutingEstimated);
      } else {
        cerr << "Error(void RHSlicingNode::estimateChannelsSize()): Technology missing." << endl;
        }*/
      if (_rg){
        DbU::Unit hpitch = _rg->getHorizontalPitch();
        if ( getMaxWireOccupation()> 0 ){
          int occupancyH = getMaxWireOccupation ();  //+1 for having enough space
          if (occupancyH%2 != 0) occupancyH++;
          this->updateMasterSize( occupancyH*hpitch );
        } else this->updateMasterSize( 2*hpitch );
        this->setRoutingEstimated(RoutingEstimated);
      } else {
        cerr << "Error(void RHSlicingNode::estimateChannelsSize()): Technology missing." << endl;
      }
    }
  }
}


void RHSlicingNode::_expandRoutingChannel ()
{
  SlicingNode* master = this;
  while ( master->getMaster() != NULL ) master = master->getMaster();

  // cerr << "RHSlicingNode::_expandRoutingChannel() " << this
  //      << " to " << DbU::getValueString( master->getHeight() ) << endl;

  setHeight( master->getHeight() );
  setRoutingEstimated( RoutingEstimated );
}


void RHSlicingNode::expandRoutingChannel ( DbU::Unit height, DbU::Unit width )
{
  // cerr << "RHSlicingNode::expandRoutingChannel(DbU::Unit h, DbU::Unit w) " << this
  //      << " to " << DbU::getValueString( height ) << endl;

  setHeight( height );
  setRoutingEstimated( RoutingEstimated );
}


void RHSlicingNode::updateMasterSize( DbU::Unit size )
{
  SlicingNode* master = this;
  while(master->getMaster() != NULL) master = master->getMaster();
  if (master->getHeight() < size) master->setHeight(size);
}


void RHSlicingNode::setGCell( Anabatic::GCell* gcell )
{
  _gcell = gcell;
  if (_gcell) {
    if (_railInstance) _gcell->setType( Anabatic::Flags::HRailGCell );
    else               _gcell->setType( Anabatic::Flags::HChannelGCell );
  }

  cdebug_log(535,0) << "RHSlicingNode::setGCell() " << _gcell << endl;
}

// Error Methods
void RHSlicingNode::setWidth( DbU::Unit width )
{
  cerr << "Error(void RHSlicingNode::setWidth( DbU::Unit width )): Routing does not have width." << endl; 
}


string  RHSlicingNode::_getTypeName () const
{ return "RHSlicingNode"; }


// -----------------------------------------------------------------------------------------------//
// Class : RVSlicingNode
// -----------------------------------------------------------------------------------------------//


RVSlicingNode::RVSlicingNode( DbU::Unit width ): RHVSlicingNode()
{
  RVBoxSet* node = RVBoxSet::create(width);
  _nodeSets->push_back(node);
  _boxSet = node;
  _minWidth = width;
  if (_parent != NULL){
    _minHeight = _parent->getHeight();
  } else {
    _minHeight = 0;
  }

  DbU::Unit vpitch = _rg->getVerticalPitch();
  if (width % vpitch)
    cerr << Warning( "RVSlicingNode::RVSlicingNode(): On %s, width is not pitched (%s, pitch:%s)."
                   , getString(this).c_str()
                   , DbU::getValueString(width).c_str()
                   , DbU::getValueString(vpitch).c_str()
                   ) << endl;
}


RVSlicingNode::RVSlicingNode( Hurricane::Instance* i ): RHVSlicingNode()
{
  RVBoxSet* node = RVBoxSet::create(i->getMasterCell()->getAbutmentBox().getWidth());
  _nodeSets->push_back(node);
  _boxSet = node;
  _minHeight = i->getMasterCell()->getAbutmentBox().getWidth();
  setRailInstance(i);
  if (_parent != NULL){
    _minWidth = _parent->getWidth();
  } else {
    _minWidth = 0;
  }
}


RVSlicingNode::~RVSlicingNode(){};


RVSlicingNode* RVSlicingNode::create( DbU::Unit width )
{
  _count++;
  _countAll++;
  return new RVSlicingNode(width);
}


RVSlicingNode* RVSlicingNode::create( Hurricane::Net* net, Hurricane::Layer* layer, int npitch, string cname, string iname )
{
//cerr << "RVSlicingNode::Create (Layer and co)" << endl; 
  Cell* cell   = Hurricane::Cell::create(Hurricane::DataBase::getDB()->getRootLibrary(), cname);
  Net*  subnet = Hurricane::Net::create(cell, net->getName());
  
  DbU::Unit width = CRL::AllianceFramework::get()->getRoutingGauge()->getVerticalPitch()* npitch;
  subnet->setExternal(true);
  Hurricane::Vertical* v = Hurricane::Vertical::create( subnet, layer, width/2, width );
  NetExternalComponents::setExternal( v );
  Hurricane::Instance* instance = Hurricane::Instance::create(_cell, iname, cell); 
  instance->getPlug(subnet)->setNet(net);
  cell->setAbutmentBox(Box(0, 0, width, 0));
  SlicingNode::addRailSegments(v);
  return new RVSlicingNode(instance); 
}


RVSlicingNode* RVSlicingNode::clone( unsigned int tr )
{
  RVSlicingNode* node = RVSlicingNode::create(this->getWidth());
  return node; 
}

  
DbU::Unit RVSlicingNode::getHeight () const
{
//cerr << "DbU::Unit RVSlicingNode::getHeight () const" << endl;
  if ( (_parent) ){
    return _parent->getHeight();
  } else {
    return 0;
  }
}


void RVSlicingNode::setWidth( DbU::Unit width )
{
  _boxSet->setWidth( width );
}


OpenChams::SlicingNode* RVSlicingNode::toOpenChams()
{
  OpenChams::SlicingNode* rnode = OpenChams::RSlicingNode::create(this->getWidth());
  return rnode;
}

void RVSlicingNode::estimateChannelsSize()
{
  if (getRailInstance() == NULL){
    if (_gcell){
    /*ChannelRouting* c = ChannelRouting::create();
      for ( Contact* contact : _gcell->getGContacts() ) {
        DbU::Unit ymin = contact->getY();
        DbU::Unit ymax = contact->getY();
        NetRoutingState* state = NetRoutingExtension::get( contact->getNet() );
        DbU::Unit wpitch = 1;
        if (state) wpitch = state->getWPitch();
        for ( Component* comp: contact->getSlaveComponents() ){
          Vertical*   v = dynamic_cast<Vertical*>(comp);
          Horizontal* h = dynamic_cast<Horizontal*>(comp);
          if (h){
            if (ymin > h->getY()) ymin = h->getY();
            if (ymax < h->getY()) ymax = h->getY();

            if (ymin < getY())             ymin = getY();
            if (ymax > getY()+getHeight()) ymax = getY()+getHeight();

          } else if (v){
            ymin = getY();
            ymax = getY()+getHeight();
          }
        }
        c->insertChannel(ymin, ymax, wpitch);
      }

      if (_rg){
        DbU::Unit vpitch = _rg->getVerticalPitch();

        if ( c->getMaxCount() > 0 ){
          int occupancyV = c->getMaxCount()+1;  //+1 for having enough space
          if (occupancyV%2 != 0) occupancyV++;
          this->updateMasterSize(occupancyV*vpitch);
        //this->setWidth(occupancyV*vpitch);
        } else this->updateMasterSize(2*vpitch);
        this->setRoutingEstimated(RoutingEstimated);
      } else {
        cerr << "Error(void RHSlicingNode::estimateChannelsSize ()): Technology missing." << endl;
        }*/
      

      if (_rg){
        DbU::Unit vpitch = _rg->getVerticalPitch();

        if ( getMaxWireOccupation() > 0 ){
          int occupancyV = getMaxWireOccupation();  //+1 for having enough space
          if (occupancyV%2 != 0) occupancyV++;
          this->updateMasterSize(occupancyV*vpitch);
        //this->setWidth(occupancyV*vpitch);
        } else this->updateMasterSize(2*vpitch);
        this->setRoutingEstimated(RoutingEstimated);
      } else {
        cerr << "Error(void RHSlicingNode::estimateChannelsSize ()): Technology missing." << endl;
        }
    }
  }
}


void RVSlicingNode::_expandRoutingChannel ()
{
  SlicingNode* master = this;
  while(master->getMaster() != NULL) master = master->getMaster();
  this->setWidth(master->getWidth());
  this->setRoutingEstimated(RoutingEstimated);
}


void RVSlicingNode::expandRoutingChannel( DbU::Unit height, DbU::Unit width )
{
  this->setWidth(width);
  this->setRoutingEstimated(RoutingEstimated);
}


void RVSlicingNode::setGCell( Anabatic::GCell* gcell )
{
  _gcell = gcell;
  if (_gcell) {
    if (_railInstance) _gcell->setType( Anabatic::Flags::VRailGCell );
    else               _gcell->setType( Anabatic::Flags::VChannelGCell );
  }
  
  cdebug_log(535,0) << "RVSlicingNode::setGCell() " << _gcell << endl;
}


void RVSlicingNode::updateMasterSize( DbU::Unit size )
{
  SlicingNode* master = this;
  while(master->getMaster() != NULL) master = master->getMaster();
  if (master->getWidth() < size) master->setWidth(size);
}


// Error Methods
void RVSlicingNode::setHeight( DbU::Unit height )
{
  cerr << "Error(void RVSlicingNode::setHeight( DbU::Unit height )): Routing does not have height." << endl; 
}


string  RVSlicingNode::_getTypeName () const
{ return "RVSlicingNode"; }


// -----------------------------------------------------------------------------------------------//
// Class : HVSetState
// -----------------------------------------------------------------------------------------------//
  

HVSetState::HVSetState( HVSlicingNode* node ): _HVSnode( node )
{
  _currentSet = vector<size_t> ();
  _nextSet    = vector<size_t> ();
  _nodeSets   = NodeSets::create();
  _counter    = 1;
  initSet();
  initModulos();
}


HVSetState::~HVSetState(){}


NodeSets* HVSetState::getNodeSets()
{
  _nodeSets->sort();
  return _nodeSets; 
}


void HVSetState::print()
{
  int index = 0;
  cerr << "currentSet:" << endl;
  vector<size_t>::iterator it2 = _currentSet.begin();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    cerr << index << ": H = " << node->at((*it2))->getHeight() << ", W = " << node->at((*it2))->getWidth() << endl;
    it2++;
    index++;
  }
  cerr << "counter     = " << _counter  << endl;
  cerr << "end counter = " << _modulos.back()<< endl;
  cerr << "modulos:" << endl;
  index = 0;
  for (vector<int>::iterator it = _modulos.begin(); it != _modulos.end(); it++)
    { 
      cerr << index << ": modulo = " << (*it) << endl;
      index++;
    }
  _nodeSets->print();
}


void HVSetState::initSet()
{
  _nextSet.clear();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){ 
    if ( (*it)->isPreset() ) 
      { _nextSet.push_back((*it)->getNodeSets()->findIndex((*it)->getHeight(), (*it)->getWidth())); }
    else 
      { _nextSet.push_back(0); }
  }
  _currentSet = _nextSet;
}


void HVSetState::initModulos()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Considering a horizontal/vertical node with X children and each children has N possibilities:
// child 0: N0 possibilities,
// child 1: N1 possibilities,
//          .
//          .
//          .
// child X: NX possibilities.
// If a child is preset then, it only has 1 possibility and N = 1. So we have:
// number of possibilities to be studied = N0 * N1 * ... * NX.
// Children are seen like a <vector> and modulos are used to know when a child needs to choose its next possibility.
// It starts like this:
// - child 0 goes through its N0 possibilities
// - when child 0 was at its last possibility, it goes back to its first one and child 1 pick its second possibility.
// - and we keep going . . . 
// -----------------------------------------------------------------------------------------------//

  int modulo = 1;
  int index  = 0;
    
  _modulos.clear();
  _modulos.push_back(1);
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
        
    if ( it != _HVSnode->getChildren().begin() )       { _modulos.push_back(modulo); }
    if ( (!isSymmetry(index)) && (!(*it)->isPreset()) ){ modulo *= (*it)->getNodeSets()->size(); }
    index++;
  }
  _modulos.push_back(modulo);
}


void HVSetState::next()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Set the next combination. See notes above.
// -----------------------------------------------------------------------------------------------//

  int                      index    = 0;
  pair<int,int>            symmetry = pair<int,int>();
  vector<size_t>::iterator itpair   = _nextSet.begin();

  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){

    if ( this->isSymmetry(index, symmetry) )
      { (*itpair) = _nextSet[symmetry.first]; }
    else {
      if ( (((_counter-1)%_modulos[index]) == _modulos[index]-1) && (!(*it)->isPreset()) ) {
        if ( (*itpair)+1 != (*it)->getNodeSets()->size() ) { (*itpair)++  ; }
        else                                              { (*itpair) = 0; }
      }
    }
    index++;
    itpair++;
  }
  _counter += 1;
  _currentSet = _nextSet;
}


// -----------------------------------------------------------------------------------------------//
// Class : HSetState
// -----------------------------------------------------------------------------------------------//
  

HSetState::HSetState( HSlicingNode* node ):HVSetState(node) {}


HSetState::~HSetState(){}


pair<DbU::Unit,DbU::Unit> HSetState::getCurrentWs()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Calculate the min and max width of the current combination
// Routing nodes need to be taken into account to not calculate a wrong width.
// -----------------------------------------------------------------------------------------------//

  DbU::Unit wmin = 0;
  DbU::Unit wmax = 0;
  if (!_currentSet.empty()) { 
    vector< size_t >::const_iterator it2 = _currentSet.begin();
    vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin();
      
    while( (wmin == 0) && (it != _HVSnode->getChildren().end()) ){
      NodeSets* node = (*it)->getNodeSets();
      if ( node->at((*it2))->getWidth() != 0 )
        { wmin = node->at((*it2))->getWidth(); }
      it++;
      it2++;
    }

    it2 = _currentSet.begin();
    for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
      NodeSets* node = (*it)->getNodeSets();
      if ( (node->at((*it2))->getWidth() < wmin)&&(node->at((*it2))->getWidth() != 0) )
        { wmin = node->at((*it2))->getWidth(); }
      if (node->at((*it2))->getWidth() > wmax)
        { wmax = node->at((*it2))->getWidth(); }
      it2++;
    }
  }
  return pair<DbU::Unit,DbU::Unit> (wmin, wmax);  
}


DbU::Unit HSetState::getCurrentH()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Calculate the height of the current combination
// Routing nodes need to be taken into account to not calculate a wrong height.
// -----------------------------------------------------------------------------------------------//

  DbU::Unit currentH = 0;
  vector< size_t >::const_iterator it2 = _currentSet.begin();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    currentH += node->at((*it2))->getHeight(); 
    it2++;
  }
  return currentH;
}


DbU::Unit HSetState::getCurrentW()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Calculate the width of the current combination
// Routing nodes need to be taken into account to not calculate a wrong width.
// -----------------------------------------------------------------------------------------------//

  DbU::Unit currentW = 0;
  vector< size_t >::const_iterator it2 = _currentSet.begin();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    if (node->at((*it2))->getWidth() > currentW)
      { currentW = node->at((*it2))->getWidth(); }
    it2++;
  }
  return currentW;
}
  

void HSetState::print()
{
  HVSetState::print();
  cerr << "currentH = " << getCurrentH() << endl;
  cerr << "currentW = " << getCurrentW() << endl;
  cerr << endl;
}

  
void HSetState::next()
{
  push_back(); 
  HVSetState::next();
}


void HSetState::push_back()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Check if conditions on tolerance are filled.
// If yes, add the current set to the NodeSets
// -----------------------------------------------------------------------------------------------//

  pair<DbU::Unit,DbU::Unit> paireWidths = getCurrentWs();
  DbU::Unit                 width       = paireWidths.second;
  DbU::Unit                 wmin        = paireWidths.first;
  DbU::Unit                 height      = 0;
  if ( width - wmin <= _HVSnode->getToleranceBandW() ){
    vector<BoxSet*> vect = vector<BoxSet*>();
    vector<size_t>::iterator it2 = _currentSet.begin();
    
    for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
      NodeSets* node = (*it)->getNodeSets();
      vect.push_back(node->at((*it2)));
      height += node->at((*it2))->getHeight(); 
      it2++;
    }

    _nodeSets->push_back(vect, height, width, HorizontalSNode); // create the BoxSet of the current accepted set
  }
}


// -----------------------------------------------------------------------------------------------//
// Class : VSetState
// -----------------------------------------------------------------------------------------------//
  

VSetState::VSetState( VSlicingNode* node ):HVSetState(node) {}


VSetState::~VSetState(){}


pair<DbU::Unit,DbU::Unit> VSetState::getCurrentHs ()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Same as HSetState but for Vertical Node. See notes above.
// -----------------------------------------------------------------------------------------------//

  DbU::Unit hmin = 0;
  DbU::Unit hmax = 0;
  if (!_currentSet.empty()) {
    vector<size_t>::const_iterator      it2 = _currentSet.begin();
    vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin();

    while( (hmin == 0) && (it != _HVSnode->getChildren().end()) ){
      NodeSets* node = (*it)->getNodeSets();
      if ( node->at((*it2))->getHeight() != 0 )
        { hmin = node->at((*it2))->getHeight(); }
      it++;
      it2++;
    }

    it2 = _currentSet.begin();
    for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
      NodeSets* node = (*it)->getNodeSets();
      if ( (node->at((*it2))->getHeight() < hmin)&&(node->at((*it2))->getHeight() != 0) )
        { hmin = node->at((*it2))->getHeight(); }

      if (node->at((*it2))->getHeight() > hmax)
        { hmax = node->at((*it2))->getHeight(); }
      it2++;
    }
  }
  return pair<DbU::Unit,DbU::Unit> (hmin, hmax);  
}


DbU::Unit VSetState::getCurrentH()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Same as HSetState but for Vertical Node. See notes above.
// -----------------------------------------------------------------------------------------------//
  DbU::Unit currentH = 0;
  vector< size_t >::const_iterator it2 = _currentSet.begin();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    if (node->at((*it2))->getHeight() > currentH)
      { currentH = node->at((*it2))->getHeight(); }
    it2++;
  }

  return currentH;
}


DbU::Unit VSetState::getCurrentW()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Same as HSetState but for Vertical Node. See notes above.
// -----------------------------------------------------------------------------------------------//
  DbU::Unit currentW = 0;
  vector< size_t >::const_iterator it2 = _currentSet.begin();
  for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
    NodeSets* node = (*it)->getNodeSets();
    currentW += node->at((*it2))->getWidth(); 
    it2++;
  }
  return currentW;
}


void VSetState::print()
{
  HVSetState::print();
  cerr << "currentH = " << getCurrentH() << endl;
  cerr << "currentW = " << getCurrentW() << endl;
  cerr << endl;
}


void VSetState::next()
{
  push_back(); 
  HVSetState::next();
}


void VSetState::push_back()
{
// -----------------------------------------------------------------------------------------------//
// Notes: Same as HSetState but for Vertical Node. See notes above.
// -----------------------------------------------------------------------------------------------//

  pair<DbU::Unit,DbU::Unit> paireHeights = getCurrentHs();
  DbU::Unit                 height       = paireHeights.second;
  DbU::Unit                 hmin         = paireHeights.first;
  DbU::Unit                 width        = 0;

  if (height - hmin <= _HVSnode->getToleranceBandH()){
    vector<BoxSet*> vect = vector<BoxSet*>();
    vector<size_t>::iterator it2 = _currentSet.begin();
    
    for (vector<SlicingNode*>::const_iterator it = _HVSnode->getChildren().begin(); it != _HVSnode->getChildren().end(); it++){
      NodeSets* node = (*it)->getNodeSets();
      vect.push_back(node->at((*it2)));
      width += node->at((*it2))->getWidth(); 
      it2++;
    }
    _nodeSets->push_back(vect, height, width, VerticalSNode);
  }
}


// -----------------------------------------------------------------------------------------------//
// Class : ChannelRouting
// -----------------------------------------------------------------------------------------------//
  
ChannelRouting::ChannelRouting(){}


ChannelRouting::~ChannelRouting(){}


ChannelRouting* ChannelRouting::create()
{
  return new ChannelRouting();
}


void ChannelRouting::destroy()
{
  delete this;
}


void ChannelRouting::reset()
{
  _limits.clear();
  _counts.clear();
}


int ChannelRouting::getMaxCount() const
{
  int count = 0;
  for (vector<int>::const_iterator it = _counts.begin(); it != _counts.end(); it++){
    count = max( (*it), count );
  }
  return count;
}


void pvectF(vector<DbU::Unit> vec)
{
  for (vector<DbU::Unit>::const_iterator it = vec.begin(); it != vec.end(); it++){
    cerr << (*it) << " - ";
  }
  cerr << endl;
}


void pvectI(vector<int> vec)
{
  for (vector<int>::const_iterator it = vec.begin(); it != vec.end(); it++){
    cerr << (*it) << " - ";
  }
  cerr << endl;
}


void ChannelRouting::insertChannel( DbU::Unit xy1, DbU::Unit xy2, unsigned int w )
{
  for (unsigned int i = 0; i < w; ++i){
    insertChannel(xy1, xy2);
  }
}


void ChannelRouting::insertChannel( DbU::Unit xy1, DbU::Unit xy2 )
{
  DbU::Unit min = xy1;
  DbU::Unit max = xy2;
    ;
  if (_limits.empty()){
    _limits.push_back(min);
    _limits.push_back(max);
    _counts.push_back(1);
  } else {
    if        (max < _limits.front()){
      _limits.insert(_limits.begin(), max);
      _limits.insert(_limits.begin(), min);
      _counts.insert(_counts.begin(), 0);
      _counts.insert(_counts.begin(), 1);
    } else if (min > _limits.back ()){
      _limits.push_back(min);
      _limits.push_back(max);
      _counts.push_back(0);
      _counts.push_back(1);
    } else {
      int index = 0;
      vector<DbU::Unit>::iterator itL = _limits.begin();
      vector<int>::iterator       itC = _counts.begin();

      if (min < (*itL)){
        _limits.insert(itL, min);
        _counts.insert(itC, 1  );
      } else {
        while( ((*itL) < min) && (itL != _limits.end()) ){
          itL++;
          index++;
          if (itC != _counts.end())  itC++;
        }
        
        if ((*itL) != min){
          _limits.insert(itL, min);
          _counts.insert(itC, *(itC-1) );
          itL = _limits.begin()+index;
          itC = _counts.begin()+index;
        } 
        itL = _limits.begin()+index;
        itC = _counts.begin()+index;

        
        while( ((*itL) < max) && (itL != _limits.end()) ){
          itL++;
          index++;
          if (itC != _counts.end()) {
            (*itC) += 1;
            itC++;
          } 
        }
        if ( itL != _limits.end() ){
          if ((*(itL)) > max){
            _limits.insert(itL, max);
            _counts.insert(itC, *(itC-1)-1 );
          } 
        } else {
          if ((*(itL-1)) != max){
            _limits.push_back(max);
            _counts.push_back(1);
          }
        }
      }
      
    }
  }
}


void ChannelRouting::print() const
{
  cerr << "limits: ";
  for (vector<DbU::Unit>::const_iterator it = _limits.begin(); it != _limits.end(); it++){
    cerr << DbU::getPhysical((*it), DbU::Micro) << " - ";
  }
  cerr << endl;
  cerr << "count: ";
  for (vector<int>::const_iterator it = _counts.begin(); it != _counts.end(); it++){
    cerr << (*it) << " - ";
  }
  cerr << endl;
}
