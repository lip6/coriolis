#ifndef _KNIK_SLICINGTREENODE_H
#define _KNIK_SLICINGTREENODE_H

#include "knik/Vertex.h"

namespace Knik {

    class SlicingTreeNode {
    // ********************
        // Attributes
        // **********
        protected:
            SlicingTreeNode* _previous;

        // Constructor & Destructor
        // ************************
        public:
            SlicingTreeNode() : _previous(NULL) {};
            virtual ~SlicingTreeNode() {};
            virtual void destroy() = 0;

        // Modifiers
        // *********
        public:
                    void setPrevious ( SlicingTreeNode* previous ) { _previous = previous; };
            virtual void setLeft     ( SlicingTreeNode* left )     {};
            virtual void setRight    ( SlicingTreeNode* right )    {};
            virtual void setBottom   ( SlicingTreeNode* bottom )   {};
            virtual void setTop      ( SlicingTreeNode* top )      {};
            virtual void setVertex   ( Vertex* vertex )            {};

        // Accessors
        // *********
        public:
                    SlicingTreeNode* getPrevious() { return _previous; };
            virtual SlicingTreeNode* getLeft()     { return NULL; };
            virtual SlicingTreeNode* getRight()    { return NULL; };
            virtual SlicingTreeNode* getBottom()   { return NULL; };
            virtual SlicingTreeNode* getTop()      { return NULL; };
            virtual Vertex*          getVertex()   { return NULL; };

            virtual DbU::Unit getCutCoordinate() { return 0; };

        // Predicates
        // **********
        public:
            virtual bool isTerminal() const { return false; };

            virtual bool isVertical()   const = 0;
            virtual bool isHorizontal() const = 0;
    };


    class SlicingTreeCutNode : public SlicingTreeNode {
    // ************************************************
        // Attributes
        // **********
        protected:
            DbU::Unit             _cutCoordinate;
            SlicingTreeNode* _left;
            SlicingTreeNode* _right;

        // Constructor & Destructor
        // ************************
        public:
            SlicingTreeCutNode() : SlicingTreeNode(), _left(NULL), _right(NULL) {};
            virtual ~SlicingTreeCutNode() {};
            void destroy() { if ( _left )    _left->destroy();
                            if ( _right )   _right->destroy();
                            delete ( this ); };

        // Modifiers
        // *********
        public:
            void setLeft   ( SlicingTreeNode* left  ) { _left  = left;  };
            void setRight  ( SlicingTreeNode* right ) { _right = right; };
            void setBottom ( SlicingTreeNode* left  ) { _left  = left;  };
            void setTop    ( SlicingTreeNode* right ) { _right = right; };
        
        // Accessors
        // *********
        public:
            DbU::Unit             getCutCoordinate() { return _cutCoordinate; };
            SlicingTreeNode* getLeft()          { return _left; };
            SlicingTreeNode* getRight()         { return _right; };
            SlicingTreeNode* getBottom()        { return _left; };
            SlicingTreeNode* getTop()           { return _right; };

        // Predicates
        // **********
        public:
            virtual bool isVertical()   const = 0;
            virtual bool isHorizontal() const = 0;
    };

    class SlicingTreeHorizCutNode : public SlicingTreeCutNode {
    // ********************************************************
        // Constructor & Destructor
        // ************************
        public:
            SlicingTreeHorizCutNode ( DbU::Unit cutCoordinate ) : SlicingTreeCutNode() { _cutCoordinate = cutCoordinate; };
            ~SlicingTreeHorizCutNode() {};

        // Predicates
        // **********
        public:
            bool isVertical()   const { return false; };
            bool isHorizontal() const { return true; };

        // Others
        // ******
        public:
            string  _getTypeName() const { return _TName("SlicingTreeHorizCutNode"); };
            Record* _getRecord() const { Record* record = new Record ( _getString () );
                                   record->add ( getSlot ( "CutCoordinate", _cutCoordinate ) );
                                   record->add ( getSlot ( "LeftNode"     , _left          ) );
                                   record->add ( getSlot ( "RightNode"    , _right         ) );
                                   record->add ( getSlot ( "Vertical"     , isVertical()   ) );
                                   record->add ( getSlot ( "Horizontal"   , isHorizontal() ) );
                                   record->add ( getSlot ( "Previous"     , _previous      ) );
                                   return record; };
            string  _getString() const { return   "<" + _getTypeName ()
                                                + ":" + getString ( _cutCoordinate )
//                                                + "," + getString ( _left )
//                                                + "," + getString ( _right )
                                                + ">"; };
    };

    class SlicingTreeVertiCutNode : public SlicingTreeCutNode {
    // ********************************************************
        // Constructor & Destructor
        // ************************
        public:
            SlicingTreeVertiCutNode ( DbU::Unit cutCoordinate ) : SlicingTreeCutNode() { _cutCoordinate = cutCoordinate; };
            ~SlicingTreeVertiCutNode() {};

        // Predicates
        // **********
        public:
            bool isVertical()   const { return true; };
            bool isHorizontal() const { return false; };

        // Others
        // ******
        public:
            string  _getTypeName() const { return _TName("SlicingTreeVertiCutNode"); };
            Record* _getRecord() const { Record* record = new Record ( _getString () );
                                   record->add ( getSlot ( "CutCoordinate", _cutCoordinate ) );
                                   record->add ( getSlot ( "LeftNode"     , _left          ) );
                                   record->add ( getSlot ( "RightNode"    , _right         ) );
                                   record->add ( getSlot ( "Vertical"     , isVertical()   ) );
                                   record->add ( getSlot ( "Horizontal"   , isHorizontal() ) );
                                   record->add ( getSlot ( "Previous"     , _previous      ) );
                                   return record; };
            string  _getString() const { return "<" + _getTypeName()
                                                    + ":" + getString ( _cutCoordinate ) 
//                                                  + "," + getString ( _left )
//                                                  + "," + getString ( _right )
                                                    + ">"; };
    };

    class SlicingTreeTerminalNode : public SlicingTreeNode {
    // *****************************************************
        // Attributes
        // **********
        protected:
            Vertex* _vertex;

        // Constructor & Destructor
        // ************************
        public:
            SlicingTreeTerminalNode() : SlicingTreeNode(), _vertex(NULL) {};
            ~SlicingTreeTerminalNode() {};
            void destroy() { delete ( this ); };

        // Modifiers
        // *********
        public:
            void setVertex ( Vertex* vertex ) { _vertex = vertex; };

        // Accessors
        // *********
        public:
            Vertex* getVertex() { return _vertex; };

        // Predicates
        // **********
        public:
            bool isTerminal()   const { return true; };
            bool isVertical()   const { return false; };
            bool isHorizontal() const { return false; };

        // Others
        // ******
        public:
            string  _getTypeName() const { return _TName("SlicingTreeTerminalNode"); };
            Record* _getRecord() const { Record* record = new Record ( _getString () );
                                   record->add ( getSlot ( "Terminal", isTerminal() ) );
                                   record->add ( getSlot ( "Vertex"  , _vertex      ) );
                                   record->add ( getSlot ( "Previous", _previous    ) );
                                   return record; };
            string  _getString() const { return "<" + _getTypeName () 
                                                    + ":" + getString ( _vertex ) 
                                                    + ">"; };
    };
    
} // namespace Knik

INSPECTOR_P_SUPPORT(Knik::SlicingTreeVertiCutNode);
INSPECTOR_P_SUPPORT(Knik::SlicingTreeHorizCutNode);
INSPECTOR_P_SUPPORT(Knik::SlicingTreeTerminalNode);

#endif
