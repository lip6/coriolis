// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ScriptWidget.h"                         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_SCRIPT_WIDGET_H
#define  HURRICANE_SCRIPT_WIDGET_H

#include  <QDialog>

class QLineEdit;


namespace Hurricane {

  class Cell;


  class ScriptWidget : public QDialog {
      Q_OBJECT;
    public:
      static  void           runScript     ( QWidget* parent, QString& scriptName, Cell* );
              const QString  getScriptName () const;
              void           setScriptName ( QString scriptName );
    protected:
                             ScriptWidget  ( QWidget* parent=NULL );
    protected:
              QLineEdit*     _lineEdit;
  };


} // Hurricane namespace.

#endif  // HURRICANE_SCRIPT_WIDGET_H

