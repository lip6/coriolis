
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
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       " ./equinox/EquinoxEngine.h                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __EQUINOX_EQUINOX_ENGINE__
#define  __EQUINOX_EQUINOX_ENGINE__

#include <equinox/IntervalTree.h>
#include <equinox/Equis.h>
#include <hurricane/Occurrences.h>
#include <hurricane/Components.h>
#include <equinox/EquinoxFilters.h>
#include <hurricane/SetCollection.h>
#include <equinox/Strategy.h>
namespace  {
  using namespace Hurricane;

}
namespace Hurricane {
  class Box;
  class Cell;
  class Name;
  class Record;
  class Occurrence;  
  class BasicLayer;

  template<class Type> class GenericFilter;

}

namespace CRL {
  class ToolEngine;
}

namespace Equinox {


  using std::string;
  using std::set;
  using std::map;
  using std::vector;

  using Hurricane::Cell;
  using Hurricane::Box;
  using Hurricane::Name;
  using Hurricane::Record;
  using Hurricane::GenericFilter;
  using Hurricane::BasicLayer;
  using Hurricane::OccurrenceFilter;
  using Hurricane::Occurrence;  
  using Hurricane::Occurrences;

using Hurricane::ComponentFilter;
  using CRL::ToolEngine;


  class Tile;
  class Equi;
  template<typename ITEM,typename ENGINE>
  class SweepLine;

// -------------------------------------------------------------------
// Class  :  "Equinox::EquinoxEngine".
 

  class EquinoxEngine : public ToolEngine {
    

    // Friends   
  public:
    friend  class GraphicEquinoxEngine;
    friend  class Strategy;
    friend  class WithAlimStrategy;
    friend  class WithoutAlimStrategy;
    
    // Statics
  public:
    /**/    static  EquinoxEngine*            create                     (Cell*);
    inline  static  const Name&               staticGetName              ();
    inline  static  EquinoxEngine*            get                        (const Cell* );
    inline  static  GenericFilter<Equi*>      getIsRoutingFilter         ();
    /**/    static  Strategy *                getStrategy                ();
    /**/    static  ComponentFilter           getIsUsedByExtractFilter   ();
  private:				      			         
    inline  static  void                      setStrategy                (Strategy *);
					      			         
					      			         
  public :				      			         
    inline  virtual const Name&               getName                    () const;
    inline          Equis                     getRoutingEquis            () const;
    inline          Equis                     getEquis                   () const;
    inline          unsigned long long        getNumOfEquis              ();
    inline          void                      addEqui                    (Equi* equi);
    inline          void                      removeEqui                 (Equi* equi);
    inline          map<Occurrence, Equi*>&  _getOccurrences             ();

    /**/    virtual Record*                   _getRecord                 () const;
    /**/    virtual string                    _getString                 () const;
    /**/    virtual string                    _getTypeName               () const;
    /**/            Occurrences               getComponentOccurrencesBy  (Occurrence); 
    inline          bool                      isExtracted                () const; 		          

    /**/            bool                      hasEqui                    (Equi* equi);

    /**/            void                      removeEquis                ();

    /**/            bool                     _IsToFilter                 (const Equi*) const; 
    /**/            void                     _FilterEquis                ();
    /**/            Occurrence               _GetTopNetOccurrence        (const Equi* equi) const;
    /**/            void                     _WriteNetlist               ();
    /**/            void                     _CheckOut                   (Equi*);
    /**/            void                      withAlimExtract            (unsigned int);
    /**/            void                      withoutAlimExtract         (unsigned int);
    /**/            void                      deviceExtract              (unsigned int);
    /**/            void                      extract                    (unsigned int);
    /**/            void                      test                       ();
    /**/            Box                      _getSweepLineBox            ();
    /**/            void                      insertInterval             (Tile*,stack <Equinox::Interval*>*enumResultStack);
    /**/            void                      removeInterval             (Tile* item);
  protected:
    /**/            Equi*                     getEquiByOccurrence        (Occurrence occurrence);
    /**/            Occurrence                getEquiOccurrence          (Occurrence occurrence);
    /**/            Occurrence                getUpperEquiOccurrence     (Occurrence occurrence);
    /**/            void                      setIsExtracted             (const bool flag) ;
    /**/            void                      scan                       (unsigned int);  
    /**/            void                      getOccurrencesOfEquis      ();	   
    /**/            void                      cleanUpLeafCell            ();
    /**/            void                      flushEquis                 (Cell*);	
  private:		          	      			         
    /**/            void                      selectWindowsSize          ();
    /**/            void                      initTiles                  ();
    /**/            void                      postSweepLine              ();
    /**/            void                      printEquis                 ();

    // Attributes
  private:		          	          
    static  Name                             _toolName;
    /**/    unsigned int                     _cWindows    ;
    /**/    unsigned int                     _nbWindows   ;
    
  protected:
    static  Strategy *                       _strategy;
    /**/    bool                             _isExtracted;			          
    /**/    set<Equi*>                       _equis;
    /**/    map<Occurrence, Equi*>           _occurrences;
    /**/    vector<Tile*>*                   _tilesByXmin;
    /**/    vector<Tile*>*                   _tilesByXmax;

  protected:
    // Constructors & Destructors.
    /**/                                      EquinoxEngine              (Cell*);
    /**/    virtual                          ~EquinoxEngine              ();
    /**/    virtual void                     _postCreate                 ();
    /**/    virtual void                     _preDestroy                 ();
    /**/            void                     _destroy                    ();
    /**/            void                     _depthDestroy               ();
  private:							       
    /**/                                      EquinoxEngine              (const EquinoxEngine&);
    /**/           EquinoxEngine&             operator=                  (const EquinoxEngine&);
    
    
    
    
  }; // End of class EquinoxEngine
  
  // Inline Functions.

  inline  bool                        EquinoxEngine::isExtracted        ()                    const { return _isExtracted; };
  inline  const Name&                 EquinoxEngine::getName            ()                    const { return _toolName; };
  inline  EquinoxEngine*              EquinoxEngine::get                (const Cell* cell )         { return static_cast<EquinoxEngine*>(ToolEngine::get(cell,staticGetName())); };

  inline  const   Name&               EquinoxEngine::staticGetName      ()                          { return _toolName; }
  inline  map<Occurrence,Equi*>&      EquinoxEngine::_getOccurrences    ()                          { return _occurrences; };
  inline   void                       EquinoxEngine::setStrategy        (Strategy * s)              { if (_strategy) delete _strategy;   _strategy = s;    };
  inline  EquiFilter                  EquinoxEngine::getIsRoutingFilter ()                          { return IsRoutingFilter();}
  inline  Equis                       EquinoxEngine::getRoutingEquis    ()                    const { return getCollection(_equis).getSubSet(getIsRoutingFilter()); };
  inline  Equis                       EquinoxEngine::getEquis           ()                    const { return getCollection(_equis); };
  inline  unsigned long long          EquinoxEngine::getNumOfEquis      ()                          { return _equis.size(); }
  inline  void                        EquinoxEngine::addEqui            ( Equi* equi)               { _equis.insert(equi);   }
  inline  void                        EquinoxEngine::removeEqui         ( Equi* equi)               { _equis.erase(equi); }

} // End of Equinox namespace.


#endif  // __EQUINOX_EQUINOX_ENGINE__
