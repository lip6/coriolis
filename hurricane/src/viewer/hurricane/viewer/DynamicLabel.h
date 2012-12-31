
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/viewer/DynamicLabel.h"        |
// +-----------------------------------------------------------------+


# ifndef  __DYNAMIC_LABEL_H__
#   define  __DYNAMIC_LABEL_H__

# include  <QString>
# include  <QWidget>

class QLabel;

# include  "hurricane/Commons.h"
# include  "hurricane/DbU.h"


namespace Hurricane {


  class DynamicLabel : public QWidget {
      Q_OBJECT;

    public:
      enum Indent { NoIndent=1, HalfIndent=2 };
    public:
    // Constructor.
                            DynamicLabel    ( QWidget* parent=NULL );
    // Methods.                             
              QString       getStaticText   () const;
              QString       getDynamicText  () const;
              void          setDynamicWidth ( int chars, int flags=HalfIndent );
              void          setStaticText   ( const QString& text );
              void          setDynamicText  ( const QString& text );
              void          setDynamicText  ( DbU::Unit u );

    protected:
    // Internal - Attributes.
              QLabel*       _staticLabel;
              QLabel*       _dynamicLabel;

    // Internal - Constructor.
                            DynamicLabel   ( const DynamicLabel& );
              DynamicLabel& operator=      ( const DynamicLabel& );
  };


} // End of Hurricane namespace.


# endif
