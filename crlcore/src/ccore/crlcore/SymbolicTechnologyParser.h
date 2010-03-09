
// -*- C++ -*-


# ifndef  __CRL_SYMBOLIC_TECHNOLOGY_PARSER_H__
#   define  __CRL_SYMBOLIC_TECHNOLOGY_PARSER_H__


class QString;

# include  "crlcore/XmlParser.h"

namespace Hurricane {
  class DataBase;
  class Technology;
  class Layer;
  class BasicLayer;
}


namespace CRL {


  using namespace std;

  using Hurricane::Name;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;


  class SymbolicTechnologyParser : public XmlParser {

    public:
    // Methods.
      static  SymbolicTechnologyParser* create                   ( DataBase* db, QXmlStreamReader* reader );
      static  bool                      load                     ( DataBase* db, const string& path );

    // Internal - enum.
    protected:
      enum  TagSet  { TagsTechnology    = 2
                    , TagsName
                    , TagsBasicLayer
                    , TagsBasicLayers
                    , TagsLayer
                    , TagsSymbolic
                    , TagsPrecision
                    , TagsGridStep
                    , TagsRules
                    , TagsRule
                    , TagSetSize
                    };

      enum RuleType { DoubleValue    = (1<<0)
                    , ValueMask      = DoubleValue
                    , ExtentionCap   = (1<<4) | DoubleValue 
                    , ExtentionWidth = (1<<5) | DoubleValue 
                    , Enclosure      = (1<<6) | DoubleValue 
                    , MinimumWidth   = (1<<7) | DoubleValue 
                    , MinimumSide    = (1<<8) | DoubleValue 
                    , InvalidRule    = (unsigned int)-1
                    };

    // Internal - Attributes.
      static  const QString             UnnamedRuleError;
      static  const QString             UnnamedBasicLayerError;
      static  const QString             UnnamedSymbolicLayerError;
      static  const QString             UnnamedLayerError;
      static  const QString             InvalidRulePathError;
      static  const QString             MissingValueError;
      static  const QString             UnknownRuleError;
      static  const QString             UndefinedLayerError;
      static  const QString             NotACompositeLayerError;
      static  const QString             NotABasicLayerError;
      static  const QString             LayerOutnumber;
      static  const QString             LayerMissingLayer;
              DataBase*                 _dataBase;
              Technology*               _technology;
              BasicLayer*               _basicLayer;
              map<const Name,Layer*>    _layers;
              vector<BasicLayer*>       _layerComponents;

    // Internal - Constructors.
                                        SymbolicTechnologyParser ( DataBase* db, QXmlStreamReader* reader=NULL );
                                        SymbolicTechnologyParser ( const SymbolicTechnologyParser& );
              SymbolicTechnologyParser& operator=                ( const SymbolicTechnologyParser& );
      
    // Internal - Methods.
              void                      parseTechnology          ();
              void                      parseName                ();
              void                      parseBasicLayer          ();
              void                      parseRegularLayer        ();
              void                      parseDiffusionLayer      ();
              void                      parseTransistorLayer     ();
              void                      parseViaLayer            ();
              void                      parseContactLayer        ();
              void                      parseLayer               ();
              void                      parseSymbolic            ();
              void                      parsePrecision           ();
              void                      parseGridStep            ();
              void                      parseRules               ();
              void                      parseRule                ();
              unsigned int              splitRulePath            ( const QString& path
                                                                 , Layer*&        ruleLayer
                                                                 , BasicLayer*&   basicLayer
                                                                 );
              BasicLayer*               getBasicLayer            ( const QString& name, bool ignoreError );
              Layer*                    getLayer                 ( const QString& name, bool ignoreError );
      virtual void                      _postLoad                ();
      virtual const char*               _getMessage              ( MessageId id );

  };


} // End of CRL namespace.


# endif
