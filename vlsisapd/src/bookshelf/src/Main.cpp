
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Main.cpp"                               |
// +-----------------------------------------------------------------+


#include  <iostream>
#include  <memory>
using namespace std;

#include  <boost/program_options.hpp>
namespace boptions = boost::program_options;

#include  "vlsisapd/bookshelf/Exception.h"
#include  "vlsisapd/bookshelf/Node.h"
#include  "vlsisapd/bookshelf/Pin.h"
#include  "vlsisapd/bookshelf/Net.h"
#include  "vlsisapd/bookshelf/Row.h"
#include  "vlsisapd/bookshelf/Circuit.h"
using namespace Bookshelf;


int main ( int argc, char* argv[] )
{
  int returnCode = 0;

  try {
    boptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h", "Print this help." )
      ( "aux,a" , boptions::value<string>()
                , "The path of the Bookshelf <.aux> file." );

    boptions::variables_map arguments;
    boptions::store  ( boptions::parse_command_line(argc,argv,options), arguments );
    boptions::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl; 
      exit ( 0 );
    }

    if ( arguments.count("aux") ) {
      auto_ptr<Circuit> circuit ( Circuit::parse(arguments["aux"].as<string>()) );

      circuit->check ();

      vector<Row*>& rows = circuit->getRows ();
      for ( size_t irow=0 ; irow<rows.size() ; ++irow ) {
        rows[irow]->setNumsites ( 1096.0 );
      }

      size_t rowAddition = 40;
      for ( size_t irow=0 ; irow<rowAddition ; ++irow ) {
        Row* row = new Row();
        row->setCoordinate   ( 1520.0 + (irow+1)*16.0 );
        row->setHeight       (   16.0 );
        row->setSitewidth    (    1.0 );
        row->setSitespacing  (    1.0 );
        row->setSiteorient   ( Orientation::N );
        row->setSitesymmetry ( Symmetry::Y );
        row->setSubrowOrigin (    0.0 );
        row->setNumsites     ( 1096.0 );
        circuit->addRow ( row );
      }
      circuit->setNumRows ( circuit->getRows().size() );
      circuit->setSclName ( arguments["aux"].as<string>()+"square.scl" );

      circuit->drive ( ".", Circuit::Scl );

      // map<string,Node*>&          nodes = circuit->getNodes();
      // map<string,Node*>::iterator inode = nodes.begin();
      // for ( ; inode != nodes.end() ; ++inode ) {
      //   cerr << "NODE:" << (*inode).second->getName() << endl;
      //   map<size_t,Pin*>&          pins = (*inode).second->getPins();
      //   map<size_t,Pin*>::iterator ipin = pins.begin();
      //   for ( ; ipin != pins.end() ; ++ipin ) {
      //     cerr << "  pin:" << (*ipin).second->getNet()->getName() << endl;
      //   }
      // }
    }
  }
  catch ( Exception& e ) {
    cerr << e.what() << endl;
    returnCode = 128;
  }
  catch ( exception& e ) {
    cerr << "[ERROR] Catched exception:\n  " << e.what() << endl;
    returnCode = 127;
  }

  return returnCode;
}
