// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/Pareto.h"                               |
// +-----------------------------------------------------------------+


#ifndef  BORA_PARETO_H
#define  BORA_PARETO_H


namespace Bora {

  class Pareto {
    public:
                            Pareto            ();
                           ~Pareto            ();
             void           mergePoint        ( double x, double y );
      inline int            size              () const;
      inline int            capacity          () const;
      inline const double*  xs                () const;
      inline const double*  ys                () const;
             void           clear             ();
             void           dump              ();
    private:                                  
             void           _resize           ( int newsize );
             void           _insert           ( int index, double x, double y );
             void           _remove           ( int index );
             void           _restoreMonotonic ();
    private:
      int     _capacity;
      int     _size;
      double* _xs;
      double* _ys;
  };


  inline int            Pareto::capacity () const { return _size; }
  inline int            Pareto::size     () const { return _size; }
  inline const double*  Pareto::xs       () const { return _xs; }
  inline const double*  Pareto::ys       () const { return _ys; }

}  // Bora namespace.

#endif  // BORA_PARETO_H
