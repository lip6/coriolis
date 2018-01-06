// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./unicorn/SaveCellDialog.h"               |
// +-----------------------------------------------------------------+


#ifndef  UNICORN_SAVE_CELL_DIALOG_H
#define  UNICORN_SAVE_CELL_DIALOG_H

#include <QDialog>

class QCheckBox;
class QLineEdit;


namespace Unicorn {


  class SaveCellDialog : public QDialog {
      Q_OBJECT;

    public:
      static  bool           runDialog        ( QWidget* parent, QString& name );
              const QString  getCellName      () const;
    public slots:
              void           setCellName      ( const QString& );
    protected:
                             SaveCellDialog   ( QWidget* parent=NULL );
    protected:
              QLineEdit*     _lineEdit;
  };


} // Unicorn namespace.

#endif  // UNICORN_SAVE_CELL_DIALOG_H
