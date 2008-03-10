// ****************************************************************************************************
// File: DtrAccess.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_DTRACCESS
#define HURRICANE_DTRACCESS

#include "Layer.h"
#include "Layers.h"

//BEGIN_NAMESPACE_HURRICANE


namespace Hurricane {

class DtrAccess {

#   if !defined(__DOXYGEN_PROCESSOR__)
// Attributes
// **********
    private: typedef map<string, list<double> > Label2RuleMap;
    private: static DtrAccess * _instance;

    private: Label2RuleMap _label2ruleMap;
    private: map<string, list<string> > _label2layerNameMap;
    private: map<string, list<double> > _label2electricalMap;
    private: map<string, int> _label2modellingMap;
    private: map<string, double> _label2simplecapamimMap;
	     
    private: map<string, list<long> > _label2RdsRuleMap;
    private: map<string, list<Layer*> > _label2layerMap;


    // For reusability of software	      
    // ***************************
    private : static map<string, DtrAccess*> _registry;

 
// Constructors
// ************
    protected : DtrAccess();
    private : DtrAccess(const DtrAccess&);		
    private : DtrAccess& operator=(const DtrAccess&);

    protected : static DtrAccess * Create();		
    protected : virtual void _PostCreate();
# endif 
      
    public : static DtrAccess* Instance();

// Destructors
// ***********
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : virtual ~DtrAccess() {};
    protected : virtual void _PreDelete(); 
# endif 


    public : virtual void Delete();


// Accessors
// *********	    
    
    // If can't find data , throw Hurricane::Error
    // *******************************************	     
    public : GenericCollection<double> GetRuleByLabel(const string&) const; 
    public : GenericCollection<long>  GetRdsRuleByLabel(const string&) const; 
    public : GenericCollection<string> GetLayerNamesByLabel(const string&) const;
    public : Layers GetLayersByLabel(const string&) const;
    public : GenericCollection<double> GetElectricalsByLabel(const string& ) const;
    public : int GetModellingByLabel(const string&) const;
    public : double GetSimpleCapaMimByLabel(const string&) const;


#   if !defined(__DOXYGEN_PROCESSOR__)
    public : double GetSingleRealRuleByLabel(const string& s) const 
	     // *************************************************
	     {
	       GenericLocator<double> locator = GetRuleByLabel(s).GetLocator();
	       return locator.GetElement(); 
	     }

    public : long GetSingleRdsRuleByLabel(const string& s) const 
	     // ************************************************
	     {
	       GenericLocator<long> locator = GetRdsRuleByLabel(s).GetLocator();
	       return locator.GetElement(); 
	     }

    public : string GetSingleLayerNameByLabel(const string& s) const 
	     // ****************************************************
	     {
	       GenericLocator<string> locator = GetLayerNamesByLabel(s).GetLocator();
	       return locator.GetElement(); 
	     }

    public : Layer* GetSingleLayerByLabel(const string& s) const
	     // ************************************************
	     {
	       LayerLocator locator = GetLayersByLabel(s).GetLocator();
	       return locator.GetElement();
	     }
	   
    public : double GetSingleRealRuleByLabel(char* prefix, const string& type, char* suffix) const
	     // **********************************************************************************
	     {
	        return GetSingleRealRuleByLabel(prefix + type + suffix);       
	     }

    public : long GetSingleRdsRuleByLabel(char* prefix, const string& type, char* suffix) const 
	     // *******************************************************************************
	     {
	        return GetSingleRdsRuleByLabel(prefix + type + suffix); 
	     }

    public : string GetSingleLayerNameByLabel(char* prefix, const string& type, const string& suffix)             const
	     // ************************************************************************************
	     {
	        return GetSingleLayerNameByLabel(prefix + type + suffix);
	     }


    public : Layer* GetSingleLayerByLabel(char* prefix, const string& type, const string& suffix) const
	     // ***************************************************************************************
	     {
                return GetSingleLayerByLabel(prefix + type + suffix);	     
	     }

# endif

// Updators
// ********
    public : void AddRuleByLabel(const string& label, const list<double>& rule){ _label2ruleMap[label]=rule; };
    public : void AddLayersByLabel(const string& label, const list<string>& layers) { _label2layerNameMap[label]=layers; };
    public : void AddElectricalsByLabel(const string& label, const list<double>& electricals)
	      { _label2electricalMap[label]=electricals; };

    public : void AddModellingByLabel(const string& label, const int modelling) { _label2modellingMap[label]=modelling; };
    public : void AddSimpleCapaMimByLabel(const string& label, const double capamim) { _label2simplecapamimMap[label]=capamim; };

// Operations
// **********	     

    // For reusability of software
    // ***************************
    protected : static DtrAccess* LookUp(const string&);
    protected : static void Register(const string& , DtrAccess* );
	     

#   if !defined(__DOXYGEN_PROCESSOR__)
// Others
// ******
    public: virtual string _GetTypeName() const {return _TName("DtrAccess");};
    public: virtual string _GetString() const;
    public: virtual Record* _GetRecord() const;

    public: map<string, list<double> >& _GetLabel2RuleMap()  { return _label2ruleMap; };
    public: map<string, list<string> >& _GetLabel2LayerNameMap()  { return _label2layerNameMap; };
    public: map<string, list<double> >& _GetLabel2ElectricalMap()  
	     { return _label2electricalMap; };
    public: map<string, int>& _GetLabel2ModellingMap() { return _label2modellingMap; };
    public: map<string, double>& _GetLabel2SimpleCapaMimMap() 
	     { return _label2simplecapamimMap; };
# endif

};

}


//END_NAMESPACE_HURRICANE


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string GetString(const H::DtrAccess&);


#endif // HURRICANE_DTRACCESS
