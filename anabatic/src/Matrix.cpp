// -*- mode: C++; explicit-buffer-name: "Matrix.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Matrix.cpp"                                  |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include <iomanip>
#include "hurricane/Cell.h"
#include "anabatic/Matrix.h"
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::setfill;
  using std::ostringstream;
  using Hurricane::Error;


  Matrix::Matrix ()
    : _area  ()
    , _side  (1)
    , _gcells()
    , _imax  (0)
    , _jmax  (0)
  { }


  Matrix::Matrix ( Box area, DbU::Unit side )
    : _area  (area)
    , _side  (side)
    , _gcells()
    , _imax  (0)
    , _jmax  (0)
  {
    _imax = _area.getWidth () / side + ((_area.getWidth () % side) ? 1 : 0);
    _jmax = _area.getHeight() / side + ((_area.getHeight() % side) ? 1 : 0);
    _gcells.resize( _imax*_jmax );
  }


  Matrix::~Matrix ()
  { }


  void  Matrix::setCell ( Cell* cell, DbU::Unit side )
  {
    _area = cell->getAbutmentBox();
    _side = side;
    _imax = _area.getWidth () / side + ((_area.getWidth () % side) ? 1 : 0);
    _jmax = _area.getHeight() / side + ((_area.getHeight() % side) ? 1 : 0);
    _gcells.resize( _imax*_jmax );

    cdebug_log(110,0) << "Matrix::setCell(): " << this << endl;
  }


  GCell*  Matrix::getUnder ( DbU::Unit x, DbU::Unit y ) const
  {
    if (x == _area.getXMax()) --x;
    if (y == _area.getYMax()) --y;

    int index = xy2maxIndex(x,y);
    cdebug_log(110,0) << "Matrix::getUnder() ("
                    << DbU::getValueString(x) << " "
                    << DbU::getValueString(y) << ") index:" << index << endl;

    return (index < 0) ? NULL : _gcells[index]->getUnder(x,y);
  }


  void  Matrix::updateLookup ( GCell* gcell )
  {
    cdebug_log(110,1) << "Matrix::updateLookup(): " << gcell << endl;

    if (gcell->isFlat()) {
      cdebug_log(110,0) << " GCell is flat, no update." << endl;
      cdebug_tabw(110,-1);
      return;
    }

    Box gcellBb    = gcell->getBoundingBox();
    Box updateArea = _area.getIntersection( gcellBb );

    cdebug_log(110,0) << "_side:      " << DbU::getValueString(_side) << endl; 
    cdebug_log(110,0) << "_area:      " << _area << endl; 
    cdebug_log(110,0) << "updateArea: " << updateArea << endl; 

    if (updateArea.isEmpty()) {
      cerr << Error( "Matrix::updateLookup(): %s is not under area of %s."
                   , getString(gcell).c_str()
                   , getString(this).c_str()
                   ) << endl;
    }

    Index  indexMin = Index::asMin( this, updateArea.getXMin(), updateArea.getYMin() );
    Index  indexMax = Index::asMax( this, updateArea.getXMax(), updateArea.getYMax() );
    int    xspan    = indexMax.i() - indexMin.i();

    DbU::Unit dx = updateArea.getXMin() - _area.getXMin();
    DbU::Unit dy = updateArea.getYMin() - _area.getYMin();

    cdebug_log(110,0) <<  "raw_i:" << (dx / _side + ((dx%_side) ? 1 : 0))
                      << " raw_j:" << (dy / _side + ((dy%_side) ? 1 : 0)) << endl;
    cdebug_log(110,0) << "indexMin:" << indexMin << endl;
    cdebug_log(110,0) << "indexMax:" << indexMax << endl;
    cdebug_log(110,0) << "xspan:   " << xspan << endl;

    if (not indexMin.valid() or not indexMax.valid()) { cdebug_tabw(110,-1); return; }

    int index = indexMin.index();
    while ( index <= indexMax.index() ) {
      cdebug_log(110,0) << "i,j = " << index2i(index) << "," << index2j(index)
                        << " " << getGridPoint(index) << endl;
      if (updateArea.contains(getGridPoint(index))) _gcells[index] = gcell;

      if (index2j(index) <= indexMax.j()) ++index;
      else                                  index += _imax - xspan;
    }

    cdebug_tabw(110,-1);
  }


  void  Matrix::resize ( Cell* cell, const vector<GCell*>& gcells )
  {
    setCell( cell, _side );
    for ( GCell* gcell : gcells ) updateLookup( gcell );
  }


  void  Matrix::show () const
  {
    cdebug_log(111,0) << this << endl;
    for ( size_t i=0 ; i<_gcells.size() ; ++i ) {
    //cdebug_log(111,0) << "[" << setw(3) << setfill('0') << i << setfill(' ') << "] ("
    //                  << setw(3) << index2i(i) << ","
    //                  << setw(3) << index2j(i) << ") " << _gcells[i] << endl;
      cdebug_log(111,0) << "[" <<  i << "] ("
                        << index2i(i) << ","
                        << index2j(i) << ") " << _gcells[i] << endl;
    }
  }


  string  Matrix::_getTypeName () const
  { return "Matrix"; }


  string  Matrix::_getString () const
  {
    ostringstream os;
    os << "<" << _getTypeName() << " " << _imax << "x" << _jmax
       << " " << _area << "/" << DbU::getValueString(_side) <<  ">";
    return os.str();
  }


  Record* Matrix::_getRecord () const
  {
    Record* record = new Record( _getString() );
    record->add(      getSlot     ("_area"  ,  _area  ) );
    record->add( DbU::getValueSlot("_side"  , &_side  ) );
    record->add(      getSlot     ("_imax"  ,  _imax  ) );
    record->add(      getSlot     ("_jmax"  ,  _jmax  ) );
    record->add(      getSlot     ("_gcells", &_gcells) );
    return record;
  }


}  // Anabatic namespace;
