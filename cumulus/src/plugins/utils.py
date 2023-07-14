
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2020-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./plugins/utils.py"                            |
# +-----------------------------------------------------------------+


from ..Hurricane import Breakpoint, Box, Vertical, RoutingPad
from ..helpers   import trace


def breakpoint ( editor, level, message ):
    """
    Stop Coriolis when the stop level is at least equal to `level`
    and display `message`. If there is an `editor`, perform a fit.
    """
    if editor:
        editor.fit()
        editor.refresh()
        Breakpoint.stop( level, message )


def getPlugByName ( instance, netName ):
    """
    Search in an Instance for a Plug whose *master net* is named `netName`.
    """
    masterCell = instance.getMasterCell()
    masterNet  = masterCell.getNet( netName )
    if masterNet:
        return instance.getPlug( masterNet )
    return None


def getPlugByNet ( instance, net ):
    """
    Search in a Net for a Plug belonging to `instance`. Per Hurricane
    build rules it is unique.
    """
    for plug in net.getPlugs():
        if plug.getInstance() == instance:
            return plug
    return None


def getRpBb ( instance, netName ):
    """
    Compute the bounding box of the Vertical external component of net `netName`
    of the Instance `instance` with instance transformation applied.

    This function is not robust as it looks for the *first* external Vertical
    segment it can find in the instance master cell.
    """
    bb = Box()
    for net in instance.getMasterCell().getNets():
        if net.isExternal() and net.getName() == netName:
            for component in net.getExternalComponents():
                if isinstance(component,Vertical):
                    bb = component.getBoundingBox()
    instance.getTransformation().applyOn( bb )
    return bb


def showNet ( cell, netName ):
    """Display all the components of Net `netName`, along with their bounding boxes."""
    net = cell.getNet(netName)
    if not net:
        print( ErrorMessage( 3, 'Cell %s doesn\'t have net %s' % (cell.getName(),netName) ) )
        return
  
    print( 'Components of "{}":'.format(netName) )
    for component in net.getComponents():
        print( '| {} bb:{}'.format(component, component.getBoundingBox()) ) 
    return


def hpathToName ( path ):
    """
    Translate a hierarchical path into a string. This function is to be used
    when a flattend name is required. Should be VHDL compliant.
    """
    s = ''
    while not path.isEmpty():
        head = path.getHeadInstance()
        path = path.getTailPath()
        if len(s): s += '_'
        s += head.getName()
    return s


def getInstanceMatching ( cell, pattern, level=0 ):
    """
    getInstanceMatching(): returns first instance with the word being searched.
    """
    for instance in cell.getInstances():
        name = instance.getName()
        if pattern in name:
            trace( 550, '\t{} {} match pattern "{}"\n'.format('  '*level,instance,pattern) )
            return instance
    return None


def rgetInstanceMatching ( cell, path, level=0 ):
    """
    Get the instance designated by path (recursively). The path argument can be
    either a string of instance names separated by dots or directly a list of
    instances names.
 
    It also "reconstructs" the actual full path name of the instances
    found recursively.
    """
    if isinstance(path,str):
        path = path.split( '.' )
    elif not isinstance(path, list):
        raise ErrorMessage( 1, 'rgetInstanceMatching(): "path" argument is ' \
                               'neither a string or a list ({})."' \
                               .format(path) )
    # Find instance at this level.
    instance = getInstanceMatching( cell, path[0], level )
    if instance is None:
        raise ErrorMessage( 1, 'rgetInstanceMatching(): No instance "{}" in cell "{}".' \
                               .format(path[0], cell.getName()) )
    iname = instance.getName()
    # Last instance (leaf search), return it.
    if len(path) == 1:
        return instance, iname
    # Chew down another level, another brick in the wall
    rinstance, rname = rgetInstanceLike( instance.getMasterCell(), path[1:], level+1 )
    # Accumulate the names recursively found "level0.level1.level2..."
    return rinstance, '{}.{}'.format( iname, rname )
