

#include "hurricane/Collection.h"
#include "hurricane/Occurrence.h"
#include "hurricane/Cell.h"
#include "hurricane/Library.h"


namespace CRL {

  using namespace Hurricane;


// ****************************************************************************************************
// AllUniqueCellOccurrencesFrom declaration
// ****************************************************************************************************

class AllUniqueCellOccurrencesFrom : public Collection<Occurrence> {
// *********************************************

// Types
// *****

	public: typedef Collection<Occurrence> Inherit;

	public: class Locator : public Hurricane::Locator<Occurrence> {
	// *******************************************************

		public: typedef Hurricane::Locator<Occurrence> Inherit;

        private: const Cell*     _cell;
        private: LibraryLocator  _libraryLocator;
        private: CellLocator     _cellLocator;
        private: OccurrenceLocator _allUniqueCellOccurrenceFrom;
        private: Occurrence      _cellOccurrence;

		public: Locator();
		public: Locator(const Cell* cell,Library* library=NULL);
		public: Locator(const Locator& locator);

		public: Locator& operator=(const Locator& locator);

		public: virtual Occurrence getElement() const;
		public: virtual Hurricane::Locator<Occurrence>* getClone() const;

		public: virtual bool isValid() const;

		public: virtual void progress();

		public: virtual string _getString() const;

        private: Occurrence getUniqueCellOccurrence(Cell* cell,Cell* aux_cell=NULL,Path path=Path());

	};

// Attributes
// **********

	private: const Cell* _cell;

// Constructors
// ************

	public: AllUniqueCellOccurrencesFrom();
	public: AllUniqueCellOccurrencesFrom(const Cell* cell);
	public: AllUniqueCellOccurrencesFrom(const AllUniqueCellOccurrencesFrom& allUniqueCellOccurrencesFrom);

// Operators
// *********

	public: AllUniqueCellOccurrencesFrom& operator=(const AllUniqueCellOccurrencesFrom& allUniqueCellOccurrencesFrom);

// Accessors
// *********

	public: virtual Collection<Occurrence>* getClone() const;
	public: virtual Hurricane::Locator<Occurrence>* getLocator() const;

// Others
// ******

	public: virtual string _getString() const;

};

// ****************************************************************************************************
// getAllUniqueCellOccurrencesFrom(Cell* cell) method
// ****************************************************************************************************

Occurrences getAllUniqueCellOccurrencesFrom(Cell* cell)
// **************************************************
{
    return AllUniqueCellOccurrencesFrom(cell);
}

// ****************************************************************************************************
// AllUniqueCellOccurrencesFrom implementation
// ****************************************************************************************************

AllUniqueCellOccurrencesFrom::AllUniqueCellOccurrencesFrom()
// ***************************
: 	Inherit(),
	_cell(NULL)
{
}

AllUniqueCellOccurrencesFrom::AllUniqueCellOccurrencesFrom(const Cell* cell)
// *******************************************
: 	Inherit(),
	_cell(cell)
{
}

AllUniqueCellOccurrencesFrom::AllUniqueCellOccurrencesFrom(const AllUniqueCellOccurrencesFrom& allUniqueCellOccurrencesFrom)
// ********************************************************
: 	Inherit(),
	_cell(allUniqueCellOccurrencesFrom._cell)
{
}

AllUniqueCellOccurrencesFrom& AllUniqueCellOccurrencesFrom::operator=(const AllUniqueCellOccurrencesFrom& allUniqueCellOccurrencesFrom)
// *******************************************************************
{
	_cell = allUniqueCellOccurrencesFrom._cell;
	return *this;
}

Collection<Occurrence>* AllUniqueCellOccurrencesFrom::getClone() const
// ***********************************************
{
	return new AllUniqueCellOccurrencesFrom(*this);
}

Locator<Occurrence>* AllUniqueCellOccurrencesFrom::getLocator() const
// **********************************************
{
	return new Locator(_cell);
}

string AllUniqueCellOccurrencesFrom::_getString() const
// *************************************
{
	string s = "<" + _TName("Cell::AllUniqueCellOccurrencesFrom");
	if (_cell) {
		s += " " + getString(_cell);
	}
	s += ">";
	return s;
}

// ****************************************************************************************************
// AllUniqueCellOccurrencesFrom::Locator implementation
// ****************************************************************************************************

AllUniqueCellOccurrencesFrom::Locator::Locator()
// **********************************
:	Inherit()
,    _cell(NULL)
,	_libraryLocator()
,	_cellLocator()
,   _allUniqueCellOccurrenceFrom()
,   _cellOccurrence()
{
}

AllUniqueCellOccurrencesFrom::Locator::Locator(const Cell* cell,Library* library)
// *************************************************************
:	Inherit()
,    _cell(cell)
,	_libraryLocator()
,	_cellLocator()
,   _allUniqueCellOccurrenceFrom()
,   _cellOccurrence()
{
    if (!library)
    {
        library=cell->getLibrary();
        while(library->getLibrary())
            library=library->getLibrary();
    }
    _libraryLocator=library->getLibraries().getLocator();

    _cellLocator=library->getCells().getLocator();

    while (!_cellOccurrence.isValid() && _cellLocator.isValid())
    {
        _cellOccurrence=getUniqueCellOccurrence(_cellLocator.getElement());
        _cellLocator.progress();
    }

    if (!_cellOccurrence.isValid())
        while (!_allUniqueCellOccurrenceFrom.isValid() && _libraryLocator.isValid())
        {
            _allUniqueCellOccurrenceFrom=Locator(_cell,_libraryLocator.getElement());
            _libraryLocator.progress();
        }
}

AllUniqueCellOccurrencesFrom::Locator::Locator(const Locator& locator)
// ****************************************************
:	Inherit()
,    _cell(locator._cell)
,	_libraryLocator(locator._libraryLocator)
,	_cellLocator(locator._cellLocator)
,   _allUniqueCellOccurrenceFrom(locator._allUniqueCellOccurrenceFrom)
,   _cellOccurrence(locator._cellOccurrence)
{
}

AllUniqueCellOccurrencesFrom::Locator& AllUniqueCellOccurrencesFrom::Locator::operator=(const Locator& locator)
// ******************************************************************************
{
    _cell=locator._cell;
    _libraryLocator=locator._libraryLocator;
    _cellLocator=locator._cellLocator;
    _allUniqueCellOccurrenceFrom=locator._allUniqueCellOccurrenceFrom;
    _cellOccurrence=locator._cellOccurrence;
	return *this;
}

Occurrence AllUniqueCellOccurrencesFrom::Locator::getElement() const
// *********************************************
{
    if (_cellOccurrence.isValid())
        return _cellOccurrence;
    else if (_allUniqueCellOccurrenceFrom.isValid())
        return _allUniqueCellOccurrenceFrom.getElement();
    else
        return Occurrence();
}

Locator<Occurrence>* AllUniqueCellOccurrencesFrom::Locator::getClone() const
// *****************************************************
{
	return new Locator(*this);
}

bool AllUniqueCellOccurrencesFrom::Locator::isValid() const
// *****************************************
{
	return _cellOccurrence.isValid() || _allUniqueCellOccurrenceFrom.isValid();
}

void AllUniqueCellOccurrencesFrom::Locator::progress()
// ************************************
{
    _cellOccurrence=Occurrence();
    if (_cellLocator.isValid())
    {
        do {
            _cellOccurrence=getUniqueCellOccurrence(_cellLocator.getElement());
            _cellLocator.progress();
        }
        while (!_cellOccurrence.isValid() && _cellLocator.isValid());
    }
    else if (_allUniqueCellOccurrenceFrom.isValid())
        _allUniqueCellOccurrenceFrom.progress();

    if (!_cellOccurrence.isValid())
        while (!_allUniqueCellOccurrenceFrom.isValid() && _libraryLocator.isValid())
        {
            _allUniqueCellOccurrenceFrom=Locator(_cell,_libraryLocator.getElement());
            _libraryLocator.progress();
        }
}

string AllUniqueCellOccurrencesFrom::Locator::_getString() const
// **************************************************
{
	string s = "<" + _TName("AllUniqueCellOccurrencesFrom::Locator");
	s += " " + getString(_cell);
	s += " " + getString(_cellLocator);
	s += " " + getString(_libraryLocator);
	s += " " + getString(_allUniqueCellOccurrenceFrom);
    s += " " + getString(_cellOccurrence);
	s += ">";
	return s;
}

Occurrence AllUniqueCellOccurrencesFrom::Locator::getUniqueCellOccurrence(Cell* cell,Cell* aux_cell,Path path)
{
    if (!aux_cell)
        aux_cell=cell;
    
    if (aux_cell == _cell)
        return Occurrence(cell,path);

    InstanceLocator slaveLocator=aux_cell->getSlaveInstances().getLocator();
    if (!slaveLocator.isValid())
        return Occurrence();
    Instance* slave=slaveLocator.getElement();

    slaveLocator.progress();
    if (slaveLocator.isValid())
        return Occurrence();

    return getUniqueCellOccurrence(cell,slave->getCell(),Path(slave,path));
}

} //namespace CRL
