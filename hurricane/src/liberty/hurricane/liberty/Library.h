// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2013-2026, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                              Hippolyte MELICA    |
// |  E-mail      :   hippolyte.melica@etu.sorbonne-universite.fr    |
// | =============================================================== |
// |  C++ Module  :  "./Library.h"                                   |
// +-----------------------------------------------------------------+


#pragma once
#include "Statement.h"
#include "Group.h"
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace Liberty {

  /**
   *  Base class to parse Liberty Library of cells.
   * */
  class Library: public Group {
    public:
      Library (const std::string &filepath);  // TODO: find a way to fiter stuff
      ~Library ();

              Group      *getCellGroup( const std::string &cell_name) const;
      inline  std::string getFilePath ()                              const;

      void load(); /// Process the actual loading of library.

    private:
      void _include(const std::string &filename);
    private:
      std::filesystem::path         _path ;  /// Library search path (file directory for includes)
      std::map<std::string, Group*> _cells;  /// Contains cells (for search only)
  };

  inline std::string Library::getFilePath() const
  {
    return _path.string();
  }

}  // namespace Liberty
