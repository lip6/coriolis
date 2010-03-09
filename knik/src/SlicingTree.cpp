#include "knik/SlicingTree.h"

namespace Knik {

SlicingTree::SlicingTree()
// ***********************
    : _root ( NULL )
    , _xSize ( 0 )
    , _ySize ( 0 )
{
}

//SlicingTree* SlicingTree::create ( NimbusEngine* nimbus )
SlicingTree* SlicingTree::create ()
// ******************************************************
{
    SlicingTree* slicingTree = new SlicingTree();

//    slicingTree->_postCreate ( nimbus );
    slicingTree->_postCreate ();

    return slicingTree;
}

//void SlicingTree::_postCreate ( NimbusEngine* nimbus )
void SlicingTree::_postCreate ()
// ***************************************************
{
//    GCell* lowerLeftGC = nimbus->getGrid()->getLowerLeftCorner( nimbus->getDepth() );
//    GCell* gcell = lowerLeftGC;
//    while ( gcell ) {
//        if ( gcell->getRightFence() )
//            _xCutCoordinates.push_back ( gcell->getXMax() );
//        _xSize++;
//        gcell = gcell->getRightOfMe();
//    }
//    gcell = lowerLeftGC;
//    while ( gcell ) {
//        if ( gcell->getUpFence() )
//            _yCutCoordinates.push_back ( gcell->getYMax() );
//        _ySize++;
//        gcell = gcell->getUpOfMe();
//    }
//    GCell* rootGCell = nimbus->getGrid()->getRoot();
//
//    if ( nimbus->getDepth() == 1 ) {
//        // We've got a regular flat grid
//        if ( _xSize != 0 ) {
//            if ( _ySize != 0 )
//                _root = createNodesFromVertiCuts ( 0, _xCutCoordinates.size()-1, 0, _yCutCoordinates.size()-1 );
//            else
//                _root = createNodesFromVertiCuts ( 0, _xCutCoordinates.size()-1, 0, 0 );
//        }
//        else {
//            if ( _ySize != 0 )
//                _root = createNodesFromHorizCuts ( 0, _yCutCoordinates.size()-1 );
//            else
//                _root = new SlicingTreeTerminalNode();
//        }
//        _root->setPrevious ( NULL );
//    }
//    else {
//        // We've got a (maybe unregular) hierarcic grid
//        _root = createNodeFromGCell ( rootGCell );
//        _root->setPrevious ( NULL );
//
//        //cerr << _root << endl;
//    }
}

SlicingTree::~SlicingTree()
// ************************
{
}

void SlicingTree::destroy()
// ***********************
{
    _preDestroy();
    delete ( this );
}

void SlicingTree::_preDestroy()
// ***************************
{
    // on doit "deleter" le _root (SlicingTreeNode) qui lui même doit "deleter" tous ses fils et recursivement jusqu'aux feuilles (SlicingTreeTerminal)
    if ( _root )
        _root->destroy();
}

//SlicingTreeNode* SlicingTree::createNodeFromGCell ( GCell* gcell )
SlicingTreeNode* SlicingTree::createNodeFromGCell ( )
// ***************************************************************
{
//    if ( !gcell->isARoutingLeaf() ) {
//        DbU::Unit xCut = gcell->getSubUpperRight()->getXMin();
//        DbU::Unit yCut = gcell->getSubUpperRight()->getYMin();
//        SlicingTreeVertiCutNode* vertiCutNode      = new SlicingTreeVertiCutNode ( xCut );
//        SlicingTreeHorizCutNode* leftHorizCutNode  = new SlicingTreeHorizCutNode ( yCut );
//        SlicingTreeHorizCutNode* rightHorizCutNode = new SlicingTreeHorizCutNode ( yCut );
//        vertiCutNode->setLeft  ( leftHorizCutNode  );
//        vertiCutNode->setRight ( rightHorizCutNode );
//        leftHorizCutNode->setPrevious  ( vertiCutNode );
//        rightHorizCutNode->setPrevious ( vertiCutNode );
//
//        SlicingTreeNode* subNode = createNodeFromGCell ( gcell->getSubBottomLeft() );
//        leftHorizCutNode->setBottom ( subNode );
//        subNode->setPrevious ( leftHorizCutNode );
//
//        subNode = createNodeFromGCell ( gcell->getSubUpperLeft() );
//        leftHorizCutNode->setTop ( subNode );
//        subNode->setPrevious( leftHorizCutNode );
//
//        subNode =  createNodeFromGCell ( gcell->getSubBottomRight() );
//        rightHorizCutNode->setBottom ( subNode );
//        subNode->setPrevious ( rightHorizCutNode );
//
//        subNode = createNodeFromGCell ( gcell->getSubUpperRight() );
//        rightHorizCutNode->setTop ( subNode );
//        subNode->setPrevious ( rightHorizCutNode );
//
//        return vertiCutNode;
//    }
//    else {
//        SlicingTreeTerminalNode* terminalNode = new SlicingTreeTerminalNode();
//        return terminalNode;
//    }
    return NULL;
}

SlicingTreeNode* SlicingTree::createNodesFromVertiCuts ( unsigned int xLower, unsigned int xUpper, unsigned int yLower, unsigned int yUpper )
// ******************************************************************************************************************************************
{
    SlicingTreeNode* node;
    unsigned int xCutIndex = getCentralIndex ( xLower, xUpper );
    node = new SlicingTreeVertiCutNode ( _xCutCoordinates[xCutIndex] );
    
    SlicingTreeNode* leftNode;
    if ( xLower < xCutIndex )
        leftNode  = createNodesFromVertiCuts ( xLower, xCutIndex, yLower, yUpper );
    else {
        if ( yLower != yUpper )
            leftNode = createNodesFromHorizCuts ( yLower, yUpper );
        else
            leftNode = new SlicingTreeTerminalNode();
    }

    SlicingTreeNode* rightNode;
    if ( (xUpper > (xCutIndex + 1)) || ((xCutIndex + 1) == (_xCutCoordinates.size() - 1)) )
        rightNode = createNodesFromVertiCuts ( xCutIndex+1, xUpper, yLower, yUpper );
    else {
        if ( yLower != yUpper )
            rightNode = createNodesFromHorizCuts ( yLower, yUpper );
        else
            rightNode = new SlicingTreeTerminalNode();
    }
    leftNode->setPrevious  ( node );
    rightNode->setPrevious ( node );
    node->setLeft  ( leftNode );
    node->setRight ( rightNode );

    return node;
}

SlicingTreeNode* SlicingTree::createNodesFromHorizCuts ( unsigned int yLower, unsigned int yUpper )
// ************************************************************************************************
{
    SlicingTreeNode* node;
    unsigned int yCutIndex = getCentralIndex ( yLower, yUpper );
    node = new SlicingTreeHorizCutNode ( _yCutCoordinates[yCutIndex] );

    SlicingTreeNode* bottomNode;
    if ( yLower < yCutIndex )
        bottomNode = createNodesFromHorizCuts ( yLower, yCutIndex );
    else
        bottomNode = new SlicingTreeTerminalNode();

    SlicingTreeNode* topNode;
    if ( (yUpper > (yCutIndex + 1)) || ((yCutIndex + 1) == (_yCutCoordinates.size() - 1)) )
        topNode    = createNodesFromHorizCuts ( yCutIndex+1, yUpper );
    else
        topNode    = new SlicingTreeTerminalNode();

    bottomNode->setPrevious ( node );
    topNode->setPrevious    ( node );
    node->setBottom ( bottomNode );
    node->setTop    ( topNode );

    return node;
}

unsigned int SlicingTree::getCentralIndex ( unsigned int lower, unsigned int upper )
// *********************************************************************************
{
    return (lower + upper) / 2;
}

Vertex* SlicingTree::getVertex ( Point position )
// **********************************************
{
    assert ( _root );
    if ( _root->isTerminal() )
        return _root->getVertex();

    SlicingTreeCutNode* slicingTreeCutNode = dynamic_cast<SlicingTreeCutNode*>(_root);
    SlicingTreeNode*    slicingTreeNode = NULL;

    while ( slicingTreeCutNode ) {
        if ( slicingTreeCutNode->isVertical() ) {
            if ( position.getX() < slicingTreeCutNode->getCutCoordinate() )
                slicingTreeNode = slicingTreeCutNode->getLeft();
            else
                slicingTreeNode = slicingTreeCutNode->getRight();
        }
        else {
            if ( position.getY() < slicingTreeCutNode->getCutCoordinate() )
                slicingTreeNode = slicingTreeCutNode->getBottom();
            else
                slicingTreeNode = slicingTreeCutNode->getTop();
        }
        if ( slicingTreeNode->isTerminal() )
            break;

        slicingTreeCutNode = dynamic_cast<SlicingTreeCutNode*>(slicingTreeNode);
    }

    /// verif
    assert ( slicingTreeNode );
    return slicingTreeNode->getVertex();
}

SlicingTreeNode* SlicingTree::getTerminalNode ( Point position )
// *************************************************************
{
    assert ( _root );
    if ( _root->isTerminal() )
        return _root;

    SlicingTreeCutNode* slicingTreeCutNode = dynamic_cast<SlicingTreeCutNode*>(_root);
    SlicingTreeNode*    slicingTreeNode = NULL;

    while ( slicingTreeCutNode ) {
        if ( slicingTreeCutNode->isVertical() ) {
            if ( position.getX() < slicingTreeCutNode->getCutCoordinate() )
                slicingTreeNode = slicingTreeCutNode->getLeft();
            else
                slicingTreeNode = slicingTreeCutNode->getRight();
        }
        else {
            if ( position.getY() < slicingTreeCutNode->getCutCoordinate() )
                slicingTreeNode = slicingTreeCutNode->getBottom();
            else
                slicingTreeNode = slicingTreeCutNode->getTop();
        }
        if ( slicingTreeNode->isTerminal() )
            break;

        slicingTreeCutNode = dynamic_cast<SlicingTreeCutNode*>(slicingTreeNode);
    }

    /// verif
    assert ( slicingTreeNode );
    return slicingTreeNode;
}

Record* SlicingTree::_getRecord() const
// ******************************
{
    Record* record = new Record ( getString ( this ) );

    record->add ( getSlot ( "Root", _root ) );

    return record;
}

string SlicingTree::_getString() const
// ***********************************
{
    return   "<" + _TName ( "SlicingTree" )
           + ":" + getString ( _root )
           + ">";
}

} // end namespace
