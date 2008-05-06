// ****************************************************************************************************
// File: DtrAccess.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "DataBase.h"
#include "Technology.h"
#include "BasicLayer.h"
#include "Error.h"

#include "RdsUnit.h"
#include "DtrAccess.h"


extern void ParseDtr(const char*, Hurricane::DtrAccess*);


namespace Hurricane {


// ****************************************************************************************************
// DtrAccess implementation
// ****************************************************************************************************


// static data defintion
// *********************
DtrAccess* DtrAccess::_singleton = NULL;

map<string, DtrAccess*> DtrAccess::_registry;

DtrAccess * DtrAccess::create() {
   DtrAccess* dtraccess = new DtrAccess();

   dtraccess->_postCreate();
   
   return dtraccess;
}


void DtrAccess::_postCreate() {
   const char* dtrFileName = getenv("DTR_FILE");
   if(!dtrFileName) {
     throw Error("Can't not get Macro DTR_FILE.");
   }

   // Use API of DtrParser for get technology informations 
   // ****************************************************
   ParseDtr(dtrFileName, this);

   // Traduit Micro to RdsUnit
   // ************************
   for (Label2RuleMap::iterator lrmit = _label2ruleMap.begin();
           lrmit != _label2ruleMap.end();
           ++lrmit) {
       for (list<double>::iterator ldit = lrmit->second.begin();
               ldit != lrmit->second.end();
               ++ldit) {
            _label2RdsRuleMap[lrmit->first].push_back(ConvertRealToRdsUnit(*ldit)); 
       }
   }

   // get Objet Layer from Technology with its name. 
   // **********************************************
   
  DataBase* db = getDataBase();  

  if(!db) {
      throw Error("In getV1Trans::Generate : can't find DataBase.");
  }

  Technology* tech = db->getTechnology();

  if (!tech) {
      throw Error("In getV1Trans::Generate : can't find Technology.");
  }


   map<string, list<string> >::iterator it_layermap = _label2layerNameMap.begin(),
     it_end_layermap = _label2layerNameMap.end();

   while(it_layermap != it_end_layermap) {
   
     list<string>::iterator m = (*it_layermap).second.begin(), 
       n = (*it_layermap).second.end();

     while(m!=n) {
       Layer * layer = tech->getLayer(Name(*m));
       if(!layer) { 
           layer = BasicLayer::create(tech, Name(*m), BasicLayer::Type::UNDEFINED, 100);
	 //throw Error("Error : in function DtrAccess::_postCreate , Can't find Layer " 
	 //    + getString(*m) + " in technology file when parser DtrFile.");
//	 cerr << Warning("In function DtrAccess::_postCreate , Can't find Layer " 
//	     + getString(*m) + " in technology file when parser DtrFile");
       }
	 
       _label2layerMap[(*it_layermap).first].push_back(layer);
       m++;
     }
     it_layermap++;
   }
   
}


DtrAccess * DtrAccess::getDtrAccess() {
  // User or environnement supplies this at startup
  // **********************************************
  const char * singleton_name = getenv("DTRACCESS_SINGLETON");

  if(!singleton_name) { // if MACRO IS INVALID
     if(!_singleton) {
        _singleton = DtrAccess::create();
     }
  } else {  
    if(!_singleton){
       if( !(_singleton=LookUp(string(singleton_name))) ) // if singleton hasn't been registered
          _singleton = DtrAccess::create();
    }
  }
  return _singleton;
}

GenericCollection<double> DtrAccess::getRuleByLabel(const string& label) const
// ***************************************************************************
{
    map<string, list<double> >::const_iterator i = _label2ruleMap.find(label);

    if(i==_label2ruleMap.end()) 
      throw Error("Can't find in DtrFile rule the label : " + getString(label));

    return getCollection((*i).second);
}

GenericCollection<long> DtrAccess::getRdsRuleByLabel(const string& label) const
// ******************************************************************************
{
    map<string, list<long> >::const_iterator i = _label2RdsRuleMap.find(label);

    if(i==_label2RdsRuleMap.end()) 
      throw Error("Can't find in DtrFile The Rds Value of Rule by label : " + getString(label));

    return getCollection((*i).second);
}


GenericCollection<string> DtrAccess::getLayerNamesByLabel(const string& label) const
// *********************************************************************************
{
    map<string, list<string> >::const_iterator i = _label2layerNameMap.find(label);

    if(i==_label2layerNameMap.end())
      throw Error("Can't find in DtrFile layers the label : " + label);

    return getCollection((*i).second);
}


GenericCollection<Layer*> DtrAccess::getLayersByLabel(const string& label) const
// ******************************************************************************
{
   map<string, list<Layer*> >::const_iterator i = _label2layerMap.find(label);

   if(i==_label2layerMap.end()) 
     throw Error("Can't find in DtrFile objet Layer by label : " + label );

   return getCollection((*i).second);
}


GenericCollection<double> DtrAccess::getElectricalsByLabel(const string& label) const
// **********************************************************************************
{
    map<string, list<double> >::const_iterator i = _label2electricalMap.find(label);

    if(i==_label2electricalMap.end())
      throw Error("Can't find in DtrFile electricals by label : " + label);

    return getCollection((*i).second);
}


int DtrAccess::getModellingByLabel(const string& label) const
// **********************************************************
{
    map<string, int>::const_iterator i = _label2modellingMap.find(label);

    if(i==_label2modellingMap.end())
      throw Error("Can't find in DtrFile modelling by label : " + getString(label));

    return (*i).second;
}


double DtrAccess::getSimpleCapaMimByLabel(const string& label) const
// ****************************************************************
{
    map<string, double>::const_iterator i = _label2simplecapamimMap.find(label);

    if(i==_label2simplecapamimMap.end())
      throw Error("Can't find in DtrFile simple capa mim by label : " + getString(label));

    return (*i).second;
}


// ------------------------------------------------------
// Function "DtrAccess::LookUp(const string&)". 
//

  /* \static DtrAccess* DtrAccess::LookUp(const string& singletonname) 
   * \param   singletonname   normally the name of derive class of DtrAccess.
   * \return  addresse of objet singleton of derive class if success.
   *          NULL if failure.
   *
   * Find the singleton objet if it exist according to the singletonname.
   *
   */

DtrAccess* DtrAccess::LookUp(const string& singletonname)
// ******************************************************
{
   map<string, DtrAccess*>::iterator i = _registry.find(singletonname);

   if(i==_registry.end())
     return NULL;

   return (*i).second;
}  


// ------------------------------------------------------------------------
// Function Register(const string&, DtrAccess*)

   /*  \void DtrAccess::Register(const string& singletonname, DtrAccess* dtraccess)
    *  \param  singletonname   the name of derive class.  
    *  \param  dtraccess       addresse of objet singleton of derive class. 
    *   
    *   
    *  This function is to be called when the singleton objet of derive class is 
    *  created.
    *
    */   

void DtrAccess::Register(const string& singletonname, DtrAccess* dtraccess)
// *************************************************************************
{
   _registry[singletonname]=dtraccess;
}  


string DtrAccess::_getString() const
// **********************************
{
  string s("Singleton DtrAccess");
  return s;
}


Record* DtrAccess::_getRecord() const
// **********************************
{
  Record* record = new Record(_getString());
  return record;
}

}

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const Hurricane::DtrAccess& access)
// **********************************************
{
  return access._getString();
}


