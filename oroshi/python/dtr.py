# -*- coding: utf-8 -*-

from helpers import trace


class Rules ( object ):

  ruleSet = [ 'minSpacing_nWell'
            , 'minWidth_pImplant'
            , 'minSpacing_pImplant'
            , 'minSpacing_rpolyh_pImplant'
            , 'minEnclosure_pImplant_poly2con'
            , 'minEnclosure_nImplant_active'
            , 'minEnclosure_pImplant_active'
            , 'minSpacing_nImplant_pImplant'
            , 'minSpacing_cut0'
            , 'minWidth_cut0'
            , 'minWidth_active'
            , 'minEnclosure_active_cut0'
            , 'transistorMinL'
            , 'transistorMinW'
            , 'minSpacing_poly'
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
            , 'minSpacingWide1_metal3'
            , 'minEnclosure_metal3_cut2'
            , 'minSpacingOnMetBot_cut2'
            , 'minSpacingOnMetCap_cut2'
            , 'maxWidth_metcap'
            , 'minSpacing_metbot'
            , 'minSpacing_cut1_metcap'
            , 'minSpacing_cut2_metcap'
            , 'minEnclosure_metbot_metcap'
            , 'minEnclosure_metbot_cut1'
            , 'minEnclosure_metbot_cut2'
            , 'minEnclosure_metcap_cut2'
	    , 'minWidth_metcap'
            , 'minWidth_metcapdum'
            , 'minWidth_cpoly'
            , 'minWidth_poly2'
            , 'minWidth_rpolyh'
            , 'minWidthHighPrec_rpolyh'
            , 'minSpacing_cpoly'
            , 'minSpacing_poly2'
            , 'minSpacing_rpolyh'
            , 'minSpacing_cut0_cpoly'
            , 'minSpacing_diff_poly2'
            , 'minSpacing_poly_poly2'
            , 'minEnclosure_poly_cpoly' 
            , 'minEnclosure_cpoly_cut0'
            , 'minEnclosure_poly2_cut0'
            , 'MIMCap'
            , 'PIPCap'
            , 'MIMPerimeterCap'
            , 'PIPPerimeterCap'
            , 'RPOLYHSheetRes'
            , 'RPOLY2PHSheetRes'
            , 'MET1RPOLYHContRes'
            , 'minWidth_hres'
            , 'minSpacing_hres'
            , 'minEnclosure_hres_poly2'
            , 'minSpacing_hres_poly1'
            , 'minSpacing_hres_poly2'
            , 'minSpacing_hres_active'
            , 'corrFactor90'
            , 'corrFactor135'
            , 'minRpolyhSquares'
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
    try:
      if len(words) == 1:
        if   words[0].endswith('Cap'     ): value = self.dtr.getUnitRule( words[0] ).getValue()
        elif words[0].endswith('ContRes' ): value = self.dtr.getUnitRule( words[0] ).getValue()
        elif words[0].endswith('Res'     ): value = self.dtr.getUnitRule( words[0] ).getValue()
        elif words[0].endswith('ctor90'  ): value = self.dtr.getUnitRule( words[0] ).getValue()
        elif words[0].endswith('ctor135' ): value = self.dtr.getUnitRule( words[0] ).getValue()
        elif words[0].endswith('quares'  ): value = self.dtr.getUnitRule( words[0] ).getValue()
      if (value is None) and len(words) < 4:
        value = self.dtr.getPhysicalRule( *tuple(words) ).getValue()
    except Exception, e:
      print e

    if not value is None:
      self.__dict__[attribute] = value

    return
