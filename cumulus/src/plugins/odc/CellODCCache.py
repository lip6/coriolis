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
# |  Python      :   "./plugins/odc/CellODCCache.py"                |
# +-----------------------------------------------------------------+

from coriolis.Hurricane import Cell, Instance

from .CellODC import CellODC


class CellODCCache:
    def __init__(self):
        self._cell_cache: dict[str, CellODC] = {}

    def __getitem__(self, key):
        if type(key) is str:
            return self._cell_cache[key]
        elif type(key) is not Cell and type(key) is not Instance:
            raise TypeError
        try:
            try:
                master_cell = key.getMasterCell()
            except AttributeError:
                master_cell = key
            return self._cell_cache[master_cell.getName()]
        except KeyError:
            self._cell_cache[master_cell.getName()] = CellODC(master_cell)
            return self._cell_cache[master_cell.getName()]
