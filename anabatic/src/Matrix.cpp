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
#include "hurricane/Cell.h"
#include "anabatic/Matrix.h"
#include "anabatic/GCell.h"


namespace Anabatic {

  using std::cout;
  using std::cerr;
  using std::endl;
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
    _imax = _area.getWidth () / side;
    _jmax = _area.getHeight() / side;
    _gcells.resize( _imax*_jmax );
  }


  Matrix::~Matrix ()
  { }


  void  Matrix::setCell ( Cell* cell, DbU::Unit side )
  {
    _area = cell->getAbutmentBox();
    _side = side;
    _imax = _area.getWidth () / side;
    _jmax = _area.getHeight() / side;
    _gcells.resize( _imax*_jmax );
  }


  void  Matrix::updateLookup ( GCell* gcell )
  {
    cdebug.log(110,1) << "Matrix::updateLookup(): " << gcell << endl;

    Box gcellBb    = gcell->getBoundingBox();
    Box updateArea = _area.getIntersection( gcellBb );

    if (updateArea.isEmpty()) {
      cerr << Error( "Matrix::updateLookup(): %s is not under area of %s."
                   , getString(gcell).c_str()
                   , getString(this).c_str()
                   ) << endl;
    }

    Index  indexMin = Index( this, updateArea.getXMin()  , updateArea.getYMin()   );
    Index  indexMax = Index( this, updateArea.getXMax()-1, updateArea.getYMax()-1 );
    int    xspan    = indexMax.i() - indexMin.i();

    cdebug.log(110) << "indexMin:" << indexMin << endl;
    cdebug.log(110) << "indexMax:" << indexMax << endl;

    int index = indexMin.index();
    while ( index <= indexMax.index() ) {
      _gcells[index] = gcell;

      if (index <= indexMax.j()) ++index;
      else                         index += _imax - xspan;
    }

    cdebug.tabw(110,-1);
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
