
// -*- C++ -*-


# ifndef  __CRL_GRAPHICS_PARSER_H__
#   define  __CRL_GRAPHICS_PARSER_H__


# include  "hurricane/Name.h"
# include  "crlcore/XmlParser.h"

namespace Hurricane {
  class DisplayStyle;
}


namespace CRL {


  using Hurricane::Name;
  using Hurricane::DisplayStyle;


  class GraphicsParser : public XmlParser {

    // Methods.
    public:
      static  GraphicsParser*    create                 ( QXmlStreamReader* reader );
      static  bool               load                   ( const string& path );
      inline  DisplayStyle*      getDefaultDisplayStyle () const;

    // Internal - enum.
    protected:
      enum  TagSet { TagsGraphics      = 2
                   , TagsDisplayStyles = 3
                   , TagsDisplayStyle  = 4
                   , TagsDrawingGroup  = 5
                   };

    // Internal - Attributes.
              DisplayStyle*      _displayStyle;
              DisplayStyle*      _defaultDisplayStyle;
              Name               _drawingGroupName;

    // Internal - Constructors.
                                 GraphicsParser     ( QXmlStreamReader* reader=NULL );
                                 GraphicsParser     ( const GraphicsParser& );
              GraphicsParser&    operator=          ( const GraphicsParser& );
      
    // Internal - Methods.
              void               parseDefault       ();
              void               parseDescription   ();
              void               parseDarkening     ();
              void               parseInherit       ();
              void               parseDrawingStyle  ();
              void               parseDrawingGroup  ();
              void               parseDisplayStyle  ();
              void               parseDisplayStyles ();
              void               parseGraphics      ();
      virtual void               _postLoad          ();
      virtual const char*        _getMessage        ( MessageId id );

  };


  inline DisplayStyle* GraphicsParser::getDefaultDisplayStyle () const
  { return _defaultDisplayStyle; }


} // End of CRL namespace.


# endif
