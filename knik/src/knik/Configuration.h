// -*- mode: C++; explicit-buffer-name: "Configuration.h<knik>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |                 Knik - Global Router                            |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Configuration.h"                        |
// +-----------------------------------------------------------------+


#ifndef  KNIK_CONFIGURATION_H
#define  KNIK_CONFIGURATION_H


namespace Hurricane {
  class Layer;
}


namespace Knik {


  using Hurricane::Layer;


  class Configuration {
    public:
      static Configuration* get           ();
      static const Layer*   getPinMetal   ();
      static const Layer*   getGMetalH    ();
      static const Layer*   getGMetalV    ();
      static const Layer*   getGContact   ();
      void destroy();
    private:
      static Configuration* _singleton;
             const Layer*   _pinMetal;
             const Layer*   _gMetalH;
             const Layer*   _gMetalV;
             const Layer*   _gContact;
    protected:
                            Configuration ( const Layer* pinMetal
                                          , const Layer* gMetalH
                                          , const Layer* gMetalV
                                          , const Layer* gContact
                                          );
      inline const Layer*   _getPinMetal  () const;
      inline const Layer*   _getGMetalH   () const;
      inline const Layer*   _getGMetalV   () const;
      inline const Layer*   _getGContact  () const;
  };


  inline const Layer* Configuration::_getPinMetal () const { return _pinMetal; }
  inline const Layer* Configuration::_getGMetalH  () const { return _gMetalH; }
  inline const Layer* Configuration::_getGMetalV  () const { return _gMetalV; }
  inline const Layer* Configuration::_getGContact () const { return _gContact; }


} // Knik namespace.

#endif // KNIK_CONFIGURATION_H
