# -*- coding: utf-8 -*-

import copy
import datetime
from   ..Hurricane  import DataBase, UpdateSession, DbU, Box, Net, \
                           Horizontal, Vertical, Contact, Pad,     \
                           NetExternalComponents
from   ..CRL        import AllianceFramework
from   ..           import Constant
from   ..helpers.io import ErrorMessage as Error
from   ..helpers    import setTraceLevel, trace
from   ..Analog     import Device
from   .            import getRules, rules, dtr, toUnity, adjustOnGrid

#setTraceLevel( 100 )

def traceMT ( mt ):
  trace( 100, '+', '\tMeta-Transistor Topological Datas\n' )
  trace( 100, '\t  +-------------------------------------------------+\n' )
  trace( 100, '\t  | Gate name  |               %20s |\n' % mt['gate']  )
  trace( 100, '\t  +------------+------------------------------------+\n' )
  trace( 100, '\t  | NF         |                         %10d |\n' % mt['NF']  )
  trace( 100, '\t  | MIN        |                         %10r |\n' % mt['MIN']  )
  trace( 100, '\t  +------------+-----------------------+------------+\n' )
  trace( 100, '\t  | Style      | NSint                 | %10d |\n' % mt['style.NSint'] )
  trace( 100, '\t  |            | DSint                 | %10d |\n' % mt['style.NDint'] )
  trace( 100, '\t  |            | NSend                 | %10d |\n' % mt['style.NSend'] )
  trace( 100, '\t  |            | DSend                 | %10d |\n' % mt['style.NDend'] )
  trace( 100, '\t  +------------+---------+-------------+------------+\n' )
  trace( 100, '\t  | Stress     | BSIM4   | SAeff       | %10.2g |\n' % mt['stress.SAeff_Bsim4'  ] )
  trace( 100, '\t  |            |         | SBeff       | %10.2g |\n' % mt['stress.SBeff_Bsim4'  ] )
  trace( 100, '\t  |            |         | SAinv       | %10.2g |\n' % mt['stress.SAinv_Bsim4'  ] )
  trace( 100, '\t  |            |         | SBinv       | %10.2g |\n' % mt['stress.SBinv_Bsim4'  ] )
  trace( 100, '\t  |            |         | SBinv       | %10.2g |\n' % mt['stress.SBinv_Bsim4'  ] )
  trace( 100, '\t  |            |         | alpha       | %10.2g |\n' % mt['stress.alpha'        ] )
  trace( 100, '\t  |            |         | alphaInv    | %10.2g |\n' % mt['stress.alphaInv'     ] )
  trace( 100, '\t  |            |         | LODeffect   | %10.2g |\n' % mt['stress.LODeffect'    ] )
  trace( 100, '\t  |            +---------+-------------+------------+\n' )
  trace( 100, '\t  |            | Crolles | SAeff       | %10.2g |\n' % mt['stress.SAeff_Crolles'] )
  trace( 100, '\t  |            |         | SBeff       | %10.2g |\n' % mt['stress.SBeff_Crolles'] )
  trace( 100, '\t  |            |         | SAinv       | %10.2g |\n' % mt['stress.SAinv_Crolles'] )
  trace( 100, '\t  |            |         | SBinv       | %10.2g |\n' % mt['stress.SBinv_Crolles'] )
  trace( 100, '\t  |            |         | po2actEff   | %10.2g |\n' % mt['stress.po2actEff_Crolles'] )
  trace( 100, '\t  +------------+---------+-------------+------------+\n' )
  trace( 100, '\t  | Parasitics | ASeff                %10.2g m2 |\n' % mt['parasitics.ASeff'] )
  trace( 100, '\t  |            | PSeff                %10.2g m  |\n' % mt['parasitics.PSeff'] )
  trace( 100, '\t  |            | ADeff                %10.2g m2 |\n' % mt['parasitics.ADeff'] )
  trace( 100, '\t  |            | PDeff                %10.2g m  |\n' % mt['parasitics.PDeff'] )
  trace( 100, '\t  +------------+------------------------------------+\n' )
  trace( 100, '-' )
  return


class Wiring ( object ):

  def __init__ ( self, device, wiring ):
    chain = wiring.split('.')
    net   = chain[0]
    side  = chain[1]
    if net == 'B' and device.isBulkConnected(): net = 'S'

    self.net      = device.getNet( net )
    self.topTrack = -1
    self.botTrack = -1
    self.wTrack   =  1
    if len(chain) >= 3: self.wTrack = int(chain[2])
    length = len(side)
    for i in range( 0, length//2, 2 ):
      track = -2
      if side[i+1] != 'X': track = int( side[i+1] )

      if side[i] == 't': self.topTrack = track
      if side[i] == 'b': self.botTrack = track
      if side[i] == 'z': pass
    return

  def isDrain  ( self ): return self.net.getName()[0] == 'D'
  def isSource ( self ): return self.net.getName()[0] == 'S'
  def isGate   ( self ): return self.net.getName()[0] == 'G'
  def isBulk   ( self ): return self.net.getName()[0] == 'B'

  def isTop ( self ):
    if self.topTrack != -1: return Stack.NorthBulk
    return None

  def isBot  ( self ):
    if self.botTrack != -1: return Stack.SouthBulk
    return None

  def getTop    ( self ):        return self.topTrack
  def getBot    ( self ):        return self.botTrack
  def setTop    ( self, track ): self.topTrack = track
  def setBot    ( self, track ): self.botTrack = track
  def getWTrack ( self ):        return self.wTrack

  def __str__ ( self ):
    s = self.net.getName()
    if not self.isTop() and not self.isBot(): s += 'z'
    else:
      if self.isBot(): 
        s += '.b%d' % self.botTrack 
        s += '.%d' % self.wTrack
      if self.isTop():
        s += '.t%d' % self.topTrack 
        s += '.%d' % self.wTrack
    return s


class Bulk ( object ):

  def __init__ ( self, stack, axis, usource, utarget, flags ):
    self.stack         = stack
    self.axis          = axis
    self.usource       = usource
    self.utarget       = utarget
    self.flags         = flags
    self.ucontacts     = []
    self.ucontactsBulk = []
    return


  def addContact ( self, upos, enabled=True ):
   #if self.ucontacts:
   #  trace( 100, '\taddContact [ ' )
   #  for pos, en in self.ucontacts: trace( 100, "%s+%s " % (DbU.getValueString(pos),en) )
   #  trace( 100, '] inserting: %s+%s\n' % (DbU.getValueString(upos),enabled) )

    if len(self.ucontacts) == 0 or self.ucontacts[-1][0] < upos:
      self.ucontacts.append( (upos,enabled) )
    else:
      for i in range(len(self.ucontacts)):
        if self.ucontacts[i][0] > upos:
          self.ucontacts.insert( i, (upos,enabled) )
          return
        if self.ucontacts[i][0] == upos:
          return
      self.ucontacts.append( (upos,enabled) )
    return


  def _computeContacts ( self, ucontacts, pitch ):
   #trace( 100, ',+', '\tBulk._computeContacts() @%s\n' % DbU.getValueString(self.axis) )
   #trace( 100,       '\thorPitch:   %s\n' % DbU.getValueString(self.stack.horPitch) )
   #trace( 100,       '\tbulk pitch: %s\n' % DbU.getValueString(pitch) )
   #trace( 100,       '\tusource:    %s\n' % DbU.getValueString(self.usource) )
   #trace( 100,       '\tutarget:    %s\n' % DbU.getValueString(self.utarget) )

    icontact = 0
    ucontact = self.usource
    while ucontact <= self.utarget:
     #trace( 100, '\tucontact: %s\n' % DbU.getValueString(ucontact) )

      if len(ucontacts) == 0:
        ucontacts.insert( 0, (ucontact,True) )
      else:
        if icontact == 0 and ucontact < ucontacts[0][0]:
          if ucontacts[0][0] - ucontact >= pitch:
            ucontacts.insert( 0, (ucontact,True) )
        else:
          while icontact+1 < len(ucontacts) and ucontacts[icontact+1][0] < ucontact:
            if icontact+1 == len(ucontacts): break
            icontact += 1

          if ucontact - ucontacts[icontact][0] >= pitch:
            if    icontact + 1 >= len(ucontacts) \
               or ucontacts[icontact+1][0] - ucontact >= pitch:
              ucontacts.insert( icontact+1, (ucontact,True) )
            icontact += 1
          else:
            ucontact = ucontacts[icontact][0]

      ucontact += pitch

    trace( 100, '-' )
    return


  def computeContacts ( self ):
   #self.ucontactsBulk = copy.deepcopy( self.ucontacts )

    pitch =             self.stack.minWidth_metal1 + self.stack.minSpacing_metal1
    pitch = max( pitch, self.stack.minWidth_cut0   + self.stack.minEnclosure_metal1_cut0*2 + self.stack.minSpacing_metal1 )

    if self.stack.isVH:
      self._computeContacts( self.ucontactsBulk, pitch )
    
    pitch = max( pitch, self.stack.minWidth_cut1   + self.stack.minEnclosure_metal1_cut1*2 + self.stack.minSpacing_metal1 )
    pitch = max( pitch, self.stack.minWidth_metal2 + self.stack.minSpacing_metal2 )
    pitch = max( pitch, self.stack.minWidth_cut1   + self.stack.minEnclosure_metal2_cut1*2 + self.stack.minSpacing_metal2 )
    pitch = max( pitch, self.stack.minWidth_cut2   + self.stack.minEnclosure_metal2_cut2*2 + self.stack.minSpacing_metal2 )
    if self.stack.isVH:
      pitch = max( pitch, self.stack.minWidth_metal3 + self.stack.minSpacing_metal3 )
      pitch = max( pitch, self.stack.minWidth_cut2   + self.stack.minEnclosure_metal3_cut2*2 + self.stack.minSpacing_metal3 )

   #trace( 100, '\tOriginal ucontacts[]\n' )
   #for ucontact, enabled in self.ucontacts:
   #  trace( 100, '\tucontact: %s enabled:%d\n' % (DbU.getValueString(ucontact),enabled) )
   #trace( 100, '\n' )

    self._computeContacts( self.ucontacts, pitch )
    if not self.stack.isVH:
      self.ucontactsBulk = self.ucontacts
    return


  def doLayout ( self ):
    active    = rules.getRealLayer( 'active' )
    metal1    = rules.getRealLayer( 'metal1' )
    metal2    = rules.getRealLayer( 'metal2' )
    metal3    = rules.getRealLayer( 'metal3' )
    cut0      = rules.getRealLayer( 'cut0' )
    cut1      = rules.getRealLayer( 'cut1' )
    cut2      = rules.getRealLayer( 'cut2' )
    bulkNet   = self.stack.bulkNet

    self.computeContacts()

   #trace( 100, '\tBulk.doLayout() @%s\n' % DbU.getValueString(self.axis) )
    
    if self.flags & (Stack.NorthBulk | Stack.SouthBulk):
      xsource = self.usource
      xtarget = self.utarget
      if not self.stack.hasWestBulk(): xsource = self.ucontacts[ 0][0]
      if not self.stack.hasEastBulk(): xtarget = self.ucontacts[-1][0]

      width = self.stack.minWidth_metal1
      width = max( width, self.stack.minEnclosure_metal1_cut0*2 + self.stack.minWidth_cut0 )
      Horizontal.create( bulkNet
                       , metal1
                       , self.axis
                       , width
                       , xsource-self.stack.wire1Width//2
                       , xtarget+self.stack.wire1Width//2 )
      width = self.stack.minWidth_cut0 + 2* self.stack.minEnclosure_active_cut0
      Horizontal.create( bulkNet
                       , active
                       , self.axis
                       , width
                       , self.usource-width//2
                       , self.utarget+width//2 )
      width += 2* self.stack.minEnclosure_bImplant_active
      Horizontal.create( bulkNet
                       , self.stack.bImplantLayer
                       , self.axis
                       , width
                       , self.usource-width//2
                       , self.utarget+width//2 )

      for xcontact, enabled in self.ucontactsBulk:
        if enabled:
          Contact.create( bulkNet
                        , cut0
                        , xcontact
                        , self.axis
                        , self.stack.minWidth_cut0
                        , self.stack.minWidth_cut0 )

          cutBb = Box( xcontact, self.axis, xcontact, self.axis  ) 

         #metal1EnclBb = Box( cutBb ).inflate( self.stack.minEnclosure_metal1_cut0 + self.stack.minWidth_cut0/2 )
         #Pad.create( bulkNet, metal1, metal1EnclBb )

      for xcontact, enabled in self.ucontacts:
        trace( 100, '\tucontact: %s enabled:%d\n' % (DbU.getValueString(xcontact),enabled) )

        if enabled:
          Contact.create( bulkNet
                        , cut1
                        , xcontact
                        , self.axis
                        , self.stack.minWidth_cut1
                        , self.stack.minWidth_cut1 )
          if self.stack.isVH:
            Contact.create( bulkNet
                          , cut2
                          , xcontact
                          , self.axis
                          , self.stack.minWidth_cut2
                          , self.stack.minWidth_cut2 )

          cutBb = Box( xcontact, self.axis, xcontact, self.axis  ) 

          metal1EnclBb = Box( cutBb ).inflate( self.stack.minEnclosure_metal1_cut1 + self.stack.minWidth_cut1//2 )
          Pad.create( bulkNet, metal1, metal1EnclBb )
          
          metal2EnclBb = Box( cutBb ).inflate( self.stack.minEnclosure_metal2_cut1 + self.stack.minWidth_cut1//2 )
          if self.stack.isVH:
            metal2EnclBb.merge( Box( cutBb ).inflate( self.stack.minEnclosure_metal2_cut2 + self.stack.minWidth_cut2//2 ) )

          Pad.create( bulkNet, metal2, metal2EnclBb )
            
          if self.stack.isVH:
            metal3EnclBb = Box( cutBb ).inflate( self.stack.minEnclosure_metal3_cut2 + self.stack.minWidth_cut2//2 )
            Pad.create( bulkNet, metal3, metal3EnclBb )
    
    if self.flags & (Stack.EastBulk | Stack.WestBulk):
      metal1Width = self.stack.minWidth_metal1
      metal1Width = max( metal1Width, self.stack.minWidth_cut0 + 2*self.stack.minEnclosure_metal1_cut0 )
      metal1Width = max( metal1Width, self.stack.minWidth_cut1 + 2*self.stack.minEnclosure_metal1_cut1 )
      
      Vertical.create( bulkNet
                     , metal1
                     , self.axis
                     , metal1Width
                     , self.usource-self.stack.wire1Width//2
                     , self.utarget+self.stack.wire1Width//2 )
      width = self.stack.minWidth_cut0 + 2*self.stack.minEnclosure_active_cut0
      Vertical.create( bulkNet
                     , active
                     , self.axis
                     , width
                     , self.usource-width//2
                     , self.utarget+width//2 )
      width += 2*self.stack.minEnclosure_bImplant_active
      Vertical.create( bulkNet
                     , self.stack.bImplantLayer
                     , self.axis
                     , width
                     , self.usource-width//2
                     , self.utarget+width//2 )

      for icontact in range(len(self.ucontacts)):
        if    (icontact   == 0                   and self.stack.hasSouthBulk()) \
           or (icontact+1 == len(self.ucontacts) and self.stack.hasNorthBulk()): continue
        Contact.create( bulkNet
                      , cut0
                      , self.axis
                      , self.ucontacts[icontact][0]
                      , self.stack.minWidth_cut0
                      , self.stack.minWidth_cut0 )
    return


## Draw a Stack of Transistors.
#
#               A Stack of Transistors is a set of transistor put into a regular
#               band and connected through their sources/drains. All share the
#               exact same W & L. The way they are connecteds defines what functionnality
#               the Stack implement.
#               
#               The abutment box of the stack is adjusted so that both height and width
#               are even multiples of the track pitches, so the device can be easily
#               placed and handled by the mixed router. The extra space needed for
#               padding is added around the active area. Due to the presence of tracks
#               at the top and bottom of the stack, the active area will be horizontally
#               centered but \b not vertically.
#               
#               The drawing of the stack is controlled through a set of variables
#               (attributes) that allows to create it regardless of the technology.
#               The technology is taken into account in the way those variables are
#               computed and, obviously, their values. The following schematics details
#               the main stack drawing variables along with their computations.
#               
#  \section     secStackLayout  Stack Layout
#               
#  \subsection  secGatePitch  Gate pitch
#               
#               - \c self.gatePitch : the pitch of transistors gates, inside the stack.
#                 It also applies to dummy transistors.
#               
#               \image html   gate-pitch-1.png "Gate Pitch"
#               \image latex  gate-pitch-1.pdf "Gate Pitch" width=.9\linewidth
#               
#               
#  \subsection  secActiveSideWidth  Active Side Width
#               
#               - \c self.activeSideWidth : the distance between the axis of the last
#                 transistor gate (on the left or right) and the edge of the active
#                 area (\e not the diffusion area).
#               
#               \image html   active-side-width-1.png "Active Side Width"
#               \image latex  active-side-width-1.pdf "Active Side Width" width=.9\linewidth
#               
#               
#  \subsection  secHTrackDistance  H-Track Distance
#               
#               - \c self.hTrackDistance : the minimal distance between either the top or
#                 bottom edge of the active area and the \e axis of the first track.
#               
#               \image html   htrack-distance-1.png "H-Track distance"
#               \image latex  htrack-distance-1.pdf "H-Track distance" width=.9\linewidth
#               
#               
#  \subsection  secOverallVariables  BoundingBox & Overall Variables
#               
#               - \c self.xpitches : the number of vertical track pitches needed to fully
#                 enclose the active area.
#               - \c self.ypitches : the number of horizontal track pitches needed to fully
#                 enclose the active area.
#               - \c self.activeOffsetX & \c self.activeOffsetY : the offsets of the active area
#                 from the bottom left corner of the abutment box.
#               - \c self.diffusionWidth & \c self.diffusionHeight are the minimun dimensions
#                 required to fit the active area.
#               - \c self.topTracksNb() : the number of tracks at the top of the stack.
#               - \c self.botTracksNb() : the number of tracks at the bottom of the stack.
#               
#               \image html   stack-layout-3.png "General Stack Layout"
#               \image latex  stack-layout-3.pdf "General Stack Layout" width=.9\linewidth
#
#
# \section      secWiringSpecs  Wiring Specifications
#
#               Stack routing is done through vertical \c metal1 wires coming from the
#               gates and diffusions areas and \c metal2 horizontal wires that can be
#               either above or below the active area. \c metal2 wires (or track) goes
#               through the whole stack and are assigned to one net only. A net will
#               have at least one track above or below and may have both.
#
#               The connections to the diffusions areas and gates of the various
#               fingers are specified through a list. The stack is made of a regular
#               alternation of diffusions and gates. The list tells, for each one
#               starting from the left, to which net and track they are connected.
#               For a stack of \f$NFs\f$ transistor fingers, the must wiring specification
#               must contains \f$ 3 + (NFs-1) \times 2\f$ elements. The list is given
#               through one \e string with each elements separated by one or more
#               whitespace. The syntax for \e one element is detailed \ref secAtomicWiring.
#
#               <b>Track numbering scheme</b>
#
#               Tracks above (top) the active area and below (bottom) each have their
#               own numbering. In both case, the count start \e from the active area.
#               This, the top tracks will be numbered by increasing Y and the bottom
#               tracks by \e decreasing Y.
#
#               <b>Track/Net assignement</b>
#
#               The track/net assignement is deduced from the atomic wiring specifications.
#               It also allows to compute the total number of tracks needed above and
#               below the active area.
#               
#               \image html   wiring-spec-2.png "Wiring Specification"
#               \image latex  wiring-spec-2.pdf "Wiring Specification" width=.9\linewidth
#
# \subsection   secAtomicWiring  Atomic Wiring Specification
#
#               An atomic wiring specification has the same syntax for either diffusions
#               or gates. It \e must not comprise any whitespaces. it is made of the
#               following parts:
#               - The net name to connect to.
#               - Whether the track is above the active area (\c "t") or below (\c "b").
#                 The special case (\c "z") means that this element must be left
#                 unconnected (is such case possible?).
#               - The number of the track.
#               
#               \image html   wiring-spec-1.png "Atomic Wiring Specification"
#               \image latex  wiring-spec-1.pdf "Atomic Wiring Specification" width=.4\linewidth
#
#
# \section      secStackImplDetails   Stack Implementation Details
#
#               The \c __setattr__() and \c __getattr__ functions have been redefined
#               so that the technological values (rules) can be accessed has normal
#               attributes of the Stack class, in addition to the regular ones.


class Stack ( object ):

  NorthBulk       = 0x0004
  SouthBulk       = 0x0008
  EastBulk        = 0x0010
  WestBulk        = 0x0020
  RingBulk        = NorthBulk|SouthBulk|EastBulk|WestBulk
  SourceIsolated  = 0x0040
  SourceShared    = 0x0080
  SourceMerged    = 0x0100
  DrainIsolated   = 0x0200
  DrainShared     = 0x0400
  DrainMerged     = 0x0800

  rules = getRules()


  def __setattr__ ( self, attribute, value ):
    if hasattr(Stack.rules,attribute):
      print( '[ERROR] Stack.{} attribute is read-only (ignored).'.format(attribute) )
      return
    self.__dict__[attribute] = value
    return


  def __getattr__ ( self, attribute ):
    if attribute.find('_') != -1:
      if attribute in [ 'minEnclosure_nImplant_active'
                      , 'minEnclosure_pImplant_active'
                      ]:
        raise Error( 3, 'Stack.__getattr__(): Do not access directly rule "%s".' % attribute )
      
      if self.isNmos():
        if attribute == 'minEnclosure_tImplant_active': return getattr(Stack.rules,'minEnclosure_nImplant_active')
        if attribute == 'minEnclosure_bImplant_active': return getattr(Stack.rules,'minEnclosure_pImplant_active')
      else:
        if attribute == 'minEnclosure_tImplant_active': return getattr(Stack.rules,'minEnclosure_pImplant_active')
        if attribute == 'minEnclosure_bImplant_active': return getattr(Stack.rules,'minEnclosure_nImplant_active')
      
      return getattr(Stack.rules,attribute)

    if attribute in self.__dict__: return self.__dict__[attribute]
    return None


  @staticmethod
  def _addToTracks ( tracks, trackNb, net ):
    if trackNb < len(tracks):
      if tracks[trackNb] and tracks[trackNb] != net:
        raise Error( 3, 'Stack._addToTracks(): Track %d is already assigned to net "%s" (requested by net "%s")'
                         % ( trackNb, tracks[trackNb].getName(), net.getName() ) )
      tracks[ trackNb ] = net
    else:
      for i in range( len(tracks), trackNb ): tracks.append( None ) 
      tracks.append( net )
    return

  @staticmethod
  def _addToWTracks ( tracks, trackNb, net ):
    if trackNb < len(tracks):
      if tracks[trackNb] and tracks[trackNb] != net:
        raise Error( 3, 'Stack._addToTracks(): Track %d is already assigned to net "%s" (requested by net "%s")'
                         % ( trackNb, tracks[trackNb].getName(), net.getName() ) )
      tracks[ trackNb ] = net
    else:
      for i in range( len(tracks), trackNb ): tracks.append( 0 ) 
      tracks.append( net )
    return


  @staticmethod
  def toGeomod ( geoFlags, nf ): 
    geomod = 0

   # Check for geomod 9 & 10 first, because they require the additionnal
   # condition that "nf" must be even. They lead to simpler equations in
   # The BSIM4 model.
   #if nf%2 == 0:

    if   geoFlags == (Stack.SourceIsolated | Stack.DrainIsolated): geomod = 0
    elif geoFlags == (Stack.SourceIsolated | Stack.DrainShared  ): geomod = 1 
    elif geoFlags ==  Stack.SourceIsolated:                        geomod = 1
    elif geoFlags == (Stack.SourceShared   | Stack.DrainIsolated): geomod = 2
    elif geoFlags ==                         Stack.DrainIsolated : geomod = 2
    elif geoFlags == (Stack.SourceShared   | Stack.DrainShared  ): geomod = 3
    elif geoFlags ==  Stack.SourceShared:                          geomod = 3
    elif geoFlags ==                         Stack.DrainShared   : geomod = 3
    elif geoFlags ==  0:                                           geomod = 3
    elif geoFlags == (Stack.SourceIsolated | Stack.DrainMerged  ): geomod = 4
    elif geoFlags == (Stack.SourceShared   | Stack.DrainMerged  ): geomod = 5
    elif geoFlags ==                         Stack.DrainMerged   : geomod = 5
    elif geoFlags == (Stack.SourceMerged   | Stack.DrainIsolated): geomod = 6
    elif geoFlags == (Stack.SourceMerged   | Stack.DrainShared  ): geomod = 7
    elif geoFlags ==  Stack.SourceMerged:                          geomod = 7
    elif geoFlags == (Stack.SourceMerged   | Stack.DrainMerged  ): geomod = 8
   # Modes 9 & 10 are not clear to me, but we shouldn't need it ever.

    if geomod in [0, 4, 6, 8] and nf%2 == 0:
      print( '[WARNING] Stack.toGeomod(): In geomod {}, NF must be odd ({})'.format(geomod,nf))

    return geomod


  ## <b>[API]</b> Constructor
  #  
  #   param rules   The physical rule set.
  #  \param device  The Hurricane AMS device into which the layout will be drawn.
  #  \param NERC    Number of contact rows in external (first & last) diffusion connectors.
  #  \param NIRC    Number of contact rows in middle diffusion connectors.
  #   param w       The \b width of every transistor of the stack (aka \e fingers).
  #   param L       The \b length of every transistor.
  #   param NFs     The total number of fingers (dummies includeds).
  #   param NDs     The number of dummies to put on each side of the stack. 
  
  def __init__ ( self, device, NERC, NIRC ):
    self.dimensioned     = False
    self.device          = device
    self.w               = adjustOnGrid(device.getW() // device.getM())
    self.L               = adjustOnGrid(device.getL())
    self.NDs             = device.getExternalDummy()                    # Number of Dummies at each end of the stack.
    self.NFs             = device.getM() * self.metaTnb() + self.NDs*2  # Total number of Fingers (including dummies).
    self.NERC            = NERC
    self.NIRC            = NIRC
    self.wirings         = []
    self.topTracks       = []
    self.botTracks       = []
    self.flags           = 0
    self.bulks           = [ None, None, None, None ]  # 0:North, 1:South, 2:East, 3:West.
    self.metaTransistors = {}

    self.topWTracks      = [] # widths top tracks
    self.botWTracks      = [] # widths bot tracks

    if device.isBulkConnected(): self.bulkNet = device.getNet( 'S' )
    else:                        self.bulkNet = device.getNet( 'B' )

    bulkType = self.device.getBulkType()
    if bulkType & 0x0001: self.flags |= Stack.NorthBulk
    if bulkType & 0x0002: self.flags |= Stack.SouthBulk
    if bulkType & 0x0004: self.flags |= Stack.EastBulk
    if bulkType & 0x0008: self.flags |= Stack.WestBulk

    if self.isNmos():
      self.tImplantLayer = rules.getRealLayer( 'nImplant' )
      self.bImplantLayer = rules.getRealLayer( 'pImplant' )
      self.wellLayer     = None
    else:
      self.tImplantLayer = rules.getRealLayer( 'pImplant' )
      self.bImplantLayer = rules.getRealLayer( 'nImplant' )
      self.wellLayer     = rules.getRealLayer( 'pWell' )

    return


  def metaTnb ( self ):
    metaT = 0
    for i in self.device.getInstances(): metaT += 1
    return metaT


  ## <b>[API]</b> Set the Stack wiring specification.
  #  
  #  \param wiringSpec  A string defining the connections for the
  #                     gates and diffusion areas.
  #
  #  For a comprehensive explanation of the wiring specification, refers
  #  to \ref secWiringSpecs .

  def setWirings ( self, wiringSpec ):
    
    trace( 100, 'SetWirings \n' )
    
    restrictions = {}
    for net in self.device.getNets():
      if net.getName() == 'anonymous':
        restrictions[net] = Device.AllBlocked
      else:
        restrictions[net] = Device.NorthBlocked|Device.SouthBlocked

    hasTopBulkTrack = False
    hasBotBulkTrack = False
    specs           = wiringSpec.split()
    i               = 0
    topBulkWiring   = None
    botBulkWiring   = None

    trace( 100, specs )

    for spec in specs:
      wiring = Wiring( self.device, spec )
      self.wirings.append( wiring )

      if wiring.isTop():
        #restrictions[wiring.net] = restrictions[wiring.net] & ~Device.NorthBlocked
        if wiring.net == self.bulkNet:
          hasTopBulkTrack = True
          topBulkWiring   = wiring
          continue
        Stack._addToTracks ( self.topTracks , wiring.topTrack, wiring.net )
        Stack._addToWTracks( self.topWTracks, wiring.topTrack, wiring.wTrack )
        restrictions[wiring.net] = restrictions[wiring.net] & ~Device.NorthBlocked

      if wiring.isBot():
        #restrictions[wiring.net] = restrictions[wiring.net] & ~Device.SouthBlocked
        if wiring.net == self.bulkNet:
          hasBotBulkTrack = True 
          botBulkWiring   = wiring
          continue
        Stack._addToTracks ( self.botTracks , wiring.botTrack, wiring.net )
        Stack._addToWTracks( self.botWTracks, wiring.botTrack, wiring.wTrack )
        restrictions[wiring.net] = restrictions[wiring.net] & ~Device.SouthBlocked

    for net, flags in restrictions.items():
      self.device.setRestrictions( net, flags )

    if self.flags & Stack.NorthBulk or hasTopBulkTrack:
      if not self.topTracks or self.topTracks[-1] != self.bulkNet: 
        for net in self.topTracks:
          if net == self.bulkNet:
            raise Error( 3, 'Stack.setWirings(): Bulk track (net:"%s") must be topmost.' \
                             % self.bulkNet.getName() )
        index = len(self.topTracks)
        Stack._addToTracks( self.topTracks , index, self.bulkNet )
        if   (topBulkWiring != None): Stack._addToWTracks( self.topWTracks, index, topBulkWiring.wTrack )
        elif (botBulkWiring != None): Stack._addToWTracks( self.topWTracks, index, botBulkWiring.wTrack )
        else:                         Stack._addToWTracks( self.topWTracks, index, self.device.getParameter('B.w').getValue() )

      bulkIndex = len(self.topTracks)-1
      for wiring in self.wirings:
        if wiring.net == self.bulkNet: wiring.setTop( bulkIndex )

    Stack._addToTracks ( self.topTracks , len(self.topTracks) , None )
    Stack._addToWTracks( self.topWTracks, len(self.topWTracks), self.device.getParameter('B.w').getValue() )

    if self.flags & Stack.SouthBulk or hasBotBulkTrack:
      if not self.botTracks or self.botTracks[-1] != self.bulkNet: 
        for net in self.botTracks:
          if net == self.bulkNet:
            raise Error( 3, 'Stack.setWirings(): Bulk track (net:"%s") must be bottommost.' \
                            % self.bulkNet.getName() )
        index = len(self.botTracks)
        Stack._addToTracks( self.botTracks , index, self.bulkNet )
        if   (botBulkWiring != None): Stack._addToWTracks( self.botWTracks, index, botBulkWiring.wTrack )
        elif (topBulkWiring != None): Stack._addToWTracks( self.botWTracks, index, topBulkWiring.wTrack )
        else:                         Stack._addToWTracks( self.botWTracks, index, self.device.getParameter('B.w').getValue() )
      bulkIndex = len(self.botTracks)-1
      for wiring in self.wirings:
        if wiring.net == self.bulkNet: wiring.setBot( bulkIndex )

    Stack._addToTracks ( self.botTracks , len(self.botTracks) , None )
    Stack._addToWTracks( self.botWTracks, len(self.botWTracks), self.device.getParameter('B.w').getValue() )

    trace( 100, '\tbotTracks:%d topTracks:%d\n' % (len(self.topTracks),len(self.botTracks)) )
    for i in range(len(self.wirings)):
      trace( 100, '\t| wirings[%d]: %s\n' % (i,str(self.wirings[i])) )
      if self.wirings[i].net == self.bulkNet:
        if hasBotBulkTrack: self.wirings[i].setBot( len(self.botTracks)-2 )
        if hasTopBulkTrack: self.wirings[i].setTop( len(self.topTracks)-2 )
        trace( 100, '\t+ wirings[%d]: %s\n' % (i,str(self.wirings[i])) )

    for i in range(len(self.wirings)):
      gateName = self.wirings[i].net.getName()
      if gateName[0] != 'G': continue

      leftMost  = False
      rightMost = False
      if i == 1:                   leftMost  = True
      if i == len(self.wirings)-1: rightMost = True

     # MIN means "minimize the number of sources", so according to the
     # number of transistor fingers, it is almost equivalent to
     # "drain first".
      if not gateName in self.metaTransistors:
        MIN = 0
        if self.wirings[i-1].isDrain(): MIN = 1

        self.metaTransistors[gateName] = { 'gate':self.wirings[i].net, 'MIN':MIN, 'NF':1
                                         , 'leftMost':leftMost, 'rightMost':rightMost
                                         }
      else:
        self.metaTransistors[gateName]['NF'] += 1
        self.metaTransistors[gateName]['rightMost'] = rightMost

   # Compute style parameters for use in geomod equations.
    for mt in self.metaTransistors.values():
      geoFlags = 0
      nf       = mt['NF']
      if nf % 2:
        mt['style.NDend'] = 1
        mt['style.NSend'] = 1
        mt['style.NDint'] = nf-1
        mt['style.NSint'] = nf-1
        if mt['leftMost']:
          if mt['MIN'] == 1: geoFlags |= Stack.DrainIsolated
          else:              geoFlags |= Stack.SourceIsolated
        else:
          if mt['MIN'] == 1: geoFlags |= Stack.DrainShared
          else:              geoFlags |= Stack.SourceShared
        if mt['leftMost']:
          if mt['MIN'] == 1: geoFlags |= Stack.SourceIsolated
          else:              geoFlags |= Stack.DrainIsolated
        else:
          if mt['MIN'] == 1: geoFlags |= Stack.SourceShared
          else:              geoFlags |= Stack.DrainShared
      else:
        if mt['MIN']:
          mt['style.NDend'] = 2
          mt['style.NSend'] = 0
          mt['style.NDint'] = (nf//2 - 1)*2
          mt['style.NSint'] =  nf
          if mt['leftMost'] or mt['rightMost']: geoFlags |= Stack.DrainIsolated
          else:                                 geoFlags |= Stack.DrainShared
        else:
          mt['style.NDend'] = 0
          mt['style.NSend'] = 2
          mt['style.NSint'] = (nf//2 - 1)*2
          mt['style.NDint'] =  nf
          if mt['leftMost'] or mt['rightMost']: geoFlags |= Stack.SourceIsolated
          else:                                 geoFlags |= Stack.SourceShared

      mt['style.geomod'] = Stack.toGeomod( geoFlags, mt['NF'] )
    return


  def isNmos       ( self ):    return self.device.isNMOS()
  def isPmos       ( self ):    return self.device.isPMOS()
  def hasNorthBulk ( self ):    return self.flags & Stack.NorthBulk
  def hasSouthBulk ( self ):    return self.flags & Stack.SouthBulk
  def hasEastBulk  ( self ):    return self.flags & Stack.EastBulk
  def hasWestBulk  ( self ):    return self.flags & Stack.WestBulk
  def wiring       ( self, i ): return self.wirings[i]
  def tracksNb     ( self ):    return len(self.topTracks) + len(self.botTracks)
  def topTracksNb  ( self ):    return len(self.topTracks)
  def botTracksNb  ( self ):    return len(self.botTracks)

  def isTopTrack   ( self, net ):
    if net == self.bulkNet: return False

    trackIndex = 2
    if self.flags & Stack.NorthBulk: trackIndex = 3
    if trackIndex > len(self.topTracks): return False
    if self.topTracks[-trackIndex] != net: return False
    return True

  def isBotTrack   ( self, net ):
    if net == self.bulkNet: return False

    trackIndex = 2
    if self.flags & Stack.SouthBulk: trackIndex = 3
    if trackIndex > len(self.botTracks): return False
    if self.botTracks[-trackIndex] != net: return False
    return True
  
######################################
  def tracksNbPitch ( self ):
    nb = 0
    for i in range (self.topTracksNb()-1):
      nb += self.topWTracks[i]
    for i in range (self.botTracksNb()-1):
      nb += self.botWTracks[i]
    return nb

  def getBotTrackY ( self, i ):
    nb = self.horPitch
    for j in range(self.botTracksNb()-2-i):
      nb += self.horPitch*self.botWTracks[self.botTracksNb()-2-j]
    return nb

  def getTopTrackY ( self, i ):
    y = (self.ypitches)*self.horPitch + self.getBotTrackY(0) + self.getHorizontalWidth(self.botWTracks[0])
    #y = self.activeOffsetY + (self.ypitches-1)*self.horPitch #- self.hTrackDistance
    for j in range(i):
      y += self.horPitch*self.topWTracks[j]
    return y
  
  def getLastTopTrackY   ( self ):            return self.getTopTrackY(self.topTracksNb()-1)
  def getLastTopWTracks  ( self ):            return self.topWTracks[self.topTracksNb()-2]
  def getLastBotWTracks  ( self ):            return self.botWTracks[self.botTracksNb()-2]
  def getHorizontalWidth ( self, trackSpan ): return (self.horPitch * (trackSpan - 1))
  def getHorizontalAxis  ( self, trackSpan ): return self.getHorizontalWidth(trackSpan)//2
      
  def getWiringWidth ( self, wiring, isTopConnect ):
    if isTopConnect: return self.horPitch * (self.topWTracks[wiring.topTrack] - 1)
    return self.horPitch * (self.botWTracks[wiring.botTrack] - 1)

######################################
  def DMCI ( self ):
    if not self.dimensioned: self.computeDimensions()
    return   self.sideActiveWidth \
           - self.L//2            \
           - self.metal1ToGate    \
           - self.eDiffMetal1Width//2

  def DMCG ( self ):
    if not self.dimensioned: self.computeDimensions()
    return (self.gatePitch - self.L)//2

  def DMCGT ( self ): return 0.0

  def DGG ( self ):
    if not self.dimensioned: self.computeDimensions()
    return self.gatePitch - self.L

  def DGI ( self ):
    if not self.dimensioned: self.computeDimensions()
    return self.sideActiveWidth - self.L//2
  

  ## <b>[internal]</b> Compute Stack dimensions from the technological rules.
  #
  #  <b>Internal function.</b> Perform the computation of:
  #  - \c self.metal1Pitch
  #  - \c self.minWidth_metal1
  #  - \c self.metal2Pitch
  #  - \c self.minWidth_metal2
  #  - \c self.gatePitch
  #  - \c self.sideActiveWidth
  #  - \c self.hTrackDistance
  #  - \c self.xpitches
  #  - \c self.ypitches
  #  - \c self.activeOffsetX
  #  - \c self.activeOffsetY
  #  - \c self.boundingBox

  def computeDimensions ( self ):
    if self.dimensioned: return
    self.dimensioned = True

    trace( 100, '\tStack.computeDimensions(): Start time %s.\n' % str(datetime.datetime.now()) )

    specsNb = 3 + (self.NFs - 1)*2
    if len(self.wirings) != specsNb:
      raise Error( 3, [ 'Stack._computeDimension(): Wiring inconsistency,' \
                        ' have %d specs but should be %d for a NFs of %d.' \
                        % (len(self.wirings), specsNb, self.NFs)
                      ] )

    rg = AllianceFramework.get().getRoutingGauge()
    self.metal2Pitch = rg.getHorizontalPitch()
    self.metal3Pitch = rg.getVerticalPitch()
    self.isVH        = rg.isVH()

    foundHor = False
    foundVer = False
    for depth in range(rg.getDepth()):
      rlg = rg.getLayerGauge(depth)
      if rlg.getType() == Constant.PinOnly: continue
      if rlg.getDirection() == Constant.Horizontal and not foundHor:
        self.horPitch = rlg.getPitch()
        foundHor = True
      if rlg.getDirection() == Constant.Vertical and not foundVer:
        self.verPitch = rlg.getPitch()
        foundVer = True

    width1 = self.minWidth_cut0 + self.minEnclosure_metal1_cut0*2
    width2 = self.minWidth_cut1 + self.minEnclosure_metal1_cut1*2 
    self.wire1Width = max( self.minWidth_metal1, width1, width2 )

    pitch1 = self.minWidth_metal2 + self.minSpacing_metal2
    pitch2 = self.minWidth_cut1 + self.minEnclosure_metal2_cut1*2 + self.minSpacing_metal2
    self.metal2TechnoPitch = max( pitch1, pitch2 )

    if self.metal2Pitch == 0: self.metal2Pitch = self.metal2TechnoPitch
    if self.metal2Pitch < self.metal2TechnoPitch:
      raise Error( 3, 'Stack.computeDimensions(): Routing gauge pitch %s is inferior to technology minimum of %s.'
                      % ( DbU.getValueString(self.metal2Pitch)
                        , DbU.getValueString(self.metal2TechnoPitch) ) )

    width1 = self.minWidth_cut1 + self.minEnclosure_metal2_cut1*2
    if self.isVH:
      self.wire2Width = self.minWidth_metal2
      self.wire3Width = self.minWidth_metal3
    else:
      self.wire2Width = max( self.minWidth_metal2, width1 )

    pitch1 = self.minGateSpacing_poly + self.minWidth_cut0
    pitch2 = self.minSpacing_cut1 + self.minWidth_cut1
    self.gateVia1Pitch = max( pitch1, pitch2 )

    pitch1 = self.minWidth_cut0 + self.minSpacing_cut0
    pitch2 = self.minWidth_cut1 + self.minSpacing_cut1
    self.contactDiffPitch = max( pitch1, pitch2 )
    self.contactDiffSide  = max( self.minWidth_cut1, self.minWidth_cut0 )

    iDiffContactWidth = self.contactDiffSide + (self.NIRC - 1)*self.contactDiffPitch
    eDiffContactWidth = self.contactDiffSide + (self.NERC - 1)*self.contactDiffPitch
    overlap           = max( self.minEnclosure_metal1_cut0, self.minEnclosure_metal1_cut1 )
    gateVia1Side      = max( self.minWidth_cut1, self.minWidth_cut0 )
    self.iDiffMetal1Width = iDiffContactWidth + overlap*2
    self.eDiffMetal1Width = eDiffContactWidth + overlap*2 

    pitch1 = self.L                      \
           + iDiffContactWidth           \
           + self.minSpacing_cut0_poly*2
    pitch2 = self.minWidth_cut0            \
           + self.minEnclosure_poly_cut0*2 \
           + self.minGateSpacing_poly
    pitch3 = self.minSpacing_metal1*2 \
           + self.iDiffMetal1Width    \
           + max( self.L, gateVia1Side+2*overlap )
    self.gatePitch    = max( pitch1, pitch2, pitch3 )
    self.metal1ToGate = (self.gatePitch - self.L - self.iDiffMetal1Width) // 2

    self.sideActiveWidth = self.minEnclosure_active_cut0 \
                         - self.minEnclosure_metal1_cut0 \
                         + self.eDiffMetal1Width         \
                         + self.metal1ToGate             \
                         + self.L//2

    hTrackDistance1 = self.minWidth_cut0//2 + self.minSpacing_cut0_active
    hTrackDistance2 = self.minWidth_cut0//2 + self.minEnclosure_poly_cut0 + self.minSpacing_poly_active
    self.hTrackDistance = max( hTrackDistance1, hTrackDistance2 )

    vBulkDistance1 = self.minWidth_cut0//2             \
                   + self.minEnclosure_active_cut0     \
                   + self.minEnclosure_tImplant_active \
                   + self.minEnclosure_bImplant_active
    vBulkDistance2 = self.minWidth_cut0//2          \
                   + self.minEnclosure_active_cut0  \
                   + self.minSpacing_nImplant_pImplant
    self.vBulkDistance = max( vBulkDistance1, vBulkDistance2 )

    activeHeight  = self.w + 2*self.hTrackDistance
    self.ypitches = activeHeight // self.horPitch
    if activeHeight % self.horPitch: self.ypitches += 1
    if (self.ypitches + self.tracksNbPitch()) % 2: self.ypitches += 1

    diffusionWidth = (self.sideActiveWidth + self.minEnclosure_tImplant_active)*2 + (self.NFs-1)*self.gatePitch
    deviceMinWidth = diffusionWidth
    if self.flags & Stack.WestBulk: deviceMinWidth += self.vBulkDistance + self.verPitch
    if self.flags & Stack.EastBulk: deviceMinWidth += self.vBulkDistance + self.verPitch

    self.xpitches  = deviceMinWidth // self.verPitch
    if self.xpitches % 2:
      self.xpitches += 1
    else:
      if diffusionWidth % self.verPitch:
        self.xpitches += 2
    
    self.activeOffsetY = self.getBotTrackY(0) + self.getHorizontalWidth(self.botWTracks[0]) \
                       + self.hTrackDistance \
                       + (self.ypitches*self.horPitch - activeHeight)//2 \

    self.bbHeight      = self.getLastTopTrackY() 

    self.bbWidth       = self.xpitches * self.verPitch
    diffusionRealWidth = self.bbWidth
    if self.flags & Stack.WestBulk: diffusionRealWidth -= self.vBulkDistance + self.verPitch
    if self.flags & Stack.EastBulk: diffusionRealWidth -= self.vBulkDistance + self.verPitch
    self.activeOffsetX = self.minEnclosure_tImplant_active + (diffusionRealWidth - diffusionWidth)//2
    if self.flags & Stack.WestBulk: self.activeOffsetX += self.vBulkDistance + self.verPitch

    self.boundingBox = Box( 0, 0, self.bbWidth, self.bbHeight )
    self.activeBox   = Box( self.activeOffsetX
                          , self.activeOffsetY
                          , self.activeOffsetX + (self.NFs - 1) * self.gatePitch + 2*self.sideActiveWidth
                          , self.activeOffsetY + self.w
                          )

    westBulkX  = self.activeOffsetX
    eastBulkX  = self.activeOffsetX + diffusionWidth - self.minEnclosure_tImplant_active*2
    southBulkY = self.boundingBox.getYMin() + self.horPitch
    northBulkY = self.boundingBox.getYMax() - self.horPitch
    

    self.bulkWidth = self.minWidth_cut0 + 2*self.minEnclosure_active_cut0

    if self.flags & Stack.EastBulk:
      eastBulkX = self.boundingBox.getXMax() - self.horPitch
      self.bulks[2] = Bulk( self, eastBulkX, southBulkY, northBulkY, Stack.EastBulk )

    if self.flags & Stack.WestBulk:
      westBulkX = self.horPitch
      self.bulks[3] = Bulk( self, westBulkX, southBulkY, northBulkY, Stack.WestBulk )

    if self.flags & Stack.NorthBulk:
      self.bulks[0] = Bulk( self, northBulkY, westBulkX, eastBulkX, Stack.NorthBulk )

    if self.flags & Stack.SouthBulk:
      self.bulks[1] = Bulk( self, southBulkY, westBulkX, eastBulkX, Stack.SouthBulk )

    self.DMCI  = toUnity(   self.sideActiveWidth 
                          - self.L//2
                          - self.metal1ToGate
                          - self.eDiffMetal1Width//2 )
    self.DMCG  = toUnity( (self.gatePitch - self.L)//2 )
    self.DMCGT = 0
    self.DGG   = toUnity(  self.gatePitch - self.L )
    self.DGI   = toUnity( self.sideActiveWidth - self.L//2 )

    trace( 100, '+' )
    trace( 100, '\t  +----------------------------------+\n' )
    trace( 100, '\t  | Parameters                       |\n' )
    trace( 100, '\t  +=====================+============+\n' )
    trace( 100, '\t  | w (finger)          | %10s |\n' % DbU.getValueString(self.w)  )
    trace( 100, '\t  | L                   | %10s |\n' % DbU.getValueString(self.L)  )
    trace( 100, '\t  | NFs                 | %10d |\n' % self.NFs  )
    trace( 100, '\t  +=====================+============+\n' )
    trace( 100, '\t  | Computed                         |\n' )
    trace( 100, '\t  +=====================+============+\n' )
    trace( 100, '\t  | metal2 pitch        | %10s |\n' % DbU.getValueString(self.metal2Pitch)  )
    trace( 100, '\t  | metal2 Techno pitch | %10s |\n' % DbU.getValueString(self.metal2TechnoPitch)  )
    trace( 100, '\t  | gatePitch           | %10s |\n' % DbU.getValueString(self.gatePitch)  )
    trace( 100, '\t  | sideActiveWidth     | %10s |\n' % DbU.getValueString(self.sideActiveWidth)  )
    trace( 100, '\t  | contactDiffPitch    | %10s |\n' % DbU.getValueString(self.contactDiffPitch)  )
    trace( 100, '\t  | hTrackDistance      | %10s |\n' % DbU.getValueString(self.hTrackDistance)  )
    trace( 100, '\t  | vBulkDistance       | %10s |\n' % DbU.getValueString(self.vBulkDistance)  )
    trace( 100, '\t  | activeOffsetX       | %10s |\n' % DbU.getValueString(self.activeOffsetX)  )
    trace( 100, '\t  | activeOffsetY       | %10s |\n' % DbU.getValueString(self.activeOffsetY)  )
    trace( 100, '\t  | active pitches      | %10d |\n' % self.ypitches )
    trace( 100, '\t  +---------------------+------------+\n' )
    trace( 100, '\n' )
    trace( 100, '\tStack.computeDimensions(): End time %s.\n' % str(datetime.datetime.now()) )
    trace( 100, '-' )

    for mt in self.metaTransistors.values():
      self.computeStressEffect( mt )
      self.computeLayoutParasitics( mt )
      traceMT( mt )
    return

  ## <b>[API]</b> Draw the complete layout.
  #
  #  Draw the commplete layout of the Stack. 

  def doLayout ( self, bbMode ):
    trace( 100, '+', '\tStack.doLayout().\n' )

    self.computeDimensions()

    if not bbMode:
      UpdateSession.open()
      
      self.drawActive()
      self.drawWell()
      
      xoffset = self.activeOffsetX + self.sideActiveWidth
      for i in range(self.NFs):
        self.drawGate( xoffset + i*self.gatePitch, self.wirings[1+2*i] )
      
      for i in range(self.NFs+1):
        if i == 0:          # Leftmost diffusion area.
          NRC   = self.NERC
          width = self.eDiffMetal1Width
          axis  = self.activeOffsetX        \
                + self.sideActiveWidth      \
                - self.L//2                 \
                - self.metal1ToGate         \
                - width//2
        elif i == self.NFs: # Rightmost diffusion area.
          NRC   = self.NERC
          width = self.eDiffMetal1Width
          axis  = self.activeOffsetX            \
                + self.sideActiveWidth          \
                + self.gatePitch*(self.NFs - 1) \
                + self.L//2                     \
                + self.metal1ToGate             \
                + width//2
        else:               # Middle diffusion areas.
          NRC   = self.NIRC
          width = self.iDiffMetal1Width
          axis  = self.activeOffsetX   \
                + self.sideActiveWidth \
                - self.gatePitch//2    \
                + self.gatePitch*i
      
        self.drawSourceDrain( axis, self.wirings[2*i], width, NRC )

      capSpacing = self.minSpacing_metal2 + self.minWidth_metal2//2
      capSpacing = max( capSpacing, self.minSpacing_metal3 + self.minWidth_metal3//2 )
      
      metal2  = rules.getRealLayer( 'metal2' )
      metal3  = rules.getRealLayer( 'metal3' )
      trackNb = 0
      if self.topTracks: trackNb = len(self.topTracks)
      for i in range(trackNb):
        if not self.topTracks[i]: continue
        h = Horizontal.create( self.topTracks[i]
                             , metal2
                             , self.getTopTrackY(i) + self.getHorizontalAxis (self.topWTracks[i])
                             , self.wire2Width      + self.getHorizontalWidth(self.topWTracks[i])
                             , capSpacing
                             , self.bbWidth - capSpacing )
        if self.isVH:
          h = Horizontal.create( self.topTracks[i]
                               , metal3
                               , self.getTopTrackY(i) + self.getHorizontalAxis (self.topWTracks[i])
                               , self.wire3Width      + self.getHorizontalWidth(self.topWTracks[i])
                               , capSpacing
                               , self.bbWidth - capSpacing )
        NetExternalComponents.setExternal( h )
      
      trackNb = 0
      if self.botTracks: trackNb = len(self.botTracks)
      for i in range(trackNb):
        if not self.botTracks[i]: continue
        h = Horizontal.create( self.botTracks[i]
                             , metal2
                             , self.getBotTrackY(i) + self.getHorizontalAxis (self.botWTracks[i])
                             , self.wire2Width      + self.getHorizontalWidth(self.botWTracks[i])
                             , capSpacing
                             , self.bbWidth - capSpacing )
        if self.isVH:
          h = Horizontal.create( self.botTracks[i]
                               , metal3
                               , self.getBotTrackY(i) + self.getHorizontalAxis (self.botWTracks[i])
                               , self.wire3Width      + self.getHorizontalWidth(self.botWTracks[i])
                               , capSpacing
                               , self.bbWidth - capSpacing )
        NetExternalComponents.setExternal( h )

      
      for bulk in self.bulks:
        if bulk: bulk.doLayout()
      
      UpdateSession.close()

    trace( 100, '\tStack.doLayout(): End time %s\n' %str(datetime.datetime.now()) )
    trace( 100, '-' )
    return self.boundingBox


  def drawActive ( self ):
    trace( 100, '+', '\tDrawActive().\n' )
    activeNet = self.device.getNet( 'active' )
    if not activeNet: activeNet = Net.create( self.device, 'active' )
    activeNet.setAutomatic( True )

    tImplantNet = self.device.getNet( 'nImplant' )
    if not tImplantNet: tImplantNet = Net.create( self.device, 'nImplant' )
    tImplantNet.setAutomatic( True )

    active      = rules.getRealLayer( 'active' )
    width       = self.w
    length      = (self.NFs - 1) * self.gatePitch + 2*self.sideActiveWidth
    axis        = width // 2
    xoffset     = self.activeOffsetX
    yoffset     = self.activeOffsetY
    segment     = Horizontal.create( activeNet, active, yoffset+axis, width, xoffset, xoffset+length )

    width       = width  + 2*self.minEnclosure_tImplant_active 
    length      = length + 2*self.minEnclosure_tImplant_active 
    axis        = width // 2
    xoffset     = self.activeOffsetX - self.minEnclosure_tImplant_active
    yoffset     = self.activeOffsetY - self.minEnclosure_tImplant_active
    segment     = Horizontal.create( tImplantNet
                                   , self.tImplantLayer
                                   , yoffset+axis
                                   , width
                                   , xoffset
                                   , xoffset+length )
    return


  def drawWell ( self ):
    if self.wellLayer:
      trace( 100, '+', '\tDrawWell().\n' )
      Pad.create( self.device.getNet('anonymous'), self.wellLayer, self.boundingBox )
    return


  def drawGate ( self, axis, wiring ):
    trace( 100, '\tStack.drawGate(): %s\n' % wiring )

    gate    = rules.getRealLayer( 'poly' )
    cut0    = rules.getRealLayer( 'cut0' )
    cut1    = rules.getRealLayer( 'cut1' )
    cut2    = rules.getRealLayer( 'cut2' )
    metal1  = rules.getRealLayer( 'metal1' )
    metal2  = rules.getRealLayer( 'metal2' )
    metal3  = rules.getRealLayer( 'metal3' )
    width   = self.L

    if wiring.isTop(): ytarget = self.getTopTrackY( wiring.topTrack )
    else:              ytarget = self.activeOffsetY + self.minExtension_poly_active + self.w

    if wiring.isBot(): ysource = self.getBotTrackY( wiring.botTrack )
    else:              ysource = self.activeOffsetY - self.minExtension_poly_active

    segment = Vertical.create( wiring.net, gate, axis, width, ysource, ytarget )

    contactHeight = self.minWidth_cut0 + 2*self.minEnclosure_poly_cut0
    contactWidth  = max( contactHeight, self.L )
    contactsNb    = (contactWidth - 2*self.minEnclosure_poly_cut0) // self.gateVia1Pitch
    if contactsNb:
      contactPitch = contactWidth // contactsNb
    else:
      contactsNb   = 1
      contactPitch = self.L

    gateVia1Overlap = max( self.minEnclosure_metal1_cut0, self.minEnclosure_metal1_cut1 )
    gateVia1Side    = max( self.minWidth_cut1, self.minWidth_cut0 )

    for connector in ( (wiring.isTop(),ytarget,True), (wiring.isBot(),ysource,False) ):
      if not connector[0]: continue

      isTopConnect = connector[2]
      yoffset      = connector[1]
      xcontact     = axis - self.L//2 + contactPitch//2
      contactBb    = Box( axis, yoffset ).inflate( contactWidth//2, contactHeight//2)
      width        = gateVia1Side + 2*gateVia1Overlap + self.getWiringWidth(wiring, isTopConnect)
      y            = yoffset + self.getWiringWidth(wiring, isTopConnect)//2
      rowHeight    = self.horPitch

      if isTopConnect:
        contactBb = Box( axis, yoffset ).inflate( contactWidth//2, contactHeight//2+self.getWiringWidth(wiring, isTopConnect))

      Pad.create( wiring.net, gate, contactBb ) # GateExtension Contact
      Horizontal.create( wiring.net
                       , metal1
                       , y
                       , width
                       , xcontact - gateVia1Side//2 - gateVia1Overlap
                       , xcontact + (contactsNb-1)*contactPitch + gateVia1Side//2 + gateVia1Overlap )# M1 area
      

      cut1Bb = Box()
      cut2Bb = Box()
      for i in range(contactsNb):
        if isTopConnect: rangeWidth = range(self.topWTracks[wiring.topTrack])
        else:            rangeWidth = range(self.botWTracks[wiring.botTrack])

        for j in rangeWidth:
          contactBb = Box( xcontact, yoffset ).inflate( self.minWidth_cut0//2 )
          Pad.create( wiring.net, cut0, contactBb )
          contactBb = Box( xcontact, yoffset ).inflate( self.minWidth_cut1//2 )
          Pad.create( wiring.net, cut1, contactBb )
          cut1Bb.merge( contactBb )
          if self.isVH:
            contactBb = Box( xcontact, yoffset ).inflate( self.minWidth_cut2//2 )
            Pad.create( wiring.net, cut2, contactBb )
            cut2Bb.merge( contactBb )
          yoffset += rowHeight
        yoffset -= rowHeight*self.topWTracks[wiring.topTrack]

        if self.isVH:
          metal2EnclBb =      Box( cut1Bb ).inflate( self.minEnclosure_metal2_cut1 )
          metal2EnclBb.merge( Box( cut2Bb ).inflate( self.minEnclosure_metal2_cut2 ) )
          Pad.create( wiring.net, metal2, metal2EnclBb )
          
          metal3EnclBb =      Box( cut2Bb ).inflate( self.minEnclosure_metal3_cut2 )
          Pad.create( wiring.net, metal3, metal3EnclBb )
          
          enable = False
          if wiring.net == self.bulkNet: enable = True
          if connector[0] & Stack.NorthBulk and self.bulks[0]: self.bulks[0].addContact( xcontact, enable )
          if connector[0] & Stack.SouthBulk and self.bulks[1]: self.bulks[1].addContact( xcontact, enable )
      
        xcontact += contactPitch

    return


  def drawSourceDrain ( self, axis, wiring, width, cols ):
    trace( 100, '\tStack.drawSourceDrain(): %s @%s width:%s NRC=%d\n' \
           % (wiring, DbU.getValueString(axis), DbU.getValueString(width), cols ) )

    metal1  = rules.getRealLayer( 'metal1' )
    metal2  = rules.getRealLayer( 'metal2' )
    metal3  = rules.getRealLayer( 'metal3' )
    cut0    = rules.getRealLayer( 'cut0' )
    cut1    = rules.getRealLayer( 'cut1' )
    cut2    = rules.getRealLayer( 'cut2' )
    rows    = max( 1, (self.w - 2*self.minEnclosure_active_cut0) // self.contactDiffPitch )
    ypitch  = self.w // rows
    yoffset = self.activeOffsetY + ypitch//2
    xpitch  = self.contactDiffPitch
    ypitch2 = self.horPitch
    xoffset = axis - (self.contactDiffPitch * (cols - 1))//2

    if self.w < 2*self.minEnclosure_active_cut0 + self.minWidth_cut0:
      active = rules.getRealLayer( 'active' )

      box = Box( xoffset, yoffset, xoffset + (cols-1)*xpitch, yoffset )
      box.inflate( self.minWidth_cut0 + self.minEnclosure_active_cut0 )
      Pad.create( wiring.net, active, box )

      box.inflate( self.minEnclosure_tImplant_active )
      Pad.create( wiring.net, self.tImplantLayer, box )

    for j in range(rows):
      for i in range(cols):
        contact = Contact.create( wiring.net
                                , cut0
                                , xoffset + i*xpitch
                                , yoffset + j*ypitch
                                , self.minWidth_cut0
                                , self.minWidth_cut0 )

    if wiring.isTop():
      ytarget = self.getTopTrackY( wiring.topTrack )
      for i in range(cols):
        for j in range(self.topWTracks[wiring.topTrack]):
          Contact.create( wiring.net
                        , cut1
                        , xoffset + i*xpitch
                        , ytarget + j*ypitch2
                        , self.minWidth_cut1
                        , self.minWidth_cut1 )
          Contact.create( wiring.net
                        , cut2
                        , xoffset + i*xpitch
                        , ytarget + j*ypitch2
                        , self.minWidth_cut2
                        , self.minWidth_cut2 )

        enable = False
        if wiring.net == self.bulkNet: enable = True
        if self.bulks[0]: self.bulks[0].addContact( xoffset + i*xpitch, enable )

      if self.isVH:
        cutBb = Box( xoffset
                   , ytarget
                   , xoffset + xpitch *(cols - 1)
                   , ytarget + ypitch2*(self.topWTracks[wiring.topTrack] - 1) )
        
        metal2EnclBb =      Box( cutBb ).inflate( self.minEnclosure_metal2_cut1 + self.minWidth_cut1//2 )
        metal2EnclBb.merge( Box( cutBb ).inflate( self.minEnclosure_metal2_cut2 + self.minWidth_cut2//2 ) )
        Pad.create( wiring.net, metal2, metal2EnclBb )
        
        metal3EnclBb =      Box( cutBb ).inflate( self.minEnclosure_metal3_cut2 + self.minWidth_cut2//2 )
        Pad.create( wiring.net, metal3, metal3EnclBb )
        
      ytarget += ypitch2*(self.topWTracks[wiring.topTrack]-1)
    else:
      ytarget = yoffset + ypitch*(rows - 1) 

    if wiring.isBot():
      ysource = self.getBotTrackY( wiring.botTrack )
      for i in range(cols):
        for j in range(self.botWTracks[wiring.botTrack]):
          Contact.create( wiring.net
                        , cut1
                        , xoffset + i*xpitch
                        , ysource + j*ypitch2
                        , self.minWidth_cut1
                        , self.minWidth_cut1 )
          Contact.create( wiring.net
                        , cut2
                        , xoffset + i*xpitch
                        , ysource + j*ypitch2
                        , self.minWidth_cut2
                        , self.minWidth_cut2 )

        if self.bulks[1]:
          if self.isBotTrack(wiring.net) or wiring.net == self.bulkNet:
            enable = False
            if wiring.net == self.bulkNet: enable = True
            self.bulks[1].addContact( xoffset + i*xpitch, enable )

        if self.isVH:
          cutBb = Box( xoffset
                     , ysource
                     , xoffset + xpitch *(cols - 1)
                     , ysource + ypitch2*(self.botWTracks[wiring.botTrack] - 1) )
          
          metal2EnclBb =      Box( cutBb ).inflate( self.minEnclosure_metal2_cut1 + self.minWidth_cut1//2 )
          metal2EnclBb.merge( Box( cutBb ).inflate( self.minEnclosure_metal2_cut2 + self.minWidth_cut2//2 ) )
          Pad.create( wiring.net, metal2, metal2EnclBb )
          
          metal3EnclBb =      Box( cutBb ).inflate( self.minEnclosure_metal3_cut2 + self.minWidth_cut2//2 )
          Pad.create( wiring.net, metal3, metal3EnclBb )
    else:
      ysource = yoffset

    bcontact = Contact.create( wiring.net
                             , metal1
                             , axis
                             , ysource
                             , width
                             , self.wire1Width )
    tcontact = Contact.create( wiring.net
                             , metal1
                             , axis
                             , ytarget
                             , width
                             , self.wire1Width )
    segment = Vertical.create( bcontact, tcontact, metal1, axis, width )

    return


  def computeStressEffect ( self, mt ):
   # Stress effect computation as specified in S. Youssef thesis, p.59.
    eastBulkWidth = 0
    westBulkWidth = 0
    if self.flags & Stack.WestBulk: westBulkWidth = self.bulkWidth
    if self.flags & Stack.EastBulk: eastBulkWidth = self.bulkWidth

    L            = toUnity( self.L )
    sumA_Bsim4   = 0.0
    sumB_Bsim4   = 0.0
    sumA_Crolles = 0.0
    sumB_Crolles = 0.0
    
    for i in range(self.NFs):
      if not self.wirings[i].net != mt['gate']: continue

      sa = self.DGI + westBulkWidth +             i     *(L + self.DGG)
      sb = self.DGI + eastBulkWidth + (self.NFs - i - 1)*(L + self.DGG)

      sumA_Bsim4   += 1 / (sa + 0.5*self.DGG)
      sumB_Bsim4   += 1 / (sb + 0.5*self.DGG)
      sumA_Crolles += 1 /  sa
      sumB_Crolles += 1 /  sb

    SAinv_Bsim4   = sumA_Bsim4   / mt['NF']
    SBinv_Bsim4   = sumB_Bsim4   / mt['NF']
    SAinv_Crolles = sumA_Crolles / mt['NF']
    SBinv_Crolles = sumB_Crolles / mt['NF']

    mt['stress.SAinv_Bsim4'] =     SAinv_Bsim4
    mt['stress.SBinv_Bsim4'] =     SBinv_Bsim4
    mt['stress.SAeff_Bsim4'] = 1 / SAinv_Bsim4
    mt['stress.SBeff_Bsim4'] = 1 / SBinv_Bsim4
    mt['stress.LODeffect'  ] = 1 / SAinv_Bsim4 + 1 / SBinv_Bsim4  
    mt['stress.alpha'      ] = 2 / ( SAinv_Bsim4 + SBinv_Bsim4 )
    mt['stress.alphaInv'   ] = 1 / mt['stress.alpha']

    mt['stress.SAinv_Crolles'    ] =     SAinv_Crolles
    mt['stress.SBinv_Crolles'    ] =     SBinv_Crolles
    mt['stress.SAeff_Crolles'    ] = 1 / SAinv_Crolles
    mt['stress.SBeff_Crolles'    ] = 1 / SBinv_Crolles
    mt['stress.po2actEff_Crolles'] = 2 / ( SAinv_Crolles + SBinv_Crolles )

    return


  def computeLayoutParasitics ( self, mt ):
   #trace( 100, '\tStack.computeLayoutParasitics(): %s\n' % str(mt) )

    NSend = mt['style.NSend']
    NDend = mt['style.NDend']
    NSint = mt['style.NSint']
    NDint = mt['style.NDint']
    NF    = mt['NF']
    Weff  = toUnity( self.w )
    DMCI  = self.DMCI
    DMCG  = self.DMCG
    DMCGT = self.DMCGT
    
    if mt['style.geomod'] == 0:
      ASeff = NSend * (    DMCG + DMCI - DMCGT) * Weff  + NSint    *(DMCG - DMCGT)*Weff
      PSeff = NSend * ( 2*(DMCG + DMCI - DMCGT) + Weff) + NSint * 2*(DMCG - DMCGT)
      ADeff = NDend * (    DMCG + DMCI - DMCGT) * Weff  + NDint    *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMCG + DMCI - DMCGT) + Weff) + NDint * 2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 1:
      ASeff = NSend *     (DMCG + DMCI - DMCGT) * Weff  + NSint   *(DMCG - DMCGT)*Weff
      PSeff = NSend * ( 2*(DMCG + DMCI - DMCGT) + Weff) + NSint *2*(DMCG - DMCGT)
      ADeff = NDend *     (DMCG - DMCGT)*Weff           + NDint   *(DMCG - DMCGT)*Weff
      PDeff = NDend *   2*(DMCG - DMCGT)                + NDint *2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 2:
      ASeff = NSend *     (DMCG - DMCGT) * Weff         + NSint   *(DMCG - DMCGT)*Weff
      PSeff = NSend * 2 * (DMCG - DMCGT)                + NSint *2*(DMCG - DMCGT)
      ADeff = NDend *     (DMCG + DMCI - DMCGT) * Weff  + NDint   *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMCG + DMCI - DMCGT) + Weff) + NDint *2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 3:
      ASeff = NSend      *(DMCG - DMCGT) * Weff         + NSint   *(DMCG - DMCGT)*Weff
      PSeff = NSend    *2*(DMCG - DMCGT)                + NSint *2*(DMCG - DMCGT)
      ADeff = NDend      *(DMCG + DMCI - DMCGT) * Weff  + NDint   *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMCG + DMCI - DMCGT) + Weff) + NDint *2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 4:
      ASeff = NSend *     (DMCG + DMCI - DMCGT) * Weff  + NSint   *(DMCG - DMCGT)*Weff
      PSeff = NSend * ( 2*(DMCG + DMCI - DMCGT) * Weff) + NSint *2*(DMCG - DMCGT)
      ADeff = NDend *     (DMDG - DMCGT)*Weff           + NDint   *(DMCG - DMCGT)*Weff
      PDeff = NDend *   2*(DMDG - DMCGT)                + NDint *2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 5:
      ASeff = NSend      *(DMCG - DMCGT)*Weff           + NSint  *(DMCG - DMCGT)*Weff
      PSeff = NSend  * 2 *(DMCG - DMCGT)                + NSint*2*(DMCG - DMCGT)
      ADeff = NDend      *(DMDG - DMCGT) * Weff         + NDint  *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMDG - DMCGT) * Weff)        + NDint*2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 5:
      ASeff = NSend      *(DMDG - DMCGT)*Weff           + NSint  *(DMDG - DMCGT)*Weff
      PSeff = NSend    *2*(DMCG - DMCGT)                + NSint*2*(DMCG - DMCGT)
      ADeff = NDend      *(DMCG + DMCI - DMCGT) * Weff  + NDint  *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMCG + DMCI - DMCGT) * Weff) + NDint*2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 6:
      ASeff = NSend      *(DMDG - DMCGT)*Weff           + NSint  *(DMDG - DMCGT)*Weff
      PSeff = NSend    *2*(DMCG - DMCGT)                + NSint*2*(DMCG - DMCGT)
      ADeff = NDend      *(DMCG + DMCI - DMCGT) * Weff  + NDint  *(DMCG - DMCGT)*Weff
      PDeff = NDend * ( 2*(DMCG + DMCI - DMCGT) * Weff) + NDint*2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 7:
      ASeff = NSend      *(DMDG - DMCGT)*Weff           + NSint  *(DMCG - DMCGT)*Weff
      PSeff = NSend    *2*(DMDG - DMCGT)                + NSint*2*(DMCG - DMCGT)
      ADeff = NDend      *(DMCG - DMCGT)*Weff           + NDint  *(DMCG - DMCGT)*Weff
      PDeff = NDend    *2*(DMCG - DMCGT)                + NDint*2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 8:
      ASeff = NSend      *(DMDG - DMCGT)*Weff           + NSint  *(DMCG - DMCGT)*Weff
      PSeff = NSend    *2*(DMDG - DMCGT)                + NSint*2*(DMCG - DMCGT)
      ADeff = NDend      *(DMDG - DMCGT)*Weff           + NDint  *(DMCG - DMCGT)*Weff
      PDeff = NDend    *2*(DMDG - DMCGT)                + NDint*2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 9:
      ASeff =    (DMCG + DMCI - DMCGT) * Weff  + (NF-1)  *(DMCG - DMCGT)*Weff
      PSeff = (2*(DMCG + DMCI - DMCGT) + Weff) + (NF-1)*2*(DMCG - DMCGT)
      ADeff = NF   *(DMCG - DMCGT)*Weff 
      PDeff = NF* 2*(DMCG - DMCGT)
    elif mt['style.geomod'] == 10:
      ASeff = NF   *(DMCG - DMCGT)*Weff 
      PSeff = NF* 2*(DMCG - DMCGT)
      ADeff =    (DMCG + DMCI - DMCGT) * Weff  + (NF-1)  *(DMCG - DMCGT)*Weff
      PDeff = (2*(DMCG + DMCI - DMCGT) + Weff) + (NF-1)*2*(DMCG - DMCGT)
    else:
      raise Error( 3, 'Stack.computeLayoutParasitics(): Unknown GEOMOD %s for meta-transistor \"%s\".'
                      % ( mt['style.geomod'], mt['gate'].getName() ) )

    mt['parasitics.ASeff'] = ASeff
    mt['parasitics.PSeff'] = PSeff
    mt['parasitics.ADeff'] = ADeff
    mt['parasitics.PDeff'] = PDeff

    return
