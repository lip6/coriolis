#!/usr/bin/env python3

import sys
import traceback
from   coriolis.Hurricane  import DbU, Breakpoint, Cell, DataBase, Net, Horizontal, Vertical, Contact, Instance, Plug, Transformation, NetExternalComponents, BasicLayer, RegularLayer
from   coriolis.helpers.io import ErrorMessage, WarningMessage, catch
from   coriolis.helpers    import loadUserSettings, setTraceLevel, trace, l, u, n
from   coriolis.helpers.overlay import UpdateSession
from coriolis.helpers.technology import createBL
from   coriolis import CRL

def addGuardRing (cell):
    with UpdateSession():
        #afdn = CRL.AllianceFramework.get()
        technology = DataBase.getDB().getTechnology()
# The Guard ring Deep NWell layer uses TALU8
        BlDnWell   = createBL( technology, 'dnWell' , BasicLayer.Material.blockage )
        dnwell   = RegularLayer.create( technology, 'BLOCKAGE8', BlDnWell )
        technology.setSymbolicLayer( dnwell      .getName() )

        ptie     = technology.getLayer( 'PTIE' )
        metal1   = technology.getLayer( 'METAL1' )
        metal2   = technology.getLayer( 'METAL2' )
        via      = technology.getLayer( 'VIA12' )
        cntp     = technology.getLayer( 'CONT_BODY_P' )
        vss      = cell.getNet('vss')
        aBox     = cell.getAbutmentBox()
        xMin     = aBox.getXMin()
        xMax     = aBox.getXMax()
        yMin     = aBox.getYMin()
        yMax     = aBox.getYMax()
        yCenter  = aBox.getYCenter()
        yHight  = aBox.getHeight()
        xWidth  = aBox.getWidth()

        vssdn    = Net.create(cell,'vssdn')
        dnw      = Net.create(cell,'dnw')
	# tie will connect to vss
        #tie      = Net.create(cell,'tie')

        Horizontal.create(dnw, dnwell, yCenter, (yHight + l(56)), (xMin - l(34)), (xMax + l(34)))
        h1=Horizontal.create(vssdn, ptie, yMax + l(72), l(20), xMin - l(80), xMax + l(80))
        h2=Horizontal.create(vssdn, ptie, yMin - l(72), l(20), xMin - l(80), xMax + l(80))
        Vertical.create(vssdn, ptie, xMax + l(72), l(20), yMin - l(72), yMax + l(72))
        Vertical.create(vssdn, ptie, xMin - l(72), l(20), yMin - l(72), yMax + l(72))
        vss1 = Horizontal.create(vssdn, metal1, yMax + l(72), l(22), xMin - l(84), xMax + l(84))
        vss2 = Horizontal.create(vssdn, metal1, yMin - l(72), l(22), xMin - l(84), xMax + l(84))
        vss3 = Vertical.create(vssdn, metal2, xMax + l(72), l(20), yMin - l(72), yMax + l(72))
        vss4 = Vertical.create(vssdn, metal2, xMin - l(72), l(20), yMin - l(72), yMax + l(72))
        Contact.create(h1, cntp, (xMin+xMax)//2, l(0), xMax + l(85), l(18))
        Contact.create(h2, cntp, (xMin+xMax)//2, l(0), xMax + l(85), l(18))
        Contact.create(h1, via, -l(72), l(0),l(20),l(20))
        Contact.create(h1, via, xMax + l(72), l(0), l(20),l(20))
        Contact.create(h2, via, -l(72), l(0), l(20),l(20))
        Contact.create(h2, via, xMax + l(72), l(0), l(20),l(20))

        vssdn.setType(Net.Type.GROUND)
        vssdn.setExternal(True)
        NetExternalComponents.setExternal(vss1)
        NetExternalComponents.setExternal(vss2)
        NetExternalComponents.setExternal(vss3)
        NetExternalComponents.setExternal(vss4)

        vss.merge(vssdn)

        aBox.inflate(l(80))
        cell.setAbutmentBox(aBox)

        return 



