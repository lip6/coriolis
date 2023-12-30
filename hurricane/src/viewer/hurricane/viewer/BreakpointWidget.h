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
// |  C++ Header  :       "./BreakpointWidget.h"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include <QEventLoop>
#include <QDialog>
class QLabel;
class QSpinBox;


namespace Hurricane {


  class BreakpointWidget : public QDialog {
      Q_OBJECT;
    public:
      static void  readQtSettings   ();
      static void  saveQtSettings   ();
    public:
                   BreakpointWidget ( QWidget* parent=NULL);
             void  setMessage       ( const QString& );
             int   getStopLevel     () const;
             void  setStopLevel     ( int );
             int   execNoModal      ();
    public slots:
             void  updateStopLevel  ( int );
    protected slots:
             void  raiseFinished    ( int );
    private:
      static QByteArray  _geometry;
             QLabel*     _message;
             QSpinBox*   _stopLevel;
             bool        _isFinished;
             QEventLoop* _eventLoop;
  };



} // Hurricane namespace.
