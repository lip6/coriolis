#!/usr/bin/python

import sys        
from   math         import sqrt, ceil
from   ..Hurricane  import *
from   ..CRL        import *
from   ..           import Constant
from   ..helpers.io import ErrorMessage as Error
from   ..helpers    import trace, u
from   .            import getRules


def doBreak( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()

def toDbU    ( l ): return DbU.fromPhysical(     l , DbU.UnitPowerMicro )
def toPhy    ( l ): return DbU.toPhysical  ( int(l), DbU.UnitPowerMicro )

def toFoundryGrid ( l ):
    twoGrid = DbU.fromGrid( 2.0 )
    return l + (twoGrid - (l % twoGrid))

def isNearNorth ( y, ab ): return ab.getYMax() - y < y - ab.getYMin()


class Resistor ( object ):

    rules = getRules()

    def __init__( self, device, nets, resistorType, resistance = 0, resDim = { "width" : 10, "length" : 0 }, direction = "horizontal", bends = 0, shape = 90 ):

        self.device = device
        self.nets   = nets

        trace( 100, '\twidth:{0}, length:{1}, bends:{2}\n'.format(resDim['width'],resDim['length'],bends) )
        
       #resDim["width" ] = toDbU(resDim["width" ])
       #resDim["length"] = toDbU(resDim["length"]) 

        self.__initArgs__  ( resistorType, direction, bends, shape )
        self.setRules()
        self.__initResDim__( resistance  , resDim) 

        if self.snakeMode: 
            self.snakeSegmentDict           = {}
            self.resPlateExtrimity1         = {}
            self.resPlateExtrimity2         = {}
            self.resPlateExtension1         = {}
            self.resPlateExtension2         = {}
            self.snakeCornerDict            = {}
            if self.shape == 135 : 
                self.rightCorPointsVector   = []
                self.leftCorPointsVector    = []

        self.resPlateExtensions             = {}
        self.resPlateExtrimities            = {}

        self.resistorPlateDict              = {}
        self.abutmentBoxDict                = { "XMin" : 0, "YMin" : 0 }
        self.abutmentBox                    =    Box ()
        self.terminal1Box                   =    Box()
        self.terminal2Box                   =    Box()

        self.t1cutsEnclosure                = {}
        self.t2cutsEnclosure                = {}
 
        self.t1CutCenterDict                = {}
        self.t2CutCenterDict                = {}
            
        if self.resistorType == "RPOLYH" : 
            self.pImplant_layerDict         = {}
            self.hresLayerDict              = {}
        if self.resistorType == "RPOLY2PH" :
            self.restrmLayerDict            = {}

        self.resdefLayerDict                = {}
        self.enclosure_resistor_abutmentBox = {}

        return



    def __initArgs__( self, resistorType, direction, bends, shape ):

        if resistorType == "RPOLYH" or resistorType == "RPOLY2PH" :   self.resistorType = resistorType
        else                                                      :   raise Error(1,'__initArgs__() : Unsupported resistor type : %s.' % resistorType)

        if direction in ["horizontal", "vertical"]                :   self.direction = direction
        else                                                      :   raise Error(1,'__initDirection__() : Direction must be either "horizontal" or "vertical".')

        if bends >= 0                                             :
            if bends == 0                                         : [ self.bends, self.snakeMode             ] = [ bends , False        ]
            elif bends  > 0 and shape in [90, 135]                : [ self.bends, self.snakeMode, self.shape ] = [ bends , True, shape  ]
            else                                                  :   raise Error(1,'__initArgs__() : Resistor shape must be either 90 or 135 degrees : shape = %s.' %shape)
        else                                                      :   raise Error(1,'__initArgs__() : Bends number must not be negative : bends = %s.' %bends)
        
        return


    def __initContactsNumber__( self, resDim ):

        layer_minWidth      = Resistor.rules.minWidth_cut0 + 2*Resistor.rules.minEnclosure_poly2_cut0
        layerWidth          = self.resPlateExtensions_minWidth if resDim["width"] < layer_minWidth else resDim["width"]

        self.contactsNumber = self.cutMaxNumber(layerWidth, Resistor.rules.minWidth_cut0, Resistor.rules.minSpacing_cut0, Resistor.rules.minEnclosure_poly2_cut0)

        return


    
    def __initResDim__( self, resistance, resDim ):

        if self.__isDimOk__( resDim, "width" ) :
        
            self.__initContactsNumber__(resDim)
            print("contacts",self.contactsNumber)

            if   resistance > 0 and resDim["length"] > 0 :
            
                if  resistance - resistance/1000 <= self.__computeResistance__( resDim, self.resistorType ) <= resistance + resistance/1000 and self.__isDimOk__( resDim, "length" ):
                    self.resDim = resDim
                else : 
                    raise Error(1,'__initResDim__() : No compatibiliy between resistance value, %s, and resistor dimensions, "width" : %s, "length" : %s.' %(resistance, toPhy(resDim["width"]), toPhy(resDim["length"])))

            elif resistance > 0 and resDim["length"] == 0 :
                print("resistance0",resistance)
                self.resDim = self.__computeResDim__( resistance, self.resistorType, resDim )

            elif resistance == 0 and resDim["width"] > 0 and resDim["length"] > 0 and self.__isDimOk__( resDim, "length" ): 
                self.resDim = resDim

            else : raise Error(1, '__initResDim__() : Invalid input parameters : resistance and/or resistor dimensions (%s, %s). Please provide at least one valid parameter.' %(resistance, resDim))

        return


    def setSidesDimRules( self ):

        if  self.resistorType == "RPOLYH"  :
            self.__setattr__( "minWidth_resistorPlate"                   ,  Resistor.rules.minWidth_rpolyh         )
        if self.resistorType == "RPOLY2PH" : 
            self.__setattr__( "minWidth_resistorPlate"                   ,  Resistor.rules.minWidthHighPrec_rpolyh )
        self.__setattr__    ( "minLength_resistorPlate"                  ,  Resistor.rules.minRpolyhSquares        )

        return


    def __isDimOk__( self, resdim, side ) :

        self.setSidesDimRules()

        print( resdim )
        if list(resdim.keys()) == ["width","length"] and side in ["width","length"]:
        
            if side == "width"  :
                rule = self.minWidth_resistorPlate
            if side == "length" :
                print("test0",resdim["width"])
                rule = toDbU( self.minLength_resistorPlate*toPhy(resdim["width"]) )

            state = True if resdim[side] >= rule else False   
            if state == False : raise Error(1,'__isDimOk__() : Resistor or bend side dimension, %s = %s, is below the minimal limit, %s.'
                                           % (side, toPhy(resdim[side]), toPhy(rule)))

        else : raise Error(1,'__isDimOk__() : The resistor or bend dimensions and the specified side, %s and %s, must be either "width" or "length".'
                          %(resdim,side))

        return state



    def __computeResistance__( self, resDim, resistorType ): 
        
        resistancePart1 =   self.__getSheetResistance__( resistorType, "resistorPlate" )*toPhy(resDim["length"])/toPhy(resDim["width"])
        resistancePart2 = 2*self.__getSheetResistance__( resistorType, "contact"       )/self.contactsNumber
        print("computedRes",resistancePart1 + resistancePart2)  
        return resistancePart1 + resistancePart2

         

    def __computeResDim__( self, resistance, resistorType, resDim ):

        contactsResistance = self.__getSheetResistance__( resistorType, "contact" )/self.contactsNumber
        print("contactsResistance",contactsResistance)
        wl_ratio           = (resistance - 2*contactsResistance)/self.__getSheetResistance__( resistorType, "resistorPlate" )
        width              = toPhy(resDim["width"])
        length             = wl_ratio*width

        resDim["length"]   = toFoundryGrid( toDbU(length) )
        print('length0',length)
        if self.__isDimOk__(resDim, "width") and  self.__isDimOk__(resDim, "length") : 
            return resDim 


    def __getSheetResistance__( self, resistorType, material ):

        if material == 'resistorPlate':
            if     resistorType == "RPOLYH"  : resistance = Resistor.rules.RPOLYHSheetRes
            elif   resistorType == "RPOLY2PH": resistance = Resistor.rules.RPOLY2PHSheetRes
            else : raise Error( 1, '__getSheetResistance__() : Unsupported resistor type : %s.' % resistorType ) 

        elif material == "contact" :
            resistance = Resistor.rules.MET1RPOLYHContRes

        return resistance



    def __computeSnakeCornerDim__( self ):
        
        self.snakeCornerDict["width" ] =   self.resDim["width"]
        self.snakeCornerDict["length"] = 2*self.resDim["width"] + self.minSpacing_resistorPlate 

        return 



    def __getCornerResistanceCorrectionFactor__( self ):

        if self.shape not in [90, 135] : raise Error(1,'__getCornerResistanceCorrectionFactor__() : Invalid shape : %s.' %shape)
        if self.shape == 135           : correctionFactor = Resistor.rules.corrFactor135
        if self.shape == 90            : correctionFactor = Resistor.rules.corrFactor90

        print(correctionFactor)
        return correctionFactor




    def setRules( self ):
        trace( 101, '+,', '\tResistor.setRules()\n' )

        self.minSpacing_resistorPlate           = Resistor.rules.minSpacing_rpolyh
        self.minWidth_contact                   = Resistor.rules.minWidth_cut0
        self.minHeight_contact                  = Resistor.rules.minWidth_cut0
        self.minSpacing_contact                 = Resistor.rules.minSpacing_cut0
        self.minEnclosure_resistorPlate_contact = Resistor.rules.minEnclosure_poly2_cut0
        self.minEnclosure_routingLayer_contact  = Resistor.rules.minEnclosure_metal1_cut0
        self.minEnclosure_metal1_cut0           = Resistor.rules.minEnclosure_metal1_cut0
        self.minEnclosure_metal1_cut1           = Resistor.rules.minEnclosure_metal1_cut1
        self.minWidth_cut1                      = Resistor.rules.minWidth_cut1
        self.minSpacing_cut1                    = Resistor.rules.minSpacing_cut1

        if self.resistorType == "RPOLYH":
            self.minWidth_pImplantLayer                 =  Resistor.rules.minWidth_pImplant
            self.minSpacing_pImplantLayer               =  Resistor.rules.minSpacing_pImplant
            self.minSpacing_resistorPlate_pImplantLayer =  Resistor.rules.minSpacing_rpolyh_pImplant
            self.minEnclosure_pImplantLayer_contact     =  Resistor.rules.minEnclosure_pImplant_poly2con
            self.minEnclosure_hres_poly2                =  Resistor.rules.minEnclosure_hres_poly2
            self.minWidth_hres                          =  Resistor.rules.minWidth_hres
            self.minSpacing_hres                        =  Resistor.rules.minSpacing_hres
            self.minSpacing_hres_poly1                  =  Resistor.rules.minSpacing_hres_poly1
            self.minSpacing_hres_poly2                  =  Resistor.rules.minSpacing_hres_poly2
            self.minSpacing_hres_active                 =  Resistor.rules.minSpacing_hres_active
            
        self.resPlateExtensions_minWidth   = self.minWidth_contact + 2*self.minEnclosure_resistorPlate_contact

       # Get the METAL 1/2/3 layers.
        self.cut1   = DataBase.getDB().getTechnology().getLayer( 'cut1' )
        self.metal1 = DataBase.getDB().getTechnology().getLayer( 'metal1' )
        self.metal2 = DataBase.getDB().getTechnology().getLayer( 'metal2' )
        self.metal3 = DataBase.getDB().getTechnology().getLayer( 'metal3' )
       # Get the symbolic pitch.
        rg = AllianceFramework.get().getRoutingGauge()
        self.METAL2Pitch = rg.getHorizontalPitch()
        self.METAL3Pitch = rg.getVerticalPitch()
        self.isVH        = rg.isVH()
        
        foundHor = False
        foundVer = False
        for depth in range(rg.getDepth()):
            rlg = rg.getLayerGauge(depth)
            if rlg.getLayer().getMask() == self.metal2.getMask(): self.metal2Width = rlg.getWireWidth()
            if rlg.getLayer().getMask() == self.metal3.getMask(): self.metal3Width = rlg.getWireWidth()

            if rlg.getType() == Constant.PinOnly: continue
            if rlg.getDirection() == Constant.Horizontal and not foundHor:
                trace( 101, '\tself.hpitch set\n' )
                self.hpitch = rlg.getPitch()
                foundHor = True
            if rlg.getDirection() == Constant.Vertical and not foundVer:
                trace( 101, '\tself.vpitch set\n' )
                self.vpitch = rlg.getPitch()
                foundVer = True

        trace( 101, '-,' )
        return


    def getLayers( self ):

        technology                 = DataBase.getDB().getTechnology()
        self.layers                = {}

        self.layers["resistorPlate"] = technology.getLayer( "poly2"     )
        self.layers["contacts"     ] = technology.getLayer( "cut0"      )
        self.layers["routingLayer" ] = technology.getLayer( "metal1"    )
        self.layers["metal2"       ] = technology.getLayer( "metal2"    )
        self.layers["pImplant"     ] = technology.getLayer( "pImplant"  )
        self.layers["resdef"       ] = technology.getLayer( "resdef"    )

        if self.resistorType == "RPOLYH"   : self.layers["hres"  ] =  technology.getLayer( "hres"   )
        if self.resistorType == "RPOLY2PH" : self.layers["restrm"] =  technology.getLayer( "restrm" )
 
        return self.layers


    def create( self, bbMode = False ):

        UpdateSession.open()           

        layerDict = self.getLayers()
        self.computeDimensions(bbMode)
        self.drawAbutmentBox  ()
        self.device.setAbutmentBox( self.abutmentBox )
        
        if bbMode == False :

            if self.resistorType == "RPOLYH" : self.drawLayer("hres",  layerDict["hres"]) #self.drawHRESLayer( layerDict["hres"] )
        
            self.drawLayer        ("resistorPlate", layerDict["resistorPlate"]) 

            self.drawResPlateExtrimities( layerDict["resistorPlate"], layerDict["pImplant"] )
            if self.snakeMode : self.drawCorners(layerDict["resistorPlate"])
         
            self.drawContacts( layerDict["contacts"])
            self.drawRoutingLayer(layerDict["routingLayer"])
            if self.resistorType == "RPOLY2PH" : self.drawRestrmLayer(layerDict["restrm"])

            self.drawResdefLayer(layerDict["resdef"])
            self.drawTerminals()

        UpdateSession.close()

        return self.abutmentBoxDict



    def computeDimensions( self, bbMode ):

        if  self.snakeMode    :
            self.__computeSnakeCornerDim__    ()
            self.computeSnakeSegmentDimensions()
            if self.resistorType == "RPOLYH"   : self.snakeSegments_spacing = max(self.minSpacing_resistorPlate_pImplantLayer, self.minSpacing_resistorPlate)
            if self.resistorType == "RPOLY2PH" : self.snakeSegments_spacing =     self.minSpacing_resistorPlate

        self.computeAbutmentBoxDimensions()
        
        if bbMode == False :
            if not self.snakeMode :
                self.computeResistorPlateDimensions()
            if     self.snakeMode :
                self.computeFirstSnakeSegmentPosition()

        return



    def computePImplantLayerDim ( self ):

        pImplantLayer_width               = max(self.minWidth_pImplantLayer, 2*self.minEnclosure_pImplantLayer_contact + self.minWidth_contact)
        self.pImplant_layerDict["width" ] = self.contactsNumber*self.minWidth_contact + (self.contactsNumber-1)*self.minSpacing_contact + 2*self.minEnclosure_pImplantLayer_contact
        self.pImplant_layerDict["length"] = pImplantLayer_width
 
        return


    ## extensions2 refers to a part of the corner
    def computeResPlateExtensionsDim( self ):

        self.resPlateExtensions_minWidth   = self.minWidth_contact + 2*self.minEnclosure_resistorPlate_contact
        self.resPlateExtensions["width"]   = self.resPlateExtensions_minWidth if self.resDim["width"] < self.resPlateExtensions_minWidth else self.resDim["width"]
        
        if self.resistorType == "RPOLYH"   : self.resPlateExtensions["length"] = self.minWidth_contact + self.minEnclosure_resistorPlate_contact + self.minEnclosure_pImplantLayer_contact 
        if self.resistorType == "RPOLY2PH" : self.resPlateExtensions["length"] = self.minWidth_contact + self.minEnclosure_resistorPlate_contact 
        print("self.resPlateExtensions['length']",self.resPlateExtensions["length"])
        if self.snakeMode :
            self.resPlateExtension1           = self.resPlateExtensions
            self.resPlateExtension2["width" ] = self.snakeSegmentDict["width"]
            self.resPlateExtension2["length"] = self.snakeSegmentDict["width"]
 
        return
 



    def computeResPlateExtrimitiesDim( self ):

        if self.resistorType == "RPOLYH"       :
            self.resPlateExtrimities   ["width" ] = self.pImplant_layerDict ["width" ] + 2*self.enclosure_resistor_hres
            self.resPlateExtrimities   ["length"] = self.pImplant_layerDict ["length"] +   self.enclosure_resistor_hres     #self.pImplant_layerDict

        if self.resistorType == "RPOLY2PH"     :  
            for key in self.resPlateExtensions.keys(): self.resPlateExtrimities   [key] = self.resPlateExtensions [key] 
 
        if self.snakeMode                      :
            for key in self.resPlateExtrimities.keys(): self.resPlateExtrimity1   [key] = self.resPlateExtrimities [key] 

            if self.resistorType == "RPOLYH"   :
                self.resPlateExtrimity2["width" ] = self.resPlateExtension2 ["width" ] + 2*self.enclosure_resistor_hres
                self.resPlateExtrimity2["length"] = self.resPlateExtension2 ["length"] +   self.enclosure_resistor_hres

            if self.resistorType == "RPOLY2PH" :
                for key in self.resPlateExtension2.keys() : self.resPlateExtrimity2 [key] = self.resPlateExtension2[key] 

            for key in self.resPlateExtrimity1.keys()     : self.resPlateExtrimities[key] = max(self.resPlateExtrimity1[key] , self.resPlateExtrimity2[key]) 

        return




    def computeAbutmentBoxDimensions( self ):

        if self.resistorType == "RPOLYH"   :  self.enclosure_resistor_abutmentBox = self.minSpacing_hres  
        if self.resistorType == "RPOLY2PH" :  self.enclosure_resistor_abutmentBox = self.minSpacing_resistorPlate

        self.computeResPlateExtensionsDim()

        print("self.resPlateExtensionsabt0['length']",toPhy(self.resPlateExtensions["length"]))
        if not self.snakeMode : layerWidth = self.resPlateExtensions["width"]
        if     self.snakeMode : layerWidth = self.resPlateExtension1["width"]

        self.contactsNumber =   self.cutMaxNumber( layerWidth, self.minHeight_contact, self.minSpacing_contact, self.minEnclosure_resistorPlate_contact )

        if self.resistorType == "RPOLYH" : 
            self.computePImplantLayerDim()
            self.computeHRESLayerDimensions()

        self.computeResPlateExtrimitiesDim()

        if not self.snakeMode :
            abutmentBoxSide1 = self.resPlateExtrimities["width" ] + 2*self.enclosure_resistor_abutmentBox
            abutmentBoxSide2 = self.resDim             ["length"] + 2*self.resPlateExtrimities["length"]  + 2*self.enclosure_resistor_abutmentBox

        if self.snakeMode     :
            if self.resistorType == "RPOLYH"   : resistor_width = list(self.hresLayerDict.values())[0]
            if self.resistorType == "RPOLY2PH" : resistor_width = (self.bends+1)*self.snakeSegmentDict["width"] + self.bends*self.snakeSegments_spacing  

            abutmentBoxSide1 = resistor_width + 2*self.enclosure_resistor_abutmentBox  #width
            if self.bends > 1 :
                abutmentBoxSide2 = self.snakeSegmentDict["length"] + 2*self.resPlateExtrimities["length"]  + 2*self.enclosure_resistor_abutmentBox
            if self.bends == 1 :
                abutmentBoxSide2 = self.snakeSegmentDict["length"] + self.resPlateExtrimity1["length"] + self.resPlateExtrimity2["length"]  + 2*self.enclosure_resistor_abutmentBox

        if self.direction == "vertical"   :
            self.abutmentBoxDict["width" ] = abutmentBoxSide1
            self.abutmentBoxDict["height"] = abutmentBoxSide2 
                
        if self.direction == "horizontal" :
            self.abutmentBoxDict["width" ] = abutmentBoxSide2
            self.abutmentBoxDict["height"] = abutmentBoxSide1

        return



    def computeHRESLayerDimensions( self ):

        self.enclosure_resistor_hres = self.minEnclosure_hres_poly2
        if not self.snakeMode : hresLayerSide1 = max(self.minWidth_hres,   self.pImplant_layerDict["width" ] + 2*self.enclosure_resistor_hres)   
        if     self.snakeMode : hresLayerSide1 = max(self.minWidth_hres,  (self.bends+1)*self.snakeSegmentDict["width"] + self.bends*self.snakeSegments_spacing + 2*self.enclosure_resistor_hres)

        if self.direction == "vertical"   :
            self.hresLayerDict["length"] = hresLayerSide1 
                
        if self.direction == "horizontal" :
            self.hresLayerDict["width" ] = hresLayerSide1


        return



    def computeResistorPlateDimensions( self ):

        self.resistorPlateDict["width"  ]     = self.resDim["width" ]
        self.resistorPlateDict["length" ]     = self.resDim["length"]

        if self.direction == "vertical"      : keysList = ["XCenter","YMin","YMax","XMin","width" ]
        if self.direction == "horizontal"    : keysList = ["YCenter","XMin","XMax","YMin","height"]

        self.resistorPlateDict[ keysList[0] ] = self.abutmentBoxDict  [keysList[3]] + self.abutmentBoxDict  [keysList[4]]/2
        self.resistorPlateDict[ keysList[1] ] = self.abutmentBoxDict  [keysList[1]] + self.enclosure_resistor_abutmentBox + self.resPlateExtrimities["length"]
        self.resistorPlateDict[ keysList[2] ] = self.resistorPlateDict[keysList[1]] + self.resistorPlateDict["length"]


        return


    ## The first bend is the lowest one (horizontally)
    def computeSnakeSegmentDimensions( self ):

        snakeSegmentDict           = {}
        snakeSegmentDict["width" ] =  toPhy(self.resDim["width" ]) 
        totalResLength             =  toPhy(self.resDim["length"]) 
        correctionFactor           =  self.__getCornerResistanceCorrectionFactor__()

        snakeSegmentDict["length"] = (totalResLength- self.bends*( snakeSegmentDict["width"]*(correctionFactor+1) + toPhy(self.minSpacing_resistorPlate) )) / (self.bends + 1)
        snakeSegmentDict     ["length"] = toFoundryGrid( toDbU(snakeSegmentDict["length"]) )
        snakeSegmentDict     ["width" ] = toFoundryGrid( toDbU(snakeSegmentDict["width" ]) )
        print("snakesegW",snakeSegmentDict["width"  ] )
        if self.__isDimOk__(snakeSegmentDict, "length") : 
            self.snakeSegmentDict["width"  ] = snakeSegmentDict["width" ]
            self.snakeSegmentDict["length" ] = snakeSegmentDict["length"]
            
        return


    ## The first bend is the lowest one (horizontally)
    def computeFirstSnakeSegmentPosition( self ) :

     if self.direction == "vertical"        :

            if self.bends >  1 : firstExtrimity_width = self.resPlateExtrimities["width"]
            if self.bends == 1 : firstExtrimity_width = self.resPlateExtrimity1 ["width"]

            if self.resistorType == "RPOLYH"   : 
                self.snakeSegmentDict["XCenter"] = self.abutmentBoxDict["XMin"] + self.enclosure_resistor_abutmentBox + self.snakeSegmentDict["width"]/2 + self.enclosure_resistor_hres 
                self.snakeSegmentDict["YMin"   ] = self.abutmentBoxDict["YMin"] + self.enclosure_resistor_abutmentBox + firstExtrimity_width #length

            if self.resistorType == "RPOLY2PH" : 
                self.snakeSegmentDict["XCenter"] = self.abutmentBoxDict["XMin"] + self.enclosure_resistor_abutmentBox + self.snakeSegmentDict["width"]/2 
                self.snakeSegmentDict["YMin"   ] = self.abutmentBoxDict["YMin"] + self.enclosure_resistor_abutmentBox + firstExtrimity_width 

            self.snakeSegmentDict    ["YMax"   ] = self.snakeSegmentDict["YMin"] + self.snakeSegmentDict["length"]


     if self.direction == "horizontal"      :

            if self.bends >  1 : firstExtrimity_length = self.resPlateExtrimities["length"]
            if self.bends == 1 : firstExtrimity_length = self.resPlateExtrimity1 ["length"]

            if self.resistorType == "RPOLYH"   : 
                self.snakeSegmentDict["YCenter"] = self.abutmentBoxDict["YMin"] + self.enclosure_resistor_abutmentBox + self.snakeSegmentDict["width"]/2 + self.enclosure_resistor_hres 
                self.snakeSegmentDict["XMin"   ] = self.abutmentBoxDict["XMin"] + self.enclosure_resistor_abutmentBox + firstExtrimity_length

            if self.resistorType == "RPOLY2PH" :
                self.snakeSegmentDict["YCenter"] = self.abutmentBoxDict["YMin"] + self.enclosure_resistor_abutmentBox + self.snakeSegmentDict["width"]/2 
                self.snakeSegmentDict["XMin"   ] = self.abutmentBoxDict["XMin"] + self.enclosure_resistor_abutmentBox + firstExtrimity_length 

            self.snakeSegmentDict    ["XMax"   ] = self.snakeSegmentDict["XMin"] + self.snakeSegmentDict["length"]


    def computeHRESLayerPosition( self ):
        if self.direction == "vertical"      :
            self.hresLayerDict["XCenter"] = self.abutmentBox.getXCenter()
            self.hresLayerDict["YMin"   ] = self.abutmentBox.getYMin() + self.enclosure_resistor_abutmentBox
            self.hresLayerDict["YMax"   ] = self.abutmentBox.getYMax() - self.enclosure_resistor_abutmentBox
       
        if self.direction == "horizontal"    :
            self.hresLayerDict["YCenter"] = self.abutmentBox.getYCenter()
            self.hresLayerDict["XMin"   ] = self.abutmentBox.getXMin() + self.enclosure_resistor_abutmentBox
            self.hresLayerDict["XMax"   ] = self.abutmentBox.getXMax() - self.enclosure_resistor_abutmentBox


    def drawLayer( self, layerLabel, layer):

        thiknessKey          = "width"
        maxIterations        =  1
        centerTranslation    =  0

        if layerLabel not in ["hres","resistorPlate"] : raise Error(1,'drawLayer() : Supported layer labels "hres" or "resistorPlate".')

        if layerLabel == "hres"   : 
            self.computeHRESLayerPosition()
            positionParams = self.hresLayerDict
            if self.direction == "vertical"   : thiknessKey = "length"

        if layerLabel == "resistorPlate" :
        
            if not self.snakeMode : 
                positionParams = self.resistorPlateDict

            if     self.snakeMode :
                positionParams = self.snakeSegmentDict
                maxIterations  = self.bends + 1
                centerTranslation    = self.snakeSegmentDict["width"] + self.snakeSegments_spacing

        if self.direction == "vertical"        : 
            for i in range(0, maxIterations)   :
                print( self.nets[0], layer \
                    , positionParams["XCenter"  ] + i*centerTranslation \
                    , positionParams[thiknessKey] \
                    , positionParams["YMin"     ] \
                    , positionParams["YMax"     ] )
                Vertical.create  ( self.nets[0], layer
                                 , int(positionParams["XCenter"  ] + i*centerTranslation)
                                 , int(positionParams[thiknessKey])
                                 , int(positionParams["YMin"     ])
                                 , int(positionParams["YMax"     ])
                                 )

        if self.direction == "horizontal"      : 
            for i in range(0, maxIterations)   :
                Horizontal.create( self.nets[0], layer
                                 , int(positionParams["YCenter"  ] + i*centerTranslation)
                                 , int(positionParams[thiknessKey])
                                 , int(positionParams["XMin"     ])
                                 , int(tositionParams["XMax"     ] )
                                 ) 
        return



    def drawResdefLayer( self, layer ):

        self.computeResDefDimensions ()
        if not self.snakeMode :
            self.drawResdefForResistorPlate( layer )
        if     self.snakeMode : 
            self.drawResdefForSnakeSegments( layer )
            self.drawResdefForCorners      ( layer )        


        return


    def drawResdefForResistorPlate( self, layer ):

        if self.direction == "vertical"        :
            Vertical.create  ( self.nets[0], layer
                             , int(self.resdefLayerDict["XCenter"])
                             , int(self.resdefLayerDict["width"  ])
                             , int(self.resdefLayerDict["YMin"   ])
                             , int(self.resdefLayerDict["YMax"   ])
                             )

        if self.direction == "horizontal"      : 
            Horizontal.create( self.nets[0], layer
                             , int(self.resdefLayerDict["YCenter"])
                             , int(self.resdefLayerDict["width"  ])
                             , int(self.resdefLayerDict["XMin"   ])
                             , int(self.resdefLayerDict["XMax"   ])
                             ) 
        print("self.resdefLayerDict['width'  ]",self.resdefLayerDict["width"  ])
        print("self.resistorPlateDict['width'  ]",self.resistorPlateDict["width"  ])

        return


    def drawResdefForSnakeSegments( self, layer ):

        if not self.snakeMode              : centerTranslation  =  0
        if     self.snakeMode              : centerTranslation  = self.snakeSegmentDict["width"] + self.snakeSegments_spacing

        if self.resistorType == "RPOLYH"   : extrimityTranslation = 0
        if self.resistorType == "RPOLY2PH" : extrimityTranslation = self.minWidth_contact/4
        
        if self.direction == "vertical"        :

            for i in range(0, self.bends + 1)  : 
                if self.resistorType == "RPOLYH"   : factor0, factor1 = 1,1
                if self.resistorType == "RPOLY2PH" : 
                    factor0 = 0 if ( i == self.bends and self.bends % 2 == 0 )           else 1
                    factor1 = 0 if ( i == self.bends and self.bends % 2 != 0 ) or i == 0 else 1
                Vertical.create  ( self.nets[0], layer
                                 , int(self.resdefLayerDict["XCenter"] + i*centerTranslation)
                                 , int(self.resdefLayerDict["width"  ])
                                 , int(self.resdefLayerDict["YMin"   ] + factor0*extrimityTranslation)
                                 , int(self.resdefLayerDict["YMax"   ] - factor1*extrimityTranslation)
                                 )

        if self.direction == "horizontal"      : 

            for i in range(0, self.bends + 1)  :
                if self.resistorType == "RPOLYH"   : factor0, factor1 = 1, 1
                if self.resistorType == "RPOLY2PH" : 
                    factor0 = 0 if i == 0 or (i == self.bends and self.bends % 2 != 0) else 1
                    factor1 = 0 if            i == self.bends and self.bends % 2 == 0  else 1
                Horizontal.create( self.nets[0], layer
                                 , int(self.resdefLayerDict["YCenter"] + i*centerTranslation)
                                 , int(self.resdefLayerDict["width"  ])
                                 , int(self.resdefLayerDict["XMin"   ] + factor0*extrimityTranslation)
                                 , int(self.resdefLayerDict["XMax"   ] - factor1*extrimityTranslation)
                                 ) 
        return



    def drawResdefForCorners( self, layer ):

        k,factor = 0,0
        if self.shape == 135    :
            factor = 1
            self.computeFirstSnakeCornerPosition90()

        translation             = self.snakeSegmentDict ["width"  ] + self.snakeSegments_spacing
        centersTranslationConst = self.snakeSegmentDict ["width"  ] + self.snakeSegmentDict["length"]

        if self.shape == 90     : widthExtension = self.minWidth_contact/2
        if self.shape == 135    : widthExtension = self.minWidth_contact/4 + toDbU(0.2)

        if self.direction == "vertical"        : 
            snakeCornersYCenters    = [ self.snakeCornerDict["YCenter"] - factor*( toDbU(0.2)/2 + self.minWidth_contact/8 ) 
                                      , self.snakeCornerDict["YCenter"] + factor*( toDbU(0.2)/2 + self.minWidth_contact/8 ) + centersTranslationConst ]

            for i in range(0,self.bends)       : 
                Horizontal.create( self.nets[0], layer
                                 , int(snakeCornersYCenters[k]      )
                                 , int(self.snakeCornerDict["width"] + widthExtension )
                                 , int(self.snakeCornerDict["XMin" ] - self.minWidth_contact/4 + i*translation)
                                 , int(self.snakeCornerDict["XMax" ] + self.minWidth_contact/4 + i*translation)
                                 ) 
                k = k+1 if k<1 else 0

        if self.direction == "horizontal"      : 
            snakeCornersXCenters = [ self.snakeCornerDict["XCenter"] + factor*( toDbU(0.2)/2 + self.minWidth_contact/8 )
                                   , self.snakeCornerDict["XCenter"] - factor*( toDbU(0.2)/2 + self.minWidth_contact/8 ) - centersTranslationConst ] 

            for i in range(0,self.bends)       : 
                Vertical.create  ( self.nets[0], layer
                                 , int(snakeCornersXCenters[k])
                                 , int(self.snakeCornerDict["width"] + widthExtension  )
                                 , int(self.snakeCornerDict["YMin" ] - self.minWidth_contact/4 + i*translation)
                                 , int(self.snakeCornerDict["YMax" ] + self.minWidth_contact/4 + i*translation) 
                                 ) 
                k = k+1 if k<1 else 0

        return



    def drawSnakeSegments( self, layer ):

        translation       =  self.snakeSegmentDict["width"] + self.snakeSegments_spacing 

        if self.direction == "vertical"       : 

            for i in range(0, self.bends + 1) : 
                Vertical.create  ( self.nets[0], layer
                                 , self.snakeSegmentDict["XCenter"] + i*translation
                                 , self.snakeSegmentDict["width" ]
                                 , self.snakeSegmentDict["YMin"  ]
                                 , self.snakeSegmentDict["YMax"  ] 
                                 ) 

        if self.direction == "horizontal"     :
        
            for i in range(0, self.bends + 1) :
                Horizontal.create( self.nets[0], layer
                                 , self.snakeSegmentDict["YCenter"] + i*translation
                                 , self.snakeSegmentDict["width"  ]
                                 , self.snakeSegmentDict["XMin"   ]
                                 , self.snakeSegmentDict["XMax"   ]
                                 )          
        return



    def drawAbutmentBox( self ):

        ab = Box( self.abutmentBoxDict["XMin"  ]
                , self.abutmentBoxDict["YMin"  ]
                , self.abutmentBoxDict["width" ]
                , self.abutmentBoxDict["height"]
                )
        
        height        = ab.getHeight()
        heightAdjust  = height % (2*self.hpitch)
        if heightAdjust:
          heightAdjust = 2*self.hpitch - heightAdjust
          ab.inflate( 0, heightAdjust//2 )

        width = ab.getWidth()
        widthAdjust = width % (2*self.vpitch)
        if widthAdjust:
          widthAdjust = 2*self.vpitch - widthAdjust
          ab.inflate( widthAdjust//2, 0 )

        self.abutmentBox = ab
        return




    def drawResPlateExtrimities( self, resPlateLayer, pImplantLayer ):

        self.drawResPlateExtensions(resPlateLayer)
        if self.resistorType == "RPOLYH" :
            self.drawPImplantLayer(pImplantLayer)

        return

    ## extension1 is on the right side of the resistor (if direction == horizontal) and the bottom side otherwhise
    def computeResPlateExtensionsPosition( self ):

        self.resPlateExtensions["ex1"] = {}

        if self.direction == "vertical"      :

            if not self.snakeMode : self.resPlateExtensions["ex1"]["XMin"] = self.abutmentBox.getXCenter() - self.resPlateExtensions["width" ]/2
            if     self.snakeMode : self.resPlateExtensions["ex1"]["XMin"] = self.abutmentBox.getXCenter() + (self.bends*(self.snakeSegmentDict["width"] + self.snakeSegments_spacing))/2 - self.resPlateExtensions["width"]/2

            if not self.snakeMode                         : translation =  self.resPlateExtensions["length"]   + self.resDim["length"]/2
            if     self.snakeMode and self.bends % 2 == 0 : translation =  self.resPlateExtensions["length"]   + self.snakeSegmentDict["length"]/2
            if     self.snakeMode and self.bends % 2 != 0 : translation =  self.snakeSegmentDict  ["length"]/2
            print("self.resPlateExtensions5['length']",toPhy(self.resPlateExtensions["length"]))
#            print("self.snakeSegmentDict['length']/2",toPhy(self.snakeSegmentDict["length"]/2))
#            print("translation",toPhy(self.resPlateExtensions['length']   + self.snakeSegmentDict["length"]/2))
            print("self.abutmentBox.getYCenter(",toPhy(self.abutmentBox.getYCenter()))
            if not self.snakeMode or  self.snakeMode and self.bends % 2 == 0 : self.resPlateExtensions["ex1"]["YMin"] = self.abutmentBox.getYCenter() - translation  
            if     self.snakeMode and self.bends % 2 != 0                    : self.resPlateExtensions["ex1"]["YMin"] = self.abutmentBox.getYCenter() + translation 
            
            print("self.resPlateExtensions['ex1']['YMin']",toPhy(self.resPlateExtensions["ex1"]["YMin"]))
            self.resPlateExtensions["ex1"]["XMax"] = self.resPlateExtensions["ex1"]["XMin"] + self.resPlateExtensions["width" ]
            self.resPlateExtensions["ex1"]["YMax"] = self.resPlateExtensions["ex1"]["YMin"] + self.resPlateExtensions["length"]

        if self.direction == "horizontal"    :

            if not self.snakeMode : resistorLength = self.resDim          ["length"]
            if     self.snakeMode : resistorLength = self.snakeSegmentDict["length"]

            self.resPlateExtensions["ex1"]["XMin"] = self.abutmentBox.getXCenter() - self.resPlateExtensions["length"]   - resistorLength/2 

            if not self.snakeMode : self.resPlateExtensions["ex1"]["YMin"] = self.abutmentBox.getYCenter() - self.resPlateExtensions["width" ]/2 
            if     self.snakeMode : self.resPlateExtensions["ex1"]["YMin"] = self.snakeSegmentDict["YCenter"]  - self.resPlateExtensions["width" ]/2

            self.resPlateExtensions["ex1"]["XMax"] = self.resPlateExtensions["ex1"]["XMin"] + self.resPlateExtensions["length"]
#            print("self.resPlateExtensions['length']",toPhy(self.resPlateExtension2["length"]))
            self.resPlateExtensions["ex1"]["YMax"] = self.resPlateExtensions["ex1"]["YMin"] + self.resPlateExtensions["width" ]

        return



    def drawResPlateExtensions( self, layer ):
    
        self.computeResPlateExtensionsPosition()
        if not self.snakeMode : resistorLength = self.resDim          ["length"]
        if     self.snakeMode : resistorLength = self.snakeSegmentDict["length"]

        translation1                     = resistorLength + self.resPlateExtensions["length"]
        if self.snakeMode : translation2 = self.bends*(self.snakeSegmentDict["width"] + self.snakeSegments_spacing) #- self.resPlateExtensions["width"] 

        if self.direction == "vertical"   :

            if not self.snakeMode                         : [ hTranslation , vTranslation ] = [ 0            , translation1 ]
            if     self.snakeMode and self.bends % 2 == 0 : [ hTranslation , vTranslation ] = [ translation2 , translation1 ]
            if     self.snakeMode and self.bends % 2 != 0 : [ hTranslation , vTranslation ] = [ translation2 , 0            ]

            self.terminal1Box = Box( int(self.resPlateExtensions["ex1"]["XMin"])
                                   , int(self.resPlateExtensions["ex1"]["YMin"])
                                   , int(self.resPlateExtensions["ex1"]["XMax"])
                                   , int(self.resPlateExtensions["ex1"]["YMax"])
                                   )

            self.terminal2Box = Box( int(self.resPlateExtensions["ex1"]["XMin"] - hTranslation)
                                   , int(self.resPlateExtensions["ex1"]["YMin"] + vTranslation)
                                   , int(self.resPlateExtensions["ex1"]["XMax"] - hTranslation)
                                   , int(self.resPlateExtensions["ex1"]["YMax"] + vTranslation)
                                   )

        if self.direction == "horizontal" :

            if not self.snakeMode                         : [ hTranslation , vTranslation ] = [ translation1 , 0            ]
            if     self.snakeMode and self.bends % 2 == 0 : [ hTranslation , vTranslation ] = [ translation1 , translation2 ]
            if     self.snakeMode and self.bends % 2 != 0 : [ hTranslation , vTranslation ] = [ 0            , translation2 ]

            print("vTranslation",vTranslation)
            print("hTranslation",hTranslation)
            
            self.terminal1Box = Box( self.resPlateExtensions["ex1"]["XMin"]
                                   , self.resPlateExtensions["ex1"]["YMin"]
                                   , self.resPlateExtensions["ex1"]["XMax"]
                                   , self.resPlateExtensions["ex1"]["YMax"]
                                   )

            self.terminal2Box = Box( self.resPlateExtensions["ex1"]["XMin"] + hTranslation
                                   , self.resPlateExtensions["ex1"]["YMin"] + vTranslation
                                   , self.resPlateExtensions["ex1"]["XMax"] + hTranslation
                                   , self.resPlateExtensions["ex1"]["YMax"] + vTranslation
                                   )

        terminal1Pad = Pad.create(self.nets[0], layer, self.terminal1Box)
        terminal2Pad = Pad.create(self.nets[1], layer, self.terminal2Box)

        return



    def computePImplantLayerPosition( self ):

        factor1 = factor2 = -1
        if not self.snakeMode : centerTranslation = 0
        if     self.snakeMode : centerTranslation = self.bends*(self.snakeSegments_spacing + self.snakeSegmentDict["width"])/2

        if self.direction == "vertical"    : 
            keysList      = ["YCenter","XMin","XMax","YMin"]
            functionsList = [self.abutmentBox.getYCenter(), self.abutmentBox.getXCenter()]
            if self.snakeMode and self.bends % 2 != 0 : factor1 = 1
            if self.snakeMode                         : factor2 = 1

        if self.direction == "horizontal"  : 
            keysList      = ["XCenter","YMin","YMax","XMin"]
            functionsList = [self.abutmentBox.getXCenter(), self.abutmentBox.getYCenter()]

        if not self.snakeMode : resistorLength = self.resDim          ["length"]
        if     self.snakeMode : resistorLength = self.snakeSegmentDict["length"]
        
        self.pImplant_layerDict[ keysList[0] ] = functionsList[0] + factor1*( resistorLength/2  + self.pImplant_layerDict["length"]/2 )
        self.pImplant_layerDict[ keysList[1] ] = functionsList[1] + factor2*centerTranslation   - self.pImplant_layerDict["width" ]/2 #- centerTranslation - self.pImplant_layerDict["width" ]/2   
        self.pImplant_layerDict[ keysList[2] ] = self.pImplant_layerDict[keysList[1]]           + self.pImplant_layerDict["width" ]

        return


    def drawPImplantLayer( self, layer ):

        self.computePImplantLayerPosition()
        if not self.snakeMode : resistorLength = self.resDim          ["length"]
        if     self.snakeMode : resistorLength = self.snakeSegmentDict["length"]
        
        if not self.snakeMode or self.snakeMode and self.bends % 2 == 0 : centerTranslation      = resistorLength + self.pImplant_layerDict["length"]
        if     self.snakeMode                   and self.bends % 2 != 0 : centerTranslation      = 0
        if not self.snakeMode                                           : extrimitiesTranslation = 0
        if     self.snakeMode                                           : extrimitiesTranslation = self.bends*(self.snakeSegments_spacing + self.snakeSegmentDict["width"]) #+ self.pImplant_layerDict["width"]/2

        if self.direction == "vertical"   :
            for i in [0,1] : Horizontal.create( self.nets[i], layer
                                              , int(self.pImplant_layerDict["YCenter"] + i*centerTranslation)
                                              , int(self.pImplant_layerDict["length" ])
                                              , int(self.pImplant_layerDict["XMin"   ] - i*extrimitiesTranslation)
                                              , int(self.pImplant_layerDict["XMax"   ] - i*extrimitiesTranslation)
                                              )

        if self.direction == "horizontal" :
            for i in [0,1] : Vertical.create  ( self.nets[i], layer
                                              , int(self.pImplant_layerDict["XCenter"] + i*centerTranslation)
                                              , int(self.pImplant_layerDict["length" ])
                                              , int(self.pImplant_layerDict["YMin"   ] + i*extrimitiesTranslation)
                                              , int(self.pImplant_layerDict["YMax"   ] + i*extrimitiesTranslation)

                                              )

        return

    ## t1Cut connected to terminal1 (on the right side/bottom side of rectangular)
    def computecenterTranslationDimensions( self ):

        enclosure1 = ( self.resPlateExtensions["width"] - (self.contactsNumber*self.minHeight_contact + (self.contactsNumber -1)*self.minSpacing_contact) )/2
        enclosure2 =   self.minEnclosure_resistorPlate_contact

        if self.resistorType == "RPOLYH"   : enclosure3 =   self.minEnclosure_pImplantLayer_contact
        if self.resistorType == "RPOLY2PH" : enclosure3 =   0 

        if self.direction == "vertical":

            self.t1cutsEnclosure["horizontal"] = enclosure1
            if not self.snakeMode or self.snakeMode and self.bends % 2 == 0 : self.t1cutsEnclosure["vertical"] = enclosure2 
            if     self.snakeMode                   and self.bends % 2 != 0 : self.t1cutsEnclosure["vertical"] = enclosure3 

            self.t2cutsEnclosure["horizontal"] = enclosure1
            self.t2cutsEnclosure["vertical"  ] = enclosure3 
 
        if self.direction == "horizontal" :

            self.t1cutsEnclosure["horizontal"] = enclosure2 
            self.t1cutsEnclosure["vertical"  ] = enclosure1

            if not self.snakeMode or self.snakeMode and self.bends % 2 == 0 : self.t2cutsEnclosure["horizontal"] = enclosure3 
            if     self.snakeMode                   and self.bends % 2 != 0 : self.t2cutsEnclosure["horizontal"] = enclosure2 
            #self.t2cutsEnclosure["horizontal"] = enclosure3

            self.t2cutsEnclosure["vertical"  ] = enclosure1

        self.t1CutCenterDict         ["XCenter"   ] = self.terminal1Box.getXMin() + self.t1cutsEnclosure ["horizontal"] + self.minWidth_contact/2 
        self.t1CutCenterDict         ["YCenter"   ] = self.terminal1Box.getYMin() + self.t1cutsEnclosure ["vertical"  ] + self.minHeight_contact/2

        self.t2CutCenterDict         ["XCenter"   ] = self.terminal2Box.getXMin() + self.t2cutsEnclosure ["horizontal"] + self.minWidth_contact/2 
        self.t2CutCenterDict         ["YCenter"   ] = self.terminal2Box.getYMin() + self.t2cutsEnclosure ["vertical"  ] + self.minHeight_contact/2

        return



    def drawContacts( self, layer ):

        self.computecenterTranslationDimensions()

        if self.direction == "vertical"   : cutLineDirection = "horizontal"                  
        if self.direction == "horizontal" : cutLineDirection = "vertical"

        self.cutLine              ( self.nets[0], layer
                                  , self.t1CutCenterDict["XCenter"]
                                  , self.t1CutCenterDict["YCenter"]
                                  , self.minWidth_contact
                                  , self.minHeight_contact
                                  , self.minSpacing_contact
                                  , self.contactsNumber 
                                  , cutLineDirection
                                  )

        self.cutLine              ( self.nets[1], layer
                                  , self.t2CutCenterDict["XCenter"]
                                  , self.t2CutCenterDict["YCenter"]
                                  , self.minWidth_contact
                                  , self.minHeight_contact
                                  , self.minSpacing_contact
                                  , self.contactsNumber
                                  , cutLineDirection 
                                  )

        return


    def drawRoutingLayer( self, layer ):

        layer_width  = self.minWidth_contact + 2*self.minEnclosure_routingLayer_contact
        layer_height = self.contactsNumber*self.minWidth_contact + (self.contactsNumber-1)*self.minSpacing_contact + 2*self.minEnclosure_routingLayer_contact

        if self.direction == "vertical"   : key = "XCenter"
        if self.direction == "horizontal" : key = "YCenter"

        source1 = self.t1CutCenterDict[key] - self.minWidth_contact/2 - self.minEnclosure_routingLayer_contact
        target1 = source1 + layer_height

        source2 = self.t2CutCenterDict[key] - self.minWidth_contact/2 - self.minEnclosure_routingLayer_contact
        target2 = source2 + layer_height


        if self.direction == "vertical"   :
            Horizontal.create( self.nets[0]
                             , layer
                             , int(self.t1CutCenterDict["YCenter"])
                             , int(layer_width)
                             , int(source1)
                             , int(target1) )
            Horizontal.create( self.nets[1]
                             , layer
                             , int(self.t2CutCenterDict["YCenter"])
                             , int(layer_width)
                             , int(source2)
                             , int(target2)) 

            trace( 101, '\tIN PAD self.t1CutCenterDict["XCenter"] = {0}\n'.format(DbU.getValueString(self.t1CutCenterDict["XCenter"])) )
            trace( 101, '\tIN PAD source1 = {0}\n'.format(DbU.getValueString(source1)) )
            trace( 101, '\tIN PAD target1 = {0}\n'.format(DbU.getValueString(target1)) )

           #h = Horizontal.create( self.nets[0]
           #                     , self.layers['metal2']
           #                     , self.t1CutCenterDict["YCenter"]
           #                     , layer_width
           #                     , source1
           #                     , target1 )
           #NetExternalComponents.setExternal( h )
           #
           #h = Horizontal.create( self.nets[1]
           #                     , self.layers['metal2']
           #                     , self.t2CutCenterDict["YCenter"]
           #                     , layer_width
           #                     , source2
           #                     , target2 ) 
           #NetExternalComponents.setExternal( h )

        if self.direction == "horizontal" :
            Vertical.create (self.nets[0], layer, self.t1CutCenterDict["XCenter"], layer_width, source1, target1)
            Vertical.create (self.nets[1], layer, self.t2CutCenterDict["XCenter"], layer_width, source2, target2)                           

        return



    def drawRestrmLayer( self, layer ):

        self.computeRestrmLayerPosition() 
        if not self.snakeMode : resistorLength = self.resDim          ["length"]
        if     self.snakeMode : resistorLength = self.snakeSegmentDict["length"]

        if not self.snakeMode or     self.snakeMode and self.bends % 2 == 0 : centerTranslation = resistorLength + self.restrmLayerDict["width"]
        if     self.snakeMode                       and self.bends % 2 != 0 : centerTranslation = 0 

        if not self.snakeMode : extrimitiesTranslation  = 0
        if     self.snakeMode : extrimitiesTranslation  = self.bends*(self.snakeSegments_spacing + self.snakeSegmentDict["width"]) 

        if self.direction == "vertical"   :
            for i in [0,1]: Horizontal.create( self.nets[i], layer
                                             , self.restrmLayerDict["YCenter"] + i*centerTranslation
                                             , self.restrmLayerDict["width"  ]
                                             , self.restrmLayerDict["XMin"   ] - i*extrimitiesTranslation
                                             , self.restrmLayerDict["XMax"   ] - i*extrimitiesTranslation
                                             ) 

        if self.direction == "horizontal" :
            for i in [0,1]: Vertical.create  ( self.nets[i], layer
                                             , self.restrmLayerDict["XCenter"] + i*centerTranslation
                                             , self.restrmLayerDict["width"  ]
                                             , self.restrmLayerDict["YMin"   ] + i*extrimitiesTranslation
                                             , self.restrmLayerDict["YMax"   ] + i*extrimitiesTranslation
                                             ) 

        return



    def computeRestrmLayerPosition( self ):

        self.restrmLayerDict["width"]  = self.minWidth_contact/2
        factor                         = 1

        if self.direction == "vertical"   :
            keysList = ["YCenter","XMin","XMax"]
#            if not self.snakeMode or self.snakeMode and self.bends % 2 == 0 : factor = 1
            if     self.snakeMode                   and self.bends % 2 != 0 : factor = -1
        if self.direction == "horizontal" : 
            keysList = ["XCenter","YMin","YMax"]

        self.restrmLayerDict[ keysList[0] ] = self.t1CutCenterDict          [ keysList[0] ] + factor*self.restrmLayerDict["width"]/2
        self.restrmLayerDict[ keysList[1] ] = self.resPlateExtensions["ex1"][ keysList[1] ] - self.restrmLayerDict["width"]
        self.restrmLayerDict[ keysList[2] ] = self.resPlateExtensions["ex1"][ keysList[2] ] + self.restrmLayerDict["width"]

        return



    def computeResDefDimensions( self ):

        if self.direction == "vertical"   : keysList = ["YMin","YMax","XCenter","width"]
        if self.direction == "horizontal" : keysList = ["XMin","XMax","YCenter","width"]

        if not self.snakeMode : resistorElement = self.resistorPlateDict
        if     self.snakeMode : resistorElement = self.snakeSegmentDict

        if self.resistorType == "RPOLYH"   : factor = 0
        if self.resistorType == "RPOLY2PH" : factor = 1 

        self.resdefLayerDict[keysList[0]] = resistorElement[keysList[0]] - factor*self.minWidth_contact/4
        self.resdefLayerDict[keysList[1]] = resistorElement[keysList[1]] + factor*self.minWidth_contact/4
        self.resdefLayerDict[keysList[2]] = resistorElement[keysList[2]]
        self.resdefLayerDict[keysList[3]] = resistorElement[keysList[3]]
        
        if self.resDim["width"] >= self.resPlateExtensions_minWidth :
            self.resdefLayerDict[keysList[3]] = self.resdefLayerDict[keysList[3]] + self.minWidth_contact/2

        return


    def computeFirstSnakeCornerPosition90( self ):

        if self.direction == "vertical"   :

            self.snakeCornerDict["YCenter"] = self.snakeSegmentDict["YMin"   ] - self.snakeCornerDict ["width" ]/2
            self.snakeCornerDict["XMin"   ] = self.snakeSegmentDict["XCenter"] - self.snakeSegmentDict["width" ]/2
            self.snakeCornerDict["XMax"   ] = self.snakeCornerDict ["XMin"   ] + self.snakeCornerDict ["length"]

        if self.direction == "horizontal" :

            self.snakeCornerDict["XCenter"] = self.snakeSegmentDict["XMax"   ] + self.snakeCornerDict ["width" ]/2
            self.snakeCornerDict["YMin"   ] = self.snakeSegmentDict["YCenter"] - self.snakeSegmentDict["width" ]/2
            self.snakeCornerDict["YMax"   ] = self.snakeCornerDict ["YMin"   ] + self.snakeCornerDict ["length"]

        return



    def drawCorners90( self, layer ):

        self.computeFirstSnakeCornerPosition90()
        k                       = 0
        translation             = self.snakeSegmentDict["width"] + self.snakeSegments_spacing
        centersTranslationConst = self.snakeSegmentDict["width"] + self.snakeSegmentDict["length"]

        if self.direction == "vertical"   :

            snakeCornersYCenters = [ self.snakeCornerDict["YCenter"]
                                   , self.snakeCornerDict["YCenter"] + centersTranslationConst]

            for i in range(0,self.bends)  : 
                Horizontal.create( self.nets[0], layer
                                 , snakeCornersYCenters[k]
                                 , self.snakeCornerDict["width"]
                                 , self.snakeCornerDict["XMin" ] + i*translation
                                 , self.snakeCornerDict["XMax" ] + i*translation
                                 ) 
                k = k+1 if k<1 else 0

        if self.direction == "horizontal" :

            snakeCornersXCenters = [ self.snakeCornerDict["XCenter"]
                                   , self.snakeCornerDict["XCenter"] - centersTranslationConst ]

            for i in range(0,self.bends)  : 
                Vertical.create  ( self.nets[0], layer
                                 , snakeCornersXCenters[k]
                                 , self.snakeCornerDict["width"]
                                 , self.snakeCornerDict["YMin" ] + i*translation
                                 , self.snakeCornerDict["YMax" ] + i*translation 
                                 ) 
                k = k+1 if k<1 else 0

        return



    def computeFirstSnakeCornerPosition135( self ):

        param1 = (self.snakeSegmentDict["width"]*42/100)/2
        param2 = toDbU(0.2)
        param3 = self.snakeSegmentDict["width"] + param2
        param4 = param3 - param1

        if self.direction == "vertical"   : 

            rx0 = self.snakeSegmentDict["XCenter"] - self.snakeSegmentDict["width"]/2
            ry0 = self.snakeSegmentDict["YMin"   ]
            rx1 = rx0  
            ry1 = ry0 - param1
            rx2 = rx0 + param4
            ry2 = ry1 - param4 
            rx3 = rx0 + 2*self.snakeSegmentDict["width"] + self.snakeSegments_spacing - param4
            ry3 = ry2 
            rx4 = rx3 + param4 
            ry4 = ry1 
            rx5 = rx4
            ry5 = ry0 
            rx6 = rx5 - self.snakeSegmentDict["width"]
            ry6 = ry0 
            rx7 = rx0 + self.snakeSegmentDict["width"] + self.snakeSegments_spacing - param2
            ry7 = ry0 - param2 
            rx8 = rx0 + self.snakeSegmentDict["width"] + param2 
            ry8 = ry7 
            rx9 = rx0 + self.snakeSegmentDict["width"] 
            ry9 = ry0 

        if self.direction == "horizontal" : 

            rx0 = self.snakeSegmentDict["XMax"   ]
            ry0 = self.snakeSegmentDict["YCenter"] - self.snakeSegmentDict["width"]/2
            rx1 = rx0 + param1  
            ry1 = ry0
            rx2 = rx1 + (self.snakeSegmentDict["width"] + param2) - param1
            ry2 = ry1 + (self.snakeSegmentDict["width"] + param2) - param1 
            rx3 = rx2
            ry3 = ry2 + (2*self.snakeSegmentDict["width"] + self.snakeSegments_spacing) - 2*(self.snakeSegmentDict["width"] + param2 - param1)
            rx4 = rx1 
            ry4 = ry1 + 2*self.snakeSegmentDict["width"] + self.snakeSegments_spacing
            rx5 = rx0
            ry5 = ry0 + 2*self.snakeSegmentDict["width"] + self.snakeSegments_spacing
            rx6 = rx0
            ry6 = ry0 + self.snakeSegmentDict["width"] + self.snakeSegments_spacing
            rx7 = rx0 + param2 
            ry7 = ry6 - param2 
            rx8 = rx7 
            ry8 = ry0 + self.snakeSegmentDict["width"] + param2 
            rx9 = rx0
            ry9 = ry0 + self.snakeSegmentDict["width"]

        rightCorCordinatesList = [ [rx0 , ry0 ] , [rx1, ry1 ]
                                 , [rx2 , ry2 ] , [rx3, ry3 ] 
                                 , [rx4 , ry4 ] , [rx5, ry5 ] 
                                 , [rx6 , ry6 ] , [rx7, ry7 ]
                                 , [rx8 , ry8 ] , [rx9, ry9 ]
                                 ]
        for p in rightCorCordinatesList : 
            self.rightCorPointsVector.append(Point(int(p[0]),int(p[1])))
            self.leftCorPointsVector.append (Point(int(p[0]),int(p[1])))

        translationList = [ self.snakeSegmentDict["length"]
                             , self.snakeSegmentDict["length"] + 2*param1
                             , self.snakeSegmentDict["length"] + 2*param3
                             , self.snakeSegmentDict["length"] + 2*param3
                             , self.snakeSegmentDict["length"] + 2*param1
                             , self.snakeSegmentDict["length"] 
                             , self.snakeSegmentDict["length"]
                             , self.snakeSegmentDict["length"] + 2*param2 
                             , self.snakeSegmentDict["length"] + 2*param2
                             , self.snakeSegmentDict["length"]
                             ]

        if self.direction == "vertical"   : 
            for i in range(0, len(self.leftCorPointsVector)) : self.leftCorPointsVector[i].translate( 0 ,  int(translationList[i]) ) 
        if self.direction == "horizontal" :
            for i in range(0, len(self.leftCorPointsVector)) : self.leftCorPointsVector[i].translate( int(- translationList[i]) , 0) 
          
        return


    def drawCorners135( self, layer ):

        self.computeFirstSnakeCornerPosition135()
        if self.bends % 2 == 0 : [ rightCornersNum, leftCornersNum ] = [  self.bends//2    ,  self.bends//2    ]
        if self.bends % 2 != 0 : [ rightCornersNum, leftCornersNum ] = [ (self.bends+1)//2 , (self.bends-1)//2 ]

        translationFactor = 2*(self.snakeSegmentDict["width"] + self.snakeSegments_spacing)

        if self.direction == "vertical"    : [dxTranslation, dyTranslation] = [ translationFactor, 0                 ]  
        if self.direction == "horizontal"  : [dxTranslation, dyTranslation] = [ 0                , translationFactor ]  

        for i in range(0, rightCornersNum) : 
            Rectilinear.create( self.nets[0], layer, self.rightCorPointsVector ).translate(i*dxTranslation, i*dyTranslation)

        for i in range(0, leftCornersNum ) :
            if i == 0 : 
                 if self.direction == "vertical"  : Rectilinear.create(self.nets[0], layer, self.leftCorPointsVector).translate(dxTranslation//2,dyTranslation)
                 if self.direction == "horizontal": Rectilinear.create(self.nets[0], layer, self.leftCorPointsVector).translate(dxTranslation,dyTranslation//2)
                
            if i != 0 : 
                if self.direction == "vertical"  : Rectilinear.create(self.nets[0], layer, self.leftCorPointsVector).translate(i*3*dxTranslation//2,dyTranslation)
                if self.direction == "horizontal": Rectilinear.create(self.nets[0], layer, self.leftCorPointsVector).translate(dxTranslation,i*3*dyTranslation//2)

        return


    def drawCorners( self, layer):

        if self.shape == 90  : self.drawCorners90 (layer)
        if self.shape == 135 : self.drawCorners135(layer)

        return 


#    def drawResDefLayer( self, layer ):
    
#        self.computeResDefDimensions()
#        if self.direction == "vertical"    : Vertical.create   ( self.nets[0], layer, self.resdefLayerDict["XCenter"], self.resdefLayerDict["width"], self.resdefLayerDict["YMin"], self.resdefLayerDict["YMax"] ) 
#        if self.direction == "horizontal"  : Horizontal.create ( self.nets[0], layer, self.resdefLayerDict["YCenter"], self.resdefLayerDict["width"], self.resdefLayerDict["XMin"], self.resdefLayerDict["XMax"] ) 

#        return



    ## Creates a horizontal or vertical line of contacts according to the specified direction.

    def cutLine( self, net, layer, firstCutXCenter, firstCutYCenter, width_cut, height_cut, spacing_cut, cutNumber, direction ):       
 
        for i in range(0, cutNumber) :
            if direction == 'horizontal':
                segment = Contact.create( net
                                        , layer
                                        , int(firstCutXCenter + i*(width_cut + spacing_cut))
                                        , int(firstCutYCenter)
                                        , int(width_cut)
                                        , int(height_cut) )
            else:
                segment = Contact.create( net
                                        , layer
                                        , int(firstCutXCenter)
                                        , int(firstCutYCenter + i*(height_cut + spacing_cut))
                                        , int(width_cut)
                                        , int(height_cut) )

        return segment



    ## Computes the maximal number of cuts to be placed on a layer of width \c width_layer considering specifications such as the spacing between the cuts, its width and its enclosure in the layer.

    def cutMaxNumber( self, width_layer, width_cut, spacing_cut, enclosure_cut ): 

        cutNumber = int( (width_layer - 2*enclosure_cut + spacing_cut) / (width_cut + spacing_cut) )
        if cutNumber > 0: return cutNumber
        else:
            raise Error (1,"cutMaxNumber() : Zero number of cuts found. Layer width is too tight." )


    def drawTerminals ( self ):
        VIA1overhang = self.minWidth_cut1/2 + self.minEnclosure_metal1_cut1

        t1net    = self.device.getNet('t1')
        t2net    = self.device.getNet('t2')
        t1metal1 = None
        t2metal1 = None

        for component in t1net.getComponents():
            if isinstance(component,Horizontal) and component.getLayer() == self.metal1:
                t1metal1 = component
                break

        for component in t2net.getComponents():
            if isinstance(component,Horizontal) and component.getLayer() == self.metal1:
                t2metal1 = component
                break

        ab        = self.device.getAbutmentBox()
        t1OnNorth = isNearNorth( t1metal1.getY(), ab )
        t2OnNorth = isNearNorth( t2metal1.getY(), ab )

        if t1OnNorth and t2OnNorth:
            t1YM2 = ab.getYMax() - self.hpitch*2
            t2YM2 = ab.getYMax() - self.hpitch
        elif t1OnNorth:
            t1YM2 = ab.getYMax() - self.hpitch
            t2YM2 = ab.getYMin() + self.hpitch
        elif t2OnNorth:
            t1YM2 = ab.getYMin() + self.hpitch
            t2YM2 = ab.getYMax() - self.hpitch
        else:
            t1YM2 = ab.getYMin() + self.hpitch*2
            t2YM2 = ab.getYMin() + self.hpitch

        m1Width   =  self.contactsNumber   *self.minWidth_contact   \
                  + (self.contactsNumber-1)*self.minSpacing_contact \
                  + 2*self.minEnclosure_routingLayer_contact
        edgeDelta = self.minWidth_contact/2 + self.minEnclosure_metal1_cut0

        if t1OnNorth:
            t1BoxM1 = Box( int(0)
                         , int(self.t1CutCenterDict["YCenter"] - edgeDelta)
                         , int(m1Width)
                         , int(t1YM2 + VIA1overhang)
                         )
        else:
            t1BoxM1 = Box( int(0)
                         , int(t1YM2 - VIA1overhang)
                         , int(m1Width)
                         , int(self.t1CutCenterDict["YCenter"] + edgeDelta)
                         )
        t1BoxM1.translate( int(self.t1CutCenterDict["XCenter"] - edgeDelta), 0 )

        if t2OnNorth:
            t2BoxM1 = Box( int(0)
                         , int(self.t2CutCenterDict["YCenter"] - edgeDelta)
                         , int(m1Width)
                         , int(t2YM2 + VIA1overhang)
                         )
        else:
            t2BoxM1 = Box( int(0)
                         , int(t2YM2 - VIA1overhang)
                         , int(m1Width)
                         , int(self.t2CutCenterDict["YCenter"] + edgeDelta)
                         )
        t2BoxM1.translate( int(self.t2CutCenterDict["XCenter"] - edgeDelta), 0 )

        self.drawM2Terminal( t1net, t1BoxM1, t1YM2 )
        self.drawM2Terminal( t2net, t2BoxM1, t2YM2 )

        return


    def drawM2Terminal ( self, net, boxM1, axisM2 ):
        ab = self.device.getAbutmentBox()

        minBoxM1 = Box( boxM1.getCenter().getX(), axisM2 )
        minBoxM1.inflate( int(self.minWidth_cut1/2 + self.minEnclosure_metal1_cut1) )
        boxM1.merge( minBoxM1 )

        h = Horizontal.create( net
                             , self.metal2
                             , int(axisM2)
                             , int(self.metal2Width)
                             , int(ab.getXMin())
                             , int(ab.getXMax()) )
        NetExternalComponents.setExternal( h )

        Vertical.create( net
                       , self.metal1
                       , int(boxM1.getCenter().getX())
                       , int(boxM1.getWidth())
                       , int(boxM1.getYMin())
                       , int(boxM1.getYMax())
                       )

        cutNumber = ((boxM1.getWidth() - 2*self.minEnclosure_metal1_cut1 - self.minWidth_cut1) \
                    // (self.minSpacing_cut1 + self.minWidth_cut1))
        if cutNumber <= 0: cutNumber = 1
        centering = (boxM1.getWidth() - 2*self.minEnclosure_metal1_cut1 \
                  -  cutNumber    * self.minWidth_cut1               \
                  - (cutNumber-1) * self.minSpacing_cut1) // 2
        xcut1     = boxM1.getXMin() + centering \
                  + self.minEnclosure_metal1_cut1 + self.minWidth_cut1//2

        trace( 101, '\tcutNumber = {0}\n'.format(cutNumber) )
        for i in range(cutNumber):
            trace( 101, '\t[{0}]\n'.format(i) )
            Contact.create( net
                          , self.cut1
                          , int(xcut1)
                          , int(axisM2)
                          , int(self.minWidth_cut1)
                          , int(self.minWidth_cut1)
                          )
            xcut1 += self.minWidth_cut1 + self.minSpacing_cut1
        
        return
        


def scriptMain( **kw ):

    editor = None
    if 'editor' in kw and kw['editor']:
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
   #(device, resistorType, resistance = 0, resDim = { "width" : 10, "length" : 0 }, direction = "vertical", bends = 0 ):
   #resistor = Resistor( device, nets, "RPOLY2PH", 50, direction = "vertical" )
   #resistor = Resistor( device, nets, "RPOLY2PH", 200, direction = "horizontal",  resDim = { "width" : 0.8, "length" : 0 } )   #w >= 2 dogBone not possible in 0.35 um (used only in test) 
   #resistor = Resistor( device, nets, "RPOLY2PH", 800, direction = "vertical",  resDim = { "width" : 0.8, "length" : 0 }, bends = 4  )   #w >= 2 dogBone not possible 
   #resistor = Resistor( device, nets, "RPOLYH", 4800, direction = "vertical", resDim = { "width" : 0.8, "length" : 0 } )   #w = 0.8 dogBone
   #resistor = Resistor( device, nets, "RPOLYH", 48000, direction = "horizontal", resDim = { "width" : 0.8, "length" : 0 }, bends = 3 )   #w = 0.8 dogBone

   #resistor = Resistor( device, nets, "RPOLY2PH", 200, direction = "horizontal")    #RPOLY2PH normal
   #resistor = Resistor( device, nets, "RPOLYH", 7000, direction = "horizontal", resDim = { "width" : 10, "length" : 0 })    #RPOLYH normal

   #resistor = Resistor( device, nets, "RPOLYH", 20000, direction = "vertical", bends = 1, shape = 135 )   #RPOLYH snake
    resistor = Resistor( device, nets, "RPOLY2PH", 2000,  resDim = { "width" : 2.5, "length" : 0 },  direction = "vertical", bends = 1, shape = 90 )  #RPOLY2PH snake

    abutmentBoxDim = resistor.create()
    print("abutmentBoxDim",abutmentBoxDim)
                      
    AllianceFramework.get().saveCell( device, Catalog.State.Views )

    return True

