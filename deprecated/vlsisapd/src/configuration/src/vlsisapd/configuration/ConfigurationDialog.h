
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
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
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               damien.dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ConfigurationDialog.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x



#ifndef  __CFG_CONFIGURATION_DIALOG__
#define  __CFG_CONFIGURATION_DIALOG__

#include  <QDialog>
class QPushButton;

namespace Cfg {
class ConfigurationDialog : public QDialog {
    Q_OBJECT;
    public:
      ConfigurationDialog(QWidget* parent=NULL);

    signals:
      void saveToFile();
      void needRestart();

};

} // End of Cfg namespace.

#endif  // __CFG_CONFIGURATION_DIALOG_
