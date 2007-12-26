// ****************************************************************************************************
// File: Transistor.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_TRANSISTOR
#define HURRICANE_TRANSISTOR

#include "Cell.h"
#include "AnalogicalCommons.h"


//BEGIN_NAMESPACE_HURRICANE

namespace Hurricane {

class Library; 
class Name;
class Symbol;
class Record;
class GenTrans;

class Transistor : public Cell {
// ********************************

//#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

     public : typedef Cell Inherit; 


     public : class MaskVersion {
	      // ******************
                public : enum Code { VERSION1=0 };

		private: Code _code;

		public : explicit MaskVersion(const Code& code=VERSION1);
		public : MaskVersion(const MaskVersion&);
		public : MaskVersion& operator=(const MaskVersion&);

                public : bool operator==(const MaskVersion&) const; 

		public : operator const Code& () const { return _code; };
		public : const Code& GetCode() const { return _code; }; 

		public : string _GetTypeName() const { return _TName("Transistor::MaskVersion"); }; 
		public : string _GetString() const; 
		public : Record* _GetRecord() const;
		
	      };


     public  : class Type {
	      // *********
                  
		public : enum Code { INTERNAL=0, LEFT=1, RIGHT=2, SINGLE=3};

		private: Code _code;
			 
		public : explicit Type(const Code& code=INTERNAL);
		public : Type(const Type& type);
		public : Type& operator=(const Type& type);
		public : operator const Code&() const { return _code; };
			 
                public : void SetCode(const Code& code) { _code = code; }; 

		public : const Code& GetCode() const { return _code; };

		public : string _GetTypeName() const { return _TName("Transistor::Type"); }; 
		public : string _GetString() const;
		public : Record* _GetRecord() const;
	
	      };


     public : class MaskInfo {
	      // ***************
		 
		 // Attributs
		 // *********
                 private : double _l;
		 private : double _w ;
		 private : Type _type;
		 private : unsigned _nbDrainColumn;
		 private : unsigned _nbSourceColumn;

                 // Constructors
	         // ************
		 public : MaskInfo(const double& l, const double& w, const Type::Code& type=Type::INTERNAL 
				 , const unsigned& nbDrainColumn=1 
				 , const unsigned& nbSourceColumn=1);

                 public : virtual MaskInfo& operator=(const MaskInfo&);

		 private : MaskInfo(const MaskInfo& ); 

                 protected : virtual void  _PostCreate();
			   
                 // Destructors
		 // ***********
                 protected: virtual ~MaskInfo() {};
	         protected: virtual void _PreDelete(); 
		 public : virtual void Delete(); 

                 // Accessors
	         // *********
		 public : const double& GetL() const { return _l; };
		 public : const double& GetW() const { return _w; };
		 public : const unsigned & GetNbDrainColumn() const { return _nbDrainColumn; };
		 public : const unsigned & GetNbSourceColumn() const { return _nbSourceColumn; };
		 public : const Type& GetType() const { return _type; };

                 // Update
		 // ******
		 public : void SetL(const double& l) { _l=l;}; 
		 public : void SetW(const double& w) { _w=w;}; 
		 public : void SetNbDrainColumn(const unsigned& column) { _nbDrainColumn=column; };
		 public : void SetNbSourceColumn(const unsigned& column) { _nbSourceColumn=column; };
		 public : void SetType(const Type::Code& code) { _type.SetCode(code); };
		 public : void SetType(const Type& type) { _type = type; };

		 // Predicats
	         // *********

		 // Operators
	         // *********
		 public : virtual bool operator==(const MaskInfo&);

                 // Others
		 // ******
		 public : virtual string _GetTypeName() const =0;
		 public : virtual string _GetString() const;
		 public : virtual Record* _GetRecord() const;
	
	      };


     public : class MaskV1Info : public MaskInfo {
	      // *************************************

		 // type
		 // *****
		 public : typedef MaskInfo Inherit ;

		 // Attributs
		 // *********

		 // Constructors
		 // ************
		 public: MaskV1Info(const double& l, const double& w, const Type::Code& type=Type::INTERNAL 
			               , const unsigned& nbDrainColumn = 1
				       , const unsigned& nbSourceColumn = 1);

		 public: static MaskV1Info* Create(const double& l, const double& w, const Type::Code& type=Type::INTERNAL
			               , const unsigned& nbDrainColumn = 1 
				       , const unsigned& nbSourceColumn = 1);

	         public : MaskInfo& operator=(const MaskInfo&);

		 protected : void _PostCreate();

	         // Destructor
		 // ***********
		 public : virtual ~MaskV1Info() {};
		 protected: void _PreDelete();

		 // Operators
		 // *********
		 public : bool operator==(const MaskInfo&); 

		 // Others
	         // *********
		 public : virtual string _GetTypeName() const { return _TName("Transistor::MaskV1Info"); }; 
		 public : virtual string _GetString() const;
		 public : virtual Record* _GetRecord() const;
              };


#   if !defined(__DOXYGEN_PROCESSOR__)

// Attributes
// *******************
    private : char _type; 
    private : MaskInfo* _masqueInfo;	      
    private : GenTrans * _genTrans;	  
    //public : RealInfo * _realInfo;	      

    private : map<Net*, Box> _mapNet2Box;   // This Map Is For localize The Position Of Routing.

# endif 

// Constructors
// ************
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : Transistor(Library* library, const Name& name, char type);
# endif 

    public : static Transistor* Create(Library* library, const Name& name, char type);		
   
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : virtual void _PostCreate();


// Destructors
// ***********
    protected : ~Transistor() {};
    protected : virtual void _PreDelete();
# endif 

// Accessors
// *********	  
    public : char GetType() const { return _type; };
    public : MaskVersion GetMaskVersion() const { return _GetMaskInfoVersion(_masqueInfo); };
    public : const MaskInfo* GetMaskInfo() const { return _masqueInfo; };
    public : const double& GetL() const { return _masqueInfo->GetL(); };
    public : const double& GetW() const { return _masqueInfo->GetW(); };
    public : const unsigned& GetNbDrainColumn() const { return _masqueInfo->GetNbDrainColumn(); };
    public : const unsigned& GetNbSourceColumn() const { return _masqueInfo->GetNbSourceColumn(); };
    public : const char* GetDrainName() const  { return "DRAIN"; };
    public : const char* GetSourceName() const { return "SOURCE"; };
    public : const char* GetGridName() const   { return "GRID"; };
    public : Net* GetDrain() const  { return GetNet(GetDrainName()); };
    public : Net* GetSource() const { return GetNet(GetSourceName()); };
    public : Net* GetGrid() const   { return GetNet(GetGridName()); };
    public : const Type& GetAbutmentType() const { return _masqueInfo->GetType(); };

// Predicats
// *********
    public : bool IsNmos() const { return _type==TRANSN; };
    public : bool IsPmos() const { return _type==TRANSP; };
    public : bool IsInternal() const { return GetAbutmentType().GetCode()==Type::INTERNAL; };
    public : bool IsLeft() const     { return GetAbutmentType().GetCode()==Type::LEFT; };
    public : bool IsRight() const    { return GetAbutmentType().GetCode()==Type::RIGHT; };
    public : bool IsSingle() const   { return GetAbutmentType().GetCode()==Type::SINGLE; };

// Updators
// ********	    
    public : void SetL(const double& l) { _masqueInfo->SetL(l); };
    public : void SetW(const double& w) { _masqueInfo->SetW(w); };

//# endif

#   if !defined(__DOXYGEN_PROCESSOR__)
// Others
// ******
    public : virtual string _GetTypeName() const {return _TName("Transistor");};
    public : virtual string _GetString() const;
    public : virtual Record* _GetRecord() const;
    public : const GenTrans* _GetGenTrans() const {return _genTrans; }; 
    public : static MaskVersion _GetMaskInfoVersion(MaskInfo*) ;
    public : static MaskInfo* _CreateMaskInfo(const MaskVersion&) ;
    public : map<Net*, Box>* _GetMapNet2Box() { return &_mapNet2Box; };

# endif

// Operators
// *********
   public : void SetMaskInfo(MaskInfo*);
   public : void CreateLayout();
   public : void DuplicateLayout(Transistor* transistor) ;


}; 


#   if !defined(__DOXYGEN_PROCESSOR__)
// -------------------------------------------------------------------
// Class  :  "Proxy...<const Transistor::MaskVersion::Code*>".

template<>
  inline string  ProxyTypeName<Transistor::MaskVersion::Code>
                              ( const Transistor::MaskVersion::Code* object )
                              { return "<PointerSlotAdapter<Transistor::MaskVersion::Code>>"; }

template<>
  inline string  ProxyString  <Transistor::MaskVersion::Code>
                              ( const Transistor::MaskVersion::Code* object )
                              {
                                switch ( *object ) {
                                  case Transistor::MaskVersion::VERSION1: return "VERSION1";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Transistor::MaskVersion::Code>
                              ( const Transistor::MaskVersion::Code* object )
                              {
                                Record* record = new Record(GetString(object));
                                record->Add(GetSlot("Code", (unsigned int*)object));
                                return record;
                              }


// -------------------------------------------------------------------
// Class  :  "Proxy...<const Transistor::Type::Code*>".

template<>
  inline string  ProxyTypeName<Transistor::Type::Code>
                              ( const Transistor::Type::Code* object )
                              { return "<PointerSlotAdapter<Transistor::Type::Code>>"; }

template<>
  inline string  ProxyString  <Transistor::Type::Code>
                              ( const Transistor::Type::Code* object )
                              {
                                switch ( *object ) {
                                  case Transistor::Type::LEFT :    return "LEFT";
                                  case Transistor::Type::SINGLE:   return "SINGLE";
                                  case Transistor::Type::RIGHT:    return "RIGHT";
                                  case Transistor::Type::INTERNAL: return "INTERNAL";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Transistor::Type::Code>
                              ( const Transistor::Type::Code* object )
                              {
                                Record* record = new Record(GetString(object));
                                record->Add(GetSlot("Code", (unsigned int*)object));
                                return record;
                              }


# endif

}

//END_NAMESPACE_HURRICANE


// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string GetString(const H::Transistor::MaskInfo&);


#endif // HURRICANE_TRANSISTOR
