#!/usr/bin/python

import sys                
from   math           import sqrt, ceil
import numpy
from   ..Hurricane    import *
from   ..CRL          import *
from   ..helpers.io   import ErrorMessage as Error
from   ..helpers      import trace
from   .capacitorunit import CapacitorUnit
from   .              import getRules

def toPhY    ( l ): return DbU.toPhysical  ( l, DbU.UnitPowerMicro )


## Draws the layout of a compact capacitor or a matrix of adjacent identical capacitors. The matrix can be composed of one type of capacitors, either Poly-Poly or Metal-Metal in 350 nm AMS CMOS technology. 
#  When matching mode is off, every adjacent plates of any consecutive elementary capacitors are connected to each other using vertical routing layers. 
#  Otherwise, when matching mode is on, any of elementary capacitors can belong to, \f$ C_1 \f$ or \f$ C_2 \f$ according to the entered matching scheme. Thus, routing is not done in this class.
#  In both modes, the complete routing process is done using the \c RoutCapacitor class.

class CapacitorStack( CapacitorUnit ):
    
    rules = getRules()


    ## This is the class constructor. Basically, the class there are three categories of attributes. There are the ones related to the capacitor caracteristics, its type, dimensions. Also, there are attributes to parametrize the class into matching mode or not and there are other attributes realted to the layout varibales. The class has defaut input values, thus, in this constructor, there are two "sub-constructors" according to the entered input parameters. The class attributes are :
    #
    #  \param   device             The Hurricane AMS device into which the layout is drawn. 
    #  \param   capacitance        The value of the capacitor, expressed in femto Farad (fF).
    #  \param   capacitorType      Can be MIM or PIP type capacitor.
    #  \param   abutmentPosition   Refers to the abscissa (XMin) of the bottom left corner of the abutment Box. 
    #  \param   abutmentBoxYMin    Refers to the ordinate (YMin) of the bottom left corner of the abutment Box.
    #
    # Except the two last arguments, all the parameters are common with the CapacitorUnit class because the \c CapacitorStack constructor calls the mother class constructor to create either a compact capacitor of \c capacitance value or \c rowNumber* \c columnNumber unity capacitors.
    #
    #  \param   rowNumber          Number of rows in the matrix of capacitors.
    #  \param   columnNumber       Number of columns in the matrix of capacitors.

    def __init__( self, device
                      , capacitance
                      , capacitorType
                      , abutmentBoxPosition
                      , nets
                      , unitCap        = 0
                      , matrixDim      = [1,1]
                      , matchingMode   = False
                      , matchingScheme = []
                      , dummyRing      = False
                      , dummyElement   = False ):
        self.device               =  device
        self.capacitorType        =  capacitorType
        self.matrixDim            =  { "columns" : matrixDim[1], "rows" : matrixDim[0] } 
        self.unitCapDim           =  self.__computeCapDim__( unitCap, capacitorType )

        self.doMatrix             =  False
        self.abutmentBox          =  Box()
        self.abutmentBoxPosition  =  { "XMin" : abutmentBoxPosition[0], "YMin" : abutmentBoxPosition[1] }
        self.nets                 =  nets
        self.matchingMode         =  matchingMode
        self.dummyRing            =  dummyRing
        self.dummyElement         =  dummyElement        

        self.capacitorsNumber     =  len(capacitance)

        self.matchingScheme       =  matchingScheme
        self.dummyRingPosition    =  {}
        self.abutmentBox_spacing  =  0
        self.vRoutingTrack_width  =  0

        if self.__areInputDataOK__(capacitance):
          if not self.matchingMode:                
            self.compactCapDim = self.__computeCapDim__( capacitance[0] , capacitorType ) 

            if unitCap == 0:
              self.__initGivenZeroUnitCap__( capacitance[0] )
            elif unitCap != 0 and CapacitorUnit.__isCapacitorUnitOK__( self, self.unitCapDim ):
              self.__initGivenNonZeroUnitCap__( capacitance[0], unitCap )
            else:
              raise Error( 1, [ 'CapacitorStack.__init__(): Impossible to draw the unit capacitor, dimensions are either too large or too small.'
                              , '(width:{0} height:{1})'.format( DbU.getValueString(self.unitCapDim[width ])
                                                               , DbU.getValueString(self.unitCapDim[height]) ) ] )

          else:
            if unitCap == 0:
              self.__initGivenZeroUnitCapInMatchingMode__( capacitance )

            elif unitCap != 0 and CapacitorUnit.__isCapacitorUnitOK__( self, self.unitCapDim ):
              self.__initGivenNonZeroUnitCapInMatchingMode__( capacitance, unitCap )
            else:
              raise Error( 1, [ 'CapacitorStack.__init__(): Impossible to draw the unit capacitor, dimensions are either too large or too small.'
                              , '(width:{0} height:{1})'.format( DbU.getValueString(self.unitCapDim[width ])
                                                               , DbU.getValueString(self.unitCapDim[height]) ) ] )


        return


    def setRules( self ) :

        CapacitorUnit.setRules( self )

        self.minWidth_vRoutingTrack        = CapacitorStack.rules.minWidth_metal3
        self.minSpacing_vRoutingTrack      = CapacitorStack.rules.minSpacingWide1_metal3
        self.minWidth_vRoutingTrackCut     = CapacitorStack.rules.minWidth_cut2
        self.minSpacing_vRoutingTrackCut   = CapacitorStack.rules.minSpacing_cut2
        self.minEnclosure_vRoutingTrackCut = CapacitorStack.rules.minEnclosure_metal3_cut2

        if self.capacitorType == 'MIMCap':
          self.minWidth_hRoutingLayer_topPlate_cut     = CapacitorStack.rules.minWidth_cut2
          self.minEnclosure_hRoutingLayer_topPlate_cut = CapacitorStack.rules.minEnclosure_metal2_cut2
        elif self.capacitorType == 'PIPCap': 
          self.minWidth_hRoutingLayer_topPlate_cut     = CapacitorStack.rules.minWidth_cut1
          self.minEnclosure_hRoutingLayer_topPlate_cut = CapacitorStack.rules.minEnclosure_metal2_cut1

        return 




    def __initMatchingMode__( self ) :

        self.vRoutingTrack_width =   max( self.minWidth_vRoutingTrack, 2*self.minEnclosure_vRoutingTrackCut + self.minWidth_vRoutingTrackCut,self.minWidth_hRoutingLayer_topPlate_cut + 2*self.minEnclosure_hRoutingLayer_topPlate_cut )
        if ( self.capacitorsNumber  % 2 == 0 ) : [factor1 , factor2 ] = [ self.capacitorsNumber    , (self.capacitorsNumber +1) ]  
        if ( self.capacitorsNumber  % 2 != 0 ) : [factor1 , factor2 ] = [ self.capacitorsNumber +1 ,  self.capacitorsNumber +2  ] 
        self.abutmentBox_spacing         =   factor1*self.vRoutingTrack_width + factor2*self.minSpacing_vRoutingTrack 

        return



    def __initMatrixMode__( self, capacitance, unitCap ) :

        [ self.capacitance, self.unitCapacitance , self.doMatrix ] = [ capacitance , unitCap , True ]

        return


    def __initGivenZeroUnitCap__( self, capacitance ):
        if not self.matrixDim['columns']:
          raise Error( 1, 'CapacitorStack.__initGivenZeroUnitCap__(): Requested matrix of *zero* columns.' )
        if not self.matrixDim['rows']:
          raise Error( 1, 'CapacitorStack.__initGivenZeroUnitCap__(): Requested matrix of *zero* rows.' )

        if self.matrixDim.values() == [1,1]:
          self.__isCapacitorUnitOK__( self.compactCapDim )
          self.capacitance = capacitance
          self.unitCapDim  = self.compactCapDim
        else:
          unitCapacitance = capacitance / (self.matrixDim['columns'] * self.matrixDim['rows'])
          unitCapDim      = self.__computeCapDim__( unitCapacitance, self.capacitorType )

          self.__isCapacitorUnitOK__( unitCapDim )
          self.unitCapDim      = unitCapDim
          self.unitCapacitance = unitCapacitance
          self.capacitance     = capacitance
          self.doMatrix        = True
                        
        return



    def __initGivenNonZeroUnitCap__( self, capacitance, unitCap ):

        if ( self.matrixDim["columns"]>1 or self.matrixDim["rows"]>1 ) : # jai donne les dim de la  matrice
            if self.matrixDim.values()[0]*self.matrixDim.values()[1] == capacitance/unitCap : 
                self.__initMatrixMode__( capacitance, unitCap )

            else : raise Error( 1, '__init__() : Matrix dimensions and unit capacitance are not compatible : "capacitance %d divides by unit capacitance %s != columns %d * rows %d ".' %( capacitance, unitCap, self.matrixDim["columns"], self.matrixDim["rows"] ) ) 

        else : # self.matrixDim.values() == [1,1]  : # jai donne ou jai ps donne

            if capacitance == unitCap : #compact
                [ self.capacitance , self.unitCapDim  ] = [ capacitance , self.compactCapDim ]
            elif capacitance != unitCap : #matrice
                self.__initMatrixMode__( capacitance, unitCap )
                self.matrixDim  = {"columns" : int(sqrt(capacitance/unitCap)), "rows" : int(sqrt(capacitance/unitCap)) } # ici mettre toutes les combi si matching mode = [] sinon utiliser la meme combi que matching scheme

            else :  raise Error( 1,'__initGivenNonZeroUnitCap__ : capacitance must be multiple of unit capacitance' )
 
        return



    def __initGivenZeroUnitCapInMatchingMode__( self, capacitance ):
       #print '__initGivenZeroUnitCapInMatchingMode__'

        if self.matrixDim.values() == [1,1] or (self.matrixDim["columns"] == len(self.matchingScheme[0]) and self.matrixDim["rows"] == len(self.matchingScheme)) :

            unitCapList = self.computeUnitCap(capacitance) 

            if len( list(numpy.unique(unitCapList)) ) == 1 : 

                unitCapDim = self.__computeCapDim__( unitCapList[0], self.capacitorType )

                if CapacitorUnit.__isCapacitorUnitOK__(self, unitCapDim) == True : 
                    
                    self.unitCapDim = unitCapDim
                    self.__initMatrixMode__( capacitance, unitCapList[0] )
                    if self.matrixDim.values() == [1,1] : self.matrixDim  = {"columns" : len(self.matchingScheme[0]) , "rows" : len(self.matchingScheme) }

                else : raise Error(1,'__initGivenZeroUnitCapInMatchingMode__() : Impossible to draw unit capacitor, dimensions are either too large or too small, "%s".' % unitCapDim)

            else : raise Error(1,'__initGivenZeroUnitCapInMatchingMode__() : Not all capacitances are multiple of the unit capacitor.')

        else : raise Error(1,'__initGivenZeroUnitCapInMatchingMode__() : Please check compatibility between matrix dimensions and matching scheme dimensions. Both must be equal.')

        return




    def __initGivenNonZeroUnitCapInMatchingMode__( self, capacitance, unitCap ):

        if CapacitorUnit.__isCapacitorUnitOK__(self, self.unitCapDim) == True : 

            if self.matrixDim.values() == [1,1] or (self.matrixDim["columns"] == len(self.matchingScheme[0]) and self.matrixDim["rows"] == len(self.matchingScheme)) :
 
                if self.evaluateUnitCap( capacitance, unitCap ) == True :
                    self.__initMatrixMode__( capacitance, unitCap )
                    if self.matrixDim.values() == [1,1] : self.matrixDim  = {"columns" : len(self.matchingScheme[0]) , "rows" : len(self.matchingScheme) }

                else: raise Error(1,'__initGivenNonZeroUnitCapInMatchingMode__() : Non valid unit capacitor value considering the entered matching scheme. Please make sure that capacitors values are all multiples of unit capacitor.')

            else: raise Error(1,'__initGivenNonZeroUnitCapInMatchingMode__() : Please check compatibility between matrix dimensions and matching scheme dimensions. Both must be equal.')

        else: raise Error(1,'__initGivenNonZeroUnitCapInMatchingMode__() : Impossible to draw unit capacitor, dimensions are either too large or too small, "%s".' % self.unitCapDim)

        return



    def __areMatrixDimOK__( self ): return True if len(self.matrixDim.values()) else False


    def computeUnitCap( self, capacitance ):

        unitCapList = []
        for k in range(0, self.capacitorsNumber):
            unitCapList.append( capacitance[k]/self.capacitorIdOccurence(k) )

        print( self.capacitorsNumber )
        print( 'capacitance', capacitance )
        print( 'unitCapList', unitCapList )
        print( '=============' )
        return unitCapList



    def evaluateUnitCap( self, capacitance, unitCap ):

        state = True
        for k in range(0, self.capacitorsNumber):
            #print('self.capacitorIdOccurence( k )',self.capacitorIdOccurence( k ))
            factor = capacitance[k]/unitCap
            if factor != self.capacitorIdOccurence( k ) : state = False 

        return state



    ## \return True if the drawn capacitor is a compact one. This function is useful when an instance is called in another class. \b Example : when the matrix or the compact capacitors are to be fully routed.
 
    def __isUnitCap__( self ): return True if not self.doMatrix else False


    ## \return \c True if the matching scheme specifications are correct. Specifications are : 
    #  -  Similar number of elements as total number of elementary capacitor in the matrix.  
    #  -  Equal number of affected capacitors to C1 as to C2.
    #  -  Capacitor identifiers equal to '1' or '2' only.
    #  -  Otherwise, the function returns \c False. 

    def __isMatchingSchemeOK__ ( self ):

        state = True
        columsElementsNumber = [ len(self.matchingScheme[k]) for k in range(0,len(self.matchingScheme)) ] 

        if len( list(numpy.unique(columsElementsNumber)) ) > 1 : 
            state = False  
        else :    
            [ matrixDim , matchingSchemeDim ] = [ self.matrixDim["columns"]*self.matrixDim["rows"] , len(self.matchingScheme)*len(self.matchingScheme[0]) ]
            comparaison = [ self.matrixDim[key]>1 for key in self.matrixDim.keys() ]

            if ( True in comparaison ) and ( matchingSchemeDim != matrixDim ) : state = False

        return state 

 
 
    ## \return occurence of capacitor identifier in the entered matching scheme. This is useful to verify that \c self.matchingScheme is correct.

    def capacitorIdOccurence ( self, capacitorIdentifier  ):

        occurence  = sum( element.count(capacitorIdentifier) for element in self.matchingScheme  )        

        return occurence



    def __areInputDataOK__( self, capacitance ) :

        state = False

        if  ( self.__areMatrixDimOK__() == True ) :

            if self.matchingMode in [False, True] and self.dummyRing in [False,True] and self.dummyElement in [False,True]:

                [ matchingSchemeCapIds , capacitanceIds ] = [ list( numpy.unique(self.matchingScheme) ) , range(0,self.capacitorsNumber) ]
                if (len(self.matchingScheme) and set(matchingSchemeCapIds) == set(capacitanceIds) ) or (len(self.matchingScheme) == 0 and len(capacitance) == 1) :
                    if    (len(self.nets) == self.capacitorsNumber + 1 and self.dummyElement == False and self.dummyRing == True ) \
                       or (len(self.nets) == self.capacitorsNumber     and self.dummyElement == False and self.dummyRing == False) \
                       or (len(self.nets) == self.capacitorsNumber     and self.dummyElement == True  and self.dummyRing == True ) \
                       or (len(self.nets) == self.capacitorsNumber     and self.dummyElement == True  and self.dummyRing == False):

                        if ( self.matchingMode == True and self.__isMatchingSchemeOK__() ) or ( self.matchingMode == False and len(self.matchingScheme) == 0 ): 
                            state = True  
                        else: raise Error(1, '__areInputDataOK__(): Please check compatibility of the entered parameters (Matching mode, matching scheme, capacitance). It must be either equal to (False, [], one capacitance value) or ( True, matching scheme, capacitance values as much as there are capacitor ids in matching scheme ). The entered parameters are (%s, %s, %s).' %(self.matchingMode, self.matchingScheme, capacitance) ) #com2 : tester 

                    else : raise Error(1,'__areInputDataOK__() : Nets number, %s, is incompatible with number of capacitors to be drawn, %s.' %(len(self.nets), self.capacitorsNumber))

                else : raise Error(1, '__areInputDataOK__() : Please check compatibility between matching scheme elements, %s, and capacitance indexes, %s. They must be identical. Otherwise, when matching scheme is "False", capacitance indexes must be [0].' %(matchingSchemeCapIds, capacitanceIds) )

            else : raise Error(1,'__areInputDataOK__() : Matching mode, %s, dummy ring, %s, and dummy element, %s, must be all either "True" or "False".' %(self.matchingMode, self.dummyRing, self.dummyElement))

        else : raise Error(1,'__areInputDataOK__(): Both matrix dimensions "%s" must be positive.' % self.matrixDim.keys())

        return state




    ## Draw the compact or matrix of capacitors. First, . Second, . Finally, .

    def create( self, bbMode = False ):

        UpdateSession.open()           
        drawnCapacitor = {}

        self.setRules()

        if self.matchingMode == True : 
            self.__initMatchingMode__()

        self.drawAbutmentBox( self.abutmentBox_spacing )

        if    bbMode == True:
            output =  self.computeBondingBoxDimensions()
        elif  bbMode == False : 
            drawnCapacitor = self.drawCapacitorStack( )
            output = drawnCapacitor
        else:
            raise Error(1, 'create(): The bonding box mode parameter, "bbMode" must be either True or False : %s.' %bbMode )

        UpdateSession.close  ()
   
        return output


    def drawCapacitorStack( self ):

        drawnCapacitor     = []
        bottomPlateRLayer  = CapacitorUnit.getLayers( self )["bottomPlateRLayer"]
        topPlateRLayer     = CapacitorUnit.getLayers( self )["topPlateRLayer"   ]

        if self.doMatrix == True :          
            drawnCapacitor = self.capacitorMatrix ( self.abutmentBox_spacing )      

            if  self.matchingMode == False : 
                if self.dummyRing == True:
                    drawnActiveCapacitor = [drawnCapacitor[1][1:len(drawnCapacitor[1])-1]]
                    for i in range(2,self.matrixDim["rows"]+1):
                        drawnActiveCapacitor.append(drawnCapacitor[i][1:len(drawnCapacitor[1])-1])

                else : drawnActiveCapacitor = drawnCapacitor

                self.drawBottomPlatesRLayers( bottomPlateRLayer, drawnActiveCapacitor )
                self.drawTopPlatesRLayers   ( topPlateRLayer   , drawnActiveCapacitor )                

        else:
            drawnCapacitor = CapacitorUnit( self.device, self.capacitorType, [self.abutmentBoxPosition["XMin"], self.abutmentBoxPosition["YMin"]], self.capacitance ) 
            drawnCapacitor.create( self.nets[0][0], self.nets[0][1] )

        return drawnCapacitor





    ## Iteratively draws a horizontal or vertical line of capacitors according to the \c direction parameter. An exception is raised if the specified direction is different from \c {'horizontal','vertical'}. At every iteration, an instance of the CapacitorUnit class is created and its layout is drawn.
    # \return a list containing the drawn capacitors.
    #  \param dy the vertical position of the first cut in cut line. 
    # \remarks An exception is raised if the specified direction is different from \c {'horizontal','vertical'}

    def capacitorLine( self, dy, abutmentBox_spacing , matchingSchemeRowIndex = 0  ):

        line = [ CapacitorUnit( self.device, self.capacitorType, [self.abutmentBoxPosition["XMin"], dy], capacitance = self.unitCapacitance ) ]
        self.createElementInCapacitorLine( line, matchingSchemeRowIndex,0 )                 
        limit  =   self.matrixDim["columns"] + 2 if self.dummyRing == True else self.matrixDim["columns"]
        for j in range(1, limit ) :
            line.append( CapacitorUnit( self.device, self.capacitorType, [line[j-1].abutmentBox.getXMax() + abutmentBox_spacing, dy], capacitance = self.unitCapacitance ) )
            self.createElementInCapacitorLine( line, matchingSchemeRowIndex,j )                 

        return line


    def createElementInCapacitorLine( self, capacitorList, matchingSchemeRowIndex,capListIndex ):
        
        if self.matchingMode == False :
           capacitorList[capListIndex].create( self.nets[0][0], self.nets[0][1] )
        else :
            if self.dummyRing == True:
                if (matchingSchemeRowIndex == 0 or matchingSchemeRowIndex == self.matrixDim["rows"] + 1 or capListIndex == 0 or capListIndex == self.matrixDim["columns"] + 1)  : 
                    [ t , b ] = [ self.nets[-1][0] , self.nets[-1][1] ]
                else :
                    k = self.matchingScheme[matchingSchemeRowIndex-1][capListIndex-1]
                    [ t , b ] = [ self.nets[k][0] , self.nets[k][1] ]

            else : 
                k = self.matchingScheme[matchingSchemeRowIndex][capListIndex]
                [ t , b ] = [ self.nets[k][0] , self.nets[k][1] ]
            capacitorList[capListIndex].create( t, b )         

        return 

    ## Draws a matrix of identical capacitors. The matrix is iterativelly constructed. At every iteration, a new horizontal line of capacitors is drawn. 
    # \return a nested list of elementary capacitors.

    def capacitorMatrix( self, abutmentBox_spacing = 0 ):

        matrix = [ self.capacitorLine( self.abutmentBoxPosition["YMin"], abutmentBox_spacing,0 ) ]
        limit  =   self.matrixDim["rows"] + 2 if self.dummyRing == True else self.matrixDim["rows"]
        for i in range( 1, limit ):
            matrix.append( self.capacitorLine( matrix[i-1][-1].abutmentBox.getYMax() + abutmentBox_spacing, abutmentBox_spacing, i) )

        return matrix


    def dummyLine( self, direction, dx, dy ):

        dummyList = [ CapacitorUnit( self.device, self.capacitorType, [dx, dy], capacitance = self.unitCapacitance ) ]
        dummyList[0].create( self.nets[-1][0], self.nets[-1][1] )

        if direction == 'vertical':
            for i in range(1, self.matrixDim["rows"] + 2):
                dummyList.append( CapacitorUnit( self.device, self.capacitorType, [dx, dummyList[i-1].abutmentBox.getYMax() + self.abutmentBox_spacing], capacitance = self.unitCapacitance ) )
                dummyList[i].create(self.nets[-1][0], self.nets[-1][1]) 

        elif direction == 'horizontal':
            for j in range(1, self.matrixDim["columns"] + 2):
                dummyList.append( CapacitorUnit( self.device, self.capacitorType, [dummyList[j-1].abutmentBox.getXMax() + self.abutmentBox_spacing, dy], capacitance = self.unitCapacitance ) )
                dummyList[j].create(self.nets[-1][0], self.nets[-1][1]) 

        else : raise Error(1,'dummyLine() : Direction must be either "horizontal" or "vertical".' %direction)

        return dummyList




    def computeAbutmentBoxDimensions( self, abutmentBox_spacing ):

        abutmentBoxDimensions  = {}
        capDim                 = self.getCapDim() 
        
        [widthFactor1 , widthFactor2 ] = [self.matrixDim["columns"] , (self.matrixDim["columns"] - 1)] if self.dummyRing == False else [self.matrixDim["columns"] + 2, (self.matrixDim["columns"] + 1)]
        [heightFactor1, heightFactor2] = [self.matrixDim["rows"   ] , (self.matrixDim["rows"   ] - 1)] if self.dummyRing == False else [self.matrixDim["rows"   ] + 2, (self.matrixDim["rows"   ] + 1)]
        abutmentBoxDimElement  = CapacitorUnit.computeAbutmentBoxDimensions(self, capDim) 
        abutmentBoxWidth       =  widthFactor1 *abutmentBoxDimElement["width" ] + widthFactor2 *abutmentBox_spacing
        abutmentBoxHeight      =  heightFactor1*abutmentBoxDimElement["height"] + heightFactor2*abutmentBox_spacing        

        abutmentBoxDimensions  = { "XMin" : self.abutmentBoxPosition["XMin"], "YMin" : self.abutmentBoxPosition["YMin"], "width" : abutmentBoxWidth , "height" : abutmentBoxHeight,  "surface" :  abutmentBoxWidth*abutmentBoxHeight}

        return abutmentBoxDimensions


    ## Draws the abutment box of the matrix or campact capacitor. 

    def drawAbutmentBox( self, abutmentBox_spacing = 0 ):

        abutmentBoxDimensions = self.computeAbutmentBoxDimensions(abutmentBox_spacing)

        self.abutmentBox      = Box(self.abutmentBoxPosition["XMin"],self.abutmentBoxPosition["YMin"],abutmentBoxDimensions["width"]+self.abutmentBoxPosition["XMin"],abutmentBoxDimensions["height"]+self.abutmentBoxPosition["YMin"])
        self.device.setAbutmentBox( self.abutmentBox )

        return 

    def computeBondingBoxDimensions( self ):

        bondingBoxDimensions  = {}
        abutmentBoxDimensions = self.computeAbutmentBoxDimensions( self.abutmentBox_spacing )
        for key in abutmentBoxDimensions:
            if key != "XMin" and key != "YMin" : bondingBoxDimensions[key] = abutmentBoxDimensions[key]

        return bondingBoxDimensions

    ## Draws the routing layers connecting the bottom plate in the matrix of capacitors. First, the relative positions of the routing layer is of the is extracted from the elementary capacitor instance. Then, its width is computed in a way to connect adjacent plates. Then, the routing layers are iterativelly drawn.
    #  The two borders are . 

    def drawBottomPlatesRLayers( self, bottomPlateRLayer, drawnCapacitor ):

        [ dySourceBottom, dyTargetBottom ] = [ drawnCapacitor[0][0].getBotPlateRLayerYMin   (), drawnCapacitor[-1][0].getBotPlateRLayerYMax() ]        
        if ( self.matrixDim["columns"] > 1 ) :
            bottomPlateRLayer_width = ( drawnCapacitor[0][1].getBotPlateLeftRLayerXMax() - drawnCapacitor[0][0].getBotPlateRightRLayerXMin() ) 
            bottomMetalXCenters     = []

            for j in range( 0,self.matrixDim["columns"]-1 ):

                bottomMetalXCenters.append( drawnCapacitor[0][j].getBotPlateRightRLayerXMin() + bottomPlateRLayer_width/2 )
                Vertical.create ( self.nets[0][1], bottomPlateRLayer, bottomMetalXCenters[j], bottomPlateRLayer_width, dySourceBottom, dyTargetBottom ) 
       
        bordersXMin = [ drawnCapacitor[0][0].getBottomPlateLeftCutXMin(), drawnCapacitor[0][-1].getBottomPlateRightCutXMin() ]
      
        for j in range( 0,2):
            Vertical.create ( self.nets[0][1], bottomPlateRLayer , bordersXMin[j], drawnCapacitor[0][0].getBotPlateRLayerWidth(), dySourceBottom, dyTargetBottom ) 
      
        return
 
     
    ## Draws the routing layers connecting the top plates in the matrix of capacitors. First, the relative positions of the routing layers is of the is extracted from the elementary capacitor instance. Then, its width is computed in a way to connect adjacent plates. Then, the routing layers are iterativelly drawn. 
    #  The two borders are . 
    # \remarks An exception is raised if the number of rows in the matrix is lower than 2.

    def drawTopPlatesRLayers( self, topPlateRLayer, drawnCapacitor):
      
        if ( self.matrixDim["rows"] > 1 ) :
            for j in range( 0,self.matrixDim["columns"] ):
                Vertical.create ( self.nets[0][0], topPlateRLayer , drawnCapacitor[0][j].getTopPlateRLayerXCenter(), drawnCapacitor[0][j].getTopPlateRLayerWidth() , drawnCapacitor[0][0].getTopPlateRLayerYMin(), drawnCapacitor[-1][0].getTopPlateRLayerYMax() ) 

        #else : print('The matrix does not contain enough rows')    #com4 verify if this else is needed  

        return

    ## \return The width of the vertical routing tracks in matching mode. 
    #  \remark This function is useful in matching mode, ie., in \C RoutCapacitor class, when routing the two capacitors.
    def getVerticalRoutingTrack_width ( self ) : return self.vRoutingTrack_width
      

    def getAbutmentBox_spacing        ( self ) : return self.abutmentBox_spacing

    ## \return A dictionary contaning capacitor matrix's dimensions
    def getMatrixDim                  ( self ) : return self.matrixDim      


    def getCapDim                     ( self ) : return self.unitCapDim if self.doMatrix == True else self.compactCapDim

 #   def getMatchingMode               ( self ) : return self.matchingMode


    def getVRoutingTrack_spacing      ( self ) : return self.minSpacing_vRoutingTrack

    def getvRoutingTrack_width        ( self ) : return self.vRoutingTrack_width

    ## \return the matching scheme. The function is useful in  \c RoutMatchedCapacitor class to load \c self.matchingScheme attribute.
    def getMatchingScheme             ( self ) : return self.matchingScheme 


def scriptMain( **kw ):

   editor = None
   if  'editor' in kw and kw['editor']:
       editor = kw['editor']

   UpdateSession.open()
   device = AllianceFramework.get().createCell( 'capacitor' )
   device.setTerminal( True )

   bottomPlate_net0 = Net.create( device, 'b0' )
   bottomPlate_net1 = Net.create( device, 'b1' )
   bottomPlate_net2 = Net.create( device, 'b2' )
   bottomPlate_net3 = Net.create( device, 'b3' )
   bottomPlate_net0.setExternal( True )  
   bottomPlate_net1.setExternal( True )  
   bottomPlate_net2.setExternal( True )  
   bottomPlate_net3.setExternal( True )  
   b0  = device.getNet("b0")
   b1  = device.getNet("b1")
   b2  = device.getNet("b2")
   b3  = device.getNet("b3")

   topPlate_net0    = Net.create( device, 't0' )
   topPlate_net1    = Net.create( device, 't1' )
   topPlate_net2    = Net.create( device, 't2' )
   topPlate_net3    = Net.create( device, 't3' )
   topPlate_net0.setExternal( True )  
   topPlate_net1.setExternal( True )  
   topPlate_net2.setExternal( True )  
   topPlate_net3.setExternal( True )  
   t0      = device.getNet("t0")
   t1     = device.getNet("t1")
   t2     = device.getNet("t2")
   t3     = device.getNet("t3")
   if editor:
       UpdateSession.close()
       editor.setCell( device )
       editor.fit()  
       UpdateSession.open()
   nets   = [[t0, b0] , [t1, b1] , [t2, b2] ] # [t3, b3] ]
   capacitorInstance = CapacitorStack( device, [750,750], 'MIMCap', [0,0], nets,unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ [1,0,1,0] , [0,1,0,1] , [1,0,1,0] , [0,1,0,1] ], dummyRing = True)
  #capacitorInstance = CapacitorStack( device, [1488], 'MIMCap', [0,0], nets,unitCap = 93, matrixDim = [4,4], dummyRing = True)
  #capacitorInstance = CapacitorStack( device, {"C1" : 558, "C2" : 558, "C3" : 372}, 'MIMCap', [0,0], nets, unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ ['C2','C1','C2','C1'] , ['C1','C2','C1','C2'] , ['C2','C1','C2','C1'] , ['C3','C3','C3','C3'] ])
  #capacitorInstance = CapacitorStack( device, {"C1" : 558, "C2" : 558, "C3" : 186, "C4" : 186}, 'MIMCap', [0,0], nets, unitCap = 93, matrixDim = [4,4], matchingMode = True, matchingScheme =  [ ['C2','C1','C2','C1'] , ['C1','C2','C1','C2'] , ['C2','C1','C2','C1'] , ['C3','C3','C4','C4'] ])

   capacitor = capacitorInstance.create()
   #print(toPhY(capacitor["width"])) 
   #print(toPhY(capacitor["height"])) 

   AllianceFramework.get().saveCell( device, Catalog.State.Views )

   return True
