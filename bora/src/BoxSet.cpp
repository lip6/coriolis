// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      B o r a   -  A n a l o g   S l i c i n g   T r e e         |
// |                                                                 |
// |  Authors     :                         Eric LAO                 |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BoxSet.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "bora/BoxSet.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;


// -------------------------------------------------------------------
// Class  :  "Bora::BoxSet".


  BoxSet::BoxSet ( DbU::Unit height, DbU::Unit width )
    : _height(height)
    , _width (width)
    , _cpt   (1)
  { }


  BoxSet::BoxSet ( BoxSet* boxSet )
    : _height(0)
    , _width (0)
    , _cpt   (1)
  {
    if (boxSet) {
      _height = boxSet->getHeight();
      _width  = boxSet->getWidth();
      _cpt    = boxSet->getCpt();
    } 
  }

  
  BoxSet::~BoxSet ()
  { }


  void BoxSet::print () const 
  {
    cerr << "area: "            << setprecision(4) << getOccupationArea() ;
    cerr << "%, \t cpt: "       << getCpt();
    if ((getWidth() == 0) && (getHeight() == 0)) cerr << ", \t ratio (W/H):inf";
    else                                                     cerr << ", \t ratio (W/H):" << setprecision(2) << getWidth()/getHeight();
    cerr << ", \t ratio (W*H):" << setprecision(4) << DbU::getPhysical(getWidth(),DbU::Micro) * DbU::getPhysical(getHeight(),DbU::Micro) << "um²";
    if (getType() == DeviceSNode){ cerr << ", \t Nfing: " << getNFing(); }
    cerr << ", \t H: "          << setprecision(4) << DbU::getPhysical(getHeight(),DbU::Micro) << "um" ;
    cerr << ", W: "             << DbU::getPhysical(getWidth(),DbU::Micro)<< "um"; 
    cerr << endl; 
}


  const vector<BoxSet*>& BoxSet::getSet () const
  {
    cerr << Warning( "BoxSet::getSet(): DBoxSet and RHVBoxSet do not have vector of BoxSets." ) << endl;

    static const vector<BoxSet*> emptyBoxSet = vector<BoxSet*>();
    return emptyBoxSet;
  }


  int  BoxSet::getNFing () const
  {
    cerr << Warning( "BoxSet::getNFing(): Only DBoxSet has fingers." ) << endl;
    return 0;
  }


  void BoxSet::destroy()
  { delete this; }


  void  BoxSet::setWidth ( DbU::Unit )
  { cerr << Warning( "BoxSet::setWidth(): Must never be called." ) << endl; }


  void  BoxSet::setHeight ( DbU::Unit )
  { cerr << Warning( "BoxSet::setWidth(): Must never be called." ) << endl; }


// -------------------------------------------------------------------
// Class  :  "Bora::HVBoxSet".
                                       

  HVBoxSet::HVBoxSet ( const vector<BoxSet*>& dimensionSet, DbU::Unit height, DbU::Unit width )
    : BoxSet(height,width)
    ,_dimensionSet(dimensionSet)
  { }
                                       

  HVBoxSet::HVBoxSet ( HVBoxSet* boxSet )
    : BoxSet( boxSet )
    , _dimensionSet( boxSet->getSet() )
  { }


  HVBoxSet::~HVBoxSet ()
  { }


  double  HVBoxSet::getDevicesArea () const
  {
    double area = 0;
    for ( BoxSet* bs : _dimensionSet ) area += bs->getDevicesArea();
    return area;
  }


// -------------------------------------------------------------------
// Class  :  "Bora::HBoxSet".


  int  HBoxSet::_count    = 0;
  int  HBoxSet::_countAll = 0;


  HBoxSet::HBoxSet ( const vector<BoxSet*>& dimensionSet, DbU::Unit height, DbU::Unit width  )
    : HVBoxSet( dimensionSet, height, width )
  {
    if ((_height == 0) and (_width == 0)) {
      calculateHeight();
      calculateWidth ();
    }
  }


  HBoxSet::HBoxSet( HBoxSet* boxSet )
    : HVBoxSet( boxSet )
  { 
    ++_count;
    ++_countAll;
  }
  

  HBoxSet::~HBoxSet ()
  { 
    --_count;
  }


  HBoxSet* HBoxSet::create ( const vector<BoxSet*>& dimensionSet, DbU::Unit height, DbU::Unit width  )
  {
    return new HBoxSet( dimensionSet, height, width ); 
  }


  HBoxSet* HBoxSet::clone ()
  { return HBoxSet::create( getSet(), getHeight(), getWidth() ); }


  void  HBoxSet::calculateHeight () 
  {
    DbU::Unit currentH = 0;
    for ( BoxSet* bs : _dimensionSet ) currentH += bs->getHeight();
    _height = currentH;
  }


  void  HBoxSet::calculateWidth ()
  {
    DbU::Unit currentW = 0;
    for ( BoxSet* bs : _dimensionSet ) {
      if (currentW < bs->getWidth()) currentW = bs->getWidth();
    }
    _width = currentW;
  }


  void HBoxSet::destroy()
  {
    BoxSet::destroy();
  }


// -------------------------------------------------------------------
// Class  :  "Bora::VBoxSet".


  int  VBoxSet::_count    = 0;
  int  VBoxSet::_countAll = 0;


  VBoxSet::VBoxSet ( const vector<BoxSet*>& dimensionSet, DbU::Unit height, DbU::Unit width )
    : HVBoxSet( dimensionSet, height, width )
  {
    ++_count;
    ++_countAll;
    if ((_height == 0) and (_width == 0)){
      calculateHeight();
      calculateWidth ();
    }
  }


  VBoxSet::VBoxSet ( VBoxSet* boxSet )
    : HVBoxSet( boxSet )
  { }


  VBoxSet::~VBoxSet ()
  { 
    --_count;
  }


  VBoxSet* VBoxSet::create ( const vector<BoxSet*>& dimensionSet, DbU::Unit height, DbU::Unit width  )
  {
    return new VBoxSet( dimensionSet, height, width ); 
  }


  VBoxSet* VBoxSet::clone ()
  {
    return VBoxSet::create( getSet(), getHeight(), getWidth() );
  }


  void VBoxSet::calculateHeight()
  {
    DbU::Unit currentH = 0;
    for ( BoxSet* bs : _dimensionSet ) {
      if (currentH < bs->getHeight()) currentH = bs->getHeight();
    }
    _height = currentH;
  }


  void VBoxSet::calculateWidth ()
  {
    DbU::Unit currentW = 0;
    for ( BoxSet* bs : _dimensionSet ) currentW += bs->getWidth();
    _width = currentW;
  }


  void VBoxSet::destroy()
  {
    BoxSet::destroy();
  }


// -------------------------------------------------------------------
// Class  :  "Bora::DBoxSet".


  int  DBoxSet::_count    = 0;
  int  DBoxSet::_countAll = 0;


  DBoxSet::DBoxSet ( DbU::Unit height, DbU::Unit width, int nfing )
    : BoxSet( height, width )
    , _nfing(nfing)
  { 
    ++_count;
    ++_countAll;
  }


  DBoxSet::DBoxSet ( DBoxSet* boxSet )
    : BoxSet( boxSet )
    , _nfing( boxSet->getNFing() )
  { }


  DBoxSet::~DBoxSet ()
  {
    --_count;
  }


  DBoxSet* DBoxSet::create ( DbU::Unit height, DbU::Unit width, int nfing )
  {
    return new DBoxSet( height, width, nfing ); 
  }


  DBoxSet* DBoxSet::clone ()
  {
    return DBoxSet::create( getHeight(), getWidth(), getNFing() ); 
  }


  void  DBoxSet::destroy()
  {
    BoxSet::destroy();
  }


// -------------------------------------------------------------------
// Class  :  "Bora::RHVBoxSet".


  int  RHVBoxSet::_count    = 0;
  int  RHVBoxSet::_countAll = 0;


  RHVBoxSet::RHVBoxSet ( DbU::Unit height, DbU::Unit width )
    : BoxSet( height, width )
  {
    _width  = width;
    _height = height;
  }


  RHVBoxSet::RHVBoxSet ( RHVBoxSet* boxSet )
    : BoxSet( boxSet )
  { }


  RHVBoxSet::~RHVBoxSet ()
  { }


  void RHVBoxSet::destroy ()
  {
    delete this;
  }


  void RHVBoxSet::print () const 
  {
    cerr << "area: "            << setprecision(4) << getOccupationArea() ;
    cerr << "%, \t cpt: "       << getCpt();
    cerr << ", \t H: "          << setprecision(4) << DbU::getPhysical(_height,DbU::Micro) << "um" ;
    cerr << ", W: "             << DbU::getPhysical(_width,DbU::Micro)<< "um"; 
    cerr << endl; 
  }


// -------------------------------------------------------------------
// Class  :  "Bora::RHBoxSet".
  
  
  RHBoxSet::RHBoxSet ( DbU::Unit height )
    : RHVBoxSet( height, 0 )
  { 
    _count++;
    _countAll++;
  }


  RHBoxSet::RHBoxSet ( RHBoxSet* boxSet )
    : RHVBoxSet( boxSet )
  { }

  
  RHBoxSet::~RHBoxSet ()
  { }


  RHBoxSet* RHBoxSet::create ( DbU::Unit height )
  {
    return new RHBoxSet ( height );
  }


  RHBoxSet* RHBoxSet::clone ()
  {
    return RHBoxSet::create( getHeight() ); 
  }


// -------------------------------------------------------------------
// Class  :  "Bora::RVBoxSet".
  
  
  RVBoxSet::RVBoxSet ( DbU::Unit width )
    : RHVBoxSet( 0, width )
  { 
    ++_count;
    ++_countAll;
  }


  RVBoxSet::RVBoxSet( RVBoxSet* boxSet )
    : RHVBoxSet( boxSet )
  { }

  
  RVBoxSet::~RVBoxSet ()
  { }


  RVBoxSet* RVBoxSet::create ( DbU::Unit width )
  {
    return new RVBoxSet( width );
  }


  RVBoxSet* RVBoxSet::clone()
  {
    return RVBoxSet::create( getWidth() ); 
  }


}  // Bora namespace.
