# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2023, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                              Hippolyte MELICA    |
# |  E-mail      :   hippolyte.melica@etu.sorbonne-universite.fr    |
# | =============================================================== |
# |  Python      :   "./plugins/liberty.py"                         |
# +-----------------------------------------------------------------+

import os

def initLibertyLib(filepath:str) -> bool:
    from coriolis import CRL
    if not os.path.isfile(filepath):
        print(f"[ERROR] {filepath} liberty file not found.")
        return
    print(f"[INFO] Liberty information will be parsed from {filepath}")
    LibertyLib = CRL.LibertyLibrary(filepath=filepath)
    if LibertyLib.load():
        print("[INFO] Library file", filepath, "was successfully parsed.")
    else:
        print("[ERROR] parsing of ", filepath, " was not successful. CG will fail.")
        return
    LibertyLib.mapLibertyToDb() # at this point db owns LibertyLib
    print("[INFO] Liberty informations were added to Hurricane database.")
