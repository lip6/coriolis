// ****************************************************************************************************
// File: CSpiceDriver.cpp
// Authors: Wu YiFei
// Date   : 26/11/2006 
// ****************************************************************************************************

#include  "SpiceDriver.h"
//#include "Nets.h"


namespace CRL {

using namespace Hurricane;

// *************************************************************************************************
// Class CSpiceDriver Implementations.                           
// *************************************************************************************************
CSpiceDriver::CSpiceDriver(Cell* cell)
// ************************************
:_cell(cell)
,_netToIndexMap(new map<Net*, unsigned int>)
{
}


CSpiceDriver::~CSpiceDriver()
// *************************
{
  delete _netToIndexMap; 
} 


void CSpiceDriver::_DriveLogicalViewOfDevice(ofstream& outfile) 
// ************************************************************
{
  
}  


void CSpiceDriver::_DumpSubckt(ofstream& outfile)
// ********************************
{
  unsigned int netIndex = 0 ;
  unsigned int portCount = 0;

//Net * net = NULL;

  outfile << ".subckt " << getString(_cell->getName()) << " ";

  for_each_net(net, _cell->getNets())
    (*_netToIndexMap)[net] = ++netIndex; 
    if(net->isExternal()) {
      if((++portCount)%25==0) {
        outfile << endl;
	outfile << "+ ";
      }

      outfile << netIndex << " "; 
    }
  end_for  

  outfile << endl;
}  


void CSpiceDriver::_DumpNetDefinition(ofstream& outfile)
// ****************************************************
{
   map<Net*, unsigned int>::iterator i = _netToIndexMap->begin(),
      j = _netToIndexMap->end();

   while(i!=j){
     outfile << "* NET " << (*i).second << " = " << getString(((*i).first)->getName()) << endl;
     i++;
   }
}  


void CSpiceDriver::_DumpInstanciation(ofstream& outfile)
// *****************************************************
{
  // if _cell is Device 
  // use _DriveLogicalViewOfDevice()
  // *******************************
  /* to do */

  // else if _cell is Cell 
  // **********************
//Instance * ins = NULL;
//Net * masterNet = NULL;
  Net * net = NULL;
  Cell * masterCell = NULL;
  unsigned int plugCount = 0;
  for_each_instance(ins, _cell->getInstances())
    outfile << "x" << getString(ins->getName()) << " "; 
    masterCell = ins->getMasterCell();
    for_each_net(masterNet, masterCell->getNets())
      if(masterNet->isExternal()){
        net=ins->getPlug(masterNet)->getNet();
        if(!net){//there is a plug not connected.
	  cout << "Warning : plug " + getString(masterNet->getName()) + " of instance "
	     + getString(ins->getName()) + " is not connected in model "
	     + getString(_cell->getName()) << endl;  
	}else{
	  if((++plugCount)%25==0){ // many character ,  must changer a new line
	    outfile<< endl ;
	    outfile<< "+ "; 
	  }
          outfile << (*_netToIndexMap)[net] << " " ;
	}

      } 
    end_for

    outfile<< getString(masterCell->getName()) << endl;
    plugCount = 0;   
  end_for

  // else (either Device Or Cell)
  // throw Error()
  // ****************************

}  


void CSpiceDriver::_DumpEnds(ofstream& outfile)
// ********************************************
{
  outfile << ".ends " << getString(_cell->getName()) << endl;
}  


void CSpiceDriver::Drive(const string& cellpath)
// *********************************************
{
   ofstream outfile(cellpath.c_str());
    
   _DumpSubckt(outfile);
   _DumpNetDefinition(outfile);
   _DumpInstanciation(outfile);
   _DumpEnds(outfile);

}  


} // END OF NAMESPACE CRL

