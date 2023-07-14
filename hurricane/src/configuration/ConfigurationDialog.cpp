
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
// |  C++ Module  :       "./ConfigurationDialog.cpp"                |
// +-----------------------------------------------------------------+


#include <QVBoxLayout>
#include <QPushButton>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/configuration/ConfigurationWidget.h"
#include "hurricane/configuration/ConfigurationDialog.h"


namespace Cfg {


  ConfigurationDialog::ConfigurationDialog ( QWidget* parent )
    : QDialog(parent)
  {
    setModal(true);
    setWindowTitle("Configure window");

    ConfigurationWidget* confWidget = Configuration::get()->buildWidget(ConfigurationWidget::StandAlone);
    connect(confWidget                   , SIGNAL(confOk())     , this, SLOT(close()));
    connect(confWidget->getCancelButton(), SIGNAL(clicked())    , this, SLOT(close()));
    connect(confWidget->getSaveButton()  , SIGNAL(clicked())    , this, SIGNAL(saveToFile()));
    connect(confWidget                   , SIGNAL(needRestart()), this, SIGNAL(needRestart()));

    QVBoxLayout* vLayout = new QVBoxLayout ();
    vLayout->addWidget(confWidget);

    setLayout ( vLayout );
  }


}  // End of Cfg namespace.
