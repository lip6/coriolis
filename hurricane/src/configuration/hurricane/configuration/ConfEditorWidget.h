
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ConfEditorWidget.h"                     |
// +-----------------------------------------------------------------+


#pragma once
#include <fstream>
#include <QMainWindow>
class QMenu;
class QAction;


namespace Cfg {

  class ConfigurationWidget;

  class ConfEditorWidget : public QMainWindow {
      Q_OBJECT;
    public:
                                  ConfEditorWidget       ( QWidget* parent=NULL );
             void                 createActions          ();
             void                 createMenus            ();
      inline ConfigurationWidget* getConfigurationWidget ();
    public slots:
             void                 save                   ();
    private:
      ConfigurationWidget* _configurationWidget;
      QMenu*               _fileMenu;
      QAction*             _saveAction;
      QAction*             _quitAction;
  };


// Inline Functions.
  inline ConfigurationWidget* ConfEditorWidget::getConfigurationWidget ()
  { return _configurationWidget; }


} // Cfg namespace.
