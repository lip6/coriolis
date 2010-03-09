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


//! author="Igor Markov, June 15, 1997"
// freely inspired from infolines.h from UCLApack.


# ifndef  __CRL_MEMORY_H__
#   define  __CRL_MEMORY_H__

# include  <iostream>

using std::ostream;


namespace CRL {

    
// -------------------------------------------------------------------
// Class  :  "CRL::MemUsage".
//
// Prints information about the memory usage of the process in Mbytes.


  class MemUsage {
    public:
                     MemUsage        ();       
      inline double  getPeakMem      () const { return _peak; }
      inline double  getEstimate     () const { return _estimate; }
      inline         operator double () const { return _estimate; };
    protected:
             double  _peak;
             double  _estimate;
  };


  std::ostream& operator<<(std::ostream&, const MemUsage&);


// -------------------------------------------------------------------
// Class  :  "CRL::MemUsage".


  class MemChange {
    public:
              MemChange       ();
      void    resetMark       ();       // does the same as ctor.
              operator double () const; // returns the change wrt the stored value.
    protected:
      double  _memUsageMark;            // in Mbytes.
  };


} // End of CRL namespace.


# endif // __CRL_MEMORY_H__
