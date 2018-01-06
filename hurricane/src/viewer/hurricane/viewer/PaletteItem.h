// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/PaletteItem.h"         |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_PALETTE_ITEM_H
#define  HURRICANE_PALETTE_ITEM_H

#include  <QPixmap>
#include  <QWidget>
#include  <QFont>
class QPaintEvent;
class QCheckBox;

#include  "hurricane/Commons.h"


namespace Hurricane {

  class Name;
  class PaletteItem;


// -------------------------------------------------------------------
// Class :  "Hurricane::DrawingStyleSample".


  class DrawingStyleSample : public QWidget {
      Q_OBJECT;

    public:
                          DrawingStyleSample ( PaletteItem* );
                         ~DrawingStyleSample ();
    protected:
             QPixmap*     _sample;
             PaletteItem* _entry;
    private:
      static int          _side;
      static int          _border;
    public slots:
             void         redraw             ();
             void         paintEvent         ( QPaintEvent* );
  };


// -------------------------------------------------------------------
// Class :  "Hurricane::PaletteItem".


  class PaletteItem : public QWidget {
      Q_OBJECT;

    public:
      virtual const Name&    getName           () const = 0;
      virtual bool           isItemVisible     () const = 0;
      virtual bool           isItemSelectable  () const;
      virtual void           setItemVisible    ( bool ) = 0;
      virtual void           setItemSelectable ( bool );
    public slots:
      virtual void           changeStyle       ();
    signals:
              void           visibleToggled    ();
              void           selectableToggled ();
    protected:
                             PaletteItem       ();
                             PaletteItem       ( const PaletteItem& );
              PaletteItem&   operator=         ( const PaletteItem& );
  };


} // Hurricane namespace.

#endif  // HURRICANE_PALETTE_ITEM_H
