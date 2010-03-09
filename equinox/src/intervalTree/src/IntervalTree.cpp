#include <stdio.h>
#include <math.h>

// If the symbol CHECK_INTERVAL_TREE_ASSUMPTIONS is defined then the
// code does a lot of extra checking to make sure certain assumptions
// are satisfied.  This only needs to be done if you suspect bugs are
// present or if you make significant changes and want to make sure
// your changes didn't mess anything up.
// #define CHECK_INTERVAL_TREE_ASSUMPTIONS 1

using namespace std;

#include "equinox/IntervalTree.h"

namespace Equinox { 

//long MIN_LONG=-MAX_LONG;

// define a function to find the maximum of two objects.
#define ITMax(a, b) ( (a > b) ? a : b )

IntervalTreeNode::IntervalTreeNode(){
};

IntervalTreeNode::IntervalTreeNode(Interval * newInterval) 
  : storedInterval (newInterval) ,
    key(newInterval->GetLowPoint()), 
    high(newInterval->GetHighPoint()) , 
    maxHigh(high) {
};
IntervalTreeNode::~IntervalTreeNode(){
};
Interval::Interval(){
};
Interval::~Interval(){
};

void Interval::Print() const {
  cout << "No Print Method defined for instance of Interval" << endl;
}

IntervalTree::IntervalTree()
{
  nil = new IntervalTreeNode;
  nil->left = nil->right = nil->parent = nil;
  nil->red = 0;
  nil->key = nil->high = nil->maxHigh = MIN_LONG;
  nil->storedInterval = NULL;
  
  root = new IntervalTreeNode;
  root->parent = root->left = root->right = nil;
  root->key = root->high = root->maxHigh = MAX_LONG;
  root->red=0;
  root->storedInterval = NULL;

  /* the following are used for the Enumerate function */
  recursionNodeStackSize = 128;
  recursionNodeStack = (it_recursion_node *) 
    SafeMalloc(recursionNodeStackSize*sizeof(it_recursion_node));
  recursionNodeStackTop = 1;
  recursionNodeStack[0].start_node = NULL;
  
}

/***********************************************************************/
/*  FUNCTION:  LeftRotate */
/**/
/*  INPUTS:  the node to rotate on */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input: this, x */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. Also updates the maxHigh fields of x and y */
/*            after rotation. */
/***********************************************************************/


/***********************************************************************/
/*  FUNCTION:  RighttRotate */
/**/
/*  INPUTS:  node to rotate on */
/**/
/*  OUTPUT:  None */
/**/
/*  Modifies Input?: this, y */
/**/
/*  EFFECTS:  Rotates as described in _Introduction_To_Algorithms by */
/*            Cormen, Leiserson, Rivest (Chapter 14).  Basically this */
/*            makes the parent of x be to the left of x, x the parent of */
/*            its parent before the rotation and fixes other pointers */
/*            accordingly. Also updates the maxHigh fields of x and y */
/*            after rotation. */
/***********************************************************************/


/***********************************************************************/
/*  FUNCTION:  TreeInsertHelp  */
/**/
/*  INPUTS:  z is the node to insert */
/**/
/*  OUTPUT:  none */
/**/
/*  Modifies Input:  this, z */
/**/
/*  EFFECTS:  Inserts z into the tree as if it were a regular binary tree */
/*            using the algorithm described in _Introduction_To_Algorithms_ */
/*            by Cormen et al.  This funciton is only intended to be called */
/*            by the InsertTree function and not by the user */
/***********************************************************************/


/***********************************************************************/
/*  FUNCTION:  FixUpMaxHigh  */
/**/
/*  INPUTS:  x is the node to start from*/
/**/
/*  OUTPUT:  none */
/**/
/*  Modifies Input:  this */
/**/
/*  EFFECTS:  Travels up to the root fixing the maxHigh fields after */
/*            an insertion or deletion */
/***********************************************************************/



/*  Before calling InsertNode  the node x should have its key set */

/***********************************************************************/
/*  FUNCTION:  InsertNode */
/**/
/*  INPUTS:  newInterval is the interval to insert*/
/**/
/*  OUTPUT:  This function returns a pointer to the newly inserted node */
/*           which is guarunteed to be valid until this node is deleted. */
/*           What this means is if another data structure stores this */
/*           pointer then the tree does not need to be searched when this */
/*           is to be deleted. */
/**/
/*  Modifies Input: tree */
/**/
/*  EFFECTS:  Creates a node node which contains the appropriate key and */
/*            info pointers and inserts it into the tree. */
/***********************************************************************/



/***********************************************************************/
/*  FUNCTION:  GetSuccessorOf  */
/**/
/*    INPUTS:  x is the node we want the succesor of */
/**/
/*    OUTPUT:  This function returns the successor of x or NULL if no */
/*             successor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/

IntervalTreeNode * IntervalTree::GetSuccessorOf(IntervalTreeNode * x) const
 // ***********************************************************************
 { 
   IntervalTreeNode* y;
 
   if (nil!= (y = x->right)) { /* assignment to y is intentional */
     while(y->left != nil) { /* returns the minium of the right subtree of x */
       y=y->left;
     }
     return(y);
   } else {
     y=x->parent;
     while(x == y->right) { /* sentinel used instead of checking for nil */
       x=y;
       y=y->parent;
     }
     if (y == root) return(nil);
     return(y);
   }
 }



/***********************************************************************/
/*  FUNCTION:  GetPredecessorOf  */
/**/
/*    INPUTS:  x is the node to get predecessor of */
/**/
/*    OUTPUT:  This function returns the predecessor of x or NULL if no */
/*             predecessor exists. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:  uses the algorithm in _Introduction_To_Algorithms_ */
/***********************************************************************/

IntervalTreeNode * IntervalTree::GetPredecessorOf(IntervalTreeNode * x) const {
  IntervalTreeNode* y;

  if (nil != (y = x->left)) { /* assignment to y is intentional */
    while(y->right != nil) { /* returns the maximum of the left subtree of x */
      y=y->right;
    }
    return(y);
  } else {
    y=x->parent;
    while(x == y->left) { 
      if (y == root) return(nil); 
      x=y;
      y=y->parent;
    }
    return(y);
  }
}

/***********************************************************************/
/*  FUNCTION:  Print */
/**/
/*    INPUTS:  none */
/**/
/*    OUTPUT:  none  */
/**/
/*    EFFECTS:  This function recursively prints the nodes of the tree */
/*              inorder. */
/**/
/*    Modifies Input: none */
/**/
/*    Note:    This function should only be called from ITTreePrint */
/***********************************************************************/

void IntervalTreeNode::Print(IntervalTreeNode * nil,
			     IntervalTreeNode * root) const {
  storedInterval->Print();
  printf(", k=%li, h=%li, mH=%li",key,high,maxHigh);
  printf("  l->key=");
  if( left == nil) printf("NULL"); else printf("%li",left->key);
  printf("  r->key=");
  if( right == nil) printf("NULL"); else printf("%li",right->key);
  printf("  p->key=");
  if( parent == root) printf("NULL"); else printf("%li",parent->key);
  printf("  red=%li\n",red);
}

void IntervalTree::TreePrintHelper( IntervalTreeNode* x) const {
  
  if (x != nil) {
    TreePrintHelper(x->left);
    x->Print(nil,root);
    TreePrintHelper(x->right);
  }
}

IntervalTree::~IntervalTree() {
  IntervalTreeNode * x = root->left;
  stack<IntervalTreeNode *> stuffToFree;

  if (x != nil) {
    if (x->left != nil) {
      stuffToFree.push(x->left);
    }
    if (x->right != nil) {
      stuffToFree.push(x->right);
    }
    // delete x->storedInterval;
    delete x;
    while(!stuffToFree.empty() ) {
      x = stuffToFree.top();
      stuffToFree.pop();
      if (x->left != nil) {
	stuffToFree.push(x->left);
      }
      if (x->right != nil) {
	stuffToFree.push(x->right);
      }
      // delete x->storedInterval;
      delete x;
    }
  }
  delete nil;
  delete root;
  free(recursionNodeStack);
}


/***********************************************************************/
/*  FUNCTION:  Print */
/**/
/*    INPUTS:  none */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  This function recursively prints the nodes of the tree */
/*             inorder. */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

void IntervalTree::Print() const {
  TreePrintHelper(root->left);
}

/***********************************************************************/
/*  FUNCTION:  DeleteFixUp */
/**/
/*    INPUTS:  x is the child of the spliced */
/*             out node in DeleteNode. */
/**/
/*    OUTPUT:  none */
/**/
/*    EFFECT:  Performs rotations and changes colors to restore red-black */
/*             properties after a node is deleted */
/**/
/*    Modifies Input: this, x */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/



/***********************************************************************/
/*  FUNCTION:  DeleteNode */
/**/
/*    INPUTS:  tree is the tree to delete node z from */
/**/
/*    OUTPUT:  returns the Interval stored at deleted node */
/**/
/*    EFFECT:  Deletes z from tree and but don't call destructor */
/*             Then calls FixUpMaxHigh to fix maxHigh fields then calls */
/*             ITDeleteFixUp to restore red-black properties */
/**/
/*    Modifies Input:  z */
/**/
/*    The algorithm from this function is from _Introduction_To_Algorithms_ */
/***********************************************************************/



/***********************************************************************/
/*  FUNCTION:  Enumerate */
/**/
/*    INPUTS:  tree is the tree to look for intervals overlapping the */
/*             closed interval [low,high]  */
/**/
/*    OUTPUT:  stack containing pointers to the nodes overlapping */
/*             [low,high] */
/**/
/*    Modifies Input: none */
/**/
/*    EFFECT:  Returns a stack containing pointers to nodes containing */
/*             intervals which overlap [low,high] in O(max(N,k*log(N))) */
/*             where N is the number of intervals in the tree and k is  */
/*             the number of overlapping intervals                      */
/**/
/*    Note:    This basic idea for this function comes from the  */
/*              _Introduction_To_Algorithms_ book by Cormen et al, but */
/*             modifications were made to return all overlapping intervals */
/*             instead of just the first overlapping interval as in the */
/*             book.  The natural way to do this would require recursive */
/*             calls of a basic search function.  I translated the */
/*             recursive version into an interative version with a stack */
/*             as described below. */
/***********************************************************************/





long IntervalTree::CheckMaxHighFieldsHelper(IntervalTreeNode * y, 
				    const long currentHigh,
				    long match) const
{
  if (y != nil) {
    match = CheckMaxHighFieldsHelper(y->left,currentHigh,match) ?
      1 : match;
    if (y->high == currentHigh)
      match = 1;
    match = CheckMaxHighFieldsHelper(y->right,currentHigh,match) ?
      1 : match;
  }
  return match;
}

	  

/* Make sure the maxHigh fields for everything makes sense. *
 * If something is wrong, print a warning and exit */
void IntervalTree::CheckMaxHighFields(IntervalTreeNode * x) const {
  if (x != nil) {
    CheckMaxHighFields(x->left);
    if(!(CheckMaxHighFieldsHelper(x,x->maxHigh,0) > 0)) {
      throw Error("pouet");
    }
    CheckMaxHighFields(x->right);
  }
}

void IntervalTree::CheckAssumptions() const {

 CheckMaxHighFields(root->left);
}
 

} // end of namespace

