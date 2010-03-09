
// -*- C++ -*-


# include  <cassert>
# include  <string>

# include  <QFile>

# include  <QXmlStreamReader>

# include  "crlcore/XmlParser.h"




namespace CRL {


  XmlParser::XmlParser ( QXmlStreamReader* reader, size_t tagsTablesSize )
    : _tagsTables(tagsTablesSize)
    , _reader(reader)
  {
  }


  XmlParser::~XmlParser ()
  {
  }


  bool  XmlParser::_load ( const string& path, bool warnNotFound )
  {
    QFile  file ( path.c_str() );
    if ( !file.open(QFile::ReadOnly|QFile::Text) ) {
      if ( warnNotFound ) {
        cerr << "[ERROR] Cannot open " << _getMessage(OpenFile) << " file:" << endl;
        cerr << "        \"" << path << "\"." << endl;
      }
      return false;
    }

    QXmlStreamReader  reader ( &file );
    _reader = &reader;

    parseStandAlone ();

    file.close ();
    if ( reader.hasError() ) {
      cerr << "[ERROR] Syntax error " << _getMessage(OpenFile) << " in file:" << endl;
      cerr << "          \"" << path << "\"." << endl;
      cerr << "        (" << qPrintable(reader.errorString()) << ", line:" 
           << reader.lineNumber() << ")" << endl;
      return false;
    }

    _postLoad ();

    return true;
  }


  void  XmlParser::addTagEntry ( int tagSet, const char* tag, tagParser_t tagParser )
  {
    if ( (size_t)tagSet >= _tagsTables.size() ) {
      cerr << "[ERROR] tagSet id " << tagSet << " is not within tag table range." << endl;
      cerr << "        (disabling management of tag: " << tag << ")" << endl;
      return;
    }

    _tagsTables[tagSet].push_back ( TagEntry(tag,tagParser) );
  }


  void  XmlParser::parseTags ( int tagSet )
  {
    assert ( (size_t)tagSet < _tagsTables.size() );
    
    TagsTable& tags = _tagsTables[tagSet];

    _reader->readNext ();
    while ( !_reader->atEnd() ) {
      if ( _reader->isEndElement() ) {
        _reader->readNext();
        break;
      }

      if ( _reader->isStartElement() ) {
        size_t entry = 0;
        for ( ; entry<tags.size() ; entry++ ) {
          if ( _reader->name() == tags[entry]._name ) {
            (this->*(tags[entry]._parser)) ();
            break;
          }
        }
        if ( entry >= tags.size() ) {
//        if ( tags.size() == 1 ) {
//          _reader->raiseError ( QString("missing <%1> tag").arg(tags[0]._name) );
//        }
          parseUnknownTag ();
        }
      } else
        _reader->readNext ();
    }
  }


  void  XmlParser::parseUnknownTag ()
  {
    cerr << "[WARNING] Skipping unknown tag: <"
         << qPrintable(_reader->name().toString()) << ">." << endl;

    _reader->readNext ();
    while ( !_reader->atEnd() ) {
      if ( _reader->isEndElement() ) {
        _reader->readNext();
        break;
      }

      if ( _reader->isStartElement() ) {
        parseUnknownTag ();
      } else
        _reader->readNext ();
    }
  }


  void  XmlParser::parseStandAlone ()
  {
    parseTags ( TagsStandAlone );
  }


//   const char* XmlParser::readTextAsAscii ()
//   {
//     const char* value = _reader->readElementText().toStdString().c_str();
//     if ( _reader->isEndElement() ) _reader->readNext ();

//     return value;
//   }


  QString  XmlParser::readTextAsString ()
  {
    QString value = _reader->readElementText();
    if ( _reader->isEndElement() ) _reader->readNext ();

    return value;
  }


  unsigned int  XmlParser::readTextAsUInt ()
  {
    unsigned int value = _reader->readElementText().toUInt();
    if ( _reader->isEndElement() ) _reader->readNext ();

    return value;
  }


  long  XmlParser::readTextAsLong ()
  {
    long value = _reader->readElementText().toLong();
    if ( _reader->isEndElement() ) _reader->readNext ();

    return value;
  }


  double  XmlParser::readTextAsDouble ()
  {
    double value = _reader->readElementText().toDouble();
    if ( _reader->isEndElement() ) _reader->readNext ();

    return value;
  }


  void  XmlParser::printError ( const QString& error, ostream& o )
  {
    o << "[ERROR] " << qPrintable(error) << endl;

    QFile*   file     = dynamic_cast<QFile*> ( _reader->device() );
    QString  fileName = "<not a file>";
    if ( file )
      fileName = file->fileName();

    o << "        (line: " << _reader->lineNumber()
      << ", file: " << qPrintable(fileName) << ")" << endl;
  }


} // End of CRL namespace.
