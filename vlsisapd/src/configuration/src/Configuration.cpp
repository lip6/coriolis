
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


#include  <sstream>
#include  <iomanip>
#include  <libxml/xmlreader.h>
#include  "vlsisapd/configuration/Configuration.h"
#include  "vlsisapd/configuration/ConfigurationWidget.h"
#include  "vlsisapd/configuration/ParameterWidget.h"


namespace {

  using namespace std;
  using namespace Cfg;


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


  XmlParser::XmlParser ( Configuration*       conf
                       , const string&        fileName )
    : _configuration      (conf)
    , _fileName           (fileName)
    , _reader             (NULL)
    , _status             (1)
    , _tool               ("<none>")
    , _parameter          (NULL)
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
      xmlCleanupParser ();
    }

    return (_status != 0);
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

    string attrId   = _getAttributeValue("id");
    string attrType = _getAttributeValue("type");

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
      _parameter->setString ( _getAttributeValue("value"), false );
    }

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

    _configuration->getLayout().addTab ( new TabDescription(attrName) );

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

    string attrId      = _getAttributeValue("id");
    string attrType    = _getAttributeValue("type");
    string attrLabel   = _getAttributeValue("label");
    string attrColumn  = _getAttributeValue("column");
    string attrSpinBox = _getAttributeValue("spinbox");

    if ( attrId.empty() and attrType.empty() ) {
      cerr << "[ERROR] In <widget> tag, neither \"id\" nor \"type\" attribute." << endl;
      return;
    }

    int column = 0;
    if ( not attrColumn.empty() ) {
      istringstream s ( attrColumn ); s >> column;
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
    if ( attrSpinBox == "true" ) flags |= ParameterWidget::UseSpinBox;

    _configuration->getLayout().getBackTab()->addWidget ( WidgetDescription::parameter(attrId,attrLabel,column,flags) );
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
  using std::setw;


  Configuration* Configuration::_singleton = NULL;


  Configuration* Configuration::get ()
  {
    if ( _singleton == NULL ) _singleton = new Configuration ();
    return _singleton;
  }


  Configuration::Configuration ()
    : _parameters()
    , _layout    (this)
  { }


  ConfigurationWidget* Configuration::buildWidget ()
  { return _layout.buildWidget(); }


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


  void  Configuration::writeToStream ( ostream& out ) const
  {
    out << "<configuration>" << endl;

    map<const string,Parameter*>::const_iterator iparameter = _parameters.begin();
    for ( ; iparameter != _parameters.end(); ++iparameter ) {
      Parameter* p = iparameter->second;

      string id   = "\"" + p->getId() + "\"";
      string type = "\"" + Parameter::typeToString(p->getType()) + "\"";

      out << "  <parameter"
          << " id="    << setw(40) << left << id
          << " type="  << setw(12) << left << type
          << " value=\"" << p->asString() << "\"/>"
          << endl;
    }

    out << "</configuration>" << endl;
  }


  void  Configuration::readFromFile ( const std::string& fileName )
  {
    XmlParser parser ( this, fileName );
    parser.parse ();
  }



}  // End of Cfg namespace.
