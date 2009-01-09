
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
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./BreakpointWidget.h"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_BREAKPOINT_WIDGET__
#define  __HURRICANE_BREAKPOINT_WIDGET__


#include  <QDialog>
class QLabel;
class QSpinBox;


namespace Hurricane {


  class BreakpointWidget : public QDialog {
      Q_OBJECT;

    public:
            BreakpointWidget ( QWidget* parent=NULL);
      void  setMessage       ( const QString& );
      int   getStopLevel     () const;
      void  setStopLevel     ( int );
    public slots:
      void  updateStopLevel  ( int );

    private:
      QLabel*   _message;
      QSpinBox* _stopLevel;
  };



} // End of Hurricane namespace.


#endif  // __HURRICANE_BREAKPOINT_WIDGET__
