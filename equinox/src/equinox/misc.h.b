#ifndef INC_E_MISC_DOT_H
#define INC_E_MISC_DOT_H
#include<stdio.h>
#include<stdlib.h>
#include<sys/unistd.h>
#include<unistd.h>
//#include<strings.h>
#include<string.h>


namespace EQUINOX {

/*  CONVENTIONS */
/* 								      */
/*	Function names: Each word in a function name begins with      */
/*	a capital letter.  An example funcntion name is		      */
/*      CreateRedTree(a,b,c). Furthermore, each function name	      */
/*      should begin with a capital letter to easily distinguish      */
/*      them from variables.					      */
/*								      */
/*      Variable names: Each word in a variable name begins with      */
/*      a capital letter EXCEPT the first letter of the variable      */
/*      name.  For example, int newLongInt.  Global variables have    */
/*      names beginning with "g" or "global:. 	                      */
/*      An example of a global variable name is gNewtonsConstant.     */


//      The ExitProgramMacro is used to exit the program when a
//	catastrophe occurs (e.g. assertion failure).  You can define
//	your own version of the ExitProgramMacro to dump core, log
//	things or do something else that you want.  If the symbol
//	ExitProgramMacro is not defined though, then a default
//	definition is created.  

#ifndef ExitProgramMacro

#define ExitProgramMacro(a) { \
printf("Error: "); printf(a); \
printf("Exiting from line %i in file %s\n",__LINE__,__FILE__); \
printf("\nCausing Segmentation Fault to exit ungracefully\n"); \
       int * junk = NULL; (*junk)++;\
       printf("%p\n",junk);}

//       printf("%i\n",(int)junk);}

#endif /* ends #ifndef ExitProgramMacro */


#define VERIFY(condition) \
if (!(condition)) { \
fprintf(stderr, "Assumption \"%s\"\nFailed in file %s: at line:%i\n", \
#condition,__FILE__,__LINE__); \
ExitProgramMacro(#condition);}

#ifdef CHECK_ASSUMPTIONS
#define ASSUME(x) VERIFY(x)
#else
#define ASSUME(x) ;
#endif

/***********************************************************************/
/*  FUNCTION:  void Assert(int assertion, char* error)  */
/**/
/*  INPUTS: assertion should be a predicated that the programmer */
/*  assumes to be true.  If this assumption is not true the message */
/*  error is printed and the program exits. */
/**/
/*  OUTPUT: None. */
/**/
/*  Modifies input:  none */
/**/
/*  Note:  If DEBUG_ASSERT is not defined then assertions should not */
/*         be in use as they will slow down the code.  Therefore the */
/*         compiler will complain if an assertion is used when */
/*         DEBUG_ASSERT is undefined. */
/***********************************************************************/


inline void Assert(int assertion, char* error) {
  if(!assertion) {
    printf("Assertion Failed: %s\n",error);
    ExitProgramMacro("Exiting From Function Assert(...)\n");
  }
}

/***********************************************************************/
/*  FUNCTION:  SafeMalloc */
/**/
/*    INPUTS:  size is the size to malloc */
/**/
/*    OUTPUT:  returns pointer to allocated memory if succesful */
/**/
/*    EFFECT:  mallocs new memory.  If malloc fails, prints error message */
/*             and terminates program. */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

inline void * SafeMalloc(size_t size) {
  void * result;

  if ( (result = malloc(size)) ) { /* assignment intentional */
    return(result);
  } else {
    char errorMessagePartOne [200];
    char errorMessagePartTwo [200];
    sprintf(errorMessagePartOne,
	    "Exiting From SafeMalloc because malloc of size %i failed.\n",
	    size);
    sprintf(errorMessagePartTwo,
	    "Calling sbrk(0) gives %x\n",(long)sbrk(0));
    strcat(errorMessagePartOne,errorMessagePartTwo);
    ExitProgramMacro(errorMessagePartOne);
    return(0);
  }
}

/***********************************************************************/
/*  FUNCTION:  SafeCalloc */
/**/
/*    INPUTS:  size is the size to calloc */
/**/
/*    OUTPUT:  returns pointer to allocated memory if succesful */
/**/
/*    EFFECT:  callocs new memory.  If calloc fails, prints error message */
/*             and terminates program. */
/**/
/*    Modifies Input: none */
/**/
/***********************************************************************/

inline void * SafeCalloc(int numberOfElements , size_t size) {
  void * result;

  if ( (result = calloc(numberOfElements, size)) )
    { /* assignment intentional in above line */
    return(result);
  } else {
    printf("memory overflow: calloc failed in SafeCalloc(%i,%i).",
	   numberOfElements, size);
    printf("sbrk(0) gives %x\n",(long)sbrk(0));
    printf("  Exiting Program.\n");
    ExitProgramMacro("Exiting From Function SafeCalloc(...)\n");
    return(0);
  }
}

/*  NullFunction does nothing it is included so that it can be passed */
/*  as a function to RBTreeCreate when no other suitable function has */
/*  been defined */

inline void NullFunction(void * ) { ; }
inline void NullFunction(const void * ) { ; }

}

#endif








