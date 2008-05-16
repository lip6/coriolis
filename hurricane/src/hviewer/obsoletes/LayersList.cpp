
// -*- C++ -*-


# include  <vector>

# include  <QBitmap>
# include  <QPainter>
# include  <QHBoxLayout>
# include  <QVBoxLayout>
# include  <QCheckBox>
# include  <QPushButton>

# include  "Name.h"

# include  "ScreenLayer.h"
# include  "LayersList.h"
# include  "CellWidget.h"


namespace Hurricane {


ScreenLayerEntry::ScreenLayerEntry ( ScreenLayer* layer, LayersList* layersList )
  : QWidget()
  , _layersList(layersList)
{
  QHBoxLayout* layout = new QHBoxLayout ();
  layout->setContentsMargins ( 0, 0, 0, 0 );

  _sample = layer->getSample();
  layout->addWidget ( _sample );

  _checkBox = new QCheckBox ( this );
  _checkBox->setText    ( getString(layer->getName()).c_str() );
  _checkBox->setChecked ( true );
  layout->addWidget ( _checkBox );

  setLayout ( layout );
}


void  ScreenLayerEntry::setChecked ( bool state )
{
  _checkBox->setChecked ( state );
  _sample  ->setVisible ( state );
}


bool  ScreenLayerEntry::isChecked () const
{
  return _checkBox->isChecked ();
}


void  ScreenLayerEntry::toggle ()
{
  _sample->setVisible ( _checkBox->isChecked() );
  _layersList->getCellWidget()->redraw();
}



LayersList::LayersList ( CellWidget* cellWidget ) : QScrollArea()
                                                  , _cellWidget(cellWidget)
                                                  , _entries()
                                                  , _showAll(NULL)
                                                  , _hideAll(NULL)
{
  QWidget*     adaptator = new QWidget ();
  QVBoxLayout* layout    = new QVBoxLayout ();

  vector<ScreenLayer*>& screenLayers = _cellWidget->getScreenLayers ();

  for ( size_t i=0 ; i < screenLayers.size() ; i++ ) {
    _entries.push_back ( new ScreenLayerEntry(screenLayers[i],this) );
    layout->addWidget ( _entries.back() );
    connect ( _entries.back()->getCheckBox(), SIGNAL(clicked()), _entries.back(), SLOT(toggle()) );
  }
  layout->addStretch ();

  _showAll = new QPushButton ( this );
  _showAll->setIcon ( QIcon(":/images/palette_show_all.png") );
  _showAll->setFlat ( true );

  _hideAll = new QPushButton ( this );
  _hideAll->setIcon ( QIcon(":/images/palette_hide_all.png") );
  _hideAll->setFlat ( true );

  connect ( _showAll, SIGNAL(clicked()), this, SLOT(showAll()) );
  connect ( _hideAll, SIGNAL(clicked()), this, SLOT(hideAll()) );

  QHBoxLayout* bottomEntry = new QHBoxLayout ();
  bottomEntry->setContentsMargins ( 0, 0, 0, 0 );
  bottomEntry->addWidget ( _showAll );
  bottomEntry->addWidget ( _hideAll );
  layout->addLayout ( bottomEntry );

  adaptator->setLayout ( layout );
  setWidget ( adaptator );
  setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
  setFrameStyle                ( QFrame::Plain );
}


bool  LayersList::isVisible ( size_t index )
{
  if ( index < _entries.size() )
    return _entries[index]->isChecked ();

  return false;
}


void  LayersList::showAll ()
{
  for ( size_t i=0 ; i<_entries.size() ; i++ )
    _entries[i]->setChecked ( true );

  _cellWidget->redraw ();
}


void  LayersList::hideAll ()
{
  for ( size_t i=0 ; i<_entries.size() ; i++ )
    _entries[i]->setChecked ( false );

  _cellWidget->redraw ();
}


} // End of Hurricane namespace.
