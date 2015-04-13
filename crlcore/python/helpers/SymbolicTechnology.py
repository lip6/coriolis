
# -*- mode:Python -*-

import sys
import os.path
import string
import traceback
import Hurricane
from   Hurricane import DbU
from   Hurricane import DataBase
from   Hurricane import Layer
from   Hurricane import BasicLayer
from   Hurricane import DiffusionLayer
from   Hurricane import TransistorLayer
from   Hurricane import RegularLayer
from   Hurricane import ContactLayer
from   Hurricane import ViaLayer
from   CRL       import AllianceFramework
from   helpers   import ErrorMessage


symbolicFile   = '<No symbolic file specified>'


class SymbolicLayerType ( object ):

    Regular     = 1
    Diffusion   = 2
    Transistor  = 3
    Contact     = 4
    Via         = 5

    def __init__ ( self, code ):
        self._code = code
        return

    def __int__ ( self ):
        return self._code

    def __str__ ( self ):
        if self._code == SymbolicLayerType.Regular:    return 'TypeRegular'
        if self._code == SymbolicLayerType.Diffusion:  return 'TypeDiffusion'
        if self._code == SymbolicLayerType.Transistor: return 'TypeTransistor'
        if self._code == SymbolicLayerType.Contact:    return 'TypeContact'
        if self._code == SymbolicLayerType.Via:        return 'TypeVia'
        return 'TypeUnknown (%d)', self._code

    def __repr__ ( self ):
        return str(self)

    def realLayerLength ( self ):
        if self._code == SymbolicLayerType.Regular:    return 1
        if self._code == SymbolicLayerType.Diffusion:  return 3
        if self._code == SymbolicLayerType.Transistor: return 4
        if self._code == SymbolicLayerType.Contact:    return 5
        if self._code == SymbolicLayerType.Via:        return 3
        return 0


TypeRegular     = SymbolicLayerType(SymbolicLayerType.Regular)
TypeDiffusion   = SymbolicLayerType(SymbolicLayerType.Diffusion)
TypeTransistor  = SymbolicLayerType(SymbolicLayerType.Transistor)
TypeContact     = SymbolicLayerType(SymbolicLayerType.Contact)
TypeVia         = SymbolicLayerType(SymbolicLayerType.Via)


class LayersLUT ( object ):

    Real         = 0x1
    Symbolic     = 0x2
    MissingError = 0x8

    def __init__ ( self ):
        self._realLayers     = {}
        self._symbolicLayers = {}
        return

    def add ( self, layer ):
        if isinstance(layer,BasicLayer): self._realLayers    [ layer.getName() ] = layer
        else:                            self._symbolicLayers[ layer.getName() ] = layer
        return

    def lookup ( self, name, flags=Real|Symbolic ):
        layer = None
        if flags & LayersLUT.Real and self._realLayers.has_key(name):
            layer = self._realLayers[name]
        if flags & LayersLUT.Symbolic and self._symbolicLayers.has_key(name):
            layer = self._symbolicLayers[name]

        if not layer and flags&LayersLUT.MissingError:
            raise ErrorMessage(1,['Layer <%s> is not defined (yet?).'%name])

        return layer


layersLUT = LayersLUT()


def loadRealLayers ( realLayersTable, confFile ):
    global symbolicFile
    symbolicFile = confFile
    technology   = DataBase.getDB().getTechnology()

    entryNo = 0
    for entry in realLayersTable:
        entryNo += 1

        try:
            if len(entry) < 2:
                raise ErrorMessage(1,['Malformed entry in <realLayersTable>.'
                                     ,'Less than two fields: missing name and/or material.'
                                     ,str(entry)
                                     ])
            if len(entry) > 2:
                if entry[1] != BasicLayer.Material.blockage:
                    raise ErrorMessage(1,['Invalid entry in <realLayersTable>.'
                                         ,'Only blockage material can have a third field.'
                                         ,str(entry)
                                         ])
                routingLayer = technology.getBasicLayer(entry[2])
                if not routingLayer:
                    raise ErrorMessage(1,['Incoherency in <realLayersTable> entry at.'
                                         ,'The metal <%s> associated to the blockage doesn\'t exist (yet?).' % entry[2]
                                         ,str(entry)
                                         ])

            basicLayer = BasicLayer.create( technology
                                          , entry[0]
                                          , BasicLayer.Material(entry[1]) )
            layersLUT.add ( basicLayer )
            if len(entry) > 2:
                routingLayer.setBlockageLayer(basicLayer)

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<symbolicLayersTable> at index %d.' % (symbolicFile,entryNo))
    return


def loadSymbolicLayers ( symbolicLayersData, confFile ):
    global symbolicFile
    symbolicFile = confFile
    technology   = DataBase.getDB().getTechnology()

    entryNo = 0
    for entry in symbolicLayersData:
        entryNo += 1

        try:
            if len(entry) != 3:
                raise ErrorMessage(1,['Malformed entry in <symbolicLayersTable>.'
                                     ,'Must contains exactly three fields: ( name, type, (real_layers,) ).'
                                     ,str(entry)
                                     ])
            name, layerType, realLayers = entry

            if not isinstance(layerType,SymbolicLayerType):
                raise ErrorMessage(1,['Invalid entry in <symbolicLayersTable>.'
                                     ,'The layer type code is not valid, should be any of:'
                                     ,'    * TypeRegular'
                                     ,'    * TypeDiffusion'
                                     ,'    * TypeTransistor'
                                     ,'    * TypeContact'
                                     ,'    * TypeVia'
                                     ,str(entry)
                                     ])
            if layerType.realLayerLength() != len(realLayers):
                raise ErrorMessage(1,['Invalid entry in <symbolicLayersTable>.'
                                     ,'Layer of type <%s> contains %d real layers instead of %d.' \
                                          % (layerType,len(realLayers),layerType.realLayerLength())
                                     ,str(entry)
                                     ])

            realLayersArgs = []
            for layerName in realLayers:
                if layerName:
                    realLayersArgs += [ layersLUT.lookup(layerName
                                                        ,LayersLUT.Real
                                                        |LayersLUT.Symbolic
                                                        |LayersLUT.MissingError) ]
                else:
                    realLayersArgs += [ None ]
    
            symbolicLayer = None
    
            if layerType == TypeRegular:
                symbolicLayer = RegularLayer.create(technology,entry[0])
                symbolicLayer.setBasicLayer( *realLayersArgs )
            elif layerType == TypeDiffusion:
                symbolicLayer = DiffusionLayer.create(technology ,entry[0], *realLayersArgs)
            elif layerType == TypeTransistor:
                symbolicLayer = TransistorLayer.create(technology ,entry[0], *realLayersArgs)
            elif layerType == TypeContact:
                symbolicLayer = ContactLayer.create(technology ,entry[0], *realLayersArgs)
            elif layerType == TypeVia:
                symbolicLayer = ViaLayer.create(technology ,entry[0], *realLayersArgs)
    
            layersLUT.add( symbolicLayer )

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<symbolicLayersTable> at index %d.' % (symbolicFile,entryNo))
    return


def loadSymbolicRules ( symbolicRulesTable, confFile ):
    global symbolicFile
    symbolicFile = confFile
    technology   = DataBase.getDB().getTechnology()

    entryNo = 0
    for rule in symbolicRulesTable:
        entryNo += 1

        try:
            if len(rule) != 2:
                raise ErrorMessage(1,['Malformed entry in <symbolicRulesTable>.'
                                     ,'Must contains exactly two fields: ( rule_path, value ).'
                                     ,str(rule)
                                     ])
            if not isinstance(rule[1],int) and not isinstance(rule[1],float):
                raise ErrorMessage(1,['Invalid entry in <symbolicRulesTable>.'
                                     ,'Rule value must be of integer or float type.'
                                     ,str(rule)
                                     ])

            value    = DbU.fromLambda(rule[1])
            elements = rule[0].split('.')
            if len(elements) < 3:
                raise ErrorMessage(1,['Invalid entry in <symbolicRulesTable>.'
                                     ,'Rule name must contains at least three components: \"LAYER.category.dimension\".'
                                     ,str(rule)
                                     ])

            ruleLayer = layersLUT.lookup( elements[0], LayersLUT.Symbolic|LayersLUT.MissingError  )
            subLayer  = layersLUT.lookup( elements[1], LayersLUT.Real )

            if subLayer: ruleTag  = string.join(elements[2:],'.')
            else:        ruleTag  = string.join(elements[1:],'.')

            if   ruleTag == 'extention.cap':   ruleLayer.setExtentionCap  ( subLayer, value )
            elif ruleTag == 'extention.width': ruleLayer.setExtentionWidth( subLayer, value )
            elif ruleTag == 'enclosure':       ruleLayer.setEnclosure     ( subLayer, value )
            elif ruleTag == 'minimum.width':   ruleLayer.setMinimalSize   ( value )
            elif ruleTag == 'minimum.side':    ruleLayer.setMinimalSize   ( value )
            else: 
                raise ErrorMessage(1,['Invalid entry in <symbolicRulesTable>.'
                                     ,'Unknown rule kind: \".%s\", should be any of:' % ruleTag
                                     ,'    * "RULE_HEAD.extention.cap"'
                                     ,'    * "RULE_HEAD.extention.width"'
                                     ,'    * "RULE_HEAD.enclosure"'
                                     ,'    * "RULE_HEAD.minimum.width"'
                                     ,'    * "RULE_HEAD.minimum.side"'
                                     ,str(rule)
                                     ])

        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<symbolicRulesTable> at index %d.' % (symbolicFile,entryNo))
    return


def loadWorkingLayers ( workingLayersTable, confFile ):
    global symbolicFile
    symbolicFile = confFile
    technology   = DataBase.getDB().getTechnology()

    entryNo = 0
    for layerName in workingLayersTable:
        entryNo += 1
        try:
           # This call is just to generate an error if the layer is non-existent.
            layersLUT.lookup(layerName,LayersLUT.Real|LayersLUT.Symbolic|LayersLUT.MissingError)
            technology.setWorkingLayer(layerName)
        except Exception, e:
            ErrorMessage.wrapPrint(e,'In %s:<symbolicRulesTable> at index %d.' % (symbolicFile,entryNo))
    return


def loadViewerConfig ( viewerConfig, confFile ):
    global symbolicFile
    symbolicFile = confFile
    try:
        if viewerConfig.has_key('precision'): DbU.setPrecision(viewerConfig['precision'])
    except Exception, e:
        ErrorMessage.wrapPrint(e,'In %s:<viewerConfig>.')
    return
