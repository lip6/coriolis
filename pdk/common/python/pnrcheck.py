
from coriolis.designflow.copy     import Copy
from coriolis.designflow.yosys    import Yosys
from coriolis.designflow.blif2vst import Blif2Vst
from coriolis.designflow.cougar   import Cougar
from coriolis.designflow.lvx      import Lvx
from coriolis.designflow.druc     import Druc
from coriolis.designflow.pnr      import PnR
from s2r      import S2R
from scr      import SCR
from sta      import STA
from coriolis.designflow.klayout  import DRC
from coriolis.designflow.graal    import Graal
from coriolis.designflow.dreal    import Dreal
from coriolis.designflow.alias    import Alias
from coriolis.designflow.clean    import Clean
PnR.textMode = True


UseClockTree = 0x0001
NoSynthesis  = 0x0002
NoGDS        = 0x0004
IsChip       = 0x0008
ChannelRoute = 0x0010


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
    scrTargets = [ routedName+'.ap'
                     , routedName+'.vst' 
                     , routedName+'.spi' ]
    scrFlag = SCR.Route | SCR.Place
    if SCR.Iterations_value != 0:
        scrFlag = scrFlag |  SCR.Iterations
    if SCR.RandSeed_value != 0:
        scrFlag = scrFlag |  SCR.RandSeed
    if SCR.MaxRetry_value != 0:
        scrFlag = scrFlag |  SCR.MaxRetry
    if flags & ChannelRoute:
	    rulePnR    = SCR.mkRule( 'scr', scrTargets,  rtlDepends, scrFlag )
    else:
	    rulePnR    = PnR     .mkRule( 'pnr', pnrTargets, rtlDepends, scriptMain )
    ruleCougar = Cougar.mkRule( 'cougar', routedName+'_r_ext.vst', [rulePnR], flags=Cougar.Verbose )
    if STA.flags & STA.Transistor:
       cougarFlag = Cougar.Transistor | Cougar.Verbose
    else:
       cougarFlag = 0
    ruleCougarSpi = Cougar.mkRule( 'cougarSpi', routedName+'_ext.spi', [rulePnR], cougarFlag )
    staTargets = [ routedName+'_ext.cpath.rep'
                     , routedName+'_ext.cns'
                     , routedName+'_ext.cnv'
                     , routedName+'_ext.dtx'
                     , routedName+'_ext.rcx'
                     , routedName+'_ext.rep'
                     , routedName+'_ext.slack.rep'
                     , routedName+'_ext.stat'
                     , routedName+'_ext.stm'
                     , routedName+'_ext.sto'
                     , routedName+'_ext.str' ]
    ruleSta = STA.mkRule( 'sta', staTargets, [ruleCougarSpi] )
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
                                 , flags=S2R.Verbose|S2R.NoReplaceBlackboxes|S2R.PinLayer|S2R.DeleteSubConnectors )
        ruleDRC   = DRC   .mkRule( 'drc'  , ruleGds.file_target(0) )
        ruleDreal = Dreal .mkRule( 'dreal', ruleGds.file_target(0) )

    ruleClean  = Clean .mkRule( extrasClean )

    for tag in [ 'Yosys', 'B2V', 'PnR', 'Cougar', 'Lvx', 'Druc', 'Cgt', 'CougarSpi', 'Sta', 'Scr'
               , 'Layout', 'Gds', 'DRC', 'Graal', 'Dreal', 'Clean' ]:
        rule = 'rule' + tag
        if rule in locals():
            callerGlobals[ rule ] = locals()[ rule ]
