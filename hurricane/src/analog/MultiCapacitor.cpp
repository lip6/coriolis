// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./MultiCapacitor.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/UpdateSession.h"
#include "hurricane/analog/MetaCapacitor.h"
#include "hurricane/analog/MultiCapacitor.h"

namespace Analog {
  
  using namespace Hurricane;


  const Name MultiCapacitor::_capacitorName ("MultiCapacitor");
  

  MultiCapacitor::MultiCapacitor (       Library*               library
                                 , const Name&                  name
                                 , const CapacitorFamily::Type& type
                                 ,       size_t                 count )
    : Super(library, name, type)
    , _metaCapacitor(NULL)
    , _count        (count)
  { }

  
  MultiCapacitor* MultiCapacitor::create ( Library*                     library
                                         , const Name&                  name
                                         , const CapacitorFamily::Type& type
                                         ,       size_t                 count )
  {
    preCreate( _capacitorName );
    UpdateSession::open();
    MultiCapacitor* mc = new MultiCapacitor( library, name, type, count );
    mc->_postCreate( _capacitorName );
    mc->createConnections     ();
    mc->addMatrixParameter    ( "matrix" );
    mc->addCapacitiesParameter( "capacities", count );
    mc->setTerminal           ( true );
    UpdateSession::close();

    return mc;
  }
  

  void MultiCapacitor::_postCreate ( const Name& deviceName ) 
  {
    Super::_postCreate( deviceName );
  }
  

  void MultiCapacitor::createConnections () 
  {
    for ( size_t i=0 ; i<_count ; ++i ) {
      string topPlateName = "t" + getString(i);
      string botPlateName = "b" + getString(i);
      string mcName       = "c" + getString(i);

      Net*           topPlate = Net::create( this, topPlateName );
      Net*           botPlate = Net::create( this, botPlateName );
      MetaCapacitor* mc       = MetaCapacitor::create( getSubDevicesLibrary(), mcName );
      Instance*      mcInst   = Instance::create( this, mcName, mc );

      topPlate->setExternal( true );
      botPlate->setExternal( true );

      mcInst->getPlug( mc->getTopPlate() )->setNet( topPlate );
      mcInst->getPlug( mc->getBotPlate() )->setNet( botPlate );

      if (not _metaCapacitor) {
        _metaCapacitor = mc;
        setReferenceCapacitor( mc );
      }
    }
  }


  Name MultiCapacitor::getDeviceName () const
  { return _capacitorName; }


  string  MultiCapacitor::_getTypeName () const 
  { return "MultiCapacitor"; }


}  // Analog namespace.