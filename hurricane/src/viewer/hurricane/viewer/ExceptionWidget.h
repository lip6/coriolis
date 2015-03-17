// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ExceptionWidget.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_EXCEPTION_WIDGET_H
#define  HURRICANE_EXCEPTION_WIDGET_H

#include  <exception>
#include  <functional>
#include  <QDialog>
class QLabel;
class QTextEdit;


namespace Hurricane {

  class Error;
  class Exception;


  class ExceptionWidget : public QDialog {
      Q_OBJECT;
    public:
      static  void    run             ( Error& );
      static  void    run             ( Exception& );
      static  void    run             ( std::exception& );
      static  void    run             ( const QString&, const QString& where="" );
      static  void    catchAllWrapper ( std::function< void() > method );
    public:
                      ExceptionWidget ( QWidget* parent=NULL);
              void    setMessage      ( const QString& );
              void    setTrace        ( const QString& );
    protected:
      virtual void    closeEvent      ( QCloseEvent* );
    private slots:
              void    _showTrace      ( int state );
    private:
              QLabel*      _header;
              QLabel*      _message;
              QTextEdit*   _trace;
  };


} // Hurricane namespace.

#endif  // HURRICANE_EXCEPTION_WIDGET_H
