
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
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ScriptWidget.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SCRIPT_WIDGET__
#define  __HURRICANE_SCRIPT_WIDGET__

#include  <QDialog>

class QLineEdit;


namespace Hurricane {

  class Cell;


  class ScriptWidget : public QDialog {
      Q_OBJECT;
    public:
      static  bool           runScript     ( QWidget* parent, Cell* );
              const QString  getScriptName () const;
    protected:
                             ScriptWidget  ( QWidget* parent=NULL );
    protected:
              QLineEdit*     _lineEdit;
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_SCRIPT_WIDGET__

