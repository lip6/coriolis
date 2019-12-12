#!/usr/bin/python

import sys		
from   Hurricane        import *
from   CRL              import *
from   math             import sqrt, ceil
import helpers
from   helpers.io       import ErrorMessage as Error
from   helpers          import trace, u
import oroshi
#from   CapacitorUnit    import CapacitorUnit 

def toDbU    ( l ): return DbU.fromPhysical( l, DbU.UnitPowerMicro )
def toPhY    ( l ): return DbU.toPhysical  ( l, DbU.UnitPowerMicro )

def doBreak( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()

helpers.staticInitialization( True )




class Resistor():


    rules = oroshi.getRules()

    def __init__( self, device, nets, resistorType, resistance = 0, resDim = { "width" : 0.8, "length" : 0 }, direction = "horizontal", snakeMode = False, bends = 0 ):

        self.device                = device
        self.nets                  = nets

        self.__initArgs__ ( resistorType, direction, snakeMode, bends )
        
        [ resDim["width" ], resDim["length"] ] = [ toDbU(resDim["width" ]), toDbU(resDim["length"]) ]
        self.__initResDim__( resistance, resDim ) 

        if self.snakeMode : 
            self.bendDim                    = self.__computeBendDim__( self )
            self.snakeSegmentDict           = {}

        self.resistorPlateDict              = {}
        self.abutmentBoxDict                = { "XMin" : 0, "YMin" : 0 }
        self.abutmentBox                    =    Box ()
        self.terminal1Box                   =    Box()
        self.terminal2Box                   =    Box()

        self.contactsNumber                 = 0 
        self.resPlateExtensions_minWidth    = 0 
        self.cutsEnclosure                  = {}

 
        self.t1CutsDict                     = {}
        self.t2CutsDict                     = {}
            
        if self.resistorType == "RPOLYH" : 
            self.pImplant_layerDict         = {}
            self.hresLayerDict              = {}
        if self.resistorType == "RPOLY2PH" :
            self.restrmLayerDict            = {}

        self.enclosure_resistor_abutmentBox = {}
        self.resPlateExtensions             = {}
        self.resPlateExtrimities            = {}
        return



    def __initArgs__( self, resistorType, direction, snakeMode, bends ):

        if resistorType == "RPOLYH" or resistorType == "RPOLY2PH" : self.resistorType = resistorType
        else                                                      : raise Error(1,'__initArgs__() : Unsupported resistor type : %s.' % resistorType)

        if direction in ["horizontal", "vertical"]                : self.direction = direction
        else                                                      : raise Error(1,'__initDirection__() : Direction must be either "horizontal" or "vertical".')

        if snakeMode in [True, False]                             : 
            if snakeMode == False and bends == 0 or snakeMode == False and bends > 0 : self.snakeMode = snakeMode
            else                                                                     : raise Error(1,'__initDirection__() : Please check compatibility between snake mode, %s, and bends number, %s.' % (snakeMode, bends))
        else                                                      : raise Error(1,'__initDirection__() : Snake mode must be either "True" or "False".')
        
        return



    def __initResDim__( self, resistance, resDim ):

        if self.__isDimOk__( resDim, "width" ) :

            if   resistance > 0 and resDim["length"] > 0  :
            
                if  self.__computeResistance__( resDim, self.resistorType ) == resistance and self.__isDimOk__( resDim, "length" ): 
                    self.resDim = resDim
                else : 
                    raise Error(1,'__initResDim__() : Non compatibiliy between resistance value, %s, and the given resistor dimensions, %s, or invalid length, %s.' %(resistance, resDim))

            elif resistance > 0 and resDim["length"] == 0 : 
                self.resDim = self.__computeResDim__( resistance, self.resistorType, resDim )

            elif resistance == 0 and resDim["width"] > 0 and resDim["length"] > 0 and self.__isDimOk__( resDim, "length" ): 
                self.resDim = resDim

            else : raise Error(1, '__initResDim__() : Invalid input parameters : resistance and/or resistor dimensions (%s, %s). Please provide at least one valid parameter.' %(resistance, resDim))

        return



    def __isDimOk__( self, resdim, side ) :

        if resdim.keys() == ["width","length"] and side in ["width","length"]:
            if side == ["width"]: rule = Resistor.rules.minWidth_rpolyh 
            else                : rule = Resistor.rules.minWidth_rpolyh   # Resistor.rules.minLength_rpolyh if self.resistorType == "RPOLYH" else Resistor.rules.minLength_rpolyh  

            state = True if resdim[side] >= rule else False   
            if state == False : raise Error(1,'__isDimOk__() : Resistor or bend side dimension exceeds the minimal limit : (side dimension, minimal limit) = (%s ,%s).' % (resdim[side], rule))

        else : raise Error(1,'__isDimOk__() : The resistor or bend dimensions and the specified side, %s and %s, must be either "width" or "length".' %(resdim,side))

        return state



    def __computeResistance__( self, resDim, resistorType ): return self.__getResistorMaterial__( resistorType, "resistorPlate" )*toPhY(resDim["length"])*toPhY(resDim["width"])

         

    def __computeResDim__( self, resistance, resistorType, resDim ):

        wl_ratio         = resistance/self.__getResistorMaterial__( resistorType, "resistorPlate" )
        width            = toPhY(resDim["width"])
        length           = wl_ratio*width
        resDim["length"] = toDbU(length)
        print('length',length)
        if self.__isDimOk__(resDim, "width") and  self.__isDimOk__(resDim, "length") : 
            return resDim 

                                                



    def __getResistorMaterial__( self, resistorType, material ):

        if material == 'resistorPlate':
            if     resistorType == "RPOLYH"  : resistance = Resistor.rules.RPOLYHSheetRes
            elif   resistorType == "RPOLY2PH": resistance = Resistor.rules.RPOLY2PHSheetRes
            else : raise Error( 1, '__getResistorMaterial__() : Unsupported resistor type : %s.' % resistorType ) 

        elif material == "contact" :
            resistance = Resistor.rules.MET1RPOLYHContRes

        return resistance



    def __computeBendDim__( self ):

        length = 0
        if self.__isDimOk__(self.bendDim, "length") : bendDim = { "width" : self.resDim["width"], "length" : toDbU(length) } 

        return bendDim


    def setRules( self ):

        self.__setattr__    ( "minSpacing_resistorPlate"                 ,  Resistor.rules.minSpacing_rpolyh               )
        self.__setattr__    ( "minWidth_contact"                         ,  Resistor.rules.minWidth_cut0                   )
        self.__setattr__    ( "minHeight_contact"                        ,  Resistor.rules.minWidth_cut0                   )
        self.__setattr__    ( "minSpacing_contact"                       ,  Resistor.rules.minSpacing_cut0                 )
        self.__setattr__    ( "minEnclosure_resistorPlate_contact"       ,  Resistor.rules.minEnclosure_poly2_cut0         )

        if  self.resistorType == "RPOLYH": 
            self.__setattr__( "minWidth_resistorPlate"                   ,  Resistor.rules.minWidth_rpolyh                 )
            self.__setattr__( "minWidth_pImplantLayer"                   ,  Resistor.rules.minWidth_pImplant               )
            self.__setattr__( "minSpacing_pImplantLayer"                 ,  Resistor.rules.minSpacing_pImplant             )
            self.__setattr__( "minSpacing_resistorPlate_pImplantLayer"   ,  Resistor.rules.minSpacing_rpolyh_pImplant      )
            self.__setattr__( "minEnclosure_pImplantLayer_contact"       ,  Resistor.rules.minEnclosure_pImplant_poly2con  )
            self.__setattr__( "minEnclosure_hres_poly2"                  ,  Resistor.rules.minEnclosure_hres_poly2         )
            self.__setattr__( "minWidth_hres"                            ,  Resistor.rules.minWidth_hres                   )
            self.__setattr__( "minSpacing_hres"                          ,  Resistor.rules.minSpacing_hres                 )
            self.__setattr__( "minSpacing_hres_poly1"                    ,  Resistor.rules.minSpacing_hres_poly1           )
            self.__setattr__( "minSpacing_hres_poly2"                    ,  Resistor.rules.minSpacing_hres_poly2           )
            self.__setattr__( "minSpacing_hres_active"                   ,  Resistor.rules.minSpacing_hres_active          )
#            self.__setattr__( "minLength_resistorPlate"         ,  Resistor.rules.minLength_rpolyh              )

        elif self.resistorType == "RPOLY2PH": 
            self.__setattr__( "minWidth_resistorPlate"                   ,  Resistor.rules.minWidth_rpolyh                 )
            
#            self.__setattr__( "minLength_resistorPlate"         ,  Resistor.rules.minLength_rpoly2ph            )

        else : raise Error( 1, 'setRules() : Unsupported resistor type : %s.' % resistorType )

        return


    def __setattr__( self, attribute, value ):
 
        self.__dict__[attribute] = value

        return


    def getLayers( self ):

        technology                 = DataBase.getDB().getTechnology()
        layerDict                  = {}

        layerDict["resistorPlate"] =   technology.getLayer( "poly2"     )
        layerDict["contacts"     ] =   technology.getLayer( "cut0"      )
        layerDict["pImplant"     ] =   technology.getLayer( "pImplant"  )
        layerDict["resdef"       ] =   technology.getLayer( "resdef"    )

        if self.resistorType == "RPOLYH"   : layerDict["hres"  ] =  technology.getLayer( "hres"   )
        if self.resistorType == "RPOLY2PH" : layerDict["restrm"] =  technology.getLayer( "restrm" )
 
	return layerDict




    def create( self ):

        UpdateSession.open()           

        self.setRules()
        layerDict = self.getLayers()
	self.computeDimensions()
        self.drawAbutmentBox  ()
        self.device.setAbutmentBox( self.abutmentBox )
        if self.resistorType == "RPOLYH" : self.drawHRESLayer( layerDict["hres"] )
        
        if self.snakeMode == False :
            self.drawRectangularResistorPlate( layerDict["resistorPlate"] ) 
            self.drawResPlateExtrimities     ( layerDict["resistorPlate"], layerDict["pImplant"] )
#        elif : 
 #           pass
        self.drawContacts( layerDict["contacts"])
        if self.resistorType == "RPOLY2PH" : self.drawRestrmLayer(layerDict["restrm"])

        self.drawResDefLayer(layerDict["resdef"])

        UpdateSession.close()

        return



    def computeDimensions( self ):

        self.computeAbutmentBoxDimensions2()

        if self.snakeMode == False :
            self.computeResistorPlateDimensions()

  #      else : self.computeBendDimensions()

        return



    def computePImplantLayerDim ( self ):

        pImplantLayer_width              = max(self.minWidth_pImplantLayer, 2*self.minEnclosure_pImplantLayer_contact + self.minWidth_contact)
#        self.pImplant_layerDict["width"] = pImplantLayer_width if self.resDim["width"] < pImplantLayer_width else self.contactsNumber*self.minWidth_contact + (self.contactsNumber-1)*self.minSpacing_contact + 2*self.minEnclosure_pImplantLayer_contact
        self.pImplant_layerDict["width" ] = self.contactsNumber*self.minWidth_contact + (self.contactsNumber-1)*self.minSpacing_contact + 2*self.minEnclosure_pImplantLayer_contact
        self.pImplant_layerDict["length"] = pImplantLayer_width
 
        return



    def computeResPlateExtensionsDim( self ):

        self.resPlateExtensions_minWidth  = self.minWidth_contact + 2*self.minEnclosure_resistorPlate_contact
        print("self.resDim['width']",self.resDim['width'])
        print("self.resPlateExtensions_minWidth",self.resPlateExtensions_minWidth)
        self.resPlateExtensions["width"]  = self.resPlateExtensions_minWidth if self.resDim["width"] < self.resPlateExtensions_minWidth else self.resDim["width"]
        
        if self.resistorType == "RPOLYH"   : self.resPlateExtensions["length"] = self.minWidth_contact + self.minEnclosure_resistorPlate_contact + self.minEnclosure_pImplantLayer_contact 
        if self.resistorType == "RPOLY2PH" : self.resPlateExtensions["length"] = self.minWidth_contact + self.minEnclosure_resistorPlate_contact 

        return
 



    def computeResPlateExtrimitiesDim( self ):

        if self.resistorType == "RPOLYH"   :
            self.resPlateExtrimities["width" ] = self.pImplant_layerDict["width" ] + 2*self.enclosure_resistor_hres
            self.resPlateExtrimities["length"] = self.pImplant_layerDict["length"] +   self.enclosure_resistor_hres # self.hresLayerDict #self.pImplant_layerDict

        if self.resistorType == "RPOLY2PH" : 
            self.resPlateExtrimities = self.resPlateExtensions
        print("self.resPlateExtrimities['length']ExtrimDim",toPhY(self.resPlateExtrimities["length"]))

        return


    def computeAbutmentBoxDimensions( self ):

        if self.resistorType == "RPOLYH"   :  self.enclosure_resistor_abutmentBox = max(self.minSpacing_hres, self.minSpacing_hres_poly1, self.minSpacing_hres_poly2, self.minSpacing_hres_active) #max(self.minSpacing_resistorPlate, self.minSpacing_pImplantLayer, self.minSpacing_resistorPlate_pImplantLayer) 
        if self.resistorType == "RPOLY2PH" :  self.enclosure_resistor_abutmentBox = self.minSpacing_resistorPlate

        if self.snakeMode == False:

            self.computeResPlateExtensionsDim()
            self.contactsNumber =   self.cutMaxNumber( self.resPlateExtensions["width"], self.minHeight_contact, self.minSpacing_contact, self.minEnclosure_resistorPlate_contact )

            if self.resistorType == "RPOLYH" : 
                self.computePImplantLayerDim()

            self.computeResPlateExtrimitiesDim()

            abutmentBoxSide1 = self.resPlateExtrimities["width"] + 2*self.enclosure_resistor_abutmentBox 
            abutmentBoxSide2 = self.resDim["length"] + 2*self.resPlateExtrimities["length"] + 2*self.enclosure_resistor_abutmentBox

            if self.direction == "vertical"   :
                self.abutmentBoxDict["width" ] = abutmentBoxSide1
                self.abutmentBoxDict["height"] = abutmentBoxSide2 
                
            if self.direction == "horizontal" :
                self.abutmentBoxDict["width" ] = abutmentBoxSide2
                self.abutmentBoxDict["height"] = abutmentBoxSide1

        
        if self.snakeMode == True : print("hi")


        return


    def computeAbutmentBoxDimensions2( self ):

        if self.resistorType == "RPOLYH"   :  self.enclosure_resistor_abutmentBox = self.minSpacing_hres #max(self.minSpacing_resistorPlate, self.minSpacing_pImplantLayer, self.minSpacing_resistorPlate_pImplantLayer) 
        if self.resistorType == "RPOLY2PH" :  self.enclosure_resistor_abutmentBox = self.minSpacing_resistorPlate

        if self.snakeMode == False:

            self.computeResPlateExtensionsDim()
            self.contactsNumber =   self.cutMaxNumber( self.resPlateExtensions["width"], self.minHeight_contact, self.minSpacing_contact, self.minEnclosure_resistorPlate_contact )

            if self.resistorType == "RPOLYH" : 
                self.computePImplantLayerDim()
                self.computeHRESLayerDimensions()

            self.computeResPlateExtrimitiesDim()

            print("self.resPlateEtrimities['length']abtBoxDim2",toPhY(self.resPlateExtrimities['length']))
            
            abutmentBoxSide1 = self.resPlateExtrimities["width"] + 2*self.enclosure_resistor_abutmentBox
            print("self.resPlateExtrimities",self.resPlateExtrimities)
            abutmentBoxSide2 = self.resDim["length"] + 2*self.resPlateExtrimities["length"] + 2*self.enclosure_resistor_abutmentBox
            print("self.resDim['length']2",toPhY(self.resDim["length"]))
            print("self.enclosure_resistor_abutmentBox2",toPhY(self.enclosure_resistor_abutmentBox))
            print("self.resPlateEtrimities['length']2",toPhY(self.resPlateExtrimities['length']))
#            if self.resistorType == "RPOLYH"   : abutmentBoxSide2 = self.resPlateExtrimities["length"]+ 2*self.enclosure_resistor_abutmentBox
#            if self.resistorType == "RPOLY2PH" : abutmentBoxSide2 = self.resDim["length"] + 2*self.resPlateExtrimities["length"] + 2*self.enclosure_resistor_abutmentBox
            print("abutmentBoxSide2",toPhY(abutmentBoxSide2))
            if self.direction == "vertical"   :
                self.abutmentBoxDict["width" ] = abutmentBoxSide1
                self.abutmentBoxDict["height"] = abutmentBoxSide2 
                
            if self.direction == "horizontal" :
                self.abutmentBoxDict["width" ] = abutmentBoxSide2
                self.abutmentBoxDict["height"] = abutmentBoxSide1


        
        if self.snakeMode == True : print("hi")


        return



    def computeHRESLayerDimensions( self ):

        self.enclosure_resistor_hres = self.minEnclosure_hres_poly2
        hresLayerSide1 = max(self.minWidth_hres,   self.pImplant_layerDict["width" ] + 2*self.enclosure_resistor_hres)   
        #hresLayerSide2 = max(self.minWidth_hres, 2*self.pImplant_layerDict["length"] + 2*self.enclosure_resistor_hres + self.resDim["length"])  

        if self.direction == "vertical"   :
            #self.hresLayerDict["width" ] = hresLayerSide2
            self.hresLayerDict["length"] = hresLayerSide1 
                
        if self.direction == "horizontal" :
            self.hresLayerDict["width" ] = hresLayerSide1
            #self.hresLayerDict["length"] = hresLayerSide2


        return



    def computeResistorPlateDimensions( self ):

        self.resistorPlateDict["width"  ] = self.resDim["width" ]
        self.resistorPlateDict["length" ] = self.resDim["length"]

 	if self.direction == "vertical"      :
            self.resistorPlateDict["XCenter"] = self.abutmentBoxDict  ["XMin"] + self.abutmentBoxDict  ["width"]/2
            self.resistorPlateDict["YMin"   ] = self.abutmentBoxDict  ["YMin"] + self.enclosure_resistor_abutmentBox + self.resPlateExtrimities["length"]
            self.resistorPlateDict["YMax"   ] = self.resistorPlateDict["YMin"] + self.resistorPlateDict["length"]


 	if self.direction == "horizontal"    :
            print("self.abutmentBoxDict  ['XMin']",toPhY(self.abutmentBoxDict  ["XMin"]))
            print("self.enclosure_resistor_abutmentBox",toPhY(self.enclosure_resistor_abutmentBox))
            print("self.resPlateExtrimities['length']",toPhY(self.resPlateExtrimities["length"]
))
            self.resistorPlateDict["YCenter"] = self.abutmentBoxDict  ["YMin"] + self.abutmentBoxDict  ["height"]/2  
            self.resistorPlateDict["XMin"   ] = self.abutmentBoxDict  ["XMin"] + self.enclosure_resistor_abutmentBox + self.resPlateExtrimities["length"]
            print("self.resistorPlateDict['XMin'   ]",toPhY(self.resistorPlateDict["XMin"   ]))
            self.resistorPlateDict["XMax"   ] = self.resistorPlateDict["XMin"] + self.resistorPlateDict["length"]


        return


    def computeHRESLayerPosition( self ):

 	if self.direction == "vertical"      :
            self.hresLayerDict["XCenter"] = self.abutmentBox.getXCenter()
            self.hresLayerDict["YMin"   ] = self.abutmentBox.getYMin() + self.enclosure_resistor_abutmentBox
            self.hresLayerDict["YMax"   ] = self.abutmentBox.getYMax() - self.enclosure_resistor_abutmentBox

 	if self.direction == "horizontal"    :
            self.hresLayerDict["YCenter"] = self.abutmentBox.getYCenter()
            self.hresLayerDict["XMin"   ] = self.abutmentBox.getXMin() + self.enclosure_resistor_abutmentBox
            self.hresLayerDict["XMax"   ] = self.abutmentBox.getXMax() - self.enclosure_resistor_abutmentBox


        return


    def drawLayer( self, layer, positionParams ):

        if self.direction == "vertical"        : positionParamsKeys = ["XCenter","width","YMin","YMax"]
        if self.direction == "horizontal"      : positionParamsKeys = ["YCenter","width","XMin","XMax"]
        
        if type(positionParams) is dict        : 
            for key in positionParams.keys()   : 
                 if key not in positionParams  : raise Error(1,'drawLayer() : %s must contain all following keys : %s.' %(positionParams, positionParams))
        else : raise Error(1,'drawLayer() : %s must be a dictionary of %s keys.'  %(positionParams, positionParams) )

        if layer == 1: self.computeHRESLayerPosition()
        if layer == 1: self.computeResDefDimensions()

        if self.direction == "vertical"        : Vertical.create  ( self.nets[0], layer, positionParams["XCenter"], positionParams["width"], positionParams["YMin"], positionParams["YMax"] ) 
        if self.direction == "horizontal"      : Horizontal.create( self.nets[0], layer, positionParams["YCenter"], positionParams["width"], positionParams["XMin"], positionParams["XMax"] ) 

        return




    def drawHRESLayer( self, layer ):
        print("layer",layer)
        self.computeHRESLayerPosition()

        if self.direction == "vertical"    : Vertical.create   ( self.nets[0], layer, self.hresLayerDict["XCenter"], self.hresLayerDict["length"], self.hresLayerDict["YMin"], self.hresLayerDict["YMax"] ) 
        if self.direction == "horizontal"  : Horizontal.create ( self.nets[0], layer, self.hresLayerDict["YCenter"], self.hresLayerDict["width" ], self.hresLayerDict["XMin"], self.hresLayerDict["XMax"] ) 


        return



    def drawAbutmentBox( self ):

        self.abutmentBox = Box(self.abutmentBoxDict["XMin"], self.abutmentBoxDict["YMin"], self.abutmentBoxDict["width"], self.abutmentBoxDict["height"])

        return




    def drawRectangularResistorPlate( self, layer ):

        if self.direction == "vertical"   : Vertical.create  ( self.nets[0], layer, self.resistorPlateDict["XCenter"], self.resistorPlateDict["width"], self.resistorPlateDict["YMin"], self.resistorPlateDict["YMax"] ) 
        if self.direction == "horizontal" : Horizontal.create( self.nets[0], layer, self.resistorPlateDict["YCenter"], self.resistorPlateDict["width"], self.resistorPlateDict["XMin"], self.resistorPlateDict["XMax"] ) 

        return

    


    def drawResPlateExtrimities( self, resPlateLayer, pImplantLayer ):

        self.drawResPlateExtensions(resPlateLayer)
        if self.resistorType == "RPOLYH" : self.drawPImplantLayer(pImplantLayer)

        return


    def computeResPlateExtensionsPosition( self ):

        self.resPlateExtensions["ex1"] = {}

        if self.direction == "vertical"      :

            self.resPlateExtensions["ex1"]["XMin"] = self.abutmentBox.getXCenter()          - self.resPlateExtensions["width" ]/2
            self.resPlateExtensions["ex1"]["YMin"] = self.abutmentBox.getYCenter()          - self.resPlateExtensions["length"]   -  self.resDim["length"]/2  
            self.resPlateExtensions["ex1"]["XMax"] = self.resPlateExtensions["ex1"]["XMin"] + self.resPlateExtensions["width" ]
            self.resPlateExtensions["ex1"]["YMax"] = self.resPlateExtensions["ex1"]["YMin"] + self.resPlateExtensions["length"]
            
        if self.direction == "horizontal"    :

            self.resPlateExtensions["ex1"]["XMin"] = self.abutmentBox.getXCenter()          - self.resPlateExtensions["length"]   - self.resDim["length"]/2  
            self.resPlateExtensions["ex1"]["YMin"] = self.abutmentBox.getYCenter()          - self.resPlateExtensions["width" ]/2 
            self.resPlateExtensions["ex1"]["XMax"] = self.resPlateExtensions["ex1"]["XMin"] + self.resPlateExtensions["length"]
            self.resPlateExtensions["ex1"]["YMax"] = self.resPlateExtensions["ex1"]["YMin"] + self.resPlateExtensions["width" ]

        return



    def drawResPlateExtensions( self, layer ):
    
        self.computeResPlateExtensionsPosition()
        translation       = self.resDim["length"] + self.resPlateExtensions["length"]

        if self.direction == "vertical"   :

            self.terminal1Box = Box(self.resPlateExtensions["ex1"]["XMin"], self.resPlateExtensions["ex1"]["YMin"], self.resPlateExtensions["ex1"]["XMax"], self.resPlateExtensions["ex1"]["YMax"])
            self.terminal2Box = Box(self.resPlateExtensions["ex1"]["XMin"], self.resPlateExtensions["ex1"]["YMin"] + translation, self.resPlateExtensions["ex1"]["XMax"],  self.resPlateExtensions["ex1"]["YMax"] + translation)

        if self.direction == "horizontal" :

            self.terminal1Box = Box(self.resPlateExtensions["ex1"]["XMin"], self.resPlateExtensions["ex1"]["YMin"], self.resPlateExtensions["ex1"]["XMax"], self.resPlateExtensions["ex1"]["YMax"])
            self.terminal2Box = Box(self.resPlateExtensions["ex1"]["XMin"] + translation, self.resPlateExtensions["ex1"]["YMin"], self.resPlateExtensions["ex1"]["XMax"] + translation, self.resPlateExtensions["ex1"]["YMax"])

        terminal1Pad = Pad.create(self.nets[0], layer, self.terminal1Box)
        terminal2Pad = Pad.create(self.nets[1], layer, self.terminal2Box)

        return



    def computePImplantLayerPosition( self ):

        if self.direction == "vertical"    :
        
            self.pImplant_layerDict["YCenter"] = self.abutmentBoxDict   ["YMin"] + self.enclosure_resistor_abutmentBox + self.enclosure_resistor_hres + self.pImplant_layerDict["length"]/2
            self.pImplant_layerDict["XMin"   ] = self.abutmentBox.getXCenter() - self.pImplant_layerDict["width"]/2
            self.pImplant_layerDict["XMax"   ] = self.pImplant_layerDict["XMin"] + self.pImplant_layerDict["width"]

        if self.direction == "horizontal"    :

            self.pImplant_layerDict["XCenter"] = self.abutmentBoxDict   ["XMin"] + self.enclosure_resistor_abutmentBox + self.enclosure_resistor_hres + self.pImplant_layerDict["length"]/2 #self.minWidth_contact + self.minEnclosure_resistorPlate_contact)/2
            self.pImplant_layerDict["YMin"   ] = self.abutmentBox.getYCenter() - self.pImplant_layerDict["width"]/2 #self.abutmentBoxDict   ["YMin"] + self.enclosure_resistor_abutmentBox
            self.pImplant_layerDict["YMax"   ] = self.pImplant_layerDict["YMin"] + self.pImplant_layerDict["width"]


        return


    def drawPImplantLayer( self, layer ):

        self.computePImplantLayerPosition()
        translation = self.resDim["length"] + self.pImplant_layerDict["length"]

        if self.direction == "vertical"   : 
            Horizontal.create( self.nets[0], layer, self.pImplant_layerDict["YCenter"]              , self.pImplant_layerDict["length"], self.pImplant_layerDict["XMin"], self.pImplant_layerDict["XMax"] ) 
            Horizontal.create( self.nets[0], layer, self.pImplant_layerDict["YCenter"] + translation, self.pImplant_layerDict["length"], self.pImplant_layerDict["XMin"], self.pImplant_layerDict["XMax"] )

        if self.direction == "horizontal" :

            Vertical.create  ( self.nets[0], layer, self.pImplant_layerDict["XCenter"]              , self.pImplant_layerDict["length"], self.pImplant_layerDict["YMin"], self.pImplant_layerDict["YMax"] ) 
            Vertical.create  ( self.nets[1], layer, self.pImplant_layerDict["XCenter"] + translation, self.pImplant_layerDict["length"], self.pImplant_layerDict["YMin"], self.pImplant_layerDict["YMax"] )

        return


    def computeCutsDimensions( self ):

        if self.direction == "vertical":
            self.cutsEnclosure ["horizontal"] = ( self.resPlateExtensions["width"] - (self.contactsNumber*self.minHeight_contact + (self.contactsNumber -1)*self.minSpacing_contact) )/2
            self.cutsEnclosure ["vertical"  ] = self.minEnclosure_resistorPlate_contact

        if self.direction == "horizontal" :
            self.cutsEnclosure["horizontal"]  = self.minEnclosure_resistorPlate_contact
            self.cutsEnclosure["vertical"  ]  = ( self.resPlateExtensions["width"] - (self.contactsNumber*self.minHeight_contact + (self.contactsNumber -1)*self.minSpacing_contact) )/2

        self.t1CutsDict["XCenter"] = self.terminal1Box.getXMin() + self.cutsEnclosure ["horizontal"] + self.minWidth_contact/2
        self.t1CutsDict["YCenter"] = self.terminal1Box.getYMin() + self.cutsEnclosure ["vertical"  ] + self.minHeight_contact/2

        if self.resistorType == "RPOLYH"   : translation = self.resDim["length"] + self.minWidth_contact + 2*self.minEnclosure_pImplantLayer_contact
        if self.resistorType == "RPOLY2PH" : translation = self.resDim["length"] + self.minWidth_contact 

        if self.direction == "vertical":
            self.t2CutsDict["XCenter"] = self.t1CutsDict["XCenter"] 
            self.t2CutsDict["YCenter"] = self.t1CutsDict["YCenter"] + translation  

        if self.direction == "horizontal" :
            self.t2CutsDict["XCenter"] = self.t1CutsDict["XCenter"] + translation
            self.t2CutsDict["YCenter"] = self.t1CutsDict["YCenter"]

        return



    def drawContacts( self, layer ):

        self.computeCutsDimensions()

        if self.direction == "vertical"   : cutLineDirection = "horizontal"                  
        if self.direction == "horizontal" : cutLineDirection = "vertical"
        self.cutLine( self.nets[0], layer, self.t1CutsDict["XCenter"], self.t1CutsDict["YCenter"], self.minWidth_contact, self.minHeight_contact, self.minSpacing_contact, self.contactsNumber , cutLineDirection )
        self.cutLine( self.nets[1], layer, self.t2CutsDict["XCenter"], self.t2CutsDict["YCenter"], self.minWidth_contact, self.minHeight_contact, self.minSpacing_contact, self.contactsNumber , cutLineDirection )

        return



    def drawRestrmLayer( self, layer ):

        self.computeRestrmLayerPosition()  
        translation = self.resDim["length"] + self.restrmLayerDict["width"]

        if self.direction == "vertical"   : 
            Horizontal.create( self.nets[0], layer, self.restrmLayerDict["YCenter"]            , self.restrmLayerDict["width"], self.restrmLayerDict["XMin"], self.restrmLayerDict["XMax"] ) 
            Horizontal.create( self.nets[1], layer, self.restrmLayerDict["YCenter"]+translation, self.restrmLayerDict["width"], self.restrmLayerDict["XMin"], self.restrmLayerDict["XMax"] ) 
        if self.direction == "horizontal" : 
            Vertical.create  ( self.nets[0], layer, self.restrmLayerDict["XCenter"]            , self.restrmLayerDict["width"], self.restrmLayerDict["YMin"], self.restrmLayerDict["YMax"] ) 
            Vertical.create  ( self.nets[1], layer, self.restrmLayerDict["XCenter"]+translation, self.restrmLayerDict["width"], self.restrmLayerDict["YMin"], self.restrmLayerDict["YMax"] ) 


        return



    def computeRestrmLayerPosition( self ):

        self.restrmLayerDict["width"]       = self.minWidth_contact/2

        if self.direction == "vertical"   :
            self.restrmLayerDict["YCenter"] = self.t1CutsDict["YCenter"]             + self.restrmLayerDict["width"]/2
            self.restrmLayerDict["XMin"   ] = self.resPlateExtensions["ex1"]["XMin"] - self.restrmLayerDict["width"]
            self.restrmLayerDict["XMax"   ] = self.resPlateExtensions["ex1"]["XMax"] + self.restrmLayerDict["width"]

        if self.direction == "horizontal" : 
            self.restrmLayerDict["XCenter"] = self.t1CutsDict["XCenter"]             + self.restrmLayerDict["width"]/2
            self.restrmLayerDict["YMin"   ] = self.resPlateExtensions["ex1"]["YMin"] - self.restrmLayerDict["width"]
            self.restrmLayerDict["YMax"   ] = self.resPlateExtensions["ex1"]["YMax"] + self.restrmLayerDict["width"]


        return



    def computeResDefDimensions( self ):

        self.resdefDict = self.resistorPlateDict
        if self.resistorType == "RPOLY2PH" :

            if self.direction == "vertical"   : keysList = ["YMin","YMax","width"]
            if self.direction == "horizontal" : keysList = ["XMin","XMax","width"]

            self.resdefDict[keysList[0]]   = self.resdefDict[keysList[0]] - self.minWidth_contact/4
            self.resdefDict[keysList[1]]   = self.resdefDict[keysList[1]] + self.minWidth_contact/4

            if self.resDim["width"] > self.resPlateExtensions_minWidth :
                self.resdefDict[keysList[2]]   = self.resdefDict[keysList[2]] + self.minWidth_contact/2

        return



    def drawResDefLayer( self, layer ):
    
        self.computeResDefDimensions()
        if self.direction == "vertical"    : Vertical.create   ( self.nets[0], layer, self.resdefDict["XCenter"], self.resdefDict["width"], self.resdefDict["YMin"], self.resdefDict["YMax"] ) 
        if self.direction == "horizontal"  : Horizontal.create ( self.nets[0], layer, self.resdefDict["YCenter"], self.resdefDict["width"], self.resdefDict["XMin"], self.resdefDict["XMax"] ) 

        return



    ## Creates a horizontal or vertical line of contacts according to the specified direction.

    def cutLine( self, net, layer, firstCutXCenter, firstCutYCenter, width_cut, height_cut, spacing_cut, cutNumber, direction ):       
 
        for i in range(0, cutNumber) :
            segment = Contact.create( net, layer, firstCutXCenter + i*(width_cut + spacing_cut), firstCutYCenter, width_cut, height_cut ) if direction == 'horizontal' else Contact.create( net, layer, firstCutXCenter, firstCutYCenter + i*(height_cut + spacing_cut), width_cut, height_cut )

        return segment



    ## Computes the maximal number of cuts to be placed on a layer of width \c width_layer considering specifications such as the spacing between the cuts, its width and its enclosure in the layer.

    def cutMaxNumber( self, width_layer, width_cut, spacing_cut, enclosure_cut ): 

        cutNumber = int( (width_layer - 2*enclosure_cut + spacing_cut) / (width_cut + spacing_cut) )
        if cutNumber > 0 : 
 
            return cutNumber
        else : raise Error (1,"cutMaxNumber() : Zero number of cuts found. Layer width is too tight." )

















def ScriptMain( **kw ):

    editor = None
    if kw.has_key('editor') and kw['editor']:
        editor = kw['editor']

    UpdateSession.open()
    device = AllianceFramework.get().createCell( 'resistor' )
    device.setTerminal( True )

    t_1 = Net.create( device, 't1' )
    t_1.setExternal( True )  
    t1 = device.getNet("t1")
    doBreak( 1, 'Done building terminal 1 net')

    t_2 = Net.create( device, 't2' )
    t_2.setExternal( True )  
    t2 = device.getNet("t2")
    doBreak( 1, 'Done building terminal 1 net')

    if editor:
        UpdateSession.close(        )
        editor.setCell     ( device ) 
        editor.fit         (        )            
        UpdateSession.open (        )     
    nets = [t1,t2]                  
    #(  device, resistorType, resistance = 0, resDim = { "width" : 10, "length" : 0 }, direction = "vertical", snakeMode = False, bends = 0 ):
    #resistor = Resistor( device, nets, "RPOLY2PH", 50, direction = "vertical" )
    resistor = Resistor( device, nets, "RPOLY2PH", 50, direction = "horizontal" )   #l = 0.8 dogBone
#    resistor = Resistor( device, nets, "RPOLYH", 2000, direction = "horizontal" )   #RPOLYH normal
#    resistor = Resistor( device, nets, "RPOLY2PH", 100, direction = "horizontal" )  #RPOLY2PH normal
#    resistor = Resistor( device, nets, "RPOLY2PH", 25, direction = "vertical" )      #L=w/2
    resistor.create()          
                      
    AllianceFramework.get().saveCell( device, Catalog.State.Views )

    return True

