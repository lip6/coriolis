
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#ifndef __NIMBUS_ROUTTOOLS_H__
#define __NIMBUS_ROUTTOOLS_H__

#include <set>
#include "nimbus/GCells.h"
#include "nimbus/Fences.h"

namespace Hurricane {
  class Plug;
  class Contact;
  class Segment;
}

namespace Nimbus {

  using namespace std;
  using namespace CRL;
  using namespace Hurricane;

typedef set<Net*> NetSet;
typedef set<Plug*> PlugSet;
typedef set<Hook*> HookSet;
typedef set<Point*> PointSet;
typedef set<Rubber*> RubberSet;
typedef set<Segment*> SegmentSet;
typedef set<Contact*> ContactSet;


// recupere un contact, net et position donnee
//Contact* getContact (Net& net, Point& pos);


// Donne le layer du routage global
Layer* getGRLayer ();

Contact* CheckGetContact (Component* compo);

//void StickSplittersToFences (Cell* cell);

// relie deux contacts par un ou plusieurs segments
//void LRout (Contact* c1, Contact* c2);
//void Link (Contact* c1, Contact* c2);
//bool Linked (Contact* c1, Contact* c2);
ContactFilter getContactIsUnderFilter(const Box& area);
GCellFilter getGCellStepFilter(unsigned step);
FenceFilter getFenceIsInnerOfFilter(const GCell* gcell);
FenceFilter getFenceIsSubFenceOfFilter(const Fence* fence);
GCellFilter getGCellFenceFilter(Fence* fence);

}
#endif
