
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
// |  C++ Header  :       "./DemoGo.h"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __DEMO_GO__
#define  __DEMO_GO__


#include  <string>

#include  "hurricane/Box.h"
#include  "hurricane/Name.h"
#include  "hurricane/ExtensionGo.h"

namespace Hurricane {
  class Transformation;
  class BasicLayer;
  class Cell;
  class CellWidget;
}


namespace CRL {

  using namespace std;

  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Transformation;
  using Hurricane::Name;
  using Hurricane::BasicLayer;
  using Hurricane::Go;
  using Hurricane::ExtensionGo;
  using Hurricane::Cell;
  using Hurricane::CellWidget;


  class DemoGo : public ExtensionGo {
    public:
      static  void        initDrawDemoGo ( CellWidget* widget );
      static  void        drawDemoGo     ( CellWidget*            widget
                                         , const Go*              go
                                         , const BasicLayer*      basicLayer
                                         , const Box&             area
                                         , const Transformation&  transformation
                                         );
    public:
      static  DemoGo*     create         ( Cell* , const Box& );
      static  const Name& staticGetName  ();
      virtual const Name& getName        () const;
      virtual Box         getBoundingBox () const;
      virtual void        translate      ( const DbU::Unit& tx, const DbU::Unit& ty );
      virtual string      _getTypeName   () const;
    protected:
      static const Name   _extensionName;
             Box          _box;
    protected:
                          DemoGo        ( Cell* , const Box& );
  };




} // End of Hurricane namespace.


#endif  // __DEMO_GO__
