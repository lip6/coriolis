
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ConfEditorMain.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <boost/program_options.hpp>
namespace boptions = boost::program_options;

#include  <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

#include  <libxml/xmlreader.h>
#include  <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
#  include  <QGtkStyle>
#endif
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"
#include  "vlsisapd/configuration/ConfEditorWidget.h"


using namespace std;
using namespace Cfg;


int main ( int argc, char* argv[] )
{
  int returnCode = 0;

  try {
    bool disableGtkStyle;

    boptions::options_description options ("Command line arguments & options");
    options.add_options()
      ( "help,h"          , "Print this help." )
      ( "disable-gtkstyle", boptions::bool_switch(&disableGtkStyle)->default_value(false)
                          , "Run the detailed router (Kite).")
      ( "conf,c"          , boptions::value<string>()
                          , "The path of the configuration file." );

    boptions::variables_map arguments;
    boptions::store  ( boptions::parse_command_line(argc,argv,options), arguments );
    boptions::notify ( arguments );

    if ( arguments.count("help") ) {
      cout << options << endl; 
      exit ( 0 );
    }

    auto_ptr<QApplication> qa ( new QApplication(argc,argv) );
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
    if ( not disableGtkStyle ) qa->setStyle ( new QGtkStyle() );
#endif

    bfs::path::default_name_check ( bfs::portable_posix_name );

    Configuration* conf = Configuration::get ();

    if ( arguments.count("conf") ) {
      bfs::path confPath = ( arguments["conf"].as<string>() );
      if ( bfs::exists(confPath) ) {
        cout << "Reading configuration: <" << confPath.string() << ">." << endl;
        conf->readFromFile ( confPath.string() );
      } else {
        cout << "Configuration file: <" << confPath.string() << "> doesn't exists." << endl;
      }
    }

    bfs::path dotConfPath ( "./.coriolis2.configuration.xml" );
    if ( bfs::exists(dotConfPath) ) {
      cout << "Reading dot configuration: <" << dotConfPath.string() << ">." << endl;
      conf->readFromFile ( dotConfPath.string() );
    }

    ConfEditorWidget* editor = new ConfEditorWidget ();
    editor->show ();
  //QFileDialog::getOpenFileName(NULL, "Choose file", "", "");

    returnCode = qa->exec ();
  }
  catch ( exception& e ) {
    cerr << "[ERROR] Catched exception:\n  " << e.what() << endl;
    returnCode = 127;
  }

  return returnCode;
}
