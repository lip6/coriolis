/*
 *  NRCCstr.h
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_NRCCSTR_H__
#define __OPENCHAMS_NRCCSTR_H__

#include "vlsisapd/openChams/Equation.h"

namespace OpenChams {
  class NRCCstr : public Equation {
  public:
    NRCCstr(string controlVoltage);

    inline void setVoltage(std::string s);
    inline std::string getVoltage();

    virtual void printEquations();

  private:
    std::string _controlVoltage;
  };

  inline void        NRCCstr::setVoltage(std::string s) {_controlVoltage = s;}
  inline std::string NRCCstr::getVoltage()              {return _controlVoltage;}

} // namespace
#endif
