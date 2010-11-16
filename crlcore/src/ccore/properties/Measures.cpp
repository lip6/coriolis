
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Measures.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iomanip>

#include  "hurricane/Error.h"
#include  "hurricane/DBo.h"

#include  "crlcore/Measures.h"


namespace CRL {


  using std::string;
  using std::vector;
  using std::ostringstream;
  using std::setw;
  using std::right;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;

  
// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresSet".


  BaseMeasure::~BaseMeasure () {}


  const char* MissingMeasures = "Measures::%s(): %s missing the Measures extension.";


  template<>
  Name  StandardPrivateProperty<MeasuresDatas*>::_name = "CRL::Measures";

  
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
      out << setw(measure->getFieldWidth()) << right << measure->toString();
    }

    return out.str();
  }

  
// -------------------------------------------------------------------
// Class  :  "CRL::MeasuresDatas".


  MeasuresDatas::MeasuresDatas ()
    : _measures()
  { }

  
// -------------------------------------------------------------------
// Class  :  "CRL::Measures".


  const MeasuresSet* Measures::get ( const DBo* object )
  {
    Extension* extension = Extension::get ( object );
    if ( extension != NULL )
      return &extension->getValue()->_measures;

    return NULL;
  }


  Measures::Extension* Measures::_getOrCreate ( DBo* object )
  {
    Extension* extension = Extension::get ( object );
    if ( extension == NULL ) {
      extension = Extension::create ( new MeasuresDatas() );
      object->put ( extension );
    }
    return extension;
  }


} // End of CRL namespace.
