

# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2019-2026, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Ali OUDRHIRI              |
# |  E-mail      :            ali.oudrhiri@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./common/dft.py"                      |
# +-----------------------------------------------------------------+






class DftStdCells:
    def __init__(self):
        self.dff_names = []
        self.ff_to_scanff = {}
        self.mux_name = None
        self.buf_name = None
        self.ff_pins = {}
        self.scan_pins = {}
        self.mux_pins = {}
        self.buf_pins = {}
        self.mux_orientation = "ID"
        self.ff_orientation  = "ID"
