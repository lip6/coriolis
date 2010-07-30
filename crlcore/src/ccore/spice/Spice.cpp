// ****************************************************************************************************
// File: Spice.cpp
// Authors: Wu YiFei
// Date   : 25/11/2007 
// ****************************************************************************************************


// ****************************************************************************************************
// Include Files.                     
// ****************************************************************************************************

# include  "Spice.h"
# include  "SpiceParser.h"
# include  "SpiceDriver.h"
# include  "crlcore/AllianceFramework.h"

namespace CRL {

using namespace Hurricane;


// ************************************************************************************************
// Utilitarians (Functions).                          
// ************************************************************************************************

// -------------------------------------------------------------------
// Function  :  "spiceParser(const string& cellPath, Cell* )".

  /*! \overload void spiceParser ( const string& cellPath, Cell* cell )
   *  \param   cellPath    the path of cell.
   *  \param   cell        model to parse. 
   *
   *  Parse hierarchy spice file. 
   */

void  spiceParser ( const string cellPath, Cell* cell )
// ****************************************************
{
   //  recursiveDepth++;
   //  Effect un objet for release memory .
   // **********************************
   MemoryReleaser releaser;

   CSpiceParser parser(AllianceFramework::get());    
   parser.Parse(cellPath, cell);


   // if(--recursiveDepth==0) release modelNameToPortListMap and badModelNameSet.
   // ***************************************************************************
     
}


void  spiceDriver ( const string cellPath, Cell* cell, unsigned int& saveState)
// ****************************************************************************
{
  CSpiceDriver driver(cell);
 
  driver.Drive(cellPath);
}




} // END OF NAMESPACE CRL


// ************************************************************************************************
// Generic functions.
// ************************************************************************************************


