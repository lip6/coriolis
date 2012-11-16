
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./vlsisapd/openChams/Layout.h"                 |
// +-----------------------------------------------------------------+


#ifndef __OPENCHAMS_LAYOUT_H__
#define __OPENCHAMS_LAYOUT_H__

#include <map>
#include <string>


namespace OpenChams {

  class Circuit;
  class Node;


  class Layout {
	public:
                   Layout        (Circuit*);
             void  addInstance   (const std::string& name, const std::string& style);
      inline bool  hasNoInstance ();
      inline Node* getHBTreeRoot ();
      inline void  setHBTreeRoot (Node*);
      inline const std::map<std::string, std::string>& getInstances();
    private:
      Circuit* _circuit;
      Node*    _hbTreeRoot;
      std::map<std::string, std::string> _instances; // device name <-> style (name)
  };
    

  inline bool  Layout::hasNoInstance () { return _instances.empty(); };
  inline Node* Layout::getHBTreeRoot () { return _hbTreeRoot; }
  inline void  Layout::setHBTreeRoot (Node* root) { _hbTreeRoot = root; }
  inline const std::map<std::string, std::string>& Layout::getInstances() { return _instances; }


} // namespace

#endif

