
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


#ifndef __METISGRAPH_H
#define	__METISGRAPH_H

#ifdef HAVE_HMETIS_LIB

#include "metis/MetisEngine.h"

namespace Metis {

  class MetisGraph
  {
      friend class MetisEngine;
    public:
      typedef set<Occurrence> OccurrenceSet;
      typedef pair<GCell*, OccurrenceSet> PartResult;
      typedef vector<PartResult*> PartResultVector;
    public:
      class TooLowNVTXSException
      {
        public:
          int _nvtxs;
        public:
              TooLowNVTXSException(int nvtxs): _nvtxs(nvtxs) {}
      };
    private:
      MetisEngine*      _metis;
      Cell*             _cell;
      GCell*            _gcell;                          // Placement zone
      OccurrenceSet     _toPlaceInstanceOccurrencesSet;
      OccurrenceSet     _rootNetOccurrencesSet;
      PartResultVector  _partResultVector;
      int               _edgeCut;                        // EdgeCut from last part 
    public:
           MetisGraph ( MetisEngine*, GCell* box);
           MetisGraph ( MetisEngine*   metis
                      , MetisGraph*    previous
                      , GCell*         gcell
                      , OccurrenceSet& toplaceinstanceoccurrences);
          ~MetisGraph ();
      int  part       ( linefill& );
  };

}  // End of Metis namespace.


#endif /* HAVE_HMETIS_LIB */

#endif /* __METISGRAPH_H */
