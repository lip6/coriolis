#ifndef _KNIK_VERTEXES_H
#define _KNIK_VERTEXES_H

#include "hurricane/Collection.h"

namespace Knik {

class Vertex;

// ****************************************************************************************************
// Vertexes declaration
// ****************************************************************************************************
typedef GenericCollection<Vertex*> Vertexes;

// ****************************************************************************************************
// VertexLocator declaration
// ****************************************************************************************************
typedef GenericLocator<Vertex*> VertexLocator;

// ****************************************************************************************************
// VertexFilter declaration
// ****************************************************************************************************
typedef GenericFilter<Vertex*> VertexFilter;

// ****************************************************************************************************
// for_each_vertex declaration
// ****************************************************************************************************

#define for_each_vertex(vertex, vertexes)\
/****************************************************/\
{\
    VertexLocator _locator = vertexes.getLocator();\
    while (_locator.isValid()) {\
        Vertex* vertex = _locator.getElement();\
        _locator.progress();

} // end namespace

#endif
