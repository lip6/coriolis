
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2011-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Node.h"                   |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_NODE_H__
#define __OPENCHAMS_NODE_H__

#include <string>


namespace OpenChams {


  class Node {
    public:
      enum Position { NONE=0, RIGHT=1, TOP=2 };
	public:
      inline const std::string& getName     () const;
      inline Position           getPosition () const;
      inline Node*              getParent   ();
      inline Node*              getRight    ();
      inline Node*              getTop      ();
      inline bool               isRoot      ();
      inline void               setRight    (Node*);
      inline void               setTop      (Node*);
	protected:
                                Node        (const std::string& nodeName, Position pos, Node* parent);
      virtual                  ~Node        () {};
    private:
      std::string  _name;
      Position     _position;
      Node*        _parent;
      Node*        _right;
      Node*        _top;
  };


  inline const std::string& Node::getName    () const { return _name;     }
  inline Node::Position     Node::getPosition() const { return _position; }
  inline Node*              Node::getParent  () { return _parent;   }
  inline Node*              Node::getRight   () { return _right;    }
  inline Node*              Node::getTop     () { return _top;      }
  inline bool               Node::isRoot     () { return _parent == NULL; }
  inline void               Node::setRight   (Node* right) { _right = right; }
  inline void               Node::setTop     (Node* top) { _top = top; }


  class Bloc : public Node {
    public:
      Bloc(const std::string& blocName, Position pos=Node::NONE, Node* parent=NULL);
  };


  class Group : public Node {
    public:
      enum Align { NONE=0 , VERTICAL=1 , HORIZONTAL=2 };
    public:
                   Group       (const std::string& groupName, Position pos=Node::NONE, Node* parent=NULL);
      inline void  setRootNode (Node*);
      inline void  setIsolated (bool);
      inline void  setPaired   (bool);
      inline void  setAlign    (Align);
      inline Node* getRootNode ();
      inline bool  isIsolated  ();
      inline bool  isPaired    ();
      inline Align getAlign    ();
    private:
      Node* _root;
      bool  _isolated;
      bool  _paired;
      Align _align;
  };


  inline void         Group::setRootNode(Node* root) { _root = root; }
  inline void         Group::setIsolated(bool isolated) { _isolated = isolated; }
  inline void         Group::setPaired  (bool paired) { _paired = paired; }
  inline void         Group::setAlign   (Group::Align align) { _align = align; }
  inline Node*        Group::getRootNode() { return _root; }
  inline bool         Group::isIsolated () { return _isolated; }
  inline bool         Group::isPaired   () { return _paired; }
  inline Group::Align Group::getAlign   () { return _align; }


} // OpenChams namespace.

#endif

