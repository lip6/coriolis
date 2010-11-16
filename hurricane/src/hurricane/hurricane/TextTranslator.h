
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
// |  C++ Header  :       "./TextTranslator.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_TEXT_TRANSLATOR__
#define  __HURRICANE_TEXT_TRANSLATOR__

#include  <string>
#include  <vector>

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


}  // End of Hurricane namespace.


#endif  // __HURRICANE_TEXT_TRANSLATOR__
