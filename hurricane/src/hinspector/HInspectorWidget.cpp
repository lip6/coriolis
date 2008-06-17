

#include <QFontMetrics>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QKeyEvent>
#include <QGroupBox>
#include <QVBoxLayout>

#include "hurricane/viewer/Graphics.h"
#include "hurricane/inspector/RecordModel.h"
#include "hurricane/inspector/HInspectorWidget.h"
#include "hurricane/Slot.h"


HInspectorWidget::HInspectorWidget ( Record* rootRecord, QWidget* parent )
    : QWidget(parent)
    , _slotsHistory()
    , _recordModel(NULL)
    , _sortModel(NULL)
    , _recordsHistoryComboBox(NULL)
    , _slotsView(NULL)
    , _rowHeight(20)
    , _rootRecord(rootRecord)
{
  setAttribute   ( Qt::WA_DeleteOnClose );

  _rowHeight = QFontMetrics(Graphics::getFixedFont()).height() + 4;

//recordsHistoryComboBox = new QComboBox(this);
    
  _slotsView = new QTableView(this);
  _slotsView->setShowGrid(false);
  _slotsView->setAlternatingRowColors(true);
  _slotsView->setSelectionBehavior(QAbstractItemView::SelectRows);
  _slotsView->setSortingEnabled(true);

  QHeaderView* horizontalHeader = _slotsView->horizontalHeader ();
  horizontalHeader->setStretchLastSection ( true );
  horizontalHeader->setMinimumSectionSize ( 200 );

  QHeaderView* verticalHeader = _slotsView->verticalHeader ();
  verticalHeader->setVisible ( false );

  Slot* rootSlot = getSlot ( "TopLevelSlot", _rootRecord );
  _slotsHistory.push_back ( rootSlot );

  _recordModel = new RecordModel ( rootSlot->getClone(), this );
  _sortModel   = new QSortFilterProxyModel ( this );
  _sortModel->setSourceModel       ( _recordModel );
  _sortModel->setDynamicSortFilter ( true );
  _sortModel->setFilterKeyColumn   ( 1 );

  _slotsView->setModel ( _sortModel );
  _slotsView->horizontalHeader()->setStretchLastSection ( true );
  _slotsView->resizeColumnToContents ( 0 );

  int rows = _sortModel->rowCount ();
  for ( rows-- ; rows >= 0 ; rows-- )
    _slotsView->setRowHeight ( rows, _rowHeight );

  QGridLayout* inspectorLayout = new QGridLayout();
//inspectorLayout->addWidget(recordsHistoryComboBox, 0, 0, 1, 2);
  inspectorLayout->addWidget(_slotsView, 1, 0, 1, 2);

  _filterPatternLineEdit = new QLineEdit(this);
  QLabel* filterPatternLabel = new QLabel(tr("&Filter pattern:"), this);
  filterPatternLabel->setBuddy(_filterPatternLineEdit);

  inspectorLayout->addWidget(filterPatternLabel, 2, 0);
  inspectorLayout->addWidget(_filterPatternLineEdit, 2, 1);

//QGroupBox* inspectorGroupBox = new QGroupBox(tr("Hurricane inspector"), this);
//inspectorGroupBox->setLayout(inspectorLayout);

//QVBoxLayout* mainLayout = new QVBoxLayout;
//mainLayout->addWidget(inspectorGroupBox);
//setLayout(mainLayout);
  setLayout(inspectorLayout);

//connect(recordsHistoryComboBox, SIGNAL(currentIndexChanged(int)),
//        this, SLOT(recordChanged(int)));
  
  connect ( _filterPatternLineEdit
          , SIGNAL(textChanged(const QString &))
          , this
          , SLOT(textFilterChanged())
          );

  setWindowTitle(tr("Inspector"));
  resize(500, 300);
}


HInspectorWidget::~HInspectorWidget ()
{
//cerr << "HInspector::~HInspector() - " << hex << (void*)this << dec << endl;
  clearHistory ();

//cerr << "Records: " << Record::getAllocateds()  << endl;
//cerr << "Slots:   " << Slot::getAllocateds()  << endl;
}


void  HInspectorWidget::setRootRecord ( Record* record )
{
//recordsHistoryComboBox->clear();
  clearHistory ();

  _rootRecord = record;
  Slot* rootSlot = getSlot("TopLevelRecord",record);
  
  _slotsHistory.push_back ( rootSlot );
  _recordModel->setSlot ( rootSlot->getClone(), _slotsHistory.size() );
}


void  HInspectorWidget::clearHistory ()
{
  if ( !_slotsHistory.empty() ) {
    if ( _slotsHistory.size() > 1 )
      delete _slotsHistory[0]->getDataRecord();

    for ( size_t i=0 ; i < _slotsHistory.size() ; i++ )
      delete _slotsHistory[i];

    _slotsHistory.clear ();
  }
}


bool  HInspectorWidget::setSlot ( Slot* slot )
{
  if ( !slot ) return false;

  bool change = true;
//cerr << "HInspector::setSlot() - " << hex << (void*)slot << dec << endl;

  change = _recordModel->setSlot ( slot, _slotsHistory.size() );

  int rows = _sortModel->rowCount ();
  for ( rows-- ; rows >= 0 ; rows-- )
    _slotsView->setRowHeight ( rows, _rowHeight );

//cerr << "Records: " << Record::getAllocateds()  << endl;
//cerr << "Slots:   " << Slot::getAllocateds()  << endl;

  return change;
}


void  HInspectorWidget::pushSlot ( Slot* slot )
{
//cerr << "pushSlot() - [clone of] " << hex << (void*)slot << dec << endl;
  _slotsHistory.push_back ( slot->getClone() );

  if ( !setSlot(slot->getClone()) ) {
  //cerr << "pushSlot() cancelled" << endl;
    delete _slotsHistory.back ();
    _slotsHistory.pop_back ();
  }
}


void  HInspectorWidget::popSlot ()
{
  if ( _slotsHistory.size() > 1 ) {
  //cerr << "popSlot() - " << hex << (void*)_slotsHistory.back() << dec << endl;
  //cerr << "popSlot() - " << _slotsHistory.back()->getDataString() << endl;

    delete _slotsHistory.back ();
    _slotsHistory.pop_back ();

    setSlot ( _slotsHistory.back()->getClone() );
  }
}


void  HInspectorWidget::keyPressEvent(QKeyEvent *event)
{
  if ( event->matches(QKeySequence::MoveToNextChar) ) {
    QModelIndex index = _slotsView->currentIndex();
    if ( index.isValid() ) {
      Slot* slot = _recordModel->getRecord()->getSlot(_sortModel->mapToSource(index).row());

      if ( slot )
        pushSlot ( slot );
    }
  } else if ( event->matches(QKeySequence::MoveToPreviousChar) ) {
    popSlot ();
  } else {
    event->ignore();
  }
}


void  HInspectorWidget::textFilterChanged ()
{
  _sortModel->setFilterRegExp ( _filterPatternLineEdit->text() );
}
