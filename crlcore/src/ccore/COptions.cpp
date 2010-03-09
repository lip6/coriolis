

// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================

// -*- C++ -*-
//
// $Id: COptions.cpp,v 1.7 2006/02/19 00:52:46 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./COptions.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  <sstream>

# include  "crlcore/COptions.h"




namespace {

  using namespace std;

  // -----------------------------------------------------------------
  // Begin of local namespace (internal objects).


// -------------------------------------------------------------------
// Class  :  "::_Frag()".

  class _Frag {
    public: string  _val;
    public: int     _order;

    public: _Frag ( string val, int order )
             : _val(val), _order(order) { };
  };

  typedef  vector<_Frag>  _FragVect; 




  // ---------------------------------------------------------------
  // Pre-defined objects.



// -------------------------------------------------------------------
// Function  :  "badShort()".

inline string  badShort ( const string &name )
{
  string  m;

  m += "COptions::Add() :";
  m += "\n\n";
  m += "    Short options must have exactly one character \"" + name + "\".\n";

  return ( m );
}



// -------------------------------------------------------------------
// Function  :  "dupOption()".

inline string  dupOption ( const string &name )
{
  string  m;

  m += "COptions::Add() :";
  m += "\n\n";
  m += "    Duplicate option \"" + name + "\".\n";

  return ( m );
}




// -------------------------------------------------------------------
// Function  :  "unknownOption()".

inline string  unknownOption ( const string &name )
{
  string  m;

  m += "COptions::Parse() :";
  m += "\n\n";
  m += "    Unknown option \"" + name + "\".\n";

  return ( m );
}




// -------------------------------------------------------------------
// Function  :  "unknownKey()".

inline string  unknownKey ( const string &name )
{
  string  m;

  m += "COptions::Parse() :";
  m += "\n\n";
  m += "    Unknown option key \"" + name + "\".\n";

  return ( m );
}




// -------------------------------------------------------------------
// Function  :  "missingArg()".

inline string  missingArg ( const string &name )
{
  string  m;

  m += "COptions::Parse() :";
  m += "\n\n";
  m += "    Missing mandatory argument of option \"" + name + "\"\n";

  return ( m );
}




// -------------------------------------------------------------------
// Function  :  "reParse()".

inline string  reParse ()
{
  string  m;

  m += "COptions::Parse() :";
  m += "\n\n";
  m += "    Attempt to re-parse options. You must explicitely clear\n";
  m += "    the COptions object before calling another COptions::Parse().\n";

  return ( m );
}


  // -----------------------------------------------------------------
  // End of local namespace (internal objects).

}


namespace CRL {




// x-----------------------------------------------------------------x
// |                      Global Definitions                         |
// x-----------------------------------------------------------------x


  const string  COptions::_true     = "__true";
  const string  COptions::_false    = "__false";
  const string  COptions::_remains  = "__remains";
  const string  COptions::_argvzero = "__argv[0]";
  const string  COptions::_missing  = "__missing";




// x-----------------------------------------------------------------x
// |                      Methods Definitions                        |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Conversion  :  "COptions::Option::operator string()".

COptions::Option::operator string ()
{
  if ( _times == 0       ) return ( COptions::_false );
  if ( _times <= _select ) return ( COptions::_false );
  if ( ! _hasArg         ) return ( COptions::_true  );

  return ( _args[ _select ] );
}


  string& COptions::Option::getArg ( size_t i )
  {
    return _args[i];
  }




// -------------------------------------------------------------------
// Constructor  :  "COptions::COptions()".

COptions::COptions () : _clear(true)
                      , _argHelp(false)
                      , _argVerbose(false)
                      , _argVeryVerbose(false)
                      , _argCatchCore(false)
                      , _missingBuiltin("",_missing)
{
  // Special option holding all remaining non options arguments.
  Add ( "", _remains , true );
  Add ( "", _argvzero, true );
}




// -------------------------------------------------------------------
// Method  :  "COptions::Find()".

COptions::OptionVect::iterator  COptions::Find ( string name  )
{
  OptionVect::iterator  it;
  OptionVect::iterator  end = _options.end ();

  if ( name.size() == 0 ) return ( end );
  if ( name.size() == 1 ) {
    for ( it=_options.begin() ; it!=end ; it++ ) {
      if ( it->getShort() == name ) break;
    }
  } else {
    for ( it=_options.begin() ; it!=end ; it++ ) {
      if ( it->getLong() == name ) break;
    }
  }

  return ( it );
}




// -------------------------------------------------------------------
// Method  :  "COptions::Add()".

void  COptions::Add ( string  nameShort
                    , string  nameLong
                    , bool    hasArg
                    , string  defaultArg )
{
  OptionVect::iterator  it;
  OptionVect::iterator  end = _options.end ();

  if ( nameShort.size() > 1 ) throw ( Error(badShort(nameShort)) );

  if ( ( it=Find(nameShort)  ) == end ) {
    if ( ( it=Find(nameLong) ) == end ) {
      _options.push_back ( Option(nameShort,nameLong,hasArg,defaultArg) );
    } else
      throw ( Error (dupOption(nameLong)) );
  } else
    throw ( Error (dupOption(nameShort)) );
}




// -------------------------------------------------------------------
// Method  :  "COptions::AddHelp()".

void  COptions::AddHelp ()
{
  Add ( "h", "help" );
  _argHelp = true;
}




// -------------------------------------------------------------------
// Method  :  "COptions::AddVerbose()".

void  COptions::AddVerbose ()
{
  Add ( "v", "verbose" );
  _argVerbose = true;
}




// -------------------------------------------------------------------
// Method  :  "COptions::AddVeryVerbose()".

void  COptions::AddVeryVerbose ()
{
  Add ( "V", "very-verbose" );
  _argVeryVerbose = true;
}




// -------------------------------------------------------------------
// Method  :  "COptions::AddVeryVerbose()".

void  COptions::AddCatchCore ()
{
  Add ( "D", "core-dump" );
  _argCatchCore = true;
}




// -------------------------------------------------------------------
// Method  :  "COptions::getHelp()".

COptions::Option &COptions::getHelp ()
{
  if ( ! _argHelp ) return ( _missingBuiltin );

  return ( (*this)["help"] );
}




// -------------------------------------------------------------------
// Method  :  "COptions::getVerbose()".

COptions::Option &COptions::getVerbose ()
{
  if ( ! _argVerbose ) return ( _missingBuiltin );

  return ( (*this)["verbose"] );
}




// -------------------------------------------------------------------
// Method  :  "COptions::getVeryVerbose()".

COptions::Option &COptions::getVeryVerbose ()
{
  if ( ! _argVeryVerbose ) return ( _missingBuiltin );

  return ( (*this)["very-verbose"] );
}




// -------------------------------------------------------------------
// Method  :  "COptions::getCatchCore()".

COptions::Option &COptions::getCatchCore ()
{
  if ( ! _argCatchCore ) return ( _missingBuiltin );

  return ( (*this)["core-dump"] );
}





// -------------------------------------------------------------------
// Method  :  "COptions::Parse()".

void  COptions::Parse ( int argc, char *argv[] )
{
  if ( ! _clear ) throw ( Error(reParse()) );
  _clear = false;

  _FragVect  fragOpts;
  _FragVect  fragArgs;
  string     arg;
  size_t     equal;
  int        order = 0;
  bool       longOpt;


  // First step : separate options from arguments in two
  // indexed list (index is relative to the argv[] ordering.
  for ( int i=1 ; i<argc ; i++ ) {
    arg = argv[i];

    if ( arg.empty()   ) continue;
    if ( arg[0] == '-' ) {
      ( arg[1] == '-' ) ? longOpt = true : longOpt = false;

      equal = arg.find ( '=' );
      if ( equal < arg.size() ) {
        if ( longOpt )
          fragOpts.push_back ( _Frag( arg.substr(2,equal-2), order++) );
        else {
          for ( unsigned j=1 ; j<equal ; j++ )
            fragOpts.push_back ( _Frag( arg.substr(j,1), order++) );
        }
        fragArgs.push_back ( _Frag( arg.substr(equal+1), order++) );
      } else {
        if ( longOpt )
          fragOpts.push_back ( _Frag( arg.substr(2), order++) );
        else {
          for ( unsigned j=1 ; j<arg.size() ; j++ )
            fragOpts.push_back ( _Frag( arg.substr(j,1), order++) );
        }
      }
    } else {
      fragArgs.push_back ( _Frag( arg, order++) );
    }
  }


  // Second step : try to match options.
  _FragVect::iterator   itOpt  = fragOpts.begin ();
  _FragVect::iterator   endOpt = fragOpts.end   ();
  _FragVect::iterator   itArg  = fragArgs.begin ();
  _FragVect::iterator   endArg = fragArgs.end   ();
  OptionVect::iterator  opt;

  for ( ; itOpt!=endOpt ; itOpt++ ) {
    opt = Find ( itOpt->_val );

    if ( opt == _options.end() ) throw ( Error(unknownOption(itOpt->_val)) );

    if ( opt->getHasArg() ) {
      if ( (itArg != endArg ) && ( itArg->_order == itOpt->_order+1 ) )
        { opt->AddArg ( itArg->_val ); itArg++; continue; }

      if ( opt->getDefault().size() > 0 )
        { opt->AddArg ( opt->getDefault() ); continue; }

      throw ( Error(missingArg(itOpt->_val)) );
    }

    opt->AddArg ( "__true" );
  }


  // Third step : set default arguments (if not found on
  // the command line).
  for ( opt=_options.begin() ; opt!=_options.end() ; opt++ ) {
    if ( ( opt->getDefault().size() > 0 ) && ( opt->getTimes() == 0 ) )
      opt->AddArg ( opt->getDefault() );
  }


  // Storing argv[0] (program's name).
  Find ( _argvzero )->AddArg ( argv[0] );

  // Storing all non-options remaining arguments.
  opt = Find ( _remains );
  for ( ; itArg != endArg ; itArg++ )
    opt->AddArg ( itArg->_val );
}





// -------------------------------------------------------------------
// Method  :  "COptions::Clear()".

void  COptions::Clear ()
{
  OptionVect::iterator  it;
  OptionVect::iterator  end = _options.end ();

  for ( it=_options.begin() ; it!=end ; it++ ) it->Clear ();

  _clear = true;
}




// -------------------------------------------------------------------
// Operator  :  "COptions::operator[]()".

COptions::Option &COptions::operator[] ( string key )
{
  size_t  separ = key.find ( '/' );
  int     index = 0;

  if ( separ < key.size() ) {
    istringstream sindex ( key.substr ( separ + 1 ) );
    sindex >> index;
  }

  OptionVect::iterator  opt = Find ( key.substr(0,separ) );
  if ( opt == _options.end () ) {
    throw ( Error(unknownKey(key)) );
  }

  opt->SetSelect ( index );
  return ( *opt );
}




// -------------------------------------------------------------------
// Method  :  "COptions::_getPrint()".

string COptions::_getPrint ()
{
  ostringstream         os;
  ostringstream         key;
  OptionVect::iterator  it;
  OptionVect::iterator  end = _options.end ();
  string                name;
  string                minus;


  os << (string)(*this)[_argvzero];

  for ( it=_options.begin() ; it!=end ; it++ ) {
    if ( it->getLong() == _argvzero ) continue;
    if ( it->getLong() == _remains  ) continue;
    if ( it->getLong().size() > 0 ) {
      minus = "--";
      name  = it->getLong();
    } else {
      minus = "-";
      name  = it->getShort();
    }

    for ( int i=0; i < it->getTimes(); i++ ) {
      if ( it->getHasArg() ) {
        key.str ("");
        key << name << "/" << i;
        os << " " << minus << name << "=" << (string)(*this)[ key.str() ];
      } else
        os << " " << minus << name;
    }
  }

  it = Find ( _remains );
  for ( int i=0; i < it->getTimes(); i++ ) {
    key.str ("");
    key << _remains << "/" << i;
    os << " " << (string)(*this)[ key.str() ];
  }

  return ( os.str() );
}




}
