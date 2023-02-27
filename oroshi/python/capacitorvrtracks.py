#!/usr/bin/python
import sys                
from   collections      import OrderedDict
import numpy
from   ..Hurricane      import *
from   ..CRL            import *
from   ..helpers.io     import ErrorMessage as Error
from   ..helpers        import trace
from   .                import getRules
from   .capacitorunit   import CapacitorUnit
from   .capacitormatrix import CapacitorStack


## Route two matched capacitors, C1 and C2, drawn in a capacitor matrix.
#  Connections are put in place with reference to a given matching scheme.
#  Elementary capacitor units are connected to horizontal and vertical routing
#  tracks that represent top plates and bottom plates nets of C1 and C2.
#  Supported types of capacitors are Poly-Poly and Metal-Metal.
#  Technologycal rules are provided by 350 nm AMS CMOS technology with
#  three-four metal layers. Metal layers that are used for routeing are
#  placed similarly to horziontal-vertical (HV) symbolic Alliance CAD tool router,
#  where horizontal metal channels are drawn in metal 2 and the vertical ones are
#  in metal 3. Given a matrix of dimensions \f$ R*C \f$, the total number of
#  vertical tracks is \f$ 2C+2 \f$ equivalent to \f$ C+1 \f$ couples,
#  ensuring that every elementary capacitor is positioned between four
#  vertical tracks, two from each side. In fact, every adjacent couple of these
#  tracks represent top plates and bottom plates of C1 or C2 as shown in Figure 1.
#
#  \image html Layout.png "Layout" width=.1\linewidth
#
#  An elementary capacitor unit can be a part of C1 or C2 according to the
#  matching scheme. However, to respect common-centroid layout specifications,
#  for C1 and C2 to be equal, the matrix number of colums and number of rows
#  must be both even. Addionnally, the number of elementary capacitors dedicated
#  to C1 must be equal to those dedicated to C2. These two conditions are tested
#  in one of the class methods. An exception is raised if at least one of the two
#  is not respected.


class VerticalRoutingTracks ( CapacitorStack ):

    rules = getRules()

    def __init__( self, capacitorInstance, capacitor, minimizeVRT = False ) :

        self.device                   = capacitorInstance.device
        self.capacitorInstance        = capacitorInstance
        self.capacitor                = capacitor
        self.matchingScheme           = capacitorInstance.matchingScheme
        self.capacitorType            = capacitorInstance.capacitorType
        self.abutmentBox              = capacitorInstance.abutmentBox
        self.matrixDim                = self.capacitorInstance.matrixDim   
        self.nets                     = capacitorInstance.nets
        self.capacitorsNumber         = capacitorInstance.capacitorsNumber
        self.dummyRing                = capacitorInstance.dummyRing
        self.dummyElement             = capacitorInstance.dummyElement
        self.capacitorIds             = range(0,self.capacitorsNumber) 
        self.abutmentBox_spacing      = capacitorInstance.abutmentBox_spacing   
        self.vRoutingTrack_width      = self.capacitorInstance.vRoutingTrack_width    
        self.vRoutingTrackXCenter     = [] 
        self.vRoutingTrackDict        = {}  

        self.minimizeVRT              = minimizeVRT
        self.vRTsToEliminate          = []
        self.vRTsDistribution         = []
        self.platesDistribution       = []

        return


    def getVTrackYMin ( self ): return self.vRoutingTrackDict["YMin"] - self.hRoutingTrack_width/2
    def getVTrackYMax ( self ): return self.vRoutingTrackDict["YMax"] + self.hRoutingTrack_width/2


    ## Sets vertical stretching value considering spacing between elementary capacitors in the matrix.
    #  \return stratching value.
    def __setStretching__( self ): return self.minSpacing_botPlate + self.abutmentBox_spacing/2        

    ## Defines technology rules used to draw the layout. Some of the rules, namely those describing routeing layers and tracks are applicable for both MIM and PIP capacitors. However, cuts rules are different. \remark All \c CapacitorStack class rules are also reloaded in this class. An exception is raised if the entered capacitor type is unsupported. 
    #  \return a dictionary  with rules labels as keys and rules content as values.

    def setRules ( self ):
        CapacitorStack.setRules( self )

        self.minWidth_hRoutingTrackCut                    = VerticalRoutingTracks.rules.minWidth_cut2
        self.minWidth_hRoutingLayer_vRoutingTrack_cut     = VerticalRoutingTracks.rules.minWidth_cut2
        self.minWidth_hRoutingTrack                       = VerticalRoutingTracks.rules.minWidth_metal2
        self.minEnclosure_hRoutingLayer_vRoutingTrack_cut = VerticalRoutingTracks.rules.minEnclosure_metal2_cut2
        self.minEnclosure_hRoutingTrackCut                = VerticalRoutingTracks.rules.minEnclosure_metal2_cut2
        self.minWidth_hRoutingLayer                       = VerticalRoutingTracks.rules.minWidth_metal2
        self.minSpacing_hRoutingTrack                     = VerticalRoutingTracks.rules.minSpacing_metbot
        return


    def create( self ):

        UpdateSession.open           ()
         
        self.setRules                ()
        vRoutingTracksLayer  =  DataBase.getDB().getTechnology().getLayer("metal3" )
       #print 'self.capacitorInstance.doMatrix:', self.capacitorInstance.doMatrix
       #print 'self.capacitorsNumber:', self.capacitorsNumber

        if self.capacitorInstance.doMatrix == True and self.capacitorsNumber > 1 : 
            self.minimizeVRTs()
            self.computeVRTDimensions()
            self.drawVRoutingTracks( vRoutingTracksLayer )
        else : raise Error(1, 'create() : Impossible to route. This class only routes a matrix of two or more capacitors. Please use  <routeCapacitorMatrix> class instead.')
        UpdateSession.close          ()

        return


    ## Iteratively draws vertical routing tracks given the physical layer
    #  \c vRoutingTracksLayer. Every elementary capacitor is consequently
    #  positioned between four routing tracks, two from each side.
    #  Each couple of adjacent routeing tracks represent top plate and
    #  bottom plate nets of Ci, where i is in [1,2]. As given in Figure 2,
    #  capacitor \f$ C_{ij} \f$ with an even j value situated in even columns
    #  have and inversely for odd columns numbers. 

    def drawVRoutingTracks ( self, vRoutingTracksLayer ) :
        netsDistribution = self.__setNetsDistribution__()
        k = 0
        for j in range( 0, self.matrixDim["columns"] + 1 ):
          for key in self.vRoutingTrackXCenter[j]:
            if self.vRoutingTrackXCenter[j][key] is not None:
              Vertical.create( netsDistribution[j][k]
                             , vRoutingTracksLayer
                             , int(self.vRoutingTrackXCenter[j][key])
                             , int(self.vRoutingTrack_width)
                             , int(self.getVTrackYMin())
                             , int(self.getVTrackYMax() ))
              k = k + 1 if k < len(key)-1 else 0
        return



    def computeVRTDimensions ( self ) :

        self.hRoutingTrack_width       = max( self.minWidth_hRoutingTrack, self.minWidth_hRoutingTrackCut + 2*self.minEnclosure_hRoutingTrackCut ) 
        abutmentBoxYMin                = self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["YMin"]
        abutmentBoxYMax                = abutmentBoxYMin + self.capacitorInstance.computeAbutmentBoxDimensions( self.abutmentBox_spacing )["height"]

        self.minimumPosition           = abutmentBoxYMin - self.__setStretching__() - self.hpitch + self.metal2Width
        self.maximumPosition           = abutmentBoxYMax + self.__setStretching__() + self.hpitch - self.metal2Width

        height       = self.maximumPosition - self.minimumPosition
        heightAdjust = height % (2*self.hpitch)
        if heightAdjust:
          heightAdjust = 2*self.hpitch - heightAdjust
          trace( 101, '\tAdjust height: {0}\n'.format( DbU.getValueString(heightAdjust) ))
          trace( 101, '\tHeight before adjust: {0}\n'.format( DbU.getValueString(self.maximumPosition - self.minimumPosition) ))
          trace( 101, '\tminimum before adjust: {0}\n'.format( DbU.getValueString(self.minimumPosition) ))
          self.maximumPosition += heightAdjust/2
          self.minimumPosition -= heightAdjust/2
          trace( 101, '\tHeight after adjust: {0}\n'.format( DbU.getValueString(self.maximumPosition - self.minimumPosition) ))
          trace( 101, '\tminimum after adjust: {0}\n'.format( DbU.getValueString(self.minimumPosition) ))

        vRTsNumber = self.__computeVRTsNumber__()
       #self.vRoutingTrackDict["YMin"] = self.minimumPosition - vRTsNumber*(self.hRoutingTrack_width + self.minSpacing_hRoutingTrack)
       #self.vRoutingTrackDict["YMax"] = self.maximumPosition + vRTsNumber*(self.hRoutingTrack_width + self.minSpacing_hRoutingTrack)
        self.vRoutingTrackDict["YMin"] = self.minimumPosition - (vRTsNumber-1)*self.hpitch
        self.vRoutingTrackDict["YMax"] = self.maximumPosition + (vRTsNumber-1)*self.hpitch

        self.__setPlatesDistribution__()
        self.computeXCenters()

        return


    def __computeVRTsNumber__ ( self ):

        if self.dummyElement == True :
            vRTsNumber = 2*self.capacitorsNumber - 1 
        else : 
            vRTsNumber = 2*self.capacitorsNumber + 1 if self.dummyRing == True else 2*self.capacitorsNumber        

        return vRTsNumber




    def computeXCenters( self ):

        unitCapDim               = self.capacitorInstance.getCapDim()
        abutmentBoxUnitCap_width = CapacitorUnit.computeAbutmentBoxDimensions( self, unitCapDim )["width"]
        unitCapXMin              = self.abutmentBox.getXMin() if self.dummyRing == False else self.abutmentBox.getXMin() + abutmentBoxUnitCap_width + self.capacitorInstance.abutmentBox_spacing 

        vRoutingTrack_spacing    = self.capacitorInstance.minSpacing_vRoutingTrack  
        [ factor1 , factor2 ]    = [ self.capacitorsNumber , self.capacitorsNumber-1 ] if  ( self.capacitorsNumber % 2 == 0 ) else [ self.capacitorsNumber + 1 , self.capacitorsNumber ]

        if self.vRTsToEliminate[0][0] == 1 :
            self.vRoutingTrackXCenter.append( OrderedDict() )
            self.vRoutingTrackXCenter[0][ self.platesDistribution[0][0] ] = unitCapXMin - factor1*vRoutingTrack_spacing - (factor2)*self.vRoutingTrack_width - self.vRoutingTrack_width/2
        else :
            self.vRoutingTrackXCenter.append( OrderedDict() )
            self.vRoutingTrackXCenter[0][ self.platesDistribution[0][0] ] = None

        leftVRTNumber = self.capacitorsNumber if ( self.capacitorsNumber % 2 == 0 ) else self.capacitorsNumber + 1

        for k in range( 1, leftVRTNumber ):
            self.vRoutingTrackXCenter[0][self.platesDistribution[0][k]] = unitCapXMin - (factor1-k)*vRoutingTrack_spacing -(factor2-k)*self.vRoutingTrack_width - self.vRoutingTrack_width/2 if self.vRTsToEliminate[0][k] == 1 else None 

        #print('self.vRoutingTrackXCenter',self.vRoutingTrackXCenter)        
        for j in range( 1, self.matrixDim["columns"] + 1 ):
            factor3     = j - 1 if self.dummyRing == False else j  
            unitCapXMin = self.abutmentBox.getXMin() + factor3*( abutmentBoxUnitCap_width + self.capacitorInstance.abutmentBox_spacing )  
            unitCapXMax = unitCapXMin                +  abutmentBoxUnitCap_width 

            if self.vRTsToEliminate[j][0] == 1: 
                self.vRoutingTrackXCenter.append( OrderedDict() ) 
                self.vRoutingTrackXCenter[j][ self.platesDistribution[j][0] ] = unitCapXMax + vRoutingTrack_spacing + self.vRoutingTrack_width/2  
            else : 
                self.vRoutingTrackXCenter.append( OrderedDict() ) 
                self.vRoutingTrackXCenter[j][ self.platesDistribution[j][0] ] = None

            if self.capacitorsNumber % 2 == 0:
                rightVRTNumber = self.capacitorsNumber
            else : 
                if (j % 2 == 0) : 
                    rightVRTNumber = self.capacitorsNumber + 1
                else :  rightVRTNumber = self.capacitorsNumber - 1 if self.dummyElement == False else self.capacitorsNumber - 2

            for k in range( 1, rightVRTNumber ):
                
                    self.vRoutingTrackXCenter[j][ self.platesDistribution[j][k] ] = unitCapXMax + (k+1)*vRoutingTrack_spacing + (k)*self.vRoutingTrack_width + self.vRoutingTrack_width/2   if self.vRTsToEliminate[j][k] == 1         else None

        #print('self.vRoutingTrackXCenter',self.vRoutingTrackXCenter)        

        return



    def __findUsedCapIdsPerColumn__( self ):

        usedCapIdsPerColumn = []
        for j in range(0, self.matrixDim["columns"] ):    
            usedCapIdsPerColumn.append( [self.matchingScheme[0][j]] )
            for i in range(1, self.matrixDim["rows"] ):
                usedCapIdsPerColumn[j].append( self.matchingScheme[i][j] ) 

            usedCapIdsPerColumn[j] = numpy.unique(usedCapIdsPerColumn[j])

        return usedCapIdsPerColumn



    def __findCapIdsToEliminatePerColumn__( self, usedCapIdsPerColumn ):

        capIdsToEliminatePerColumn = []
        for j in range(0, len(usedCapIdsPerColumn) ):    
            if len(usedCapIdsPerColumn[j]) == self.capacitorsNumber:
                capIdsToEliminatePerColumn.append( [None] )
            else :
                capIdsToEliminatePerColumn.append( list (set(usedCapIdsPerColumn[j])^set(self.capacitorIds) ) )

        return capIdsToEliminatePerColumn



    def __findCapIdsToEliminate__( self, capIdsToEliminatePerColumn ):

        capIdsToEliminate = []
        capIdsj       = capIdsToEliminatePerColumn[0]
        #print('capIdsj',capIdsj)
        sharedVRTIds  = self.capacitorIds[0:self.capacitorsNumber//2] if self.capacitorsNumber % 2 == 0 else self.capacitorIds[0: int(self.capacitorsNumber//2+1)]        
        #print('sharedVRTIds',sharedVRTIds)
        intersection2 = list( set(capIdsj).intersection(set(sharedVRTIds)) ) 
        capIdsToEliminate.append( [None] )  if len(intersection2) == 0  else capIdsToEliminate.append( intersection2 )

        for j in range(0, len(capIdsToEliminatePerColumn) ):
            capIdsj       = capIdsToEliminatePerColumn[j]
            if (j % 2 == 0):
                sharedVRTIds  = self.capacitorIds[self.capacitorsNumber//2 : self.capacitorsNumber] if (self.capacitorsNumber % 2 == 0) else self.capacitorIds[int(self.capacitorsNumber//2+1) : self.capacitorsNumber]
        
            else:
                sharedVRTIds  = self.capacitorIds[0:self.capacitorsNumber//2] if (self.capacitorsNumber % 2 == 0) else self.capacitorIds[0: int(self.capacitorsNumber//2+1)]

            #print('sharedVRTIds',sharedVRTIds)

            if j == len(capIdsToEliminatePerColumn)-1:
                intersection1 = list( set(capIdsj) )
            else :
                capIdsjp1     = capIdsToEliminatePerColumn[j+1]
                intersection1 = list( set(capIdsj).intersection(set(capIdsjp1)) )

            intersection2 = list( set(intersection1).intersection(set(sharedVRTIds)) ) 
            capIdsToEliminate.append( [None] )  if len(intersection2) == 0  else capIdsToEliminate.append( intersection2 )

        return capIdsToEliminate



    def __findVRTsToEliminate__( self, capIdsToEliminate ):

        #print('capIdsToEliminate',capIdsToEliminate)        
        for j in range( 0,len(self.vRTsDistribution) ) :
            for k in range( 0,len(self.vRTsDistribution[j]) ) :
                if k == 0 :
                    if self.vRTsDistribution[j][0] in capIdsToEliminate[j] :
                        self.vRTsToEliminate.append( [0] ) 
                        self.vRTsToEliminate[j].append( 0 ) 
                    else : 
                        self.vRTsToEliminate.append( [1] ) 
                        self.vRTsToEliminate[j].append( 1 ) 
                    #print('vRTsToEliminate',self.vRTsToEliminate)
                else : 
                    if self.vRTsDistribution[j][k] in capIdsToEliminate[j]:
                        [self.vRTsToEliminate[j].append( 0 ) for u in range(0,2)]
                    else : 
                        [self.vRTsToEliminate[j].append( 1 ) for u in range(0,2)]

        return 



    def minimizeVRTs( self ) :
        
        self.__setVRTsDistribution__()

        if   self.minimizeVRT == False  :
             for j in range( 0, self.matrixDim["columns"]+1 ) :
                 self.vRTsToEliminate.append( [1] ) 
                 self.vRTsToEliminate[j].append( 1 ) 

                 for k in range( 0,len(self.vRTsDistribution[j]) ) :
                     [self.vRTsToEliminate[j].append( 1 ) for u in range(0,2)]

        elif self.minimizeVRT == True :
            usedCapIdsPerColumn        = self.__findUsedCapIdsPerColumn__       ()
            capIdsToEliminatePerColumn = self.__findCapIdsToEliminatePerColumn__(usedCapIdsPerColumn       )
            capIdsToEliminate          = self.__findCapIdsToEliminate__         (capIdsToEliminatePerColumn)
            self.__findVRTsToEliminate__           (capIdsToEliminate         )

            #print("self.matchingScheme"       ,self.matchingScheme)
            #print("usedCapIdsPerColumn"       ,usedCapIdsPerColumn)
            #print("capIdsToEliminatePerColumn",capIdsToEliminatePerColumn)
            #print("capIdsToEliminate"         ,capIdsToEliminate)
            #print('self.vRTsToEliminate'      ,self.vRTsToEliminate)        

        else : raise Error(1,'minimizeVRTs() : ')

        return 



    def __setVRTsDistribution__( self ):
        if self.capacitorsNumber % 2 == 0:
            element = [ self.capacitorIds[0:self.capacitorsNumber//2]
                      , self.capacitorIds[self.capacitorsNumber//2:self.capacitorsNumber] ]
        else:
            element = [ self.capacitorIds[0:int( self.capacitorsNumber//2 + 1 )]
                      , self.capacitorIds[int( self.capacitorsNumber//2 + 1 ):self.capacitorsNumber] ]
        u = 0
        for j in range(0,self.matrixDim["columns"]+1) :
            self.vRTsDistribution.append( element[u] ) 
            u = u+1 if u < 1 else 0

        #print('self.vRTsDistribution',self.vRTsDistribution)        

        return



    def __setNetsDistribution__( self ):

        netsList = self.nets[0:len(self.nets)-1] if self.dummyRing == True and self.dummyElement == False else self.nets
        if len(netsList) % 2 == 0:
            element = [ netsList[0:len(netsList)//2]
                      , netsList[len(netsList)//2:len(netsList)] ]
        else:
            element = [ netsList[0:int( len(netsList)//2 + 1 )]
                      , netsList[int( len(netsList)//2 + 1 ):len(netsList)] ]
        u = 0
        netsDistribution = []
        for j in range(0,self.matrixDim["columns"]+1) :
            netsDistribution.append( [element[u][0][0]] ) 
            netsDistribution[j].append( element[u][0][1] ) 
            for k in range(1,len(element[u])):
                netsDistribution[j].append( element[u][k][0] ) 
                netsDistribution[j].append( element[u][k][1] ) 
            u = u+1 if u < 1 else 0

        #print('netsDistribution',netsDistribution)        

        return netsDistribution



    def __setPlatesDistribution__( self ):

        element = [ self.capacitorIds[0:self.capacitorsNumber//2], self.capacitorIds[self.capacitorsNumber//2:self.capacitorsNumber] ] if self.capacitorsNumber % 2 == 0 else [ self.capacitorIds[0:int( self.capacitorsNumber//2 + 1 )], self.capacitorIds[int( self.capacitorsNumber//2 + 1 ):self.capacitorsNumber] ]
        u = 0
        for j in range(0,self.matrixDim["columns"]+1) :
                
            self.platesDistribution.append( ['t' + str(element[u][0])] ) 
                
            #print('self.platesDistribution',self.platesDistribution)        

            if self.dummyElement == False or self.dummyElement == True and j % 2 == 0 and len(element[u]) > 1 : 
                self.platesDistribution[j].append( 'b' + str(element[u][0]) ) 

            for k in element[u][1:len(element[u])]:
                self.platesDistribution[j].append( 't' + str(k) ) 
                
                if self.dummyElement == False or self.dummyElement == True and j % 2 == 0 or  self.dummyElement == True and j % 2 != 0 and k != element[u][len(element[u])-1]  : 
                    self.platesDistribution[j].append( 'b' + str(k) ) 

            u = u+1 if u < 1 else 0

        #print('self.platesDistribution',self.platesDistribution)        

        return



    def gethRoutingTrack_width  ( self ) : return self.hRoutingTrack_width

    def getvRoutingTrackXCenter ( self ) : return self.vRoutingTrackXCenter







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

    nets   = [[t0, b0] , [t1, b1] , [t2, b2] ] # [t3, b3] ]
    #capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 93, matchingMode = True, matchingScheme =  [ ['C2','C2','C2','C2'] , ['C1','C2','C2','C2'] , ['C1','C2','C2','C2'] , ['C1','C2','C2','C1'] ] )
    capacitorInstance = CapacitorStack( Device, [372,1116], 'MIMCap', [0,0], nets,unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,1,1] , [1,1,1,0] , [0,1,1,1]],  dummyRing = True)
#    capacitorInstance = CapacitorStack( Device, [372,1116], 'MIMCap', [0,0], nets,unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,1,1] , [1,1,1,0] , [0,1,1,1] ], dummyRing = True)

  #  capacitorInstance = CapacitorStack( Device, [279], 'MIMCap', [0,0], nets, unitCap = 279 ) #, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,2,1] , [1,1,1,0] , [2,1,1,1] ], dummyElement = True )#dummyRing = True)
 #   capacitorInstance = CapacitorStack( Device, [279,1023, 186], 'MIMCap', [0,0], nets, unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,1,1,0] , [0,1,2,1] , [1,1,1,0] , [2,1,1,1] ], dummyElement = True )#dummyRing = True)
 #   capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 93, matchingMode = True, matchingScheme =  [ ['C2','C2','C1','C2'] , ['C1','C2','C2','C1'] , ['C1','C1','C2','C3'] , ['C1','C1','C2','C3'] ] )

#    capacitorInstance = CapacitorStack( Device, capacitance, 'MIMCap', [0,0], nets, unitCap = 93, matchingMode = True, matchingScheme =  [ ['C1','C4','C1','C2'] , ['C1','C2','C4','C1'] , ['C3','C1','C4','C3'] , ['C3','C1','C2','C3'] ] )
    capacitor         = capacitorInstance.create( ) 
    capWithVRT        = VerticalRoutingTracks( capacitorInstance, capacitor, True ) # )
    capWithVRT.create()

    AllianceFramework.get().saveCell( Device, Catalog.State.Views )

    return True

