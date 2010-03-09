

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
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================


// -*- C++ -*-
//
// $Id: COptions.h,v 1.5 2006/02/19 00:52:46 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Options.h"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  "crlcore/Utilities.h"




#ifndef  __CRL_OPTIONS_H__
#define  __CRL_OPTIONS_H__




namespace CRL {


  using std::vector;



// -------------------------------------------------------------------
// Class  :  "CRL::COptions()".

  class COptions {

    private: class Option {
      private: string          _nameShort;
      private: string          _nameLong;
      private: bool            _hasArg;
      private: string          _defaultArg;
      private: vector<string>  _args;
      private: int             _times;
      private: int             _select;

      public: Option ( string  nameShort
                     , string  nameLong
                     , bool    hasArg=false
                     , string  defaultArg="" )
                : _nameShort(nameShort)
                , _nameLong(nameLong)
                , _hasArg(hasArg)
                , _defaultArg(defaultArg)
                , _args()
                , _times(0)
                , _select(0)
                { };

      public: string  getShort   () { return ( _nameShort  ); };
      public: string  getLong    () { return ( _nameLong   ); };
      public: bool    getHasArg  () { return ( _hasArg     ); };
      public: string  getDefault () { return ( _defaultArg ); };
      public: int     getTimes   () { return ( _times      ); };
      public: string& getArg     ( size_t i );

      public: void    SetSelect  ( int select ) { _select = select; };
      public: int     AddArg     ( string val )
                        { _args.push_back (val); return ( _times++ ); };
      public: void    Clear ()
                        { _args.clear (); _times = 0; _select = 0; }

      public: operator string ();
      public: operator bool   () { return ( _times > 0 ); };
      public: operator int    () { return ( _times ); };
    };

    typedef  vector<Option>  OptionVect;


    // Constants.
    public: static const string  _true;
    public: static const string  _false;
    public: static const string  _remains;
    public: static const string  _argvzero;
    public: static const string  _missing;

    // Attributes.
    private: bool        _clear;
    private: bool        _argHelp;
    private: bool        _argVerbose;
    private: bool        _argVeryVerbose;
    private: bool        _argCatchCore;
    private: Option      _missingBuiltin;
    private: OptionVect  _options;

    // Constructor.
    public: COptions ();

    // Private methods.
    private: OptionVect::iterator  Find ( string name );

    // Accessors.
    public: bool    getClear       () { return ( _clear ); };
    public: Option &getHelp        ();
    public: Option &getVerbose     ();
    public: Option &getVeryVerbose ();
    public: Option &getCatchCore   ();

    // Mofifiers.
    public: void  Clear ();
    public: void  Parse ( int argc, char *argv[] );
    public: void  Add   ( string  nameShort
                        , string  nameLong
                        , bool    hasArg=false
                        , string  defaultArg="" );
    public: void  AddHelp ();
    public: void  AddVerbose ();
    public: void  AddVeryVerbose ();
    public: void  AddCatchCore ();

    // Operators.
    public: Option &operator[] ( string key );

    // Hurricane managment.
    public: string _getPrint ();
  };




} // End of CRL namespace.




// x-----------------------------------------------------------------x
// |           Functions Overload for Hurricane Managment            |
// x-----------------------------------------------------------------x


inline std::string  getPrint    ( CRL::COptions &opts )                  { return ( opts._getPrint() ); }
inline std::ostream& operator<< ( std::ostream &o, CRL::COptions& opts ) { return ( o << getPrint(opts) ); }


#endif
