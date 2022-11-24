#ifndef _KNIK_SLICINGTREE_H
#define _KNIK_SLICINGTREE_H

//#include "nimbus/Nimbus.h"
//#include "nimbus/GCell.h"

#include "knik/SlicingTreeNode.h"

namespace Knik {

    using namespace std;
//    using namespace Nimbus;

    class SlicingTree {
    // **************
        // Attributes
        // **********
        protected:
            SlicingTreeNode* _root;
            unsigned int     _xSize;
            unsigned int     _ySize;
            vector<DbU::Unit>     _xCutCoordinates;
            vector<DbU::Unit>     _yCutCoordinates;

        // Constructors & Destructors
        // **************************
        protected:
            SlicingTree();
            ~SlicingTree();
        public:
            //static SlicingTree* create ( NimbusEngine* nimbus );
            static SlicingTree* create ();
            void destroy();
            //void _postCreate ( NimbusEngine* nimbus );
            void _postCreate ();
            void _preDestroy();

        // Modifiers
        // *********
        public:
            //SlicingTreeNode* createNodeFromGCell      ( GCell* gcell );
            SlicingTreeNode* createNodeFromGCell      ();
            SlicingTreeNode* createNodesFromVertiCuts ( unsigned int xLower, unsigned int xUpper, unsigned int yLower, unsigned int yUpper );
            SlicingTreeNode* createNodesFromHorizCuts ( unsigned int yLower, unsigned int yUpper );

        // Accessors
        // *********
        public:
            unsigned int     getCentralIndex ( unsigned int lower, unsigned int upper );
            Vertex*          getVertex       ( Point position );
            SlicingTreeNode* getTerminalNode ( Point position );

        // Others
        // ******
        public:
            string  _getTypeName() const { return _TName("SlicingTree"); };
            string  _getString() const;
            Record* _getRecord() const;
    };

} // end namespace

#endif
