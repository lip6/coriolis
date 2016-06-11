

// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
//  Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//
// The Coriolis Project  is free software;  you can  redistribute it and/or
// modify  it  under the  terms  of  the  GNU  General  Public License  as
// published by  the Free  Software Foundation; either  version 2  of  the
// License, or (at your option) any later version.
//
// The Coriolis Project  is distributed in the hope that it will be useful,
// but  WITHOUT  ANY  WARRANTY;  without  even  the  implied  warranty  of
// MERCHANTABILITY  or  FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
// General Public License for more details.
//
// You should have received a copy  of  the  GNU  General  Public  License
// along with  the Coriolis Project;  if  not,  write to the  Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
//
// License-Tag
//
//
// Date   : 25/10/2005
// Author : Damien Dupuis                        <Damien.Dupuis@lip6.fr>
//
// Authors-Tag

// ************************************************************************************
// File: CParsBK.cpp
// ************************************************************************************


#include <string.h>
#include <sstream>
#include <regex.h>
#include "hurricane/DataBase.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "crlcore/Utilities.h"
#include "crlcore/AllianceFramework.h"
#include "Bookshelf.h"


const char* badRegex =
  "CParsBK::CParsBK () :\n\n"
  "  Invalid regular expression for %s :\n"
  "    %s.\n";

namespace {


   using namespace Hurricane;
   using namespace CRL;




// char* lower ( char* str )
// {
//   for ( char* ptr = str; *ptr != '\0' ; ptr++ )
//     *ptr = (char)tolower ( *ptr );
//
//   return str;
// }

// CParsAP should read this code :
// *******************************
//  inline char * __passDelim ( char* s, const char* delims ) {
//      if ( s == NULL )
//          return NULL;
//      
//      char* newS = NULL;
//      while ( *s != (char)0 ) {
//          for ( unsigned i = 0 ; i < strlen ( delims )  ; i++ ) {
//              if ( *s == delims[i] ) {
//                  if ( delims[i] == ' ' ) cerr << "Vive moi j'ai lu un espace !!!" << endl;
//                  if ( delims[i] == '\t' ) cerr << "Vive moi j'ai lu une tabulation !!!" << endl;
//                  if ( delims[i] == '\n' ) cerr << "Vive moi j'ai lu un saut de ligne !!!" << endl;
//                  *s = (char)0;
//                  break;
//              }
//          }
//          if ( *s != (char)0 ) { // si on a trouvé un vrai token différent de fin de chaine et delims
//              newS = s;
//              break;
//          }
//          s++;
//      }
//      return newS; 
//  }
//
//  inline char * __chrDelim ( char* s, const char* delims ) {
//      if ( s == NULL )
//          return NULL;
//  }
//  
//  char* __strtok ( char* s, const char* delims )
//  {
//    static       char* _start  = NULL;
//    static const char* _delims = NULL;
//
//    if ( delims ) _delims = delims;
//    if ( s )      _start  = __passDelim ( s, _delims );
//
//    char* token = _start;
//
//    if ( token && _delims ) {
//      char* nextToken = NULL;
//      for ( unsigned i = 0 ; i < strlen ( _delims ) ; i++ ) {   // pass les delimiteurs
//        if ( ( nextToken = strchr ( token, _delims[i] ) ) != NULL ) {
//          _start = __passDelim ( nextToken, _delims );
//          return token;
//        }
//      }
//    }
//
//    return NULL;
//  }



// **************************************************************************************
// The Class BKParser
// **************************************************************************************

class BKParser {
// *************

// BKParser Types
// **************
    enum BKState { AUX, NODES, NETS, WTS, PL, SCL, END };

// BKParser Attributes
// *******************


    public: AllianceFramework* _framework;
    public: Technology*        _technology;
    public: Layer::Mask        _mask;
    public: FILE *             _file;
    public: unsigned           _bufferSize;
    public: char               _buffer[256];
    public: unsigned           _lineNumber;
    public: bool               _done;
    public: Cell *             _cell;
    public: long               _scale;          // Scale factor for coordinates
    public: int                _version;        // BK Format Version
    public: int                _warningNumber;
    public: int                _errorNumber;
    public: unsigned           _anonymousNetId;
    public: string             _fileStringTab [6];
    public: regex_t            _regex [6];
    public: unsigned           _nbNets;         // For genertating automatic NetNames

// Constructors & Destructor
// *************************

    public:  BKParser ( AllianceFramework* framework );
    public: ~BKParser();

// Others
// ******

    public: void OnAbort     ( const string& message, unsigned state );
    public: void OnWarning   ( const string& message, unsigned state );
    public: void OnError     ( const string& message, unsigned state );

    public: bool ScanAux     ();                                                                              // to read different FileNames
    public: bool ScanNodes   ( Name& name, DbU::Unit& width, DbU::Unit& height, bool& isPad );                          // to read Instances
    public: bool ScanNets    ( Name& insName, Net::Direction& dir, DbU::Unit& dx, DbU::Unit& dy );                      // to read Nets
    public: bool ScanWts     ( Name& name, unsigned& weight );                                                // to read instances Weights
    public: bool ScanPl      ( Name& name, DbU::Unit& x, DbU::Unit& y, Transformation::Orientation& orient, bool& isFixed );// to read Placement
    //public: bool ScanScl     ( DbU::Unit& coordinate, ....to be completed  );                                    // to read Rows
    public: bool ScanNum     ( unsigned& num );
    public: bool ScanDegree  ( unsigned& degree, Name& netName );

    public: bool isNumber    ( char* token );
    public: bool isFloat     ( char* token );
    public: bool isName      ( char* token );
    public: bool isSymetry   ( char* token );
    public: bool isDirection ( char* token );

    public: void LoadFromFile(const string& cellPath, Cell* cell);

    private: Name  getNewNetName ();
};




// ************************************************************************************
// Implementation of Class BKParser
// ************************************************************************************

BKParser::BKParser ( AllianceFramework* framework )
// ************************************************
  : _framework ( framework ),
    _technology ( NULL ),
    _mask ( ~0 ),
    _file ( NULL ),
    _bufferSize ( 256 ),
    _buffer(),
    _lineNumber ( 0 ),
    _done ( false ),
    _cell ( NULL ),
    _scale ( 1 ),
    _version ( 0 ),
    _warningNumber ( 0 ),
    _errorNumber ( 0 ),
    _anonymousNetId ( 0 ),
    _fileStringTab (),
    _regex (),
    _nbNets ( 0 )
{
    _buffer[0]='\0';
    _technology = DataBase::getDB()->getTechnology ();
}


BKParser::~BKParser()
// ******************
{
}


void BKParser::OnAbort ( const string& message, unsigned state )
// *************************************************************
{
    string reason = message;
    if ( ! _fileStringTab[state].empty() ) {
        reason += " in File: " + _fileStringTab[state];
        if ( _lineNumber )
            reason += " (line " + getString ( _lineNumber ) + ")";
    }
    throw Error(reason);
}


void BKParser::OnError ( const string& message, unsigned state )
// *************************************************************
{
    _errorNumber++;
    string reason = "[ERROR] ";
    if ( ! _fileStringTab[state].empty() ) {
        reason += _fileStringTab[state];
        if ( _lineNumber )
            reason += "," + getString ( _lineNumber ) + " : ";
    }
    reason += message;
    cout << reason << endl;
}


void BKParser::OnWarning ( const string& message, unsigned state )
// ***************************************************************
{
    _warningNumber++;
    string reason = message;
    if ( ! _fileStringTab[state].empty() ) {
        reason += " in File: " + _fileStringTab[state];
        if ( _lineNumber )
            reason += " (line " + getString ( _lineNumber ) + ")";
    }
    cout << tab << "[WARNING] " << reason << endl;
}


Name BKParser::getNewNetName()
// ***************************
{
  ostringstream  name ( "anonymous_net_" );
  name << _anonymousNetId++;

  return name.str();
}


// Net* MergeNets ( Net* net1, Net* net2 )
// // ************************************
// {
//     if ( net2->getName()[0] != '~' ) {
//         if ( ( net1->getName()[0] == '~' ) ||
//              ( net2->isGlobal()   && !net1->isGlobal()   ) ||
//              ( net2->isExternal() && !net1->isExternal() ) ) {
//             Net* tmpNet = net1;
//             net1 = net2;
//             net2 = tmpNet;
//         }
//     }
//     net1->setExternal ( net1->isExternal() || net2->isExternal() );
//     net1->setGlobal   ( net1->isGlobal()   || net2->isGlobal()   );
//    
//     net1->merge ( net2 ); // Caution : net2 will be deleted
//     return net1;
// }


// ************************************************************************************
// BK Record Scanning methods
// ************************************************************************************


bool BKParser::isNumber ( char* token ) {
    cdebug_log(100,0) << "isNumber = " << token;

    unsigned i = 0;
    char tok;
    while ( ( tok = token[i++] ) != char(0) ) {
        if ( ( tok < '0' ) || ( tok > '9' ) )
            return false;
    }
    return true;
}

bool BKParser::isFloat ( char* token ) {
  cdebug_log(100,0) << "isFloat = " << token;

    unsigned i = 0;
    char tok;
    while ( ( tok = token[i++] ) != char(0) ) {
        if ( ( tok != '-' ) && ( tok != '+' ) && ( tok != '.' )
        && ( ( tok < '0' ) || ( tok > '9' ) ) )
            return false;
    }
    return true;
}

bool BKParser::isName ( char* token ) {
    cdebug_log(100,0) << "isName = " << token;

    unsigned i = 0;
    char tok;
    while ( ( tok = token[i++] ) != char(0) ) {
        if ( ( tok != '_' ) && ( tok != '-' ) && ( tok != '+' ) && ( tok != '.' )
        && ( ( tok < '0' ) || ( tok > '9' ) )
        && ( ( tok < 'a' ) || ( tok > 'z' ) )
        && ( ( tok < 'A' ) || ( tok > 'Z' ) ) )
            return false;
    }
    return true;
}

bool BKParser::isSymetry ( char* token ) {
    cdebug_log(100,0) << "isSymetry = " << token;
    if ( ( ( token[0] == 'X' ) && ( token[1] == char(0) ) )
      || ( ( token[0] == 'Y' ) && ( token[1] == char(0) ) )
      || ( ( token[0] == 'R' ) && ( token[1] == '9' ) && ( token[2] == '0' ) && ( token[3] == char(0) ) ) )
        return true;
    return false;
}
bool BKParser::isDirection ( char* token ) {
    cdebug_log(100,0) << "isDirection = " << token;

    if ( ( ( token[0] == 'I' ) || ( token[0] == 'O' ) || ( token[0] == 'B' ) ) && ( token[1] == char(0) ) )
        return true;
    return false;
}


bool BKParser::ScanAux ()
// **********************
{
    // **********************************************************************************************************
    // The Aux record looks like :
    // RowBasedPlacement :  <cell_name>.nodes  <cell_name>.nets  <cell_name>.wts  <cell_name>.pl  <cell_name>.scl
    // **********************************************************************************************************
    cdebug_log(100,0) << "ScanAux = " << _buffer;

    // ***********************
    // Patterns initialization
    // ***********************
    const char* regexPattern [6];
    regexPattern[NODES] = "([-_\\.[:alnum:]]+\\.nodes)";
    regexPattern[NETS]  = "([-_\\.[:alnum:]]+\\.nets)";
    regexPattern[WTS]   = "([-_\\.[:alnum:]]+\\.wts)";
    regexPattern[PL]    = "([-_\\.[:alnum:]]+\\.pl)";
    regexPattern[SCL]   = "([-_\\.[:alnum:]]+\\.scl)";

    char  regexError[1024];
    int   regexCode;
    

    UpdateSession::open();
    
    size_t      nmatch = 0;
    regmatch_t* pmatch;
    for ( unsigned state = NODES ; state < END ; state++ ) { 
        // Patterns Compilation :
        // **********************
        if ( ( regexCode = regcomp ( &_regex[state], regexPattern[state], REG_EXTENDED ) ) ) {
          regerror ( regexCode, &_regex[state], regexError, 1024 );
          throw Error ( badRegex, "state", regexError );
        }
        
        // Pattern recognition and fileStringTab initialization :
        // ******************************************************
        nmatch = _regex[state].re_nsub;
        pmatch = ( regmatch_t* ) malloc ( sizeof (*pmatch) * nmatch );
        if ( !pmatch )
            throw Error ( "Big malloc problem : I hate malloc ;-)" );
        regexCode = regexec ( &_regex[state], _buffer, nmatch, pmatch, 0 );
        if ( regexCode == 0 ) {
            unsigned start = pmatch[0].rm_so;
            unsigned end   = pmatch[0].rm_eo;
            unsigned size  = end - start;

            char     subString[size+1];
            strncpy ( subString, &_buffer[start], size );
            subString[size] = '\0';
            _fileStringTab[state] = string ( subString );
        }

        free ( pmatch );
        regfree ( &_regex[state] );
    }

    return ( _fileStringTab[NODES] != "" ) && ( _fileStringTab[NETS] != "" );
}

bool BKParser::ScanNum ( unsigned& num )
// *************************************
{
    // *******************************
    // The NodeNum record looks like :
    // NumNodes : <num>
    // *******************************
    cdebug_log(100,0) << "ScanNum = " << _buffer;

    char *p_type, *p_num;
    if ( ( ( p_type = strtok ( _buffer, "\t \n:" ) ) != NULL ) &&
         ( ( p_num  = strtok ( NULL   , "\t \n:" ) ) != NULL ) ) {
        num = atol ( p_num );
        return true;
    }

    return false;
}

bool BKParser::ScanDegree ( unsigned& degree, Name& netName )
// **********************************************************
{
    // **********************************
    // The NetDregree record looks like :
    // NetDegree : <degree> [netName]
    // **********************************
    cdebug_log(100,0) << "ScanDegree = " << _buffer;

    bool mDegree = false;
    bool mName   = false;
    char *p_type, *p_token;
    if ( ( p_type = strtok ( _buffer, "\t \n:" ) ) == NULL ) {
        return false;
    }
    while ( ( p_token = strtok ( NULL, "\t \n:" ) ) != NULL ) {
        if ( isNumber ( p_token ) ) {
            degree  = atol ( p_token );
            mDegree = true;
            _nbNets++;
        }
        else if ( isName ( p_token ) ) {
            netName = p_token;
            mName   = true;
        }
    }
    if ( mDegree && !mName ) {
        netName = Name ( "NET" + getString ( _nbNets ) );
        
        char ext = 'a';
        while ( ( _cell->getNet ( netName ) != NULL ) && ( ext <= 'z' ) ) {
            netName = Name ( getString ( netName ) + getString ( ext++ ) );
        }
        if ( ext > 'z' ) {
            OnError ( "Can't create automatic netName :" + getString ( netName ), NETS );
            return false;
        }
    }

    return mDegree;
}

bool BKParser::ScanNodes ( Name& name, DbU::Unit& width, DbU::Unit& height, bool& isPad )
// ****************************************************************************
{
    // **************************************
    // The Node record looks like :
    // <ins_name> <width> <height> [terminal]
    // **************************************
    cdebug_log(100,0) << "ScanNodes = " << _buffer;

    char *p_name, *p_width, *p_height, *p_term;
    if ( ( ( p_name   = strtok ( _buffer, "\t \n" ) ) != NULL ) &&
         ( ( p_width  = strtok ( NULL   , "\t \n" ) ) != NULL ) &&
         ( ( p_height = strtok ( NULL   , "\t \n" ) ) != NULL ) ) {
        name   = p_name;
        width  = DbU::lambda ( atol ( p_width  ) );
        height = DbU::lambda ( atol ( p_height ) );
        if ( ( p_term = strtok ( NULL, "\n" ) ) != NULL ) {
            isPad = ( p_term[0] != (char)0 );
        }

        return true;
    }

    return false;
}

bool BKParser::ScanNets ( Name& insName, Net::Direction& dir, DbU::Unit& dx, DbU::Unit& dy )
// *******************************************************************************
{
    // *********************************
    // The Net record looks like :
    // NetDegree : <degree> <net_name>
    // *********************************
    cdebug_log(100,0) << "ScanNets = " << _buffer;
    
    bool mName      = false;
    bool mDirection = false;
    bool mOffset    = false;
    char *p_name, *p_token;
    if ( ( p_name = strtok ( _buffer, "\t \n:" ) ) == NULL ) {
        return false;
    }
    insName = p_name;
    mName = true;
    while ( ( p_token = strtok ( NULL, "\t \n:" ) ) != NULL ) {
        if ( isDirection ( p_token ) ) {
            if      ( string ( p_token ) == "I" )
                dir = Net::Direction::IN;
            else if ( string ( p_token ) == "O" )
                dir = Net::Direction::OUT;
            else if ( string ( p_token ) == "B" )
                dir = Net::Direction::INOUT;
            else
                throw Error ( "Unknown net direction read !!" );
            mDirection = true;
        }
        else if ( isFloat ( p_token ) ) {
            dx = DbU::lambda ( atof ( p_token ) );
            if ( ( p_token = strtok ( NULL, "\t \n:" ) ) == NULL )
                OnError ( "Wrong syntax :offset misreading", NETS );
            if ( !isFloat ( p_token ) )
                OnError ( "Wrong syntax :offset misreading", NETS );
            dy = DbU::lambda ( atof ( p_token ) );
            mOffset = true;
            _nbNets++;
        }
    }
    if ( !mDirection )
        dir = Net::Direction::IN;  // Default Direction
    if ( !mOffset ) {
        dx = 0;
        dy = 0;
    }

    return mName;
}


bool BKParser::ScanWts ( Name& name, unsigned& weight )
// ****************************************************
{
    // ******************************
    // The Weight record looks like :
    // <ins_name> <weight>
    // ******************************
    cdebug_log(100,0) << "ScanWts = " << _buffer;

    //char *p_x, *p_y, *p_model, *p_name, *p_transf;

    //if (((p_x = __strtok (_buffer + 2, ",")) != NULL) &&
    //     ((p_y = __strtok (NULL, ",")) != NULL) &&
    //     ((p_model = __strtok (NULL, ",")) != NULL) &&
    //     ((p_name = __strtok (NULL, ",")) != NULL) &&
    //    ((p_transf = __strtok (NULL, "\n")) != NULL) ) {

    //    x = getUnit(atol(p_x) / _scale);
    //    y = getUnit(atol(p_y) / _scale);
    //    masterCellName = Name(lower(p_model));
    //    instName = Name(lower(p_name));

    //    string transfo = p_transf;
    //    // Conversion to Hurricane orientation code
    //    if (transfo == "NOSYM") orientation=Transformation::Orientation::ID;
    //    else if (transfo == "ROT_P") orientation=Transformation::Orientation::R1;
    //    else if (transfo == "SYMXY") orientation=Transformation::Orientation::R2;
    //    else if (transfo == "ROT_M") orientation=Transformation::Orientation::R3;
    //    else if (transfo == "SYM_X") orientation=Transformation::Orientation::MX;
    //    else if (transfo == "SY_RM") orientation=Transformation::Orientation::XR;
    //    else if (transfo == "SYM_Y") orientation=Transformation::Orientation::MY;
    //    else if (transfo == "SY_RP") orientation=Transformation::Orientation::YR;
    //    else {
    //        OnError("Unknown Orientation code " + transfo);
    //        orientation=Transformation::Orientation::ID;
    //    }
    //    return true;
    //}
    return false;
}


bool BKParser::ScanPl ( Name& name, DbU::Unit& x, DbU::Unit& y, Transformation::Orientation& orient, bool& isFixed )
// *******************************************************************************************************
{
    // *************************************
    // The Placement record looks like :
    // <ins_name> <x> <y> : <orient> [FIXED]
    // *************************************
    cdebug_log(100,0) << "ScanPl = " << _buffer;

    char *p_name, *p_x, *p_y, *p_orient, *p_fixed;
    if ( ( ( p_name   = strtok ( _buffer, "\t \n"  ) ) != NULL ) &&
         ( ( p_x      = strtok ( NULL   , "\t \n"  ) ) != NULL ) &&
         ( ( p_y      = strtok ( NULL   , "\t \n:" ) ) != NULL ) &&
         ( ( p_orient = strtok ( NULL   , "\t \n:"  ) ) != NULL ) ) {
        name = p_name;
        x    = DbU::lambda ( atol ( p_x ) );
        y    = DbU::lambda ( atol ( p_y ) );
        if      ( string ( p_orient ) == "N" )
            orient = Transformation::Orientation::ID;
        else if ( string ( p_orient ) == "E" )
            orient = Transformation::Orientation::R3;
        else if ( string ( p_orient ) == "S" )
            orient = Transformation::Orientation::R2;
        else if ( string ( p_orient ) == "W" )
            orient = Transformation::Orientation::R1;
        else if ( string ( p_orient ) == "FN" )
            orient = Transformation::Orientation::MX;
        else if ( string ( p_orient ) == "FE" )
            orient = Transformation::Orientation::YR;
        else if ( string ( p_orient ) == "FS" )
            orient = Transformation::Orientation::MY;
        else if ( string ( p_orient ) == "FW" )
            orient = Transformation::Orientation::XR;
        else
            throw Error ( "Unknown transformation read !!" );

        if ( ( p_fixed = strtok ( NULL, "\n" ) ) != NULL ) {
            isFixed = ( p_fixed[0] != (char)0 );
        }

        return true;
    }

    return false;
}


//bool BKParser::ScanScl ( Name& name )
//// **********************************
//{
//    // *******************************
//    // The Big Via record looks like :
//    // B <X>,<Y>,<DX>,<DY>,<ViaType>,<NetName|*>
//    // Ex :
//    // *******************************
//    trace << "ScanScl = " << _buffer;
//    //char *p_x, *p_y, *p_dx, *p_dy, *p_viaType, *p_name;
//    //if (((p_x = __strtok (_buffer + 2, ",")) != NULL) &&
//    //     ((p_y = __strtok (NULL, ",")) != NULL) &&
//    //     ((p_dx = __strtok (NULL, ",")) != NULL) &&
//    //     ((p_dy = __strtok (NULL, ",")) != NULL) &&
//    //     ((p_viaType = __strtok (NULL, ",")) != NULL) &&
//    //     ((p_name = __strtok (NULL, "\n")) != NULL)) {
//
//    //    x = getUnit(atol(p_x) / _scale);
//    //    y = getUnit(atol(p_y) / _scale);
//    //    w = getUnit(atol(p_dx) / _scale);
//    //    h = getUnit(atol(p_dy) / _scale);
//    //    layerName = Name(p_viaType);
//    //    if ((p_name[ 0 ] == '\0') || (! strcmp(p_name, "*" )))
//    //        netName = Name("");
//    //    else
//    //        netName = Name(lower(p_name));
//
//    //    return true;
//    //}
//    return false;
//}
//

// ***************************************************************************************
// Miscellaneous utilitarians
// ***************************************************************************************

Transformation getTransformation ( const Box& abox, const DbU::Unit& x, const DbU::Unit& y, const Transformation::Orientation& orientation )
// *******************************************************************************************************************************
{
    // Creates the Hurricane Transformation record for an Instance, from :
    // * the Instance Abutment Box
    // * the x, y location of the lower left corner
    // * the Hurricane orientation code.

    switch ( orientation ) {
        case Transformation::Orientation::ID :
            return Transformation ( x - abox.getXMin(), y - abox.getYMin(), orientation);
        case Transformation::Orientation::R1 :
            return Transformation ( x + abox.getYMax(), y - abox.getXMin(), orientation);
        case Transformation::Orientation::R2 :
            return Transformation ( x + abox.getXMax(), y + abox.getYMax(), orientation);
        case Transformation::Orientation::R3 :
            return Transformation ( x - abox.getYMin(), y + abox.getXMax(), orientation);
        case Transformation::Orientation::MX :
            return Transformation ( x + abox.getXMax(), y - abox.getYMin(), orientation);
        case Transformation::Orientation::XR :
            return Transformation ( x + abox.getYMax(), y + abox.getXMax(), orientation);
        case Transformation::Orientation::MY :
            return Transformation ( x - abox.getXMin(), y + abox.getYMax(), orientation);
        case Transformation::Orientation::YR :
            return Transformation ( x - abox.getYMin(), y - abox.getXMin(), orientation);
    }
    return Transformation();
}


// void UpdateSizes ( Contact* contact )
// // **********************************
// {
//     DbU::Unit width = 0;
//     DbU::Unit height = 0;
//     const Layer* layer = contact->getLayer();
//   //if ( layer->getType() == CompositeLayer::Type::VIA ) {
//     width  = max ( contact->getWidth(),  layer->getMinimalSize() );
//     height = max ( contact->getHeight(), layer->getMinimalSize() );
//   //}
//
//     for_each_hook ( hook, contact->getBodyHook()->getSlaveHooks() ) {
//         Component* component = hook->getComponent();
//         if ( Vertical* vertical = dynamic_cast<Vertical*> ( component ) ) {
//             width  = max ( width,  vertical->getWidth() );
//         } else if ( Horizontal* horizontal = dynamic_cast<Horizontal*> ( component ) ) {
//             height = max ( height, horizontal->getWidth() );
//         }
//         end_for;
//     }
//     contact->setSizes ( width, height );
// }


// ***************************************************************************************
// BK Loader
// ***************************************************************************************


void  BKParser::LoadFromFile ( const string& cellPath, Cell* cell )
// ****************************************************************
{
    bool disabledFlag = Go::autoMaterializationIsDisabled();
    Go::disableAutoMaterialization();   // We force the mode

    CatalogProperty *sprop = ( CatalogProperty* ) cell->getProperty ( CatalogProperty::getPropertyName() );
    if ( sprop == NULL )
      throw Error ( "Missing CatalogProperty in cell %s.\n" , getString ( cell->getName() ).c_str() );

    Catalog::State *state = sprop->getState ();

    state->setPhysical ( true );

    IoFile filestream ( cellPath );
    filestream.open ( "r" );
    _file = filestream.getFile ();

    _cell         = cell;

    string _pathString = cellPath.substr ( 0, cellPath.rfind ( "/" ) );
    
    unsigned int pstate = AUX;

    string fileString;
    _fileStringTab[0] = cellPath;
    fileString = _pathString + "/" + _fileStringTab[pstate];
    
    bool noError = true;
    bool scanOk;

    unsigned int nodes_count          = 0;
    unsigned int nets_count           = 0;
    unsigned int netsall_count        = 0;
    unsigned int lines_done           = 0;
    unsigned int lines_progress_count = 0; 
    
    const char*  attente              = "\\|/-";
    unsigned int att_idx              = 0;
    
    try {
        while ( noError ) {
            _buffer[0] = fgetc ( _file );
            if ( _buffer[0] == '\n' ) {
                _lineNumber++;
                continue;
            }
            fgets ( &_buffer[1], sizeof ( _buffer ) - 1, _file );
            _lineNumber++;

            if ( _buffer[0] == '#'  )               continue;

            
            if ( ! strncmp ( _buffer, "UCLA", 4 ) ) continue;
            
            if ( _buffer[0] != ( char ) EOF ) {
                switch ( pstate ) {
                    case AUX:
                        cerr << "         " << tab << "Charging aux...\r";
                        scanOk = ScanAux ();
                        if ( !scanOk ) {
                            noError = false;
                            OnAbort ( "Problem while parsing : missing .nodes or .nets file", pstate );
                        }
                        break;
                    case NODES:
                        {
                        cerr << "         " << tab << "Charging... \r"; cerr.flush();
                        if ( ! strncmp ( _buffer, "NumNodes",      8 ) ) {
                            unsigned num = 0;
                            scanOk = ScanNum ( num );
                            if ( !scanOk ) {
                                noError = false;
                                OnError ( "\nProblem while parsing NumNodes", pstate );
                            }
                            nodes_count = num;
                            continue;
                        }
                        if ( ! strncmp ( _buffer, "NumTerminals", 12 ) )
                            continue;
                        Name name;
                        DbU::Unit width, height;
                        bool isPad = false;
                        scanOk = ScanNodes ( name, width, height, isPad );
                        if ( !scanOk ) {
                            noError = false;
                            OnError ( "\nProblem while parsing a node", pstate );
                        }
                        if ( nodes_count == 0 )
                            OnAbort ( "\nWrong declaration of NumNodes", pstate );
                        lines_progress_count++; 
                        lines_done = 100 * lines_progress_count / nodes_count;
                        cerr << "         " << tab << "Charging...  [" << lines_done << "%]  "
                             << attente[att_idx] << "      \r"; cerr.flush();
                        att_idx = (att_idx+1) % 4;
                        Cell* masterCell = Cell::create ( _cell->getLibrary(), Name ( getString ( name ) + "_model" ) );
                        masterCell->setAbutmentBox ( Box ( 0, 0, width, height ) );
                        if ( isPad )    masterCell->setPad ( true );
                        Instance::create ( _cell, name,  masterCell );
                        break;
                        }
                    case NETS:
                        cerr << "         " << tab << "Charging... \r"; cerr.flush();
                        if ( ! strncmp ( _buffer, "NumNets", 7 ) ) {
                            unsigned num = 0;
                            scanOk = ScanNum ( num );
                            if ( !scanOk ) {
                                noError = false;
                                OnError ( "\nProblem while parsing NumNets", pstate );
                            }
                            nets_count += num;
                            netsall_count += num;
                            continue;
                        }
                        if ( ! strncmp ( _buffer, "NumPins",   7 ) ) {
                            unsigned num = 0;
                            scanOk = ScanNum ( num );
                            if ( !scanOk ) {
                                noError = false;
                                OnError ( "\nProblem while parsing NumPins", pstate );
                            }
                            netsall_count += num;
                            continue;
                        }
                        if ( nets_count == 0 )
                            OnAbort ( "\nWrong declaration of NumNets", pstate );
                        if ( netsall_count == nets_count )
                            OnAbort ( "\nWrong declaration of NumPins", pstate );

                        if ( ! strncmp ( _buffer, "NetDegree", 9 ) ) {
                            unsigned degree = 0;
                            Name     netName;
                            scanOk = ScanDegree ( degree, netName );
                            if ( !scanOk ) {
                                noError = false;
                                OnError ( "\nProblem while parsing a net", pstate );
                            }
                            lines_progress_count++; 
                            Net* net = Net::create ( _cell, netName );
                            if ( !net )    throw Error ( "Can't create net " + getString ( netName ) );
                            for ( unsigned i = 0 ; i < degree ; i++ ) {
                                // Now reading next line
                                _buffer[0] = fgetc ( _file );
                                if ( _buffer[0] == '\n' ) {
                                    _lineNumber++;
                                    continue;
                                }
                                fgets ( &_buffer[1], sizeof ( _buffer ) - 1, _file );
                                _lineNumber++;

                                if ( _buffer[0] == '#'  )               continue;

                                Name           insName;
                                Net::Direction dir;
                                DbU::Unit           dx;
                                DbU::Unit           dy;
                                scanOk = ScanNets ( insName, dir, dx, dy );
                                if ( !scanOk ) {
                                    noError = false;
                                    string message = "\nProblem while parsing net " + getString ( netName )
                                                   + " with degree " + getString ( degree );
                                    OnAbort ( message, pstate );
                                }
                                Instance* instance = _cell->getInstance ( insName );
                                if ( !instance )   throw Error ( "Can't get instance " + getString ( insName ) );
                                Cell*     model    = instance->getMasterCell();
                                if ( !model )      throw Error ( "Can't get model of instance " + getString ( insName ) );
                                unsigned index = 0;
                                Name tempName = netName;
                                while ( model->getNet ( tempName ) )
                                    tempName = Name ( getString ( netName ) + "_" + getString ( index++ ) );
                                //cerr << endl << "    Trying to create masterNet " << getString ( tempName ) << " in masterCell " << model << endl;
                                Net* masterNet = Net::create ( model, tempName );
                                if ( !masterNet )  throw Error ( "Can't create master net " + getString ( tempName ) + " on instance " + getString ( insName ) );
                                masterNet->setDirection ( dir );
                                masterNet->setPosition  ( Point ( dx, dy ) );
                                masterNet->setExternal  ( true );  // To create plug
                                Plug* plug = instance->getPlug ( masterNet );
                                if ( !plug )       throw Error ( "Can't get plug of net " + getString ( netName ) + " on instance " + getString ( insName ) );
                                plug->setNet ( net );
                                lines_progress_count++; 
                                lines_done = 100 * lines_progress_count / netsall_count;
                                cerr << "         " << tab << "Charging...  [" << lines_done << "%]  "
                                     << attente[att_idx] << "      \r"; cerr.flush();
                                att_idx = (att_idx+1) % 4;
                            }
                        }

                        break;            
                    case WTS:             
                        cerr << "         " << tab << "Charging... \r"; cerr.flush();
                        break;
                    case PL:
                        {
                        Name name;
                        DbU::Unit x=0, y=0;
                        Transformation::Orientation orient;
                        bool isFixed = false;
                        scanOk = ScanPl ( name, x, y, orient, isFixed );
                        if ( !scanOk ) {
                            noError = false;
                            OnError ( "\nProblem while parsing", pstate );
                        }
                        if ( nodes_count == 0 )
                            OnAbort ( "\nWrong declaration of NumNodes or NumTerminals", pstate );
                        lines_progress_count++; 
                        lines_done = 100 * lines_progress_count / nodes_count;
                        cerr << "         " << tab << "Charging...  [" << lines_done << "%]  "
                             << attente[att_idx] << "      \r"; cerr.flush();
                        att_idx = (att_idx+1) % 4;
                        Instance * instance = _cell->getInstance ( name );
                        if ( instance ) {
                            Box abox = instance->getAbutmentBox();
                            instance->setTransformation ( getTransformation ( abox, x, y, orient ) );
                            if ( isFixed )
                                instance->setPlacementStatus ( Instance::PlacementStatus::FIXED );
                        }
                        else {
                            OnError ( "\nCouldn't find instance : " + getString ( name ) + " : not in .nodes file", pstate );
                        }
                        break;
                        }
                    case SCL:
                        cerr << "         " << tab << "Charging... \r";
                        break;
                    default:
                        break;
                }
            }
            else {
                pstate++;
                if ( pstate == END )    
                   break;
                
                if ( ! _fileStringTab[pstate].empty() ) {
                    fclose ( _file );
                    _lineNumber          = 0;
                    att_idx              = 0;
                    lines_done           = 0;
                    fileString = _pathString + "/" + _fileStringTab[pstate];
                    _file = fopen ( fileString.c_str(), "r" );
                    if ( pstate != NODES ) {
                        cerr << endl;
                        if ( !lines_progress_count )
                            cerr << "         " << tab << "Warning : Nothing parsed !!" << endl;
                        lines_progress_count = 0;
                    }
                    cerr << "       " << tab << "+ " << fileString << endl;
                    if ( ! _file )
                        OnError ( "Unable to find file : " + fileString, pstate );
                }
            }
        } // End of while
        cerr << endl;
    } // End of try
    catch (Exception& exception) {
        fclose(_file);
        if (_cell) _cell->destroy();
        tab--;
        cout << tab << exception.what() << endl;
        OnAbort("Can't load cell " + getString ( _cell->getName() ), AUX);
    }
    catch (...) {
        fclose(_file);
        if (_cell) _cell->destroy();
        tab--;
        cout << tab << Error("Can't load cell " + getString(_cell->getName()))._getString() << endl;
        OnAbort("Can't load cell " + getString ( _cell->getName() ), AUX );
    }

    //filestream.close ();

    if (!_cell) OnAbort("Cell Was Not Created", AUX);
    
    UpdateSession::close();

    Go::enableAutoMaterialization();
    _cell->materialize();

    if (disabledFlag)  // We restore the initial state if needed
        Go::disableAutoMaterialization();

    // Resizing AbutmentBox !!
    _cell->setAbutmentBox ( _cell->getBoundingBox() );

    //trace_off();
}

}  // End of anonymous namespace.




namespace CRL {


void  bookshelfParser ( const string cellPath, Cell *cell )
{
  cmess2 << "     " << tab << "+ " << cellPath << endl;

  BKParser  parser ( AllianceFramework::get() );

  parser.LoadFromFile ( cellPath, cell );
}


}


// **************************************************************************************
// Copyright (C) LIP6  &  BULL S.A. 2003-2005 All rights reserved
// **************************************************************************************
