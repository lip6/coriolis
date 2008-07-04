
// -*- C++ -*-


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
    // Constructor.
                            DynamicLabel   ( QWidget* parent=NULL );
    // Methods.
              QString       getStaticText  () const;
              QString       getDynamicText () const;
              void          setStaticText  ( const QString& text );
              void          setDynamicText ( const QString& text );
              void          setDynamicText ( DbU::Unit u );

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
