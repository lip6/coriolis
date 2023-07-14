// ****************************************************************************************************
// File: CSpiceParser.h
// Authors: Wu YiFei
// Date   : 26/11/2006 
// ****************************************************************************************************




#include  "SpiceParser.h"

#include <iomanip>
#include <iostream>
#include <vector>

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"

#include  "SpiceCommons.h"

//#include "CCATAL.h"

namespace CRL {

//using namespace std;
using namespace Hurricane;

static unsigned short sizeOfBuffer = 150;

// These two contenors must be vided when Parser finish.
// ****************************************************
static map<string, list<Net*> > modelNameToPortListMap;   
static set<string> badModelNameSet; 


// *************************************************************************************************
// Class CSpiceParser Implementations.                           
// *************************************************************************************************
bool CSpiceParser::hasSyntaxError = false;

CSpiceParser::CSpiceParser(AllianceFramework* framework)
// *****************************************************
:_cellPath("")
,_portIndexSet(new set<string>)
,_portIndexList(new list<string>)
,_subModelNameSet(new set<string>)
,_indexToNetNameMap(new map<string, string>)
,_netNameSet(new set<string>)
,_instanceToNetsIndexsMap(new map<string, list<string> >)
,_instanceNameToMasterModelNameMap(new map<string, string>)
,_lineNumber(0)
,_nbOfErrors(0)
,_includeBegin(false)
,_subcktBegin(false)
,_subcktEnd(false)
,_framework(framework)
,_model(NULL)
{
}


CSpiceParser::~CSpiceParser()
// **************************
{
  delete _portIndexSet;
  delete _portIndexList;
  delete _subModelNameSet;
  delete _indexToNetNameMap;
  delete _netNameSet;
  delete _instanceToNetsIndexsMap;
  delete _instanceNameToMasterModelNameMap;
}


void CSpiceParser::_PrintError(const string& reason)
// *************************************************
{
  _nbOfErrors++;
  cout <<"In " << _cellPath << " line "<< setw(4) << left<<   _lineNumber << " : " << reason << endl;
}  


void CSpiceParser::_ScanNetDefinition()
// ************************************
{
IF_DEBUG_CSPICEPARSER
  cout << "  Scan net defintion" << endl;
END_IF

  if( (!_subcktBegin) || _subcktEnd ){ //  check spice instruction order
    _PrintError("Net definition must be in the instruction SUBCKT") ;
    return ;
  }

  char * token = NULL;
  vector<string> buf;
 
  while((token=strtok(NULL, " "))) {
     buf.push_back(token);
IF_DEBUG_CSPICEPARSER
     cout << token << endl;
END_IF
  }

  if((buf.size()!=3) ||
     (buf[1]!="=") ) {
     _PrintError("Net Definition must be <index = netname>");
     return;
  }   

  if(_indexToNetNameMap->find(buf[0])!=_indexToNetNameMap->end()) { // redefiniton of net
    _PrintError("Redefinition of net " + buf[0]);
  }else{
    (*_indexToNetNameMap)[buf[0]] = buf[2];
  }
  
  if(_netNameSet->find(buf[2])!=_netNameSet->end()){ // Repetition of net name
    _PrintError("Repetition of net name " + buf[2]);
  }else{
    _netNameSet->insert(buf[2]); 
  }
}  


void CSpiceParser::_ScanInsInclude()
// *********************************
{
IF_DEBUG_CSPICEPARSER
  cout << "  Scan Instruction Include" << endl;
END_IF
  _includeBegin = true;

  if( _subcktBegin ||  _subcktEnd ) { //  check spice instruction order  
     _PrintError("INCLUDE instruction must be before the subckt instruction");
     return;
  }
  
  char * token;
  string arguments;
  vector<string> buf;
  string masterModelName;
  Cell* masterCell = NULL;
 
  while((token=strtok(NULL, " "))) {
     buf.push_back(token);
     arguments += token;
     arguments += " ";
  }

  if(buf.size()!=1) { 
    _PrintError("instruction include must include only one argument not sevel : " + arguments);
    return;
  }

  // check the name of included spice file.
  // The name of file must finish by .spi.
  // **************************************
  masterModelName = strtok(const_cast<char*>(buf[0].c_str()), ".");
  _subModelNameSet->insert(masterModelName);
  token = strtok(NULL, ".");
  if(strcmp(token, "spi")!=0){
    _PrintError("Invalid included file name like : " + buf[0] 
	+ " must finish by .spi");
    return;
  }

  // use CRL::getCell() pour charger the spice file.
  // if cell=CRL::getCell()==NULL printError and Return
  // **************************************************
  masterCell = _framework->getCell(masterModelName, Catalog::State::Logical);
  if(!masterCell) {
    _PrintError("Can't find model " + masterModelName + " in database");
    return;
  }

IF_DEBUG_CSPICEPARSER
  cout << "Read Spice File " << buf[0] << endl;
END_IF
}


void CSpiceParser::_ScanInsSubckt()
// ********************************
{
IF_DEBUG_CSPICEPARSER
  cout << "  Scan Instruction SUBCKT" << endl;
END_IF
  _subcktBegin = true;
   
  if( _subcktEnd ) { //  check spice instruction order  
     _PrintError("SUBCKT instruction must be before the ENDS instruction");
     return;
  }

  // syntax of instruction subckt : model_name(buf[0]) IndexOfPort(buf[1]) ...
  // **************************************************************************
  char * token = strtok(NULL, " ");
  bool redeclarationPortIndex = false; 
 
  // check mode_name is valid. 
  // *************************
  if(getString(_model->getName())!=token){
    _PrintError("the model name in the instruction SUBCKT is not same as the name of Model"); 
    return;
  }

  while((token=strtok(NULL, " "))) {

     if(_portIndexSet->find(token)!=_portIndexSet->end()) {
       _PrintError("redeclaration of port index " + string(token));
       redeclarationPortIndex = true;     
     }else {
      _portIndexList->push_back(token);
      _portIndexSet->insert(token);
     }
  }

  if(redeclarationPortIndex) {
     _portIndexSet->clear();
  }
}


void CSpiceParser::_ScanInsEnds()
// ******************************
{
IF_DEBUG_CSPICEPARSER
  cout << "  Scan Instruction Ends" << endl;
END_IF
  _subcktEnd = true;

  if( !_subcktBegin ) { //  check spice instruction order  
     _PrintError("ENDS instruction must follow the SUBCKT instruction");
     return;
  }
   
  // syntax of instruction ENDS : model_name
  // ***************************************
  char * token = strtok(NULL, " ");
 
  // check mode_name is valid. 
  // *************************
  if(getString(_model->getName())!=token){
    _PrintError("the model name in the instruction ENDS isn't same as the name of Model"); 
    return;
  }
}  


void CSpiceParser::_ScanInstruction(char* instruction)
// ***************************************************  
{
  char * token = strtok(instruction, " ");
  
  if( (strcmp(token, ".subckt")==0) || 
      (strcmp(token, ".SUBCKT")==0) ) { // instruction SUBCKT 
     _ScanInsSubckt();
  }else if((strcmp(token, ".include")==0) || 
           (strcmp(token, ".INCLUDE")==0) ) {  // instruction INCLUDE
     _ScanInsInclude();
  }else if((strcmp(token, ".ends")==0) || 
           (strcmp(token, ".ENDS")==0) ) {  // instruction ENDS
     _ScanInsEnds();
  }else {  // unknown instruction
     _PrintError("Unknown instruction : " + string(token));
  }
}


void CSpiceParser::_ScanInstantiation(char* instantiation)
// ******************************************************* 
{
IF_DEBUG_CSPICEPARSER
  cout << "Scan Instantiation " << instantiation << endl;
END_IF
  if((!_subcktBegin) || _subcktEnd ) {//  check spice instruction order 
     _PrintError("ENDS instruction must follow the SUBCKT instruction");
     return;
  }
  
  // (1) Check instance Name and model Name: repetition , invalid string.
  // (2) Check if Model via CRL::getCell() is valid. 
  // (3) Check if instance is well branched. nb of plugs equal to nb of signals
  // **************************************************************************

  // Syntax of instantiation is : xnameOfInstance indexOfNet ...... nameOfModel
  // **************************************************************************

  vector<string> buf;
  string instanceName;
  string modelName;
  char * token = strtok(instantiation, " ");
  instanceName = (token+1);
  Cell * masterCell = NULL;

  // (1) check instance name and Model Name.
  // ***************************************
  if(instanceName=="") {
    _PrintError("instance Name must not be vide");
    return;
  }

  if(_instanceToNetsIndexsMap->find(instanceName)!=_instanceToNetsIndexsMap->end()) {
    _PrintError("repetition instance Name : " + instanceName);
    return;
  }  
 
  while((token=strtok(NULL, " "))) {
     buf.push_back(token);
  }
  
  vector<string>::iterator i = buf.begin() , j = buf.end();
  if(_subModelNameSet->find(modelName = *(--j))==_subModelNameSet->end()) {
    _PrintError("Invalid model name " + modelName);
    return;
  } 


  // (2) check if model is valid via CRL::getCell()
  // **********************************************
  // if cell doesn't exists In Memory PrintError Return. 
  // if cell exist but hasn't ports, PrintError Return.
  masterCell = _framework->getCell(modelName, Catalog::State::InMemory);

  if (!masterCell) {
     _PrintError("Can't find model " + modelName + " in database");
     return;
  }else {
     if(masterCell->_getNetMap()._getSize()==0){//If the logical view of model is empty
       _PrintError("The logical view of model " + modelName + " is empty");
       return;
     }    
  }
  
  // (3) Check if instance is well branched. nb of plugs equal to nb of signals
  // **************************************************************************
  // if(masterCell->getPlugs().getSize() != (buf.size() - 1) ) PrintError Return.
  if(masterCell->getExternalNets().getSize()!=(buf.size()-1)) {
    _PrintError("The number of connector of instance " + instanceName +
	"is not equal to the number of ports of model " + modelName);
    return;
  }

  while(i!=j){
     (*_instanceToNetsIndexsMap)[instanceName].push_back(*i);  
     i++;
  }

  (*_instanceNameToMasterModelNameMap)[instanceName]=modelName;
}


void CSpiceParser::_getEntierLine(string& bigbuf, ifstream& infile)
// ****************************************************************
{
   int pos = 0;
   char buf[sizeOfBuffer];

   pos = infile.tellg();
   while(!infile.getline(buf, sizeOfBuffer).eof()) {
      _lineNumber++;

      if(buf[0]!='+')
	break;
  
      bigbuf += ' ';
      bigbuf += (buf+1);
      pos = infile.tellg();
   }

   infile.seekg(pos);
   _lineNumber--;
}  



// The main algorithm of Function Parse is : 
// 
// firstly to parcours all lines. 
// end for every line detect as many syntax errors as possible(but
// not all errors)
// and save good information of a line if there isn't any error
// in this line.
//
// secondly if there isn't any error in previous phase, detect globaly
// syntax errors like :
// (1) there is a port isn't named.  
// (2) there is a signal doesn't exist but is branched to a instance.
// 
// If there isn't any error any more, create really the logical view 
// of model. 
// ******************************************************************
void CSpiceParser::Parse(const string cellPath, Cell* cell)
// *********************************************************
{
  CatalogProperty *sprop =
    (CatalogProperty*)cell->getProperty ( CatalogProperty::getPropertyName() );
  if ( sprop == NULL )
    throw Error ( "Missing CatalogProperty in cell %s.\n" , getString(cell->getName()).c_str() );
  
  Catalog::State *state = sprop->getState ();
  state->setLogical ( true );

//  if ( state->isTerminalNetlist() ) cell->setTerminalNetlist ( true );

  if(badModelNameSet.find(getString(cell->getName()))!=badModelNameSet.end()) {
    state->setLogical (false);
    return; 
  }

  cout<<"##Begin parse cell " << getString(cell) << endl;

  ifstream infile(cellPath.c_str());

  _model = cell;
  _cellPath = cellPath;

  char buf[sizeOfBuffer];
  
  while(!infile.getline(buf, sizeOfBuffer).eof()) {
    _lineNumber++;
    
    if(buf==string("")){
IF_DEBUG_CSPICEPARSER
      cout << "vide line" << endl;
END_IF
      continue;
    }else {

      char * firstToken;
      string bigbuf;

      switch(buf[0]){
	case '*' :
	  firstToken = strtok(buf+1, " ");
	  if(strcmp(firstToken, "NET") == 0 || 
	     strcmp(firstToken, "net") == 0) {
	     _ScanNetDefinition();
	  }
	  else {
	    continue;  
	  }
	  break;

	case '.' : 
	  bigbuf = buf;
          _getEntierLine(bigbuf, infile);
	  _ScanInstruction(const_cast<char*>(bigbuf.c_str()));
	  break;

	case 'x' :
	  bigbuf = buf;
          _getEntierLine(bigbuf, infile);
	  _ScanInstantiation(const_cast<char*>(bigbuf.c_str()));
	  break;

	default :
	  _PrintError("Unknown syntax like" + string(buf));
	  break;
      } // end of switch
    
    } // end of if
    
  } // end of while, end of reading spice file.


  if(!_nbOfErrors){ // the syntax of every line of spice file is correct. 

IF_DEBUG_CSPICEPARSER
    cout << "check connectique" << endl;
END_IF
    // check connectique
    // (1) if there is a port isn't named.  
    // (2) if there is a signal doesn't exist but is branched to a instance.
    // **********************************************************************

    // (1) if there is a port isn't named.  
    // ***********************************
    set<string>::iterator it1_begin = _portIndexSet->begin() , it1_end = _portIndexSet->end();
    while(it1_begin!=it1_end){
      if(_indexToNetNameMap->find(*it1_begin)==_indexToNetNameMap->end()){
        _PrintError("index Port " + *it1_begin + " is not named either defined");
      }
      it1_begin++;   
    }

    // (2) if there is a signal doesn't exist but is branched to a instance.
    // **********************************************************************
    map<string, list<string> >::iterator it2_begin = _instanceToNetsIndexsMap->begin(),
       it2_end = _instanceToNetsIndexsMap->end();

    while(it2_begin!=it2_end){
      list<string>::iterator it3_begin = (*it2_begin).second.begin(), 
	it3_end = (*it2_begin).second.end();

      while(it3_begin!=it3_end){
	if(_indexToNetNameMap->find(*it3_begin) == _indexToNetNameMap->end()){
          _PrintError("instance " + (*it2_begin).first + " is branched to unknown signal "
	      + *it3_begin);
	}
	it3_begin++;
      }
      
      it2_begin++;
    }

IF_DEBUG_CSPICEPARSER
    cout << "End check connectique" << endl;
END_IF

    if(!_nbOfErrors) {

      // create the logical view of this model.
      // **************************************
//static map<string, list<Net*> > _modelNameToPortListMap;
      string modelName = getString(_model->getName());
      
      // create Ports(External Net) and signals(Internal Net)
      // ****************************************************
      list<string>::iterator it0_begin = _portIndexList->begin(),
	it0_end = _portIndexList->end();

      while(it0_begin!=it0_end){ // create Port
	Net * port = Net::create(_model, (*_indexToNetNameMap)[*it0_begin]);  
	port->setExternal(true);
	modelNameToPortListMap[modelName].push_back(port);
        it0_begin++;
      }

      map<string, string>::iterator it4_begin = _indexToNetNameMap->begin(),
	 it4_end = _indexToNetNameMap->end();

      while(it4_begin!=it4_end){
         if(_portIndexSet->find((*it4_begin).first)==_portIndexSet->end()){ // internal net
	   Net::create(_model, (*it4_begin).second)->setExternal(false);  // create Signal
	 }
         it4_begin++;
      }

      // create Instances and effect connection.
      // ***************************************
      map<string, list<string> >::iterator it5_begin = _instanceToNetsIndexsMap->begin(),
        it5_end = _instanceToNetsIndexsMap->end();

      while(it5_begin!=it5_end){  // For all instances.
        Cell * masterModel = 
	  _framework->getCell((*_instanceNameToMasterModelNameMap)[(*it5_begin).first], Catalog::State::InMemory);

	Instance * ins = Instance::create(_model, (*it5_begin).first, masterModel);

	list<string>::iterator it6_begin = (*it5_begin).second.begin(),
	  it6_end = (*it5_begin).second.end();  // signal index list iterator

      list<Net*>::iterator it7_begin =      // masterModel port list iterator 
	    modelNameToPortListMap[getString(masterModel->getName())].begin();
#ifndef NDEBUG
	  list<Net*>::iterator it7_end = 
	    modelNameToPortListMap[getString(masterModel->getName())].end();
#endif
	
	while(it6_begin!=it6_end){  // For all port<=> signals to branch. 
         ins->getPlug(*it7_begin)->setNet(_model->getNet((*_indexToNetNameMap)[*it6_begin])); 
	 it7_begin++;
	 it6_begin++;
	}  // end for all port<=> signals to branch.

	assert(it7_begin==it7_end);

        it5_begin++;
      }  // end for all instances.

      cout<<"##End parse cell " << getString(cell) << endl;
      return;
    }

  } // end of if the syntax of every line of spice file is correct. 

  cout<<"##End parse model " << getString(_model) << endl;
  
  badModelNameSet.insert(getString(_model->getName()));

  state->setLogical(false);

  hasSyntaxError = true;
  cout<< _nbOfErrors << " errors are detected in spice file " << _cellPath << endl;
  

//  _portIndexSet->clear();
//  _indexToNetNameMap->clear();
//  _netNameSet->clear();
//  _instanceToNetsIndexsMap->clear();

}  

// *************************************************************************************************
// Class CSpiceParser Implementations.                           
// *************************************************************************************************
unsigned int MemoryReleaser::recursiveDepth = 0;


MemoryReleaser::MemoryReleaser()
// ****************************
{
  recursiveDepth++;

}  


MemoryReleaser::~MemoryReleaser()
// ******************************
{
  if(--recursiveDepth==0){
    // Releaser Memory.
    // ***************
    modelNameToPortListMap.clear();   
    badModelNameSet.clear(); 
  }
}  


}
