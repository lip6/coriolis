
// -*- C++ -*-


# ifndef  __HPALETTE__
#   define  __HPALETTE__

# include  <vector>
# include  <QWidget>
# include  <QScrollArea>
# include  <QPixmap>

# include  "hurricane/Commons.h"


  class QCheckBox;
  class QPushButton;


namespace Hurricane {


  using namespace std;


  class Name;
  class BasicLayer;
  class HPaletteEntry;
  class CellWidget;


  class HPalette : public QScrollArea {
      Q_OBJECT;

    public:
    // Constructor.
                                     HPalette         ( QWidget* parent=NULL );
    // Methods.
             bool                    isDrawable       ( size_t index );
      inline CellWidget*             getCellWidget    ();
      inline vector<HPaletteEntry*>& getEntries       ();
             void                    redrawCellWidget ();
             HPaletteEntry*          find             ( const Name& name );
    signals:
    // Signals.
             void                    paletteChanged   ();
    public slots:
    // Slots.
             void                    showAll          ();
             void                    hideAll          ();

    protected:
    // Internal - Attributes.
             vector<HPaletteEntry*>  _entries;
             QPushButton*            _showAll;
             QPushButton*            _hideAll;
    // Internal - Constructors.
                                      HPalette        ( const HPalette& );
             HPalette&                operator=       ( const HPalette& );

  };


  // Inline Functions.
  inline vector<HPaletteEntry*>& HPalette::getEntries () { return _entries; }


} // End of Hurricane namespace.


# endif
