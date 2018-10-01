// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     : Christophe Alexandre, Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./Device.cpp"                                  |
// +-----------------------------------------------------------------+


#include <cfloat>
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/Pad.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Library.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/Reference.h"
#include "hurricane/analog/Device.h"


namespace {

  using namespace Hurricane;


  static Name anonymousNetName( "anonymous" );

}  // anonymous namespace.


namespace Analog {

  using namespace Hurricane;


  Device::Device ( Library* library, const Name& name )
                 : Cell              (library, name)
                 , _temperature      (0.0)
                 , _parameterSet     ()
                 , _layoutStyles     (NULL)
                 , _subDevicesLibrary(NULL)
                 , _anonymous        (NULL)
                 , _layouts          ()
                 , _netRestrictions  ()
  { }
  

  void Device::preCreate ( const Name& deviceName )
  {
    Technology*       tech = DataBase::getDB()->getTechnology();
    DeviceDescriptor* dd   = tech->getDeviceDescriptor( deviceName );
    if (not dd) 
      throw Error( "Device::preCreate(): No DeviceDescriptor for \"%s\"."
                 , getString(deviceName).c_str() );
  }
  

  void Device::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate();
    setTerminal( true );
  
    _subDevicesLibrary = Library::create( getLibrary(), getName() );
    _anonymous = Net::create( this, anonymousNetName );
    _anonymous->setAutomatic( true );
    Technology*               tech      = DataBase::getDB()->getTechnology();
    DeviceDescriptor*         dd        = tech->getDeviceDescriptor( deviceName );
    DeviceDescriptor::Layouts ddLayouts = dd->getLayouts();

    for ( auto ddLayout : ddLayouts ) _layouts.insert( ddLayout );
  
    Choices layouts;
    for ( auto layout : _layouts ) layouts << getString(layout.first);
    _layoutStyles = addChoiceParameter( "Layout Styles", layouts );
  }
  

  void Device::_preDestroy ()
  {
    for ( Parameter* parameter : _parameterSet ) delete parameter;
    _subDevicesLibrary->destroy();
    Super::_preDestroy();
  }


  Parameter* Device::getParameter ( const string& parameterId )
  {
    for ( Parameter* parameter : _parameterSet) {
      if (parameter->getName() == parameterId)
        return parameter;
    }
    return NULL;
  }
  

  const string Device::getLayoutScript ()
  {
    Name              layoutName = Name( _layoutStyles->getValue() );
    Layouts::iterator findLayout = _layouts.find( layoutName );

    if (findLayout != _layouts.end()) return (*findLayout).second;
    return NULL;
  }
  

  void  Device::destroyLayout ()
  {
    UpdateSession::open();
  
    // size_t icomp = 0;
    // for ( Component* component : getComponents() ) {
    //   cerr << setw(4) << icomp << " | " << component << endl;
    // }
  
    while ( not getComponents().isEmpty() ) getComponents().getFirst()->destroy();
    while ( not getReferences().isEmpty() ) getReferences().getFirst()->destroy();
  
    for ( Net* net : getNets() ) {
      Rubbers rubbers = net->getRubbers();
      while ( rubbers.getFirst() ) rubbers.getFirst()->_destroy();
    }
  
    UpdateSession::close();
  }
  

  unsigned int Device::getRestriction( Hurricane::Net* net ) const
  { return 0xAA; }
  

  unsigned int  Device::getRestrictions2 ( Hurricane::Net* net ) const
  {
    map<Net*,unsigned int>::const_iterator inet = _netRestrictions.find( net );
    if (inet == _netRestrictions.end()) return AllBlocked;
    return (*inet).second;
  }
  

  void  Device::setRestrictions2 ( Hurricane::Net* net, unsigned int flags )
  {
    if (not net) return;
    if (net->getCell() != this) {
      cerr << Hurricane::Warning( "Device::setRestrictions2(): Net \"%s\" (of Device \"%s\")"
                                  " do not belong to \"%s\"."
                                , getString(net->getName()).c_str() 
                                , getString(net->getCell()->getName()).c_str() 
                                , getString(getName()).c_str() 
                                ) << endl;
      return;
    }
  
    _netRestrictions[ net ] = flags;
  }
  

  Hurricane::Record* Device::_getRecord () const
  {
    Record* record = Super::_getRecord();
    if (record) {
      record->add( getSlot("_subDevicesLibrary",  _subDevicesLibrary) );
      record->add( getSlot("_netRestrictions"  , &_netRestrictions  ) );
    }
  
    return record;
  }


}  // Analog namespace.
