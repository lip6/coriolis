
from .copy     import Copy
from .yosys    import Yosys
from .blif2vst import Blif2Vst
from .cougar   import Cougar
from .lvx      import Lvx
from .druc     import Druc
from .pnr      import PnR
from .alias    import Alias
from .clean    import Clean
PnR.textMode = True


UseClockTree = 0x0001
NoSynthesis  = 0x0002
IsChip       = 0x0004


def mkRuleSet ( callerGlobals, vlogDesignName, flags=0, extraRtlDepends=[], extrasClean=[] ):
    from doDesign import scriptMain

    vhdlDesignName = vlogDesignName.lower()
    if flags & IsChip:
        routedName = 'chip'
    else:
        routedName = vhdlDesignName
        if flags & UseClockTree:
            routedName = vhdlDesignName + '_cts'

    if not (flags & NoSynthesis):
        ruleYosys  = Yosys   .mkRule( 'yosys', vlogDesignName+'.v' )
        ruleB2V    = Blif2Vst.mkRule( 'b2v'  , [ vhdlDesignName+'.vst'
                                               , vlogDesignName+'.spi' ]
                                             , [ruleYosys]
                                             , flags=0 )
        rtlDepends = [ ruleB2V ]
        pnrTargets = [ routedName+'_r.ap'
                     , routedName+'_r.vst'
                     , routedName+'_r.spi' ]
    else:
        rtlDepends = [ vhdlDesignName+'.vst' ]
        pnrTargets = [ routedName+'_r.ap'
                     , routedName+'_r.vst'
                     , routedName+'_r.spi'
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
    ruleClean  = Clean .mkRule( extrasClean )

    for tag in [ 'Yosys', 'B2V', 'PnR', 'Cougar', 'Lvx', 'Druc', 'Cgt', 'Layout', 'Clean' ]:
        rule = 'rule' + tag
        if rule in locals():
            callerGlobals[ rule ] = locals()[ rule ]
