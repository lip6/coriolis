// ****************************************************************************************************
// File: CSpiceParser.h
// Authors: Wu YiFei
// Date   : 26/11/2006 
// ****************************************************************************************************



#ifndef CSPICEPARSER_H
#define CSPICEPARSER_H

#include <set>
#include <list>
#include <map>
#include <fstream>
#include <string>

#include  "hurricane/Cell.h"


namespace CRL {

class AllianceFramework;


using std::string;
using std::set;
using std::map;
using std::list;
using std::ifstream;

using Hurricane::Cell;

class CSpiceParser { 
// *****************

// Attributs
// *********
  private :  string _cellPath;
  private :  set<string>  * _portIndexSet;
  private :  list<string> * _portIndexList;
  private :  set<string> * _subModelNameSet;
  private :  map<string, string> * _indexToNetNameMap;
  private :  set<string> * _netNameSet;
  private :  map<string, list<string> > * _instanceToNetsIndexsMap;
  private :  map<string, string> * _instanceNameToMasterModelNameMap;
  private :  unsigned int _lineNumber;
  private :  unsigned int _nbOfErrors;
  private :  bool _includeBegin; 
  private :  bool _subcktBegin; 
  private :  bool _subcktEnd;
  private :  AllianceFramework* _framework;
  private :  Cell * _model;

  protected :  static bool hasSyntaxError;

// Constructors
// ************
  public : CSpiceParser(AllianceFramework* framework);
	   
// Destructors
// ***********
  public : ~CSpiceParser() ;

// Accessors:
  public : static bool HasSyntaxError() { return hasSyntaxError; }

// Operations
// **********
  public : void Parse(const string , Cell*);

// Others
// ******
  protected :  void _ScanNetDefinition();
  protected :  void _PrintError(const string&);
  protected :  void _getEntierLine(string&, ifstream&);
  protected :  void _ScanInstruction(char *);
  protected :  void _ScanInstantiation(char *);

  protected :  void _ScanInsSubckt();
  protected :  void _ScanInsInclude();
  protected :  void _ScanInsEnds();

};


class MemoryReleaser {
// *******************

// Attributes
// **********  
  protected : static unsigned int recursiveDepth;

// Constructors
// *************  
  public  : MemoryReleaser();
  private : MemoryReleaser(const MemoryReleaser&);
  private : MemoryReleaser& operator=(const MemoryReleaser&);
  private : static void* operator new(size_t) ;
  private : static void operator delete(void* );

// Destructors
// ***********
  public : ~MemoryReleaser() ;

};

}

#endif
