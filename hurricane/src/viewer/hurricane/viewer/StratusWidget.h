
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
// |  C++ Header  :       "./StratusWidget.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_STRATUS_WIDGET__
#define  __HURRICANE_STRATUS_WIDGET__

#include  <QDialog>

class QLineEdit;


namespace Hurricane {

  class Cell;


  class StratusWidget : public QDialog {
      Q_OBJECT;
    public:
      static  bool           runScript     ( QWidget* parent );
              const QString  getScriptName () const;
    protected:
                             StratusWidget ( QWidget* parent=NULL );
    protected:
              QLineEdit*     _lineEdit;
  };


} // End of Hurricane namespace.


#endif  // __HURRICANE_STRATUS_WIDGET__

