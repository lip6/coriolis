#pragma once	// ensures that this header file is only included once

#include <vector>

#include "hurricane/Contact.h"

using Hurricane::Contact;

class Node {
  public :
    double             R;
    double             Rt;
    double             C;
    Node*              Np;
    std::vector<Node*> Ne;
    Contact*           _contact;
    int                label;
    int                ap;
  public:
    Node  ();
    Node  ( Node* parent, Contact* ct );
   ~Node  ();
};
