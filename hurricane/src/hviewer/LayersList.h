
// -*- C++ -*-


# ifndef  __LAYERSLIST__
#   define  __LAYERSLIST__

# include  <vector>
# include  <QWidget>
# include  <QScrollArea>
# include  <QPixmap>

# include  "Commons.h"


  class QCheckBox;
  class QPushButton;


namespace Hurricane {


  using namespace std;


  class LayerSample;
  class ScreenLayer;
  class LayersList;
  class CellWidget;


  class ScreenLayerEntry : public QWidget {
      Q_OBJECT;

    protected:
      LayersList*  _layersList;
      LayerSample* _sample;
      QCheckBox*   _checkBox;

    public:
                 ScreenLayerEntry ( ScreenLayer* layer, LayersList* layerList );

    public:
      bool       isChecked        () const;
      void       setChecked       ( bool state );
      QCheckBox* getCheckBox      () { return _checkBox; };

    public slots:
      void       toggle           ();
  };


  class LayersList : public QScrollArea {
      Q_OBJECT;

    protected:
      CellWidget*                _cellWidget;
      vector<ScreenLayerEntry*>  _entries;
      QPushButton*               _showAll;
      QPushButton*               _hideAll;

    public:
                                 LayersList    ( CellWidget* cellWidget );

    public:
      CellWidget*                getCellWidget () { return _cellWidget; };
      vector<ScreenLayerEntry*>& getEntries    () { return _entries; };
      bool                       isVisible     ( size_t index );

    public slots:
      void                       showAll       ();
      void                       hideAll       ();
  };


} // End of Hurricane namespace.


# endif
