// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./HApplication.cpp"                       |
// +-----------------------------------------------------------------+


#include  <iostream>
#include  <iomanip>
#include  <QSettings>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/viewer/Graphics.h"
#include  "hurricane/viewer/ExceptionWidget.h"
#include  "hurricane/viewer/HApplication.h"


namespace Hurricane {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::setfill;
  using std::exception;


// -------------------------------------------------------------------
// Class  :  "HApplication".


  HApplication::HApplication ( int& argc, char** argv )
    : QApplication ( argc, argv )
  { sharedCtor(); }


  HApplication::HApplication ( int& argc, char** argv, bool GUIenabled )
    : QApplication ( argc, argv, GUIenabled )
  { sharedCtor(); }


// HApplication::HApplication ( int& argc, char** argv, Type type )
//   : QApplication ( argc, argv, type )
// { sharedCtor(); }


#if defined(Q_WS_X11)
  HApplication::HApplication ( Display* display, Qt::HANDLE visual, Qt::HANDLE colormap )
    : QApplication ( display, visual, colormap )
  { sharedCtor(); }


  HApplication::HApplication ( Display* display, int& argc, char** argv, Qt::HANDLE visual, Qt::HANDLE colormap )
    : QApplication ( display, argc, argv, visual, colormap )
  { sharedCtor(); }
#endif


  bool  HApplication::notify ( QObject* object, QEvent* event )
  {
    try {
      return QApplication::notify( object, event );
    }
    catch ( Bug& e ) {
      ExceptionWidget::run( e );
    }
    catch ( Error& e ) {
      ExceptionWidget::run( e );
    }
    catch ( Exception& e ) {
      ExceptionWidget::run( e );
    }
    catch ( exception& e ) {
      ExceptionWidget::run( e );
    }
    catch ( ... ) {
      static const char* message =
        "&nbsp;&nbsp;Unmanaged exception, neither a <b>Hurricane::Error</b><br>"
        "nor a <b>std::exception</b>."; 

      ExceptionWidget::run( message );
    }
    return true;
  }


  void  HApplication::sharedCtor ()
  {
    QCoreApplication::setOrganizationName  ( "SU-LIP6" );
    QCoreApplication::setOrganizationDomain( "lip6.fr" );
    QCoreApplication::setApplicationName   ( "Coriolis" );
  }


} // Hurricane namespace.
