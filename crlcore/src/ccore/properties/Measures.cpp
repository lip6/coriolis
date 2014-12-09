// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Measures.cpp"                           |
// +-----------------------------------------------------------------+


#include  <iomanip>
#include  "hurricane/Error.h"
#include  "hurricane/DBo.h"
#include  "crlcore/Measures.h"


template<>
Hurricane::Name  Hurricane::StandardPrivateProperty<CRL::MeasuresDatas>::_name = "CRL::Measures";


namespace CRL {


  using std::string;
  using std::vector;
  using std::ostringstream;
  using std::cerr;
  using std::endl;
  using std::setw;
  using std::right;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;

  
// -------------------------------------------------------------------
// Class  :  "CRL::BaseMeasure".


          BaseMeasure::~BaseMeasure () {}
  bool    BaseMeasure::isSimpleData () const { return true; }
  void    BaseMeasure::toGnuplot    ( const string& ) const {}
  string  BaseMeasure::_getString   () const { return "<Undefined Measure>"; }
  Record* BaseMeasure::_getRecord   () const { return NULL; }

  
// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresSet".


  MeasuresSet::~MeasuresSet ()
  {
    iterator imeasure = begin();
    for ( ; imeasure != end() ; ++imeasure )
      delete (*imeasure).second;
  }


  string  MeasuresSet::toStringHeaders ( const vector<Name>& names ) const
  {
    ostringstream out;
    out << "#";

    for ( size_t i=0 ; i<names.size() ; ++i ) {
      const_iterator imeasure = find ( names[i] );
      if ( imeasure == end() ) continue;

      const BaseMeasure* measure = (*imeasure).second;
      if ( measure->isSimpleData() )
        out << setw(measure->getFieldWidth()) << right << measure->getName();
    }

    return out.str();
  }


  string  MeasuresSet::toStringDatas ( const vector<Name>& names ) const
  {
    ostringstream out;
    out << " ";

    for ( size_t i=0 ; i<names.size() ; ++i ) {
      const_iterator imeasure = find ( names[i] );
      if ( imeasure == end() ) continue;

      const BaseMeasure* measure = (*imeasure).second;
      if ( measure->isSimpleData() )
        out << setw(measure->getFieldWidth()) << right << measure->toString();
    }

    return out.str();
  }


  void  MeasuresSet::toGnuplot ( Name name, const string& basename ) const
  {
    const_iterator imeasure = find ( name );
    if ( imeasure == end() ) return;

    const BaseMeasure* measure = (*imeasure).second;
    if ( measure->isSimpleData() ) return;

    measure->toGnuplot ( basename );
  }


  string  MeasuresSet::_getString () const
  {
    ostringstream s;
    s << "<MeasuresSet " << size() << ">";
    return s.str();
  }


  Record* MeasuresSet::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if ( record ) {
      const_iterator imeasure = begin();
      for ( ; imeasure != end() ; ++imeasure ) {
        record->add ( getSlot ( getString((*imeasure).first), ((*imeasure).second) ) );
      }
    }
    return record;
  }

  
// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresDatas".


  MeasuresDatas::MeasuresDatas ()
    : _measures()
  { }

  
  MeasuresDatas::MeasuresDatas ( const MeasuresDatas& other )
    : _measures()
  {
    // if ( not other._measures.empty() ) {
    //   cerr << Error("MeasuresDatas copy constructor called on non-empty MeasuresDatas is forbidden.\n"
    //                 "(source has %u elements)", other._measures.size() ) << endl;
    // }
  }

  
// -------------------------------------------------------------------
// Class  :  "CRL::Measures".


  const char* MissingMeasures = "Measures::%s(): %s missing the Measures extension.";


  const MeasuresSet* Measures::get ( const DBo* object )
  {
    Extension* extension = Extension::get ( object );
    if ( extension != NULL )
      return &extension->getValue()._measures;

    return NULL;
  }


  Measures::Extension* Measures::_getOrCreate ( DBo* object )
  {
    Extension* extension = Extension::get ( object );
    if ( extension == NULL ) {
      extension = Extension::create ( MeasuresDatas() );
      object->put ( extension );
    }
    return extension;
  }


} // End of CRL namespace.
