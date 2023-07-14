// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/MetaResistor.h"             |
// +-----------------------------------------------------------------+


#ifndef ANALOG_METARESISTOR_H
#define ANALOG_METARESISTOR_H

#include "hurricane/Cell.h"


namespace Analog {

  using namespace Hurricane;


  class MetaResistor : public Cell {
    public:
      typedef Cell Super;
    public:
      static MetaResistor*   create     ( Library* , const Name& );
      inline Net*            getSource  () const;
      inline Net*            getPin1    () const;
      inline Net*            getPin2    () const;
      inline float           getWE      () const;
      inline float           getLE      () const;
      inline float           getR       () const;
      inline int             getBends   () const;
      inline void            setWE      ( float );
      inline void            setLE      ( float );
      inline void            setR       ( float );
      inline void            setBends   ( int );
    protected:                          
             void           _postCreate ();
    private:
                            MetaResistor ( Library* , const Name& );
    private:
      Net*          _pin1;
      Net*          _pin2;
      Net*          _anonymous;
      float         _r;
      float         _we;
      float         _le;
      int           _bends;
  };

  
  inline Net*   MetaResistor::getPin1    () const           { return _pin1; }
  inline Net*   MetaResistor::getPin2    () const           { return _pin2; }
  inline float  MetaResistor::getWE      () const           { return _we; }
  inline float  MetaResistor::getLE      () const           { return _le; }
  inline float  MetaResistor::getR       () const           { return _r; }
  inline int    MetaResistor::getBends   () const           { return _bends; }
  inline void   MetaResistor::setWE      ( float we )       { _we = we; }
  inline void   MetaResistor::setLE      ( float le )       { _le = le; }
  inline void   MetaResistor::setR       ( float r )        { _r = r; }
  inline void   MetaResistor::setBends   ( int bends )      { _bends = bends; }
  

}  // Analog namespace.

#endif  // ANALOG_METARESISTOR_H
