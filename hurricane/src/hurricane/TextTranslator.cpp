
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TextTranslator.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  "hurricane/TextTranslator.h"


namespace Hurricane {

  using std::string;
  using std::vector;
  using std::cerr;
  using std::endl;


  TextTranslator  TextTranslator::_toTextTranslator;
  

  string  TextTranslator::translate ( const string& source ) const
  {
    string translated = source;

    for ( size_t pos=0 ; pos<translated.size() ; ) {
      bool match = false;

      for ( size_t i=0 ; i<_translations.size() ; ++i ) {
        const string& original    = _translations[i]._original;
        const string& translation = _translations[i]._translation;

        if ( translated.compare(pos,original.size(),original) == 0 ) {
          translated.replace(pos,original.size(),translation);
          pos += translation.size();
          match = true;
          break;
        }
      }

      pos += (match) ? 0 : 1;
    }

    return translated;
  }


  const TextTranslator& TextTranslator::toTextTranslator ()
  {
    if ( _toTextTranslator._translations.empty() ) {
      _toTextTranslator.addTranslation ( "<br>"     , "\n" );
      _toTextTranslator.addTranslation ( "<em>"     , ""   );
      _toTextTranslator.addTranslation ( "</em>"    , ""   );
      _toTextTranslator.addTranslation ( "<strong>" , ""   );
      _toTextTranslator.addTranslation ( "</strong>", ""   );
      _toTextTranslator.addTranslation ( "<tt>"     , ""   );
      _toTextTranslator.addTranslation ( "</tt>"    , ""   );
      _toTextTranslator.addTranslation ( "<b>"      , ""   );
      _toTextTranslator.addTranslation ( "</b>"     , ""   );
      _toTextTranslator.addTranslation ( "<i>"      , ""   );
      _toTextTranslator.addTranslation ( "</i>"     , ""   );
      _toTextTranslator.addTranslation ( "<big>"    , ""   );
      _toTextTranslator.addTranslation ( "</big>"   , ""   );
      _toTextTranslator.addTranslation ( "<small>"  , ""   );
      _toTextTranslator.addTranslation ( "</small>" , ""   );
      _toTextTranslator.addTranslation ( "&lt;"     , "<"  );
      _toTextTranslator.addTranslation ( "&gt;"     , ">"  );
    }

    return _toTextTranslator;
  }


}  // End of Hurricane namespace.
