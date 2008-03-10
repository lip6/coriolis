// ****************************************************************************************************
// File: DtrAccess.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "DtrAccess.h"
#include "RdsUnit.h"
#include "Error.h"

#include "DataBase.h"
#include "Technology.h"

extern void ParseDtr(const char*, Hurricane::DtrAccess*);


BEGIN_NAMESPACE_HURRICANE


// ****************************************************************************************************
// DtrAccess implementation
// ****************************************************************************************************


// static data defintion
// *********************
DtrAccess* DtrAccess::_instance = NULL;

map<string, DtrAccess*> DtrAccess::_registry;

DtrAccess::DtrAccess()
// *******************
{
}


DtrAccess * DtrAccess::Create() {
   DtrAccess * dtraccess = new DtrAccess();

   dtraccess->_PostCreate();
   
   return dtraccess;
}


void DtrAccess::_PostCreate() {
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

   // Get Objet Layer from Technology with its name. 
   // **********************************************
   
  DataBase* db = GetDataBase();  

  if(!db) {
      throw Error("In GetV1Trans::Generate : can't find DataBase.");
  }

  Technology* tech = db->GetTechnology();

  if (!tech) {
      throw Error("In GetV1Trans::Generate : can't find Technology.");
  }


   map<string, list<string> >::iterator it_layermap = _label2layerNameMap.begin(),
     it_end_layermap = _label2layerNameMap.end();

   while(it_layermap != it_end_layermap) {
   
     list<string>::iterator m = (*it_layermap).second.begin(), 
       n = (*it_layermap).second.end();

     while(m!=n) {
       Layer * layer = tech->GetLayer(Name(*m));
       if(!layer) { 
	 throw Error("Error : in function DtrAccess::_PostCreate , Can't find Layer " 
	     + GetString(*m) + " in technology file when parser DtrFile.");
//	 cerr << Warning("In function DtrAccess::_PostCreate , Can't find Layer " 
//	     + GetString(*m) + " in technology file when parser DtrFile");
       }
	 
       _label2layerMap[(*it_layermap).first].push_back(layer);
       m++;
     }
     it_layermap++;
   }
   
}


DtrAccess * DtrAccess::Instance() {
  // User or environnement supplies this at startup
  // **********************************************
  const char * singleton_name = getenv("DTRACCESS_SINGLETON");

  if(!singleton_name) { // if MACRO IS INVALID
     if(!_instance) {
        _instance = DtrAccess::Create();
     }
  } else {  
    if(!_instance){
       if( !(_instance=LookUp(string(singleton_name))) ) // if singleton hasn't been registered
          _instance = DtrAccess::Create();
    }
  }
  return _instance;
}


void DtrAccess::_PreDelete()
// ***********************
{
  // Do something
  // ************
}


void DtrAccess::Delete()
// ********************
{
   _PreDelete();
   delete this;
}


GenericCollection<double> DtrAccess::GetRuleByLabel(const string& label) const
// ***************************************************************************
{
    map<string, list<double> >::const_iterator i = _label2ruleMap.find(label);

    if(i==_label2ruleMap.end()) 
      throw Error("Can't find in DtrFile rule the label : " + GetString(label));

    return GetCollection((*i).second);
}


GenericCollection<long> DtrAccess::GetRdsRuleByLabel(const string& label) const
// ******************************************************************************
{
    map<string, list<long> >::const_iterator i = _label2RdsRuleMap.find(label);

    if(i==_label2RdsRuleMap.end()) 
      throw Error("Can't find in DtrFile The Rds Value of Rule by label : " + GetString(label));

    return GetCollection((*i).second);
}


GenericCollection<string> DtrAccess::GetLayerNamesByLabel(const string& label) const
// *********************************************************************************
{
    map<string, list<string> >::const_iterator i = _label2layerNameMap.find(label);

    if(i==_label2layerNameMap.end())
      throw Error("Can't find in DtrFile layers the label : " + label);

    return GetCollection((*i).second);
}


GenericCollection<Layer*> DtrAccess::GetLayersByLabel(const string& label) const
// ******************************************************************************
{
   map<string, list<Layer*> >::const_iterator i = _label2layerMap.find(label);

   if(i==_label2layerMap.end()) 
     throw Error("Can't find in DtrFile objet Layer by label : " + label );

   return GetCollection((*i).second);
}


GenericCollection<double> DtrAccess::GetElectricalsByLabel(const string& label) const
// **********************************************************************************
{
    map<string, list<double> >::const_iterator i = _label2electricalMap.find(label);

    if(i==_label2electricalMap.end())
      throw Error("Can't find in DtrFile electricals by label : " + label);

    return GetCollection((*i).second);
}


int DtrAccess::GetModellingByLabel(const string& label) const
// **********************************************************
{
    map<string, int>::const_iterator i = _label2modellingMap.find(label);

    if(i==_label2modellingMap.end())
      throw Error("Can't find in DtrFile modelling by label : " + GetString(label));

    return (*i).second;
}


double DtrAccess::GetSimpleCapaMimByLabel(const string& label) const
// ****************************************************************
{
    map<string, double>::const_iterator i = _label2simplecapamimMap.find(label);

    if(i==_label2simplecapamimMap.end())
      throw Error("Can't find in DtrFile simple capa mim by label : " + GetString(label));

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


string DtrAccess::_GetString() const
// **********************************
{
  string s("Singleton DtrAccess");
  return s;
}


Record* DtrAccess::_GetRecord() const
// **********************************
{
  Record* record = new Record(_GetString());
  return record;
}

END_NAMESPACE_HURRICANE

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string GetString(const H::DtrAccess& access)
// **********************************************
{
  return access._GetString();
}


