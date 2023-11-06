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
#include "katana/Constants.h"

#include "meltemi/MeltemiEngine.h"

using Anabatic::Edge;
using Anabatic::GCell;
using Hurricane::Error;
using Katana::Flags;

namespace Meltemi {
  Name MeltemiEngine::_toolName = "Meltemi";

  const Name& MeltemiEngine::staticGetName()
  {
    return _toolName;
  }

  MeltemiEngine* MeltemiEngine::get(const Cell* cell)
  {
    return static_cast<MeltemiEngine*>(ToolEngine::get(cell, staticGetName()));
  }

  MeltemiEngine::MeltemiEngine(Cell* cell)
    : Super(cell)
  {
  }

  MeltemiEngine* MeltemiEngine::create(Cell* cell)
  {
    if (! cell)
      throw Error("MeltemiEngine::create(): NULL cell argument.");

    MeltemiEngine* meltemi = new MeltemiEngine(cell);

    meltemi->_postCreate();
    return meltemi;
  }

  void MeltemiEngine::_coloquinteCallback(coloquinte::PlacementStep step)
  {
    // Run the callback to read back the results in the circuit
    if (step != coloquinte::PlacementStep::UpperBound) {
      _coloquinteCallbackCore(step, _needsGraphicUpdate(step));
      return;
    }

    // Restore a previous save of the circuit parameters
    if (! _cellWidth.empty()) {
      _circuit->setCellWidth(_cellWidth);
      _circuit->setCellHeight(_cellHeight);
    }
    // Save the circuit parameters
    _cellWidth  = _circuit->cellWidth();
    _cellHeight = _circuit->cellHeight();


    // Read the placement back
    _coloquinteCallbackCore(step, true);

    // Disable error messages when running Katana
    unsigned mask = mstream::getActiveMask();
    mstream::disable((unsigned)-1);

    // Run the KatanaEngine global routing
    _katana = Katana::KatanaEngine::get(getCell());
    if (_katana == NULL)
    {
      _katana = Katana::KatanaEngine::create(getCell());
    }
    _katana->digitalInit(Flags::PlacementCallback);
    _katana->runGlobalRouter(Flags::PlacementCallback);


    // Use the congestion info to inflate the cells
    auto   congestion       = computeCongestionMap();
    auto   expansion        = _circuit->computeCellExpansion(congestion);
    double averageExpansion = _circuit->expandCellsByFactor(expansion);
    // Appears in the Coloquinte log
    std::cout << "\tBloat " << averageExpansion;

    /*
     * TODO:
     *  * Find benchmarks that are difficult to route
     *  * Evaluate history methods (keeping expansion information between iterations)
     *  * Assess the impact of incorporating these in Coloquinte directly
    */

    // Get rid of the global routing solution and the engine
    _katana->ripupAll();
    _katana->destroy();
    _katana = NULL;

    // Reenable error messages
    mstream::enable(mask);
  }

  std::vector<std::pair<coloquinte::Rectangle, float>> MeltemiEngine::computeCongestionMap()
  {
    if (_katana == NULL) {
      throw Error("Katana engine is not available for congestion map computation");
    }

    std::vector<std::pair<coloquinte::Rectangle, float>> ret;

    for (GCell* cell : _katana->getGCells()) {
      double usage = 0.0;
      for (Edge* edge : cell->getEdges()) {
        unsigned dem = edge->getRealOccupancy();
        unsigned cap = edge->getCapacity();
        usage        = std::max(usage, (double)dem / (double)std::max(cap, 1u));
      }

      auto box   = cell->getBoundingBox();
      auto pitch = getSliceStep();

      coloquinte::Rectangle rect((int)(box.getXMin() / pitch), (int)(box.getXMax() / pitch), (int)(box.getYMin() / pitch), (int)(box.getYMax() / pitch));
      ret.emplace_back(rect, usage);
    }

    return ret;
  }
}  // namespace Meltemi
