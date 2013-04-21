
// -*- C++ -*-
//
// This file is part of the VSLSI Stand-Alone Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |      V L S I  Stand - Alone  Parsers / Drivers                  |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ConfEditorMain.cpp"                          |
// +-----------------------------------------------------------------+


#include  <Python.h>
#include  <cstdio>
#include  <libxml/xmlreader.h>

#include  <boost/program_options.hpp>
namespace boptions = boost::program_options;

#include  <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(4,5,0)) and not defined (__APPLE__)
#  include  <QGtkStyle>
#endif
#include  "vlsisapd/utilities/Path.h"
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
                          , "Do not uses GtkStyle (due to the File/Open native dialog bug).")
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

    Configuration* conf = Configuration::get ();

    if ( arguments.count("conf") ) {
      Utilities::Path confPath = ( arguments["conf"].as<string>() );
      if ( confPath.exists() ) {
        cout << "Reading configuration: <" << confPath.string() << ">." << endl;
        conf->readFromFile ( confPath.string() );
      } else {
        cout << "Configuration file: <" << confPath.string() << "> doesn't exists." << endl;
      }
    }

    Utilities::Path dotConfPath ( "./.coriolis2.configuration.xml" );
    if ( dotConfPath.exists() ) {
      cout << "Reading dot configuration: <" << dotConfPath.string() << ">." << endl;
      conf->readFromFile ( dotConfPath.string() );
    }

  //cout << "misc.catchCore:   " << conf->getParameter("misc.catchCore"  )->asBool() << endl;
  //cout << "kite.eventsLimit: " << conf->getParameter("kite.eventsLimit")->asInt () << endl;

    Utilities::Path pyDotConfPath ( "./.coriolis2.configuration.py" );
    if ( pyDotConfPath.exists() ) {
      cout << "Reading python dot configuration: <" << pyDotConfPath.string() << ">." << endl;
      Py_Initialize ();
      FILE* fd = fopen ( pyDotConfPath.string().c_str(), "r" );
      if ( fd ) {
        PyRun_SimpleFile ( fd, pyDotConfPath.string().c_str() );
        fclose ( fd );
      } else {
        cout << "Cannot open Python configuration file: <" << pyDotConfPath.string() << ">." << endl;
      }
      Py_Finalize ();
    }

    ConfEditorWidget* editor = new ConfEditorWidget ();
    editor->show ();
  //editor->getConfigurationWidget()->selectTab ( "Kite" );
  //editor->getConfigurationWidget()->hideTabs ( "Kite;Mauka" );

    returnCode = qa->exec ();
  }
  catch ( exception& e ) {
    cerr << "[ERROR] Catched exception:\n  " << e.what() << endl;
    returnCode = 127;
  }

  return returnCode;
}
