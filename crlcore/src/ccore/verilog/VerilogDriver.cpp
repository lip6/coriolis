
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Verilog / Hurricane  Interface                         |
// |                                                                 |
// |  Authors     :          Jean-Paul CHAPUT, Serge Rabyking        |
// |  E-mail      :          Jean-Paul.Chaput@lip6.fr                |
// | =============================================================== |
// |  C++ Module  :   "./verilog/VerilogDriver.cpp"                  |
// +-----------------------------------------------------------------+


#include <ctime>
#include <cmath>
#include <cstdio>
#include <cfenv>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
using namespace std;

#include "hurricane/configuration/Configuration.h"
#include "hurricane/Warning.h"
#include "hurricane/DataBase.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Diagonal.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/Polygon.h"
#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/Cell.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/NetExtension.h"
#include "crlcore/ToolBox.h"
#include "crlcore/Verilog.h"
//#include "crlcore/VerilogBit.h"
//#include "crlcore/VerilogEntity.h"

using namespace CRL;

namespace CRL {

  static void _write_hdr(ofstream &out)
  {
    time_t  clock    = time( nullptr );
    tm      tm       = *localtime( &clock );
    char    stamp[1024];
    strftime( stamp, 1024, "%b %d, %Y, %H:%M", &tm );

    out << "/* Coriolis Verilog Driver */" << std::endl;
    out << "/* Generated on " << stamp << " */" << std::endl;
  }

  static void _populate_non_terminal_cells(Cell* cell, std::set<Cell*> &cells)
  {
    if(!cells.insert(cell).second)
    {
      // insertion not happend because it was there already
      return;
    }
    for(Occurrence occurrence: cell->getNonTerminalNetlistInstanceOccurrences())
    {
      Cell* mod = static_cast<Instance*>( occurrence.getEntity() )->getMasterCell();
      // recursively find all related Non-terminal cells (part of the user's design hierarchy)
      _populate_non_terminal_cells(mod, cells);
    }
  }

  std::pair<std::string, int> _wire2bus(std::string name)
  {
    std::pair<std::string, int> bus(name, -1);
    int i = name.size() - 1;
    while (std::isspace(name[i])) --i;// remove trailing spaces just in case
    if(name[i] == ')') // name ends with bracket so is in the form "some_wire_name(index)"
    {
      for(int j = i;;)
      {
        if(--j < 0)
        {
          // didn't find starting bracket
          // this should never happen
          std::cerr << "Wrongly formatted net name: " << name << std::endl;
          assert(false);
        }
        if(name[j] == '(') // find starting bracket
        {
          bus.second = std::stoi(name.substr(j+1, i-j-1)); // get index
          while (std::isspace(name[j-1])) --j; // ignore spaces before starting bracket
          bus.first.erase(j); // trim name to not include index part with brackets
          break;
        }
      }
    }
    return bus;
  }

  static bool _cellHasNetPlug(Cell* cell, Net* net)
  {
    for(Instance* instance: cell->getInstances()) // go through all cells instances that form our cell
    {
      for(Plug* plug: instance->getPlugs()) // plugs are connect points of the cells
      {
        if (plug->getNet() == net)
        {
          return true;
        }
      }
    }
    return false;
  }

  static void _write_cell(ofstream &out, Cell* cell)
  {
    out << std::endl;
    std::set<Net*> nets;
    std::vector<Net*> ports, wires;
    for(Instance* instance: cell->getInstances()) // go through all cells instances that form our cell
    {
      for(Plug* plug: instance->getPlugs()) // plugs are connect points of the cells
      {
        Net* net = plug->getMasterNet();
        if(net->isPower() || net->isGround()) // VDD and VSS are not part of Verilog netlist
        {
          continue;
        }
        net = plug->getNet();
        if(!net)
        {
          continue; // unconnected plug
        }
        if(net->isExternal()) // propogates through ports
        {
          ports.push_back(net);
          nets.insert(net); // collect all nets we use
        }
        else
        {
          if(!nets.insert(net).second) // the insertion not happend as it was already there
          {
            // this net was already enumerated in some other cell
            // so this wire used at least between two cells and need to be declared
            wires.push_back(net);
          }
        }
      }
    }
    std::map<Net*, std::pair<std::string, int> > net2bus;
    for(Net *net: nets)
    {
      net2bus.insert(std::make_pair(net, _wire2bus(getString(net->getName()))));
    }
    std::vector<std::string> names;
    // collect all port names without indexes in ordered way
    for(Net *net: ports)
    {
      std::string &name = net2bus[net].first;
      auto it = std::lower_bound(names.begin(), names.end(), name);
      if(it == names.end() || (*it) != name) // name not in the list yet
      {
        names.insert(it, name);
      }
    }
    // declare Verilog module with ports names
    out << "module " << getString(cell->getName()) << "(";
    for ( auto it = names.begin(); it != names.end(); ++it)
    {
      out << (*it);
      if (it + 1 != names.end())
      {
        out << ", ";
      }
    }
    out << ");\n";
    // now individually declare each port with direction and width
    for (auto &name: names)
    {
      int idx_min = -1, idx_max;
      Net::Direction dir;
      for (auto it: net2bus)
      {
        if (it.second.first == name) // find net with name
        {
          int idx = it.second.second;
          if (idx < 0)
          {
            if (idx_min >= 0)
            {
              std::cerr << "Net name \"" << name << "\" used with index " << idx_min
                        << " and without index" << std::endl;
              assert(false);
            }
            dir = it.first->getDirection();
            break;
          }
          if (idx_min < 0)
          {
            idx_min = idx_max = idx;
            dir = it.first->getDirection();
          }
          else
          {
            if (idx < idx_min)
            {
              idx_min = idx;
            }
            if (idx > idx_max)
            {
              idx_max = idx;
            }
            if (dir != it.first->getDirection())
            {
              int other = (idx == idx_min)?idx_max:idx_min;
              std::cerr << "Net \"" << name << "\" with index " << idx << " and " << other
                        << " has different directions" << std::endl;
              assert(false);
            }
          }
        }
      }
      if (dir == Net::Direction::IN)
      {
        out << "  input ";
      }
      else if (dir == Net::Direction::INOUT)
      {
        out << "  inout ";
      }
      else if (dir == Net::Direction::OUT)
      {
        out << "  output ";
      }
      else
      {
        std::cerr << "Undetermined direction " << dir << " for the net \"" << name << "\"" << std::endl;
        assert(false);
      }
      if (idx_min >= 0)
      {
        out << "[" << idx_max << ":" << idx_min << "] ";
      }
      out << name << ";" << std::endl;
    }
    // collect all wires (and not ports) names without indexes in ordered way
    names.clear();
    for(Net *net: wires)
    {
      std::string &name = net2bus[net].first;
      auto it = std::lower_bound(names.begin(), names.end(), name);
      if(it == names.end() || (*it) != name) // name not in the list yet
      {
        names.insert(it, name);
      }
    }
    // now individually declare each wire with width
    for (auto &name: names)
    {
      int idx_min = -1, idx_max = -1;
      for (auto it: net2bus)
      {
        if (it.second.first == name) // find all nets by name
        {
          int idx = it.second.second;
          if (idx < 0) // without index
          {
            if (idx_min >= 0)
            {
              std::cerr << "Net name \"" << name << "\" used with index " << idx_min
                        << " and without index" << std::endl;
              assert(false);
            }
            break;
          }
          if (idx_min < 0) // first time encountered
          {
            idx_min = idx_max = idx;
          }
          else
          {
            if (idx < idx_min)
            {
              idx_min = idx;
            }
            if (idx > idx_max)
            {
              idx_max = idx;
            }
          }
        }
      }
      out << "  wire ";
      if (idx_min >= 0)
      {
        out << "[" << idx_max << ":" << idx_min << "] ";
      }
      out << name << ";" << std::endl;
    }
    // free not needed resources
    names.clear();
    ports.clear();
    wires.clear();
    nets.clear();
    // declare instancies with connections
    for(Instance* instance: cell->getInstances()) // go through all cells instances that form our cell
    {
      std::vector<Plug*> conns;
      for(Plug* plug: instance->getPlugs()) // plugs are connect points of the cells
      {
        Net* net = plug->getMasterNet();
        if(net->isPower() || net->isGround()) // VDD and VSS are not part of Verilog netlist
        {
          continue;
        }
        if (!plug->getNet())
        {
          continue; // unconnected plug
        }
        if (!instance->isTerminalNetlist()&&
            !_cellHasNetPlug(instance->getMasterCell(), net))
        {
          // the plug is redundant and actually has no connection inside cell
          continue;
        }
        // insert in sorted order
        auto it = std::lower_bound(conns.begin(), conns.end(), plug,
                     [](Plug* lhs, Plug* rhs) -> bool
                     { return getString(lhs->getMasterNet()->getName()) <
                              getString(rhs->getMasterNet()->getName()); });
        conns.insert(it, plug);
      }
      if (conns.empty()) // instance has no connections apart from VSS&VDD
      {
        continue;
      }
      // declare instance
      out << "  " << getString(instance->getMasterCell()->getName()) << " "
                  << getString(instance->getName()) << " (" << std::endl;
      // declare connections
      for (auto it = conns.begin(); it != conns.end(); ++it)
      {
        auto &info = net2bus[(*it)->getNet()];
        out << "    ." << getString((*it)->getMasterNet()->getName()) << "(" << info.first;
        if (info.second >= 0) // indexed wire
        {
          out << "[" << info.second << "]";
        }
        out << ")";
        if ( it+1 != conns.end()) // not last, so need comma
        {
          out << ",";
        }
        out << std::endl;
      }
      out << "  );" << std::endl;
    }
    out << "endmodule" << std::endl;
  }

// -------------------------------------------------------------------
// Class  :  "CRL::Verilog".

  bool  Verilog::save ( Cell* cell, uint64_t flags )
  {
    ofstream out(getString(cell->getName()) + ".v");
    _write_hdr(out);
    if (flags & TopCell)
    {
      _write_cell(out, cell);
    }
    else
    {
      std::set<Cell*> cells;
      _populate_non_terminal_cells(cell, cells);
      for (auto &it: cells)
      {
        _write_cell(out, it);
      }
    }
    out.close();
    return true;
  }

}  // CRL namespace.
