//  -*- mode: C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/DesignBlob.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_DESIGN_BLOB_H
#define  HURRICANE_DESIGN_BLOB_H

#include "hurricane/Commons.h"


namespace Hurricane {

  class Cell;

// -------------------------------------------------------------------
// Classes  :  "DesignBlob".

  class DesignBlob {
    public:
      inline              DesignBlob   ( Cell* );
      inline Cell*        getTopCell   () const;
             void         toJson       ( JsonWriter* ) const;
      static DesignBlob*  fromJson     ( const std::string& filename );
      inline std::string  _getTypeName () const;
    private:
      Cell* _topCell;
  };


  inline              DesignBlob::DesignBlob   ( Cell* topCell ) : _topCell(topCell) { }
  inline Cell*        DesignBlob::getTopCell   () const { return _topCell; }
  inline std::string  DesignBlob::_getTypeName () const { return "DesignBlob"; }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonDesignBlob".

  class JsonDesignBlob : public JsonObject {
    public:
      static  void            initialize     ();
                              JsonDesignBlob ( unsigned long flags );
      virtual std::string     getTypeName    () const;
      virtual JsonDesignBlob* clone          ( unsigned long ) const;
      virtual void            toData         ( JsonStack& ); 
};


} // Hurricane namespace.

#endif
