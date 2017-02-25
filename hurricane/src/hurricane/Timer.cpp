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


#include <fstream> 
#include <sstream> 
#include <iomanip> 
#include "hurricane/Timer.h"


namespace Hurricane {


  size_t Timer::_baseMemorySize = Timer::getMemorySize();


// -------------------------------------------------------------------
// Class :  "Timer".


  Timer::Timer ( double limitInSec )
    : timeLimit(limitInSec)
    , _memorySize(getMemorySize())
  {
    status = TimerOff;
  }


  void  Timer::start ( double limitInSec )
  {
    if ( status != TimerOff )
      throw Error ( "Can't start timer twice" );
    status = TimerOn;

    timeLimit = limitInSec;
 
    if ( time(&realTime1) == -1 )
      throw Error ( "Can't get time" );
    if ( getrusage(RUSAGE_SELF,&_ru) == -1 )
      throw Error(" Can't get time");
    UserTime1 = _ru.ru_utime.tv_sec+1e-6*_ru.ru_utime.tv_usec;
    SysTime1  = _ru.ru_stime.tv_sec+1e-6*_ru.ru_stime.tv_usec;
  }


  void Timer::stop ()
  {
    if ( status != TimerOn )
      throw Error ( "Can't stop timer twice!" );
    status = TimerOff;
    if ( time(&realTime2) == -1 )
      throw Error ( "Can't get time" );
    if ( getrusage(RUSAGE_SELF,&_ru) == -1 )
      throw Error ( " Can't get time" );
    UserTime2 = _ru.ru_utime.tv_sec+1e-6*_ru.ru_utime.tv_usec;
    SysTime2  = _ru.ru_stime.tv_sec+1e-6*_ru.ru_stime.tv_usec;
  }


  void  Timer::suspend ()
  {
    stop ();
    status = TimerSuspended;
  }


  void Timer::resume ()
  {
    if ( status != TimerSuspended )
      throw Error ( "Can't resume timer unless suspended" );
    status = TimerOn;
  }


  double Timer::getUserTime () const
  {
    if ( status != TimerOff && status != TimerSuspended )
      throw Error ( "Have to stop timer to get a reading" );
    return UserTime2 - UserTime1;
  }


  double Timer::getSysTime () const
  {
    if ( status != TimerOff && status != TimerSuspended )
      throw Error ( "Have to stop timer to get a reading" );
    return SysTime2 - SysTime1;
  }


  double Timer::getCombTime () const
  {
    if ( status != TimerOff && status != TimerSuspended )
      throw Error ( "Have to stop timer to get a reading" );
    return ( UserTime2 - UserTime1 ) + ( SysTime2 - SysTime1 );
  }


  double Timer::getRealTime () const
  {
    if ( status != TimerOff && status != TimerSuspended )
      throw Error ( "Have to stop timer to get a reading" );
    return difftime ( realTime2, realTime1 );
  }


  double Timer::getUnixTime () const
  {
    time_t  utime;
    time_t  zero  = 0;

    if ( time(&utime) == -1 )
      throw Error ( "Can't get time" );
    return difftime ( utime, zero );
  } 


  size_t  Timer::getMemorySize ()
  {
    size_t s, m;
    return getMemorySize(s,m);
  }


  size_t  Timer::getMemorySize ( size_t& memory, size_t& shared )
  {
#if !defined __linux__
    return (size_t)sbrk( 0 );
#else
#if 0
    string              mmapFile = "/proc/" + to_string(getpid()) + "/maps" ;
    ifstream            mmap     ( mmapFile );
    char                line     [ 4096 ];
    unsigned long long  rmemory  = 0;
    unsigned long long  rshared  = 0;

    while ( not mmap.eof() ) {
      mmap.getline( line, 4096 );
      const string       sline    ( line );
      istringstream      iss      ( sline );
      unsigned long long baddress = 0;
      unsigned long long eaddress = 0;
      string             perms;
      unsigned long long offset;
      int                major;
      int                minor;
      unsigned long long inode;
      string             pathname;
      char               separ;

      iss >> hex >> baddress >> separ >> eaddress >> dec >> perms >> offset
          >> major >> separ >> minor >> inode >> pathname;

      if (not pathname.empty() and pathname.compare(0,6,"[heap]") != 0)
        rshared += eaddress - baddress;
      else
        rmemory += eaddress - baddress;
    }

    memory = (size_t)rmemory;
    shared = (size_t)rshared;

    mmap.close();
    return memory;
#endif
    long                pagesize  = sysconf( _SC_PAGESIZE );
    string              statmFile = "/proc/" + to_string(getpid()) + "/statm" ;
    ifstream            statm     ( statmFile );
    unsigned long long  size      = 0;
    unsigned long long  resident  = 0;
    unsigned long long  share     = 0;
    unsigned long long  text      = 0;
    unsigned long long  lib       = 0;
    unsigned long long  data      = 0;
    unsigned long long  dt        = 0;

    statm >> size >> resident >> share >> text >> lib >> data >> dt;

    memory = (size_t)size * pagesize;

    statm.close();
    return memory;
#endif
  }


  string  Timer::_getString () const
  {
    //CPUNormalizer cpunorm;
    double  userSec = getUserTime ();
    double  normSec = getUserTime (); // * cpunorm.getNormalizingFactor();
    char    buffer[20];

    if ( userSec > 0.01 ) {
      double frac =userSec-floor(userSec);
      double delta=frac-0.001*floor(frac*1000.0);

      userSec -= delta;
      frac     = normSec - floor(normSec); 
      delta    = frac - 0.001*floor(frac*1000);
      normSec -= delta;
    }
  
    sprintf ( buffer, "%.1e", getSysTime() );
    
    ostringstream os;
    os << " " << userSec       << " user,"
       << " " << buffer        << " system," 
       << " " << getRealTime() << " real," 
       << " " << normSec       << " norm'd "
       << "sec ";

    return os.str();
  }


  string  Timer::getStringTime ( double duration )
  {
    string s;

    unsigned int  hours = (unsigned int)duration / 3600;
    if ( hours )
      s += getString(hours) + "h ";
      
    unsigned int  minutes = ((unsigned int)duration % 3600) / 60;
    if ( hours || minutes )
      s += getString(minutes) + "m ";

    double seconds = duration;
    if ( hours || minutes ) {
      minutes = ((unsigned int)duration) / 60;
      seconds = duration - ((float)minutes * 60.0);
    }
    
    s += getString((float)seconds) + "s";

    return s;
  }


  string Timer::getStringMemory ( size_t size )
  {
    ostringstream oss;

    if      (size >> 30) oss << fixed << setprecision(1) << ((float)size)/powf(2.0,30.0) << "Gb";
    else if (size >> 20) oss << fixed << setprecision(1) << ((float)size)/powf(2.0,20.0) << "Mb";
    else if (size >> 10) oss << size << "Kb";
    else                 oss << size << " bytes";

    return oss.str();
  }


} // End of Hurricane namespace.
