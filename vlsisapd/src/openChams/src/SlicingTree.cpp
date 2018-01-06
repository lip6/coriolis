
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                            Eric Lao              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./SlicingTree.cpp"                             |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/SlicingTree.h"

using namespace std;

namespace OpenChams {

// -----------------------------------------------------------------------------------------------//
// Class : SlicingNode
// -----------------------------------------------------------------------------------------------//


  void printList( list<SlicingNode*> liste )
  {
    for ( list<OpenChams::SlicingNode*>::const_iterator it = liste.begin(); it != liste.end(); it++ ){
      (*it)->print(); 
    }
  }

  SlicingNode::SlicingNode( SlicingNode* parent )
  {
    _parent    = parent;
    _alignment = UnknownAlignment;
  }
  

  void SlicingNode::setParent ( SlicingNode* parent )
  {
    _parent = parent;
  }
                     

  SlicingNode* SlicingNode::getParent () const
  {
    return _parent;
  }


  void SlicingNode::setAlignment ( std::string alignment ) 
  {
    if (!_parent){
      if      ( alignment == "AlignRight"  ){ _alignment = AlignRight      ; }
      else if ( alignment == "AlignLeft"   ){ _alignment = AlignLeft       ; }
      else if ( alignment == "AlignCenter" ){ _alignment = AlignCenter     ; }
      else if ( alignment == "AlignTop"    ){ _alignment = AlignTop        ; }
      else                                  { _alignment = UnknownAlignment; }
    } else {
      if (_parent->getType() == HorizontalSNode ){
        if      ( alignment == "AlignRight"  ){ _alignment = AlignRight      ; }
        else if ( alignment == "AlignCenter" ){ _alignment = AlignCenter     ; }
        else                                  { _alignment = AlignLeft       ; }
      } else if ( _parent->getType() == VerticalSNode ){
        if      ( alignment == "AlignCenter" ){ _alignment = AlignCenter     ; }
        else if ( alignment == "AlignTop"    ){ _alignment = AlignTop        ; }
        else                                  { _alignment = AlignBottom     ; }
      } else { _alignment = UnknownAlignment; }
    }
  }


  string SlicingNode::alignmentToString () const
  {
    string salignment = "";
    if      ( _alignment == AlignRight  ){ salignment = "AlignRight"      ; }
    else if ( _alignment == AlignLeft   ){ salignment = "AlignLeft"       ; }
    else if ( _alignment == AlignCenter ){ salignment = "AlignCenter"     ; }
    else if ( _alignment == AlignTop    ){ salignment = "AlignTop"        ; }
    else                                 { salignment = "UnknownAlignment"; }
    return salignment;
  }


  bool SlicingNode::checkInitialPlacement( int& cpt ) const 
  {
    bool initialPlacement = false;
    if ( cpt < 2 ){ initialPlacement = true; }
    return initialPlacement;
  }


  list<SlicingNode*> SlicingNode::getLeaves() const
  {
    return list<SlicingNode*>();
  }


// Error Methods
  std::string SlicingNode::getInstanceName () const
  {
    cerr << "Error(std::string SlicingNode::getInstanceName ()): Instance name is used in Device only." << endl;
    return "";
  }


  unsigned int SlicingNode::getAlignment () const
  {
    cerr << "Error(unsigned int SlicingNode::getAlignment ()): No Alignment in Routing Node." << endl;
    return UnknownAlignment;
  }


  bool SlicingNode::getPreset () const
  {
    cerr << "Error(bool SlicingNode::getPreset ()): Preset is used in Device only." << endl;
    return false;
  }


  double SlicingNode::getX () const
  {
    cerr << "Error(double SlicingNode::getX ()): X is used in Device only." << endl;
    return 0;
  }


  double SlicingNode::getY () const
  {
    cerr << "Error(double SlicingNode::getY ()): Y is used in Device only." << endl;
    return 0;
  }


  int SlicingNode::getNFing () const
  {
    cerr << "Error(int SlicingNode::getNFing ()): Nfing is used in Device only." << endl;
    return 1;
  }


  double SlicingNode::getStart () const
  {
    cerr << "Error(double SlicingNode::getStart ()): Start is used in Device only." << endl;
    return 0;
  }


  double SlicingNode::getStep () const
  {
    cerr << "Error(double SlicingNode::getStep ()): Step is used in Device only." << endl;
    return 0;
  }


  double SlicingNode::getCount () const
  {
    cerr << "Error(double SlicingNode::getCount ()): Step is used in Device only." << endl;
    return 0;
  }


  void SlicingNode::setInstanceName ( std::string instanceName )
  {
    cerr << "Error(void SlicingNode::setInstanceName ( std::string instanceName )): Instance name is used in Device only." << endl;
  }


  void SlicingNode::setPreset ( std::string preset )
  {
    cerr << "Error(void SlicingNode::setPreset ( std::string preset )): Preset is used in Device only." << endl;
  }


  void SlicingNode::setX ( std::string x )
  {
    cerr << "Error(void SlicingNode::setX ( std::string x )): X is used in Device only." << endl;
  }


  void SlicingNode::setY ( std::string y )
  {
    cerr << "Error(void SlicingNode::setY ( std::string y )): Y is used in Device only." << endl;
  }


  void SlicingNode::setNFing ( std::string nfing )
  {
    cerr << "Error(void SlicingNode::setNFing ( std::string nfing )): NFing is used in Device only." << endl;
  }


  void SlicingNode::setStart ( std::string start )
  {
    cerr << "Error(void SlicingNode::setStart ( std::string start )): Start is used in Device only." << endl;
  }


  void SlicingNode::setStep ( std::string step )
  {
    cerr << "Error(void SlicingNode::setStep ( std::string step )): Step is used in Device only." << endl;
  }


  void SlicingNode::setCount ( std::string count )
  {
    cerr << "Error(void SlicingNode::setCount ( std::string count )): Count is used in Device only." << endl;
  }


  void SlicingNode::setPreset ( bool preset )
  {
    cerr << "Error(void SlicingNode::setPreset ( bool preset )): Preset is used in Device only." << endl;
  }


  void SlicingNode::setX ( double x )
  {
    cerr << "Error(void SlicingNode::setX ( double x )): X is used in Device only." << endl;
  }


  void SlicingNode::setY ( double y )
  {
    cerr << "Error(void SlicingNode::setY ( double y )): Y is used in Device only." << endl;
  }


  void SlicingNode::setNFing ( int nfing )
  {
    cerr << "Error(void SlicingNode::setNFing ( int nfing )): NFing is used in Device only." << endl;
  }


  void SlicingNode::setStart ( double start )
  {
    cerr << "Error(void SlicingNode::setStart ( double start )): Start is used in Device only." << endl;
  }


  void SlicingNode::setStep ( double step )
  {
    cerr << "Error(void SlicingNode::setStep ( double step )): Step is used in Device only." << endl;
  }


  void SlicingNode::setCount ( double count )
  {
    cerr << "Error(void SlicingNode::setCount ( double count )): Count is used in Device only." << endl;
  }


  double SlicingNode::getToleranceRatioH () const
  {
    cerr << "Error(double SlicingNode::getToleranceRatioH ()): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return 0;
  }
      

  const vector<SlicingNode*> SlicingNode::getChildren () const
  {
    cerr << "Error(const vector<SlicingNode*> SlicingNode::getChildren () const): Children are used in Horizontal or Vertical Node only." << endl;
    return vector<SlicingNode*>();
  }

  
  const list<pair <int,int> > SlicingNode::getSymmetries () const
  {
    cerr << "Error(list<pair <int,int> > SlicingNode::getSymmetries () const): Symmetries are used in Horizontal or Vertical Node only." << endl;
    return list<pair<int,int > >();
  }


  double SlicingNode::getToleranceRatioW () const
  {
    cerr << "Error(double SlicingNode::getToleranceRatioW ()): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return 0;
  }


  double SlicingNode::getToleranceBandH () const
  {
    cerr << "Error(double SlicingNode::getToleranceBandH ()): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return 0;
  }


  double SlicingNode::getToleranceBandW () const
  {
    cerr << "Error(double SlicingNode::getToleranceBandW ()): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return 0;
  }


  void SlicingNode::setToleranceRatioH ( std::string value )
  {
    cerr << "Error(void SlicingNode::setToleranceRatioH ( std::string value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceRatioW ( std::string value )
  {
    cerr << "Error(void SlicingNode::setToleranceRatioW ( std::string value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceBandH ( std::string value )
  {
    cerr << "Error(void SlicingNode::setToleranceBandH ( std::string value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceBandW ( std::string value )
  {
    cerr << "Error(void SlicingNode::setToleranceBandW ( std::string value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
    
  }


  void SlicingNode::setToleranceRatioH ( double value )
  {
    cerr << "Error(void SlicingNode::setToleranceRatioH ( double value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceRatioW ( double value )
  {
    cerr << "Error(void SlicingNode::setToleranceRatioW ( double value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceBandH ( double value )
  {
    cerr << "Error(void SlicingNode::setToleranceBandH ( double value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::setToleranceBandW ( double value )
  {
    cerr << "Error(void SlicingNode::setToleranceBandW ( double value )): Tolerances are used in Horizontal or Vertical Node only." << endl;
    
  }


  void SlicingNode::push_back ( SlicingNode* node )
  {
    cerr << "Error(void SlicingNode::push_back ( SlicingNode* node )): Children are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::addSymmetry ( std::string source, std::string target )
  {
    cerr << "Error(void SlicingNode::addSymmetry ( std::string source, std::string target )): Symmetries are used in Horizontal or Vertical Node only." << endl;
  }


  void SlicingNode::addSymmetry ( int source, int target )
  {
    cerr << "Error(void SlicingNode::addSymmetry ( int source, int target )): Symmetries are used in Horizontal or Vertical Node only." << endl;
  }


  double SlicingNode::getValue () const
  {
    cerr << "Error(double SlicingNode::getValue ()): Value is used in Routing Node only." << endl;
    return 0;
  }


  void SlicingNode::setValue ( std::string value )
  {
    cerr << "Error(void SlicingNode::setValue ( std::string value )): Value is used in Routing Node only." << endl;
  }


  string SlicingNode::presetToString () const
  {
    cerr << "Error(string SlicingNode::presetToString () const): Preset is used in Device only." << endl;
    return "";
  }


  string SlicingNode::xToString () const
  {
    cerr << "Error(string SlicingNode::xToString () const): X is used in Device only." << endl;
    return "";
  }


  string SlicingNode::yToString () const
  {
    cerr << "Error(string SlicingNode::yToString () const): Y is used in Device only." << endl;
    return "";
  }


  string SlicingNode::nfingToString () const
  {
    cerr << "Error(string SlicingNode::nfingToString () const): NFing is used in Device only." << endl;
    return "";
  }


  string SlicingNode::startToString () const
  {
    cerr << "Error(string SlicingNode::startToString () const): Start is used in Device only." << endl;
    return "";
  }


  string SlicingNode::stepToString () const
  {
    cerr << "Error(string SlicingNode::stepToString () const): Step is used in Device only." << endl;
    return "";
  }


  string SlicingNode::countToString () const
  {
    cerr << "Error(string SlicingNode::countToString () const): Count is used in Device only." << endl;
    return "";
  }


  string SlicingNode::valueToString () const
  {
    cerr << "Error(string SlicingNode::valueToString () const): Value is used in Routing Node only." << endl;
    return "";
  }


  string SlicingNode::toleranceRatioHToString () const
  {
    cerr << "Error(string SlicingNode::toleranceRatioHToString () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return "";
  }


  string SlicingNode::toleranceRatioWToString () const
  {
    cerr << "Error(double SlicingNode::getToleranceRatioW ()): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return "";
  }


  string SlicingNode::toleranceBandHToString () const
  {
    cerr << "Error(string SlicingNode::toleranceBandHToString () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return "";
  }


  string SlicingNode::toleranceBandWToString () const
  {
    cerr << "Error(string SlicingNode::toleranceBandWToString () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return "";
  }

  bool SlicingNode::hasSameParentToleranceRatioH () const
  {
    cerr << "Error(bool SlicingNode::hasSameParentToleranceRatioH () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return false;
  }


  bool SlicingNode::hasSameParentToleranceRatioW () const
  {
    cerr << "Error(bool SlicingNode::hasSameParentToleranceRatioW () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return false;
  }


  bool SlicingNode::hasSameParentToleranceBandH () const
  {
    cerr << "Error(bool SlicingNode::hasSameParentToleranceBandH () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return false;
  }


  bool SlicingNode::hasSameParentToleranceBandW () const
  {
    cerr << "Error(bool SlicingNode::hasSameParentToleranceBandW () const): Tolerances are used in Horizontal or Vertical Node only." << endl;
    return false;
  }


// -----------------------------------------------------------------------------------------------//
// Class : DSlicingNode
// -----------------------------------------------------------------------------------------------//


  DSlicingNode::DSlicingNode( std::string instanceName, SlicingNode* parent ): SlicingNode(parent)
  {
    _instanceName = instanceName;
    _alignment    = UnknownAlignment;
    _preset       = false;
    _x            = 0;
    _y            = 0;
    _nfing        = 0;
    _start        = 0;
    _step         = 0;
    _count        = 0;
  }

  void DSlicingNode::setInstanceName( std::string instanceName )
  {
    _instanceName = instanceName; 
  }

  
  void DSlicingNode::setPreset( std::string preset )
  {
    if ( preset == "true" ){ _preset = true ; }
    else                   { _preset = false; }
  }


  void DSlicingNode::setX( std::string x )
  {
    std::string::size_type sz;
    _x = stof(x, &sz); 
  }


  void DSlicingNode::setY( std::string y )
  { 
    std::string::size_type sz;
    _y = stof(y, &sz);
  }


  void DSlicingNode::setNFing( std::string nfing )
  {
    std::string::size_type sz;
    _nfing = stoi(nfing, &sz); 
  }


  void DSlicingNode::setStart( std::string start )
  {
    std::string::size_type sz;
    _start = stod(start, &sz);
  }


  void DSlicingNode::setStep( std::string step )
  {
    std::string::size_type sz;
    _step = stod(step, &sz);
  }


  void DSlicingNode::setCount( std::string count )
  {
    std::string::size_type sz;
    _count = stod(count, &sz);
  }


  void DSlicingNode::print() const
  {
    cerr << "instance : " << _instanceName << endl;

    if (_parent != NULL){
      unsigned int type = _parent->getType();
      if      ( type == HorizontalSNode ){ cerr << "Parent   : Type: HorizontalSNode" ; }
      else if ( type == VerticalSNode   ){ cerr << "Parent   : Type: VerticalSNode"   ; }
      else                               { cerr << "Parent   : Type: UnknownType"; }
      cerr << ", trh: " << _parent->getToleranceRatioH();
      cerr << ", trw: " << _parent->getToleranceRatioW();
      cerr << ", tbh: " << _parent->getToleranceBandH();
      cerr << ", tbw: " << _parent->getToleranceBandW();
      cerr <<  endl;
    } else {
      cerr << "Parent   : None " << endl;
    }

    if      ( _alignment == AlignLeft   ){ cerr << "alignment: AlignLeft"   << endl; }
    else if ( _alignment == AlignRight  ){ cerr << "alignment: AlignRight"  << endl; }
    else if ( _alignment == AlignCenter ){ cerr << "alignment: AlignCenter" << endl; }
    else if ( _alignment == AlignTop    ){ cerr << "alignment: AlignTop"    << endl; }
    else if ( _alignment == AlignBottom ){ cerr << "alignment: AlignBottom" << endl; }
    else                                 { cerr << "alignment: Unknown"     << endl; }
    cerr << "preset   : " << _preset << endl;
    cerr << "x        : " << _x      << endl;
    cerr << "y        : " << _y      << endl;
    cerr << "nfing    : " << _nfing  << endl;
    cerr << "start    : " << _start  << endl;
    cerr << "step     : " << _step   << endl;
    cerr << "count    : " << _count  << endl;
    cerr << "type     : Device"      << endl;
    cerr << endl;
  }
  
  
  void DSlicingNode::toXML ( ofstream& file, unsigned indent )
  {
    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "<device instance=\"" << _instanceName << "\">" << endl;

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameter type=\"alignment\" alignment=\"" << alignmentToString() << "\"/>" << endl;

    if (_preset){
      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "<parameter type=\"preset\" preset=\"" << presetToString() << "\"/>" << endl;
    }

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameter type=\"nfing\" nfing=\"" << nfingToString() << "\"/>" << endl;


    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameter type=\"x\" x=\"" << xToString() << "\"/>" << endl;
    

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameter type=\"y\" y=\"" << yToString() << "\"/>" << endl;
    

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameter type=\"nodeSets\" start=\"" << startToString() << "\" step=\"" << stepToString() << "\" count=\"" << countToString() << "\"/>" << endl;

    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "</device>" << endl;
  }


  bool DSlicingNode::checkInitialPlacement ( int& cpt ) const
  {
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


// -----------------------------------------------------------------------------------------------//
// Class : HVSlicingNode
// -----------------------------------------------------------------------------------------------//


  HVSlicingNode::HVSlicingNode( SlicingNode* parent ): SlicingNode( parent )
  {
    _alignment       = UnknownAlignment;
    _toleranceRatioH = 0;
    _toleranceRatioW = 0;
    _toleranceBandH  = 0;
    _toleranceBandW  = 0;
    _symmetries      = list<pair <int,int> > ();
    _children        = vector<SlicingNode*>  (); 
  }


  void HVSlicingNode::setToleranceRatioH( std::string value )
  {
    std::string::size_type sz;
    _toleranceRatioH = stof(value, &sz);
  }


  void HVSlicingNode::setToleranceRatioW( std::string value )
  {
    std::string::size_type sz;
    _toleranceRatioW = stof(value, &sz);
  }


  void HVSlicingNode::setToleranceBandH( std::string value )
  {
    std::string::size_type sz;
    _toleranceBandH = stof(value, &sz);
  }


  void HVSlicingNode::setToleranceBandW( std::string value )
  {
    std::string::size_type sz;
    _toleranceBandW = stof(value, &sz);
  }


  void HVSlicingNode::setToleranceRatioH( double value )
  {
    _toleranceRatioH = value;
  }


  void HVSlicingNode::setToleranceRatioW( double value )
  {
    _toleranceRatioW = value;
  }


  void HVSlicingNode::setToleranceBandH( double value )
  {
    _toleranceBandH = value;
  }


  void HVSlicingNode::setToleranceBandW( double value )
  {
    _toleranceBandW = value;
  }


  void HVSlicingNode::push_back( SlicingNode* node )
  {
    if (node != NULL){ 
      node->setParent(this);
      _children.push_back(node); 
    }
  }


  void HVSlicingNode::addSymmetry( string source, string target )
  {
    std::string::size_type sz;
    int isource = stoi(source, &sz);
    int itarget = stoi(target, &sz);
    _symmetries.push_back(pair<int,int>(isource, itarget));
  }


  void HVSlicingNode::addSymmetry( int source, int target )
  {
    _symmetries.push_back(pair<int,int>(source, target));
  }


  void HVSlicingNode::recursiveDestroy()
  {
    for (std::vector<SlicingNode*>::iterator it = _children.begin(); it != _children.end(); it++){
      (*it)->recursiveDestroy();
    }
    delete(this);
  }


  void HVSlicingNode::print() const
  {
    if      ( _alignment == AlignLeft   ){ cerr << "alignment: AlignLeft"   << endl; }
    else if ( _alignment == AlignRight  ){ cerr << "alignment: AlignRight"  << endl; }
    else if ( _alignment == AlignCenter ){ cerr << "alignment: AlignCenter" << endl; }
    else if ( _alignment == AlignTop    ){ cerr << "alignment: AlignTop"    << endl; }
    else if ( _alignment == AlignBottom ){ cerr << "alignment: AlignBottom" << endl; }
    else                                 { cerr << "alignment: Unknown"     << endl; }
    cerr << "tRatioH  : " << _toleranceRatioH << endl;
    cerr << "tRatioW  : " << _toleranceRatioW << endl;
    cerr << "tBandH   : " << _toleranceBandH << endl;
    cerr << "tBandW   : " << _toleranceBandW << endl;
  }


  bool HVSlicingNode::hasSameParentToleranceRatioH () const
  {
    bool same = false;
    if (_parent){
      same = (_toleranceRatioH == _parent->getToleranceRatioH());
    }
    return same;
  }


  bool HVSlicingNode::hasSameParentToleranceRatioW () const
  {
    bool same = false;
    if (_parent){
      same = (_toleranceRatioW == _parent->getToleranceRatioW());
    }
    return same;
  }


  bool HVSlicingNode::hasSameParentToleranceBandH () const
  {
    bool same = false;
    if (_parent){
      same = (_toleranceBandH == _parent->getToleranceBandH());
    }
    return same;
  }


  bool HVSlicingNode::hasSameParentToleranceBandW () const
  {
    bool same = false;
    if (_parent){
      same = (_toleranceBandW == _parent->getToleranceBandW());
    }
    return same;
  }


  bool HVSlicingNode::checkInitialPlacement( int& cpt ) const
  {
    bool initialPlacement = false;
    if (cpt < 2){
      initialPlacement = true;
      for (vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++){
        if (cpt < 2){ initialPlacement = (*it)->checkInitialPlacement(cpt); } 
      }
    }
    return initialPlacement;
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


// -----------------------------------------------------------------------------------------------//
// Class : HSlicingNode
// -----------------------------------------------------------------------------------------------//


  HSlicingNode::HSlicingNode( SlicingNode* parent ): HVSlicingNode(parent)
  {
    _alignment = AlignLeft;
  }

  
  void HSlicingNode::print() const
  {
    cerr << "type     : Horizontal " << endl;

    if (_parent != NULL){
      unsigned int type = _parent->getType();
      if      ( type == HorizontalSNode ){ cerr << "Parent   : Type: HorizontalSNode" ; }
      else if ( type == VerticalSNode   ){ cerr << "Parent   : Type: VerticalSNode"   ; }
      else                               { cerr << "Parent   : Type: UnknownType"; }
      cerr << ", trh: " << _parent->getToleranceRatioH();
      cerr << ", trw: " << _parent->getToleranceRatioW();
      cerr << ", tbh: " << _parent->getToleranceBandH();
      cerr << ", tbw: " << _parent->getToleranceBandW();
      cerr <<  endl;
    } else {
      cerr << "Parent   : None " << endl;
    }
    HVSlicingNode::print();

    for (list<pair<int,int> >::const_iterator it1 = _symmetries.begin(); it1 != _symmetries.end(); it1++){
      cerr << "Symmetrie: (" << (*it1).first << " - " << (*it1).second << ")" << endl;
    }
    cerr << endl;

    for(vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++ ){
      (*it)->print();
    }
    cerr << endl;
  }
  
  
  void HSlicingNode::toXML ( ofstream& file, unsigned indent )
  {
    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "<horizontal>" << endl;

  // Parameters
    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameters>" << endl;

    for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
    file << "<parameter type=\"alignment\" alignment=\"" << alignmentToString() << "\"/>" << endl;

    if (!hasSameParentToleranceRatioH ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceRatioH\" toleranceRatioH=\"" << toleranceRatioHToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceRatioW ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceRatioW\" toleranceRatioW=\"" << toleranceRatioWToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceBandH ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceBandH\" toleranceBandH=\"" << toleranceBandHToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceBandW ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceBandW\" toleranceBandW=\"" << toleranceBandWToString() << "\"/>" << endl;
    }

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "</parameters>" << endl;

  // Symmetries
    if (!_symmetries.empty()){
      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "<symmetries>" << endl;

      for ( std::list<std::pair<int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++ ){
        for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
        file << "<parameter symmetrie source=\"" << std::to_string((*it).first) << "\" target=\"" << std::to_string((*it).second) << "\"/>" << endl;
      }

      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "</symmetries>" << endl;
    }

  // Children
    if (!_children.empty()){
      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "<children>" << endl;

      for (std::vector<SlicingNode*>::const_iterator it2 = _children.begin(); it2 != _children.end(); it2++){
        (*it2)->toXML(file, indent+2);
      }

      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "</children>" << endl;
    }

    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "</horizontal>" << endl;
  }


// -----------------------------------------------------------------------------------------------//
// Class : VSlicingNode
// -----------------------------------------------------------------------------------------------//


  VSlicingNode::VSlicingNode( SlicingNode* parent ): HVSlicingNode(parent)
  {
    _alignment = AlignBottom;
  }

  
  void VSlicingNode::print() const
  {
    cerr << "type     : Vertical " << endl;

    if (_parent != NULL){
      unsigned int type = _parent->getType();
      if      ( type == HorizontalSNode ){ cerr << "Parent   : Type: HorizontalSNode" ; }
      else if ( type == VerticalSNode   ){ cerr << "Parent   : Type: VerticalSNode"   ; }
      else                               { cerr << "Parent   : Type: UnknownType"; }
      cerr << ", trh: " << _parent->getToleranceRatioH();
      cerr << ", trw: " << _parent->getToleranceRatioW();
      cerr << ", tbh: " << _parent->getToleranceBandH();
      cerr << ", tbw: " << _parent->getToleranceBandW();
      cerr <<  endl;
    } else {
      cerr << "Parent   : None " << endl;
    }
    HVSlicingNode::print();

    for (list<pair<int,int> >::const_iterator it1 = _symmetries.begin(); it1 != _symmetries.end(); it1++){
      cerr << "Symmetrie: (" << (*it1).first << " - " << (*it1).second << ")" << endl;
    }
    cerr << endl;

    for(vector<SlicingNode*>::const_iterator it = _children.begin(); it != _children.end(); it++ ){
      (*it)->print();
    }
    cerr << endl;
  }
  
  
  void VSlicingNode::toXML ( ofstream& file, unsigned indent )
  {
    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "<vertical>" << endl;

  // Parameters
    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "<parameters>" << endl;

    for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
    file << "<parameter type=\"alignment\" alignment=\"" << alignmentToString() << "\"/>" << endl;

    if (!hasSameParentToleranceRatioH ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceRatioH\" toleranceRatioH=\"" << toleranceRatioHToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceRatioW ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceRatioW\" toleranceRatioW=\"" << toleranceRatioWToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceBandH ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceBandH\" toleranceBandH=\"" << toleranceBandHToString() << "\"/>" << endl;
    }

    if (!hasSameParentToleranceBandW ()){
      for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
      file << "<parameter type=\"toleranceBandW\" toleranceBandW=\"" << toleranceBandWToString() << "\"/>" << endl;
    }

    for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
    file << "</parameters>" << endl;

  // Symmetries
    if (!_symmetries.empty()){
      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "<symmetries>" << endl;

      for ( std::list<std::pair<int,int> >::const_iterator it = _symmetries.begin(); it != _symmetries.end(); it++ ){
        for (unsigned i = 0 ; i < indent+2 ; i++){ file << "  "; }
        file << "<parameter symmetrie source=\"" << std::to_string((*it).first) << "\" target=\"" << std::to_string((*it).second) << "\"/>" << endl;
      }

      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "</symmetries>" << endl;
    }

  // Children
    if (!_children.empty()){
      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "<children>" << endl;

      for (std::vector<SlicingNode*>::const_iterator it2 = _children.begin(); it2 != _children.end(); it2++){
        (*it2)->toXML(file, indent+2);
      }

      for (unsigned i = 0 ; i < indent+1 ; i++){ file << "  "; }
      file << "</children>" << endl;
    }

    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "</vertical>" << endl;
  }


// -----------------------------------------------------------------------------------------------//
// Class : RSlicingNode
// -----------------------------------------------------------------------------------------------//


  RSlicingNode::RSlicingNode( std::string value, SlicingNode* parent ): SlicingNode(parent)
  {
    std::string::size_type sz;
    _value = stof(value, &sz);
  }


  RSlicingNode::RSlicingNode( double value, SlicingNode* parent ): SlicingNode(parent)
  {
    _value = value;
  }

 
  void RSlicingNode::setValue( string value )
  {
    std::string::size_type sz;
    _value = stof(value, &sz);
  }

  
  void RSlicingNode::print() const
  {
    cerr << "type     : Routing " << endl;
    cerr << "value    : " << _value << endl;
    cerr << endl;
  }


  void RSlicingNode::toXML ( ofstream& file, unsigned indent )
  {
    for (unsigned i = 0 ; i < indent ; i++){ file << "  "; }
    file << "<routing value=\"" << valueToString() << "\"/>" << endl;
  }

} // OpenChams namespace
