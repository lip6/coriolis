// ****************************************************************************************************
// File: Transistor.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#include "AnalogicalCommons.h"
#include "Transistor.h"
#include "GenTrans.h"

#include "Vertical.h"
#include "Horizontal.h"

#include "UpdateSession.h"

BEGIN_NAMESPACE_HURRICANE


// ****************************************************************************************************
// Transistor::MaskVersion implementation
// ****************************************************************************************************
Transistor::MaskVersion::MaskVersion(const Code& code)
// *******************************************************
:_code(code)
{
}  

Transistor::MaskVersion::MaskVersion(const MaskVersion& version)
// *******************************************************************
:_code(version._code)  
{
}


Transistor::MaskVersion& Transistor::MaskVersion::operator=(const MaskVersion& version)
// ******************************************************************************************
{
   _code = version._code;
   return *this;
}


bool Transistor::MaskVersion::operator==(const MaskVersion& version) const
// ***************************************************************************
{
   return _code==version._code; 
}  


string Transistor::MaskVersion::_GetString() const
// *************************************************
{
  switch(_code) {
    case VERSION1 : return "VERSION1"; 
  }
  return "ABNORMAL";
}


Record* Transistor::MaskVersion::_GetRecord() const
// **************************************************
{
  Record* record = new Record(GetString(this));
  record->Add(GetSlot("Code", _code));
  return record;
}


// ****************************************************************************************************
// Transistor::Type implementation
// ****************************************************************************************************
Transistor::Type::Type(const Code& code)
// *************************************
:_code(code)
{
}  


Transistor::Type::Type(const Type& type)
// *************************************
:_code(type._code)
{
}


Transistor::Type& Transistor::Type::operator=(const Type& type)
// ************************************************************
{
  _code = type._code;
  return *this;
}  
  

string Transistor::Type::_GetString() const
// *****************************************
{
  switch(_code) {
    case INTERNAL : return "INTERNAL";
    case LEFT : return "LEFT";
    case RIGHT : return "RIGHT";
    case SINGLE : return "SINGLE";
  }

  return "ABNORMAL";
}


Record* Transistor::Type::_GetRecord() const
// *****************************************
{
   Record* record = new Record(GetString(this));
   record->Add(GetSlot("Code", _code));
   return record;
}


// ****************************************************************************************************
// Transistor::MaskInfo implementation
// ****************************************************************************************************
Transistor::MaskInfo::MaskInfo(const double& l, const double& w, const Type::Code& type
	                         , const unsigned& nbDrainColumn 
	                         , const unsigned& nbSourceColumn)
// ****************************************************************************************************  
:    _l(l) 
   , _w(w)
   , _type(type)
   , _nbDrainColumn(nbDrainColumn)
   , _nbSourceColumn(nbSourceColumn)
{};


Transistor::MaskInfo& Transistor::MaskInfo::operator=(const MaskInfo& masqueinfo)
// ************************************************************************************
{
   _l = masqueinfo.GetL();
   _w = masqueinfo.GetW();
   _type= masqueinfo.GetType();
   _nbDrainColumn = masqueinfo.GetNbDrainColumn();
   _nbSourceColumn = masqueinfo.GetNbSourceColumn();

   return *this;
}  


void Transistor::MaskInfo::_PostCreate()
// ***************************************
{
}


void Transistor::MaskInfo::_PreDelete()
// **************************************
{
} 


void Transistor::MaskInfo::Delete()
// **********************************
{
   _PreDelete();
   delete this;
}  


bool Transistor::MaskInfo::operator==(const MaskInfo& masqueinfo)
// ******************************************************************
{
   if(_l == masqueinfo.GetL() &&
      _w == masqueinfo.GetW() &&
      _type== masqueinfo.GetType() &&
      _nbDrainColumn == masqueinfo.GetNbDrainColumn() &&
      _nbSourceColumn == masqueinfo.GetNbSourceColumn() 
     )
     return true;

   return false;
}  


string Transistor::MaskInfo::_GetString() const
// **********************************************
{
  string s = "<" + _GetTypeName() + " " 
                 + GetString(_l)  + " "
		 + GetString(_w)  + " "
	         + _type._GetString() + " "
		 + GetString(_nbSourceColumn) + " "
		 + GetString(_nbDrainColumn) 
		 + ">";
  return s;
}


Record* Transistor::MaskInfo::_GetRecord() const
// ***********************************************
{
  Record * record = new Record(_GetString());
  return record;
}


// ****************************************************************************************************
// Transistor::MaskV1Info implementation
// ****************************************************************************************************
Transistor::MaskV1Info::MaskV1Info(const double& l, const double& w, const Type::Code& type 
                                     , const unsigned& nbDrainColumn
		                     , const unsigned& nbSourceColumn)
// ****************************************************************************************************
:       Inherit(l
               , w
	       , type 
               , nbDrainColumn
               , nbSourceColumn
	       )
{}


Transistor::MaskV1Info* Transistor::MaskV1Info::Create(const double& l, const double& w
                                                       , const Type::Code& type 
                                                       , const unsigned& nbDrainColumn 
		                                       , const unsigned& nbSourceColumn)
// **********************************************************************************************
{
   MaskV1Info* masquev1info = new MaskV1Info(l, w, type, nbDrainColumn, nbSourceColumn);
   
   masquev1info->_PostCreate();

   return masquev1info;
}


Transistor::MaskInfo& Transistor::MaskV1Info::operator=(const MaskInfo& masqueinfo)
// **************************************************************************************
{
//   (*(static_cast<Inherit*>(this)))=masqueinfo;
   Inherit::operator=(masqueinfo);
   
   return  *this;
}  


void Transistor::MaskV1Info::_PostCreate() 
// *****************************************
{
  Inherit::_PostCreate();
}  


void Transistor::MaskV1Info::_PreDelete()
// ****************************************
{
  Inherit::_PreDelete();
}  


bool Transistor::MaskV1Info::operator == (const MaskInfo& masqueinfo)
// **********************************************************************
{
  //return (*(static_cast<Inherit*>(this)))==masqueinfo; 
  return Inherit::operator==(masqueinfo); 
}  


string Transistor::MaskV1Info::_GetString() const
// ************************************************
{
  string s = Inherit::_GetString();
  return s;
}  
  
Record* Transistor::MaskV1Info::_GetRecord() const
// *************************************************
{
  Record* record = Inherit::_GetRecord();
  return record;
}  

// ****************************************************************************************************
// Transistor implementation
// ****************************************************************************************************

Transistor::Transistor(Library* library, const Name& name, char type)
// *******************************************************************
:	Inherit(library, name),
        _type(type),
	_masqueInfo(NULL),
	_genTrans(NULL)
{
}


Transistor* Transistor::Create(Library* library, const Name& name, char type)
// **************************************************************************
{
	Transistor* transistor = new Transistor(library, name, type);

	transistor->_PostCreate();

	return transistor;
}



void Transistor::_PreDelete()
// ******************************
{
   // Delete aggregated objets. 
   // *************************
   if(_masqueInfo)
      _masqueInfo->Delete();

   if(_genTrans)
      delete _genTrans;
  
   Inherit::_PreDelete();
}


void Transistor::_PostCreate()
// *******************************
{
   Inherit::_PostCreate();

   (Net::Create(this, Name("DRAIN")))->SetExternal(true);
   (Net::Create(this, Name("SOURCE")))->SetExternal(true);
   (Net::Create(this, Name("GRID")))->SetExternal(true);
   (Net::Create(this, Name("BULK")))->SetExternal(true);

   // By default, transistor's length and heigth is NULL, and is internal.
   // ********************************************************************
   _masqueInfo = new MaskV1Info(0.0, 0.0);
}


string Transistor::_GetString() const
// ***********************************
{
     string s = Inherit::_GetString();
     s.insert(s.length()-1, " " + GetString(_type));
     s.insert(s.length()-1, " " + GetAbutmentType()._GetString());
     return s;
}


Record* Transistor::_GetRecord() const
// ************************************
{
	Record* record = Inherit::_GetRecord();
	return record;
}


Transistor::MaskVersion Transistor::_GetMaskInfoVersion(MaskInfo* masqueinfo) 
// ***************************************************************************************
{
    if(!masqueinfo)
       throw Error("Error : In Transistor::_GetMaskInfoVersion, param masqueinfo is NULL");

    if(dynamic_cast<MaskV1Info*>(masqueinfo))
       return MaskVersion(MaskVersion::VERSION1);

    throw Error("Error : In Transistor::_GetMaskInfoVersion, can't dynamic cast param masqueinfo");
    return MaskVersion(MaskVersion::VERSION1); 
}  


Transistor::MaskInfo* Transistor::_CreateMaskInfo(const MaskVersion& version)
// *******************************************************************************
{
    switch((const MaskVersion::Code&)version) {
      case MaskVersion::VERSION1 : 
	 return MaskV1Info::Create(0.0, 0.0);
	 
      default : 
	 throw Error ("Error : In Transistor::_CreateMaskInfoBy, unknown param version"); 
	 return NULL;
    }
}  


void Transistor::SetMaskInfo(MaskInfo* masqueinfo)
// ***************************************************
{
   if(!masqueinfo)
      throw Error("Error : In Transistor::CreateLayout : masqueinfo is NULL");

   // Set new Param. 
   // ***************
   MaskVersion newversion = _GetMaskInfoVersion(masqueinfo);
   MaskVersion oldversion = _GetMaskInfoVersion(_masqueInfo);

   if(newversion == oldversion) { // If they are the same version.
      if((*_masqueInfo)==(*masqueinfo)) // If they are identical.
	 return;
      else
	 (*_masqueInfo)=(*masqueinfo); 
   }
   else {  // If change the version.
      _masqueInfo->Delete(); 
      _masqueInfo = _CreateMaskInfo(newversion);
      (*_masqueInfo) == (*masqueinfo); 
   }
}  


void Transistor::CreateLayout()
// ****************************
{
   MaskVersion version      = _GetMaskInfoVersion(_masqueInfo);
   MaskV1Info* masquev1info = NULL;

   // Select algorithme with technology and masque version. 
   // *****************************************************
   switch((const MaskVersion::Code&)version) {
   
     case MaskVersion::VERSION1 : 
       masquev1info = dynamic_cast<MaskV1Info*>(_masqueInfo);
       _genTrans = new GenV1Trans(masquev1info); 
       break; 

     default : 
       break;   
   }

   SetTerminal(false);

   // Launch the selected algorithme.
   // ******************************
   _genTrans->Calculate(this);
   _genTrans->Generate(this);

   Materialize();   

   delete _genTrans;
   _genTrans = NULL;
   
}  


void Transistor::DuplicateLayout(Transistor* transistor)
// *****************************************************
{
   OpenUpdateSession();

   SetTerminal(false);

   Net * tmp = NULL;
   Contact * con = NULL;
   Segment * seg = NULL;

   for_each_net(net, transistor->GetNets())
      if( !( tmp=GetNet(net->GetName()) ) ) { // 
          tmp = Net::Create(this, net->GetName());
	  tmp->SetExternal(net->IsExternal());
      }

      for_each_component(component, net->GetComponents())
          if( (con=dynamic_cast<Contact*>(component)) ){
	    Contact::Create(tmp, component->GetLayer(), con->GetX(), con->GetY(), con->GetWidth(), con->GetHeight());  
	  }
	  else if( (seg=dynamic_cast<Vertical*>(component)) ) {
	       Vertical::Create(tmp, component->GetLayer(), seg->GetSourceX(), seg->GetWidth(), seg->GetSourceY(), 
		   seg->GetTargetY()); 
	   }
	  else if( (seg=dynamic_cast<Horizontal*>(component)) ){
	       Horizontal::Create(tmp, component->GetLayer(), seg->GetSourceY(), seg->GetWidth(), seg->GetSourceX(), 
	     	   seg->GetTargetX()); 
	  }
          else 
            throw Error ("Error : In Transistor::DuplicateLayout, find illegal elem : " + GetString(component) +
		"In Transistor, all component must be contact or segment" ) ;
	  end_for
   end_for

   SetAbutmentBox(transistor->GetAbutmentBox());
	  
   _mapNet2Box.clear();
   
   map<Net*, Box>::iterator i = transistor->_GetMapNet2Box()->begin(), 
       j = transistor->_GetMapNet2Box()->end();

   while(i!=j) {
     _mapNet2Box[GetNet((*i).first->GetName())]=(*i).second;
     i++;
   }

   Materialize();   

   CloseUpdateSession();

}  

END_NAMESPACE_HURRICANE


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string GetString(const H::Transistor::MaskInfo& masqueinfo)
// **********************************************************
{
  return masqueinfo._GetString();
}  

