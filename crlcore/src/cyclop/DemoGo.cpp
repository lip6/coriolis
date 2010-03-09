
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DemoGo.cpp"                             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QColor>
#include  <QPen>

#include  "hurricane/Cell.h"
#include  "hurricane/viewer/CellWidget.h"

#include  "DemoGo.h"


namespace CRL {


  const Name DemoGo::_extensionName = "Cyclop::DemoGo";


  DemoGo::DemoGo ( Cell* cell, const Box& box )
    : ExtensionGo(cell)
    , _box(box)
  { }


  DemoGo* DemoGo::create ( Cell* cell, const Box& box )
  {
    DemoGo* go = new DemoGo ( cell, box );
    go->_postCreate ();
    return go;
  }


  const Name& DemoGo::staticGetName ()
  { return _extensionName; }

  const Name& DemoGo::getName () const
  { return _extensionName; }

  Box  DemoGo::getBoundingBox () const
  { return _box; }

  void  DemoGo::translate ( const DbU::Unit& tx, const DbU::Unit& ty )
  { _box.translate ( tx, ty ); }

  string  DemoGo::_getTypeName () const
  { return "DemoGo"; }


  void  DemoGo::drawDemoGo ( CellWidget*            widget
                           , const Go*              go
                           , const BasicLayer*      basicLayer
                           , const Box&             area
                           , const Transformation&  transformation
                           )
  {
    cerr << "DemoGo::drawDemoGo() - " << go << endl;
    widget->drawBox ( transformation.getBox(go->getBoundingBox()) );
  }


  void  DemoGo::initDrawDemoGo ( CellWidget* widget )
  {
    cerr << "DemoGo::initDrawDemoGo() - " << endl;
    QPen demoPen = QPen(QColor(255,0,0));
    demoPen.setWidth ( 10 );
    widget->getDrawingPlanes().setPen   ( demoPen );
    widget->getDrawingPlanes().setBrush ( Qt::NoBrush );
  }



} // End of CRL namespace.
