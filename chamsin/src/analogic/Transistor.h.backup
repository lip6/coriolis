// ****************************************************************************************************
// File: Transistor.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef HURRICANE_TRANSISTOR
#define HURRICANE_TRANSISTOR

#include "Cell.h"
using namespace Hurricane;

#include "Transistors.h"

namespace Hurricane {

class GenTrans;



class Transistor : public Cell {

//#   if !defined(__DOXYGEN_PROCESSOR__)

// Types
// *****

     public: typedef Cell Inherit; 

     public: class Polarity {
                 public: enum Code {N=0, P=1}; 

		 private: Code _code;

		 public : Polarity(const Code& code=N);
		 public : Polarity(const Polarity&);
		 public : Polarity& operator=(const Polarity&);

		 public : operator const Code& () const { return _code; };
		 public : const Code& getCode() const { return _code; }; 

		 public : string _getTypeName() const { return _TName("Transistor::Polarity"); }; 
		 public : string _getString() const; 
		 public : Record* _getRecord() const;
             };



     public : class MaskVersion {
                public : enum Code { VERSION1=0 };

		private: Code _code;

		public : explicit MaskVersion(const Code& code=VERSION1);
		public : MaskVersion(const MaskVersion&);
		public : MaskVersion& operator=(const MaskVersion&);

		public : operator const Code& () const { return _code; };
		public : const Code& getCode() const { return _code; }; 

		public : string _getTypeName() const { return _TName("Transistor::MaskVersion"); }; 
		public : string _getString() const; 
		public : Record* _getRecord() const;
		
	      };


     public  : class Type {
	      // *********
                  
		public : enum Code { INTERNAL=0, LEFT=1, RIGHT=2, SINGLE=3};

		private: Code _code;
			 
		public : explicit Type(const Code& code=INTERNAL);
		public : Type(const Type& type);
		public : Type& operator=(const Type& type);
		public : operator const Code&() const { return _code; };
			 
                public : void setCode(const Code& code) { _code = code; }; 

		public : const Code& getCode() const { return _code; };

		public : string _getTypeName() const { return _TName("Transistor::Type"); }; 
		public : string _getString() const;
		public : Record* _getRecord() const;
	
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

			   
                 // Destructors
		 // ***********
                 public: virtual ~MaskInfo() {};

                 // Accessors
	         // *********
		 public : const double& getL() const { return _l; };
		 public : const double& getW() const { return _w; };
		 public : const unsigned & getNbDrainColumn() const { return _nbDrainColumn; };
		 public : const unsigned & getNbSourceColumn() const { return _nbSourceColumn; };
		 public : const Type& getType() const { return _type; };

                 // Update
		 // ******
		 public : void setL(const double& l) { _l=l;}; 
		 public : void setW(const double& w) { _w=w;}; 
		 public : void setNbDrainColumn(const unsigned& column) { _nbDrainColumn=column; };
		 public : void setNbSourceColumn(const unsigned& column) { _nbSourceColumn=column; };
		 public : void setType(const Type::Code& code) { _type.setCode(code); };
		 public : void setType(const Type& type) { _type = type; };

		 // Predicats
	         // *********

		 // Operators
	         // *********
		 public : virtual bool operator==(const MaskInfo&);

                 // Others
		 // ******
		 public : virtual string _getTypeName() const =0;
		 public : virtual string _getString() const;
		 public : virtual Record* _getRecord() const;
	
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

	         public : MaskInfo& operator=(const MaskInfo&);


	         // Destructor
		 // ***********
		 public : virtual ~MaskV1Info() {};

		 // Operators
		 // *********
		 public : bool operator==(const MaskInfo&); 

		 // Others
	         // *********
		 public : virtual string _getTypeName() const { return _TName("Transistor::MaskV1Info"); }; 
		 public : virtual string _getString() const;
		 public : virtual Record* _getRecord() const;
              };


#   if !defined(__DOXYGEN_PROCESSOR__)

// Attributes
// *******************
    private : Polarity _polarity; 
    private : MaskInfo* _masqueInfo;	      
    private : GenTrans * _genTrans;	  
    //public : RealInfo * _realInfo;	      

    private : map<Net*, Box> _mapNet2Box;   // This Map Is For localize The Position Of Routing.

# endif 

// Constructors
// ************
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : Transistor(Library* library, const Name& name, const Polarity& polarity);
# endif 

    public : static Transistor* create(Library* library, const Name& name, const Polarity& polarity);		
   
#   if !defined(__DOXYGEN_PROCESSOR__)
    protected : virtual void _postCreate();


// Destructors
// ***********
    protected : ~Transistor() {};
    protected : virtual void _preDestroy();
# endif 

// Accessors
// *********	  
    public : const Polarity& getPolarity() const { return _polarity; };
    public : MaskVersion getMaskVersion() const { return _getMaskInfoVersion(_masqueInfo); };
    public : const MaskInfo* getMaskInfo() const { return _masqueInfo; };
    public : const double getL() const { return _masqueInfo->getL(); };
    public : const double getW() const { return _masqueInfo->getW(); };
    public : const unsigned& getNbDrainColumn() const { return _masqueInfo->getNbDrainColumn(); };
    public : const unsigned& getNbSourceColumn() const { return _masqueInfo->getNbSourceColumn(); };
    public : const char* getDrainName() const  { return "DRAIN"; };
    public : const char* getSourceName() const { return "SOURCE"; };
    public : const char* getGridName() const   { return "GRID"; };
    public : Net* getDrain() const  { return getNet(getDrainName()); };
    public : Net* getSource() const { return getNet(getSourceName()); };
    public : Net* getGrid() const   { return getNet(getGridName()); };
    public : const Type& getAbutmentType() const { return _masqueInfo->getType(); };

// Predicats
// *********
    public : bool isNmos() const { return _polarity==Polarity::N; };
    public : bool isPmos() const { return _polarity==Polarity::P; };
    public : bool isInternal() const { return getAbutmentType().getCode()==Type::INTERNAL; };
    public : bool isLeft() const     { return getAbutmentType().getCode()==Type::LEFT; };
    public : bool isRight() const    { return getAbutmentType().getCode()==Type::RIGHT; };
    public : bool isSingle() const   { return getAbutmentType().getCode()==Type::SINGLE; };

// Updators
// ********	    
    public : void setL(const double& l) { _masqueInfo->setL(l); };
    public : void setW(const double& w) { _masqueInfo->setW(w); };

//# endif

#   if !defined(__DOXYGEN_PROCESSOR__)
// Others
// ******
    public : virtual string _getTypeName() const {return _TName("Transistor");};
    public : virtual string _getString() const;
    public : virtual Record* _getRecord() const;
    public : const GenTrans* _getGenTrans() const {return _genTrans; }; 
    public : static MaskVersion _getMaskInfoVersion(MaskInfo*) ;
    public : static MaskInfo* _createMaskInfo(const MaskVersion&) ;
    public : map<Net*, Box>* _getMapNet2Box() { return &_mapNet2Box; };

# endif

// Operators
// *********
   public : void setMaskInfo(MaskInfo*);
   public : void createLayout();
   public : void duplicateLayout(Transistor* transistor) ;


}; 
#if !defined(__DOXYGEN_PROCESSOR__)

// -------------------------------------------------------------------
// Class  :  "Proxy...<const Transistor::Polarity::Code*>".

template<>
  inline string  ProxyTypeName<Transistor::Polarity::Code>
                              ( const Transistor::Polarity::Code* object )
                              { return "<PointerSlotAdapter<Transistor::Polarity::Code>>"; }

template<>
  inline string  ProxyString  <Transistor::Polarity::Code>
                              ( const Transistor::Polarity::Code* object )
                              {
                            switch ( *object ) {
                                  case Transistor::Polarity::N: return "N";
                                  case Transistor::Polarity::P: return "P";
                                }
                                return "ABNORMAL";
                              }

template<>
  inline Record* ProxyRecord  <Transistor::Polarity::Code>
                              ( const Transistor::Polarity::Code* object )
                          {
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
                                return record;
                          }





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
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
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
                                Record* record = new Record(getString(object));
                                record->add(getSlot("Code", (unsigned int*)object));
                                return record;
                              }


#endif

}
 
 

// ****************************************************************************************************
// Generic functions
// ****************************************************************************************************

string getString(const Hurricane::Transistor::MaskInfo&);




#endif // HURRICANE_TRANSISTOR
