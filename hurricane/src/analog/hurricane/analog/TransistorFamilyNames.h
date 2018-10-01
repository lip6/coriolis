// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :            C. Alexandre, J.-M. Caba              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/TransistorFamilyNames.h"    |
// +-----------------------------------------------------------------+


#ifndef ANALOG_TRANSISTOR_FAMILY_NAMES_H
#define ANALOG_TRANSISTOR_FAMILY_NAMES_H

namespace Analog {


  class TransistorFamilyNames {
    public:
      static const char* PMOSTransistorBU;
      static const char* PMOSTransistorBC;
      static const char* NMOSTransistorBU;
      static const char* NMOSTransistorBC;
      static const char* PMOSDifferentialPairBU;
      static const char* PMOSDifferentialPairBC;
      static const char* NMOSDifferentialPairBU;
      static const char* NMOSDifferentialPairBC;
      static const char* PMOSSimpleCurrentMirrorBU;
      static const char* PMOSSimpleCurrentMirrorBC;
      static const char* NMOSSimpleCurrentMirrorBU;
      static const char* NMOSSimpleCurrentMirrorBC;
      static const char* PMOSLevelShifterBU;
      static const char* PMOSLevelShifterBC;
      static const char* NMOSLevelShifterBU;
      static const char* NMOSLevelShifterBC;
      static const char* PMOSCrossCoupledPairBU;
      static const char* PMOSCrossCoupledPairBC;
      static const char* NMOSCrossCoupledPairBU;
      static const char* NMOSCrossCoupledPairBC;
      static const char* PMOSCommonSourcePairBU;
      static const char* PMOSCommonSourcePairBC;
      static const char* NMOSCommonSourcePairBU;
      static const char* NMOSCommonSourcePairBC;
      static const char* PMOSCommonGatePairBU;
      static const char* PMOSCommonGatePairBC;
      static const char* NMOSCommonGatePairBU;
      static const char* NMOSCommonGatePairBC;
      static const char* PMOSCommonDrainBU;
      static const char* PMOSCommonDrainBC;
      static const char* NMOSCommonDrainBU;
      static const char* NMOSCommonDrainBC;
      static const char* PMOSCascodeBU;
      static const char* PMOSCascodeBC;
      static const char* NMOSCascodeBU;
      static const char* NMOSCascodeBC;
  };


}  // Analog namespace.

#endif  // ANALOG_TRANSISTOR_FAMILY_NAMES_H
