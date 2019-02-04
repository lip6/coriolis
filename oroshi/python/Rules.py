# -*- coding: utf-8 -*-

from helpers import trace


class Rules ( object ):

  ruleSet = [ 'minEnclosure_nImplant_active'
            , 'minEnclosure_pImplant_active'
            , 'minSpacing_nImplant_pImplant'
            , 'minSpacing_cut0'
            , 'minWidth_cut0'
            , 'minWidth_active'
            , 'minEnclosure_active_cut0'
            , 'transistorMinL'
            , 'transistorMinW'
            , 'minGateSpacing_poly'
            , 'minSpacing_poly_active'
            , 'minExtension_active_poly'
            , 'minExtension_poly_active'
            , 'minEnclosure_poly_cut0'
            , 'minSpacing_cut0_poly'
            , 'minWidth_cut0'
            , 'minSpacing_cut0_active'
            , 'minWidth_metal1'
            , 'minSpacing_metal1'
            , 'minEnclosure_metal1_cut0'
            , 'minEnclosure_metal1_cut1'
            , 'minWidth_cut1'
            , 'minSpacing_cut1'
            , 'minWidth_metal2'
            , 'minSpacing_metal2'
            , 'minEnclosure_metal2_cut1'
            , 'minEnclosure_metal2_cut2'
            , 'minWidth_cut2'
            , 'minSpacing_cut2'
            , 'minWidth_cut1'
            , 'minSpacing_cut1'
            , 'minWidth_metal3'
            , 'minSpacing_metal3'
            , 'minEnclosure_metal3_cut2'
            ]

  def __init__ ( self, dtr ):
    trace( 100, '\tRules.__init__()\n' )
    self.dtr   = dtr

    for rule in Rules.ruleSet: self.addAttr(rule)
    return

  def addAttr ( self, attribute ):
    if self.__dict__.has_key(attribute): return

   #print 'Rules.addAttr(): %s' % attribute
    value = None
    words = attribute.split( '_' )
    if len(words) == 1 and words[0].endswith('Cap'): value = self.dtr.getUnitRule( words[0] ).getValue()
    if len(words) < 4:                               value = self.dtr.getPhysicalRule( *tuple(words) ).getValue()
    if not value is None:
      self.__dict__[attribute] = value

    return
