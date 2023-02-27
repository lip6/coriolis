# -*- coding: utf-8 -*-

from ..Hurricane  import DbU, DataBase
from ..helpers    import trace
from ..helpers.io import ErrorMessage as Error
            

class Rules ( object ):
    """
    The Rules object provides an easier access to the design rules stored
    in the Technology databse. Instead of having to perform a function call
    like:

    .. code:: Python

       tech  = DataBase.getDB().getTechnology()
       value = tech.getPhysicalRule( 'minEnclosure', 'pImplant', 'active' )

    We can write access the rule as an attribute of the ``rule`` object.

    .. code:: Python
   
       import oroshi

       value = oroshi.rules.minEnclosure_pImplant_active

    Only the rules defined in the Rules.ruleSet list will be loaded.
    """

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

    def __init__ ( self ):
        """
        Create an empty rule set. The rules must be loaded afterwards by calling
        ``Rules.load()``.
        """
        trace( 100, '\tRules.__init__()\n' )
        self.dtr = None

    def isLoaded ( self ):
        """Tells if the rules have already been loaded."""
        return self.dtr is not None

    def load ( self, dtr ):
        """
        Load the rule set from the technology into the Rules object.

        .. note:: The ``dtr`` parameter is just another name for the currently
                  used Hurricane::Technology.
        """
        trace( 100, '\tRules.load()\n' )
        if self.dtr:
            print( Error( 1, 'Rules.load(): Attempt to load rules from DTR a second time (ignored).' ))
            return
        self.dtr = dtr
        for rule in Rules.ruleSet: self.addAttr(rule)

    def getRealLayer ( self, stdName ):
        """
        Get a Layer object by it's name. The alias translation from generic
        names is used to return the real technology name.

        For example:

        ==================  ===================
        Generic Layer Name  SkyWater 130nm Name
        ==================  ===================
        nWell               nwm
        active              difftap
        pImplant            psdm
        cut0                licon
        metal1              li
        cut1                via
        metal2              metal1
        ==================  ===================
        """
        return self.dtr.getLayer( stdName )

    def attrTranslate ( self, attribute ):
        """
        Translate a rule complete name, given in ``attribute``, using the *generic*
        layer names into another string, using the target technology layer names.

        For example, for SkyWater 130nm: ::

            minEnclosure_pImplant_active => minSpacing_psdm_difftap
        """
        words       = attribute.split( '_' )
        translateds = [ words[0] ]
        for word in words[1:]:
            realLayer = self.getRealLayer( word )
            if realLayer is None:
                print( Error( 1, 'rules.attrTranslate(): Unable to translate generic layer "{}".' \
                                 .format( word )))
                realLayerName = word
            else:
                realLayerName = realLayer.getName()
            translateds.append( realLayerName )
        return '_'.join( translateds )
    
    def addAttr ( self, attribute ):
        """
        Add a new attribute into the dictionnary of rule set. The attribute fields,
        separated by '_' are broken down to get the corresponding rule in the
        technology and be set as value of said attribute.

        The attribute is the concatenation of the rule name and the various layers
        it applies on. For example: ::

            (minEnclosure, pImplant, active) => 'minEnclosure_pImplant_active'
        """
        techAttribute = self.attrTranslate( attribute )
        if attribute in self.__dict__: return
        
       #print( 'Rules.addAttr(): {} -> {}'.format( attribute, techAttribute ))
        value = None
        words = techAttribute.split( '_' )
        try:
            if len(words) == 1:
                if   words[0].endswith('Cap'     ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
                elif words[0].endswith('ContRes' ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
                elif words[0].endswith('Res'     ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
                elif words[0].endswith('ctor90'  ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
                elif words[0].endswith('ctor135' ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
                elif words[0].endswith('quares'  ): value = self.dtr.getUnitRule( words[0] ).getDoubleValue()
            if (value is None) and len(words) < 4:
                rule = self.dtr.getPhysicalRule( *tuple(words) )
                if rule.isDouble():
                    value = rule.getDoubleValue()
                   #print( 'Accessed value (Unit):{}'.format(value) )
                else:
                    value = rule.getValue()
                   #print( 'Accessed value (DbU):{}'.format(DbU.getValueString(value)) )
        except Exception as e:
            print( e )
        
        if not value is None:
            self.__dict__[attribute] = value
        
        return
