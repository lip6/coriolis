//#ifndef E_INTERVAL_TREE
//#define E_INTERVAL_TREE

#ifndef INTERVALTREE_H
#define INTERVALTREE_H

#include<math.h>
#include<limits.h>
#include<iostream>
#include<stack>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include <hurricane/Error.h>
#include <equinox/Interval.h>

//  The interval_tree.h and interval_tree.cc files contain code for 
//  interval trees implemented using red-black-trees as described in 
//  the book _Introduction_To_Algorithms_ by Cormen, Leisserson, 
//  and Rivest.  

//  CONVENTIONS:  
//                Function names: Each word in a function name begins with 
//                a capital letter.  An example funcntion name is  
//                CreateRedTree(a,b,c). Furthermore, each function name 
//                should begin with a capital letter to easily distinguish 
//                them from variables. 
//                                                                     
//                Variable names: Each word in a variable name begins with 
//                a capital letter EXCEPT the first letter of the variable 
//                name.  For example, int newLongInt.  Global variables have 
//                names beginning with "g".  An example of a global 
//                variable name is gNewtonsConstant. 


#ifndef MAX_LONG
#define MAX_LONG LONG_MAX // some architechturs define INT_MAX not MAX_INT
#define MIN_LONG -MAX_LONG 
#endif

#define ITMax(a, b) ( (a > b) ? a : b )

//#ifndef MAX_INT
//#define MAX_INT INT_MAX // some architechturs define INT_MAX not MAX_INT
//#endif
// The Interval class is an Abstract Base Class.  This means that no
// instance of the Interval class can exist.  Only classes which
// inherit from the Interval class can exist.  Furthermore any class
// which inherits from the Interval class must define the member
// functions GetLowPoint and GetHighPoint.
//
// The GetLowPoint should return the lowest point of the interval and
// the GetHighPoint should return the highest point of the interval.  


namespace Equinox {

  using std::stack;
  using Hurricane::Error;


#   if !defined(__DOXYGEN_PROCESSOR__)

static inline bool Overlap(long& a1, long& a2, long& b1, long& b2) { 
// *****************************************************************
  if (a1 <= b1) {
    return( (b1 <= a2) );
  } else {
    return( (a1 <= b2) );
  }
}

#endif


class IntervalTreeNode {
// *********************

  friend class IntervalTree;
  public:
    IntervalTreeNode(Interval * );

#   if !defined(__DOXYGEN_PROCESSOR__)
    void Print(IntervalTreeNode*, IntervalTreeNode*) const;
    IntervalTreeNode();
    ~IntervalTreeNode();

  protected:
    Interval * storedInterval;
    long key;
    long high;
    long maxHigh;
    long red; /* if red=0 then the node is black */
    IntervalTreeNode * left;
    IntervalTreeNode * right;
    IntervalTreeNode * parent;
# endif
};


#   if !defined(__DOXYGEN_PROCESSOR__)

struct it_recursion_node {
// ***********************

public:
  /*  this structure stores the information needed when we take the */
  /*  right branch in searching for intervals but possibly come back */
  /*  and check the left branch as well. */

  IntervalTreeNode * start_node;
  unsigned long parentIndex;
  long tryRightBranch;
} ;

#endif


class IntervalTree {
// *****************

// constructors 
// ************  
  public: IntervalTree();


// destructors 
// ***********  
  public: ~IntervalTree();


// Prints
// ******  
#   if !defined(__DOXYGEN_PROCESSOR__)

  public: void Print() const;



inline void * SafeMalloc(size_t size) {
  void * result;

  if ( (result = malloc(size)) ) { /* assignment intentional */
    return(result);
  } else {
    char errorMessagePartOne [200];
    char errorMessagePartTwo [200];
    strcat(errorMessagePartOne,errorMessagePartTwo);
    throw Error("Bad malloc");
    return(0);
  }
}




inline void * SafeCalloc(int numberOfElements , size_t size) {
  void * result;

  if ( (result = calloc(numberOfElements, size)) )
    { /* assignment intentional in above line */
    return(result);
  } else {
    printf("  Exiting Program.\n");
    throw Error("Bad calloc");
    return(0);
  }
}













// Operations
// **********
  public   :   void FixUpMaxHigh(IntervalTreeNode * x) {
  	     // *****************************************
                 while(x != root) {
                   x->maxHigh=ITMax(x->high,ITMax(x->left->maxHigh,x->right->maxHigh));
                   x=x->parent;
                 }
               #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                 CheckAssumptions();
               #endif
               };
  
  
  protected:   void LeftRotate(IntervalTreeNode* x) {
  	     // ***********************************
  
                  IntervalTreeNode* y;
              
                  /*  I originally wrote this function to use the sentinel for */
                  /*  nil to avoid checking for nil.  However this introduces a */
                  /*  very subtle bug because sometimes this function modifies */
                  /*  the parent pointer of nil.  This can be a problem if a */
                  /*  function which calls LeftRotate also uses the nil sentinel */
                  /*  and expects the nil sentinel's parent pointer to be unchanged */
                  /*  after calling this function.  For example, when DeleteFixUP */
                  /*  calls LeftRotate it expects the parent pointer of nil to be */
                  /*  unchanged. */
                
                  y=x->right;
                  x->right=y->left;
                
                  if (y->left != nil) y->left->parent=x; /* used to use sentinel here */
                  /* and do an unconditional assignment instead of testing for nil */
                  
                  y->parent=x->parent;   
                
                  /* instead of checking if x->parent is the root as in the book, we */
                  /* count on the root sentinel to implicitly take care of this case */
                  if( x == x->parent->left) {
                    x->parent->left=y;
                  } else {
                    x->parent->right=y;
                  }
                  y->left=x;
                  x->parent=y;
                
                  x->maxHigh=ITMax(x->left->maxHigh,ITMax(x->right->maxHigh,x->high));
                  y->maxHigh=ITMax(x->maxHigh,ITMax(y->right->maxHigh,y->high));
                #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                  CheckAssumptions();
                #elif defined(DEBUG_ASSERT)
                  Assert(!nil->red,"nil not red in ITLeftRotate");
                  Assert((nil->maxHigh=MIN_LONG),
                	 "nil->maxHigh != MIN_LONG in ITLeftRotate");
                #endif
                };
             
  
  protected:   void RightRotate(IntervalTreeNode* y) {
  	     // ***************************************
                 IntervalTreeNode* x;
               
                 /*  I originally wrote this function to use the sentinel for */
                 /*  nil to avoid checking for nil.  However this introduces a */
                 /*  very subtle bug because sometimes this function modifies */
                 /*  the parent pointer of nil.  This can be a problem if a */
                 /*  function which calls LeftRotate also uses the nil sentinel */
                 /*  and expects the nil sentinel's parent pointer to be unchanged */
                 /*  after calling this function.  For example, when DeleteFixUP */
                 /*  calls LeftRotate it expects the parent pointer of nil to be */
                 /*  unchanged. */
               
                 x=y->left;
                 y->left=x->right;
               
                 if (nil != x->right)  x->right->parent=y; /*used to use sentinel here */
                 /* and do an unconditional assignment instead of testing for nil */
               
                 /* instead of checking if x->parent is the root as in the book, we */
                 /* count on the root sentinel to implicitly take care of this case */
                 x->parent=y->parent;
                 if( y == y->parent->left) {
                   y->parent->left=x;
                 } else {
                   y->parent->right=x;
                 }
                 x->right=y;
                 y->parent=x;
               
                 y->maxHigh=ITMax(y->left->maxHigh,ITMax(y->right->maxHigh,y->high));
                 x->maxHigh=ITMax(x->left->maxHigh,ITMax(y->maxHigh,x->high));
               #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                 CheckAssumptions();
               #elif defined(DEBUG_ASSERT)
                 Assert(!nil->red,"nil not red in ITRightRotate");
                 Assert((nil->maxHigh=MIN_LONG),
               	 "nil->maxHigh != MIN_LONG in ITRightRotate");
               #endif
               };
               
# endif
  
  public:    Interval * DeleteNode(IntervalTreeNode * z)
             // *****************************************************
             {
                 IntervalTreeNode* y;
                 IntervalTreeNode* x;
                 Interval * returnValue = z->storedInterval;
               
                 y= ((z->left == nil) || (z->right == nil)) ? z : GetSuccessorOf(z);
                 x= (y->left == nil) ? y->right : y->left;
                 if (root == (x->parent = y->parent)) { /* assignment of y->p to x->p is intentional */
                   root->left=x;
                 } else {
                   if (y == y->parent->left) {
                     y->parent->left=x;
                   } else {
                     y->parent->right=x;
                   }
                 }
                 if (y != z) { /* y should not be nil in this case */
               
               #ifdef DEBUG_ASSERT
                   Assert( (y!=nil),"y is nil in DeleteNode \n");
               #endif
                   /* y is the node to splice out and x is its child */
                 
                   y->maxHigh = MIN_LONG;
                   y->left=z->left;
                   y->right=z->right;
                   y->parent=z->parent;
                   z->left->parent=z->right->parent=y;
                   if (z == z->parent->left) {
                     z->parent->left=y; 
                   } else {
                     z->parent->right=y;
                   }
                   FixUpMaxHigh(x->parent); 
                   if (!(y->red)) {
                     y->red = z->red;
                     DeleteFixUp(x);
                   } else
                     y->red = z->red; 
                   delete z;
               #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                   CheckAssumptions();
               #elif defined(DEBUG_ASSERT)
                   Assert(!nil->red,"nil not black in ITDelete");
                   Assert((nil->maxHigh=MIN_LONG),"nil->maxHigh != MIN_LONG in ITDelete");
               #endif
                 } else {
                   FixUpMaxHigh(x->parent);
                   if (!(y->red)) DeleteFixUp(x);
                   delete y;
               #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                   CheckAssumptions();
               #elif defined(DEBUG_ASSERT)
                   Assert(!nil->red,"nil not black in ITDelete");
                   Assert((nil->maxHigh=MIN_LONG),"nil->maxHigh != MIN_LONG in ITDelete");
               #endif
                 }
                 return returnValue;
             };
        
#   if !defined(__DOXYGEN_PROCESSOR__)

  protected: void TreeInsertHelp(IntervalTreeNode * z) 
	     // ************************************** 
	     {
               /*  This function should only be called by InsertITTree (see above) */
               IntervalTreeNode* x;
               IntervalTreeNode* y;
                 
               z->left=z->right=nil;
               y=root;
               x=root->left;
               while( x != nil) {
                 y=x;
                 if ( x->key > z->key) { 
                   x=x->left;
                 } else { /* x->key <= z->key */
                   x=x->right;
                 }
               }
               z->parent=y;
               if ( (y == root) ||
                    (y->key > z->key) ) { 
                 y->left=z;
               } else {
                 y->right=z;
               }
               
               #if defined(DEBUG_ASSERT)
                 Assert(!nil->red,"nil not red in ITTreeInsertHelp");
                 Assert((nil->maxHigh=MIN_LONG),
               	 "nil->maxHigh != MIN_LONG in ITTreeInsertHelp");
               #endif
            };
                 
     
  protected: void DeleteFixUp(IntervalTreeNode * x)
	     // ***********************************
	     {
                IntervalTreeNode * w;
                IntervalTreeNode * rootLeft = root->left;
              
                while( (!x->red) && (rootLeft != x)) {
                  if (x == x->parent->left) {
                    w=x->parent->right;
                    if (w->red) {
              	w->red=0;
              	x->parent->red=1;
              	LeftRotate(x->parent);
              	w=x->parent->right;
                    }
                    if ( (!w->right->red) && (!w->left->red) ) { 
              	w->red=1;
              	x=x->parent;
                    } else {
              	if (!w->right->red) {
              	  w->left->red=0;
              	  w->red=1;
              	  RightRotate(w);
              	  w=x->parent->right;
              	}
              	w->red=x->parent->red;
              	x->parent->red=0;
              	w->right->red=0;
              	LeftRotate(x->parent);
              	x=rootLeft; /* this is to exit while loop */
                    }
                  } else { /* the code below is has left and right switched from above */
                    w=x->parent->left;
                    if (w->red) {
              	w->red=0;
              	x->parent->red=1;
              	RightRotate(x->parent);
              	w=x->parent->left;
                    }
                    if ( (!w->right->red) && (!w->left->red) ) { 
              	w->red=1;
              	x=x->parent;
                    } else {
              	if (!w->left->red) {
              	  w->right->red=0;
              	  w->red=1;
              	  LeftRotate(w);
              	  w=x->parent->left;
              	}
              	w->red=x->parent->red;
              	x->parent->red=0;
              	w->left->red=0;
              	RightRotate(x->parent);
              	x=rootLeft; /* this is to exit while loop */
                    }
                  }
                }
                x->red=0;
              
              #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
                CheckAssumptions();
              #elif defined(DEBUG_ASSERT)
                Assert(!nil->red,"nil not black in ITDeleteFixUp");
                Assert((nil->maxHigh=MIN_LONG),
              	 "nil->maxHigh != MIN_LONG in ITDeleteFixUp");
              #endif

             };

# endif
                
  public:    IntervalTreeNode * Insert(Interval * newInterval)
             // **********************************************************
             {
               IntervalTreeNode * y;
               IntervalTreeNode * x;
               IntervalTreeNode * newNode;
             
               x = new IntervalTreeNode(newInterval);
               TreeInsertHelp(x);
               FixUpMaxHigh(x->parent);
               newNode = x;
               x->red=1;
               while(x->parent->red) { /* use sentinel instead of checking for root */
                 if (x->parent == x->parent->parent->left) {
                   y=x->parent->parent->right;
                   if (y->red) {
             	x->parent->red=0;
             	y->red=0;
             	x->parent->parent->red=1;
             	x=x->parent->parent;
                   } else {
             	if (x == x->parent->right) {
             	  x=x->parent;
             	  LeftRotate(x);
             	}
             	x->parent->red=0;
             	x->parent->parent->red=1;
             	RightRotate(x->parent->parent);
                   } 
                 } else { /* case for x->parent == x->parent->parent->right */
                          /* this part is just like the section above with */
                          /* left and right interchanged */
                   y=x->parent->parent->left;
                   if (y->red) {
             	x->parent->red=0;
             	y->red=0;
             	x->parent->parent->red=1;
             	x=x->parent->parent;
                   } else {
             	if (x == x->parent->left) {
             	  x=x->parent;
             	  RightRotate(x);
             	}
             	x->parent->red=0;
             	x->parent->parent->red=1;
             	LeftRotate(x->parent->parent);
                   } 
                 }
               }
               root->left->red=0;
               return(newNode);
             
             #ifdef CHECK_INTERVAL_TREE_ASSUMPTIONS
               CheckAssumptions();
             #elif defined(DEBUG_ASSERT)
               Assert(!nil->red,"nil not red in ITTreeInsert");
               Assert(!root->red,"root not red in ITTreeInsert");
               Assert((nil->maxHigh=MIN_LONG),
             	 "nil->maxHigh != MIN_LONG in ITTreeInsert");
             #endif
             };



        /*  The basic idea for the function below is to take the IntervalSearch */
        /*  function from the book and modify to find all overlapping intervals */
        /*  instead of just one.  This means that any time we take the left */
        /*  branch down the tree we must also check the right branch if and only if */
        /*  we find an overlapping interval in that left branch.  Note this is a */
        /*  recursive condition because if we go left at the root then go left */
        /*  again at the first left child and find an overlap in the left subtree */
        /*  of the left child of root we must recursively check the right subtree */
        /*  of the left child of root as well as the right child of root. */
  public: stack<Interval *> * Enumerate(long low, long high)
          // *****************************************************
          {
             stack<Interval *> * enumResultStack;
             IntervalTreeNode* x=root->left;
             long stuffToDo = (x != nil);
             
             // Possible speed up: add min field to prune right searches //
           
           #ifdef DEBUG_ASSERT
             Assert((recursionNodeStackTop == 0),
           	 "recursionStack not empty when entering IntervalTree::Enumerate");
           #endif
             currentParent = 0;
             enumResultStack = new stack<Interval *>;
           
             while(stuffToDo) {
               if (Overlap(low,high,x->key,x->high) ) {
                 enumResultStack->push(x->storedInterval);
                 recursionNodeStack[currentParent].tryRightBranch=1;
               }
               if(x->left->maxHigh >= low) { // implies x != nil 
                 if ( recursionNodeStackTop == recursionNodeStackSize ) {
           	recursionNodeStackSize *= 2;
           	recursionNodeStack = (it_recursion_node *) 
           	  realloc(recursionNodeStack,
           		  recursionNodeStackSize * sizeof(it_recursion_node));
           	if (recursionNodeStack == NULL) 
           	  throw Error("realloc failed in IntervalTree::Enumerate\n");
                 }
                 recursionNodeStack[recursionNodeStackTop].start_node = x;
                 recursionNodeStack[recursionNodeStackTop].tryRightBranch = 0;
                 recursionNodeStack[recursionNodeStackTop].parentIndex = currentParent;
                 currentParent = recursionNodeStackTop++;
                 x = x->left;
               } else {
                 x = x->right;
               }
               stuffToDo = (x != nil);
               while( (!stuffToDo) && (recursionNodeStackTop > 1) ) {
           	if(recursionNodeStack[--recursionNodeStackTop].tryRightBranch) {
           	  x=recursionNodeStack[recursionNodeStackTop].start_node->right;
           	  currentParent=recursionNodeStack[recursionNodeStackTop].parentIndex;
           	  recursionNodeStack[currentParent].tryRightBranch=1;
           	  stuffToDo = ( x != nil);
           	}
               }
             }
           #ifdef DEBUG_ASSERT
             Assert((recursionNodeStackTop == 1),
           	 "recursionStack not empty when exiting IntervalTree::Enumerate");
           #endif
             return(enumResultStack);   
          }; 
  
#   if !defined(__DOXYGEN_PROCESSOR__)
  
  protected: void CheckAssumptions() const;
  
        /*  A sentinel is used for root and for nil.  These sentinels are */
        /*  created when ITTreeCreate is caled.  root->left should always */
        /*  point to the node which is the root of the tree.  nil points to a */
        /*  node which should always be black but has aribtrary children and */
        /*  parent and no key or info.  The point of using these sentinels is so */
        /*  that the root and nil nodes do not require special cases in the code */
  
  protected: IntervalTreeNode * root;
  protected: IntervalTreeNode * nil;
  
  public: IntervalTreeNode * GetPredecessorOf(IntervalTreeNode *) const;

  public: IntervalTreeNode * GetSuccessorOf(IntervalTreeNode * ) const;
  protected: void TreePrintHelper(IntervalTreeNode *) const;
  protected: void CheckMaxHighFields(IntervalTreeNode *) const;
  protected: long CheckMaxHighFieldsHelper(IntervalTreeNode * y, 
                  			const long currentHigh,
                  			long match) const;
  
// Attributs
// *********
  private:  unsigned long recursionNodeStackSize;
  private:  it_recursion_node * recursionNodeStack;
  private:  unsigned long currentParent;
  private:  unsigned long recursionNodeStackTop;
# endif


};


} // end of namespace
#endif






