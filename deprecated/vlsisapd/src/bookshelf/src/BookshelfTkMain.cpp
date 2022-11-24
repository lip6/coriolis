
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2010, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |           B o o k s h e l f   P a r s e r                       |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./BookshelfTkMain.cpp"                    |
// +-----------------------------------------------------------------+


#include  <cmath>
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


void  doDumpDualNetlist ( Circuit* circuit )
{
  map<string,Node*>&          nodes = circuit->getNodes();
  map<string,Node*>::iterator inode = nodes.begin();
  for ( ; inode != nodes.end() ; ++inode ) {
    cout << "NODE:" << (*inode).second->getName() << endl;
    map<size_t,Pin*>&          pins = (*inode).second->getPins();
    map<size_t,Pin*>::iterator ipin = pins.begin();
    for ( ; ipin != pins.end() ; ++ipin ) {
      cout << "  pin:" << (*ipin).second->getNet()->getName() << endl;
    }
  }
}


void  doFindMegaCells ( Circuit* circuit )
{
  size_t megacellsNb = 0;

  map<string,Node*>&          nodes = circuit->getNodes();
  map<string,Node*>::iterator inode = nodes.begin();
  for ( ; inode != nodes.end() ; ++inode ) {
    Node* node = (*inode).second;

    map<size_t,Pin*>&          pins = node->getPins();
  //map<size_t,Pin*>::iterator ipin = pins.begin();
    if ( (double)pins.size() > node->getWidth() ) {
      cout << setw(4) << right << megacellsNb
           << ":megacell:" << node->getName()
           << " with " << pins.size() << " terminals"
           << " for " << setprecision(4) << node->getWidth() << " pitchs."
           << endl;
    //for ( ; ipin != pins.end() ; ++ipin ) {
    //  cout << "  pin:" << (*ipin).second->getNet()->getName() << endl;
    //}
      ++megacellsNb;
    }
  }
}


void  doIspd04Square ( Circuit* circuit )
{
  vector<Row*>& rows      = circuit->getRows ();
  size_t        deltaRows = (size_t)(round ( sqrt(2.0) * (double)rows.size() )) - rows.size();
  double        numsites  = round ( rows[0]->getNumsites() / sqrt(2.0) );

  for ( size_t irow=0 ; irow<rows.size() ; ++irow ) {
    rows[irow]->setNumsites ( numsites );
  }
  double deltaRowCoordinate = rows[rows.size()-1]->getCoordinate();

  for ( size_t irow=0 ; irow<deltaRows ; ++irow ) {
    deltaRowCoordinate += 16.0;

    Row* row = new Row();
    row->setCoordinate   ( deltaRowCoordinate );
    row->setHeight       (               16.0 );
    row->setSitewidth    (                1.0 );
    row->setSitespacing  (                1.0 );
    row->setSiteorient   (     Orientation::N );
    row->setSitesymmetry (        Symmetry::Y );
    row->setSubrowOrigin (                0.0 );
    row->setNumsites     (           numsites );
    circuit->addRow ( row );
  }
  circuit->setNumRows ( circuit->getRows().size() );
  circuit->setSclName ( circuit->getDesignName()+"square.scl" );

  circuit->drive ( ".", Circuit::Scl );
}


int main ( int argc, char* argv[] )
{
  int returnCode = 0;

  try {
    bool ispd04square;
    bool findMegaCells;
    bool dumpDualNetlist;

    boptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"        , "Print this help." )
      ( "aux,a"         , boptions::value<string>()
                        , "The path of the Bookshelf <.aux> file." )
      ( "dual-netlist"  , boptions::bool_switch(&dumpDualNetlist)->default_value(false)
                        , "Print dual netlist representation (pins on each nodes)." )
      ( "find-megacells", boptions::bool_switch(&findMegaCells)->default_value(false)
                        , "Guess Mega-Cells by their pins number." )
      ( "ispd04-square" , boptions::bool_switch(&ispd04square)->default_value(false)
                        , "Change form factor of ISPD04 benchmarks to square." );

    boptions::variables_map arguments;
    boptions::store  ( boptions::parse_command_line(argc,argv,options), arguments );
    boptions::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl; 
      exit ( 0 );
    }

    if ( arguments.count("aux") ) {
      unique_ptr<Circuit> circuit ( Circuit::parse(arguments["aux"].as<string>()) );

      circuit->check ();

      if ( ispd04square    ) doIspd04Square    ( circuit.get() );
      if ( findMegaCells   ) doFindMegaCells    ( circuit.get() );
      if ( dumpDualNetlist ) doDumpDualNetlist ( circuit.get() );


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
