
// -*- C++ -*-


# ifndef  __PALETTE__
#   define  __PALETTE__

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
  class PaletteEntry;
  class CellWidget;


  class Palette : public QScrollArea {
      Q_OBJECT;

    // Constructor.
    public:
                                    Palette          ( CellWidget* cellWidget );
    // Methods.
             bool                   isDrawable       ( size_t index );
      inline CellWidget*            getCellWidget    ();
      inline vector<PaletteEntry*>& getEntries       ();
             void                   redrawCellWidget ();
             PaletteEntry*          find             ( const Name& name );
    // Slots.
    public slots:
             void                   showAll          ();
             void                   hideAll          ();

    // Internal - Attributes.
    protected:
             CellWidget*            _cellWidget;
             vector<PaletteEntry*>  _entries;
             QPushButton*           _showAll;
             QPushButton*           _hideAll;
    // Internal - Constructors.
                                    Palette          ( const Palette& );
             Palette&               operator=        ( const Palette& );

  };


  // Inline Functions.
  inline CellWidget*            Palette::getCellWidget () { return _cellWidget; }
  inline vector<PaletteEntry*>& Palette::getEntries    () { return _entries; }


} // End of Hurricane namespace.


# endif
