#!/usr/bin/python

import sys                
import numpy
from   math             import sqrt, ceil
from   ..Hurricane      import *
from   ..CRL            import *
from   ..helpers.io     import ErrorMessage as Error
from   ..helpers        import trace
from   .                import getRules
from   .capacitorunit   import CapacitorUnit
from   .capacitormatrix import CapacitorStack


## Route a compact or a matrix of capacitors by connecting it to routing tracks.
#  For a fixed instance, only one type of capacitor is supported at a time,
#  either the Poly-Poly type or Metal-Metal in 350 nm AMS CMOS technology. 
#
#  The dummy mode is also supported.
#  The dummyRing mode is not yet supported.

def toDbU    ( l ): return DbU.fromPhysical( l, DbU.UnitPowerMicro )
def toPhY    ( l ): return DbU.toPhysical  ( l, DbU.UnitPowerMicro )

def doBreak( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()


class RouteCapacitorSingle ( CapacitorUnit ):

    rules = getRules()

    ## The constructor computes some of the class attributes and initialises others which
    #  will be computed later inside some of the class methods. 
    #  \param   device                The Hurricane AMS device into which the layout is drawn. 
    #  \param   capacitorInstance     Instance of the class CapacitorMatrix.
    #  \param   capacitor             A nested list containing a compact capacitor
    #                                 (in the first element) or elementary capacitors of
    #                                 a matrix of capacitors (each row in a list).
    #  \param   dummyMode             is \c "True" when dummy capacitor is drawn.
    #  \param   capacitorType         Can be MIM or PIP type capacitor.
    #  \param   abutmentBox           The abutment box of the compact or matrix capacitor.
    #  \param   routingTrackYCenter   A nested dictionary containing the ordinates of top and
    #                                 bottom ( including upper and lower) routing tracks. 
    #  \param   xPlateRLayerXCenter   A nested dictionary containing 
    #  \param   xPlateRLayer_width    A dictionary containing the widths of the top and bottom
    #                                 plates routing layers.
    #  \param   routingTrack_width    The width of a routing track. Is fixed according to
    #                                 technological parameters.
    #  \param   tracksNumbers         A dictionary containing the number of top and bottom tracks.
    #                                 The allowed maximum total number of tracks is two. 
    #  \param   topPlateWSpec         Routing specifications for the top plates.
    #  \param   bottomPlateWSpec      Routing specifications for the bottom plates.
    #
    #  \remarks An exception is raised if the entered routing specifications are invalid.
    #           Invalidity can be due to a wrong total number of tracks or bad wiring
    #           specifications of top and bottom tracks. More information about the valid
    #           specifications are given in \c function. 

    def __init__ ( self, capacitorInstance, capacitor, dummyMode = False, tracksNumbers = [1,1], topPlateWSpec = [1,0] , bottomPlateWSpec = [0,1] ):

        if capacitorInstance.matchingMode:
          raise Error( 1, [ 'RouteCapacitorSingle.__init__(): This class is only dedicated to route matrices equivalent to one capacitor.'
                          , 'In case of a matched matrix of capacitors, please use <RoutMatchedCapacitor>.' ] )

        self.device                = capacitorInstance.device
        self.capacitorInstance     = capacitorInstance
        self.capacitor             = capacitor
        self.dummyMode             = dummyMode
        self.capacitorType         = capacitorInstance.capacitorType
        self.matrixDim             = capacitorInstance.matrixDim
        self.abutmentBox           = capacitorInstance.abutmentBox
        self.abutmentBox_spacing   = self.capacitorInstance.getAbutmentBox_spacing ()
        self.bondingBox            = Box()
        self.capacitorType         = capacitorInstance.capacitorType
        self.nets                  = capacitorInstance.nets[0]   

        self.routingTracksXMinXMax = {} 
        self.routingTrackYCenter   = { "top": {}, "bottom": {} }
        self.xPlateRLayerXCenter   = { "top": [], "bottom": [], "bottomBorders" : [] }
        self.xPlateRLayer_width    = {}
        self.routingTrack_width    = 0

        if ( self.__isRoutingTracksNumberOK__( tracksNumbers ) ):

            self.tracksNumbers       = { "top" : tracksNumbers[0] ,"bottom" : tracksNumbers[1] }
            possibleRoutingSchemes = self.__setPossibleRoutingSchemes__( tracksNumbers )

            if dummyMode == True :

                if     self.connectToTopTracksOnly() :
                    self.topPlateWSpec  = [1,0]

                elif self.connectToBottomTracksOnly() : 
                    self.topPlateWSpec  = [0,1]

                else : 
                    self.topPlateWSpec  = [1,1] 

                self.bottomPlateWSpec   = self.topPlateWSpec

            elif ( dummyMode == False and  self.__isWiringSpecOK__(topPlateWSpec, bottomPlateWSpec, possibleRoutingSchemes) ) :
                self.topPlateWSpec      = topPlateWSpec
                self.bottomPlateWSpec   = bottomPlateWSpec

            else : raise Error(1,'__init__() : Invalid routing specifications in terms of number, format or routing scheme.') 

        else : raise Error(1,' __init__() : One routing track on one or both sides of the capacitor is allowed in dummy mode. Otherwise, routing tracks can be drawn one on each side or two on one side : (top trak number : %s, bottom track number : %s and dummy mode is %s). ' %( tracksNumbers[0], tracksNumbers[1], dummyMode )) #com verify the difference between a simple print and the dicitionary self.trackingNumber

        return

    ## Using functions calls and the entered parameters, This function draws the routing tracks, then, connects the compact or matrix of capacitors to routing tracks using routing layers and adequate cuts. \c rout function also initialises: cuts layers according to :
    #  - the capacitor type (ie., cuts2 if MIMCAP, cut1 if PIPCAP )
    #  - routing tracks layers according to the designer specifications.

    def route ( self, bbMode=False ):
        UpdateSession.open()

        bondingBox = {}
        self.setRules         ()
        self.computeDimensions( bbMode )

        if bbMode :
          bondingBox = self.computeLayoutDimensionsInbbMode()

        if not bbMode:
          routingTracksLayer = DataBase.getDB().getTechnology().getLayer("metal2")
          bottomPlateRLayer  = CapacitorUnit.getLayers( self )["bottomPlateRLayer"]
          topPlateRLayer     = bottomPlateRLayer

          if self.capacitorType == 'MIMCap':
            topbottomCutLayer = DataBase.getDB().getTechnology().getLayer("cut2")
          elif self.capacitorType == 'PIPCap':
            topbottomCutLayer = DataBase.getDB().getTechnology().getLayer("cut1")
          else:
            raise Error( 1, 'RouteCapacitorSingle.route(): Unsupported capacitor type "{0}".'.format( self.capacitorType ))

          self.drawRoutingTracks ( routingTracksLayer )
          self.drawPlatesVRLayers( 'topPlate'                 , topPlateRLayer       , self.topPlateWSpec   , self.xPlateRLayerXCenter["top"   ], self.xPlateRLayer_width["top"    ] )
          self.drawPlatesVRLayers( 'bottomPlate'              , bottomPlateRLayer    , self.bottomPlateWSpec, self.xPlateRLayerXCenter["bottom"], self.xPlateRLayer_width["bottom" ] )
          self.drawCuts          ( self.nets[0], 'topPlate'   , self.topPlateWSpec   , topbottomCutLayer    , self.xPlateRLayer_width ["top"   ], self.xPlateRLayerXCenter["top"   ] )
          self.drawCuts          ( self.nets[1], 'bottomPlate', self.bottomPlateWSpec, topbottomCutLayer    , self.xPlateRLayer_width ["bottom"], self.xPlateRLayerXCenter["bottom"] )

        ab = self.device.getAbutmentBox()
        trace( 101, '\tAB before tracks: {0}\n'.format( ab ))
        trace( 101, '\tBottom track numbers: {0}\n'.format( self.tracksNumbers['bottom'] ))
        dyTop = self.tracksNumbers['top'   ] * self.hpitch
        dyBot = self.tracksNumbers['bottom'] * self.hpitch
        self.device.setAbutmentBox( ab.inflate( 0, dyBot, 0, dyTop ) )
        trace( 101, '\tAB after tracks: {0}\n'.format( ab ))

        UpdateSession.close()

        return bondingBox



    ## Builds and returns a dictionary of the needed technological rules to route
    #  the capacitor.
    #  The significant rules are :
    #  - the minimum spacing between the routing tracks according to their metal layer,
    #  - the minimum width of a plate, a cut or a routing metal,
    #  - the minimum width, height and spacing between the cuts on the routing track.
    #  - etc.  
    #
    #  At the exception of the minimum spacing between routing tracks, every rule has
    #  two possible values according to the capacitor type.
    #  \remarks An exception is raised if the entered capacitor type is unsupported.

    def setRules ( self ):
        CapacitorUnit.setRules( self )    
        self.minSpacing_routingTrackMetal = RouteCapacitorSingle.rules.minSpacing_metal2

        if self.capacitorType == 'MIMCap' : 
          self.minHeight_routingTrackcut  = RouteCapacitorSingle.rules.minWidth_cut2
          self.minSpacing_routingTrackcut = RouteCapacitorSingle.rules.minSpacing_cut2
          self.minWidth_routingTrackcut   = RouteCapacitorSingle.rules.minWidth_cut2
        elif self.capacitorType == 'PIPCap':
          self.minHeight_routingTrackcut  = RouteCapacitorSingle.rules.minWidth_cut1
          self.minSpacing_routingTrackcut = RouteCapacitorSingle.rules.minSpacing_cut1
          self.minWidth_routingTrackcut   = RouteCapacitorSingle.rules.minWidth_cut1

        return


    ## Checks if the wiring specifications are compatible with the possible routing schemes. 
    #  \return  \c "True" if all conditions are satisfied.
    #  \param   topWiringSpec           The desired connection of the top plate. 
    #  \param   bottomWiringSpec        The desired connection of the top plate. 
    #  \param   possibleRoutingSchemes  A list of the possible connections computed according
    #                                   to routing tracks specifications. 

    def __isWiringSpecOK__ ( self, topPlateWiringSpec, bottomPlateWiringSpec, possibleRoutingSchemes ): 

        state = False
        if len(topPlateWiringSpec ) == len( bottomPlateWiringSpec ) == 2 and [ topPlateWiringSpec, bottomPlateWiringSpec ] in possibleRoutingSchemes :
            state = True

        else : raise Error(1, '__isWiringSpecOK__() : Invalid wiring specifications for top and/or bottom plates, %s and/or %s, respectively. The possible wiring schemes are : %s. ' %(topPlateWiringSpec, bottomPlateWiringSpec, possibleRoutingSchemes) )

        return state



    ## Builds and retuns a list containing the possible routing schemes according to routing tracks specifications. All the possibilities are summarized in Table 1. 
    #  \param   topTrackNumber           The specified number of top routing tracks. Two tracks are defined : the lower and upper ones. Considering a maximum number of top and bottom tracks equal to two, \c topTracksNumber takes 0, 1, 2 when \c bottomTrackNumber is equal to 2, 1, 0, respectively. 
    #  \param   bottomTrackNumber        The specified number of bottom routing tracks. The same rules and specifications as top tracks apply to bottom tracks.
    #  \param   possibleRoutingSchemes   A list of the possible connections computed according to routing tracks specifications. 

    def __setPossibleRoutingSchemes__( self, tracksNumbers ):

        [topTrackNumber , bottomTrackNumber] = [tracksNumbers[0] , tracksNumbers[1]]
        possibleRoutingSchemes = None

        if self.dummyMode == False: possibleRoutingSchemes = [ [[0,1],[1,0]], [[1,0],[0,1]] ]

        return possibleRoutingSchemes



    ## Checks if the tracks numbers are valid. The requirements which must be satifsied are : 
    #  - A maximum number of total tracks equal to two (ie., the sum of bottom tracks number and top tracks number must be equal to 2). 
    #  - The specified numbers for top and bottom tracks belong to the set \c {0,1,2}. Therefore, if the sum is equal to two and one or both numbers are not in {0,1,2}, this is considered as invalid. 
    #  \return \c "True" if all conditions are satisfied.
    #  \param   topTrackNumber   The specified number for top tracks.
    #  \param   topTrackNumber   The specified number for bottom tracks.
    #  \throw   <object>         Wrong values for routing tracks 
    def __isRoutingTracksNumberOK__ ( self, tracksNumbers ): 

        [topTrackNumber , bottomTrackNumber] = [tracksNumbers[0] , tracksNumbers[1]]
        state = False
        if   self.dummyMode == True:
            if ( bottomTrackNumber == 1 and topTrackNumber == 0 or bottomTrackNumber == 0 and topTrackNumber == 1  or bottomTrackNumber == 1 and topTrackNumber == 1 ) : 
                state = True

        elif self.dummyMode == False:
            if ( bottomTrackNumber == 1 and topTrackNumber == 1 or bottomTrackNumber == 0 and topTrackNumber == 2 or bottomTrackNumber == 2 and topTrackNumber == 0  ) :
                state = True

        else : raise Error( 1, '__isRoutingTracksNumberOK__() : Wrong values for routing tracks "%s , %s". ' %(bottomTrackNumber, topTrackNumber) ) #com test this

        return state



    ## Computes relevant attributes (layout parameters) by calling two functions to compute properties and dimensions of :
    #  - the routing layers of top and bottom plates
    #  - the routing tracks
    def computeDimensions( self, bbMode ):

        self.computeRLayersDimensions( bbMode )

        routingTracksLayer = DataBase.getDB().getTechnology().getLayer("metal2")
        bottomPlateRLayer  = CapacitorUnit.getLayers( self )["bottomPlateRLayer"]
        topPlateRLayer     = bottomPlateRLayer

        ab            = self.device.getAbutmentBox()
       #plateSpacing1 = self.minSpacing_botPlate
       #plateSpacing2 = self.hpitch - (self.metal2Width + self.routingTrack_width)/2
       #plateSpacing  = max( plateSpacing1, plateSpacing2 ) + self.metal2Width/2
        plateSpacing  = self.minSpacing_botPlate + self.metal2Width/2
        trace( 101, '\tAB height before plate inflate: {0}\n'.format( DbU.getValueString(ab.getHeight()) ))
        trace( 101, '\tminSpacing_botPlate: {0}\n'.format( DbU.getValueString(self.minSpacing_botPlate) ))
        trace( 101, '\tplateSpacing: {0}\n'.format( DbU.getValueString(plateSpacing) ))
        ab.inflate( 0, plateSpacing )
        self.device.setAbutmentBox( ab )
        trace( 101, '\tAB height after plate inflate: {0} {1}\n'.format( DbU.getValueString(ab.getHeight()), ab.getHeight() ))
        
        height        = ab.getHeight()
        heightAdjust  = height % (2*self.hpitch)
        if heightAdjust:
          trace( 101, '\tAB height before V-adjust: {0}\n'.format( DbU.getValueString(ab.getHeight()) ))
          heightAdjust = 2*self.hpitch - heightAdjust
          trace( 101, '\tAB height adjust: {0}\n'.format( DbU.getValueString(heightAdjust) ))
          self.device.setAbutmentBox( ab.inflate( 0, heightAdjust/2 ))
          trace( 101, '\tAB after V-adjust: {0}\n'.format( ab ))
          trace( 101, '\tAB height after V-adjust: {0}\n'.format( DbU.getValueString(ab.getHeight()) ))

        width = ab.getWidth()
        widthAdjust = width % (2*self.vpitch)
        if widthAdjust:
          widthAdjust = 2*self.vpitch - widthAdjust
          self.device.setAbutmentBox( ab.inflate( widthAdjust/2, 0 ))

        self.abutmentBox = ab
        self.computeHRoutingTrackDimensions()

        return



    def computeRLayersDimensions ( self, bbMode ):
        self.routingTrack_width = 2 * self.minEnclo_routingTrackMetal_cut \
                                + self.minWidth_routingTrackcut

        if not bbMode : 
          self.xPlateRLayer_width["top"] = self.capacitor.getTopPlateRLayerWidth() if self.capacitorInstance.matrixDim.values() == [1,1] else self.capacitor[0][0].getTopPlateRLayerWidth() 

          if self.capacitorInstance.__isUnitCap__() :
            self.computeRLayersDimensionsCompactCap  ()
          else:
            self.computeRLayersDimensionsMatrixCap()
        return


    def computeRLayersDimensionsCompactCap ( self ):
        self.bordersRLayerXMin                    = [ self.capacitor.getBottomPlateLeftCutXMin(), self.capacitor.getBottomPlateRightCutXMin() ]
        self.xPlateRLayer_width ["bottomBorders"] =   self.capacitor.getBotPlateRLayerWidth            ()
        self.xPlateRLayer_width ["bottom"       ] = []
        self.xPlateRLayerXCenter["bottom"       ] = []
        self.xPlateRLayerXCenter["top"          ].append( self.capacitor.getTopPlateRLayerXCenter      () )
        self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor.getBotPlateLeftRLayerXCenter  () ) 
        self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor.getBotPlateRightRLayerXCenter () ) 
        return


    def computeRLayersDimensionsMatrixCap( self ):
        if  self.capacitorInstance.matrixDim["columns"] > 1 : 
            self.xPlateRLayer_width["bottom"]  = ( self.capacitor[0][1].getBotPlateLeftRLayerXMax() - self.capacitor[0][0].getBotPlateRightRLayerXMin() )
            self.bordersRLayerXMin = [ self.capacitor[0][0].getBottomPlateLeftCutXMin(), self.capacitor[0][-1].getBottomPlateRightCutXMin() ]

        elif self.capacitorInstance.matrixDim["columns"] == 1 : 
            self.bordersRLayerXMin = [ self.capacitor[0][0].getBottomPlateLeftCutXMin(), self.capacitor[0][0].getBottomPlateRightCutXMin() ]

        else : raise Error( 1, 'computeRLayersDimensionsMatrixCap() : Negative number of columns in the matrix "%s".' % self.capacitorInstance.matrixDim["columns"] )

        self.xPlateRLayer_width["bottomBorders"] = self.capacitor[0][0].getBotPlateRLayerWidth()

        for i in range( 0, self.matrixDim["columns"] ):
            self.xPlateRLayerXCenter["top"].append( self.capacitor[0][i].getTopPlateRLayerXCenter() )
        for i in range( 0,self.matrixDim["columns"] -1 ):
            self.xPlateRLayerXCenter["bottom"].append( self.capacitor[0][i].getBotPlateRightRLayerXMin() + self.xPlateRLayer_width["bottom"]/2 )

        if self.capacitorInstance.matrixDim["columns"] > 1 :
            self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor[0][ 0].getBotPlateLeftRLayerXCenter () ) 
            self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor[0][-1].getBotPlateRightRLayerXCenter() ) 
        else :
            self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor[0][0].getBotPlateLeftRLayerXCenter  () ) 
            self.xPlateRLayerXCenter["bottomBorders"].append( self.capacitor[0][0].getBotPlateRightRLayerXCenter () ) 


        return

    ## Actual function that computes routing tracks dimensions and positions. 

    def computeHRoutingTrackDimensions( self ):
        trace( 101, ',+', '\tcomputeHRoutingTrackDimensions(): ab {0}\n'.format(self.device.getAbutmentBox()) )

        self.routingTracksXMinXMax = { "XMin" : self.abutmentBox.getXMin()
                                     , "XMax" : self.abutmentBox.getXMax() }
        
        if self.dummyMode:
          if self.connectToTopTracksOnly():
            yTL = self.abutmentBox.getYMax()

            self.routingTrackYCenter["top"]["lower"] = yTL

            self.topLowerTrackDict = { "YMin" : yTL - self.routingTrack_width/2
                                     , "YMax" : yTL + self.routingTrack_width/2 }

          elif self.connectToBottomTracksOnly():
            yBU = self.abutmentBox.getYMin()

            self.routingTrackYCenter["bottom"]["upper"] = yBU

            self.bottomUpperTrackDict = { "YMin" : yBU - self.routingTrack_width/2
                                        , "YMax" : yBU + self.routingTrack_width/2 }
          else:
            yTL = self.abutmentBox.getYMax()
            yBU = self.abutmentBox.getYMin()

            self.routingTrackYCenter["top"   ]["lower"] = yTL
            self.routingTrackYCenter["bottom"]["upper"] = yBU

            self.topLowerTrackDict    = { "YMin" : yTL - self.routingTrack_width/2
                                        , "YMax" : yTL + self.routingTrack_width/2 }
            self.bottomUpperTrackDict = { "YMin" : yBU - self.routingTrack_width/2
                                        , "YMax" : yBU + self.routingTrack_width/2 }

        else:
          if self.connectToTopTracksOnly() : 
            yTL = self.abutmentBox.getYMax()
            yTU = yTL + self.hpitch

            self.routingTrackYCenter["top"]["lower"] = yTL
            self.routingTrackYCenter["top"]["upper"] = yTU

            self.topLowerTrackDict = { "YMin" : yTL - self.routingTrack_width/2
                                     , "YMax" : yTL + self.routingTrack_width/2 }
            self.topUpperTrackDict = { "YMin" : yTU - self.routingTrack_width/2
                                     , "YMax" : yTU + self.routingTrack_width/2}

          elif self.connectToBottomTracksOnly(): 
            yBU = self.abutmentBox.getYMin()
            yBL = yBU - self.hpitch

            self.routingTrackYCenter["bottom"]["upper"] = yBU
            self.routingTrackYCenter["bottom"]["lower"] = yBL

            self.bottomUpperTrackDict = { "YMin" : yBU - self.routingTrack_width/2
                                        , "YMax" : yBU + self.routingTrack_width/2 }
            self.bottomLowerTrackDict = { "YMin" : yBL - self.routingTrack_width/2
                                        , "YMax" : yBL + self.routingTrack_width/2 }

          elif self.connectToTopAndBottomTracks(): 
            yTL = self.abutmentBox.getYMax()
            yBU = self.abutmentBox.getYMin()

            trace( 101, '\tyBU: {0}\n'.format( DbU.getValueString(yBU) ))

            self.routingTrackYCenter["top"   ]["lower"] = yTL
            self.routingTrackYCenter["bottom"]["upper"] = yBU

            self.topLowerTrackDict    = { "YMin" : yTL - self.routingTrack_width/2
                                        , "YMax" : yTL + self.routingTrack_width/2 }
            self.bottomUpperTrackDict = { "YMin" : yBU - self.routingTrack_width/2
                                        , "YMax" : yBU + self.routingTrack_width/2 }

          else:
            raise Error( 1, [ 'RouteCapacitorSingle.computeHRoutingTrackDimensions(): Wrong number or values for routing tracks.'
                            , 'Top : {0}, bottom : {1}.'.format( self.tracksNumbers["top"], self.tracksNumbers["bottom"]) ] )

        trace( 101, '-' )
        return


    def computeLayoutDimensionsInbbMode( self ):

        bondingBoxDict            = {}
        bondingBoxDict["width"  ] =  self.routingTracksXMinXMax["XMax"] - self.routingTracksXMinXMax["XMin"] 
        bondingBoxDict["XMin"   ] =  self.routingTracksXMinXMax["XMin"]

        if   self.dummyMode == True :

            if   self.connectToTopTracksOnly    () : 
                bondingBoxDict["height" ] = self.topLowerTrackDict["YMax"] - self.abutmentBox.getYMin()
                bondingBoxDict["YMin"   ] = self.abutmentBox.getYMin()

            elif self.connectoToBottomTracksOnly() : 
                bondingBoxDict["height" ] = self.abutmentBox.getYMax()     - self.bottomUpperTrackDict["YMin"]
                bondingBoxDict["YMin"   ] = self.bottomUpperTrackDict["YMin"]

            else : 
                bondingBoxDict["height" ] = self.topLowerTrackDict["YMax"] - self.abutmentBox.getYMin()
                bondingBoxDict["YMin"   ] = self.bottomUpperTrackDict["YMin"]

        elif self.dummyMode == False:

            if   self.connectToTopTracksOnly    () : 
                bondingBoxDict["height" ] = self.topUpperTrackDict["YMax"] -  self.abutmentBox.getYMin()
                bondingBoxDict["YMin"   ] = self.abutmentBox.getYMin()

            elif self.connectoToBottomTracksOnly() :  
                bondingBoxDict["height" ] = self.abutmentBox.getYMax()     - self.bottomLowerTrackDict["YMin"]
                bondingBoxDict["YMin"   ] = self.bottomLowerTrackDict["YMin"]

            else: 
                bondingBoxDict["height" ] = self.topLowerTrackDict["YMax"] - self.bottomUpperTrackDict["YMin"]
                bondingBoxDict["YMin"   ] = self.bottomUpperTrackDict["YMin"]

        else : raise Error( 1, 'computeLayoutDimensionsInbbMode() : The dummy mode must be either "True" or "False", "%s". ' % self.dummyMode )        

        bondingBoxDict["surface"] = bondingBoxDict["width"]*bondingBoxDict["height"]

        self.bondingBox = Box( bondingBoxDict["XMin"], bondingBoxDict["YMin"], bondingBoxDict["XMin"] + bondingBoxDict["width"] , bondingBoxDict["YMin"] + bondingBoxDict["height"] )

        return bondingBoxDict


    ## Draws routing tracks, above and/or below the capacitor. A maximum total number of two tracks is drawn. In dummy mode, one track is drawn.  
    # \param routingTracksLayer  Layer of the routing track.
    # \remark All routing tracks, top and bottom (upper and lower), are drawn using the same layer.

    def drawRoutingTracks( self , routingTracksLayer  ):

        self.drawTopOrBottomRoutingTracks ( "top"   , routingTracksLayer ) 
        self.drawTopOrBottomRoutingTracks ( "bottom", routingTracksLayer ) 

        return


    def drawTopOrBottomRoutingTracks ( self, tracksPosition, routingTracksLayer ) :
        trace( 101, ',+', '\tRouteCapacitorSingle.drawTopOrBottomRoutingTracks()\n' )
        if tracksPosition in ["top","bottom"] :
          attribut = [ "lower", "upper" ]            
          nets     = self.__setNetsDistributionHRTs__()
          for i in range( 0, self.tracksNumbers[tracksPosition] ):
            index               = i   if tracksPosition == "top" else i-1
            netindex            = i-1 if tracksPosition == "top" else i
            trace( 101, '\ttrackPos={0}, index={1}, attribute={2}\n'.format( tracksPosition
                                                                           , index
                                                                           , attribut[index] ))
            routingtrackYCenter = self.routingTrackYCenter[tracksPosition][attribut[index]] 
            horizontal          = Horizontal.create( nets[netindex]
                                                   , routingTracksLayer
                                                   , routingtrackYCenter
                                                   , self.routingTrack_width
                                                   , self.routingTracksXMinXMax["XMin"]
                                                   , self.routingTracksXMinXMax["XMax"] )
            NetExternalComponents.setExternal( horizontal )
            trace( 101, '\t{0}\n'.format( horizontal ))
        else:
          raise Error( 1, 'RouteCapacitorSingle.drawOneRoutingTrack(): The track position must be either "top" or "bottom" ("{0}").'.format(tracksPosition))
        trace( 101, '-' )
        return


    def __setNetsDistributionHRTs__( self ):
        trace( 101, ',+', '\tRouteCapacitorSingle.__setNetsDistributionHRTs__()\n' )
        if self.dummyMode:
          trace( 1010, '\tDummy mode\n' )
          netsDistribution = self.nets
        else:
          if (self.topPlateWSpec, self.bottomPlateWSpec) == ([1,0], [0,1]):
            netsDistribution = self.nets
          else: 
            netsDistribution = [ self.nets[1], self.nets[0] ]
        trace( 101, '\tnetsDistribution = [ {0}, {1} ]\n'.format( netsDistribution[0].getName()
                                                                , netsDistribution[1].getName() ))
        trace( 101, '-' )
        return netsDistribution



    ##  Draws the routing layers connecting top and bottom plates to the associated routing track.
    #  \param    Plate                       The capacitor's plate to be routed (ie., top, bottom).
    #  \paramx   PlateRLayer               Routing layer.
    #  \param    PlateWSpec            Connection specifications of the plate.
    #  \param    xPlateRLayerXCenter   Horizontal position of the routing layer. 
    #  \param    xPlateRLayer_width    Width of the routing layer. 
    #  \throw    < plate-name    >     \c Undefined \c plate
    #  \throw    < specification >     \c Invalid \c routing \c specifications

    def drawPlatesVRLayers( self, Plate, xPlateRLayer, PlateWSpec, xPlateRLayerXCenter, xPlateRLayer_width ):

        if  self.capacitorInstance.__isUnitCap__() :
            [firstElementInCapacitor , lastElementInCapacitor] = [self.capacitor , self.capacitor]
        elif not( self.capacitorInstance.__isUnitCap__() ):
            [firstElementInCapacitor , lastElementInCapacitor] = [self.capacitor[0][0] , self.capacitor[-1][0]]

        [ doTop, doBottom ] = [1, 0] if Plate == 'topPlate' else [0, 1]

        if   ( Plate == 'topPlate' )  :
            YMinDict = {
                        "toTopToUpper"    : firstElementInCapacitor.getTopPlateRLayerYMin(),
                        "toTopToLower"    : firstElementInCapacitor.getTopPlateRLayerYMin(), 
                        "toBottomToUpper" : lastElementInCapacitor.getTopPlateRLayerYMax(), 
                        "toBottomToLower" : lastElementInCapacitor.getTopPlateRLayerYMax()
                        }
            net = self.nets[0]
            xMetalXCenter = self.xPlateRLayerXCenter["top"]

        elif ( Plate == 'bottomPlate' ) :
            YMinDict = {
                        "toTopToUpper"    : firstElementInCapacitor.getBotPlateRLayerYMin(),
                        "toTopToLower"    : firstElementInCapacitor.getBotPlateRLayerYMin(), 
                        "toBottomToUpper" : lastElementInCapacitor.getBotPlateRLayerYMax(), 
                        "toBottomToLower" : lastElementInCapacitor.getBotPlateRLayerYMax()
                        }
            net = self.nets[1]
            xMetalXCenter = self.xPlateRLayerXCenter["bottom"]

        else : raise Error( 1, 'drawPlatesVRLayers() : Undefined plate, "%s".' % Plate )

        if   ( self.connectToTopTracksOnly   ()   and self.connectToUpper( PlateWSpec ) ) :
            [ dySource, dyTarget ] = [ YMinDict["toTopToUpper"   ] , self.topUpperTrackDict["YMax"] ]        

        elif ( self.connectToTopTracksOnly   ()   and   self.connectToLower( PlateWSpec ) ) or ( self.connectToTopAndBottomTracks () and self.connectToUpper( PlateWSpec ) ) :
            dyTarget               = self.topLowerTrackDict   ["YMax"]        
            dySource               = self.bottomUpperTrackDict["YMin"] if self.dummyMode == True and self.connectToTopAndBottomTracks () else YMinDict["toTopToLower"   ]
        elif ( self.connectToBottomTracksOnly()   and   self.connectToUpper( PlateWSpec ) ) or ( self.connectToTopAndBottomTracks () and self.connectToLower( PlateWSpec ) ) :
            [ dySource, dyTarget ] = [ YMinDict["toBottomToUpper"] , self.bottomUpperTrackDict["YMin"] ]        

        elif ( self.connectToBottomTracksOnly()   and self.connectToLower( PlateWSpec ) ) :
            [ dySource, dyTarget ] = [ YMinDict["toBottomToLower"] , self.bottomLowerTrackDict["YMin"] ]

        else : raise Error( 1, 'drawPlatesVRLayers() : Invalid routing specifications "%s".' % PlateWSpec )

        for i in range( 0, self.matrixDim["columns"] - doBottom ):
            Vertical.create ( net, xPlateRLayer, xPlateRLayerXCenter[i], xPlateRLayer_width, dySource, dyTarget ) 

        if doBottom :  
            for i in range( 0,2):
                Vertical.create ( self.nets[1], xPlateRLayer , self.xPlateRLayerXCenter["bottomBorders"][i], self.xPlateRLayer_width["bottomBorders"], dySource, dyTarget ) 

        return





    ## Draws one or multiple cuts between a routing track and a routing layer to connect the capacitor plate to the track. The function supports cuts for top and bottom plates. First, using wiring specifications, he position of the cuts is identified. Second, the maximum nupmber of cuts is computed, then, its enclosure in the routing layer is adjusted. Third, the cuts are iteratively drawn on every intersection between the routing track and the plate's routing layer. 
    #  \remark  Since in the special case of bottom plate, routing layers on matrix borders are thinner than the intermediate ones, two extra steps are excecuted to draw cuts connecting border routing layers to the routing track. The two steps are computing the maximum number of cuts, which is lower than intermediate cuts, and ajusting its enclosure. 
    #  \throw   <Invalid-specifictions >  \c Not \c possible \c to \c compute cuts vertical position due to invalid routing specifications                 
    #  \remak                              The number of cuts is maximized according to the track's width.
    #  \param   net                        Net of the Hurricane Device to which the cuts will be connected.
    #  \param   Plate                           Capacitor's plate, top or bottom.
    #  \param   PlateWSpec                 Wiring specifications of the plate. It is useful to identify the track on which the cuts are to draw.
    #  \param   layer                      Cut's layer.
    #  \param   xPlateRLayer_width         Width of the plate's routing layer. It is useful to compute the maximum number of cuts.
    #  \param   xPlateRLayerXCenter        Hrizontal position of the plate's routing layer, which also defines the cut's position.

    def drawCuts( self, net, Plate, PlateWSpec, layer, xPlateRLayer_width, xPlateRLayerXCenter ):

        [ doTop, doBottom ] = [1, 0] if Plate == 'topPlate' else [0, 1]
        cutsYCenter = self. __setCutsYCenter__( PlateWSpec ) 

        if not( self.capacitorInstance.__isUnitCap__() ) or ( self.capacitorInstance.__isUnitCap__() and doTop ) :          

           #print("xPlateRLayer_width",xPlateRLayer_width)
            cutsNumber           =   CapacitorUnit.cutMaxNumber( self, xPlateRLayer_width, self.minWidth_routingTrackcut, self.minSpacing_routingTrackcut, self.minEnclo_routingTrackMetal_cut )
            enclosure_RLayer_cut = ( xPlateRLayer_width - cutsNumber*self.minWidth_routingTrackcut - ( cutsNumber - 1 )*self.minSpacing_routingTrackcut ) / 2

            for i in range( 0, self.matrixDim["columns"] - doBottom ):    
                cutXCenter = xPlateRLayerXCenter[i] - xPlateRLayer_width/2 + enclosure_RLayer_cut        + self.minWidth_routingTrackcut/2
                CapacitorUnit.cutLine( self, net, layer, cutXCenter, cutsYCenter, self.minWidth_routingTrackcut, self.minHeight_routingTrackcut, self.minSpacing_routingTrackcut, cutsNumber , 'horizontal' )
                if self.dummyMode == True and self.connectToTopAndBottomTracks() :
                    CapacitorUnit.cutLine( self, net, layer, cutXCenter, self.routingTrackYCenter["bottom"]["upper"], self.minWidth_routingTrackcut, self.minHeight_routingTrackcut, self.minSpacing_routingTrackcut, cutsNumber , 'horizontal' )

        if doBottom :  
            borderscutsNumber    =   CapacitorUnit.cutMaxNumber( self, self.xPlateRLayer_width["bottomBorders"], self.minWidth_routingTrackcut, self.minSpacing_routingTrackcut, self.minEnclo_routingTrackMetal_cut )
            enclosure_RLayer_cut = ( self.xPlateRLayer_width["bottomBorders"] - borderscutsNumber*self.minWidth_routingTrackcut - ( borderscutsNumber - 1 )*self.minSpacing_routingTrackcut ) / 2

            for i in range( 0,2):
                CapacitorUnit.cutLine( self, net, layer, self.xPlateRLayerXCenter["bottomBorders"][i], cutsYCenter, self.minWidth_routingTrackcut, self.minHeight_routingTrackcut, self.minSpacing_routingTrackcut, borderscutsNumber , 'horizontal' )
                if self.dummyMode == True and self.connectToTopAndBottomTracks() :
                    CapacitorUnit.cutLine( self, net, layer, self.xPlateRLayerXCenter["bottomBorders"][i], self.routingTrackYCenter["bottom"]["upper"], self.minWidth_routingTrackcut, self.minHeight_routingTrackcut, self.minSpacing_routingTrackcut, borderscutsNumber , 'horizontal' )
        return



    def __setCutsYCenter__( self, PlateWSpec ): 
        
        if   ( self.connectToTopTracksOnly   () and self.connectToLower( PlateWSpec ) )  or ( self.connectToTopAndBottomTracks() and self.connectToUpper( PlateWSpec ) ) :
            cutsYCenter = self.routingTrackYCenter["top"]["lower"]

        elif ( self.connectToTopTracksOnly   () and self.connectToUpper( PlateWSpec ) ) :
            cutsYCenter = self.routingTrackYCenter["top"]["upper"] 

        elif ( self.connectToBottomTracksOnly() ) and ( self.connectToUpper( PlateWSpec ) ) or ( self.connectToTopAndBottomTracks() and self.connectToLower( PlateWSpec ) ) :
            cutsYCenter = self.routingTrackYCenter["bottom"]["upper"]

        elif ( self.connectToBottomTracksOnly() and self.connectToLower( PlateWSpec ) ) :
            cutsYCenter = self.routingTrackYCenter["bottom"]["lower"] 

        else : raise Error( 1, '__setCutsYCenter__() : Not possible to compute cuts vertical position due to invalid routing specifications "%s".' % PlateWSpec )


        return cutsYCenter



    ## \return \c True if the plate is to be connected to one of the two top tracks.
    #  \param WiringSpecChain Wiring specifications of a capacitor's top or bottom plate 
    def connectToTopTracksOnly    (self) :  return  True  if  self.tracksNumbers["top"] == 2 and self.tracksNumbers["bottom"] == 0 and self.dummyMode == False or self.tracksNumbers["top"] == 1 and self.tracksNumbers["bottom"] == 0 and self.dummyMode == True  else  False


    ## \return \c True if the plate is to be connected to one of the two bottom  tracks.
    #  \param WiringSpecChain Wiring specifications of a capacitor's top or bottom plate 
    def connectToBottomTracksOnly (self) :  return  True  if  self.tracksNumbers["top"] == 0 and self.tracksNumbers["bottom"] == 2 and self.dummyMode == False or  self.tracksNumbers["top"] == 0 and self.tracksNumbers["bottom"] == 1 and self.dummyMode == True else False


    def connectToTopAndBottomTracks( self )     :  return  True  if  self.tracksNumbers["top"] == 1 and self.tracksNumbers["bottom"] == 1  else  False


    ## \return \c True if the plate is to be connected to the upper track of top or bottom tracks.
    #  \param WiringSpecChain Wiring specifications of a capacitor's top or bottom plate 
    def connectToUpper( self, plateWiringSpec ) :  return  True  if  plateWiringSpec    [1]    == 1                                        else  False


    ## \return \c True if the plate is to be connected to the lower track of top or bottom tracks.
    #  \param WiringSpecChain Wiring specifications of a capacitor's top or bottom plate 
    def connectToLower( self, plateWiringSpec ) :  return  True  if  plateWiringSpec    [0]    == 1                                         else  False



def scriptMain( **kw ):

    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']

    UpdateSession.open()
    Device = AllianceFramework.get().createCell( 'capacitor' )
    Device.setTerminal( True )

    bottomPlate_net0 = Net.create( Device, 'b0' )
    bottomPlate_net0.setExternal( True )  
    b0  = Device.getNet("b0")
    doBreak( 1, 'Done building bottomPlate')

    topPlate_net0    = Net.create( Device, 't0' )
    topPlate_net0.setExternal( True ) 
    t0     = Device.getNet("t0")
    doBreak( 1, 'Done building topPlate')


    if editor:
        UpdateSession.close()
        editor.setCell( Device )
        editor.fit()  
        UpdateSession.open()

    nets = [[t0,b0]] 

## A matrix of unit capacitors (all are active or all are dummy capacitors) 

#    capacitance = [1600]
#    capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets,unitCap = 400)
#    capacitor  = capacitorInstance.create()
#
#    routedCap  = RouteCapacitorSingle( capacitorInstance, capacitor, dummyMode = True, tracksNumbers = [1,0] )  
#    routedCap  = RouteCapacitorSingle( capacitorInstance, capacitor, tracksNumbers = [2,0], topPlateWSpec = [0,1] , bottomPlateWSpec = [1,0] ) 
#    routedCap  = RouteCapacitorSingle( capacitorInstance, capacitor, dummyMode = False , tracksNumbers = [1,1], topPlateWSpec = [0,1] , bottomPlateWSpec = [1,0]) 

## Unit capacitor ( an active capacitor )
    capacitance = [600]
    capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets,unitCap = 600)
    capacitor  = capacitorInstance.create()
    routedCap  = RouteCapacitorSingle( capacitorInstance, capacitor, topPlateWSpec = [0,1] , bottomPlateWSpec = [1,0] ) 

## Unit capacitor ( a dummy  capacitor )
#    capacitance = [600]
#    capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets,unitCap = 600)
#    capacitor  = capacitorInstance.create()
#    routedCap  = RouteCapacitorSingle( capacitorInstance, capacitor, dummyMode = True, tracksNumbers = [1,0] ) 


    bondingBox = routedCap.route()    

    AllianceFramework.get().saveCell( Device, Catalog.State.Views )

    return True

