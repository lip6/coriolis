
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ExportCellDialog.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __UNICORN_EXPORT_CELL_DIALOG_H__
#define  __UNICORN_EXPORT_CELL_DIALOG_H__

#include  <QDialog>

class QCheckBox;
class QComboBox;
class QLineEdit;


namespace Unicorn {


  class ExportCellDialog : public QDialog {
      Q_OBJECT;

    public:
      enum Formats { AllianceDef=1 };
    public:
                     ExportCellDialog ( QWidget* parent=NULL );
      bool           runDialog        ( QString& name, int& format );
      const QString  getCellName      () const;
      int            getFormat        () const;
      void           setCellName      ( const QString& );
    protected:
      QLineEdit*     _lineEdit;
      QComboBox*     _formatComboBox;
  };


} // End of Unicorn namespace.


#endif
