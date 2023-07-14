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
// |  C++ Header  :  "./hurricane/analog/TransistorFamily.h"         |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/DbU.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/MetaTransistor.h"

namespace Hurricane {
  class Name;
  class Library;
}


namespace Analog {


  class TransistorFamily : public Device {
    public:
      typedef Device Super;
  
      enum Type { NMOS=1, PMOS };
    public:
      inline        MetaTransistor* getReferenceTransistor     ();
      inline  const MetaTransistor* getReferenceTransistor     () const;
      inline  const Type&           getType                    () const;
    // Geometrical (layout) parameters commons to all MOS transistors.
      inline        bool            isNMOS                     () const;
      inline        bool            isPMOS                     () const;
      inline        bool            isBulkConnected            () const;
      inline        bool            isSourceFirst              () const;
      inline        long            getBulkType                () const;
      inline        float           getWmin                    () const;
      inline        float           getWmax                    () const;
      inline        int             getNfing                   () const;
      inline        DbU::Unit       getW                       () const;
      inline        DbU::Unit       getL                       () const;
      inline        long            getM                       () const;
      inline        long            getExternalDummy           () const;
      inline        void            setSourceFirst             ( const bool );
      inline        void            setWmin                    ( float );
      inline        void            setWmax                    ( float );
      inline        void            setNfing                   ( int );
      inline        void            setExternalDummy           ( long );
      inline        void            setBulkType                ( long );
    // Electrical parameters commons to all MOS transistors.
      inline        float           getWE                      () const;
      inline        float           getLE                      () const;
      inline        float           getIDS                     () const;
      inline        float           getVGS                     () const;
      inline        float           getVDS                     () const;
      inline        float           getVBS                     () const;
      inline        float           getVG                      () const;
      inline        float           getVD                      () const;
      inline        float           getVS                      () const;
      inline        float           getVB                      () const;
      inline        float           getVEG                     () const;
      inline        float           getVTH                     () const;
      inline        void            setWE                      ( float );
      inline        void            setLE                      ( float );
      inline        void            setIDS                     ( float );
      inline        void            setVGS                     ( float );
      inline        void            setVDS                     ( float );
      inline        void            setVBS                     ( float );
      inline        void            setVG                      ( float );
      inline        void            setVD                      ( float );
      inline        void            setVS                      ( float );
      inline        void            setVB                      ( float );
      inline        void            setVEG                     ( float );
      inline        void            setVTH                     ( float );
    // Sizing parameters commons to all MOS transistors.
      inline        int             getOperatorIndex           () const;
      inline        void            setOperatorIndex           ( int );
      virtual       Record*         _getRecord                 () const;
  
    protected:
                                    TransistorFamily           ( Hurricane::Library*
                                                               , const Hurricane::Name&
                                                               , const Type& );
                    void            _postCreate                ( const Name& deviceName );
      inline        void            setReferenceTransistor     ( MetaTransistor* );
      virtual       void            createConnections          ( bool bulkConnected ) = 0;
    private:
      inline        MetaTransistor* _secureGetReferenceTransistor ();
      inline  const MetaTransistor* _secureGetReferenceTransistor () const;
    private:
      const Type                 _type;
            MetaTransistor*      _referenceTransistor;
            int                  _operatorIndex;
            StepParameter*       _w;
            StepParameter*       _l;
            FormFactorParameter* _m;
            SpinBoxParameter*    _externalDummy;
            MCheckBoxParameter*  _sourceFirst;
            MCheckBoxParameter*  _bulkType;
  };
  
  
  inline       MetaTransistor*         TransistorFamily::getReferenceTransistor () { return _referenceTransistor; }
  inline const MetaTransistor*         TransistorFamily::getReferenceTransistor () const { return _referenceTransistor; }
  inline const TransistorFamily::Type& TransistorFamily::getType                () const { return _type; }
  inline       int                     TransistorFamily::getOperatorIndex       () const { return _operatorIndex; }
  inline       void                    TransistorFamily::setOperatorIndex       ( int i ) { _operatorIndex = i; }
  inline       void                    TransistorFamily::setReferenceTransistor ( MetaTransistor* mt ) { _referenceTransistor = mt; }
  inline       bool                    TransistorFamily::isNMOS                 () const { return getType() == NMOS; }
  inline       bool                    TransistorFamily::isPMOS                 () const { return getType() == PMOS; }
  inline       bool                    TransistorFamily::isBulkConnected        () const { return !getNet(Hurricane::Name("B")); }
  inline       bool                    TransistorFamily::isSourceFirst          () const { return (_sourceFirst->getValue() == 1); }
  // Mutators.
  inline       void                    TransistorFamily::setSourceFirst         ( const bool sourceFirst ) { _sourceFirst->setValue ( (sourceFirst)?1:0 ); }
  inline       void                    TransistorFamily::setWmin                ( float wmin  ) { _secureGetReferenceTransistor()->setWmin (wmin ); }
  inline       void                    TransistorFamily::setWmax                ( float wmax  ) { _secureGetReferenceTransistor()->setWmax (wmax ); }
  inline       void                    TransistorFamily::setNfing               ( int   nfing ) { _secureGetReferenceTransistor()->setNfing(nfing); _m->setValue(nfing); }
  inline       void                    TransistorFamily::setExternalDummy       ( long  ndumm ) { _externalDummy->setValue(ndumm); }
  inline       void                    TransistorFamily::setBulkType            ( long  btype ) { _bulkType->setValue(btype); }
  inline       void                    TransistorFamily::setWE                  ( float we    ) { _secureGetReferenceTransistor()->setWE   (we   ); }
  inline       void                    TransistorFamily::setLE                  ( float le    ) { _secureGetReferenceTransistor()->setLE   (le   ); }
  inline       void                    TransistorFamily::setIDS                 ( float ids   ) { _secureGetReferenceTransistor()->setIDS  (ids  ); }
  inline       void                    TransistorFamily::setVGS                 ( float vgs   ) { _secureGetReferenceTransistor()->setVGS  (vgs  ); }
  inline       void                    TransistorFamily::setVDS                 ( float vds   ) { _secureGetReferenceTransistor()->setVDS  (vds  ); }
  inline       void                    TransistorFamily::setVBS                 ( float vbs   ) { _secureGetReferenceTransistor()->setVBS  (vbs  ); }
  inline       void                    TransistorFamily::setVG                  ( float vg    ) { _secureGetReferenceTransistor()->setVG   (vg   ); }
  inline       void                    TransistorFamily::setVD                  ( float vd    ) { _secureGetReferenceTransistor()->setVD   (vd   ); }
  inline       void                    TransistorFamily::setVS                  ( float vs    ) { _secureGetReferenceTransistor()->setVS   (vs   ); }
  inline       void                    TransistorFamily::setVB                  ( float vb    ) { _secureGetReferenceTransistor()->setVB   (vb   ); }
  inline       void                    TransistorFamily::setVEG                 ( float veg   ) { _secureGetReferenceTransistor()->setVEG  (veg  ); }
  inline       void                    TransistorFamily::setVTH                 ( float vth   ) { _secureGetReferenceTransistor()->setVTH  (vth  ); }
  // Accessors.                        
  inline       float                   TransistorFamily::getWmin                () const { return _secureGetReferenceTransistor()->getWmin (); }
  inline       float                   TransistorFamily::getWmax                () const { return _secureGetReferenceTransistor()->getWmax (); }
  inline       int                     TransistorFamily::getNfing               () const { return _secureGetReferenceTransistor()->getNfing(); }
  inline       DbU::Unit               TransistorFamily::getW                   () const { return _w->getValue(); }
  inline       DbU::Unit               TransistorFamily::getL                   () const { return _l->getValue(); }
  inline       long                    TransistorFamily::getM                   () const { return _m->getValue(); }
  inline       long                    TransistorFamily::getExternalDummy       () const { return _externalDummy->getValue(); }
  inline       long                    TransistorFamily::getBulkType            () const { return _bulkType->getValue(); }
  inline       float                   TransistorFamily::getWE                  () const { return _secureGetReferenceTransistor()->getWE   (); }
  inline       float                   TransistorFamily::getLE                  () const { return _secureGetReferenceTransistor()->getLE   (); }
  inline       float                   TransistorFamily::getIDS                 () const { return _secureGetReferenceTransistor()->getIDS  (); }
  inline       float                   TransistorFamily::getVGS                 () const { return _secureGetReferenceTransistor()->getVGS  (); }
  inline       float                   TransistorFamily::getVDS                 () const { return _secureGetReferenceTransistor()->getVDS  (); }
  inline       float                   TransistorFamily::getVBS                 () const { return _secureGetReferenceTransistor()->getVBS  (); }
  inline       float                   TransistorFamily::getVG                  () const { return _secureGetReferenceTransistor()->getVG   (); }
  inline       float                   TransistorFamily::getVD                  () const { return _secureGetReferenceTransistor()->getVD   (); }
  inline       float                   TransistorFamily::getVS                  () const { return _secureGetReferenceTransistor()->getVS   (); }
  inline       float                   TransistorFamily::getVB                  () const { return _secureGetReferenceTransistor()->getVB   (); }
  inline       float                   TransistorFamily::getVEG                 () const { return _secureGetReferenceTransistor()->getVEG  (); }
  inline       float                   TransistorFamily::getVTH                 () const { return _secureGetReferenceTransistor()->getVTH  (); }
  
  inline  MetaTransistor* TransistorFamily::_secureGetReferenceTransistor ()
  {
    if (!_referenceTransistor) throw Hurricane::Error("No MetaTransistor");
    return _referenceTransistor;
  }
  
  inline const MetaTransistor* TransistorFamily::_secureGetReferenceTransistor () const
  {
    if (!_referenceTransistor) throw Hurricane::Error("No MetaTransistor");
    return _referenceTransistor;
  }


}  // Analog namespace.


// -------------------------------------------------------------------
// Inspector Support for  :  Net::Type::Code*".

template<>
inline std::string  getString<const Analog::TransistorFamily::Type*>
                             ( const Analog::TransistorFamily::Type* object )
                             {
                               switch ( *object ) {
                                 case Analog::TransistorFamily::NMOS: return "NMOS";
                                 case Analog::TransistorFamily::PMOS: return "PMOS";
                               }
                               return "Unknow (error)";
                             }

template<>
inline Hurricane::Record* getRecord<const Analog::TransistorFamily::Type*>
                                   ( const  Analog::TransistorFamily::Type* object )
                                   {
                                     Hurricane::Record* record = new Hurricane::Record(getString(object));
                                     record->add(getSlot("Type", (unsigned int*)object));
                                     return record;
                                   }

IOSTREAM_POINTER_SUPPORT(Analog::TransistorFamily::Type);
IOSTREAM_VALUE_SUPPORT(Analog::TransistorFamily::Type);
