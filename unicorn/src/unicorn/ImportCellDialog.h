// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./unicorn/ImportCellDialog.h"             |
// +-----------------------------------------------------------------+


#ifndef  UNICORN_IMPORT_CELL_DIALOG_H
#define  UNICORN_IMPORT_CELL_DIALOG_H

#include  <QDialog>

class QCheckBox;
class QComboBox;
class QLineEdit;


namespace Unicorn {


  class ImportCellDialog : public QDialog {
      Q_OBJECT;

    public:
                     ImportCellDialog ( QWidget* parent=NULL );
      void           addFormat        ( QString menuName, int index );
      bool           runDialog        ( QString& name, int& format, bool& newViewerRequest );
      const QString  getCellName      () const;
      bool           useNewViewer     () const;
      int            getFormat        () const;
    protected:
      QLineEdit*     _lineEdit;
      QCheckBox*     _viewerCheckBox;
      QComboBox*     _formatComboBox;
  };


} // Unicorn namespace.


#endif // UNICORN_IMPORT_CELL_DIALOG_H
