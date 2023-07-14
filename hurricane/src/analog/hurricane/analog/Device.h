// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/Device.h"                   |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/Cell.h"
#include "hurricane/analog/StepParameter.h"
#include "hurricane/analog/FloatParameter.h"
#include "hurricane/analog/CapacitorParameter.h"
#include "hurricane/analog/ChoiceParameter.h"
#include "hurricane/analog/StringParameter.h"
#include "hurricane/analog/SpinBoxParameter.h"
#include "hurricane/analog/MCheckBoxParameter.h"
#include "hurricane/analog/FormFactorParameter.h"
#include "hurricane/analog/MatrixParameter.h"
#include "hurricane/analog/CapacitiesParameter.h"


namespace Analog {


  class Device : public Hurricane::Cell {
    public:
      struct ParameterCompare: public std::binary_function<Parameter*, Parameter*, bool> {
          bool operator() ( Parameter* dp1, Parameter* dp2 ) const {
            if (dp1->getIndex() < dp2->getIndex()) return true;
            return     (dp1->getIndex() == dp2->getIndex()
                   and (dp1->getName () <  dp2->getName ()));
          }
      };
      enum Restriction { NorthBlocked = (1<<0)
                       , SouthBlocked = (1<<1)
                       , EastBlocked  = (1<<2)
                       , WestBlocked  = (1<<3)
                       , AllBlocked   = NorthBlocked|SouthBlocked|EastBlocked|WestBlocked
                       };
    public:
      typedef  Hurricane::Cell                                    Super;
      typedef  std::set<Parameter*,ParameterCompare>              Parameters;
      typedef  std::map<const Hurricane::Name,const std::string>  Layouts;
    public:
      virtual       Hurricane::Name        getDeviceName             () const = 0;
                    Parameter*             getParameter              ( const std::string& parameterId ) const;
      inline        Parameters&            getParameters             ();
              const std::string            getLayoutScript           ();
      inline        float                  getTemperature            () const;
      inline        void                   setTemperature            ( float temperature );
      virtual       unsigned int           getRestriction            ( Hurricane::Net* net ) const;
                    void                   setRestrictions2          ( Hurricane::Net* net, unsigned int flags );
                    unsigned int           getRestrictions2          ( Hurricane::Net* net ) const;
                    void                   destroyLayout             ();
    protected:
                                           Device                    (Hurricane::Library*
                                                                     , const Hurricane::Name& );
      static        void                   preCreate                 ( const Hurricane::Name& );
      virtual       void                   _postCreate               ( const Hurricane::Name& deviceName );
      virtual       void                   _preDestroy               ();
      inline        Hurricane::Library*    getSubDevicesLibrary      ();
      inline        void                   addParameter              ( Parameter* parameter );
      inline        StepParameter*         addStepParameter          ( const std::string name, long min, long max, long step );
      inline        StringParameter*       addStringParameter        ( const std::string name, const std::string value="" );
      inline        FloatParameter*        addFloatParameter         ( const std::string name, float value=0.0 );
      inline        CapacitorParameter*    addCapacitorParameter     ( const std::string name, long value );
      inline        SpinBoxParameter*      addSpinBoxParameter       ( const std::string name, long min, long max );
      inline        FormFactorParameter*   addFormFactorParameter    ( const std::string name, long min, long max );
      inline        ChoiceParameter*       addChoiceParameter        ( const std::string name, const Choices& choices );
      inline        MCheckBoxParameter*    addMCheckBoxParameter     ( const std::string name, const Choices& choices );
      inline        MatrixParameter*       addMatrixParameter        ( const std::string name );
      inline        CapacitiesParameter*   addCapacitiesParameter    ( const std::string name, size_t count );
    public:
      virtual       Hurricane::Record*     _getRecord                () const;
    private:
      float                                   _temperature;
      Parameters                              _parameterSet;
      ChoiceParameter*                        _layoutStyles;
      Hurricane::Library*                     _subDevicesLibrary;
      Hurricane::Net*                         _anonymous;
      Layouts                                 _layouts;
      std::map<Hurricane::Net*,unsigned int>  _netRestrictions;
  };
  
  
  inline Device::Parameters& Device::getParameters        ()                    { return _parameterSet; }
  inline float               Device::getTemperature       () const              { return _temperature; }
  inline void                Device::setTemperature       ( float temperature ) { _temperature = temperature; }
  inline Hurricane::Library* Device::getSubDevicesLibrary ()                    { return _subDevicesLibrary; }
  
  inline void  Device::addParameter ( Parameter* parameter )
  {
    parameter->setIndex( _parameterSet.size() );
    _parameterSet.insert( parameter );
  }
  
  inline StepParameter* Device::addStepParameter ( const std::string name, long min, long max, long step )
  {
    StepParameter* stepParameter = new StepParameter( name,min,max,step );
    addParameter( stepParameter );
    return stepParameter;
  }
  
  inline StringParameter* Device::addStringParameter ( const std::string name, const std::string value )
  {
    StringParameter* stringParameter = new StringParameter( name, value );
    addParameter( stringParameter );
    return stringParameter;
  }

  inline FloatParameter* Device::addFloatParameter ( const std::string name, float value )
  {
    FloatParameter* floatParameter = new FloatParameter( name, value );
    addParameter( floatParameter );
    return floatParameter;
  }

  inline CapacitorParameter* Device::addCapacitorParameter ( const std::string name, long value )
  {
    CapacitorParameter* capacitorParameter = new CapacitorParameter( name,value );
    addParameter( capacitorParameter );
    return capacitorParameter;
  }
  
  inline SpinBoxParameter* Device::addSpinBoxParameter ( const std::string name, long min, long max )
  {
    SpinBoxParameter* spinBoxParameter = new SpinBoxParameter( name, min, max );
    addParameter( spinBoxParameter );
    return spinBoxParameter;
  }
  
  inline FormFactorParameter* Device::addFormFactorParameter ( const std::string name, long min, long max )
  {
    FormFactorParameter* formFactorParameter = new FormFactorParameter( name, min, max );
    addParameter( formFactorParameter );
    return formFactorParameter;
  }
  
  inline ChoiceParameter* Device::addChoiceParameter ( const std::string name, const Choices& choices )
  {
    ChoiceParameter* choiceParameter = new ChoiceParameter( name, choices );
    addParameter( choiceParameter );
    return choiceParameter;
  }
  
  inline MCheckBoxParameter* Device::addMCheckBoxParameter ( const std::string name, const Choices& choices )
  {
    MCheckBoxParameter* mCheckBoxParameter = new MCheckBoxParameter( name, choices );
    addParameter( mCheckBoxParameter );
    return mCheckBoxParameter;
  }
  
  inline MatrixParameter* Device::addMatrixParameter ( const std::string name )
  {
    MatrixParameter* matrixParameter = new MatrixParameter( name );
    addParameter( matrixParameter );
    return matrixParameter;
  }
  
  inline CapacitiesParameter* Device::addCapacitiesParameter ( const std::string name, size_t count )
  {
    CapacitiesParameter* capacitiesParameter = new CapacitiesParameter( name, count );
    addParameter( capacitiesParameter );
    return capacitiesParameter;
  }


}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::Device);
