#ifndef _KNIK_FLUTE_H
#define _KNIK_FLUTE_H

#define POWVFILE "POWV9.dat"    // LUT for POWV (Wirelength Vector)
#define POSTFILE "POST9.dat"    // LUT for POST (Steiner FTree)
#define MAXD 150    // max. degree of a net that can be handled
                    // setting MAXD to more than 150 is not recommended
#define D 9         // LUT is used for d <= D, D <= 9
#define ROUTING 1   // 1 to construct routing, 0 to estimate WL only
#define REMOVE_DUPLICATE_PIN 0  // remove dup. pin for flute_wl() & flute()
#define ACCURACY 3  // Default accuracy

#ifndef DTYPE   // Data type for distance
#define DTYPE int
#endif

typedef struct
{
    DTYPE x, y;   // starting point of the branch
    int n;   // index of neighbor
} Branch;

typedef struct
{
    int deg;   // degree
    DTYPE length;   // total wirelength
    Branch *branch;   // array of tree branches
} FTree;

// Major functions
extern void readLUT();
extern DTYPE flute_wl(int d, DTYPE x[], DTYPE y[], int acc);
//Macro: DTYPE flutes_wl(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);
extern FTree flute(int d, DTYPE x[], DTYPE y[], int acc);
//Macro: FTree flutes(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);
extern DTYPE wirelength(FTree t);
extern void printtree(FTree t);
extern void plottree(FTree t);


// Other useful functions
extern DTYPE flutes_wl_LD(int d, DTYPE xs[], DTYPE ys[], int s[]);
extern DTYPE flutes_wl_MD(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);
extern DTYPE flutes_wl_RDP(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);
extern FTree flutes_LD(int d, DTYPE xs[], DTYPE ys[], int s[]);
extern FTree flutes_MD(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);
extern FTree flutes_RDP(int d, DTYPE xs[], DTYPE ys[], int s[], int acc);

#if REMOVE_DUPLICATE_PIN==1
  #define flutes_wl(d, xs, ys, s, acc) flutes_wl_RDP(d, xs, ys, s, acc) 
  #define flutes(d, xs, ys, s, acc) flutes_RDP(d, xs, ys, s, acc) 
#else
  #define flutes_wl(d, xs, ys, s, acc) flutes_wl_ALLD(d, xs, ys, s, acc) 
  #define flutes(d, xs, ys, s, acc) flutes_ALLD(d, xs, ys, s, acc) 
#endif

#define flutes_wl_ALLD(d, xs, ys, s, acc) flutes_wl_LMD(d, xs, ys, s, acc)
#define flutes_ALLD(d, xs, ys, s, acc) flutes_LMD(d, xs, ys, s, acc)
//#define flutes_ALLD(d, xs, ys, s, acc)   (d<=D ? flutes_LD(d, xs, ys, s) : (d<=D2 ? flutes_MD(d, xs, ys, s, acc) : flutes_HD(d, xs, ys, s, acc)))

#define flutes_wl_LMD(d, xs, ys, s, acc) \
    (d<=D ? flutes_wl_LD(d, xs, ys, s) : flutes_wl_MD(d, xs, ys, s, acc))
#define flutes_LMD(d, xs, ys, s, acc) \
    (d<=D ? flutes_LD(d, xs, ys, s) : flutes_MD(d, xs, ys, s, acc))

#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define abs(x) ((x)<0?(-x):(x))
#define ADIFF(x,y) ((x)>(y)?(x-y):(y-x))  // Absolute difference

#endif
