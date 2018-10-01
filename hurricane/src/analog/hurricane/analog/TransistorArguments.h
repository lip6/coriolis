// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/TransistorArguments.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_TRANSISTOR_ARGUMENTS_H
#define ANALOG_TRANSISTOR_ARGUMENTS_H

#include "hurricane/DbU.h"
#include "hurricane/analog/Arguments.h"

namespace Analog {
  

  class TransistorArguments : public Arguments {
    public:
      typedef Arguments Super;
    public:
                                    TransistorArguments ( bool                 isNMOS
                                                        , bool                 isBulkConnected
                                                        , bool                 isSourceFirst
                                                        , std::string          bulkType
                                                        , Hurricane::DbU::Unit w
                                                        , Hurricane::DbU::Unit l
                                                        , unsigned             m
                                                        , unsigned             externalDummy );
                                    TransistorArguments ( const TransistorArguments& tArgs );
      virtual TransistorArguments*  getClone            ();
      inline  bool                  isNMOS              () const;
      inline  bool                  isBulkConnected     () const;
      inline  bool                  isSourceFirst       () const;
      inline  unsigned int          getM                () const;
      inline  unsigned int          getExternalDummy    () const;
      inline  std::string           getBulkType         () const;
      inline  Hurricane::DbU::Unit  getW                () const;
      inline  Hurricane::DbU::Unit  getL                () const;
      inline  void                  setM                ( unsigned int m);
      inline  void                  setExternalDummy    ( unsigned int externalDummy );
    private:
      bool                 _isNMOS;
      bool                 _isBulkConnected;
      bool                 _isSourceFirst;
      std::string          _bulkType;
      Hurricane::DbU::Unit _w;
      Hurricane::DbU::Unit _l;
      unsigned             _m;
      unsigned             _externalDummy;
  };

  
  inline  bool                  TransistorArguments::isNMOS           () const { return _isNMOS; };
  inline  bool                  TransistorArguments::isBulkConnected  () const { return _isBulkConnected; };
  inline  bool                  TransistorArguments::isSourceFirst    () const { return _isSourceFirst; };
  inline  unsigned int          TransistorArguments::getM             () const { return _m; };
  inline  unsigned int          TransistorArguments::getExternalDummy () const { return _externalDummy; };
  inline  std::string           TransistorArguments::getBulkType      () const { return _bulkType; };
  inline  Hurricane::DbU::Unit  TransistorArguments::getW             () const { return _w; };
  inline  Hurricane::DbU::Unit  TransistorArguments::getL             () const { return _l; };
  inline  void                  TransistorArguments::setM             ( unsigned int m)  { _m = m; };
  inline  void                  TransistorArguments::setExternalDummy ( unsigned int externalDummy ) { _externalDummy = externalDummy; };
  

}  // Analog namespace.

#endif // ANALOG_TRANSISTOR_ARGUMENTS_H
