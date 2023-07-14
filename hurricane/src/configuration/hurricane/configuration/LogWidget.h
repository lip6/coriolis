
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2008-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/configuration/LogWidget.h"    |
// +-----------------------------------------------------------------+


#pragma  once
#include <QDialog>
class QLabel;
class QPushButton;


namespace Cfg {


  class LogWidget : public QDialog {
      Q_OBJECT;
    public:
                      LogWidget  ( QWidget* parent=NULL);
              void    updateLogs ( unsigned int mask );
    private:
      virtual void    closeEvent ( QCloseEvent* );
    private:
              QLabel*      _message;
              QPushButton* _contButton;
              QPushButton* _cancelButton;
  };


} // Configuration namespace.
