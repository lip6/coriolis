// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                            Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ChannelRouting.cpp"                          |
// +-----------------------------------------------------------------+


#include "bora/ChannelRouting.h"


namespace Bora {

  using namespace std;
  

// -------------------------------------------------------------------
// Class  :  "Bora::ChannelRouting".

  
  ChannelRouting::ChannelRouting ()
  { }


  ChannelRouting::~ChannelRouting ()
  { }


  ChannelRouting* ChannelRouting::create ()
  {
    return new ChannelRouting();
  }


  void  ChannelRouting::destroy ()
  {
    delete this;
  }


  void ChannelRouting::reset ()
  {
    _limits.clear();
    _counts.clear();
  }


  int  ChannelRouting::getMaxCount () const
  {
    int maxCount = 0;
    for ( int count : _counts ) maxCount = std::max( maxCount, count );
    return maxCount;
  }


  void  ChannelRouting::insertChannel ( DbU::Unit xy1, DbU::Unit xy2, unsigned int w )
  {
    cdebug_log(535,0) << "ChannelRouting::insertChannel(DbU::Unit,DbU::Unit,unsigned) ["
                      << DbU::getValueString(xy1) << " : "
                      << DbU::getValueString(xy2) << "] width:"
                      << w << endl;
    
    for ( unsigned int i=0; i<w; ++i ) insertChannel( xy1, xy2 );
  }


  void  ChannelRouting::insertChannel ( DbU::Unit xy1, DbU::Unit xy2 )
  {
    cdebug_log(535,0) << "ChannelRouting::insertChannel(DbU::Unit,DbU::Unit) ["
                      << DbU::getValueString(xy1) << " : "
                      << DbU::getValueString(xy2) << "]" << endl;
    
    DbU::Unit min = xy1;
    DbU::Unit max = xy2;

    if (_limits.empty()) {
      _limits.push_back( min );
      _limits.push_back( max );
      _counts.push_back( 1 );
    } else {
      if        (max < _limits.front()) {
        _limits.insert( _limits.begin(), max );
        _limits.insert( _limits.begin(), min );
        _counts.insert( _counts.begin(), 0   );
        _counts.insert( _counts.begin(), 1   );
      } else if (min > _limits.back()) {
        _limits.push_back( min );
        _limits.push_back( max );
        _counts.push_back( 0   );
        _counts.push_back( 1   );
      } else {
        int index = 0;
        vector<DbU::Unit>::iterator itL = _limits.begin();
        vector<int>::iterator       itC = _counts.begin();

        if (min < (*itL)) {
          _limits.insert( itL, min );
          _counts.insert( itC, 1   );
        } else {
          while( ((*itL) < min) and (itL != _limits.end()) ) {
            itL++;
            index++;
            if (itC != _counts.end())  itC++;
          }
        
          if ((*itL) != min) {
            _limits.insert( itL, min      );
            _counts.insert( itC, *(itC-1) );
            itL = _limits.begin() + index;
            itC = _counts.begin() + index;
          } 
          itL = _limits.begin() + index;
          itC = _counts.begin() + index;

          while( ((*itL) < max) && (itL != _limits.end()) ){
            itL++;
            index++;
            if (itC != _counts.end()) {
              (*itC) += 1;
              itC++;
            } 
          }
          if (itL != _limits.end()) {
            if ((*(itL)) > max){
              _limits.insert( itL, max        );
              _counts.insert( itC, *(itC-1)-1 );
            } 
          } else {
            if ((*(itL-1)) != max) {
              _limits.push_back( max );
              _counts.push_back( 1   );
            }
          }
        }
      }
    }
  }


  void  ChannelRouting::print () const
  {
    cerr << "limits: ";
    for ( DbU::Unit limit : _limits ) cerr << DbU::getPhysical(limit,DbU::Micro) << " - ";
    cerr << endl;

    cerr << "count: ";
    for ( int count : _counts ) cerr << count << " - ";
    cerr << endl;
  }


}  // Bora namespace.
