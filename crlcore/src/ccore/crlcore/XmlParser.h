
// -*- C++ -*-


# ifndef  __CRL_XML_PARSER_H__
#   define  __CRL_XML_PARSER_H__


# include  <iostream>
# include  <vector>

# include  <QString>

class QXmlStreamReader;


namespace CRL {


  using namespace std;


  class XmlParser {

    // Internal - Subclass: TagEntry.
    protected:
      typedef void  (XmlParser::* tagParser_t)();

      struct TagEntry {
                 const char*  _name;
                 tagParser_t  _parser;

          inline              TagEntry ( const char* name, tagParser_t parser );
      };

    // Internal - Subclass: TagTable.
      enum  BasicTagSet { TagsNoChilds      = 0
                        , TagsStandAlone    = 1
                        };
      class TagsTable : public vector<TagEntry> { };

    // Internal - MessageId.
      enum MessageId { OpenFile = 0 };

    // Internal - Attributes.
             vector<TagsTable>  _tagsTables;
             QXmlStreamReader*  _reader;

    // Internal - Constructors & destructors.
                                XmlParser        ( QXmlStreamReader* reader, size_t tagsTablesSize );
                                XmlParser        ( const XmlParser& );
      virtual                  ~XmlParser        ();
              XmlParser&        operator=        ( const XmlParser& );
      
    // Internal - Methods.
              void              addTagEntry      ( int tagSet, const char* tag, tagParser_t tagParser );
              void              parseTags        ( int tagSet );
              void              parseUnknownTag  ();
              void              parseStandAlone  ();
      inline  void              parseNoChilds    ();
    //        const char*       readTextAsAscii  ();
              QString           readTextAsString ();
              unsigned int      readTextAsUInt   ();
              long              readTextAsLong   ();
              double            readTextAsDouble ();
              bool              _load            ( const string& path, bool warnNotFound=true );
      virtual void              _postLoad        () = 0;
      virtual const char*       _getMessage      ( MessageId id ) = 0;
      virtual void              printError       ( const QString& error, ostream& o=cerr );

  };


  // Inline Functions.
  inline XmlParser::TagEntry::TagEntry ( const char*            name
                                       , XmlParser::tagParser_t parser
                                       ) : _name(name)
                                         , _parser(parser)
  { }


  inline  void  XmlParser::parseNoChilds () { parseTags(TagsNoChilds); }


} // End of CRL namespace.


# endif
