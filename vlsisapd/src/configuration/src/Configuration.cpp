
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
// |  C++ Module  :       "./Configuration.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstring>
#include  <sstream>
#include  <fstream>
#include  <iomanip>
#include  <vector>
#include  <libxml/xmlreader.h>
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"
#include  "vlsisapd/configuration/ConfigurationDialog.h"
#include  "vlsisapd/configuration/ParameterWidget.h"


namespace {

  using namespace std;
  using namespace Cfg;
    

  void  tokenize ( set<string>& tokens, const string& line )
  {
    static std::string separators = " ;";
           size_t      iBegin     = 0;
           size_t      iEnd       = 0;

    for ( ; iEnd < line.size() ; ++iEnd ) {
      if ( separators.find(line[iEnd]) != std::string::npos ) {
        if ( iBegin < iEnd )
          tokens.insert ( line.substr(iBegin,iEnd-iBegin) );
        iBegin = iEnd+1;
      }
    }
    if ( iBegin < iEnd )
      tokens.insert ( line.substr(iBegin,iEnd-iBegin) );
  }


  class XmlParser {
    public:
              XmlParser          ( Configuration*, const string& fileName );
      bool    parse              ();
    private:
      void    _configurationNode ();
      void    _parameterNode     ();
      void    _itemNode          ();
      void    _groupNode         ();
      void    _masterNode        ();
      void    _slaveNode         ();
      void    _layoutNode        ();
      void    _tabNode           ();
      void    _widgetNode        ();
      string  _getAttributeValue ( const char* attributeName );
    private:
      Configuration*       _configuration;
      string               _fileName;
      xmlTextReaderPtr     _reader;
      int                  _status;
      string               _tool;
      Parameter*           _parameter;
  };


  XmlParser::XmlParser ( Configuration* conf
                       , const string&  fileName )
    : _configuration(conf)
    , _fileName     (fileName)
    , _reader       (NULL)
    , _status       (1)
    , _tool         ("<none>")
    , _parameter    (NULL)
  { }


  bool  XmlParser::parse ()
  {
    LIBXML_TEST_VERSION;

    _reader = xmlReaderForFile ( _fileName.c_str(), NULL, 0 );
    if ( _reader != NULL ) {
      _status = xmlTextReaderRead ( _reader );
      while ( _status == 1 ) {
        string tag ( (const char*)xmlTextReaderConstName(_reader) );
        if ( tag == "configuration" ) _configurationNode();

        _status = xmlTextReaderRead ( _reader );
      }
      xmlFreeTextReader ( _reader );

      if ( _status != 0 ) {
        cerr << "[ERROR] Syntax error in \"" << _fileName << "\" configuration file." << endl;
      }
    //  xmlCleanupParser (); // CF libxml2 documentation if libxml2 parser is use by the application any other time : DO NOT CALL xmlCleanupParser (at least on mac osx)
    }

    return (_status == 0);
  }


  void  XmlParser::_configurationNode ()
  {
    if ( xmlTextReaderNodeType(_reader) == XML_READER_TYPE_END_ELEMENT ) return;

    _status = xmlTextReaderRead ( _reader );
    while ( _status == 1 ) {
      string tag ( (const char*)xmlTextReaderConstName(_reader) );

      if      ( tag == "parameter"     ) _parameterNode();
      else if ( tag == "group"         ) _groupNode();
      else if ( tag == "layout"        ) _layoutNode();
      else if ( tag == "configuration" ) return;

      _status = xmlTextReaderRead ( _reader );
    }
  }


  void  XmlParser::_parameterNode ()
  {
    if ( xmlTextReaderNodeType(_reader) == XML_READER_TYPE_END_ELEMENT ) return;

    string attrId        = _getAttributeValue("id");
    string attrType      = _getAttributeValue("type");
    string attrRestart   = _getAttributeValue("needRestart");
    string attrMustExist = _getAttributeValue("mustExist");
    string attrIsFile    = _getAttributeValue("isFile");
    string attrIsPath    = _getAttributeValue("isPath");

    Parameter::Type type = Parameter::String;
    if      ( attrType == "string"     ) type = Parameter::String;
    else if ( attrType == "bool"       ) type = Parameter::Bool;
    else if ( attrType == "int"        ) type = Parameter::Int;
    else if ( attrType == "double"     ) type = Parameter::Double;
    else if ( attrType == "percentage" ) type = Parameter::Percentage;
    else if ( attrType == "enumerate"  ) type = Parameter::Enumerate;

    _parameter = _configuration->getParameter ( attrId );
    if ( _parameter == NULL ) {
      _parameter = _configuration->addParameter ( attrId 
                                                , type
                                                , _getAttributeValue("value")
                                                );
    } else {
      _parameter->setString ( _getAttributeValue("value")
                            , (Parameter::AllRequirements | Parameter::FromString)
                            & ~Parameter::TypeCheck
                            );
    }

    if ( not attrRestart.empty()   ) _parameter->setFlags ( Parameter::NeedRestart );
    if ( not attrMustExist.empty() ) _parameter->setFlags ( Parameter::MustExist );
    if ( not attrIsFile.empty()    ) _parameter->setFlags ( Parameter::IsFile );
    if ( not attrIsPath.empty()    ) _parameter->setFlags ( Parameter::IsPath );

    if ( type == Parameter::Percentage ) {
      istringstream s ( _getAttributeValue("value") );
      double ratio;
      s >> ratio;

      _parameter->setPercentage ( ratio );
    }

    if (   (type == Parameter::Enumerate)
       and (xmlTextReaderIsEmptyElement(_reader) == 0) ) {
      _status = xmlTextReaderRead ( _reader );
      while ( _status == 1 ) {
        string tag ( (const char*)xmlTextReaderConstName(_reader) );
        if      ( tag == "item"      ) _itemNode();
        else if ( tag == "parameter" ) break;
        
        _status = xmlTextReaderRead ( _reader );
      }
    }

    if (  (type == Parameter::Int)
       or (type == Parameter::Enumerate) ) {
      string attrMin  = _getAttributeValue("min");
      string attrMax  = _getAttributeValue("max");

      int i;
      if ( not attrMin.empty() ) {
        istringstream s ( attrMin ); s >> i; _parameter->setMin ( i );
      }
      if ( not attrMax.empty() ) {
        istringstream s ( attrMax ); s >> i; _parameter->setMax ( i );
      }
    }

    if (  (type == Parameter::Double)
       or (type == Parameter::Percentage) ) {
      string attrMin  = _getAttributeValue("min");
      string attrMax  = _getAttributeValue("max");

      double d;
      if ( not attrMin.empty() ) {
        istringstream s ( attrMin ); s >> d; _parameter->setMin ( d );
      }
      if ( not attrMax.empty() ) {
        istringstream s ( attrMax ); s >> d; _parameter->setMax ( d );
      }
    }

    _parameter = NULL;
  }


  void  XmlParser::_itemNode ()
  {
    string attrLabel = _getAttributeValue("label");
    string attrValue = _getAttributeValue("value");
    int    value;

    istringstream s ( attrValue );
    s >> value;

    _parameter->addValue ( attrLabel, value );
  }


  void  XmlParser::_groupNode ()
  {
    if ( xmlTextReaderNodeType(_reader) == XML_READER_TYPE_END_ELEMENT ) return;

    _status = xmlTextReaderRead ( _reader );
    while ( _status == 1 ) {
      string tag ( (const char*)xmlTextReaderConstName(_reader) );

      if      ( tag == "master" ) _masterNode();
      else if ( tag == "slave"  ) _slaveNode();
      else if ( tag == "group"  ) {
        _parameter = NULL;
        return;
      }

      _status = xmlTextReaderRead ( _reader );
    }
  }


  void  XmlParser::_masterNode ()
  { _parameter = _configuration->getParameter ( _getAttributeValue("id") ); }


  void  XmlParser::_slaveNode ()
  {
    string attrId = _getAttributeValue("id");
    if ( _parameter != NULL ) _parameter->addSlave ( attrId );
  }


  void  XmlParser::_layoutNode ()
  {
    _status = xmlTextReaderRead ( _reader );
    while ( _status == 1 ) {
      string tag ( (const char*)xmlTextReaderConstName(_reader) );
      if      ( tag == "tab"    ) _tabNode();
      else if ( tag == "layout" ) return;

      _status = xmlTextReaderRead ( _reader );
    }
  }


  void  XmlParser::_tabNode ()
  {
    string attrName = _getAttributeValue("name");

    _configuration->getLayout().addTab ( attrName );

    _status = xmlTextReaderRead ( _reader );
    while ( _status == 1 ) {
      string tag ( (const char*)xmlTextReaderConstName(_reader) );
      if      ( tag == "widget" ) _widgetNode();
      else if ( tag == "tab"    ) return;

      _status = xmlTextReaderRead ( _reader );
    }
  }


  void  XmlParser::_widgetNode ()
  {
    if ( xmlTextReaderNodeType(_reader) == XML_READER_TYPE_END_ELEMENT ) return;

    string attrId       = _getAttributeValue("id");
    string attrType     = _getAttributeValue("type");
    string attrLabel    = _getAttributeValue("label");
    string attrColumn   = _getAttributeValue("column");
    string attrSpan     = _getAttributeValue("span");
    string attrSpinBox  = _getAttributeValue("spinbox");
    string attrFileName = _getAttributeValue("filename");
    string attrPathName = _getAttributeValue("pathname");

    if ( attrId.empty() and attrType.empty() ) {
      cerr << "[ERROR] In <widget> tag, neither \"id\" nor \"type\" attribute." << endl;
      return;
    }

    int column = 0;
    if ( not attrColumn.empty() ) {
      istringstream s ( attrColumn ); s >> column;
    }

    int span = 1;
    if ( not attrSpan.empty() ) {
      istringstream s ( attrSpan ); s >> span;
    }

    if ( not attrType.empty() ) {
      if      ( attrType == "title"   ) _configuration->getLayout().getBackTab()->addWidget ( WidgetDescription::title  (attrLabel) );
      if      ( attrType == "section" ) _configuration->getLayout().getBackTab()->addWidget ( WidgetDescription::section(attrLabel,column) );
      else if ( attrType == "rule"    ) _configuration->getLayout().getBackTab()->addWidget ( WidgetDescription::rule   () );
      return;
    }

    Parameter* parameter = _configuration->getParameter ( attrId );
    if ( parameter == NULL ) {
      cerr << "[ERROR] In <widget> tag, no parameter of id:" << attrId << "." << endl;
      return;
    }

    int flags = 0;
    if ( attrSpinBox  == "true" ) flags |= ParameterWidget::UseSpinBox;
    if ( attrFileName == "true" ) flags |= ParameterWidget::IsFileName;
    if ( attrPathName == "true" ) flags |= ParameterWidget::IsPathName;

    _configuration->getLayout().getBackTab()->addWidget ( WidgetDescription::parameter(attrId,attrLabel,column,span,flags) );
  }


  string  XmlParser::_getAttributeValue ( const char* attributeName )
  {
    char* attributeValue = (char*)xmlTextReaderGetAttribute(_reader,(const xmlChar*)attributeName);
    if ( attributeValue == NULL )
      return string();

    string value ( attributeValue );
    free ( attributeValue );

    return value;
  }


}  // End of anonymous namespace.


namespace Cfg {

  using std::string;
  using std::map;
  using std::ostream;
  using std::ofstream;
  using std::setw;
  using std::vector;


  Configuration* Configuration::_singleton = NULL;


  Configuration* Configuration::get ()
  {
    if ( _singleton == NULL ) _singleton = new Configuration ();
    return _singleton;
  }


  Configuration::Configuration ()
    : _parameters()
    , _layout    (this)
    , _flags     (0)
    , _logSets   ()
  { }


  ConfigurationWidget* Configuration::buildWidget ( unsigned int flags )
  { return _layout.buildWidget(flags); }

  ConfigurationDialog* Configuration::buildDialog()
  { return new ConfigurationDialog(); }


  Parameter* Configuration::getParameter ( const string& name, Parameter::Type type ) const
  {
    map<const string,Parameter*>::const_iterator iparameter = _parameters.find(name);
    if ( iparameter == _parameters.end() ) return NULL;

    if ( type != Parameter::Unknown ) {
      Parameter::Type t1 = (*iparameter).second->getType();
      Parameter::Type t2 = type;

      if ( t1 > t2 ) swap ( t1, t2 );

      if (         (t1 != t2)
         and not ( (t1 == Parameter::Double) and (t2 == Parameter::Percentage) )
         and not ( (t1 == Parameter::Int   ) and (t2 == Parameter::Enumerate ) ) )
        cerr << "[ERROR] Accessing " << Parameter::typeToString((*iparameter).second->getType())
             << " parameter <" << (*iparameter).second->getId()
             << "> as " << Parameter::typeToString(type)<< " (type mismatch)." << endl;
    }

    return (*iparameter).second;
  }


  Parameter* Configuration::addParameter ( const string&   id
                                         , Parameter::Type type
                                         , const string&   value )
  {
    Parameter* p = getParameter ( id );
    if ( p != NULL ) {
    // Update values here.
      return p;
    }

    p = new Parameter ( id, type, value );
    _parameters.insert ( make_pair(id,p) );

    return p;
  }


  const set<Configuration::LogEntry>& Configuration::getLogs ( unsigned int mask ) const
  {
    static set<LogEntry> _failsafe;

    map< unsigned int, set<LogEntry> >::const_iterator ilog = _logSets.find(mask);
    if ( ilog != _logSets.end() ) return (*ilog).second;

    return _failsafe;
  }

  void  Configuration::addLog ( unsigned int mask, const string& id )
  {
    map< unsigned int, set<LogEntry> >::iterator ilog = _logSets.find(mask);
    if ( ilog == _logSets.end() ) {
      _logSets.insert ( make_pair(mask,set<LogEntry>()) );
      ilog = _logSets.find(mask);
    }
    (*ilog).second.insert ( LogEntry(id) );
  }


  void  Configuration::removeLog ( unsigned int mask, const string& id )
  {
    map< unsigned int, set<LogEntry> >::iterator ilog = _logSets.find(mask);
    if ( ilog != _logSets.end() ) (*ilog).second.erase ( id );
  }


  void  Configuration::print ( ostream& out ) const
  {
    map<const string,Parameter*>::const_iterator iparameter = _parameters.begin();
    for ( ; iparameter != _parameters.end(); ++iparameter ) {
      Parameter* p = iparameter->second;
      out << "id:"        << iparameter->first
          << " id:"       << p->getId()
          << " type:"     << p->getType()
          << " asString:" << p->asString();

      out << " asType:";
      switch ( p->getType() ) {
        case Parameter::Unknown:    out << "<unknown>"; break;
        case Parameter::String:     out << "<string>"; break;
        case Parameter::Bool:       out << boolalpha << p->asBool(); break;
        case Parameter::Int:        out << p->asInt(); break;
        case Parameter::Double:     out << p->asDouble(); break;
        case Parameter::Percentage: out << p->asPercentage(); break;
        case Parameter::Enumerate:
          out << endl;

          const vector<Parameter::EnumValue>& values = p->getValues();
          for ( size_t i=0; i != values.size() ; ++i ) {
            out << "  label:" << values[i]._label << " value:" << values[i]._value << endl;
          }
          break;
      }

      out << endl;
    }
  }


  bool  Configuration::writeToFile ( const std::string& fileName, unsigned int flags, const string& tabs ) const
  {
    ofstream out ( fileName.c_str() );
    if ( out.fail() ) return false;

    writeToStream ( out, flags, tabs );

    out.close ();
    return true;
  }


  void  Configuration::writeToStream ( ostream& out, unsigned int flags, const string& tabs ) const
  {
    set<string> tabset;
    tokenize ( tabset, tabs );

    out << "<configuration>" << endl;

    map<const string,Parameter*>::const_iterator iparameter = _parameters.begin();
    for ( ; iparameter != _parameters.end(); ++iparameter ) {
      Parameter* p = iparameter->second;

      string id   = "\"" + p->getId() + "\"";
      string type = "\"" + Parameter::typeToString(p->getType()) + "\"";

      if ( not tabset.empty() ) {
        set<string>::iterator itab = tabset.begin();
        for ( ; itab != tabset.end() ; ++itab ) {
          if ( id.compare(1,(*itab).size(),*itab) == 0 ) {
            break;
          }
        }
        if ( itab == tabset.end() ) continue;
      }

      out << "  <parameter"
          << " id="    << setw(40) << left << id
          << " type="  << setw(12) << left << type
          << " value=\"";

      if ( p->getType() == Parameter::Percentage ) out << p->asPercentageString();
      else out << p->asString();
      out << "\"";

      if ( flags & DriveValues ) {
        if ( p->getType() == Parameter::Int ) {
          if ( p->hasMin() ) out << " min=\"" << p->getMinInt() << "\"";
          if ( p->hasMax() ) out << " max=\"" << p->getMaxInt() << "\"";
        } else if ( p->getType() == Parameter::Double ) {
          if ( p->hasMin() ) out << " min=\"" << p->getMinDouble() << "\"";
          if ( p->hasMax() ) out << " max=\"" << p->getMaxDouble() << "\"";
        }
        if ( p->hasMustExist()   ) out << " mustExist=\"true\"";
        if ( p->hasNeedRestart() ) out << " needRestart=\"true\"";
        if ( p->isFile()         ) out << " isFile=\"true\"";
        if ( p->isPath()         ) out << " isPath=\"true\"";
      }

      if ( (flags&DriveValues) and (p->getType() == Parameter::Enumerate) ) {
        out << ">" << endl;

        const std::vector<Parameter::EnumValue>& values = p->getValues();
        for ( size_t ivalue=0 ; ivalue<values.size() ; ++ivalue ) {
          out << "    <item label=\"" << values[ivalue]._label << "\" "
              <<           "value=\"" << values[ivalue]._value << "\"/>" << endl;
        }

        out << "  </parameter>" << endl;
      } else
        out << "/>" << endl;
    }

    if ( flags & DriveValues ) {
      for ( iparameter = _parameters.begin(); iparameter != _parameters.end(); ++iparameter ) {
        Parameter* p = iparameter->second;

        const vector<string>& slaves = p->getSlaves();
        if ( slaves.empty() ) continue;

        out << "  <group>" << endl;
        out << "    <master id=\"" << p->getId() << "\"/>" << endl;

        for ( size_t islave=0 ; islave<slaves.size() ; ++islave ) {
          out << "    <slave  id=\"" << slaves[islave] << "\"/>" << endl;
        }
        out << "  </group>" << endl;
      }
    }

    if ( flags & DriveLayout ) _layout.writeToStream ( out );

    out << "</configuration>" << endl;
  }


  bool  Configuration::readFromFile ( const std::string& fileName )
  {
    XmlParser parser ( this, fileName );
    return parser.parse ();
  }



}  // End of Cfg namespace.
