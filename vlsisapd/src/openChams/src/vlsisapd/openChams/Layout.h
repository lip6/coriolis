/*
 *  Layout.h
 *  openChams
 *
 *  Created by damien dupuis on 31/08/10.
 *  Copyright 2008-2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_LAYOUT_H__
#define __OPENCHAMS_LAYOUT_H__

#include <map>

namespace OpenChams {
class Name;
class Circuit;
class Node;

class Layout {
	public:
        Layout(Circuit*);
        
        void addInstance(Name name, Name style);
        
        inline bool  hasNoInstance();
        inline const std::map<Name, Name>& getInstances();

        inline Node* getHBTreeRoot();
        inline void  setHBTreeRoot(Node*);
        
    private:
        Circuit* _circuit;
        Node*    _hbTreeRoot;
        std::map<Name, Name> _instances; // device name <-> style (name)
};
    
inline bool   Layout::hasNoInstance() { return (_instances.size() == 0) ? true : false; };
inline const  std::map<Name, Name>& Layout::getInstances() { return _instances; };

inline Node* Layout::getHBTreeRoot() { return _hbTreeRoot; }
inline void  Layout::setHBTreeRoot(Node* root) { _hbTreeRoot = root; }

} // namespace
#endif

