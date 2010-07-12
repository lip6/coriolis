from Hurricane import *
import Hurricane
import CRL

import re

###########
## GetXY ##
###########
def pyGetXY ( masterCell, pathname, refname ) :
  '''This function returns the coordinates of a reference thanks to its name and the path of the instance it belongs to'''
  
  ## MasterCell ##
  if pathname != "" :
    myPath     = Path ( masterCell, pathname )
    tInst      = myPath.getTailInstance()
    masterCell = tInst.getMasterCell()

  ## Reference ##
  myRef = None
  for el in masterCell.getReferences():
    if str(el.getName()) == refname :
      myRef = el
      break

  # Error : if no reference found in mastercell
  if myRef == None :
    err =  "\n[Stratus ERROR] GetRefXY : No reference found with name " + refname + " in masterCell " + str(masterCell.getName()) + ".\n"
    raise err
   
  ## Occurrence of the Reference ##
  myOccurrence = Occurrence ( myRef, myPath )
  bb = myOccurrence.getBoundingBox()

  return ( getValue(bb.getXCenter()), getValue(bb.getYCenter()) )
    
##############
## PlaceRef ##
##############
def pyPlaceRef ( cell, name, x, y ) :
  '''This function creates a reference thanks to its coordinates'''
  
  UpdateSession.open()
  
  Reference ( cell
            , name
            , DbU_lambda ( x ), DbU_lambda ( y )
            )

  UpdateSession.close()

##################
## PlaceContact ##
##################
def pyPlaceContact ( net, layer, x, y, width, height ) :
  '''This function creates a contact'''

  UpdateSession.open()
   
  Contact ( net, layer, DbU_lambda(x), DbU_lambda(y), DbU_lambda(width), DbU_lambda(height) )

  UpdateSession.close()

##############
## PlacePin ##
##############
def pyPlacePin ( net, direct, placementStatus, layer, x, y, width, height ) :
  '''This function creates a pin'''
  
  size = 0
  for loc in net.getPins():
    size += 1

  UpdateSession.open()
    
  Pin ( net, str(net.getName()) + "." + str(size)
      , direct
      , placementStatus
      , layer
      , DbU_lambda(x), DbU_lambda(y)
      , DbU_lambda(width), DbU_lambda(height)
      )

  if ( not net.IsSupply() ) :
    CRL.createPartRing ( net.getCell(), net.getName() )

  UpdateSession.close()

##################
## PlaceSegment ##
##################
def pyPlaceSegment ( net, layer, x1, y1, x2, y2, width ) :
  '''This function creates a segment'''
  
  UpdateSession.open()
    
  if   x1 == x2 : Vertical   ( net, layer, DbU_lambda(x1), DbU_lambda(width), DbU_lambda(y1), DbU_lambda(y2) )
  elif y1 == y2 : Horizontal ( net, layer, DbU_lambda(y1), DbU_lambda(width), DbU_lambda(x1), DbU_lambda(x2) )

  UpdateSession.close()
    
###################
## CopyUpSegment ##
###################
def pyCopyUpSegment ( masterCell, pathname, netname, newnet ) :
  '''This function copies the segment of an instance in the current cell'''
  
  ## MasterCell ##
  if pathname != "" :
    myPath         = Path ( masterCell, pathname )
    transformation = myPath.getTransformation()
    
    tInst          = myPath.getTailInstance()
    masterCell     = tInst.getMasterCell()

  ## Net ##
  myNet = None
  for el in masterCell.getNets():
    if str(el.getName()) == netname :
      myNet = el
      break
    
  # Error : if no net found in mastercell
  if myNet == None :
    err =  "\n[Stratus ERROR] CopyUpSegment : No net found with name " + netname + " in masterCell " + str(masterCell.getName()) + ".\n"
    raise err

  mySegment = None
  for mySegment in myNet.getSegments():
    # Error : copy only of CALU segments
    if str ( mySegment.getLayer().getName() ) not in ( "CALU1", "CALU2", "CALU3", "CALU4", "CALU5", "CALU6" ) :
      err = "\n[Stratus ERROR] CopyUpSegment : The segments of net " + netname + " are not of type CALU.\n"
      raise err

    myLayer = mySegment.getLayer()
    myWidth = mySegment.getWidth()
    
    pointSource = mySegment.getSourcePosition()
    pointTarget = mySegment.getTargetPosition()
    
    transformation.ApplyOn ( pointSource )
    transformation.ApplyOn ( pointTarget )
    
    ## Occurrence of the segment ##
    myOccurrence = Occurrence ( mySegment, myPath )
    
    if   pointSource.getY() == pointTarget.getY() :
      Horizontal ( newnet, myLayer, pointSource.getY(), myWidth, pointSource.getX(), pointTarget.getX() )
    elif pointSource.getX() == pointTarget.getX() :
      Vertical   ( newnet, myLayer, pointSource.getX(), myWidth, pointSource.getY(), pointTarget.getY() )

  # Error : if no segment found
  if not mySegment:
    err =  "\n[Stratus ERROR] CopyUpSegment : No segment found with net " + netname + " in masterCell " + str(masterCell.getName()) + ".\n"
    raise err


