
# -*- Mode:Python -*-

import Hurricane
from   Hurricane import DbU
from   Hurricane import Layer
from   Hurricane import BasicLayer
import CRL
from   CRL       import Environment
from   CRL       import AllianceFramework
from   CRL       import RoutingGauge
from   CRL       import RoutingLayerGauge
from   CRL       import CellGauge


def Alliance ( db ):
    print '\n'
    print '==========================================================='
    print 'Checking DbU.'

    print 'DbU.fromLamdba(5)   :', DbU.fromLambda(5)
    print 'DbU.toLamdba(12000) :', DbU.toLambda(12000)

    print '==========================================================='
    print 'Checking LayerMask.'

    mask0 = Layer.Mask(0b000101)
    mask1 = Layer.Mask(0b001110)
    mask3 = mask0 & mask1
    mask4 = mask0 | mask1
    print 'mask0                 :', mask0
    print 'mask1                 :', mask1
    print 'mask0 & mask1         :', mask3
    print 'mask0 | mask1         :', mask4
    print 'mask0.containts(mask1):', mask0.contains(mask1)

    print '==========================================================='
    print 'Checking DataBase.'

    db2 = db.getTechnology().getDataBase()
    if db == db2: print "OK DataBase is unique."
    else:         print "KO DATABASE IS DUPLICATED!"

    print '==========================================================='
    print "Checking layers."

    technology = db.getTechnology()
    rlayer     = technology.getLayer('METAL2')
    print "METAL2"
    print "    Minimal size    :", DbU.toLambda(rlayer.getMinimalSize())
    print "    Minimal spacing :", DbU.toLambda(rlayer.getMinimalSpacing())
    print "    Pitch           :", DbU.toLambda(rlayer.getPitch())
    print "    Cut above       :", rlayer.getCutAbove()
    print "    isSymbolic      :", rlayer.isSymbolic()

    dlayer = technology.getLayer('NDIF')
    print 'NDIF', dlayer.getMask()
    for blayer in dlayer.getBasicLayers():
       print '   ', blayer
    
    tlayer = technology.getLayer('NTRANS')
    print 'NTRANS', tlayer.getMask()
    for blayer in tlayer.getBasicLayers():
       print '   ', blayer
    
    clayer = technology.getLayer('CONT_DIF_N')
    print 'CONT_DIF_N', clayer.getMask()
    for blayer in clayer.getBasicLayers():
       print '   ', blayer

    print '==========================================================='
    print 'Checking RoutingGauge.'

    layer = technology.getLayer('METAL3')
    rg = AllianceFramework.get().getRoutingGauge()
    print 'rg.getName()             :', rg.getName()
    print 'rg.getDepth()            :', rg.getDepth()
    print 'rg.getLayerDepth(METAL3) :', rg.getLayerDepth(layer)
    print 'rg.getLayerGauge(METAL3) :', rg.getLayerGauge(layer)
    print 'rg.getLayerDirection(2)  :', rg.getLayerDirection(2)
    print 'rg.getRoutingLayer(2)    :', rg.getRoutingLayer(2)
    rlg = rg.getLayerGauge(2)
    print 'rlg.getLayer()           :', rlg.getLayer()
    print 'rlg.getBlockageLayer()   :', rlg.getBlockageLayer()
    print 'rlg.getDensity()         :', rlg.getDensity()
    print 'rlg.getType()            :', rlg.getType()
    print 'rlg.getPitch()           :', DbU.toLambda(rlg.getPitch())
    for rlg in rg.getLayerGauges():
        print 'rlg.getLayer()           : ', rlg.getLayer()
    
    #print technology.getName()
    #for layer in technology.getLayers():
    #   print 'TECHNO:', layer

    print '==========================================================='
    print '\n'

    return
