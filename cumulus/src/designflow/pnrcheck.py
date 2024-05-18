
from .copy     import Copy
from .yosys    import Yosys
from .blif2vst import Blif2Vst
from .cougar   import Cougar
from .lvx      import Lvx
from .druc     import Druc
from .pnr      import PnR
from .s2r      import S2R
from .klayout  import DRC
from .graal    import Graal
from .dreal    import Dreal
from .alias    import Alias
from .clean    import Clean
PnR.textMode = True


UseClockTree = 0x0001
NoSynthesis  = 0x0002
NoGDS        = 0x0004
IsChip       = 0x0008


def mkRuleSet ( callerGlobals, vlogDesignName, flags=0, extraRtlDepends=[], extrasClean=[] ):
    from doDesign import scriptMain

    vhdlDesignName = vlogDesignName.lower()
    if flags & IsChip:
        routedName = 'chip_r'
    else:
        routedName = vhdlDesignName
        if flags & UseClockTree:
            routedName = vhdlDesignName + '_cts'
        routedName = routedName + '_r'

    if not (flags & NoSynthesis):
        ruleYosys  = Yosys   .mkRule( 'yosys', vlogDesignName+'.v' )
        ruleB2V    = Blif2Vst.mkRule( 'b2v'  , [ vhdlDesignName+'.vst' ]
                                             , [ruleYosys]
                                             , flags=0 )
        rtlDepends = [ ruleB2V ]
        pnrTargets = [ routedName+'.ap'
                     , routedName+'.vst'
                     , routedName+'.spi' ]
    else:
        rtlDepends = [ vhdlDesignName+'.vst' ]
        pnrTargets = [ routedName+'.ap'
                     , routedName+'.vst'
                     , routedName+'.spi' ]
    rtlDepends += extraRtlDepends
    rulePnR    = PnR     .mkRule( 'pnr', pnrTargets, rtlDepends, scriptMain )
    ruleCougar = Cougar.mkRule( 'cougar', routedName+'_r_ext.vst', [rulePnR], flags=Cougar.Verbose )
    ruleLvx    = Lvx   .mkRule( 'lvx'
                              , [ rulePnR.file_target(1)
                                , ruleCougar.file_target(0) ]
                              , flags=Lvx.Flatten )
    ruleDruc   = Druc  .mkRule( 'druc'  , [rulePnR], flags=0 )
    ruleLayout = Alias .mkRule( 'layout', [rulePnR] )
    ruleCgt    = PnR   .mkRule( 'cgt' )
    ruleGraal  = Graal .mkRule( 'graal', rulePnR.file_target(0) )

    if not (flags & NoGDS):
        ruleGds   = S2R   .mkRule( 'gds', rulePnR.file_target(0).with_suffix('.gds'), rulePnR.file_target(0)
                                 , flags=S2R.Verbose|S2R.NoReplaceBlackboxes )
        ruleDRC   = DRC   .mkRule( 'drc'  , ruleGds.file_target(0) )
        ruleDreal = Dreal .mkRule( 'dreal', ruleGds.file_target(0) )

    ruleClean  = Clean .mkRule( extrasClean )

    for tag in [ 'Yosys', 'B2V', 'PnR', 'Cougar', 'Lvx', 'Druc', 'Cgt'
               , 'Layout', 'Gds', 'DRC', 'Graal', 'Dreal', 'Clean' ]:
        rule = 'rule' + tag
        if rule in locals():
            callerGlobals[ rule ] = locals()[ rule ]
