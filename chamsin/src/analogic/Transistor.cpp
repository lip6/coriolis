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


string Transistor::MaskVersion::_getString() const
// *************************************************
{
  switch(_code) {
    case VERSION1 : return "VERSION1"; 
  }
  return "ABNORMAL";
}


Record* Transistor::MaskVersion::_getRecord() const
// **************************************************
{
  Record* record = new Record(getString(this));
  record->Add(getSlot("Code", _code));
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
  

string Transistor::Type::_getString() const
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


Record* Transistor::Type::_getRecord() const
// *****************************************
{
   Record* record = new Record(getString(this));
   record->Add(getSlot("Code", _code));
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
   _l = masqueinfo.getL();
   _w = masqueinfo.getW();
   _type= masqueinfo.getType();
   _nbDrainColumn = masqueinfo.getNbDrainColumn();
   _nbSourceColumn = masqueinfo.getNbSourceColumn();

   return *this;
}  


bool Transistor::MaskInfo::operator==(const MaskInfo& masqueinfo)
// ******************************************************************
{
   if(_l == masqueinfo.getL() &&
      _w == masqueinfo.getW() &&
      _type== masqueinfo.getType() &&
      _nbDrainColumn == masqueinfo.getNbDrainColumn() &&
      _nbSourceColumn == masqueinfo.getNbSourceColumn() 
     )
     return true;

   return false;
}  


string Transistor::MaskInfo::_getString() const
// **********************************************
{
  string s = "<" + _getTypeName() + " " 
                 + getString(_l)  + " "
		 + getString(_w)  + " "
	         + _type._getString() + " "
		 + getString(_nbSourceColumn) + " "
		 + getString(_nbDrainColumn) 
		 + ">";
  return s;
}


Record* Transistor::MaskInfo::_getRecord() const
// ***********************************************
{
  Record * record = new Record(_getString());
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

Transistor::MaskInfo& Transistor::MaskV1Info::operator=(const MaskInfo& masqueinfo)
// **************************************************************************************
{
//   (*(static_cast<Inherit*>(this)))=masqueinfo;
   Inherit::operator=(masqueinfo);
   
   return  *this;
}  

bool Transistor::MaskV1Info::operator == (const MaskInfo& masqueinfo)
// **********************************************************************
{
  //return (*(static_cast<Inherit*>(this)))==masqueinfo; 
  return Inherit::operator==(masqueinfo); 
}  


string Transistor::MaskV1Info::_getString() const
// ************************************************
{
  string s = Inherit::_getString();
  return s;
}  
  
Record* Transistor::MaskV1Info::_getRecord() const
// *************************************************
{
  Record* record = Inherit::_getRecord();
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


Transistor* Transistor::create(Library* library, const Name& name, char type)
// **************************************************************************
{
	Transistor* transistor = new Transistor(library, name, type);

	transistor->_postCreate();

	return transistor;
}



void Transistor::_preDestroy()
// ******************************
{
   // Delete aggregated objets. 
   // *************************
   if(_masqueInfo)
       delete _masqueInfo;

   if(_genTrans)
      delete _genTrans;
  
   Inherit::_preDestroy();
}


void Transistor::_postCreate()
// *******************************
{
   Inherit::_postCreate();

   (Net::create(this, Name("DRAIN")))->SetExternal(true);
   (Net::create(this, Name("SOURCE")))->SetExternal(true);
   (Net::create(this, Name("GRID")))->SetExternal(true);
   (Net::create(this, Name("BULK")))->SetExternal(true);

   // By default, transistor's length and heigth is NULL, and is internal.
   // ********************************************************************
   _masqueInfo = new MaskV1Info(0.0, 0.0);
}


string Transistor::_getString() const
// ***********************************
{
     string s = Inherit::_getString();
     s.insert(s.length()-1, " " + getString(_type));
     s.insert(s.length()-1, " " + getAbutmentType()._getString());
     return s;
}


Record* Transistor::_getRecord() const
// ************************************
{
	Record* record = Inherit::_getRecord();
	return record;
}


Transistor::MaskVersion Transistor::_getMaskInfoVersion(MaskInfo* masqueinfo) 
// ***************************************************************************************
{
    if(!masqueinfo)
       throw Error("Error : In Transistor::_getMaskInfoVersion, param masqueinfo is NULL");

    if(dynamic_cast<MaskV1Info*>(masqueinfo))
       return MaskVersion(MaskVersion::VERSION1);

    throw Error("Error : In Transistor::_getMaskInfoVersion, can't dynamic cast param masqueinfo");
    return MaskVersion(MaskVersion::VERSION1); 
}  


Transistor::MaskInfo* Transistor::_createMaskInfo(const MaskVersion& version)
// *******************************************************************************
{
    switch((const MaskVersion::Code&)version) {
      case MaskVersion::VERSION1 : 
	 return new MaskV1Info(0.0, 0.0);
	 
      default : 
	 throw Error ("Error : In Transistor::_createMaskInfoBy, unknown param version"); 
	 return NULL;
    }
}  


void Transistor::SetMaskInfo(MaskInfo* masqueinfo)
// ***************************************************
{
   if(!masqueinfo)
      throw Error("Error : In Transistor::createLayout : masqueinfo is NULL");

   // Set new Param. 
   // ***************
   MaskVersion newversion = _getMaskInfoVersion(masqueinfo);
   MaskVersion oldversion = _getMaskInfoVersion(_masqueInfo);

   if(newversion == oldversion) { // If they are the same version.
      if((*_masqueInfo)==(*masqueinfo)) // If they are identical.
	 return;
      else
	 (*_masqueInfo)=(*masqueinfo); 
   }
   else {  // If change the version.
      delete _masqueInfo; 
      _masqueInfo = _createMaskInfo(newversion);
      (*_masqueInfo) == (*masqueinfo); 
   }
}  


void Transistor::createLayout()
// ****************************
{
   MaskVersion version      = _getMaskInfoVersion(_masqueInfo);
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

   for_each_net(net, transistor->getNets())
      if( !( tmp=getNet(net->getName()) ) ) { // 
          tmp = Net::create(this, net->getName());
	  tmp->SetExternal(net->IsExternal());
      }

      for_each_component(component, net->getComponents())
          if( (con=dynamic_cast<Contact*>(component)) ){
	    Contact::create(tmp, component->getLayer(), con->getX(), con->getY(), con->getWidth(), con->getHeight());  
	  }
	  else if( (seg=dynamic_cast<Vertical*>(component)) ) {
	       Vertical::create(tmp, component->getLayer(), seg->getSourceX(), seg->getWidth(), seg->getSourceY(), 
		   seg->getTargetY()); 
	   }
	  else if( (seg=dynamic_cast<Horizontal*>(component)) ){
	       Horizontal::create(tmp, component->getLayer(), seg->getSourceY(), seg->getWidth(), seg->getSourceX(), 
	     	   seg->getTargetX()); 
	  }
          else 
            throw Error ("Error : In Transistor::DuplicateLayout, find illegal elem : " + getString(component) +
		"In Transistor, all component must be contact or segment" ) ;
	  end_for
   end_for

   SetAbutmentBox(transistor->getAbutmentBox());
	  
   _mapNet2Box.clear();
   
   map<Net*, Box>::iterator i = transistor->_getMapNet2Box()->begin(), 
       j = transistor->_getMapNet2Box()->end();

   while(i!=j) {
     _mapNet2Box[getNet((*i).first->getName())]=(*i).second;
     i++;
   }

   Materialize();   

   CloseUpdateSession();

}  

END_NAMESPACE_HURRICANE


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const H::Transistor::MaskInfo& masqueinfo)
// **********************************************************
{
  return masqueinfo._getString();
}  

