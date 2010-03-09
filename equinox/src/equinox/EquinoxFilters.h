
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
// |  C++ Header  :       "./equinox/EquinoxFilters.h"               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef  __EQUINOX_EQUINOX_FILTERS__
#define  __EQUINOX_EQUINOX_FILTERS__



/**
   
   Filter List :
   - IsRouting         (Equi) 
   - IsUsedByExtract   (Component)
   - IsEqui            (Occurrence)
   - IsComponent       (Occurrence)
   - IsNet             (Occurrence)
   - IsNetOrComponent  (Occurrence)
   - IsLeafEqui        (Occurrence)

   
 **/




namespace Hurricane {
  
  template<class Type> class GenericLocator;
  template<class Type> class GenericCollection;
  template<class Type> class GenericFilter;

  template<class Type> class Filter;


  class Occurrence;
  class Component;
  class Layer;
  class Net;

}

namespace Equinox {

  using Hurricane::GenericCollection;
  using Hurricane::GenericLocator;
  using Hurricane::GenericFilter;
  using Hurricane::Filter;
  using Hurricane::Occurrence;
  using Hurricane::Component;
  using Hurricane::Layer;
  using Hurricane::Net;


  using std::string;
  
  class Equi;

  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsRoutingEquiFilter".
  
  class IsRoutingFilter : public Filter<Equi*> 
  {
    
  public:
    /**/                         IsRoutingFilter       ();
    /**/                         IsRoutingFilter       (const IsRoutingFilter&);
    /**/    IsRoutingFilter&     operator=             (const IsRoutingFilter&);
    virtual Filter<Equi*>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Equi* item) const;

  }; // End of class IsRoutingFilter



  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsUsedByExtractFilter".
  
  class IsUsedByExtractFilter : public Filter<Component*> 
  {
    
  public:
    /**/                              IsUsedByExtractFilter   ();
    virtual Filter<Component*>*       getClone                () const;	   
    virtual string                    _getString              () const;
    virtual bool                      accept                  (Component*) const;

    /**/                              IsUsedByExtractFilter   (const IsUsedByExtractFilter&);
    /**/    IsUsedByExtractFilter&    operator=               (const IsUsedByExtractFilter&);
  }; // End of class IsUsedByExtractFilter
  
  


 

  
  
  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsNetOrComponentOccurrenceFilter".
  
  class IsNetOrComponentFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                              IsNetOrComponentFilter   ();
    /**/                              IsNetOrComponentFilter   (const IsNetOrComponentFilter&);
    /**/    IsNetOrComponentFilter&   operator=                (const IsNetOrComponentFilter&);
    virtual Filter<Occurrence>*       getClone                 () const;	   
    virtual string                    _getString               () const;
    virtual bool                      accept                   (Occurrence item) const;

  }; // End of class IsNetOrComponentFilter

// -------------------------------------------------------------------
  // Class  :  "Equinox::IsComponentOccurrenceFilter".
  
  class IsComponentFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                              IsComponentFilter     ();
    /**/                              IsComponentFilter     (const IsComponentFilter&);
    /**/    IsComponentFilter&        operator=             (const IsComponentFilter&);
    virtual Filter<Occurrence>*      getClone              () const;	   
    virtual string                    _getString            () const;
    virtual bool                      accept                (Occurrence item) const;

  }; // End of class IsComponentFilter
  


  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsNetOccurrenceFilter".
  
  class IsNetFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                             IsNetFilter   ();
    /**/                             IsNetFilter   (const IsNetFilter&);
    /**/    IsNetFilter&            operator=             (const IsNetFilter&);
    virtual Filter<Occurrence>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Occurrence item) const;

  }; // End of class IsNetFilter



  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsEquiOccurrenceFilter".
  
  class IsEquiFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                         IsEquiFilter   ();
    /**/                         IsEquiFilter   (const IsEquiFilter&);
    /**/    IsEquiFilter& operator=             (const IsEquiFilter&);
    virtual Filter<Occurrence>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Occurrence item) const;

  }; // End of class IsEquiFilter
  




  // -------------------------------------------------------------------
  // Class  :  "Equinox::IsEquiLeafOccurrenceFilter".
  
  class IsLeafEquiFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                         IsLeafEquiFilter   ();
    /**/                         IsLeafEquiFilter   (const IsLeafEquiFilter&);
    /**/    IsLeafEquiFilter& operator=             (const IsLeafEquiFilter&);
    virtual Filter<Occurrence>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Occurrence item) const;

  }; // End of class IsLeafEquiFilter
  

// -------------------------------------------------------------------
  // Class  :  "Equinox::IsNotEquiLeafOccurrenceFilter".
  
  class IsNotLeafEquiFilter : public Filter<Occurrence> 
  {
    
  public:
    /**/                         IsNotLeafEquiFilter   ();
    /**/                         IsNotLeafEquiFilter   (const IsNotLeafEquiFilter&);
    /**/    IsNotLeafEquiFilter& operator=             (const IsNotLeafEquiFilter&);
    virtual Filter<Occurrence>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Occurrence item) const;

  }; // End of class IsLeafEquiFilter
  

  class NetIsUsedByExtractorFilter : public Filter<Component*> 
  {
    
  public:
    /**/                         NetIsUsedByExtractorFilter   ();
    /**/                         NetIsUsedByExtractorFilter   (const NetIsUsedByExtractorFilter&);
    /**/    NetIsUsedByExtractorFilter& operator=             (const NetIsUsedByExtractorFilter&);
    virtual Filter<Component*>*       getClone              () const;	   
    virtual string               _getString            () const;
    virtual bool                 accept                (Component* item) const;

  }; // End of class NetIsUsedByExtractorFilter




  
} // End of Equinox namespace.



#endif //  __EQUINOX_EQUINOX_FILTERS__
