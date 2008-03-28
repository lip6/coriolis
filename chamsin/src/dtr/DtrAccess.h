// ****************************************************************************************************
// File: DtrAccess.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_DTRACCESS
#define HURRICANE_DTRACCESS

#include "Layer.h"
#include "Layers.h"

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

    protected : static DtrAccess * create();		
    protected : virtual void _postCreate();
# endif 
      
    public : static DtrAccess* Instance();

// Destructors
// ***********
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : virtual ~DtrAccess() {};
    protected : virtual void _preDestroy(); 
# endif 


    public : virtual void destroy();


// Accessors
// *********	    
    
    // If can't find data , throw Hurricane::Error
    // *******************************************	     
    public : GenericCollection<double> getRuleByLabel(const string&) const; 
    public : GenericCollection<long>  getRdsRuleByLabel(const string&) const; 
    public : GenericCollection<string> getLayerNamesByLabel(const string&) const;
    public : Layers getLayersByLabel(const string&) const;
    public : GenericCollection<double> getElectricalsByLabel(const string& ) const;
    public : int getModellingByLabel(const string&) const;
    public : double getSimpleCapaMimByLabel(const string&) const;


#   if !defined(__DOXYGEN_PROCESSOR__)
    public : double getSingleRealRuleByLabel(const string& s) const 
	     // *************************************************
	     {
	       GenericLocator<double> locator = getRuleByLabel(s).getLocator();
	       return locator.getElement(); 
	     }

    public : long getSingleRdsRuleByLabel(const string& s) const 
	     // ************************************************
	     {
	       GenericLocator<long> locator = getRdsRuleByLabel(s).getLocator();
	       return locator.getElement(); 
	     }

    public : string getSingleLayerNameByLabel(const string& s) const 
	     // ****************************************************
	     {
	       GenericLocator<string> locator = getLayerNamesByLabel(s).getLocator();
	       return locator.getElement(); 
	     }

    public : Layer* getSingleLayerByLabel(const string& s) const
	     // ************************************************
	     {
	       LayerLocator locator = getLayersByLabel(s).getLocator();
	       return locator.getElement();
	     }
	   
    public : double getSingleRealRuleByLabel(char* prefix, const string& type, char* suffix) const
	     // **********************************************************************************
	     {
	        return getSingleRealRuleByLabel(prefix + type + suffix);       
	     }

    public : long getSingleRdsRuleByLabel(char* prefix, const string& type, char* suffix) const 
	     // *******************************************************************************
	     {
	        return getSingleRdsRuleByLabel(prefix + type + suffix); 
	     }

    public : string getSingleLayerNameByLabel(char* prefix, const string& type, const string& suffix)             const
	     // ************************************************************************************
	     {
	        return getSingleLayerNameByLabel(prefix + type + suffix);
	     }


    public : Layer* getSingleLayerByLabel(char* prefix, const string& type, const string& suffix) const
	     // ***************************************************************************************
	     {
                return getSingleLayerByLabel(prefix + type + suffix);	     
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
    public: virtual string _getTypeName() const {return _TName("DtrAccess");};
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: map<string, list<double> >& _getLabel2RuleMap()  { return _label2ruleMap; };
    public: map<string, list<string> >& _getLabel2LayerNameMap()  { return _label2layerNameMap; };
    public: map<string, list<double> >& _getLabel2ElectricalMap()  
	     { return _label2electricalMap; };
    public: map<string, int>& _getLabel2ModellingMap() { return _label2modellingMap; };
    public: map<string, double>& _getLabel2SimpleCapaMimMap() 
	     { return _label2simplecapamimMap; };
#endif

};

}


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const Hurricane::DtrAccess&);


#endif // HURRICANE_DTRACCESS
