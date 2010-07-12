#!/usr/bin/env python

from stratus import *

class addaccu ( Model ) :

  def Interface ( self ) :
    self.nbit = self._param['nbit']
      
    self.a    = SignalIn ( "a", self.nbit )
    self.b    = SignalIn ( "b", self.nbit )
    self.c    = SignalIn ( "c", self.nbit )
    self.v    = SignalIn ( "v", 1 )
    self.cmd  = SignalIn ( "cmd", 1 )
    
    self.cout = SignalOut ( "cout", 1 )
    self.s    = SignalOut ( "s", self.nbit )
    
    self.vdd = VddIn ( "vdd" )
    self.vss = VssIn ( "vss" )
    
  def Netlist ( self ) :
    d_aux = Signal ( "d_aux", self.nbit )
    e_aux = Signal ( "e_aux", self.nbit )
    ovr   = Signal ( "ovr", 1  )


    Generate ( "DpgenNand2", "nand2%d" % self.nbit
             , param = { 'nbit'     : self.nbit
                       , 'physical' : True
                       }
             )
    temp = Cat ( self.v, self.v, self.v, self.v )
    self.instNand2 = Inst ( "nand2%d" % self.nbit, "instance_nand2"
                          , map   = { 'i0'   : temp
                                    , 'i1'   : self.a
                                    , 'nq'   : d_aux
                                    , 'vdd'  : self.vdd
                                    , 'vss'  : self.vss
                                    }
                           )

    Generate ( "DpgenOr2", "or2%d" % self.nbit
             , param = { 'nbit'     : self.nbit
                       , 'physical' : True
                       }
             )
    self.instOr2   = Inst ( "or2%d" % self.nbit, "instance_or2"
                          , map   = { 'i0'   : d_aux
                                    , 'i1'   : self.b
                                    , 'q'    : e_aux
                                    , 'vdd'  : self.vdd
                                    , 'vss'  : self.vss
                                    }
                          )

    Generate ( "DpgenAdsb2f", "adder%d" % self.nbit
             , param = { 'nbit'     : self.nbit
                       , 'physical' : True
                       }
             )
    self.instAdd2  = Inst ( "adder%d" % self.nbit, "instance_add2"
                          , map   = { 'i0'      : e_aux
                                    , 'i1'      : self.c
                                    , 'q'       : self.s
                                    , 'add_sub' : self.cmd
                                    , 'c31'     : self.cout
                                    , 'c30'     : ovr
                                    , 'vdd'     : self.vdd
                                    , 'vss'     : self.vss
                                    }
                          )

  def Layout ( self ) :
    Place      ( self.instNand2, NOSYM, XY ( 0, 0 ) )
    PlaceRight ( self.instOr2,   NOSYM )
    PlaceRight ( self.instAdd2,  NOSYM )


