
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//   Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//   Hugo Clément                   <Hugo.Clement@lip6.fr>
//   Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//   Christian Masson           <Christian.Masson@lip6.fr>
// 
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
// 
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
// 
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
// Date   : 29/01/2004
// Author : Christophe Alexandre  <Christophe.Alexandre@lip6.fr>
//
// Authors-Tag 


#ifndef __METIS_ENGINE_H__
#define __METIS_ENGINE_H__

#include "hurricane/Cell.h"
#include "crlcore/ToolEngine.h"
#include "crlcore/ToolEngine.h"

#include "metis/Configuration.h"


namespace Metis {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::Cell;

  class MetisGraph;

  class MetisEngine: public CRL::ToolEngine
  {
    public:
    // Types.
      typedef ToolEngine                Inherit;
      typedef std::vector<MetisGraph*>  MetisGraphs;
    public:
    // Constructor.
      static MetisEngine*    create                            ( Cell* );
      static MetisEngine*    get                               ( Cell* );
    // Methods.                                                
      static  unsigned int   computeQuadriPartitions           ( Cell*, int& xsplits, int& ysplits );
      static  void           doQuadriPart                      ( Cell* );
      static  bool           isHMetisCapable                   ();
      static  const Name&    staticGetName                     ();
      virtual const Name&    getName                           () const;
                    int      getGlobalEdgeCut                  () const; 
      inline  Configuration* getConfiguration                  ();
      inline  Configuration::RefreshCb_t&                      
                             getRefreshCb                      ();
      inline  bool           getPartOrKWayHMetis               () const;
      inline  unsigned int   getNumberOfInstancesStopCriterion () const;
      inline  int            getGlobalConnectionsWeightRatio   () const;
      inline  int            getUbFactor                       () const;
      inline  int*           getHMetisOptions                  ();
      inline  int            getHMetisOption                   ( Configuration::MetisOption );
      inline  void           setHMetisOption                   ( Configuration::MetisOption, int value );
              void           run                               ();
              void           save                              ( unsigned step );
      inline  void           setRefreshCb                      ( Configuration::RefreshCb_t );
      virtual std::string    _getTypeName                      () const {return "Metis::MetisEngine";};
      virtual Record*        _getRecord                        () const;

    private:
    // Attributes.
      static const Name     _toolName;
             Configuration* _configuration;
             unsigned       _step;
             MetisGraphs*   _actualGraphs;
             MetisGraphs*   _newGraphs;
             int            _globalEdgeCut;
    private:
    // Internals.
                    MetisEngine ( Cell* );
      virtual      ~MetisEngine ();
      virtual void  _preDestroy ();
              bool  _reInit     ();
  };


// Inline Methods.
  inline  Configuration*              MetisEngine::getConfiguration                  () { return _configuration; }
  inline  Configuration::RefreshCb_t& MetisEngine::getRefreshCb                      () { return _configuration->getRefreshCb(); }
  inline  bool                        MetisEngine::getPartOrKWayHMetis               () const { return _configuration->getPartOrKWayHMetis(); }
  inline  unsigned int                MetisEngine::getNumberOfInstancesStopCriterion () const { return _configuration->getNumberOfInstancesStopCriterion(); }
  inline  int                         MetisEngine::getGlobalConnectionsWeightRatio   () const { return _configuration->getGlobalConnectionsWeightRatio(); }
  inline  int                         MetisEngine::getUbFactor                       () const { return _configuration->getUbFactor(); }
  inline  int*                        MetisEngine::getHMetisOptions                  () { return _configuration->getHMetisOptions(); }
  inline  int                         MetisEngine::getHMetisOption                   ( Configuration::MetisOption option ) { return _configuration->getHMetisOption(option); }
  inline  void                        MetisEngine::setRefreshCb                      ( Configuration::RefreshCb_t cb ) { _configuration->setRefreshCb(cb); }
  inline  void                        MetisEngine::setHMetisOption                   ( Configuration::MetisOption option, int value ) { _configuration->setHMetisOption(option,value); }


}  // End of Metis namespace.


#endif // __METIS_ENGINE_H__
