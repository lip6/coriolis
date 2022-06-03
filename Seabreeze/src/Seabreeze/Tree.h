#pragma once

#include <set>
#include <vector>
#include <iostream>

#include "Node.h"

#include "hurricane/Contact.h"
#include "hurricane/RoutingPad.h"

using namespace std;
using Hurricane::RoutingPad;

class Tree{
  public:
                  Tree          ();
                 ~Tree          ();
    int           get_N         ();
    Node*         get_node      ( Contact* cont );
    vector<Node*> get_node_list ();
    void          new_node      ();
    void          add_node      ( Node* node );
    void          After_i       ( Node* ni );
    set<Node*>    Branch_i      ( Contact* ct );
    int           Delay_Elmore  ( RoutingPad* rp );
    void          print         ( ostream& out );
    void          clear         ();
  private:	
    vector<Node*> nodes;
};
