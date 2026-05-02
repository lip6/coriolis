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
# |  Python      :   "./plugins/odc/ODCWalker.py"                   |
# +-----------------------------------------------------------------+

from queue import LifoQueue

from coriolis.Hurricane import Net, Plug
from sympy import And, S, Symbol

from .CellODCCache import CellODCCache
from .FFDatabase import FFDatabase


def getSymbolsMap(instance):
    ret = {}
    for plug in instance.getPlugs():
        net = plug.getNet()
        master_net = plug.getMasterNet()
        if net is None:
            continue
        ret[master_net.getName()] = Symbol(net.getName(), boolean=True)
    return ret


def replaceSymbols(expr, correspondance):
    ret = expr
    ret = ret.subs(correspondance)
    return ret


class ODCWalker:
    walker_number: int = 0
    iter_count = 0

    def __init__(
        self,
        net: Net = None,
        todo: LifoQueue = None,
        results: FFDatabase = None,
        cache: CellODCCache = None,
        other=None,
        from_plug=False,
        plug: Plug = None,
        function=S.true,
    ):
        ODCWalker.walker_number += 1

        if net is None and plug is None:
            raise AttributeError
        if other is None:
            if cache is None or results is None:
                print("[ERROR] Can not build walker without cache or results.")
                raise AttributeError
            self._cache = cache
            self._from_plug = from_plug
            self._function = S.true
            self._net = net
            self._plug = plug
            self._results = results
            self._symbols: dict[str, S] = {}
            self._todo = todo
        else:  # we need to do a deep copy because this is a child of another walker.
            self._cache = other._cache  # not a copy
            self._from_plug = from_plug
            self._function = And(function, other._function)
            self._net = net
            self._plug = plug
            self._results = other._results  # not a copy
            self._symbols: dict[str, S] = other._symbols
            self._todo = other._todo  # not a copy

    def fork(self, net: Net = None, plug: Plug = None, function=S.true):
        if net:
            self._todo.put(ODCWalker(other=self, net=net, function=function))
        elif plug:
            self._todo.put(ODCWalker(other=self, plug=plug, from_plug=True, function=function))
        else:
            print("[ERROR] Calling fork for ODCWalker with no args")
            raise AttributeError

    def iterate_over_net(self):
        net = self._net
        first_plug = True
        self._plug = None
        for plug in net.getPlugs():
            master_net = plug.getMasterNet()
            if master_net.getDirection() != Net.Direction.OUT:
                continue
            if plug is None:  # end of path, no instance means external net
                continue
            if first_plug:
                self._plug = plug
                first_plug = False
            else:
                self.fork(plug=plug)

    def iterate_over_plug(self, instance, master_output, odc_info):
        # Getting out of cell
        first = True
        self._net = None
        new_function = None
        net_map = getSymbolsMap(instance)
        for plug in instance.getPlugs():  # should use list in odc_info
            net = plug.getNet()
            master_net = plug.getMasterNet()
            if master_net.getDirection() != Net.Direction.IN or master_net.isSupply() or master_net.isClock():
                continue
            if master_net.getName() not in odc_info._observability[master_output.getName()]:  # if pin is not observable
                continue  # steering pins are not observable
            if first:
                if odc_info.isSteering:
                    ext_expr = replaceSymbols(
                        odc_info._observability[master_output.getName()][master_net.getName()], net_map
                    )
                    new_function = And(self._function, ext_expr)
                self._net = net
                first = False
            else:
                function = S.true
                if odc_info.isSteering:
                    ext_expr = replaceSymbols(
                        odc_info._observability[master_output.getName()][master_net.getName()], net_map
                    )
                    function = ext_expr
                self.fork(net=net, function=function)
        if new_function is not None:
            self._function = new_function

    def run(self):
        while self._plug is not None or self._net is not None:
            ODCWalker.iter_count += 1
            if not self._from_plug:
                self.iterate_over_net()
                if self._plug is None:  # no plugs were found
                    return
            else:
                self._from_plug = False
            # All plugs explored
            instance = self._plug.getInstance()
            master_output = self._plug.getMasterNet()
            self._plug = None
            odc_info = self._cache[instance]

            # Encounters FF
            if odc_info.isFlipflop:
                stop_there = self._results.addNewFF(instance, odc_info, self._function)
                if stop_there:
                    return  # A younger mike did already leave from there
                self._function = S.true
                self._symbols = dict()

            self.iterate_over_plug(instance, master_output, odc_info)
