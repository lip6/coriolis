// ****************************************************************************************************
// File: Device.cpp
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************


#include "Device.h"

#include "Transformation.h"
#include "Point.h"
#include "Instance.h"
#include "Box.h"
#include "Error.h"

#include "Cells.h"
#include "DtrAccess.h"
#include "GenericDtrAccess.h"
using namespace Hurricane;

// ****************************************************************************************************
// Static data function 
// ****************************************************************************************************

static Instance * refins = NULL;


static set<Cell*> cellSet;


static void GetAllCells(Cell* cell)
// ********************************
{
   cellSet.insert(cell);
   
   if(!(cell->IsLeaf())){
       for_each_instance(instance, cell->GetInstances())
	 Cell * mastercell = instance->GetMasterCell();
         GetAllCells(mastercell); 
       end_for	 
   }  
}



namespace DEVICE {


// ****************************************************************************************************
// Device implementation
// ****************************************************************************************************

Device::Device(Library* library, const Name& name)
// **************************************************************************
:	Inherit(library, name)
{
}


void Device::Delete()
// **************************
{
   _PreDelete();
   delete this;
}


void Device::_PreDelete()
// ******************************
{
   // do something
   // ************
  
   Inherit::_PreDelete();
}


void Device::_PostCreate()
// *******************************
{
   Inherit::_PostCreate();

   //CDataBase* database = GetCDataBase();
   //CCatal* ccatal = database->GetCCatal();
  
   //CCatal::State* state = ccatal->GetState(GetName(), true);
   //state->SetFlags(CCatal::State::LOGICAL|CCatal::State::PHYSICAL|CCatal::State::IN_MEMORY|CCatal::State::GDS, true);
   //state->SetCell(this);
   //state->SetLibrary(GetLibrary());

   // Create GenericDtrAccess and DtrAccess
   // *************************************
   GenericDtrAccess::Instance(DtrAccess::Instance());   

}


void Device::SaveLogicalView()
// ***************************
{
   cellSet.clear();
   GetAllCells(this);

   //CDataBase * db = GetCDataBase();

  // set<Cell*>::iterator i = cellSet.begin(), j = cellSet.end();
  // 
  // while(i!=j) {
  //   db->SaveCell(*i, CCatal::State::LOGICAL ); 
  //   i++;
  // }
}  



string Device::_GetString() const
// ***************************************
{
     string s= Inherit::_GetString();
     return s;
}

Record* Device::_GetRecord() const
// *********************************
{
	Record* record = Inherit::_GetRecord();
	return record;
}


void Device::_Place(Instance* ins, const Transformation::Orientation& orientation, const Point& point)
// **************************************************************************************************  
{
    if(!ins) {
       throw Error("Can't Place Instance : ins is NULL");
    } 

    if(ins->IsPlaced()) {
       throw Error("Can't Place " + GetString(ins) + " : it has already been placed");
    }

    Transformation transformation(Point(0,0), orientation);
    Box orientedmastercellbox = transformation.getBox(ins->GetMasterCell()->GetAbutmentBox());
    
    Point translation( point.getX() - orientedmastercellbox.getXMin()
	             , point.getY() - orientedmastercellbox.getYMin() );

    Transformation transformation_ins = Transformation(translation, orientation);
    
    ins->SetTransformation(transformation_ins);
    ins->SetPlacementStatus(Instance::PlacementStatus::PLACED);
}


void Device::_SetRefIns(Instance* ins) const
// *****************************************
{
   if(!ins) {
       throw Error("Can't SetRefIns : ref instance is NULL");
   }
  
   if(ins->IsUnplaced()) {
       throw Error("Can't SetRefIns : ref instance has't been placed");
   }
   
   refins = ins;
}  


void Device::_PlaceRight(Instance* ins, const Transformation::Orientation& orientation, const Point& offset)
// ********************************************************************************************************  
{
    if(!ins) {
       throw Error("Can't PlaceRight Instance : ins is NULL");
    } 

    if(ins->IsPlaced()) {
       throw Error("Can't PlaceRight " + GetString(ins) + " : it has already been placed");
    }


    if(!refins) {
      throw Error("Can't Place Right " + GetString(ins) + " :  can't find refins");
    }
  
    Box refinsbox = refins->GetAbutmentBox();
    
    Transformation transformation(Point(0,0), orientation);
    Box orientedmastercellbox = transformation.getBox(ins->GetMasterCell()->GetAbutmentBox());
    
    Point translation( refinsbox.getXMax() - orientedmastercellbox.getXMin() + offset.getX()
	             , refinsbox.getYMin() - orientedmastercellbox.getYMin() + offset.getY() );

    Transformation transformation_ins = Transformation(translation, orientation);
    
    ins->SetTransformation(transformation_ins);
    ins->SetPlacementStatus(Instance::PlacementStatus::PLACED);

    refins = ins;
}


} // end namespace Device
