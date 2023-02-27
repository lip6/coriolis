#!/usr/bin/python

import sys		
from   math           import sqrt, ceil
from   ..Hurricane    import *
from   ..CRL          import *
from   ..helpers.io   import ErrorMessage as Error
from   ..helpers      import staticInitialization, trace
from   .              import getRules
from   .capacitorunit import CapacitorUnit

def toDbU    ( l ): return DbU.fromPhysical( l, DbU.UnitPowerMicro )
def toPhy    ( l ): return DbU.toPhysical  ( l, DbU.UnitPowerMicro )

def doBreak( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()

staticInitialization( True )




class NonUnitCapacitor(CapacitorUnit):


    rules = getRules()



    def __init__( self, device, capacitorType, direction, capacitance, capacitorUnit, abutmentBoxPosition, capUnitTopPlates_spacing = -1 ):

        self.device                = device
        self.capacitorType         = capacitorType
        self.direction             = direction
	self.capacitance           = capacitance
	self.capacitorUnit         = capacitorUnit 	
	self.abutmentBoxPosition   = abutmentBoxPosition 
	self.capUnitTopPlates_spacing = capUnitTopPlates_spacing

	self.unitCapDim = CapacitorUnit.__computeCapDim__( self, capacitorUnit, capacitorType  ) 

        if self.__isDirectionOk__(direction):
	    self.nonUnitCapDim = {"width" : self.unitCapDim["width"] , "height" : 0 } if direction == "vertical" else {"width" : 0 , "height" : self.unitCapDim["width"] }

	self.nonUnitCapDim_sideLimit = 0  
        self.dummyCapDim             = {}

        return



    def __isDirectionOk__( self, direction ):

         state = True if direction in ["horizontal","vertical"] else False
         if state == False : raise Error(1,'__isDirectionOk__() : The direction must be either "vertical" or "horizontal" : %s.' % direction)

         return state



    def setRules( self ):

	CapacitorUnit.setRules(self)

        if self.capacitorType == 'MIMCap':
            self.__setattr__( "minWidth_dummyTopPlate" , CapacitorUnit.rules.minWidth_metcapdum )

        elif self.capacitorType == 'PIPCap':
            self.__setattr__( "minWidth_dummyTopPlate" , CapacitorUnit.rules.minWidth_cpoly     )

	return



    def create( self, b, t ) :

        UpdateSession.open()           

	self.setRules()
	self.computeDimensions()

	if self.__isCapacitorOk__() :

	    print('drawing capacitor')
	    activeCapacitor = CapacitorUnit   ( self.device, self.capacitorType, self.abutmentBoxPosition, capDim = self.nonUnitCapDim ) 
    	    activeCapacitor.create( b, t )      

            side = "height" if self.direction == "vertical" else "width"
	    if self.__isNonUnitCapSideOk__( side )  :

    	        print("drawing dummy too")
#	        activeCapacitor = CapacitorUnit  ( self.device, activeCapacitance, self.capacitorType, self.abutmentBoxPosition ) 

		dummyAbutmentBoxPosition = [ self.abutmentBoxPosition[0], self.abutmentBoxPosition[1] - CapacitorUnit.computeAbutmentBoxDimensions( self, self.dummyCapDim )["height"] ] if self.direction == "vertical" else [ self.abutmentBoxPosition[0] + CapacitorUnit.computeAbutmentBoxDimensions( self, self.nonUnitCapDim )["width"] , self.abutmentBoxPosition[1] ]

                dummyCapacitor = CapacitorUnit( self.device, self.capacitorType, dummyAbutmentBoxPosition, capDim = self.dummyCapDim ) 
#    	        activeCapacitor.create( b, t )   
    	        dummyCapacitor.create ( b, t )   

            else : print("not drawing dummy")
            
#	    abutmentBoxDimensions = self.computeAbutmentBoxDimensions(capDim)
 #           CapacitorUnit.drawAbutmentBox( self )

	else : raise Error(1,'create() : Impossible to draw capacitor in the defined geometric limits.')

        UpdateSession.close()

	return










    def __isCapacitorOk__( self ):

	capacitorMax = self.computeCapacitorMax()

	print("CMax",capacitorMax) 
	print("self.capacitance",self.capacitance)
	print("self.capUnitTopPlates_spacing",toPhy(self.capUnitTopPlates_spacing))

	return True if self.capacitance < capacitorMax else False




    def computeDimensions( self ):

	if self.capUnitTopPlates_spacing == -1 :
	    CapacitorUnit.computeAbutmentBoxDimensions(self, self.unitCapDim )
	    self.capUnitTopPlates_spacing = 2*(self.minSpacing_botPlate + self.enclosure_botPlate_topPlate)

	self.nonUnitCapDim_sideLimit = 2*self.unitCapDim["width"] - self.minWidth_dummyTopPlate

        if self.direction == "vertical":
 	    self.nonUnitCapDim["height"] = self.computeNonUnitCapheight()
        else :  self.nonUnitCapDim["width"] = self.computeNonUnitCapheight()

	print("sideLimit",toPhy(self.nonUnitCapDim_sideLimit))
        print("self.capUnitTopPlates_spacing",toPhy(self.capUnitTopPlates_spacing))
        print('self.nonUnitCapDim["width"]',toPhy(self.nonUnitCapDim["width"]))
        print('self.nonUnitCapDim["height"]',toPhy(self.nonUnitCapDim["height"]))


        nonUnitCapDim_sideMax      = 2*self.unitCapDim["width"] + self.capUnitTopPlates_spacing
        self.dummyCapDim["width" ] =   self.unitCapDim["width" ]  if self.direction == "vertical" else nonUnitCapDim_sideMax - self.capUnitTopPlates_spacing - self.nonUnitCapDim["width"]
        self.dummyCapDim["height"] =   nonUnitCapDim_sideMax - self.capUnitTopPlates_spacing - self.nonUnitCapDim["height"] if self.direction == "vertical" else self.unitCapDim["width" ]

        print("self.dummyCapDim",toPhy(self.dummyCapDim["width" ]))
        print("self.dummyCapDim",toPhy(self.dummyCapDim["height" ]))
	return



    def computeCapacitorMax( self ):

	[ areaCapacitorPerUnit, perimeterCapacitorPerUnit ] = CapacitorUnit.__setCapacitorPerUnit__(self, self.capacitorType)
	capUnit_width                                       = toPhy( self.unitCapDim["width"]            )
	topPlates_spacing                                   = toPhy( self.capUnitTopPlates_spacing )
	print("[ areaCapacitorPerUnit, perimeterCapacitorPerUnit ]",[ areaCapacitorPerUnit, perimeterCapacitorPerUnit ])
	print("width",capUnit_width)
	CMax = ( 2*capUnit_width + topPlates_spacing )*capUnit_width*areaCapacitorPerUnit + ( 3*capUnit_width + topPlates_spacing )*perimeterCapacitorPerUnit

	return CMax



    def computeNonUnitCapheight( self ):


	[ areaCapacitorPerUnit, perimeterCapacitorPerUnit ] = CapacitorUnit.__setCapacitorPerUnit__(self, self.capacitorType)
	width                                               = toPhy( self.unitCapDim["width"]            )
	height                                              = self.capacitance/( (width*areaCapacitorPerUnit + perimeterCapacitorPerUnit) + width*perimeterCapacitorPerUnit )

	print("height",height)
	return toDbU(height)



    def __isNonUnitCapSideOk__( self, side ): 

        if side in ["width","height"]:
            state = True if self.nonUnitCapDim[side] < self.nonUnitCapDim_sideLimit else False

        else : raise Error(1,'__isNonUnitCapSideOk__() : The side must be either "width" or "height" : %s.' % side)

        return state


























def scriptMain( **kw ):

    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']

    UpdateSession.open()
    device = AllianceFramework.get().createCell( 'nonUnitCapacitor' )
    device.setTerminal( True )

    bottomPlate_net = Net.create( device, 'b' )
    bottomPlate_net.setExternal( True )  
    b = device.getNet("b")
    doBreak( 1, 'Done building bottomPlate')

    topPlate_net    = Net.create( device, 't' )
    topPlate_net.setExternal( True )  
    t = device.getNet("t")
    doBreak( 1, 'Done building tNet')

    if editor:
        UpdateSession.close(        )
        editor.setCell     ( device ) 
        editor.fit         (        )            
        UpdateSession.open (        )     

    nonUnitCapacitor = NonUnitCapacitor ( device, 'PIPCap', 'horizontal', 65, 50, [0,0] )
    nonUnitCapacitor.create(b,t)          
                      
    AllianceFramework.get().saveCell( device, Catalog.State.Views )

    return True

