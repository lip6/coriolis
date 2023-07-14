// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./crlcore/Utilities.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include <cstdarg>
#include <cstdio>
#include <ostream>
#include <iostream>
#include <string>
#include "hurricane/utilities/Path.h"
#include "hurricane/Commons.h"
#include "hurricane/Error.h"
#include "hurricane/Slot.h"
#include "hurricane/DbU.h"


namespace CRL {

  using Hurricane::_TName;
  using Hurricane::Record;
  using Hurricane::Error;
  using std::string;
  using std::ostream;


// -------------------------------------------------------------------
// Class  :  "CRL::System".


  class  System {
    public:
      static        System*                get           ();
      static inline bool                   getCatchCore  ();
      static inline const Utilities::Path& getPath       ( const std::string& );
      static inline bool                   setCatchCore  ( bool catchCore );
      static inline void                   runPythonInit ();
    private:
      static System*                                      _singleton;
             bool                                         _catchCore;
             std::map<const std::string,Utilities::Path>  _pathes;
    private:
                                    System         ();
                                    System         ( const System &other );
             System&                operator=      ( const System &other );
      static void                   _trapSig       ( int sig );
      inline bool                   _getCatchCore  ();
             const Utilities::Path& _getPath       ( const std::string& );
      inline bool                   _setCatchCore  ( bool catchCore );
             void                   _runPythonInit ();
  };


  inline bool                   System::getCatchCore  ()                         { return get()->_getCatchCore(); }
  inline const Utilities::Path& System::getPath       ( const std::string& key ) { return get()->_getPath(key); }
  inline bool                   System::setCatchCore  ( bool catchCore )         { return get()->_setCatchCore(catchCore); }
  inline void                   System::runPythonInit ()                         { return get()->_runPythonInit(); }


// -------------------------------------------------------------------
// Class  :  "CRL::IoFile ()".
//
// Class wrapper for the C FILE* stream.


  class IoFile {
    public:
    // Constructors.
      inline         IoFile        ( string path="<unbound>" );
    // Methods
      inline bool    isOpen        () const;
      inline bool    eof           () const;
             char*   readLine      ( char* buffer, size_t length );
      inline FILE*   getFile       ();
      inline size_t  getLineNumber () const;
             bool    open          ( const string& mode );
             void    close         ();
      inline void    rewind        ();
    // Hurricane management.
      inline string  _getTypeName  () const;
             string  _getString    () const;
             Record* _getRecord    () const;

    private:
    // Internal - Attributes.
             FILE*   _file;
             string  _path;
             string  _mode;
             size_t  _lineNumber;
             bool    _eof;

    // Internal - Constructor.
                     IoFile       ( const IoFile& );
  };




// -------------------------------------------------------------------
// Function  :  "CRL::_PName()".
//
// Generate strings with prefixes.

  inline string  _PName ( const string& s ) { return ( "CRL::" + s ); }


// Inline Methods.
  inline bool    System::_getCatchCore  () { return _catchCore; }
  inline bool    System::_setCatchCore  ( bool catchCore ) { return _catchCore = catchCore; }

  inline         IoFile::IoFile         ( string path ): _file(NULL)
                                                       , _path(path)
                                                       , _mode("")
                                                       , _lineNumber(0)
                                                       , _eof(false) {}
  inline bool    IoFile::isOpen         () const { return _file!=NULL; }
  inline bool    IoFile::eof            () const { return _eof; }
  inline FILE*   IoFile::getFile        () { return _file; }
  inline size_t  IoFile::getLineNumber  () const { return _lineNumber; }
  inline void    IoFile::rewind         () { if (_file) std::rewind(_file); _lineNumber=0; }
  inline string  IoFile::_getTypeName   () const { return _TName("IoFile"); }


// -------------------------------------------------------------------
// Error Messages.

  extern const char* BadAllocProperty;
  extern const char* BadCreate;
  extern const char* NullDataBase;
  extern const char* NullTechnology;
  extern const char* NullLibrary;
  extern const char* NullCell;
  extern const char* BadFopen;
  extern const char* BadColorValue;


}  // End of CRL namespace.


// -------------------------------------------------------------------
// Class  :  "::tty()".

class tty {
  public:
    enum Flags { Black     = 0
               , Red       = 1
               , Green     = 2
               , Yellow    = 3
               , Blue      = 4
               , Magenta   = 5
               , Cyan      = 6
               , White     = 7
               , Reset     = 9
               , Normal    = 0
               , Bright    = (1<<4)
               , ColorMask = 0x0F
               , TypeMask  = 0xF0
               };
  public:
    inline static void          setWidth     ( int );
    inline static int           getWidth     ();
    inline static void          enable       ();
    inline static void          disable      ();
    inline static bool          enabled      ();
    inline static std::ostream& cr           ( std::ostream& );
    inline static std::ostream& reset        ( std::ostream& );
    inline static std::ostream& bold         ( std::ostream& );
    inline static std::ostream& faint        ( std::ostream& );
    inline static std::ostream& italic       ( std::ostream& );
    inline static std::ostream& underline    ( std::ostream& );
    inline static std::ostream& slowBlink    ( std::ostream& );
    inline static std::ostream& rapidBlink   ( std::ostream& );
    inline static std::ostream& negative     ( std::ostream& );
    inline static std::ostream& conceal      ( std::ostream& );
    inline static std::ostream& underline2   ( std::ostream& );
    inline static std::ostream& normal       ( std::ostream& );
    inline static std::ostream& underlineOff ( std::ostream& );
    inline static std::ostream& blinkOff     ( std::ostream& );
    inline static std::ostream& positive     ( std::ostream& );
    inline static std::ostream& reveal       ( std::ostream& );
    inline static std::string   fgcolor      ( unsigned int );
    inline static std::string   bgcolor      ( unsigned int );
  private:
    static bool        _enabled;
    static const char* _intensity[4];
    static int         _width;
};

inline void          tty::setWidth     ( int width ) { _width=width; }
inline int           tty::getWidth     () { return _width; }
inline void          tty::enable       () { _enabled=true; }
inline void          tty::disable      () { _enabled=false; }
inline bool          tty::enabled      () { return _enabled; }
inline std::ostream& tty::cr           ( std::ostream& o ) { if (_enabled) o<<"\r"      ; return o; }
inline std::ostream& tty::reset        ( std::ostream& o ) { if (_enabled) o<<"\x1b[0m" ; return o; }
inline std::ostream& tty::bold         ( std::ostream& o ) { if (_enabled) o<<"\x1b[1m" ; return o; }
inline std::ostream& tty::faint        ( std::ostream& o ) { if (_enabled) o<<"\x1b[2m" ; return o; }
inline std::ostream& tty::italic       ( std::ostream& o ) { if (_enabled) o<<"\x1b[3m" ; return o; }
inline std::ostream& tty::underline    ( std::ostream& o ) { if (_enabled) o<<"\x1b[4m" ; return o; }
inline std::ostream& tty::slowBlink    ( std::ostream& o ) { if (_enabled) o<<"\x1b[5m" ; return o; }
inline std::ostream& tty::rapidBlink   ( std::ostream& o ) { if (_enabled) o<<"\x1b[6m" ; return o; }
inline std::ostream& tty::negative     ( std::ostream& o ) { if (_enabled) o<<"\x1b[7m" ; return o; }
inline std::ostream& tty::conceal      ( std::ostream& o ) { if (_enabled) o<<"\x1b[8m" ; return o; }
inline std::ostream& tty::underline2   ( std::ostream& o ) { if (_enabled) o<<"\x1b[21m"; return o; }
inline std::ostream& tty::normal       ( std::ostream& o ) { if (_enabled) o<<"\x1b[22m"; return o; }
inline std::ostream& tty::underlineOff ( std::ostream& o ) { if (_enabled) o<<"\x1b[24m"; return o; }
inline std::ostream& tty::blinkOff     ( std::ostream& o ) { if (_enabled) o<<"\x1b[25m"; return o; }
inline std::ostream& tty::positive     ( std::ostream& o ) { if (_enabled) o<<"\x1b[27m"; return o; }
inline std::ostream& tty::reveal       ( std::ostream& o ) { if (_enabled) o<<"\x1b[28m"; return o; }

inline std::string  tty::fgcolor ( unsigned int mask )
{
  if ( not _enabled ) return "";
  std::string sequence ("\x1b[");
  sequence += ((mask&Bright)?"4":"3");
  sequence += ('0'+(mask&ColorMask));
  sequence += "m";
  return sequence;
}

inline std::string  tty::bgcolor ( unsigned int mask )
{
  if ( not _enabled ) return "";
  std::string sequence ("\x1b[");
  sequence += ((mask&Bright)?"10":"9");
  sequence += ('0'+(mask&ColorMask));
  sequence += "m";
  return sequence;
}


// -------------------------------------------------------------------
// Class  :  "::mstream()".
//
// Wrapper around the STL ostream which uses a verbose level to choose
// wether to print or not.

  class mstream : public std::ostream {
    public:
      enum StreamMasks { PassThrough   = 0x00000001
                       , Verbose0      = 0x00000002
                       , Verbose1      = 0x00000004
                       , Verbose2      = 0x00000008
                       , Info          = 0x00000010
                       , Paranoid      = 0x00000020
                       , Bug           = 0x00000040
                       };
    public:
      static        void          enable          ( unsigned int mask );
      static        void          disable         ( unsigned int mask );
      inline                      mstream         ( unsigned int mask, std::ostream &s );
      inline        bool          enabled         () const;
      inline        unsigned int  getStreamMask   () const;
      static inline unsigned int  getActiveMask   ();
      inline        void          setStreamMask   ( unsigned int mask );
      inline        void          unsetStreamMask ( unsigned int mask );
    // Overload for formatted outputs.
      template<typename T> inline mstream& operator<< ( T& t );
      template<typename T> inline mstream& operator<< ( T* t );
      template<typename T> inline mstream& operator<< ( const T& t );
      template<typename T> inline mstream& operator<< ( const T* t );
                           inline mstream& put        ( char c );
                           inline mstream& flush      ();
    // Overload for manipulators.
                           inline mstream &operator<< ( std::ostream &(*pf)(std::ostream &) );

    // Internal: Attributes.
    private:
      static unsigned int  _activeMask;
             unsigned int  _streamMask;
  };


  inline               mstream::mstream        ( unsigned int mask, std::ostream& s ): std::ostream(s.rdbuf()) , _streamMask(mask) {}  
  inline bool          mstream::enabled        () const { return (_streamMask & _activeMask); }
  inline unsigned int  mstream::getStreamMask  () const { return  _streamMask; }
  inline unsigned int  mstream::getActiveMask  ()       { return  _activeMask; }
  inline void          mstream::setStreamMask  ( unsigned int mask ) { _streamMask |= mask; }
  inline void          mstream::unsetStreamMask( unsigned int mask ) { _streamMask &= ~mask; }
  inline mstream&      mstream::put            ( char c ) { if (enabled()) static_cast<std::ostream*>(this)->put(c); return *this; }  
  inline mstream&      mstream::flush          () { if (enabled()) static_cast<std::ostream*>(this)->flush(); return *this; }  
  inline mstream&      mstream::operator<<     ( std::ostream& (*pf)(std::ostream&) ) { if (enabled()) (*pf)(*this); return *this; }

// For POD Types.
  template<typename T>
  inline mstream& mstream::operator<< ( T& t )
  { if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

  template<typename T>
  inline mstream& mstream::operator<< ( T* t )
  { if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

  template<typename T>
  inline mstream& mstream::operator<< ( const T& t )
  { if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

  template<typename T>
  inline mstream& mstream::operator<< ( const T* t )
  { if (enabled()) { *(static_cast<std::ostream*>(this)) << t; } return *this; };

// For STL Types.
  inline mstream& operator<< ( mstream& o, const std::string& s )
  { if (o.enabled()) { static_cast<std::ostream&>(o) << s; } return o; };

// Specific non-member operator overload. Must be one for each type.
#define  MSTREAM_V_SUPPORT(Type)                           \
  inline mstream& operator<< ( mstream& o, Type t )        \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; }; \
                                                           \
  inline mstream& operator<< ( mstream& o, const Type t )  \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; };

#define  MSTREAM_R_SUPPORT(Type)                           \
  inline mstream& operator<< ( mstream& o, const Type& t ) \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; }; \
                                                           \
  inline mstream& operator<< ( mstream& o, Type& t )       \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; };

#define  MSTREAM_P_SUPPORT(Type)                           \
  inline mstream& operator<< ( mstream& o, const Type* t ) \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; }; \
                                                           \
  inline mstream& operator<< ( mstream& o, Type* t )       \
  { if (o.enabled()) { static_cast<std::ostream&>(o) << t; } return o; };

#define  MSTREAM_PR_SUPPORT(Type) \
         MSTREAM_P_SUPPORT(Type)  \
         MSTREAM_R_SUPPORT(Type)


// -------------------------------------------------------------------
// Shared objects.

extern mstream  cmess0;
extern mstream  cmess1;
extern mstream  cmess2;
extern mstream  cinfo;
extern mstream  cparanoid;
extern mstream  cbug;


// -------------------------------------------------------------------
// Class  :  "::linefill()".
//
// Wrapper around the STL ostream which try print unbufferized filed
// lines.

class linefill : public std::ostream {
  public:
    inline               linefill                        ( const std::string& header, mstream &s );
  // Overload for formatted outputs.
    template<typename T> inline linefill&     operator<< ( T& t );
    template<typename T> inline linefill&     operator<< ( T* t );
    template<typename T> inline linefill&     operator<< ( const T& t );
    template<typename T> inline linefill&     operator<< ( const T* t );
                         inline mstream&      base       ();
                         inline void          _print     ( const std::string& field );
                         inline linefill&     flush      ();
                         inline linefill&     reset      ();
  // Overload for manipulators.
                         inline linefill&     operator<< ( std::ostream &(*pf)(std::ostream &) );

  // Internal: Attributes.
  private:
    mstream&      _base;
    std::string   _header;
    size_t        _width;
    size_t        _lines;
};


inline               linefill::linefill   ( const std::string& header, mstream& s ): std::ostream(s.rdbuf()), _base(s), _header(header), _width(0), _lines(0) {}  
//inline std::ostream& linefill::base       () { return (*static_cast<std::ostream*>(this)); }  
inline mstream&      linefill::base       () { return _base; }  
inline linefill&     linefill::reset      () { _base << std::endl; _width=0; return *this; }  
inline linefill&     linefill::flush      () { static_cast<mstream&>(_base).flush(); return *this; }  
inline linefill&     linefill::operator<< ( std::ostream& (*pf)(std::ostream&) ) { (*pf)(static_cast<mstream&>(_base)); return *this; }

inline void  linefill::_print ( const std::string& field ) {
  size_t fieldWidth = field.length();
  if ( _width+fieldWidth > 80 ) { _width = 0; ++_lines; }
  if ( _width == 0 ) {
    if ( _lines > 0 ) base() << std::endl;
    base() << _header; _width+=_header.length();
  } else
    base() << " ";

  _width += fieldWidth + 1;
  base() << field;
  base().flush ();
}

template<typename T>
inline linefill& linefill::operator<< ( T& t )
{ std::ostringstream s; s << t; _print(s.str()); return *this; };

template<typename T>
inline linefill& linefill::operator<< ( T* t )
{ std::ostringstream s; s << t; _print(s.str()); return *this; };

template<typename T>
inline linefill& linefill::operator<< ( const T& t )
{ std::ostringstream s; s << t; _print(s.str()); return *this; };

template<typename T>
inline linefill& linefill::operator<< ( const T* t )
{ std::ostringstream s; s << t; _print(s.str()); return *this; };

// Specific non-member operator overload. Must be one for each type.
#define  LINEFILL_V_SUPPORT(Type)                            \
  inline linefill& operator<< ( linefill& o, const Type t )  \
    { std::ostringstream s; s << t; return o; };

#define  LINEFILL_R_SUPPORT(Type)                            \
  inline linefill& operator<< ( linefill& o, const Type& t ) \
    { std::ostringstream s; s << t; return o; };

#define  LINEFILL_P_SUPPORT(Type)                            \
  inline linefill& operator<< ( linefill& o, const Type* t ) \
    { std::ostringstream s; s << t; return o; };

#define  LINEFILL_PR_SUPPORT(Type) \
         LINEFILL_P_SUPPORT(Type)  \
         LINEFILL_R_SUPPORT(Type)

LINEFILL_PR_SUPPORT(std::string);


// -------------------------------------------------------------------
// Class  :  "::Dots".

class  Dots {
  public:
    static Dots          asPercentage ( const std::string& left, float );
    static Dots          asBool       ( const std::string& left, bool );
    static Dots          asInt        ( const std::string& left, int );
    static Dots          asUInt       ( const std::string& left, unsigned int );
    static Dots          asULong      ( const std::string& left, unsigned long );
    static Dots          asSizet      ( const std::string& left, size_t );
    static Dots          asDouble     ( const std::string& left, double );
    static Dots          asLambda     ( const std::string& left, Hurricane::DbU::Unit );
    static Dots          asLambda     ( const std::string& left, double );
    static Dots          asIdentifier ( const std::string& left, const std::string& );
    static Dots          asString     ( const std::string& left, const std::string& );
  private:
                         Dots         ( const std::string& left, const std::string& right );
    friend std::ostream& operator<<   ( std::ostream&, const Dots& );
  private:
    const std::string _left;
    const std::string _right;
};
