
from ..Hurricane import DbU, DataBase
from .           import dtr

if not DataBase.getDB(): DataBase.create()

rules = dtr.Rules()


class Flag ( object ):

    StandardRules = 0x0001
    MIMRules      = 0x0002
    AllRules      = StandardRules|MIMRules


def getRules ():
    global rules
    if not rules.isLoaded():
        rules.load( DataBase.getDB().getTechnology() )
    return rules


def toNano  ( value ): return DbU.toPhysical( value, DbU.UnitPowerNano  )
def toMicro ( value ): return DbU.toPhysical( value, DbU.UnitPowerMicro )
def toUnity ( value ): return DbU.toPhysical( value, DbU.UnitPowerUnity )


def adjustOnGrid ( unit, mode='upper' ):
    if   mode == 'upper': return DbU.getOnPhysicalGrid(unit, DbU.SnapModeSuperior)
    elif mode == 'lower': return DbU.getOnPhysicalGrid(unit, DbU.SnapModeInferior)
    elif mode == 'near' : return DbU.getOnPhysicalGrid(unit, DbU.SnapModeNearest )
    else:
        print( '[ERROR] oroshi.adjustOnGrid(): "{}" is not a valid mode.'.format(mode) )
        print( '        ("upper", "lower" or "near")' )
        return 0
