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
#include "SimpleGroup.h"
#include "Statement.h"
#include "hurricane/Library.h"
#include <filesystem>
#include <map>
#include <string>

namespace Liberty {

  /**
   *  Base class to handle Liberty Library of cells.
   * */
  class Library: public SimpleGroup {
    public:
      Library (std::string filepath);
      ~Library ();

              Group      *getCellGroup  (std::string cell_name              ) const ;
              void        addCellGroup  (std::string cell_name, Group *group)       ;
              void        mapLibertyToDb(Hurricane::DataBase *db            ) const ;
      inline  std::string getFilePath   ()                                    const ;

      bool load(); /// Process the actual loading of library.

    private:
      void _include(std::string filename);
    private:
      std::filesystem::path         _path   ;  /// Library search path (file directory for includes)
      std::map<std::string, Group*> _cells  ;  /// Contains cells (for search only)
      bool                          _parsed ;  /// Wether liberty has already been successfully parsed
  };

  inline std::string Library::getFilePath() const
  {
    return _path.string();
  }

}  // namespace Liberty
