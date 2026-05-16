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

from multiprocessing import Process, Queue
from queue import Empty
from queue import Queue as LocalQueue
from threading import Event, Thread

from coriolis.Hurricane import Cell
from sympy import Or, S, simplify_logic

from .CellODC import CellODC


class FFEntry:
    def __init__(self):
        self.function: S = S.true
        self.name: str = ""

    def simplify(self):
        self.function = simplify_logic(self.function)


class SimplifyTask:
    def __init__(self, name, f1, f2=None):
        self.name = name
        self.f1 = f1
        self.f2 = f2

    def compute(self):
        if self.f2 is None:
            return (self.name, simplify_logic(self.f1), False)
        return (self.name, simplify_logic(Or(self.f1, self.f2)), False)


class FFDatabase:
    def __init__(self, done, nb_simplifiers=16):
        self._ff_to_process: dict[str, list] = {}
        self._ff: dict[str, FFEntry] = {}
        self._ffs: set[str] = set()
        self._len = 0
        self._processor_thread = None
        self._results_thread = None
        self._process = []
        self._simplify_task = Queue()
        self._simplify_results = Queue()
        self._nb_proc = nb_simplifiers
        self._results = LocalQueue()
        self.done = Event()
        self.task_done = 0
        self._task_given = 0
        self._task_skipped = 0
        self._outside_done = done
        self.nets_true = set()

    def __contains__(self, cell):
        if type(cell) is Cell:
            return cell.getName() in self._ff
        return cell in self._ff

    def __getitem__(self, key):
        return self._ff[key]

    def addNewFF(self, ff: Cell, ff_info: CellODC, function):
        name = ff.getName()
        ret = name in self._ffs
        self._ffs.add(name)
        self._results.put((name, function, True))
        return ret

    def items(self):
        return self._ff.items()

    def __len__(self):
        return len(self._ff)

    def function_processor(self):
        while not self.done.is_set():
            name, function, raw = self._results.get()
            try:
                if len(self._ff_to_process[name]) >= 1:
                    if self._ff_to_process[name][0] != S.true and function != S.true:
                        task = SimplifyTask(name, function, self._ff_to_process[name].pop(0))
                        self._simplify_task.put(task)
                        self._task_given += 1
                    else:
                        self._ff_to_process[name][0] = S.true
                        self._task_skipped += 1
                else:
                    if raw and function != S.true:
                        task = SimplifyTask(name, function)
                        self._simplify_task.put(task)
                        self._task_given += 1
                    else:
                        self._ff_to_process[name].append(function)
                        self._task_skipped += 1
            except KeyError:
                if raw and function != S.true:
                    task = SimplifyTask(name, function)
                    self._simplify_task.put(task)
                    self._task_given += 1
                else:
                    self._ff_to_process[name] = [function]
                    self._task_skipped += 1
            if self._results.empty():
                if (
                    self.task_done == self._task_given
                    and self._simplify_task.empty()
                    and self._simplify_results.empty()
                    and self._outside_done.is_set()
                ):
                    self.done.set()
                    for _ in range(self._nb_proc):
                        self._simplify_task.put((None))  # poison
                    self._simplify_results.put(None)

    def get_results(self):
        while not self.done.is_set():
            result = self._simplify_results.get()
            if result is None:
                break
            self._results.put(result)
            self.task_done += 1

    def start(self):
        for _ in range(self._nb_proc):
            self._process.append(
                Process(
                    target=RunSimplify,
                    args=(
                        self._simplify_task,
                        self._simplify_results,
                    ),
                )
            )
        self._results_thread = Thread(target=self.get_results)
        self._results_thread.start()
        self._processor_thread = Thread(target=self.function_processor)
        self._processor_thread.start()
        for p in self._process:
            p.start()

    def join(self):
        for p in self._process:
            p.join()
        if self._results_thread is not None:
            self._results_thread.join()
        if self._processor_thread is not None:
            self._processor_thread.join()
        self._ff = {name: functions[0] for name, functions in self._ff_to_process.items()}


def RunSimplify(tasks, results):
    while True:
        task = tasks.get()
        if task is None:
            break
        results.put(task.compute())
