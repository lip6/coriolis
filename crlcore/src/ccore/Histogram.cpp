// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Histogram.cpp"                          |
// +-----------------------------------------------------------------+


#include  <cmath>
#include  <sstream>
#include  <fstream>
#include  <iomanip>
#include  "hurricane/utilities/Path.h"
#include  "crlcore/Histogram.h"



namespace CRL {

  using std::string;
  using std::ostream;
  using std::ofstream;
  using std::ostringstream;
  using std::setprecision;
  using std::setw;
  using std::setfill;
  using std::vector;
  using std::cerr;
  using std::endl;
  using Hurricane::Record;


  Histogram::Histogram ( double range, double step, size_t nbSets )
    : _range        (range)
    , _step         (step)
    , _mainTitle    ()
    , _titles       (nbSets)
    , _colors       (nbSets)
    , _indents      (nbSets)
    , _sets         (nbSets)
    , _totalSamples (nbSets)
    , _fileExtension()
  {
    if (_range != 0.0) {
      size_t binSize = (size_t)rint( _range / _step );
      for ( size_t iset=0 ; iset<nbSets ; ++iset ) {
        _sets[iset] = vector<float>( binSize );
      }
    }
  }


  Histogram::~Histogram ()
  { }


  void  Histogram::addSample ( double sample, size_t iset )
  {
    if (iset >= _sets.size()) return;

    size_t binIndex = (size_t)rint( sample / _step );
    if (_range != 0.0) {
      if (binIndex >= _sets.front().size()) binIndex = _sets.front().size() - 1;
    } else {
      while ( _sets.front().size() < binIndex + 1) {
        for ( vector<float>& rset : _sets ) rset.push_back( 0.0 );
      }
    }

    _sets        [iset][binIndex] += 1.0;
    _totalSamples[iset]++;
  }


  float  Histogram::getYRange () const
  {
    float yrange = 0.0;

    for ( size_t iset=0 ; iset<_sets.size() ; ++iset ) {
      for ( size_t i=0 ; i<_sets[iset].size() ; ++i )
        yrange = std::max ( yrange, _sets[iset][i] );
    }

    return ceil(yrange*10.0) / 10.0;
  }


  void  Histogram::normalize ( size_t iset )
  {
    if ( iset > _sets.size() ) return;
    for ( size_t i=0 ; i<_sets[iset].size() ; ++i ) _sets[iset][i] /= (float)_totalSamples[iset];
  }


  void  Histogram::toStream ( ostream& o )
  {
    o << setprecision(3);

    for ( size_t i=0 ; i<_sets.front().size() ; ++i ) {
      for ( size_t iset=0 ; iset<_sets.size() ; ++iset ) {
        o << _sets[iset][i] << " ";
      }
      o << "\n";
    }
  }


  string  Histogram::toString ( size_t iset )
  {
    if (iset >= _sets.size()) return "";

    ostringstream s;
    
    size_t titleWidth = _titles[iset].size();

    string hline = _indents[iset] + "+-" + string(titleWidth,'-') + "-+-------------+-----+\n";
    string tline = _indents[iset] + "| " + _titles[iset]          + " |    Count    |  %  |\n";

    s << hline << tline << hline;

    for ( size_t i=0 ; i<_sets[iset].size() ; ++i ) {
      float  value = _sets[iset][i];
      if ((value == 0.0) and (_range == 0.0)) continue;
      
      size_t percent = value*100.0 / _totalSamples[iset];
      s << _indents[iset]
        <<  "| " << setw(titleWidth) << (size_t)(_step*i)
        << " | " << setw(11)         << (size_t)(value)
        << " | " << setw( 3)         << percent
        << " | " << string( percent/2, '*' )
        << "\n";
    }

    s << hline;
    return s.str();
  }


  void  Histogram::toFile ( string path )
  {
    ofstream fd ( path.c_str() );
    toStream ( fd );
    fd.close ();
  }


  void  Histogram::toGnuplot ( string basename )
  {
    Utilities::Path datFile ( basename+_fileExtension+".dat" );
    toFile ( datFile.toString() );

    Utilities::Path pltFile ( basename+_fileExtension+".plt" );
    ofstream fd ( pltFile.toString().c_str() );

    if ( not _mainTitle.empty() )
      fd << "set title \"" << _mainTitle << "\"\n";

    fd << "set grid\n";
    fd << "set grid noxtics\n";
    fd << "set xrange [-0.5:9.5]\n";
    fd << "set xtics ( ";
    for ( size_t i=0 ; i<10 ; ++i ) {
      fd << ((i) ? " ," : "") << "\"<" << ((i+1)*10) << "%%\" " << i;
    }
    fd << " )\n";

    fd << "set yrange [0:" << setprecision(3) << getYRange() << "]\n";
    fd << "set ytics ( ";
    for ( float i=0.0 ; i<=40.0 ; i+=10.0 ) {
      fd << ((i != 0.0) ? " ," : "") << "\"" << i << "%%\" " << (i/100.0);
    }
    fd << " )\n";

    fd << "set style histogram cluster gap 1\n";
    fd << "set style fill solid noborder\n";
    fd << "set boxwidth 1\n";

    for ( size_t iset=0 ; iset<_sets.size() ; ++iset ) {
      fd << ((iset) ? "     " : "plot ");
      fd << "\"" << datFile.toString() << "\" using " << (iset+1);

      if ( not _titles[iset].empty() )  fd << " title \"" << _titles[iset] << "\"";

      fd << " with histogram";

      if ( not _colors[iset].empty() )  fd << " linecolor rgb \"" << _colors[iset] << "\"";

      fd << ((iset+1==_sets.size()) ? "\n" : ", \\\n");
    }

    fd.close ();
  }


  string  Histogram::_getString () const
  {
    ostringstream s;
    s << "<Histogram " << ((_mainTitle.empty()) ? "Anonymous" : _mainTitle) << ">";
    return s.str();
  }


  Record* Histogram::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      record->add ( getSlot("_mainTitle",_mainTitle) );
      for ( size_t iset=0 ; iset<_sets.size() ; ++iset ) {
        ostringstream attributeName;
        attributeName << "_titles[" << iset << "]";
        record->add ( getSlot(attributeName.str(),&_titles[iset]) );

        attributeName.str("");
        attributeName << "_colors[" << iset << "]";
        record->add ( getSlot(attributeName.str(),&_colors[iset]) );

        attributeName.str("");
        attributeName << "_sets[" << iset << "]";
        record->add ( getSlot(attributeName.str(),&_sets[iset]) );
      }
      record->add ( getSlot("_fileExtension",_fileExtension) );
    }

    return record;
  }


  Measure<Histogram>::Measure ( const Name& name )
    : BaseMeasure(name,0)
    , _datas         ()
  { }


  Measure<Histogram>::~Measure ()
  { for ( auto item : _datas ) delete item.second; }


  bool  Measure<Histogram>::isSimpleData () const
  { return false; }


  Histogram* Measure<Histogram>::getData ( size_t index ) const
  { return (index < _datas.size()) ? _datas[index].second : NULL; }


  void  Measure<Histogram>::setData ( size_t index, Histogram* data )
  {
    while ( _datas.size() <= index ) _datas.push_back( std::make_pair(0,(Histogram*)NULL) );
    delete _datas[index].second;
    _datas[index].second  = data;
    _datas[index].first  |= BaseMeasure::Set;
  }


  std::string  Measure<Histogram>::toString ( size_t ) const
  { return "Unsupported"; }


  void  Measure<Histogram>::toGnuplot ( size_t index, const std::string& basename ) const
  {
    Histogram*    h = getData( index );
    ostringstream passName;
    passName << basename << "." << setw(2) << setfill('0') << index;
    if (h) h->toGnuplot( passName.str() );
  }


  std::string  Measure<Histogram>::_getString () const
  { return "<Measure Histogram>"; }


  Record* Measure<Histogram>::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      record->add ( getSlot("_datas",_datas) );
    }
    return record;
  }

}  // End of CRL namespace.
