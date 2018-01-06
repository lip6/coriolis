// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/GraphicToolEngine.h"                 |
// +-----------------------------------------------------------------+


#ifndef CRL_GRAPHIC_TOOL_H
#define CRL_GRAPHIC_TOOL_H

#include  <string>
#include  <map>
#include  <functional>
#include  <QObject>

class QMenu;

#include  "hurricane/Error.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/ExceptionWidget.h"


namespace Hurricane {
  class Name;
  class Cell;
  class CellViewer;
}


namespace CRL {

  using std::string;
  using std::map;
  using Hurricane::Error;
  using Hurricane::Exception;
  using Hurricane::Name;
  using Hurricane::Cell;
  using Hurricane::CellWidget;
  using Hurricane::CellViewer;
  using Hurricane::ExceptionWidget;
    

// -------------------------------------------------------------------
// Class  :  "CRL::GraphicTool".

  class GraphicTool : public QObject {
      Q_OBJECT;

    public:
      class DrawGoFunctions {
        private:
                 CellWidget::InitExtensionGo_t* _init;
                 CellWidget::DrawExtensionGo_t* _draw;
        public:
          inline                                DrawGoFunctions ( CellWidget::InitExtensionGo_t*
                                                                , CellWidget::DrawExtensionGo_t*
                                                                );
          inline CellWidget::DrawExtensionGo_t* getDraw         () const;
          inline CellWidget::InitExtensionGo_t* getInit         () const;
      };
    public:
      typedef map<Name,DrawGoFunctions> DrawGoMap;
    public:
              void               addDrawGo           ( const Name&
                                                     , CellWidget::InitExtensionGo_t*
                                                     , CellWidget::DrawExtensionGo_t*
                                                     );
              DrawGoFunctions*   getDrawGo           ( const Name& );
      inline  const DrawGoMap&   getDrawGos          () const;
      virtual void               addToMenu           ( CellViewer* ) = 0;
      virtual const Name&        getName             () const = 0;
      virtual size_t             release             () = 0;
    protected:
      map<Name,DrawGoFunctions>  _drawGoMap;
    protected:
                                 GraphicTool     ();
      virtual                   ~GraphicTool     ();
    protected:
  };


// Inline Functions.
  inline GraphicTool::DrawGoFunctions::DrawGoFunctions ( CellWidget::InitExtensionGo_t* init
                                                       , CellWidget::DrawExtensionGo_t* draw
                                                       )
    : _init(init)
    , _draw(draw)
  { }

  inline CellWidget::DrawExtensionGo_t* GraphicTool::DrawGoFunctions::getDraw () const
  { return _draw; }

  inline CellWidget::InitExtensionGo_t* GraphicTool::DrawGoFunctions::getInit () const
  { return _init; }

  inline  const GraphicTool::DrawGoMap& GraphicTool::getDrawGos () const
  { return _drawGoMap; }


} // CRL namespace.

#endif // CRL_GRAPHIC_TOOL_H
