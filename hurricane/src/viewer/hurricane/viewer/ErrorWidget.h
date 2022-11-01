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
// |  C++ Header  :  "./hurricane/viewer/ErrorWidget.h"              |
// +-----------------------------------------------------------------+


#pragma once
#include <exception>
#include <functional>
#include <QDialog>
class QLabel;
class QTextEdit;


namespace Hurricane {


  class ErrorWidget : public QDialog {
      Q_OBJECT;
    public:
      static  bool    run             ( const QString& message, const QString& trace="" );
    public:
                      ErrorWidget     ( QWidget* parent=NULL);
              void    setMessage      ( const QString& );
              void    setTrace        ( const QString& );
    protected:
      virtual void    closeEvent      ( QCloseEvent* );
    private slots:
              void    _showTrace      ( int state );
    private:
              QLabel*    _header;
              QTextEdit* _message;
              QTextEdit* _trace;
  };


} // Hurricane namespace.
