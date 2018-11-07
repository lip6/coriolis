// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ChannelRouting.cpp"                          |
// +-----------------------------------------------------------------+


#include "bora/ChannelRouting.h"


namespace Bora {

  using namespace std;
  

// -------------------------------------------------------------------
// Class  :  "Bora::ChannelRouting".

  
  ChannelRouting::ChannelRouting ()
    : _wireIntervals()
  { }


  ChannelRouting::~ChannelRouting ()
  { }


  ChannelRouting* ChannelRouting::create ()
  {
    return new ChannelRouting();
  }


  void  ChannelRouting::destroy ()
  {
    delete this;
  }


  void ChannelRouting::reset ()
  {
    _wireIntervals.clear();
  }


  int  ChannelRouting::getMaxCount () const
  {
    return _wireIntervals.getThickness();
  }


  void  ChannelRouting::insertChannel ( DbU::Unit xy1, DbU::Unit xy2, unsigned int w )
  {
    cdebug_log(535,0) << "ChannelRouting::insertChannel(DbU::Unit,DbU::Unit,unsigned) ["
                      << DbU::getValueString(xy1) << " : "
                      << DbU::getValueString(xy2) << "] width:"
                      << w << endl;
    
    for ( unsigned int i=0; i<w; ++i ) insertChannel( xy1, xy2, (Net*)NULL );
  }


  void  ChannelRouting::insertChannel ( DbU::Unit xy1, DbU::Unit xy2, Net* net )
  {
    cdebug_log(535,0) << "ChannelRouting::insertChannel(DbU::Unit,DbU::Unit) ["
                      << DbU::getValueString(xy1) << " : "
                      << DbU::getValueString(xy2) << "] "
                      << net
                      << endl;
    
    _wireIntervals.insert( WireInterval(net,xy1,xy2) );
  }


  void  ChannelRouting::print () const
  {
    cerr << "Thickness: " << getMaxCount() << endl;
    cerr << "Wires:" << endl;
    for ( const WireInterval& wire : _wireIntervals.getElements() )
      cerr << "| " << wire << endl;
  }


}  // Bora namespace.
