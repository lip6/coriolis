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
# |  Python      :   "./plugins/odc/odc.py"                         |
# +-----------------------------------------------------------------+

from collections import OrderedDict
from datetime import datetime
from queue import LifoQueue
from threading import Event, Thread

from coriolis.Hurricane import Cell, Net

from .CellODCCache import CellODCCache
from .FFDatabase import FFDatabase
from .ODCWalker import ODCWalker


class odc:
    def __init__(self, circuit: Cell):
        self._cell: Cell = circuit
        self._todo: LifoQueue = LifoQueue()
        self._db: FFDatabase = FFDatabase()
        self._cache: CellODCCache = CellODCCache()
        self._done: Event = Event()

        for net in self._cell.getExternalNets():
            if net.getDirection() == Net.Direction.OUT:
                self._todo.put(ODCWalker(net=net, todo=self._todo, results=self._db, cache=self._cache))

    def run_odc(self):
        ODCWalker.walker_number = 0
        ODCWalker.iter_count = 0
        try:
            while not self._todo.empty():
                task = self._todo.get()
                task.run()
        except BaseException as e:
            self._done.set()
            raise e
        self._done.set()

    def computeODC(self, force_simplify=False, refresh_rate=2):
        started = datetime.now()
        print(f"Starting at {str(started).split('.')[0]}")
        runner = Thread(target=self.run_odc)
        runner.start()
        prev_walker_number = 0
        avg_walker_growth = []
        prev_walker_alive = 0
        avg_walker_alive = []
        prev_iter_count = 0
        avg_iter_speed = []
        print(f"Stats (live, refresh every {refresh_rate}s) :")
        while not self._done.is_set():
            print(f"Elapsed time : {str(datetime.now() - started).split('.')[0]}")
            walker_number = ODCWalker.walker_number
            walker_growth = (walker_number - prev_walker_number) / refresh_rate
            avg_walker_growth.append(walker_growth)
            print(f"Walker created : {walker_number} ({walker_growth:+} walker/s)")
            prev_walker_number = walker_number
            walker_alive = self._todo.qsize()
            avg_walker_alive.append(walker_alive)
            print(f"Walker alive : {walker_alive} ({(walker_alive - prev_walker_alive) / refresh_rate:+} walker/s)")
            prev_walker_alive = walker_alive
            iter_count = ODCWalker.iter_count
            iter_speed = (iter_count - prev_iter_count) / refresh_rate
            avg_iter_speed.append(iter_speed)
            print(f"Iterations : {iter_count} ({iter_speed:} iterations/s)")
            prev_iter_count = iter_count
            self._done.wait(timeout=refresh_rate)
            print("\033[F\033[K" * 4, end="")
        print("\033[F\033[K", end="")
        runner.join()
        print("Stats :")
        print(f"  Elapsed time : {str(datetime.now() - started).split('.')[0]}")
        print(f"  Walkers : {ODCWalker.walker_number}")
        print(f"    avg. growth : {sum(avg_walker_growth) / len(avg_walker_growth):+.2f} walker/s")
        print(f"    avg. alive  : {sum(avg_walker_alive) / len(avg_walker_alive):.2f} walker")
        print(f"  Iterations : {ODCWalker.iter_count}")
        print(f"    avg. speed  : {sum(avg_iter_speed) / len(avg_iter_speed):.2f} iteration/s")
        print(f"    it. per w.  : {ODCWalker.iter_count/ODCWalker.walker_number:.2f} iteration/walker")
        print("Iteration repartition")
        for index, count in enumerate(ODCWalker.iter_rep):
            print(f"{index}: {count}")
        if force_simplify:
            self._db.processFuncs()
        print("ODC done.")

    def save_to_file(self, filename="odc_results.odc"):
        if not self._done.is_set():
            print("[ERROR] ODC was not calculated yet, can not save to file.")
        results = OrderedDict(sorted(self._db.items(), key=lambda item: item[0]))
        with open(filename, "w") as f:
            for key, value in results.items():
                f.write(f"{key}: {value}\n")
