
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
// |  C++ Header  :       "./equinox/EquinoxCollections.h"           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


/********************************************************************
*  This file contains :                                             *  
*    - AllOccurrencesCollection                                     *
*          Return nets and components of Equi and all subEquis      *
*                                                                   *
*    - EquiComponentOccurrencesCollection                           *
*          Return all components Occurrences of hyperNets           *
*          of this Equi  and all subEquis.                          *
*                                                                   *
*     - CurrentComponentOccurrencesCollection                       * 
*          Return all components Occurrences of hyperNets           * 
*          of this Equi.                                            *
********************************************************************/


#ifndef   __EQUINOX_EQUINOX_COLLECTIONS__
#define   __EQUINOX_EQUINOX_COLLECTIONS__


namespace Equinox {

  class Equi;
  using namespace Hurricane;


  class AllOccurrencesCollection : public Collection<Occurrence> {


  public : typedef Collection<Occurrence> Inherit;

  public : class Locator : public Hurricane::Locator<Occurrence> {
  // *************************************************************

	     public : typedef Hurricane::Locator<Occurrence> Inherit ;
          
             private : const Equi* _equi;
             private : int _state;            // state = 0 locator is invalid
             private : OccurrenceLocator _componentLocator;
             private : OccurrenceLocator _equioccurrenceLocator;
	     private : OccurrenceLocator _occurrenceLocator;
		       
             public : Locator();
	     public : Locator(const Equi* equi);
		     
             public : Locator(const Locator& locator);

	     public : Locator& operator=(const Locator& locator);

	     public : virtual Occurrence getElement() const;
	     public : virtual Hurricane::Locator<Occurrence>* getClone() const;

	     public : virtual bool isValid() const;
		      
	     public : virtual void progress();
		      
             public : virtual string _getString() const;
		      
         };
	  
// Attributs
// *********

       private : const Equi* _equi;

// Constructors
// ************

       public : AllOccurrencesCollection();
       public : AllOccurrencesCollection(const Equi* equi);
       public : AllOccurrencesCollection(const AllOccurrencesCollection&);
		
// Operators
// *********
    
       public : AllOccurrencesCollection& operator=(const AllOccurrencesCollection&);		
// Accessors
// *********
	
       public : virtual Collection<Occurrence>* getClone() const;		
       public : virtual Hurricane::Locator<Occurrence>* getLocator() const;		

// Others
// ******
		
       public : virtual string _getString() const ;
		
  }; // end of class AllOccurrencesCollection






 class EquiComponentOccurrencesCollection : public Collection<Occurrence> {


  public : typedef Collection<Occurrence> Inherit;

  public : class Locator : public Hurricane::Locator<Occurrence> {
  // *************************************************************

	     public : typedef Hurricane::Locator<Occurrence> Inherit ;
          
             private : const Equi* _equi;
             private : int _state;            // state = 0 locator is invalid
             private : OccurrenceLocator _componentLocator;
             private : OccurrenceLocator _equioccurrenceLocator;
	     private : OccurrenceLocator _occurrenceLocator;
		       
             public : Locator();
	     public : Locator(const Equi* equi);
		     
             public : Locator(const Locator& locator);

	     public : Locator& operator=(const Locator& locator);

	     public : virtual Occurrence getElement() const;
	     public : virtual Hurricane::Locator<Occurrence>* getClone() const;

	     public : virtual bool isValid() const;
		      
	     public : virtual void progress();
		      
             public : virtual string _getString() const;
		      
         };
	  
// Attributs
// *********

       private : const Equi* _equi;

// Constructors
// ************

       public : EquiComponentOccurrencesCollection();
       public : EquiComponentOccurrencesCollection(const Equi* equi);
       public : EquiComponentOccurrencesCollection(const EquiComponentOccurrencesCollection&);
		
// Operators
// *********
    
       public : EquiComponentOccurrencesCollection& operator=(const EquiComponentOccurrencesCollection&);		
// Accessors
// *********
	
       public : virtual Collection<Occurrence>* getClone() const;		
       public : virtual Hurricane::Locator<Occurrence>* getLocator() const;		

// Others
// ******
		
       public : virtual string _getString() const ;
		
  }; // end of class EquiComponentOccurrencesCollection









 class CurrentComponentOccurrencesCollection : public Collection<Occurrence> {


  public : typedef Collection<Occurrence> Inherit;

  public : class Locator : public Hurricane::Locator<Occurrence> {
  // *************************************************************

	     public : typedef Hurricane::Locator<Occurrence> Inherit ;
          
             private : const Equi* _equi;
             private : int _state;            // state = 0 locator is invalid
             private : OccurrenceLocator  _componentLocator;
             private : OccurrenceLocator _netoccurrenceLocator;
	     private : ComponentLocator  _occurrenceLocator;
		       
             public : Locator();
	     public : Locator(const Equi* equi);
		     
             public : Locator(const Locator& locator);

	     public : Locator& operator=(const Locator& locator);

	     public : virtual Occurrence getElement() const;
	     public : virtual Hurricane::Locator<Occurrence>* getClone() const;

	     public : virtual bool isValid() const;
		      
	     public : virtual void progress();
		      
             public : virtual string _getString() const;
		      
         };
	  
// Attributs
// *********

       private : const Equi* _equi;

// Constructors
// ************

       public : CurrentComponentOccurrencesCollection();
       public : CurrentComponentOccurrencesCollection(const Equi* equi);
       public : CurrentComponentOccurrencesCollection(const CurrentComponentOccurrencesCollection&);
		
// Operators
// *********
    
       public : CurrentComponentOccurrencesCollection& operator=(const CurrentComponentOccurrencesCollection&);		
// Accessors
// *********
	
       public : virtual Collection<Occurrence>* getClone() const;		
       public : virtual Hurricane::Locator<Occurrence>* getLocator() const;		

// Others
// ******
		
       public : virtual string _getString() const ;
		
  }; // end of class CurrentComponentOccurrencesCollection









} //end of namespace




#endif  // __EQUINOX_EQUINOX_COLLECTIONS__
