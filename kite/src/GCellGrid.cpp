
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./GCellGrid.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>

#include  "hurricane/Cell.h"
#include  "katabatic/GCellGrid.h"
#include  "kite/GCell.h"
#include  "kite/GCellGrid.h"
#include  "kite/KiteEngine.h"


namespace Kite {


  using std::cerr;
  using std::endl;

  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;


// -------------------------------------------------------------------
// Class  :  "GCellGrid".


  GCellGrid::GCellGrid ( KiteEngine* kite )
    : Katabatic::Grid<GCell>()
    , _kite(kite)
  { }


  void  GCellGrid::_postCreate ()
  {
  // Clone the grid from Katabatic Here.
    Katabatic::GCellGrid*      base       = _kite->base()->getGCellGrid();
    vector<Katabatic::GCell*>* baseGCells = base->getGCellVector();

    ltrace(80) << "Kite GCell Matrix [" << baseGCells->size() << "]" << endl;

    for ( size_t i=0 ; i<baseGCells->size() ; i++ )
      _gcells.push_back ( GCell::create(this,(*baseGCells)[i]) );

    _rows         = base->getRows();
    _columns      = base->getColumns();
    _rawSize      = base->getRawSize();
    _xGraduations = base->getXGrads();
    _yGraduations = base->getYGrads();
  }


  GCellGrid::~GCellGrid ()
  { }


  void  GCellGrid::_preDestroy ()
  {
    ltrace(90) << "Kite::GCellGrid::_preDestroy()" << endl;
    ltracein(90);

    vector<GCell*>::iterator   it = _gcells.begin();
    vector<GCell*>::iterator  end = _gcells.end  ();
    for ( ; it != end ; it++ ) (*it)->destroy ();

    ltraceout(90);
  }


  Cell* GCellGrid::getCell () const
  {
    return _kite->getCell();
  }


  GCellGrid* GCellGrid::create ( KiteEngine* kite )
  {
    GCellGrid* subFCellGrid = new Kite::GCellGrid ( kite );

    subFCellGrid->_postCreate ();

    return subFCellGrid;
  }


  double  GCellGrid::getTotalWireLength () const
  {
    double  totalWL = 0;
    for ( size_t i=0 ; i < _gcells.size() ; i++ ) {
      totalWL += _gcells[i]->getOwnedWireLength ();
    }
    return totalWL;
  }


  void  GCellGrid::_check () const
  {
    cerr << "  o  Checking Kite GCell Grid." << endl;
    for ( size_t i=0 ; i < _gcells.size() ; i++ ) {
      if ( _gcells[i]->base() == NULL ) {
        cerr << "[CHECK] _gcells[" << i << "]: " << (void*)_gcells[i] << " has NULL base." << endl;
      }
    }
    cerr << "     - Successful." << endl;
  }


  string  GCellGrid::_getTypeName () const
  {
    return "Kite::GCellGrid";
  }


  string  GCellGrid::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(getRows()) + "x" + getString(getColumns()) + ">";
  }


  Record* GCellGrid::_getRecord () const
  {
    Record* record = Katabatic::Grid<GCell>::_getRecord ();
    record->add ( getSlot ( "_kite", _kite ) );
    return record;

    return record;
  }


} // End of Kite namespace.
