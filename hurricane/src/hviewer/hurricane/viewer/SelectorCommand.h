
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./SelectorCommand.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_SELECTOR_COMMAND__
#define  __HURRICANE_SELECTOR_COMMAND__


#include  <string>
#include  "hurricane/Commons.h"


namespace Hurricane {


  using std::string;
  class CellWidget;


  class SelectorCommand {
    public:
      virtual         ~SelectorCommand ();
      virtual void     doSelection     ( CellWidget*, bool delayRedraw ) = 0;
      virtual Record*  _getRecord      () const = 0;
      virtual string   _getString      () const = 0;
      virtual string   _getTypeName    () const = 0;
  };


  class NetSelectorCommand : public SelectorCommand {
    public:
                         NetSelectorCommand ( const Net* );
      virtual           ~NetSelectorCommand ();
              const Net* getNet             () const;
      virtual void       doSelection        ( CellWidget*, bool delayRedraw );
      virtual Record*    _getRecord         () const;
      virtual string     _getString         () const;
      virtual string     _getTypeName       () const;
    protected:
      const Net* _net;
  };


  class AreaSelectorCommand : public SelectorCommand {
    public:
                         AreaSelectorCommand ( const Box& );
      virtual           ~AreaSelectorCommand ();
              const Box& getArea             () const;
      virtual void       doSelection         ( CellWidget*, bool delayRedraw );
      virtual Record*    _getRecord          () const;
      virtual string     _getString          () const;
      virtual string     _getTypeName        () const;
    protected:
      const Box  _area;
  };


}


INSPECTOR_P_SUPPORT(Hurricane::SelectorCommand);


#endif  // __HURRICANE_SELECTOR_COMMAND__
