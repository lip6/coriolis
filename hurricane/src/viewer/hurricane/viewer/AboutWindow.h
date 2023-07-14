// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/AboutWindow.h"              |
// +-----------------------------------------------------------------+


#pragma once
#include <exception>
#include <QDialog>
class QLabel;
class QTextEdit;


namespace Hurricane {


  class AboutWindow : public QDialog {
      Q_OBJECT;
    public:
      static  bool  show        ();
    public:
                    AboutWindow ( QWidget* parent=NULL);
  };


} // Hurricane namespace.
