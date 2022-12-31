

from pathlib import Path
from .vasy   import Vasy
from .boom   import Boom
from .boog   import Boog
from .loon   import Loon
from .clean  import Clean


HasFsm = 0x00000001


def mkRuleSet ( callerGlobals, behFile, flags=0 ):
    if not isinstance(behFile,Path):
        behFile = Path( behFile )

    ruleStem  = behFile.stem
    toolFlags = Vasy.AddPowerSupplies|Boom.LocalOptimization
    if behFile.suffix in ('.vhdl', '.vhd', '.vlog', '.v'):
        vasyTargets = [ Path(behFile.stem + '.vbe') ]
        if flags & HasFsm:
            vasyTargets = [ Path(behFile.stem+'_model.vbe')
                          , Path(behFile.stem+'.vst') ]
        ruleVasy = Vasy.mkRule( 'vasy_'+ruleStem, vasyTargets
                                                , behFile
                                                , flags=toolFlags )
        behFile = vasyTargets[0]

    ruleBoom = Boom.mkRule( 'boom_'+ruleStem, behFile.stem + '_boom.vbe'
                                            , behFile.stem + '.vbe'
                                            , flags=toolFlags )
    ruleBoog = Boog.mkRule( 'boog_'+ruleStem, behFile.stem + '_boog.vst'
                                            , ruleBoom
                                            , flags=toolFlags )
    ruleLoon = Loon.mkRule( 'loon_'+ruleStem, behFile.stem + '.vst'
                                            , ruleBoog
                                            , flags=toolFlags )

    for tag in [ 'Vasy', 'Boom', 'Boog', 'Loon' ]:
        rule = 'rule' + tag
        if rule in locals():
            callerGlobals[ rule+'_'+ruleStem ] = locals()[ rule ]
