
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HApplication.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <csignal>
#include  <iostream>

#include  "hurricane/Error.h"
#include  "hurricane/viewer/ExceptionWidget.h"
#include  "hurricane/viewer/HApplication.h"


namespace Hurricane {


  using std::cerr;
  using std::endl;
  using std::exception;


// -------------------------------------------------------------------
// Class  :  "HApplication".


  HApplication::HApplication ( int& argc, char** argv )
    : QApplication ( argc, argv )
  { }


  HApplication::HApplication ( int& argc, char** argv, bool GUIenabled )
    : QApplication ( argc, argv, GUIenabled )
  { }


  HApplication::HApplication ( int& argc, char** argv, Type type )
    : QApplication ( argc, argv, type )
  { }


  HApplication::HApplication ( Display* display, Qt::HANDLE visual, Qt::HANDLE colormap )
    : QApplication ( display, visual, colormap )
  { }


  HApplication::HApplication ( Display* display, int& argc, char** argv, Qt::HANDLE visual, Qt::HANDLE colormap )
    : QApplication ( display, argc, argv, visual, colormap )
  { }


  bool  HApplication::notify ( QObject* object, QEvent* event )
  {
    try {
      return QApplication::notify ( object, event );
    }
    catch ( Error& e ) {
      ExceptionWidget* ew = new ExceptionWidget ();
      ew->setMessage ( e.getReason().c_str() );
      if ( ew->exec() == QDialog::Rejected )
        kill ( getpid(), SIGSEGV );
    }
    catch ( exception& e ) {
      ExceptionWidget* ew = new ExceptionWidget ();
      ew->setMessage ( e.what() );
      if ( ew->exec() == QDialog::Rejected )
        kill ( getpid(), SIGSEGV );
    }
    catch ( ... ) {
      static const char* message =
        "&nbsp;&nbsp;Unmanaged exception, neither a <b>Hurricane::Error</b><br>"
        "nor a <b>std::exception</b>."; 

      ExceptionWidget* ew = new ExceptionWidget ();
      ew->setMessage ( message );
      if ( ew->exec() == QDialog::Rejected )
        kill ( getpid(), SIGSEGV );
    }
    return true;
  }


} // End of Hurricane namespace.
