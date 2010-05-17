
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
// |  C++ Header  :       "./GotoWidget.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_GOTO_WIDGET__
#define  __HURRICANE_GOTO_WIDGET__


#include  <QDialog>
#include  <QRegExp>
class QLineEdit;
class QComboBox;

#include "hurricane/DbU.h"


namespace Hurricane {


  class GotoWidget : public QDialog {
      Q_OBJECT;
    public:
      enum DbuMode { Db=0, Grid=1, Symbolic=2, Micrometer=3, Nanometer=4 };
    public:
                        GotoWidget       ( QWidget* parent=NULL);
      inline bool       hasXy            () const;
      inline bool       hasAperture      () const;
      inline DbU::Unit  getX             () const;
      inline DbU::Unit  getY             () const;
      inline DbU::Unit  getAperture      () const;
             DbU::Unit  toDbu            ( double ) const;
             double     fromDbu          ( DbU::Unit ) const;
    public slots:
             void       acceptAndUpdate  ();
             void       changeDbuMode    ( unsigned int, DbU::UnitPower );
    private:
      QRegExp    _xyRegexp;
      QLineEdit* _xyEdit;
      QLineEdit* _apertureEdit;
      QComboBox* _dbuMode;
      DbU::Unit  _x;
      DbU::Unit  _y;
      DbU::Unit  _aperture;
      bool       _hasXy;
      bool       _hasAperture;
  };


  inline bool       GotoWidget::hasXy       () const { return _hasXy; }
  inline bool       GotoWidget::hasAperture () const { return _hasAperture; }
  inline DbU::Unit  GotoWidget::getX        () const { return _x; }
  inline DbU::Unit  GotoWidget::getY        () const { return _y; }
  inline DbU::Unit  GotoWidget::getAperture () const { return _aperture; }


} // End of Hurricane namespace.


#endif  // __HURRICANE_GOTO_WIDGET__
