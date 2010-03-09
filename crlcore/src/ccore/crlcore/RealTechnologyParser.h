
// -*- C++ -*-


#ifndef  __CRL_REAL_TECHNOLOGY_PARSER_H__
#define  __CRL_REAL_TECHNOLOGY_PARSER_H__


class QString;

#include  "crlcore/XmlParser.h"

namespace Hurricane {
  class DataBase;
  class Technology;
  class Layer;
  class BasicLayer;
}


namespace CRL {


  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;


  class RealTechnologyParser : public XmlParser {

    // Methods.
    public:
      static  RealTechnologyParser* create               ( DataBase* db, QXmlStreamReader* reader );
      static  bool                  load                 ( DataBase* db, const string& path );

    // Internal - enum.
    protected:
      enum  TagSet  { TagsTechnology    = 2
                    , TagsReal
                    , TagsName
                    , TagsLayers
                    , TagsProcessLayer
                    , TagSetSize
                    };

    // Internal - Attributes.
      static  const QString         MissingSymbolicNameError;
      static  const QString         MissingRealNameError;
      static  const QString         InvalidSymbolicNameError;
      static  const QString         MissingGridValueError;
      static  const QString         MissingGridsPerLambdaValueError;
      static  const QString         MissingGridUnitError;
      static  const QString         UnknownGridUnitError;
              DataBase*             _dataBase;
              Technology*           _technology;

    // Internal - Constructors.
                                    RealTechnologyParser ( DataBase* db, QXmlStreamReader* reader=NULL );
                                    RealTechnologyParser ( const RealTechnologyParser& );
              RealTechnologyParser& operator=            ( const RealTechnologyParser& );
      
    // Internal - Methods.
              void                  parseTechnology      ();
              void                  parseReal            ();
              void                  parseName            ();
              void                  parseGrid            ();
              void                  parseGridsPerLambda  ();
              void                  parseLayers          ();
              void                  parseProcessLayer    ();
      virtual void                  _postLoad            ();
      virtual const char*           _getMessage          ( MessageId id );

  };


} // End of CRL namespace.


#endif
