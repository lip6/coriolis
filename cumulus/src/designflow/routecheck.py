
from .copy     import Copy
from .cougar   import Cougar
from .lvx      import Lvx
from .druc     import Druc
from .pnr      import PnR
from .alias    import Alias
from .clean    import Clean
PnR.textMode = True

def mkRuleSet ( callerGlobals, designName ):
    from doDesign import scriptMain

    rulePnR   = PnR .mkRule( 'pnr', [ designName+'_r.ap'
                                    , designName+'_r.vst'
                                    , designName+'_r.spi'
                                    ]
                                  , [ designName+'.vst'
                                    , designName+'.ap'
                                    ]
                                  , scriptMain )
    ruleCougar = Cougar.mkRule( 'cougar', designName+'_r_ext.vst', [rulePnR], flags=Cougar.Verbose )
    ruleLvx    = Lvx   .mkRule( 'lvx'
                              , [ rulePnR.file_target(1)
                                , ruleCougar.file_target(0) ]
                              , flags=Lvx.Flatten )
    ruleDruc   = Druc  .mkRule( 'druc'  , [rulePnR], flags=0 )
    ruleLayout = Alias .mkRule( 'layout', [rulePnR] )
    ruleCgt    = PnR   .mkRule( 'cgt' )
    ruleClean  = Clean .mkRule()

    for tag in [ 'PnR', 'Cougar', 'Lvx', 'Druc', 'Cgt', 'Layout', 'Clean' ]:
        rule = 'rule' + tag
        callerGlobals[ rule ] = locals()[ rule ]
