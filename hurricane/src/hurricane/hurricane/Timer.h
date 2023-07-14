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

//! author="Igor Markov 06/22/97 "      
// freely inspired from abktimer from UCLApack .... just no windows.

#pragma  once
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <sys/resource.h>
#if defined(sun)
extern "C" int  getrusage ( int who, struct rusage *rusage );
#elif defined(linux)
#endif      

#include "hurricane/Error.h"
#include "hurricane/Slot.h"


namespace Hurricane {


  using std::string;


// Used to record the CPU time of process 
  class Timer {
    public:
    // Static Methods.
      static  string         getStringTime       ( double duration );
      static  string         getStringMemory     ( size_t size );
      static  size_t         getMemorySize       ( size_t& memory, size_t& shared );
      static  size_t         getMemorySize       ();
    // Constructors & Destructors.             
                             Timer               ( double limitInSec=0.0 );
                            ~Timer               () { };
    // Methods.                                
              void           start               ( double limitInSec=0.0 );
              void           stop                ();
              void           suspend             (); //call to allow taking a reading without interrupting timing
              void           resume              (); //call after taking a reading after calling suspend()
      inline  bool           isStopped           () const;
              double         getUserTime         () const; // processor time in seconds 
              double         getSysTime          () const; // processor time in seconds 
              double         getCombTime         () const; // processor time in seconds  (sum of the prev. two)
              double         getRealTime         () const; // real time in seconds 
              double         getUnixTime         () const; // Unix time (large number for in randseed)
      inline  double         getRealTimeOnTheFly () const;
      inline  double         getUserTimeOnTheFly () const;
      inline  double         getSysTimeOnTheFly  () const;
      inline  double         getCombTimeOnTheFly () const;
      inline  bool           expired             () const;
      inline  bool           realTimeExpired     (); // Methods expired and realtimeexpired can be used
                                                     // to check if the time is over. The choice of CPU
                                                     // time over real time for expiration is explained
                                                     // by a much finer granularity of measurment. The
                                                     // author observed sensitivity of up to 0.001 CPU
                                                     // sec. on Sparc architecture (while real seconds
                                                     // were integers).
                                                     // DO NOT CALL THIS METHOD ON "OTHER" PLATFORMS
                                                     // (not __SUNPRO_CC) AFTER WRAP_AROUND HAPPENED:
                                                     // (INT_MAX+0.0)/CLOCKS_PER_SEC) sec (can be 36 mins),
                                                     // call realTimeExpired() instead
      inline  size_t         getBaseMemorySize   () const;
              size_t         getIncrease         () const;
              void           resetIncrease       ();
              string         _getString          () const;

    protected:
    // Internal: Enum.
      enum Status { TimerOn
                  , TimerOff
                  , TimerSuspended
                  };

    private:
    // Internal: Attributes.
              time_t         realTime1;
      mutable time_t         realTime2;
              double         UserTime1;
      mutable double         UserTime2;
              double         SysTime1;
      mutable double         SysTime2;
              double         timeLimit;
              Status         status;
      mutable struct rusage  _ru;
              size_t         _memorySize;
      static  size_t         _baseMemorySize;

  };


// Inline Functions.
  inline bool   Timer::isStopped         () const { return status == TimerOff; }
  inline size_t Timer::getBaseMemorySize () const { return _baseMemorySize; }
  inline size_t Timer::getIncrease       () const { return getMemorySize() - _memorySize; }
  inline void   Timer::resetIncrease     () { _memorySize = getMemorySize(); }


  inline double Timer::getRealTimeOnTheFly () const
  { 
    time(&realTime2);
    return difftime(realTime2,realTime1);
  }


  inline double Timer::getUserTimeOnTheFly () const
  { 
    if ( getrusage(RUSAGE_SELF,&_ru) == -1 )
      throw Error ( "Can't get time" );
    return _ru.ru_utime.tv_sec+1e-6*_ru.ru_utime.tv_usec - UserTime1;
  }


  inline double Timer::getSysTimeOnTheFly () const
  { 
    if ( getrusage(RUSAGE_SELF,&_ru) == -1 )
      throw Error ( "Can't get time" );
    return  _ru.ru_stime.tv_sec+1e-6*_ru.ru_stime.tv_usec - SysTime1;
  }


  inline double Timer::getCombTimeOnTheFly () const
  { 
    if ( getrusage(RUSAGE_SELF,&_ru) == -1 )
      throw Error ( "Can't get time" );
    return   _ru.ru_utime.tv_sec+1e-6*_ru.ru_utime.tv_usec - UserTime1
           + _ru.ru_stime.tv_sec+1e-6*_ru.ru_stime.tv_usec - SysTime1;
  }


  inline bool Timer::expired() const
  { 
    return ( timeLimit < getCombTimeOnTheFly() ) && ( timeLimit != 0.0 ); 
  }


  inline bool Timer::realTimeExpired() 
  { 
    return ( timeLimit < getRealTimeOnTheFly() ) && ( timeLimit != 0.0 ); 
  }


}  // Hurricane namespace.


GETSTRING_VALUE_SUPPORT(Hurricane::Timer);
IOSTREAM_VALUE_SUPPORT(Hurricane::Timer);
