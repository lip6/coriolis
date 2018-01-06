// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./unicorn/ImportCell.h"                   |
// +-----------------------------------------------------------------+


#ifndef  UNICORN_IMPORT_CELL_H
#define  UNICORN_IMPORT_CELL_H

#include <string>
#include <vector>
#include <functional>
#include "unicorn/ImportCellDialog.h"

namespace Hurricane {
  class Cell;
  class Library;
}

namespace Unicorn {


  class Importer {
    public:
                    Importer ();
      virtual      ~Importer ();
      template< typename T >
      inline  bool  isA      () const;
      template< typename T >
      inline  T     getAs    () const;
      virtual void  load     ( std::string fileName ) = 0;
    protected:
      boost::any  _rvalue;
  };

  template< typename T >
  inline bool  Importer::isA () const { return _rvalue.type() == typeid(T); }

  template< typename T >
  inline T  Importer::getAs () const { return boost::any_cast<T>( _rvalue ); }


  template< typename T >
  class FileImporter : public Importer {
    public:
                       FileImporter ( std::function<T(std::string)>& loader );
      virtual         ~FileImporter ();
      virtual void     load         ( std::string fileName );
    private:
      std::function<T(std::string)>  loader_;
  };

  
  template< typename T >
  FileImporter<T>::FileImporter ( std::function<T(std::string)>& loader )
    : Importer(), loader_(loader)
  { }

  template< typename T >
  FileImporter<T>::~FileImporter () { }

  template< typename T >
  void  FileImporter<T>::load ( std::string fileName ) { _rvalue = loader_( fileName ); }


  class ImportCell {
    public:
                   ImportCell  ();
                  ~ImportCell  ();
      inline void  setDialog   ( ImportCellDialog* );
             bool  load        ( const std::string&, int formatId );
      template< typename T >
             void  addImporter ( std::string, std::function<T(std::string)> );
      inline bool  succeeded   () const;
      template< typename T >
             T     getLoaded   () const;
    private:
      std::vector<Importer*>  _importers;
      ImportCellDialog*       _dialog;
      size_t                  _activeId;
  };


  inline void  ImportCell::setDialog ( ImportCellDialog* dialog ) { _dialog = dialog; }
  inline bool  ImportCell::succeeded () const { return _activeId < _importers.size(); }


  template< typename T >
  void  ImportCell::addImporter ( std::string menuName, std::function<T(std::string)> callback )
  {
    _dialog->addFormat( menuName.c_str(), _importers.size() );
    _importers.push_back( new FileImporter<T>(callback) );
  }


  template< typename T >
  T  ImportCell::getLoaded() const
  { return (succeeded() and _importers[_activeId]->isA<T>()) ? _importers[_activeId]->getAs<T>() : NULL; }


}  // Unicorn namespace.

#endif  // UNICORN_IMPORT_CELL_H
