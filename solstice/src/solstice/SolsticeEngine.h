
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./solstice/SolsticeEngine.h"              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __SOLSTICE_SOLSTICE_ENGINE__
#define  __SOLSTICE_SOLSTICE_ENGINE__

#include  "crlcore/ToolEngine.h"

namespace Hurricane {
  class Occurrence;
  class Cell;
  class Path;
}

namespace Equinox {
  class Equi;
  class Interval;
  class Strategy;

}
namespace Solstice {

  using  namespace std;
  using  namespace Hurricane;

  using CRL::ToolEngine;

  using Equinox::Equi;
  using Equinox::Strategy;

  class Brick;
  class ShortCircuitError;
  class RoutingError;


  class SolsticeEngine : public ToolEngine {

  public:
    static         SolsticeEngine*            create                  (Cell*);
    static         SolsticeEngine*            get                     (const Cell* );    
    static  inline Name&                      getStaticName           ();
    static         Occurrence                 getTopNetOccurrence     (Occurrence occurrence);
					    			      
  protected: 				    			      
    static         Strategy *                 getStrategy             ();
    static         void                       setStrategy             (Strategy *);
					    			      
    static         void                      _depthCreate             ( Cell*);
    static         Cell*                      getCommonPath           (Path path1, 
								       Path path2, 
								       Path& newpath1, 
								       Path& newpath2);
    static         void                       setIsComparedTrue       (Cell * cell);
    static         void                       getAllSolstices         (Cell* cell,
								       set<SolsticeEngine*>& solstices);
					    			      
  public:				    			      
    virtual inline const Name&                getName                 () const;
    virtual        Record*                   _getRecord               () const;
    virtual inline string                    _getString               () const;
    virtual inline string                    _getTypeName             () const;

  public :
    /**/    inline void                       destroy                 ();
    /**/           void                       flushErrors             ();
    /**/           void                       compare                 ();
    /**/           void                       insertInterval          (Brick*,
								       stack <Equinox::Interval*>*enumResultStack);
    /**/           void                       removeInterval          (Brick* item);
    inline         const set<RoutingError*>*  getRoutingErrorSet      ();

  protected :
    /**/           void                       clearErrors             ();
    inline         void                       setIsCompared           (const bool flag);
    /**/           void                       runComparison           ();
    /**/           void                       detectShortCircuit      (Equi* equi);
    inline         bool                       isCompared              () const;			   
  private:		           
    // Attributes.	    
    static         Strategy *                _strategy;
    static         Name                      _toolName;	
    /**/           bool                      _isCompared;  
    /**/           set<RoutingError*>*       _routingErrors;
    
  protected:
    // Constructors & Destructors.
    /**/                                      SolsticeEngine          (Cell*);
    virtual                                  ~SolsticeEngine          ();
    virtual inline void                      _postCreate              ();
    virtual inline void                      _preDestroy              ();
    /**/           void                      _depthDestroy            ();

private:
    /**/                                      SolsticeEngine          (const SolsticeEngine&);
    /**/           SolsticeEngine&            operator=               (const SolsticeEngine&);

 }; // End of class SolsticeEngine

  //inline functions
  inline Name&                     SolsticeEngine::getStaticName          ()                 { return _toolName;};
  inline bool                      SolsticeEngine::isCompared             () const           { return _isCompared; };
  inline void                      SolsticeEngine::setIsCompared          (const bool flag)  { _isCompared = flag; }; 
  inline const set<RoutingError*>* SolsticeEngine::getRoutingErrorSet     ()                 { return _routingErrors;};
  inline const Name&               SolsticeEngine::getName                () const           { return _toolName;};
  inline string                    SolsticeEngine::_getString             () const           { return "<Solstice>";};
  inline string                    SolsticeEngine::_getTypeName           () const           { return "Solstice";};
  inline void                      SolsticeEngine::_postCreate            ()                 { ToolEngine::_postCreate ();};
  inline void                      SolsticeEngine::_preDestroy            ()                 { ToolEngine::_preDestroy ();flushErrors();};
  inline void                      SolsticeEngine::destroy                ()                 { _preDestroy(); delete this;};


} // End of Solstice namespace.


#endif  // __SOLSTICE_SOLSTICE_ENGINE__
