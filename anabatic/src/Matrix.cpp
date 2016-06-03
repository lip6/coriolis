// -*- mode: C++; explicit-buffer-name: "Matrix.cpp<anabatic>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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

    cdebug.log(110) << "Matrix::setCell(): " << this << endl;
  }


  GCell*  Matrix::getUnder ( DbU::Unit x, DbU::Unit y ) const
  {
    int index = xy2maxIndex(x,y);
    cdebug.log(110) << "Matrix::getUnder() ("
                    << DbU::getValueString(x) << " "
                    << DbU::getValueString(y) << " " << index << endl;
    return (index < 0) ? NULL : _gcells[index]->getUnder(x,y);
  }


  void  Matrix::updateLookup ( GCell* gcell )
  {
    cdebug.log(110,1) << "Matrix::updateLookup(): " << gcell << endl;

    if (gcell->isFlat()) {
      cdebug.log(110) << " GCell is flat, no update." << endl;
      cdebug.tabw(110,-1);
      return;
    }

    Box gcellBb    = gcell->getBoundingBox();
    Box updateArea = _area.getIntersection( gcellBb );

    cdebug.log(110) << "_side            " << _side << endl; 
    cdebug.log(110) << "_area.getXMin()  " << _area.getXMin() << endl; 
    cdebug.log(110) << "_area.getYMin()  " << _area.getYMin() << endl; 
    cdebug.log(110) << "_area.getXMax()  " << _area.getXMax() << endl; 
    cdebug.log(110) << "_area.getYMax()  " << _area.getYMax() << endl; 
    cdebug.log(110) << "updateArea.getXMin()  " << updateArea.getXMin() << endl; 
    cdebug.log(110) << "updateArea.getYMin()  " << updateArea.getYMin() << endl; 
    cdebug.log(110) << "updateArea.getXMax()  " << updateArea.getXMax() << endl; 
    cdebug.log(110) << "updateArea.getYMax()  " << updateArea.getYMax() << endl; 

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

    cdebug.log(110) <<  "raw_i:" << (dx / _side + ((dx%_side) ? 1 : 0))
                    << " raw_j:" << (dy / _side + ((dy%_side) ? 1 : 0)) << endl;
    cdebug.log(110) << "indexMin:" << indexMin << endl;
    cdebug.log(110) << "indexMax:" << indexMax << endl;
    cdebug.log(110) << "xspan:   " << xspan << endl;

    if (not indexMin.valid() or not indexMax.valid()) { cdebug.tabw(110,-1); return; }

    int index = indexMin.index();
    while ( index <= indexMax.index() ) {
      if (updateArea.contains(getGridPoint(index))) _gcells[index] = gcell;

      if (index <= indexMax.j()) ++index;
      else                         index += _imax - xspan;
    }

    cdebug.tabw(110,-1);
  }


  void  Matrix::show () const
  {
    cdebug.log(111) << this << endl;
    for ( size_t i=0 ; i<_gcells.size() ; ++i ) {
      cdebug.log(111) << "[" << setw(3) << setfill('0') << i << setfill(' ') << "] ("
                      << setw(3) << index2i(i) << ","
                      << setw(3) << index2j(i) << ") " << _gcells[i] << endl;
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
