/**************************************************************************
***    
*** Copyright (c) 1995-2000 Regents of the University of California,
***               Andrew E. Caldwell, Andrew B. Kahng and Igor L. Markov
*** Copyright (c) 2000-2002 Regents of the University of Michigan,
***               Saurabh N. Adya and Igor L. Markov
***
***  Contact author(s): abk@cs.ucsd.edu, imarkov@umich.edu
***  Original Affiliation:   UCLA, Computer Science Department,
***                          Los Angeles, CA 90095-1596 USA
***
***  Permission is hereby granted, free of charge, to any person obtaining 
***  a copy of this software and associated documentation files (the
***  "Software"), to deal in the Software without restriction, including
***  without limitation 
***  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
***  and/or sell copies of the Software, and to permit persons to whom the 
***  Software is furnished to do so, subject to the following conditions:
***
***  The above copyright notice and this permission notice shall be included
***  in all copies or substantial portions of the Software.
***
*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***
***
***************************************************************************/






// August   22, 1997   created   by Igor Markov  VLSI CAD UCLA ABKGROUP
// November 30, 1997   additions by Max Moroz,   VLSI CAD UCLA ABKGROUP

// note: not thread-safe

// This file to be included into all projects in the group
// it contains platform-specific code portability of which relies
// on symbols defined by compilers

// 970825 mro made corrections to conditional compilation in ctors for
//            Platform and User:
//            i)   _MSC_VER not __MSC_VER (starts with single underscore)
//            ii)  allocated more space for _infoLines
//            iii) changed nested #ifdefs to #if ... #elif ... #else
// 970923 ilm added abk_dump_stack()
// 971130 ilm added Max Moroz code for memory estimate and reworked
//            class MemUsage()
// 980822 mm  corrected MemUsage() to work with Solaris 2.6

#include <cstdlib>
#include <fstream>
using namespace std;

#include  "hurricane/Warning.h"
using Hurricane::Warning;

#include "crlcore/Memory.h"

#if defined(sun) || defined(__SUNPRO_CC)
 #include <sys/systeminfo.h>
#endif

#ifdef linux
 #include <sys/utsname.h>
#endif     

#if defined(linux) || defined(sun) || defined(__SUNPRO_CC)
 #include <unistd.h>
 #include <pwd.h>
 #include <sys/stat.h>
#endif

/* ======================== IMPLEMENTATIONS ======================== */

namespace {
    
double getPeakMemoryUsage()
{

#if ! (defined(sun) || defined(linux) || defined(__SUNPRO_CC))
  return -1;
#endif

#if defined(sun) || defined(__SUNPRO_CC)
  char procFileName[20];
  unsigned pid=getpid();
  sprintf(procFileName,"/proc/%d/as",pid);
  struct stat buf;
  if (stat(procFileName,&buf))  // no such file on Solaris 2.5 and earlier
  {                             // so we stat another file now
    char procFileNameOld[20];
    sprintf(procFileNameOld,"/proc/%d",pid);
    if (stat(procFileNameOld,&buf)) return -1.0;
  }
  return (1.0/(1024*1024)) * buf.st_size;
#elif defined(linux)
  char buf[1000];
  sprintf(buf,"/proc/%d/stat", static_cast<unsigned>(getpid()));
  ifstream ifs(buf);
  for(unsigned k=0; k!=23; k++) ifs >> buf;
  return (1.0/(1024*1024)) * atoi(buf);
#else
  return -1;
#endif
}

// code by Max Moroz

const int kMegaByte=1024*1024;
const int kSmallChunks=1000;
const int kMaxAllocs=20000;
const double MemUsageEps=3;

// everything in bytes

inline int memused()
{
// cout << " Peak memory " << getPeakMemoryUsage() << endl;
   return static_cast<int>(getPeakMemoryUsage()*kMegaByte);
}

double getMemoryUsageEstimate()
{
#if ! (defined(linux) || defined(sun) || defined(__SUNPRO_CC))
        return -1;
#endif
        static int prevMem=0;
        static int fail=0;

        if (fail) return -1;

//      new_handler oldHndl;
//      oldHndl=set_new_handler(mH);

        void* ptr[kMaxAllocs];
        int numAllocs=0;

        int last=memused();
//       cout << "Memused : " << last << endl;
        if (last<=0) return -1;
//      cerr<<"memused()="<<memused()<<endl;
        int chunk=8192; // system allocates 8K chunks
        int allocated=0;
        int countSmallChunks=kSmallChunks;
        // if we allocate <8K, we'd get memused()+=8K, and allocated<8K - error
        while (1)
        {
//              abkfatal(numAllocs<kMaxAllocs, "too many allocs");
                if (numAllocs>=kMaxAllocs) 
                {   
                   throw Warning("too many allocs (may be okay for 64-bit builds)");
                   // FIFO destruction
                   for (int i=0; i!=numAllocs; ++i) free(ptr[i]);
                    return -1.0;     
                }
//              cerr<<"old: "<<memused()<<"; allocating "<<chunk<<"; now: ";
                if (!(ptr[numAllocs++]=malloc(chunk)))
                {
                        fail=1;
                        return -1;
                }
//              cerr<<memused()<<endl;
                allocated+=chunk;
                if (memused()>last+MemUsageEps)
                        break;
                if (chunk<=kMegaByte && !countSmallChunks--)
                {
                                chunk*=2;
                                countSmallChunks=kSmallChunks;
                }
        }

//     int result=memused()-allocated-prevMem;

/* LIFO destruction
        while (numAllocs) free(ptr[--numAllocs]); */

        // FIFO destruction
        for (int i=0; i!=numAllocs; ++i) free(ptr[i]);

// handle extra correctly:
// in some cases we need to add its prev value to current,
// in some just store the new value

        prevMem=memused()-allocated;
//      set_new_handler(oldHndl);
        return prevMem/double(kMegaByte);
}

}

namespace CRL {

MemUsage::MemUsage()
{
  _peak=getPeakMemoryUsage();
  _estimate=getMemoryUsageEstimate();
}

void MemChange::resetMark()
{
  _memUsageMark=getMemoryUsageEstimate();
}

MemChange::MemChange()
{
  resetMark();
}

MemChange::operator double() const
{
#if defined(sun) || defined(linux) || defined(__SUNPRO_CC)
  return getPeakMemoryUsage()-_memUsageMark;
#else
  return -1.0;
#endif
}

}
