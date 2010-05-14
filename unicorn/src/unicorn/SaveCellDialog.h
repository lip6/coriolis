
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
// |  C++ Header  :       "./SaveCellDialog.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __UNICORN_SAVE_CELL_DIALOG_H__
#define  __UNICORN_SAVE_CELL_DIALOG_H__

#include  <QDialog>

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
    protected slots:
              void           formatChanged    ( int );

    protected:
              QLineEdit*     _lineEdit;
  };




} // End of Unicorn namespace.


#endif
