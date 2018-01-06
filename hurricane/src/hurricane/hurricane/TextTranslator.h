// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/TextTranslator.h"             |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_TEXT_TRANSLATOR_H
#define HURRICANE_TEXT_TRANSLATOR_H

#include <string>
#include <vector>

namespace Hurricane {


  class TextTranslator {
    public:
      static const TextTranslator& toTextTranslator ();
      inline                       TextTranslator   ();
      inline void                  addTranslation   ( const std::string& original, const std::string& translation );
             std::string           translate        ( const std::string& source ) const;
    private:
      class Translation {
        public:
          inline Translation ( const std::string& original, const std::string& translation );
        public:
          std::string  _original;
          std::string  _translation;
      };
    private:
      static TextTranslator     _toTextTranslator;
      std::vector<Translation>  _translations;
  };


// Inline Methods.
  inline TextTranslator::Translation::Translation ( const std::string& original
                                                  , const std::string& translation )
    : _original   (original)
    , _translation(translation)
  { }

  inline TextTranslator::TextTranslator () : _translations() {}

  inline void  TextTranslator::addTranslation ( const std::string& original
                                              , const std::string& translation )
  {
    _translations.push_back ( Translation(original,translation) );
  }


}  // Hurricane namespace.


#endif  // HURRICANE_TEXT_TRANSLATOR_H
