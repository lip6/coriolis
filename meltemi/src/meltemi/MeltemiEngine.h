// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2014-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./meltemi/MeltemiEngine.h"                     |
// +-----------------------------------------------------------------+

#pragma once

#include "etesian/EtesianEngine.h"

namespace Meltemi
{
    using Hurricane::Cell;

    class MeltemiEngine : public Etesian::EtesianEngine
    {
    public:
        typedef Etesian::EtesianEngine Super;

        static MeltemiEngine *create(Cell *);

    private:
        MeltemiEngine(Cell *);

    protected:
        virtual void _coloquinteCallback(coloquinte::PlacementStep step) override;
    };
}