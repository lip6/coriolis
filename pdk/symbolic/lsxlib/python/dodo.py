
from coriolis.designflow.technos import setupLCMOS

setupLCMOS()

DOIT_CONFIG = { 'verbosity' : 2 }

from coriolis.designflow.cougar   import Cougar
from coriolis.designflow.lvx      import Lvx
from coriolis.designflow.druc     import Druc
from coriolis.designflow.pnr      import PnR
from coriolis.designflow.yosys    import Yosys
from coriolis.designflow.blif2vst import Blif2Vst
from coriolis.designflow.clean    import Clean
PnR.textMode = True

ruleCgt = PnR.mkRule( 'cgt' )
