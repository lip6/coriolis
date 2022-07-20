// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./seabreeze/Node.h"                            |
// +-----------------------------------------------------------------+

#pragma  once
#include <vector>
namespace Hurricane {
  class Contact;
}

namespace Seabreeze {
  
  using Hurricane::Contact;


//----------------------------------------------------------
// Class : Seabreeze::Tree.

  class Node {
    public:
                                        Node     ();
                                        Node     ( Node* parent, Contact* );
                                       ~Node     ();
      inline        double              R        () const;
      inline        double              Rt       () const;
      inline        double              C        () const;
      inline        int                 label    () const;
      inline        int                 ap       () const;
      inline        Contact*            contact  () const;
      inline        Node*               parent   () const;
      inline  const std::vector<Node*>& childs   () const;
      inline        void                addChild ( Node* );
      inline        void                setLabel ( int );
      inline        void                setAp    ( int );
      inline        void                setRt    ( double );
      inline        void                setR     ( double );
      inline        void                setC     ( double );
    private :
      double              _R;
      double              _Rt;
      double              _C;
      Node*               _parent;
      std::vector<Node*>  _childs;
      Contact*            _contact;
      int                 _label;
      int                 _ap;
  };

  
  inline        double              Node::R        () const { return _R; }
  inline        double              Node::Rt       () const { return _Rt; }
  inline        double              Node::C        () const { return _C; }
  inline        int                 Node::label    () const { return _label; }
  inline        int                 Node::ap       () const { return _ap; }
  inline        Contact*            Node::contact  () const { return _contact; }
  inline        Node*               Node::parent   () const { return _parent; }
  inline  const std::vector<Node*>& Node::childs   () const { return _childs; }
  inline        void                Node::addChild ( Node* child ) { _childs.push_back( child ); }
  inline        void                Node::setLabel ( int label )   { _label = label; }
  inline        void                Node::setAp    ( int ap )      { _ap = ap; }
  inline        void                Node::setRt    ( double rt )   { _Rt = rt; }
  inline        void                Node::setR     ( double r  )   { _R = r; }
  inline        void                Node::setC     ( double c  )   { _C = c; }


}  // Seabreeze namespace;
