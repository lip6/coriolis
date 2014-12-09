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
// |  C++ Header  :       "./equinox/SweepLine.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef _EQUINOX_SWEEPLINE_H
#define _EQUINOX_SWEEPLINE_H

#define INTERVALTREE 1  /*Without use STL map, VERY SLOW*/

//#define ASSERT 1


#include  "crlcore/Utilities.h"

namespace Hurricane {
  class BasicLayer;
  class DataBase;
}

namespace CRL {
  class ToolEngine;
}
namespace Equinox {
  
  class EquinoxEngine;
  class Strategy;

  using namespace Hurricane;
  using CRL::ToolEngine;

// -------------------------------------------------------------------
// Class  :  "Equinox::SweepLine".
 

  template < typename ITEM, typename ENGINE > 
  class SweepLine  {
    
  private : 
    map<ITEM , IntervalTreeNode*> _intervalTreeNodeMap; 
    int                            intervalsize;
    float                          intervaltime;
    int                            contactssize;
    
    
    struct  ItemComparator{
      inline bool operator() ( ITEM const& t1,  ITEM const& t2) const;
    };
    
    
    
  public:
    typedef  vector < ITEM >                 ItemList;
    
    typedef  set < ITEM , ItemComparator   >  IntervalSet;
#ifdef INTERVALTREE
    typedef  map < BasicLayer*, IntervalTree* > IntervalSets;
#else
    typedef  map < BasicLayer*, IntervalSet* > IntervalSets;
#endif
    
  public: 
    static      SweepLine*      create               (ENGINE, 
						      Strategy*);
    virtual     void            destroy              ();
    
  public:
    /**/        Record*         _getRecord           () const;
    /**/        string          _getString           () const;
    /**/        Box             getBoundingBox       () const;
    /**/        void            run                  ( vector < ITEM  >* _itemsByXmin
                                                     , vector < ITEM  >* _itemsByXmax
                                                     , bool stopToMax = false
                                                     , int max = 0 );
    
    /**/        void            initIntervalSets           () ;
    /**/inline        void      insertInterval       (ITEM newitem);
    /**/inline        void      removeInterval       (ITEM newitem);
    
  public:

    virtual     const Name&     getName              () const;
    /**/        Strategy*       getStrategy          () ;
  protected:
    static      const Name      _extensionName;
    /**/        ENGINE     _engine;
    /**/        IntervalSets    _intervalSets; 
    /**/        Strategy*       _strategy;
    
  protected:
    /**/        void            _postCreate          ();
    /**/        void            _preDestroy          ();
    /**/                        SweepLine            ( ENGINE,
						       Strategy*    );
    virtual                     ~SweepLine            ();
    
    
  }; //end of SweepLine
  
  
  
  template < typename ITEM, typename ENGINE > 
  inline bool SweepLine<ITEM,ENGINE>::ItemComparator::operator() ( ITEM const& t1,  ITEM const& t2) const
  {
#ifdef ASSERT
    if (!t1 || !t2) throw Error("SweepLine Assert Error");
#endif
    if( t1->getYmin() < t2->getYmin() )
      return true;
    
    if( t1->getYmin() ==  t2->getYmin() )
      return t1 < t2;
    
    return false;
    
  }
  
  
  
  
  
  
  
  
  
  template < typename ITEM, typename ENGINE > 
  const Name  SweepLine<ITEM,ENGINE>::_extensionName = "Equinox::SweepLine";
  
  
  
  template < typename ITEM, typename ENGINE > 
  Strategy* SweepLine<ITEM,ENGINE>::getStrategy () 
  { 
    if (!_strategy) throw Error("Aucun strategy transmise pour la sweepLine");
    return _strategy;
  }
  
  template < typename ITEM, typename ENGINE > 
  SweepLine<ITEM,ENGINE>*      SweepLine<ITEM,ENGINE>::create     (ENGINE engine, Strategy* strategy) {
    
    SweepLine* sweepLine  = new SweepLine ( engine , strategy );
    
    if(!sweepLine){
      throw Error("can't create SweepLine : allocation failed");
    }
    
    sweepLine->_postCreate ();
    return sweepLine ;

}
  template < typename ITEM, typename ENGINE > 
  /**/ SweepLine<ITEM,ENGINE>::SweepLine   (ENGINE engine, Strategy* strategy) : 
    // ExtensionGo(cell), 
    _engine(engine),
    _intervalSets(),
    _strategy(strategy)
  {
    
  };  

  template < typename ITEM, typename ENGINE > 
  void SweepLine<ITEM,ENGINE>::_postCreate (){
    // ExtensionGo::_postCreate(); 
    initIntervalSets();
}
    template < typename ITEM, typename ENGINE > 
  /**/ SweepLine<ITEM,ENGINE>::~SweepLine  (){}
  
  template < typename ITEM, typename ENGINE > 
  void SweepLine<ITEM,ENGINE>::destroy     (){ _preDestroy(); delete this; }
  
  template < typename ITEM, typename ENGINE > 
  void SweepLine<ITEM,ENGINE>::_preDestroy (){
    //   ExtensionGo::_preDestroy();
    typename IntervalSets::iterator it_intervaltree_begin = _intervalSets.begin(),
      it_intervaltree_end = _intervalSets.end();
    
    while(it_intervaltree_begin!=it_intervaltree_end){
      delete (*it_intervaltree_begin).second;
      it_intervaltree_begin++;
    }
  }
  



    template < typename ITEM, typename ENGINE > 
  void SweepLine<ITEM,ENGINE>::initIntervalSets() 
  {
    
    forEach ( BasicLayer*, basicLayer, DataBase::getDB()->getTechnology()->getBasicLayers()) {
      if (Strategy::isExtractableLayer(*basicLayer) ) {
#ifdef INTERVALTREE
	(_intervalSets)[*basicLayer] = new IntervalTree();
#else
	(_intervalSets)[*basicLayer] = new IntervalSet();
#endif
      }
    }

  }



    template < typename ITEM, typename ENGINE > 
  Record* SweepLine<ITEM,ENGINE>::_getRecord() const
  // ***********************
  {
    //Record* record = Inherit::_getRecord();
    Record* record = NULL;
    if ( !record )
      record = new Record ( getString ( this ) );    
    return record;
  }
  
  template < typename ITEM, typename ENGINE > 
  string SweepLine<ITEM,ENGINE>::_getString() const
  // ****************************
  {
    return "<" + _TName ( "SweepLine" ) + ">";
  }



  template < typename ITEM, typename ENGINE > 
  const Name& SweepLine<ITEM,ENGINE>::getName       () const {
    return _extensionName;
  }
  


  template < typename ITEM, typename ENGINE > 
inline  void SweepLine<ITEM,ENGINE>::insertInterval(ITEM item)
  {
#ifdef ASSERT
    if (!item) throw Error("SweepLine : Insert null");
#endif
#ifdef INTERVALTREE
    //recherche du Layer du Item
#ifdef ASSERT 
    if (_intervalSets.find(item->getBasicLayer()) == _intervalSets.end())
      throw Error("Layer inconnu dans insertInterval");
#endif
    IntervalTree*  intervaltree = _intervalSets.find(item->getBasicLayer())->second;
    // Récuperation des intersections avec ce Item dans enumResultStack
    stack <Interval*>* enumResultStack = (intervaltree->Enumerate((item->getYmin()),( item->getYmax()) ));
#else
    typename IntervalSets::const_iterator mySetsIt = _intervalSets.find(item->getBasicLayer());
#ifdef ASSERT 
    if (_intervalSets.find(item->getBasicLayer()) == _intervalSets.end())
      throw Error("Layer inconnu dans insertInterval");
#endif
    IntervalSet* intervaltree = mySetsIt->second;
    stack <Interval*>* enumResultStack = new stack<Interval*>;
    typename IntervalSet::iterator itmid2down    = intervaltree->lower_bound( item ) ;
    typename IntervalSet::iterator itbegin2mid   = intervaltree->begin() ; 
    while ( itmid2down != intervaltree->end() ) 
      {
    	if ( (*itmid2down)->getYmin() <= item->getYmax() ) { enumResultStack->push((*itmid2down)); itmid2down++;}
    	else break; 
      }
    
    while ( itbegin2mid != ( intervaltree->upper_bound( item ) ) ) 
      {
    	if ( (*itbegin2mid)->getYmax() >= item->getYmin() ) { enumResultStack->push((*itbegin2mid));}
    	itbegin2mid++;
      }
#endif
    _engine->insertInterval(item,enumResultStack);


#ifdef INTERVALTREE
    _intervalTreeNodeMap[(item)] = intervaltree->Insert((item));
#else
    //Ajout du Item aux intervals
    intervaltree->insert(item);
#endif

    delete enumResultStack;

}  
  
  
  template < typename ITEM, typename ENGINE > 
 inline void SweepLine<ITEM,ENGINE>::removeInterval(ITEM item)
  {

    // /*DEBUG*/    cmess1 << "                             o removeInterval  : " << endl;


    //recherche du Layer du Item
    typename IntervalSets::const_iterator mySetsIt = _intervalSets.find(item->getBasicLayer());

#ifdef ASSERT
    //Valide la liste des layers
    if(mySetsIt ==_intervalSets.end()) throw Error("SweepLine : La clé des Layers n'existe pas pas");

    //Valide l'item à supprimer
    typename IntervalSet::const_iterator mySetIt   = mySetsIt->second->find(item);
    if(mySetIt  == mySetsIt->second->end()) throw Error("SweepLine : Un item à detruire du champ n'existe pas");
#endif

#ifdef INTERVALTREE
    //Suppression du Item
    mySetsIt->second->DeleteNode(_intervalTreeNodeMap[item]);
   _intervalTreeNodeMap.erase(item);  
#else
   mySetsIt->second->erase(item);
#endif


   _engine->removeInterval(item);
  

    
    ///*DEBUG*/    cmess1 << "                                - destroy tile  OK " << endl;
    ///*DEBUG*/    cmess1 << "                                - " << mySetsIt->second->size() << " interval(s) sur le Layer." <<endl;
}  

  






  template < typename ITEM, typename ENGINE > 
  void SweepLine<ITEM,ENGINE>::run (vector < ITEM  >* _itemsByXmin,  vector < ITEM  >* _itemsByXmax,bool stopAtMax, int max )
  {

    ///*DEBUG*/    cmess1 << "                             o SweepLine :: run  : " << endl;

    //Making iterators
    typename vector<ITEM>::iterator it1       = _itemsByXmin->begin();
    typename vector<ITEM>::iterator it1_end   = _itemsByXmin->end();
    typename vector<ITEM>::iterator it2       = _itemsByXmax->begin();
    typename vector<ITEM>::iterator it2_end   = _itemsByXmax->end();
    typename vector<ITEM>::iterator it2_begin = _itemsByXmax->begin();
      
    while(it2!=it2_end)
      {
	if( (it1 != it1_end) && ((*it1)->getXmin() <= (*it2)->getXmax()) )   /* Get some tiles  */
          {
	   typename vector<ITEM>::iterator upper_it = upper_bound(it1, it1_end, *it1, CompByXmin_Upper_Bound<ITEM>() );
	    while(it1 != upper_it) 
	      {
		insertInterval(*it1);
		it1++;
	      }
          }
	else                                                                /* Throw some tiles */
          {
	    if(stopAtMax)
	      if ((*it2)->getXmax() >= max )
		break;
	    
	   typename  vector<ITEM>::iterator upper_it = upper_bound(it2, it2_end, *it2, CompByXmax_Upper_Bound<ITEM>() );
	    while(it2 != upper_it) 
	      {
		removeInterval(*it2);
		it2++;
	      }
          }	
      } // end while   

    if (_itemsByXmin->size()) {
   ///*DEBUG*/   intervalsize= intervalsize / _tilesByXmin->size();
   ///*DEBUG*/   contactssize= contactssize / _tilesByXmin->size();
   ///*DEBUG*/   intervaltime= intervaltime / _tilesByXmin->size();
    
    ///*DEBUG*/  cmess1 << "   -- FIN SWEEPLINE : intervalsize="<< intervalsize<< " contactssize=" <<contactssize<< " intervaltime=" <<intervaltime <<endl;
    }
    _itemsByXmin->clear();
    _itemsByXmax->erase(it2_begin, it2);
    
  }








} // namespace equinox




#endif  // _EQUINOX_SWEEPLINE_H
