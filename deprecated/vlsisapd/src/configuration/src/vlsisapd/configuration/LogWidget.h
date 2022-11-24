
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
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./vlsispad/configuration/LogWidget.h"     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __CONFIGURATION_LOG_WIDGET__
#define  __CONFIGURATION_LOG_WIDGET__


#include  <QDialog>
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


} // End of Configuration namespace.


#endif  // __CONFIGURATION_LOG_WIDGET__
