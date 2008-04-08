// ****************************************************************************************************
// File: TrMos.h
// Authors: Wu YiFei
// Date   : 21/12/2006 
// ****************************************************************************************************

#ifndef DEVICE_TRMOS
#define DEVICE_TRMOS

#include "Net.h"
using namespace Hurricane;

#include "Device.h"
#include "Transistor.h"
#include "MetaTransistor.h"


namespace DEVICE {


class TrMos : public Device {
// **************************

#   if !defined(__DOXYGEN_PROCESSOR__)
// Types
// *****
    public : typedef Device Inherit; 

    public : enum PinName { D, G, S, B };

// Attributes
// *******************

    // Structural parameter.
    // ********************
    private  : Transistor::Polarity _polarity;
    private  : bool _isBsConnected;
    private  : unsigned _m;

    // Parameter of the electric synthesis. 
    // ***********************************
    /* to do */
	       
    // Physical parameter of realization. 
    // **********************************
    /* Placement. */
    private  : bool _sourceIsFirst;
    private  : bool _hasDummy;
    private  : bool _hasRing;

    /* Routing. */
    private  : vector<PinName> _lowPinOrder;     // relative position of the connectors on the basis of the top.
    private  : vector<PinName> _highPinOrder;

    private  : map<Net*, Pin*> _mapNetToPinBoxInLeftSide;
    private  : map<Net*, Pin*> _mapNetToPinBoxInRightSide;

    private  : double _widthOfSourceWire;  // by defect,  minWidth, unit of valeur is Micro
    private  : double _widthOfDrainWire;   // by defect,  minWidth, unit of valeur is Micro

    /* Others */
    private  : MetaTransistor * _tr1;
    private  : double _capaRouting;
    private  : list<Transistor*> _transistorList;


// Constructors
// ************
    protected : TrMos(Library* library, const Name& name);
    protected : virtual void _postCreate();
#endif

    public    : static TrMos* create(Library* library, const Name & name); 


#   if !defined(__DOXYGEN_PROCESSOR__)

// Operations
// **********
    public : virtual void dses()   { /* to do */}; 
    public : virtual void shape()  { /* to do */};
#endif


    public : void create(const Transistor::Polarity& polarity, const bool isbsconnected);
    public : void generate(const unsigned m, const bool sourceisfirst, const bool hasring,
		 const unsigned nbsourcecolumn, const unsigned nbdraincolumn);

// Accessors
// *********	    
    public : const Transistor::Polarity& getPolarity() const { return _polarity; };  
    public : unsigned getM() const { return _m; };	     
    public : const double getWidthOfSourceWire() const { return _widthOfSourceWire; };	     
    public : const double getWidthOfDrainWire() const { return _widthOfDrainWire; };	     
    public : MetaTransistor* getTr1() const { return _tr1; };	     
    public : Transistors getTransistors() const ;

// Updators
// ********
    public : void setMosLength(const double length) { if(_tr1) _tr1->setLe(length); }
    public : void setMosWidth(const double width) { if(_tr1) _tr1->setWe(width); }
    public : void setWidthOfSourceWire(const double width)  { _widthOfSourceWire = width; };	     
    public : void setWidthOfDrainWire(const double width) { _widthOfDrainWire=width; };	     
    public : void setLowPinOrder(const PinName, const PinName) ;	     
    public : void setHighPinOrder(const PinName, const PinName) ;	     

// Predicats
// *********
    public : bool isBsConnected() const { return _isBsConnected; };
    public : bool sourceIsFirst() const { return _sourceIsFirst; };
    public : bool hasRing() const { return _hasRing; };
 

#   if !defined(__DOXYGEN_PROCESSOR__)

// Others
// ******
    public: virtual string _getTypeName() const {return _TName("TrMos"); };
    public: virtual string _getString() const;
    public: virtual Record* _getRecord() const;

    public: vector<PinName>& getLowPinOrder() { return _lowPinOrder; };     
    public: vector<PinName>& getHighPinOrder() { return _highPinOrder; };

    public: map<Net*, Pin*>& getMapNetToPinBoxInLeftSide() { return _mapNetToPinBoxInLeftSide; };
    public: map<Net*, Pin*>& getMapNetToPinBoxInRightSide() { return _mapNetToPinBoxInRightSide; };
     
    public : virtual void _Flush();
    protected : void _PlaceAndRoute();	    
# endif

} ;

} // end of namespace Device 


#endif
