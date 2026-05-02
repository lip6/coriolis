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
# |  Python      :   "./plugins/odc/FFDatabase.py"                  |
# +-----------------------------------------------------------------+

from coriolis.Hurricane import Cell
from sympy import Or, S, simplify_logic

from .CellODC import CellODC


class FFEntry:
    def __init__(self):
        self.function: S = S.true
        self.name: str = ""

    def __str__(self):
        return f"{self.name}: {self.function}"

    def simplify(self):
        self.function = simplify_logic(self.function)


class FFDatabase:
    def __init__(self):
        self._ff: dict[str, FFEntry] = {}
        self._ffs: set[str] = set()
        self._len = 0
        self.edits = 0

    def __contains__(self, cell):
        if type(cell) is Cell:
            return cell.getName() in self._ff
        return cell in self._ff

    def __getitem__(self, key):
        return self._ff[key]

    def addNewFF(self, ff: Cell, ff_info: CellODC, function):
        if ff.getName() in self._ffs:
            old_entry = self._ff[ff.getName()]
            self._ff[ff.getName()].function = simplify_logic(Or(function, old_entry.function))
            self.edits += 1
            return True  # return true if walker should stop
        else:
            self._ffs.add(ff.getName())
            entry = FFEntry()
            entry.function = function
            entry.name = ff.getName()
            self._ff[ff.getName()] = entry
        return False

    def items(self):
        return self._ff.items()

    def __len__(self):
        return len(self._ff)

    def processFuncs(self):
        print("Simplifying ODC functions (simplification forced, could take some time...)")
        for nb, value in enumerate(self._ff.values()):
            print(f"{nb}/{len(self._ff)} ({nb * 100 / len(self._ff):.2f}%)")
            value.simplify()
            print("\033[F\033[K", end="")
        print("\033[F\033[K", end="")
        print("Simplification done.")
