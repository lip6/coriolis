/*
 *  DDP.h
 *  openChams
 *
 *  Created by Farakh JAVID on 25/10/2011.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_DDP_H__
#define __OPENCHAMS_DDP_H__

#include "vlsisapd/openChams/Equation.h"

namespace OpenChams {
  class DDP : public Equation {
  public:
    DDP();

    virtual void printEquations();
  };

} // namespace
#endif
