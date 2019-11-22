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
// |  C++ Header  :  "./hurricane/analog/ResistorFamily.h"           |
// +-----------------------------------------------------------------+


#ifndef ANALOG_RESISTOR_FAMILY_H
#define ANALOG_RESISTOR_FAMILY_H

#include "hurricane/DbU.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/MetaResistor.h"

namespace Hurricane {
  class Name;
  class Library;
}


namespace Analog {


  class ResistorFamily : public Device {
    public:
      typedef Device Super;
  
      enum Type { LOWRES=1, HIRES, RPOLYH, RPOLY2PH };
    public:
      inline        MetaResistor*   getReferenceResistor       ();
      inline  const MetaResistor*   getReferenceResistor       () const;
      inline  const Type&           getType                    () const;
    // Geometrical (layout) parameters commons to all resistors.
      inline        bool            isLOWRES                   () const;
      inline        bool            isHIRES                    () const;
      inline        bool            isRPOLYH                   () const;
      inline        bool            isRPOLY2PH                 () const;
      inline        int             getBends                   () const;
      inline        DbU::Unit       getW                       () const;
      inline        DbU::Unit       getL                       () const;
      inline        float           getR                       () const;
      inline        void            setR                       ( float );
      inline        void            setBends                   ( int );
    // Electrical parameters commons to all resistors.
      inline        float           getWE                      () const;
      inline        float           getLE                      () const;
      inline        void            setWE                      ( float );
      inline        void            setLE                      ( float );
    protected:
                                    ResistorFamily             ( Hurricane::Library*
                                                               , const Hurricane::Name&
                                                               , const Type& );
                    void            _postCreate                ( const Name& deviceName );
      inline        void            setReferenceResistor       ( MetaResistor* );
      virtual       void            createConnections          () = 0;
    private:
      inline        MetaResistor*   _secureGetReferenceResistor ();
      inline  const MetaResistor*   _secureGetReferenceResistor () const;
    private:
      const Type                 _type;
            MetaResistor*        _referenceResistor;
            StepParameter*       _w;
            StepParameter*       _l;
            FloatParameter*      _r;
            FormFactorParameter* _bends;
  };
  
  
  inline       MetaResistor*           ResistorFamily::getReferenceResistor   () { return _referenceResistor; }
  inline const MetaResistor*           ResistorFamily::getReferenceResistor   () const { return _referenceResistor; }
  inline const ResistorFamily::Type&   ResistorFamily::getType                () const { return _type; }
  inline       void                    ResistorFamily::setReferenceResistor   ( MetaResistor* mr ) { _referenceResistor = mr; }
  inline       bool                    ResistorFamily::isLOWRES               () const { return getType() == LOWRES; }
  inline       bool                    ResistorFamily::isHIRES                () const { return getType() == HIRES; }
  inline       bool                    ResistorFamily::isRPOLYH               () const { return getType() == RPOLYH; }
  inline       bool                    ResistorFamily::isRPOLY2PH             () const { return getType() == RPOLY2PH; }
  // Mutators.
  inline       void                    ResistorFamily::setR                   ( float r     ) { _secureGetReferenceResistor()->setR    (r    ); _r    ->setValue(r    ); }
  inline       void                    ResistorFamily::setBends               ( int   bends ) { _secureGetReferenceResistor()->setBends(bends); _bends->setValue(bends); }
  inline       void                    ResistorFamily::setWE                  ( float we    ) { _secureGetReferenceResistor()->setWE   (we   ); }
  inline       void                    ResistorFamily::setLE                  ( float le    ) { _secureGetReferenceResistor()->setLE   (le   ); }
  // Accessors.                        
  inline       int                     ResistorFamily::getBends               () const { return _secureGetReferenceResistor()->getBends(); }
  inline       DbU::Unit               ResistorFamily::getW                   () const { return _w->getValue(); }
  inline       DbU::Unit               ResistorFamily::getL                   () const { return _l->getValue(); }
  inline       float                   ResistorFamily::getR                   () const { return _r->getValue(); }
  inline       float                   ResistorFamily::getWE                  () const { return _secureGetReferenceResistor()->getWE   (); }
  inline       float                   ResistorFamily::getLE                  () const { return _secureGetReferenceResistor()->getLE   (); }
  
  inline  MetaResistor* ResistorFamily::_secureGetReferenceResistor ()
  {
    if (!_referenceResistor) throw Hurricane::Error("No MetaResistor");
    return _referenceResistor;
  }
  
  inline const MetaResistor* ResistorFamily::_secureGetReferenceResistor () const
  {
    if (!_referenceResistor) throw Hurricane::Error("No MetaResistor");
    return _referenceResistor;
  }


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::ResistorFamily);

#endif // ANALOG_RESISTOR_FAMILY_H
