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
    Name MeltemiEngine::_toolName = "Meltemi";

    const Name &MeltemiEngine::staticGetName()
    {
        return _toolName;
    }

    MeltemiEngine *MeltemiEngine::get(const Cell *cell)
    {
        return static_cast<MeltemiEngine *>(ToolEngine::get(cell, staticGetName()));
    }

    MeltemiEngine::MeltemiEngine(Cell *cell)
        : Super(cell)
    {
        _katana = Katana::KatanaEngine::get(cell);
        if (_katana == NULL) {
            _katana = Katana::KatanaEngine::create(cell);
        }
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
        // Run the callback to read back the results in the circuit
        _coloquinteCallbackCore(step, true);

        // TODO: run the KatanaEngine global routing
        _katana->digitalInit();
        _katana->runGlobalRouter();
        _katana->ripupAll();
    }
}