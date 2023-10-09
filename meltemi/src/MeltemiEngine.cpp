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
// |  C++ Header  :  "./MeltemiEngine.cpp"                           |
// +-----------------------------------------------------------------+

#include "hurricane/Error.h"

#include "meltemi/MeltemiEngine.h"

using Hurricane::Error;

namespace Meltemi
{

    MeltemiEngine::MeltemiEngine(Cell *cell)
        : Super(cell)
    {
    }

    MeltemiEngine *MeltemiEngine::create(Cell *cell)
    {
        if (!cell)
            throw Error("MeltemiEngine::create(): NULL cell argument.");

        MeltemiEngine *meltemi = new MeltemiEngine(cell);

        meltemi->_postCreate();
        return meltemi;
    }

    void MeltemiEngine::_coloquinteCallback(coloquinte::PlacementStep step)
    {
        std::cout << "Calling Meltemi version" << std::endl;
        EtesianEngine::_coloquinteCallback(step);
    }
}