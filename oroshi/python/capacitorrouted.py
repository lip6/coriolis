#!/usr/bin/python

import sys                
from   ..Hurricane        import *
from   ..CRL              import *
from   ..helpers.io       import ErrorMessage as Error
from   ..helpers          import staticInitialization, trace
from   .                  import getRules
from   .capacitorunit     import CapacitorUnit
from   .capacitormatrix   import CapacitorStack
from   .capacitorvrtracks import VerticalRoutingTracks


def toPhY    ( l ): return DbU.toPhysical  ( l, DbU.UnitPowerMicro )

def doBreak( level, message ):
    UpdateSession.close()
    Breakpoint.stop( level, message )
    UpdateSession.open()

staticInitialization( True )


## Routs two matched capacitors, C1 and C2, drawn in a capacitor matrix. Connections are put in place with reference to a given matching scheme. Elementary capacitor units are connected to horizontal and vertical routing tracks that represent top plates and bottom plates nets of C1 and C2 . Supported types of capacitors are Poly-Poly and Metal-Metal. Technologycal rules are provided by 350 nm AMS CMOS technology with three-four metal layers. Metal layers that are used for routing are placed similarly to horziontal-vertical (HV) symbolic Alliance CAD tool router, where horizontal metal channels are drawn in metal 2 and the vertical ones are in metal 3. Given a matrix of dimensions \f$ R*C \f$, the total number of vertical tracks is \f$ 2C+2 \f$ equivalent to \f$ C+1 \f$ couples, ensuring that every elementary capacitor is positioned between four vertical tracks, two from each side. In fact, every adjacent couple of these tracks represent top plates and bottom plates of C1 or C2 as shown in Figure 1.
#  \image html Layout.png "Layout" width=.1\linewidth
#  An elementary capacitor unit can be a part of C1 or C2 according to the matching scheme. However, to respect common-centroid layout specifications, for C1 and C2 to be equal, the matrix number of colums and number of rows must be both even. Addionnally, the number of elementary capacitors dedicated to C1 must be equal to those dedicated to C2. These two conditions are tested in one of the class methods. An exception is raised if at least one of the two is not respected.


class RoutMatchedCapacitor( VerticalRoutingTracks ):

    rules = getRules()


    ## A special method used to customize the class instance to an initial state in which :
    #  - the class attirbutes describing positions and dimensions of the layout are computed in dedicated class methods,
    #  - the attributes related to the capacitor matrix are copied from the \c CapacitorStack instance.
    #
    #  Position and dimensions attributes, also refered by layout variables,  in Figure 2, are defined below :  
    # \param   device                     The Hurricane AMS device onto which the layout is drawn. 
    # \param   capacitorInstance          Instance of \c CapacitorStack class.
    # \param   capacitor                  A nested list containing the matrix elements, which are \c CapacitorUnit objects. 
    # \param   matchingScheme             A nested list, with equal dimensions as \c capacitor attribute, containing assignements of matrix elementary units to C1 and C2, identified by 1 and 2, respectively. Therefore, \c self.matchingScheme content is a succession of 1 and 2 values, defined as \ capacitor identifiers. For example, given a matrix of dimensions 2x2, the matching scheme can be \f$ [ [1,2], [1,2] ] or [ [2,1], [2,1] ] \f$. The first sub-list dictates that the first elementary capacitor, \f$ C_{00} \f$. The second element \f$ C_{01} \f$ is affected to C2 and so on. An immediate and obvious consequence to this, is that an error is raised if \c self.matchingSchem and \c self.capacitor dimensions are not identical or if \c self.matchingScheme content is different from supported capacitor identifiers, '1' and '2'.
    #
    # \param   capacitorType              Supported types of capacitors are MIM and PIP only. An exception is raised otherwise.
    # \param   abutmentBox                The matrix's abutment box.
    # \param   matrxiDim                  The matrix dimensions, also equal to \c self.matchingScheme nested list dimensions.   
    # \param   abutmentBox_spacing        The spacing between elementary units in the matrix.
    #                                     It is computed in \c CapacitorStack and is reloaded
    #                                     in \c RoutMatchedCapacitor. \c self.abutmentBox_spacing
    #                                     includes, vertical routing tracks width and minimum
    #                                     allowed spacing between two adjacent ones.
    # \param   hRoutingLayer_width        The width of horizontal routing layers in metal 2, which connect capacitors plates to vertical routing tracks. 
    # \param   vRoutingTrack_width        The width of vertical routing tracks in metal 3, which connects identical nets together ( ie : bottom plates of C1, top plates of C2, bottom plates of C2 and top plates of C2 ). 
    # \param   hRoutingTrack_width        The width of horizontal routing tracks in metal 2, which connect identical vertical routing tracks together.  
    # \param   minSpacing_hRoutingTrack   Minimum spacing between horizontal routing tracks.
    #                                     Wide metal 2 specifications are considered since metal 2
    #                                     dimensions may exceed 10 \f$ m\f$. 
    # 
    # \remark For more information about wide metal specifications, refer to ENG-183_rev8.pdf technology manual. 
    #
    # \param   minimumPosition            The ordinate of the top plate's routing layer's
    #                                     bottom extremity after stretching. 
    # \param   maximumPosition            The ordinate of the top plate's routing layer's
    #                                     top extremity, also equivalent to the top plate's
    #                                     top extremity. 
    # \param   vRoutingTrackXCenter       A nested list of ordered dictionaries, with dimensions
    #                                     equal to \c self.matrixDim, containing abcissas of vertical
    #                                     routing tracks. All sub-lists' lengths are identical and
    #                                     are equal to 2. The first and second elements describe
    #                                     position of top plate track and bottom plate track,
    #                                     respectively.  For example, given a matrix of dimensions
    #                                     2x2, \c self.vRoutingTrackXCenter can be
    #                                     [[0, 2], [4,6], [8,10]] \f$ \mu m\f$.
    #                                     Elements of this nested list have particular indexing as
    #                                     described in Figure 2. 
    #
    # \param   hRoutingtrackYCenter       A nested dictonary containing two keys, \c topTracks
    #                                     and \c bottomTracks. Each key contains as value a dictionary
    #                                     describing centers' ordinates of four parallel horizontal
    #                                     tracks. The reason why four tracks are needed on top and
    #                                     bottom positions of the matrix is that four nets are used,
    #                                     two for every capacitor \c Ci, were \c i is in [1,2].        
    # \param   hRoutingLayerYCenter       A nested dicitonary containing two keys, \c top and
    #                                     \c bottom. Each key contains as value a dictionary
    #                                     describing centers' ordinates of horizontal routing layers. 
    # \param   vRoutingTrackDict          A dictionary of routing tracks top and bottom extremities ordinates. 
    # \param   topPlateStretching         Since not only the same metal 2 layer is used to draw top/bottom plates connections to vertical tracks but also the two plates are superimposed, the top plate's routing tracks is stretched. \c self.topPlateStretching is therefore the length added to top plate's routing layer in order to avoid short circuits between top and bottom plates routing to vertical tracks since the same metal is used for both.

    def __init__( self, vRTInstance ) :

        VerticalRoutingTracks.__init__( self, vRTInstance.capacitorInstance, vRTInstance.capacitor )

        if self.dummyRing == True :
            self.capacitor = [vRTInstance.capacitor[1][1:len(vRTInstance.capacitor[1])-1]]

            for i in range(2,self.matrixDim["rows"]+1):
                self.capacitor.append(vRTInstance.capacitor[i][1:len(vRTInstance.capacitor[1])-1])

            self.dummyRingCapacitor      = [ vRTInstance.capacitor[0], vRTInstance.capacitor[-1] ]        
           #self.dummyRingVRLayersDict   = {}
        
        self.hRoutingLayer_width         =   0 
        self.hRoutingTrack_width         =   vRTInstance.hRoutingTrack_width 
        self.minSpacing_hRoutingTrack    =   0 
        self.minimumPosition             =   0
        self.maximumPosition             =   0
        self.hRoutingtrackYCenter        = { "topTracks" : {} , "bottomTracks" : {} }
        self.hRoutingLayerYCenter        = { "topPlate"  : [] , "bottomPlate"  : [] } 

        self.vRTInstance                 =   vRTInstance
        self.topPlateStretching          =   0

        return



    ## Draws the complete layout given the capacitor matrix. \c route method is succession
    #  of calls to user-defined methods inside a newly created \c Updatesession.
    #  The following tasks are excecuted :
    #  -# A nex \c UpdateSession is created,
    #  -# all required physical layers are loaded,
    #  -# technology rules are defined according to capacitor type, 
    #  -# layout dimension parameters are computed,
    #  -# routing tracks and layers are drawn, 
    #  -# top plates are stretched,
    #  -# all required cuts are drawn,
    #  -# The \c UpdateSession is closed.
    #
    # Meanwhile, an exception is raised when the entered \c capacitor is not a
    # capacitor matrix or if the capacitor type is unsupported. 

    def route( self, bbMode=False ):

        UpdateSession.open()
         
        self.setRules()

        if not self.capacitorInstance.__isUnitCap__():
          if CapacitorStack.__isMatchingSchemeOK__(self):
            layersDict = self.setLayers        ()
            bondingBox = self.computeDimensions( bbMode )

            ab      = self.device.getAbutmentBox()
            trackNb = len(self.hRoutingtrackYCenter["topTracks"].keys())
            dY      = self.hpitch * trackNb

            trace( 101, '\tHeight before tracks enclosure: {0}\n'.format( DbU.getValueString(self.maximumPosition - self.minimumPosition) ))
            trace( 101, '\tHeight before tracks enclosure: {0}\n'.format( DbU.getValueString(ab.getHeight()) ))
            trace( 101, '\tminimum before adjust: {0}\n'.format( DbU.getValueString(self.minimumPosition) ))
            trace( 101, '\tdY: {0}\n'.format( DbU.getValueString(dY) ))

            firstVRTId = 't0'
            lastVRTId  = list(self.vRoutingTrackXCenter[-1].keys())[-1]
            firstIndex = int(firstVRTId[1])
            lastIndex  = int( lastVRTId[1])
            print( lastVRTId )
            if self.dummyRing:
              xMin = self.abutmentBox.getXMin()
              xMax = self.abutmentBox.getXMax()
            else:
              trackSpacing = (self.vRoutingTrack_width + self.vpitch + self.metal3Width)/2
              if firstIndex > len(self.vRoutingTrackXCenter[0]):
                  xMin = self.abutmentBox.getXMin() - trackSpacing
              else:
                  xMin = self.vRoutingTrackXCenter[0][firstVRTId] - trackSpacing

              if lastIndex > len(self.vRoutingTrackXCenter[-1]):
                  xMax = self.abutmentBox.getXMax() + trackSpacing
              else:
                  xMax = self.vRoutingTrackXCenter[-1][lastVRTId] + trackSpacing

            width       = xMax - xMin
            widthAdjust = width % (2*self.vpitch)
            if widthAdjust:
              widthAdjust = 2*self.vpitch - widthAdjust
              xMax += widthAdjust//2
              xMin -= widthAdjust//2
        
            self.device.setAbutmentBox( Box( int(xMin)
                                           , int(self.minimumPosition - dY)
                                           , int(xMax)
                                           , int(self.maximumPosition + dY )) )
            trace( 101, '\tHeight after tracks enclosure: {0}\n'.format( DbU.getValueString(self.device.getAbutmentBox().getHeight()) ))

            if not bbMode :
              self.drawHRLayers        ( layersDict["xPlateHRLayer" ] )
              self.drawHRoutingTracks  ( layersDict["hRoutingTracks"] )
              self.__stretchTopPlates__( self.capacitor
                                       , layersDict["xPlateRLayer"                     ] )    
              self.drawCuts            ( layersDict["cut_hRoutingLayer_vRoutingTracks" ]
                                       , layersDict["cut_hRoutingTracks_vRoutingTracks"]
                                       , layersDict["cut_hRoutingLayer_topPlate"       ] )
              if self.dummyRing:
                self.routeDummyRing( layersDict ["xPlateVRLayer_dummyRing"         ]
                                   , layersDict["cut_hRoutingTracks_vRoutingTracks"] )
          else:
            raise Error( 1, [ 'RouteMatchedCapacitor.route(): Invalid matching scheme.'
                            , '(${0})'.format( self.matchingScheme ) ] )

        else:
          raise Error( 1, [ 'RouteMatchedCapacitor.route(): Input matrix parameter is incorrect.'
                          , '({0})'.format( self.matrixDim ) ] )

        UpdateSession.close  ()

        return bondingBox



    def routeDummyRing( self, routingLayer, cutLayer ):
        
        net                               = self.nets[-1][1]
        bottomPlateRLayer_width           = self.dummyRingCapacitor[0][0].getBotPlateRLayerWidth() 
        topPlateRLayer_width              = self.dummyRingCapacitor[0][0].getTopPlateRLayerWidth() 
       #self.computeDummyRingDimensions   ()
        self.routeLeftAndRightSides       (net, routingLayer, bottomPlateRLayer_width, topPlateRLayer_width                         )
        self.routeTopOrBottomSide         (net, "topSide"    , routingLayer          , bottomPlateRLayer_width, topPlateRLayer_width)
        self.routeTopOrBottomSide         (net, "bottomSide" , routingLayer          , bottomPlateRLayer_width, topPlateRLayer_width)
        self.drawDummyRing_hRTracks_Cuts  (net, "topSide"    , cutLayer                                                              )
        self.drawDummyRing_hRTracks_Cuts  (net, "bottomSide" , cutLayer                                                              )

        return



    def routeLeftAndRightSides( self, dummyNet, routingLayer, bottomPlateRLayer_width, topPlateRLayer_width ) :

        for i in range(0,2):
          bottomPlateLeftRLayerXCenter  = self.dummyRingCapacitor[0][0 + i*(-1)].getBotPlateLeftRLayerXCenter() 
          bottomPlateRightRLayerXCenter = self.dummyRingCapacitor[0][0 + i*(-1)].getBotPlateRightRLayerXCenter() 
          Vertical.create( dummyNet
                         , routingLayer
                         , bottomPlateLeftRLayerXCenter
                         , bottomPlateRLayer_width
                         , self.getVTrackYMin()
                         , self.getVTrackYMax() ) 
          Vertical.create( dummyNet
                         , routingLayer
                         , bottomPlateRightRLayerXCenter
                         , bottomPlateRLayer_width
                         , self.getVTrackYMin()
                         , self.getVTrackYMax() ) 
            
        for i in range(0,2):
          topPlateRLayerXCenter = self.dummyRingCapacitor[0][0 + i*(-1)].topPlateRLayerDict["XCenter"]
          Vertical.create( dummyNet
                         , routingLayer
                         , topPlateRLayerXCenter
                         , topPlateRLayer_width
                         , self.getVTrackYMin()
                         , self.getVTrackYMax() ) 

        return




    def routeTopOrBottomSide( self, dummyNet, side, routingLayer, bottomPlateRLayer_width, topPlateRLayer_width):

        if   side == "topSide" : 
            dummyRingElement = self.dummyRingCapacitor[-1]
            dyTarget         = self.getVTrackYMax()
        elif side == "bottomSide" : 
            dummyRingElement = self.dummyRingCapacitor[0]
            dyTarget         = self.getVTrackYMin()
        else : raise Error(1,'routeTopOrBottomSide() : side parameter must be either "topSide" or "bottomSide" : %s' %side)

        for j in range(1,len(self.dummyRingCapacitor[0])-1):
          bottomPlateLeftRLayerXCenter  = dummyRingElement[j].getBotPlateLeftRLayerXCenter() 
          bottomPlateRightRLayerXCenter = dummyRingElement[j].getBotPlateRightRLayerXCenter() 
          Vertical.create( dummyNet
                         , routingLayer
                         , bottomPlateLeftRLayerXCenter
                         , bottomPlateRLayer_width
                         , dummyRingElement[j].getBotPlateRLayerYMin()
                         , dyTarget ) 
          Vertical.create( dummyNet
                         , routingLayer
                         , bottomPlateRightRLayerXCenter
                         , bottomPlateRLayer_width
                         , dummyRingElement[j].getBotPlateRLayerYMin()
                         , dyTarget ) 

          topPlateRLayerXCenter = dummyRingElement[j].topPlateRLayerDict["XCenter"]
          Vertical.create( dummyNet
                         , routingLayer
                         , topPlateRLayerXCenter
                         , topPlateRLayer_width
                         , dummyRingElement[j].getTopPlateRLayerYMin()
                         , dyTarget ) 


        return



    ## Defines technology rules used to draw the layout. Some of the rules,
    #  namely those describing routing layers and tracks are applicable for
    #  both MIM and PIP capacitors. However, cuts rules are different.
    #
    #  \remark All \c CapacitorStack class rules are also reloaded in this class.
    #          An exception is raised if the entered capacitor type is unsupported. 
    #
    #  \return a dictionary  with rules labels as keys and rules content as values.

    def setRules ( self ):

        VerticalRoutingTracks.setRules( self )

        self.minSpacing_hRoutingLayer                   = RoutMatchedCapacitor.rules.minSpacing_metbot
        self.minSpacing_hRoutingTrackCut                = RoutMatchedCapacitor.rules.minSpacing_cut2
        self.minSpacing_vRoutingTrackCut                = RoutMatchedCapacitor.rules.minSpacing_cut2
        self.minSpacing_hRoutingLayer_vRoutingTrack_cut = RoutMatchedCapacitor.rules.minSpacing_cut2

        if self.capacitorType == 'MIMCap':
          self.minWidth_hRoutingLayer_topPlate_cut     = RoutMatchedCapacitor.rules.minWidth_cut2
          self.minSpacing_hRoutingLayer_topPlate_cut   = RoutMatchedCapacitor.rules.minSpacing_cut2
          self.minEnclosure_hRoutingLayer_topPlate_cut = RoutMatchedCapacitor.rules.minEnclosure_metal2_cut2
        elif self.capacitorType == 'PIPCap': 
          self.minWidth_hRoutingLayer_topPlate_cut     = RoutMatchedCapacitor.rules.minWidth_cut1
          self.minSpacing_hRoutingLayer_topPlate_cut   = RoutMatchedCapacitor.rules.minSpacing_cut1
          self.minEnclosure_hRoutingLayer_topPlate_cut = RoutMatchedCapacitor.rules.minEnclosure_metal2_cut1

        return


    ## Defines all physical layers used to draw the layout. Layers are loaded using \c DataBase API. The same routing layers are used for both capacitor types except cuts layers that connect top plates to vertical routing tracks. Basicaly, metal 2, meta 3, cut 1 and cut 2 are the ones defined. 
    #  \return a dictionary composed of layers labels as keys and layers as values.

    def setLayers( self ):

        layersDict = {}

        layersDict["hRoutingTracks"                   ]  =  DataBase.getDB().getTechnology().getLayer("metal2" )
        layersDict["xPlateHRLayer"                    ]  =  DataBase.getDB().getTechnology().getLayer("metal2" )
        layersDict["xPlateRLayer"                     ]  =  CapacitorUnit.getLayers( self )["bottomPlateRLayer"]
        layersDict["cut_hRoutingTracks_vRoutingTracks"]  =  DataBase.getDB().getTechnology().getLayer("cut2"   )
        layersDict["cut_hRoutingLayer_vRoutingTracks" ]  =  DataBase.getDB().getTechnology().getLayer("cut2"   )
        layersDict["cut_hRoutingLayer_topPlate"       ]  =  DataBase.getDB().getTechnology().getLayer("cut2"   ) if self.capacitorType == 'MIMCap' else DataBase.getDB().getTechnology().getLayer("cut1")

        if self.dummyRing == True:
            layersDict["xPlateVRLayer_dummyRing"      ] =   DataBase.getDB().getTechnology().getLayer("metal3" ) if self.capacitorType == 'MIMCap' else DataBase.getDB().getTechnology().getLayer("metal1")

        return layersDict


    ## Computes, through simple instructions and functions calls, layout variables detailed in Figure 2. 

    def computeDimensions ( self, bbMode ) :

        bondingBoxDimensions           =  {}
        self.hRoutingLayer_width       =  max( self.minWidth_hRoutingLayer, self.minWidth_hRoutingLayer_vRoutingTrack_cut + 2*self.minEnclosure_hRoutingLayer_vRoutingTrack_cut, self.minWidth_hRoutingLayer_topPlate_cut + 2*self.minEnclosure_hRoutingLayer_topPlate_cut )
        
        self.abutmentBox_spacing       = self.capacitorInstance.abutmentBox_spacing        
        self.vRoutingTrack_spacing     = self.capacitorInstance.getVRoutingTrack_spacing()

        abutmentBoxXMin                = self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["XMin"] #getAbutmentBox_spacing()
        abutmentBoxYMin                = self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["YMin"] #getAbutmentBox_spacing()
        abutmentBoxYMax                = abutmentBoxYMin  + self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["height"] #getAbutmentBox_spacing()

        self.minimumPosition           =  self.vRTInstance.minimumPosition #abutmentBoxYMin - self.__setStretching__()
        self.maximumPosition           =  self.vRTInstance.maximumPosition #abutmentBoxYMax + self.__setStretching__()
        self.vRoutingTrackDict         =  self.vRTInstance.vRoutingTrackDict #self.minimumPosition - 4*self.hRoutingTrack_width - 4*self.minSpacing_hRoutingTrack

        translation1                   = self.vRoutingTrack_width/2 + self.vRoutingTrack_spacing
        translation2                   = translation1 + self.vRoutingTrack_width/2  

        self.vRoutingTrackXCenter      = self.vRTInstance.vRoutingTrackXCenter #getvRoutingTrackXCenter()
       #print("vRoutingTrackXCenter",self.vRoutingTrackXCenter)
        if   bbMode == True :
            bondingBoxDimensions = self.computeBondingBoxDimInbbMode() 

        elif bbMode == False :
            self.computeHRoutingTrackYCenter()
            self.computeHRLayerYCenter()

        else :raise Error(1, 'computeDimensions(): The bonding box mode parameter, "bbMode" must be either True or False : %s.' %bbMode )

       #print 'BOUNDING BOX:', bondingBoxDimensions
        return bondingBoxDimensions



    def computeBondingBoxDimInbbMode( self ):

        bondingBoxDimensions = {}

        abutmentBoxXMin                 = self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["XMin"]
        abutmentBoxXMax                 = abutmentBoxXMin + self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["width" ]

        bondingBoxDimensions["XMin"   ] = self.vRoutingTrackXCenter[0]["t0"] - self.vRoutingTrack_width/2

        if self.capacitorsNumber % 2 == 0 :
            factor                      = self.capacitorsNumber 
        if self.capacitorsNumber % 2 != 0 :
            factor                      = self.capacitorsNumber + 1 if self.matrixDim["columns"] % 2 == 0 else self.capacitorsNumber - 1

        bondingBoxXMax                  = abutmentBoxXMax + factor*(self.vRoutingTrack_width + self.vRoutingTrack_spacing)
        bondingBoxDimensions["YMin"   ] = self.vRoutingTrackDict["YMin"]
        bondingBoxYMax                  = self.vRoutingTrackDict["YMax"]
        bondingBoxDimensions["width"  ] = bondingBoxXMax - bondingBoxDimensions["XMin"]
        bondingBoxDimensions["height" ] = bondingBoxYMax - bondingBoxDimensions["YMin"] 
        bondingBoxDimensions["surface"] = bondingBoxDimensions["width"]*bondingBoxDimensions["height"]

        #bondingBox = Box( bondingBoxDimensions["XMin"], bondingBoxDimensions["YMin"], bondingBoxXMax, bondingBoxYMax )
        #self.device.setAbutmentBox( bondingBox )

        return bondingBoxDimensions


    ## Computes centers' ordinates of the eight horizontal routing tracks.
    #  The tracks include four on top and four on bottom of the matrix.
    #  To do the computations, fist, center of the first bottom or top track,
    #  given in Figure 2, is computed. Then, all adjacent three centers are
    #  deduced by simples translation of the first one. Translation quantity
    #  is equal to the sum of distance between adjacent routing tracks,
    #  self.hRoutingTracks_spacing, and half width of the routing track itself,
    #  \c self.hRoutingTrack_width.  

    def computeHRoutingTrackYCenter( self ):

        self.hRoutingtrackYCenter                        = { "topTracks" : {} , "bottomTracks" : {} }
       #self.hRoutingtrackYCenter["topTracks"    ]["t0"] = self.maximumPosition \
       #                                                 + self.hRoutingTrack_width/2 + self.minSpacing_hRoutingTrack 
       #self.hRoutingtrackYCenter["bottomTracks" ]["t0"] = self.minimumPosition \
       #                                                 - self.hRoutingTrack_width/2 - self.minSpacing_hRoutingTrack
        self.hRoutingtrackYCenter["topTracks"    ]["t0"] = self.maximumPosition
        self.hRoutingtrackYCenter["bottomTracks" ]["t0"] = self.minimumPosition
        
        keys = self.__setPlatesIds__()
        for k in range(1, len(keys)):
          trace( 101, '\tkeys[{0}] = {1}\n'.format( k, keys[k] ))
         #self.hRoutingtrackYCenter["topTracks"][keys[k]]         \
         #    = self.hRoutingtrackYCenter["topTracks"][keys[k-1]] \
         #    + (self.hRoutingTrack_width + self.minSpacing_hRoutingTrack) 
         #self.hRoutingtrackYCenter["bottomTracks" ][keys[k]]        \
         #    = self.hRoutingtrackYCenter["bottomTracks"][keys[k-1]] \
         #    - (self.hRoutingTrack_width + self.minSpacing_hRoutingTrack)
          self.hRoutingtrackYCenter["topTracks"][keys[k]]         \
              = self.hRoutingtrackYCenter["topTracks"][keys[k-1]] + self.hpitch
          self.hRoutingtrackYCenter["bottomTracks" ][keys[k]]        \
              = self.hRoutingtrackYCenter["bottomTracks"][keys[k-1]] - self.hpitch

        return


    ## Sets the stretching value of top plates. Then iteratively computes the centers of horizontal routing layer regarding top and bottom plates.

    def computeHRLayerYCenter ( self ):

        shortCircuitLists       = self.__findPossibleShortCircuits__()
        self.topPlateStretching = self.__setStretching__( )
       #print('        shortCircuitLists',        shortCircuitLists)
        for i in range( 0,self.matrixDim["rows"] ):
           #print("i",i)
           #print("rows", self.matrixDim["rows"])
           #print("rowsCap", len(self.capacitor))

            if shortCircuitLists[i][0] == 0 : 
                self.hRoutingLayerYCenter["bottomPlate"].append( [self.capacitor[i][0].getBottomPlateRightCutYMax ()] )      
            else : 
                bottomPlateYCenter0                   = [(self.capacitor[i][0].getBottomPlateRightCutYMax () + self.capacitor[i][0].getBottomPlateRightCutYMin() ) /2 ] 
                self.hRoutingLayerYCenter["bottomPlate"].append(bottomPlateYCenter0) 

            topPlateYCenter                           = self.__setStretchingDySourceDyTarget__( self.capacitor[i][0], self.topPlateStretching )[1] + self.hRoutingLayer_width/2
            self.hRoutingLayerYCenter["topPlate"   ].append( [topPlateYCenter ] )

            for j in range( 1,self.matrixDim["columns"] ):
                topPlateYCenter                       = self.__setStretchingDySourceDyTarget__( self.capacitor[i][j], self.topPlateStretching )[1] + self.hRoutingLayer_width/2 
                if shortCircuitLists[i][j] == 0 : 
                    self.hRoutingLayerYCenter["bottomPlate"][i].append( self.capacitor[i][j].getBottomPlateRightCutYMax () )
                    self.hRoutingLayerYCenter["topPlate"   ][i].append( topPlateYCenter )
                else :
                   bottomPlateYCenter                 = (self.capacitor[i][j].getBottomPlateRightCutYMax() + self.capacitor[i][j].getBottomPlateRightCutYMin()) /2
                   topPlateYCenter2                   = topPlateYCenter + self.minSpacing_hRoutingLayer + self.hRoutingLayer_width 
                   self.hRoutingLayerYCenter["bottomPlate"][i].append(bottomPlateYCenter)
                   self.hRoutingLayerYCenter["topPlate"   ][i].append(topPlateYCenter2)

       #print('self.hRoutingLayerYCenter',self.hRoutingLayerYCenter)
        return

    def computeLayoutDimensionsInbbMode( self ):

        bondingBoxDict            = {}
        bondingBoxDict["YMin"   ] =  self.hRoutingtrackYCenter["bottomTracks"]["botB"] - self.hRoutingTrack_width/2
        bondingBoxDict["XMin"   ] =  self.vRoutingTrackXCenter[0 ][0]                  - self.vRoutingTrack_width/2
        bondingBoxDict["height" ] =  self.hRoutingtrackYCenter["topTracks"   ]["botB"] + self.hRoutingTrack_width/2 - bondingBoxDict["YMin"] 
        bondingBoxDict["width"  ] =  self.vRoutingTrackXCenter[-1][1]                  + self.vRoutingTrack_width/2 - bondingBoxDict["XMin"]

        self.bondingBox           = Box( bondingBoxDict["XMin"], bondingBoxDict["YMin"], bondingBoxDict["XMin"] + bondingBoxDict["width"], bondingBoxDict["YMin"] + bondingBoxDict["height"] )

        return bondingBoxDict


   #def computeDummyRingDimensions( self ):
   #    for key in self.vRoutingTrackDict.keys():
   #      self.dummyRingVRLayersDict[key] = self.vRoutingTrackDict[key]
   #    return


    ##  Iteratively draws horizontal routing tracks on top and bottom positions of the matrix
    #   using physical layer \c routingTracksLayer. 

    def drawHRoutingTracks( self , routingTracksLayer ):

        lastVRTId   = list(self.vRoutingTrackXCenter[-1].keys())[-1]
        firstVRTId  = "t0"
       #doDummyRing = 1 if self.dummyRing == True else 0
       #dxSource    = self.vRoutingTrackXCenter[0][firstVRTId] - self.vRoutingTrack_width/2 if not self.dummyRing else self.abutmentBox.getXMin()
       #dxTarget    = self.vRoutingTrackXCenter[-1][lastVRTId] + self.vRoutingTrack_width/2 if not self.dummyRing else self.abutmentBox.getXMax()
        dxSource    = self.device.getAbutmentBox().getXMin()
        dxTarget    = self.device.getAbutmentBox().getXMax()

        for i in self.hRoutingtrackYCenter.keys():
          for j in self.hRoutingtrackYCenter[i].keys():
            if   j[0] == 't' : net = self.nets[ int(j[1]) ][ 0 ]
            elif j[0] == 'b' : net = self.nets[ int(j[1]) ][ 1 ]
            else:              net = self.nets[ -1        ][ 1 ]

            horizontal = Horizontal.create( net
                                          , routingTracksLayer
                                          , int(self.hRoutingtrackYCenter[i][j])
                                          , int(self.hRoutingTrack_width)
                                          , int(dxSource)
                                          , int(dxTarget) )
            NetExternalComponents.setExternal( horizontal )
        return

    
    ## Iteratively draws the horizontal routing layers starting with bottom left elementary capacitor \f$ C_{00} \f$.  

    def drawHRLayers( self, xPlateRLayer ) : 

        for i in range( 0,self.matrixDim["rows"] ):
            for j in range( 0,self.matrixDim["columns"] ):

                [ t , b ] = [  self.nets[self.matchingScheme[i][j]][0], self.nets[self.matchingScheme[i][j]][1] ]  
                dxDict                           =    self.__computeConnections__( i,j, self.matchingScheme[i][j] )

                Horizontal.create ( t
                                  , xPlateRLayer
                                  , int(self.hRoutingLayerYCenter["topPlate"   ][i][j])
                                  , int(self.hRoutingLayer_width)
                                  , int(dxDict["topPlate"    ][ "source" ])
                                  , int(dxDict["topPlate"    ][ "target" ]) ) 
                Horizontal.create ( b
                                  , xPlateRLayer
                                  , int(self.hRoutingLayerYCenter["bottomPlate"][i][j])
                                  , int(self.hRoutingLayer_width)
                                  , int(dxDict["bottomPlate" ][ "source" ])
                                  , int(dxDict["bottomPlate" ][ "target" ]) ) 

        return


    ## Draws all required cuts using physical layers : 
    # - \c layer_hRTrack_hRLayer to connect bottom plates to vertical routing tracks,
    # - \c layer_tracksCut to connect vertical routing tracks to horizontal ones, 
    # - \c layer_topPlateCut to connect top plates to vertical routing tracks.
    #  ALso in \c drawCuts, nUmber of maximum cuts number on every layer is computed and cuts enclosure is adjusted according to layer's width.

    def drawCuts( self, layer_hRTrack_hRLayer, layer_tracksCut, layer_topPlateCut ):

        cutNumber                   =  CapacitorUnit.cutMaxNumber( self, self.vRoutingTrack_width, self.minWidth_vRoutingTrackCut, self.minSpacing_vRoutingTrackCut , self.minEnclosure_vRoutingTrackCut  )
        enclosure_hRoutingTrack_cut = (self.vRoutingTrack_width - cutNumber*self.minWidth_hRoutingTrackCut - (cutNumber - 1)*self.minSpacing_hRoutingTrackCut)/2 

        self.drawCuts_vRoutingTrack_HRLayer       ( layer_hRTrack_hRLayer, cutNumber, enclosure_hRoutingTrack_cut )
        self.drawCuts_vRoutingTrack_hRoutingTrack ( layer_tracksCut      , cutNumber, enclosure_hRoutingTrack_cut )
        self.drawCuts_stretchedTopPlate           ( layer_topPlateCut                                             )

        return



    def drawCuts_vRoutingTrack_HRLayer( self, cutLayer, cutNumber, enclosure_cut  ):

        [ vRoutingTrackXCenter, cutXMin ] = [{}, {}]

        for i in range( 0, self.matrixDim["rows"] ):
            for j in range( 0, self.matrixDim["columns"] ):

                if ( j % 2 == 0 ):
                    leftVRTIds   = self.capacitorIds[0:self.capacitorsNumber//2]                       if (self.capacitorsNumber % 2 == 0)        else self.capacitorIds[0: int(self.capacitorsNumber//2+1)]
                    rightVRTIds  = self.capacitorIds[self.capacitorsNumber//2 : self.capacitorsNumber] if (self.capacitorsNumber % 2 == 0)        else self.capacitorIds[int(self.capacitorsNumber//2+1) : self.capacitorsNumber]

                else:
                    leftVRTIds   = self.capacitorIds[self.capacitorsNumber//2 : self.capacitorsNumber] if (self.capacitorsNumber % 2 == 0)        else self.capacitorIds[int(self.capacitorsNumber//2+1) : self.capacitorsNumber]
                    rightVRTIds  = self.capacitorIds[0:self.capacitorsNumber//2]                       if (self.capacitorsNumber % 2 == 0)        else self.capacitorIds[0: int(self.capacitorsNumber//2+1)]

                index1           = j                                                                  if self.matchingScheme[i][j] in leftVRTIds else j+1

                [topPlateLabel, bottomPlateLabel  ]     = [self.__setPlatesLabels__(i,j)["t"] , self.__setPlatesLabels__(i,j)["b"]]
                vRoutingTrackXCenter["topPlate"   ]     =  self.vRoutingTrackXCenter[index1][topPlateLabel + str( self.matchingScheme[i][j] )]
                vRoutingTrackXCenter["bottomPlate"]     =  self.vRoutingTrackXCenter[index1][bottomPlateLabel + str( self.matchingScheme[i][j] )]

                cutXMin["topPlate"   ]                  =  vRoutingTrackXCenter["topPlate"   ] - self.vRoutingTrack_width/2 + enclosure_cut + self.minWidth_hRoutingTrackCut/2
                cutXMin["bottomPlate"]                  =  vRoutingTrackXCenter["bottomPlate"] - self.vRoutingTrack_width/2 + enclosure_cut + self.minWidth_hRoutingTrackCut/2
                [ t , b ]        = [ self.nets[self.matchingScheme[i][j]][0] , self.nets[self.matchingScheme[i][j]][1] ]

                self.drawOneCut_vRoutingTrack_HRLayer( t, cutLayer, cutXMin["topPlate"   ],  self.hRoutingLayerYCenter["topPlate"   ][i][j], cutNumber )
                self.drawOneCut_vRoutingTrack_HRLayer( b, cutLayer, cutXMin["bottomPlate"],  self.hRoutingLayerYCenter["bottomPlate"][i][j], cutNumber )

        return



    ## Draws one cut, in layer \c cutLayer, in order to connect a vertical routing track, at position \c cutXMin in metal 2, and a horizontal routing track, at position \c cutYMin in metal 3.    
    def drawOneCut_vRoutingTrack_HRLayer( self, net, cutLayer, cutXMin, cutYMin, cutNumber ):

        CapacitorUnit.cutLine(self,net,cutLayer,cutXMin,cutYMin,self.minWidth_hRoutingLayer_vRoutingTrack_cut,self.minWidth_hRoutingLayer_vRoutingTrack_cut,self.minSpacing_hRoutingLayer_vRoutingTrack_cut,cutNumber,'horizontal') 
        return


        
    ## Draws cuts to connect vertical routing tracks in metal 2 and horizontal routing tracks in metal 3.    
    def drawCuts_vRoutingTrack_hRoutingTrack( self,cutLayer, cutNumber, enclosure_cut ):

        keysList   = list(self.hRoutingtrackYCenter.keys())

        for i in range(0, len(self.vRoutingTrackXCenter) ):
            for k in self.vRoutingTrackXCenter[i]:
                if self.vRoutingTrackXCenter[i][k] != None :
                    for l in range(0, 2) :                
                        net     = self.nets[ int(k[1]) ][0] if k[0] == 't' else self.nets[ int(k[1]) ][1]
                        cutXMin = self.vRoutingTrackXCenter[i][k] - self.vRoutingTrack_width/2 + enclosure_cut + self.minWidth_hRoutingTrackCut/2
                        CapacitorUnit.cutLine(self,net,cutLayer,cutXMin,self.hRoutingtrackYCenter[keysList[l]][k],self.minWidth_hRoutingTrackCut,self.minWidth_hRoutingTrackCut,self.minSpacing_hRoutingTrackCut,cutNumber,'horizontal') 

        return



    #  \param     cutLayer 
    def drawCuts_stretchedTopPlate( self, cutLayer ):

        layer_width                 = self.capacitor[0][0].getTopPlateRLayerWidth()
        cutNumber                   = CapacitorUnit.cutMaxNumber( self, layer_width , self.minWidth_hRoutingLayer_topPlate_cut, self.minSpacing_hRoutingLayer_topPlate_cut , self.minEnclosure_hRoutingLayer_topPlate_cut  )
        enclosure_hRoutingTrack_cut = (layer_width - cutNumber*self.minWidth_hRoutingLayer_topPlate_cut - (cutNumber - 1)*self.minSpacing_hRoutingLayer_topPlate_cut)/2 

        for i in range(0, self.matrixDim["rows"] ):
            for j in range(0, self.matrixDim["columns"] ):

                tNet  = self.nets[self.matchingScheme[i][j]][0] 
                cutXMin      = self.capacitor[i][j].getTopPlateRLayerXMin() + enclosure_hRoutingTrack_cut + self.minWidth_hRoutingLayer_topPlate_cut/2 
                CapacitorUnit.cutLine( self, tNet, cutLayer, cutXMin , self.hRoutingLayerYCenter["topPlate"][i][j] , self.minWidth_hRoutingLayer_topPlate_cut, self.minWidth_hRoutingLayer_topPlate_cut, self.minSpacing_hRoutingLayer_topPlate_cut, cutNumber, 'horizontal' ) 

        return


    def drawDummyRing_hRTracks_Cuts( self, dummyNet, side, cutLayer ):

        if   side == "topSide" : 
            dummyRingElement = self.dummyRingCapacitor[-1]
            key              = "topTracks"
        elif side == "bottomSide" : 
            dummyRingElement = self.dummyRingCapacitor[0]
            key              = "bottomTracks"
        else : raise Error(1,'routeTopOrBottomSide() : side parameter must be either "topSide" or "bottomSide" : %s' %side)
        
        topPlateLayer_width        = dummyRingElement[0].getTopPlateRLayerWidth()
        bottomPlateLayer_width     = dummyRingElement[0].getBotPlateRLayerWidth()

        topPlateCutNumber          =  CapacitorUnit.cutMaxNumber( self, topPlateLayer_width   , self.minWidth_vRoutingTrackCut, self.minSpacing_vRoutingTrackCut , self.minEnclosure_vRoutingTrackCut  )
        bottomPlateCutNumber       =  CapacitorUnit.cutMaxNumber( self, bottomPlateLayer_width, self.minWidth_vRoutingTrackCut, self.minSpacing_vRoutingTrackCut , self.minEnclosure_vRoutingTrackCut  )

        topPlateCutEnclosure       = (topPlateLayer_width    - topPlateCutNumber   *self.minWidth_vRoutingTrackCut - (topPlateCutNumber    - 1)*self.minSpacing_vRoutingTrackCut)/2 
#        bottomPlateCutEnclosure    = (bottomPlateLayer_width - bottomPlateCutNumber*self.minWidth_vRoutingTrackCut - (bottomPlateCutNumber - 1)*self.minSpacing_vRoutingTrackCut)/2 

        netsDistribution           = self.__setPlatesIds__()

       #print("netsDistribution2",netsDistribution)
       #print("self.hRoutingtrackYCenter",self.hRoutingtrackYCenter)
        for j in range(0,len(dummyRingElement)):
            topPlateCutXCenter         = dummyRingElement[j].getTopPlateRLayerXMin     () + topPlateCutEnclosure    + self.minWidth_vRoutingTrackCut/2
            bottomPlateLeftCutXCenter  = dummyRingElement[j].getBottomPlateLeftCutXMin() #dummyRingElement[j].getBotPlateLeftRLayerXMin () + bottomPlateCutEnclosure + self.minWidth_vRoutingTrackCut/2
            bottomPlateRightCutXCenter = dummyRingElement[j].getBottomPlateRightCutXMin() #dummyRingElement[j].getBotPlateRightRLayerXMin() + bottomPlateCutEnclosure + self.minWidth_vRoutingTrackCut/2f 

            CapacitorUnit.cutLine( self, dummyNet, cutLayer, topPlateCutXCenter        , self.hRoutingtrackYCenter[key][netsDistribution[-1]] , self.minWidth_vRoutingTrackCut,self.minWidth_vRoutingTrackCut,self.minSpacing_vRoutingTrackCut,topPlateCutNumber,'horizontal') 
            CapacitorUnit.cutLine( self, dummyNet, cutLayer, bottomPlateLeftCutXCenter , self.hRoutingtrackYCenter[key][netsDistribution[-1]] , self.minWidth_vRoutingTrackCut,self.minWidth_vRoutingTrackCut,self.minSpacing_vRoutingTrackCut,bottomPlateCutNumber,'horizontal') 
            CapacitorUnit.cutLine( self, dummyNet, cutLayer, bottomPlateRightCutXCenter, self.hRoutingtrackYCenter[key][netsDistribution[-1]] , self.minWidth_hRoutingTrackCut,self.minWidth_hRoutingTrackCut,self.minSpacing_hRoutingTrackCut,bottomPlateCutNumber,'horizontal') 

        return



    ## Iteratively performs top plates stretching for the capacitor matrix.
    #  Vertical segments are connected to top plate routing layer.  
    #  \param    capacitor    Capacitor matrix. 
    #  \param    rlayer       Layer of the drawn vertical rectangle.        
    def __stretchTopPlates__( self, capacitor, rlayer ):

        for i in range( 0, self.matrixDim["rows"] ):
            for j in range( 0, self.matrixDim["columns"] ):
                
                t = self.nets[self.matchingScheme[i][j]][0]
               #print('t',t)
                self.__stretchTopPlateCompactCap__( t , capacitor[i][j], rlayer, j ) 

        return



    ## Draws vertical stretched layers for a given elementary capacitor. 
    def __stretchTopPlateCompactCap__( self, net, capacitor, routingLayer, j = 0 ):

        topPlateRLayer_width    = capacitor.getTopPlateRLayerWidth() 
        topPlateRLayerXCenter   = capacitor.getTopPlateRLayerXCenter()
        [ dySource , dyTarget ] = self.__setStretchingDySourceDyTarget__( capacitor, self.topPlateStretching )

        Vertical.create( net
                       , routingLayer
                       , int(topPlateRLayerXCenter)
                       , int(topPlateRLayer_width)
                       , int(dySource)
                       , int(dyTarget) ) 
        return



    ## Sets the abcissas of the extremities of the vertical stretching to be applied to capacitor's top plates for a given elementary capacitor in the matrix.
    #  \param    capacitor  .values()  Elementary unit capacitor.
    #  \param    deltay       Stretching value.
    #  \return A list that contains \c dySource and \dyTarget as top extremity and bottom extermity, respectively.
    def __setStretchingDySourceDyTarget__( self, capacitor, deltay ):

        dySource = capacitor.getTopPlateRLayerYMin() 
        dyTarget = dySource - deltay                 

        return [ dySource , dyTarget ]


    ## Computes horizontal routing layers source and target abcissas for top and bottom plates connections to its associated routing track. 
    #  \param    (i,j)                 row and column indexes, respectively, in the matrix which describe the elementary capacitor position in the matrix.
    #  \param    capacitorIdentifier   equal to '1' if C1 and '2' if C2.
    #  \return   A nested dicitionary. The overal dictionary is composed of keys equal to \c topPlate and \d bottomPlate and values equal to sub-dictionaries. The sub-dictionaries, are in their turn composed of two keys standing for the abcissa of the source and the abcissa of the target.  
    #  \remark   Naturally, an exception is raised if an unsupported capacitor identifier is given. 

    def __computeConnections__( self, i,j, capacitorIdentifier ):

        if capacitorIdentifier in self.capacitorIds :

            dxDict = { "bottomPlate": {}, "topPlate": {} }

            if ( j % 2 == 0 ):
                leftVRTIds   = self.capacitorIds[0                       :self.capacitorsNumber//2] if (self.capacitorsNumber % 2 == 0)          else self.capacitorIds[0                              : int(self.capacitorsNumber/2+1)]
                rightVRTIds  = self.capacitorIds[self.capacitorsNumber//2 : self.capacitorsNumber  ] if (self.capacitorsNumber % 2 == 0)          else self.capacitorIds[int(self.capacitorsNumber//2+1) : self.capacitorsNumber         ]

            else:
                leftVRTIds   = self.capacitorIds[self.capacitorsNumber//2 : self.capacitorsNumber   ] if (self.capacitorsNumber % 2 == 0)          else self.capacitorIds[int(self.capacitorsNumber//2+1) : self.capacitorsNumber         ]
                rightVRTIds  = self.capacitorIds[0                       : self.capacitorsNumber//2 ] if (self.capacitorsNumber % 2 == 0)          else self.capacitorIds[0                              : int(self.capacitorsNumber//2+1)]

            
            [topPlateLabel, bottomPlateLabel  ] = [self.__setPlatesLabels__(i,j)["t"] , self.__setPlatesLabels__(i,j)["b"]]

            dxDict["topPlate"    ][ "source" ]  =  self.capacitor[i][j].getTopPlateRLayerXMax     ()  if  self.matchingScheme[i][j] in leftVRTIds  else  self.capacitor[i][j].getTopPlateRLayerXMin() 
            dxDict["topPlate"    ][ "target" ]  =  self.__findHRLDyTrarget__( i,j, topPlateLabel, leftVRTIds, rightVRTIds )

            dxDict["bottomPlate" ][ "source" ]  =  self.capacitor[i][j].getBotPlateLeftRLayerXMax ()  if  self.matchingScheme[i][j] in leftVRTIds  else  self.capacitor[i][j].getBotPlateRightRLayerXMin ()            
#            bottomPlateLabel = 'b' if self.dummyElement == False or not( self.__isCapacitorAdummy__(capacitorIdentifier) ) else 't'
           #print("bottomPlateLabel",bottomPlateLabel)
            dxDict["bottomPlate" ][ "target" ]  =  self.__findHRLDyTrarget__( i, j, bottomPlateLabel, leftVRTIds, rightVRTIds )
 
        else : raise Error(1, '__computeConnections__(): Unknown capacitor Id : %s.' %capacitorIdentifier )

       #print('dxDict',dxDict)
        return dxDict


    def __isCapacitorAdummy__( self, capacitorIdentifier ) :

        state = False 
        if self.dummyElement == True :
            if capacitorIdentifier == self.capacitorIds[-1] : state = True
        else : raise Error(1,'The matrix contains no dummies because dummy element is "False".')

        return state



    def __setPlatesLabels__( self, i, j ):

        platesLabels        = {}
        capacitorIdentifier = self.matchingScheme[i][j]        
        platesLabels["t"]   = 't'
        platesLabels["b"]   = 'b' if self.dummyElement == False or not( self.__isCapacitorAdummy__(capacitorIdentifier) ) else 't'

        return platesLabels


    def __findHRLDyTrarget__( self, i,j, plateLabel, leftVRTIds, rightVRTIds  ):

        if plateLabel in ['t','b'] :
            plateIndex = self.matchingScheme[i][j]

            [ doLeft , doRight ] = [1,0]      if   self.matchingScheme[i][j] in leftVRTIds else [0,1] 
            xCenterList          = leftVRTIds if [ doLeft , doRight ] == [1,0]             else rightVRTIds 
            index1               = j          if [ doLeft , doRight ] == [1,0]             else j+1

            for key in self.vRoutingTrackXCenter[index1].keys() :
                if key == plateLabel + str(plateIndex) :
                    index2 = key 
                    break

        else : raise Error(1,'__findHRLDyTrarget__() : Plate label must be "t" for top plate and "b" for bottom plate. The given label is : %s.' %plateLabel)

        return self.vRoutingTrackXCenter[index1][index2] - doLeft*self.vRoutingTrack_width/2 + doRight*self.vRoutingTrack_width/2



    def __setPlatesIds__( self ):

        keys = []
        for k in range(0, len(self.nets)):
            keys.append( 't' + str(k) )
            keys.append( 'b' + str(k) )

        if self.dummyRing == True or self.dummyElement == True : keys = keys[0:len(keys)-1]

        return keys


    def __findPossibleShortCircuits__( self ):

        shortCircuitLists = []
        self.vRTsDistribution = self.vRTInstance.vRTsDistribution
        for i in range(0, self.matrixDim["rows"]):
            shortCircuitLists.append([0])

            for j in range(0, self.matrixDim["columns"]):
                shortCircuitLists[i].append(0)
        for i in range(0, self.matrixDim["rows"]):
            for j in range(0, self.matrixDim["columns"]-1):
               #print('self.vRTsDistribution',self.vRTsDistribution)
                if self.matchingScheme[i][j] in self.vRTsDistribution[j+1] and self.matchingScheme[i][j+1] in self.vRTsDistribution[j+1] and self.matchingScheme[i][j] > self.matchingScheme[i][j+1] : shortCircuitLists[i][j] = 1

        return shortCircuitLists




def scriptMain( **kw ):

    editor = None
    if 'editor' in kw and kw['editor']:
        editor = kw['editor']

    UpdateSession.open()
    Device = AllianceFramework.get().createCell( 'capacitor' )
    Device.setTerminal( True )

    bottomPlate_net0 = Net.create( Device, 'b0' )
    bottomPlate_net1 = Net.create( Device, 'b1' )
    bottomPlate_net2 = Net.create( Device, 'b2' )
    bottomPlate_net3 = Net.create( Device, 'b3' )
    bottomPlate_net0.setExternal( True )  
    bottomPlate_net1.setExternal( True )  
    bottomPlate_net2.setExternal( True )  
    bottomPlate_net3.setExternal( True )  
    b0  = Device.getNet("b0")
    b1  = Device.getNet("b1")
    b2  = Device.getNet("b2")
    b3  = Device.getNet("b3")

    topPlate_net0    = Net.create( Device, 't0' )
    topPlate_net1    = Net.create( Device, 't1' )
    topPlate_net2    = Net.create( Device, 't2' )
    topPlate_net3    = Net.create( Device, 't3' )
    topPlate_net0.setExternal( True )  
    topPlate_net1.setExternal( True )  
    topPlate_net2.setExternal( True )  
    topPlate_net3.setExternal( True )  
    t0     = Device.getNet("t0")
    t1     = Device.getNet("t1")
    t2     = Device.getNet("t2")
    t3     = Device.getNet("t3")

    if editor:
        UpdateSession.close()
        editor.setCell( Device )
        editor.fit()  
        UpdateSession.open()

    nets   = [[t0, b0] , [t1, b1], [t2, b2] ] # [t3, b3] ]
    capacitorInstance = CapacitorStack( Device, [372,1116], 'MIMCap', [0,0], nets, unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,1,1] , [1,1,1,0] , [0,1,1,1] ] , dummyRing = True )#dummyRing = True)

  #  capacitorInstance = CapacitorStack( Device, [279,1023, 186], 'MIMCap', [0,0], nets, unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,2,1] , [1,1,1,0] , [2,1,1,1] ], dummyElement = True, dummyRing = True )#dummyRing = True)
  #  capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 50, matchingMode = True, matchingScheme =  [ ['C2','C1','C2','C1','C1'] , ['C1','C2','C1','C2','C2'] , ['C1','C2','C2','C2', 'C1'] , ['C1','C2','C2','C1', 'C1'], ['C2','C2','C2','C2','C2'] ] )
  #  capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 93, matchingMode = True, matchingScheme =  [ ['C2','C2','C2','C2'] , ['C1','C2','C2','C2'] , ['C1','C2','C2','C2'] , ['C1','C2','C2','C1'] ] )
  #  capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 93, matchingMode = True, matchingScheme =  [ ['C2','C2','C1','C2'] , ['C1','C2','C2','C1'] , ['C1','C1','C2','C3'] , ['C1','C1','C2','C3'] ] )
  #  capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, matrixDim = [5,5] , matchingMode = True, matchingScheme =  [ ['C2','C1','C2','C3','C1'] , ['C1','C2','C4','C3','C2'] , ['C4','C2','C2','C3', 'C1'] , ['C1','C2','C2','C1', 'C1'], ['C2','C2','C3','C2','C4'] ] )

    capacitor         = capacitorInstance.create() 
   #print('capa',capacitor)

    capWithVRT        = VerticalRoutingTracks( capacitorInstance, capacitor, True ) 
    capWithVRT.create()

    routedCap         = RoutMatchedCapacitor( capWithVRT )
    surface           = routedCap.route()   
   #print('routeMatchedCap bbMode', surface)
   #print('width', toPhY(surface["width"]))
   #print('height', toPhY(surface["height"]))

    AllianceFramework.get().saveCell( Device, Catalog.State.Views )

    return True

