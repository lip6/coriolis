
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :               damien.dupuis@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ConfigurationDialog.h"                  |
// +-----------------------------------------------------------------+


#pragma once
#include <QDialog>
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


} // Cfg namespace.
