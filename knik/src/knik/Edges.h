#ifndef _KNIK_EDGES_H
#define _KNIK_EDGES_H

#include "hurricane/Collection.h"

namespace Knik {

  using namespace Hurricane;

class Edge;

// ****************************************************************************************************
// Edges declaration
// ****************************************************************************************************
typedef GenericCollection<Edge*> Edges;

// ****************************************************************************************************
// EdgeLocator declaration
// ****************************************************************************************************
typedef GenericLocator<Edge*> EdgeLocator;

// ****************************************************************************************************
// EdgeFilter declaration
// ****************************************************************************************************
typedef GenericFilter<Edge*> EdgeFilter;

// ****************************************************************************************************
// for_each_edges declaration
// ****************************************************************************************************

#define for_each_edge(edge, edges)\
/****************************************************/\
{\
    EdgeLocator _locator = edges.getLocator();\
    while (_locator.isValid()) {\
        Edge* edge = _locator.getElement();\
        _locator.progress();

} // end namespace

#endif
