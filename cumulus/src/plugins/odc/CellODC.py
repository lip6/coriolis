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
# |  Python      :   "./plugins/odc/CellODC.py"                     |
# +-----------------------------------------------------------------+

import re

from coriolis.CRL import getLibertyGroupFromCell
from coriolis.Hurricane import Cell
from sympy import Not, S, Xor, simplify, simplify_logic, symbols
from sympy.parsing.sympy_parser import implicit_multiplication_application, parse_expr, standard_transformations


def lib_expr_parsing(expression: str, pins: dict[str, S]):
    clean_expr = expression.replace('!', '~')
    clean_expr = clean_expr.replace('"', '')
    transformations = standard_transformations + (implicit_multiplication_application,)
    parsed_func = parse_expr(clean_expr, transformations=transformations, local_dict=pins)
    return simplify(parsed_func)


class CellODC:

    def __init__(self, cell: Cell):
        try:
            master_cell = cell.getMasterCell()
        except AttributeError:
            master_cell = cell
        self._master_cell: str = master_cell.getName()
        self._pins_direction: dict[str, str] = {}  # dict of output pins, containing dict of input pins.
        self._is_ff: bool = False
        self._is_steering: bool = False
        self._observability: dict[str, dict[str]] = {} # [output_pin, [input_pin, odc_func]]
        pins_functions = {}

        grp = getLibertyGroupFromCell(master_cell)
        if grp is None:
            print(f"[WARNING] No library information attached to {self._master_cell}")
            return

        # adding pins to list
        pins = grp.getGroups("pin.*")
        for pin in pins:
            pin_name = re.search(r"\((.*)\)", pin.getGroupName()).group(1)
            pin_direction = pin.getAttribute("direction").getValue()
            self._pins_direction[pin_name] = pin_direction
        local_dict = {pin_name: symbols(pin_name) for pin_name in self._pins_direction.keys()}

        for pin in pins:
            pin_name = re.search(r"\((.*)\)", pin.getGroupName()).group(1)
            pin_direction = self._pins_direction[pin_name]
            if pin_direction == "output":
                function = pin.getAttribute("function")
                if function is None:
                    continue # no function for output in case of ff
                function = function.getValue()
                expr = lib_expr_parsing(function, local_dict)
                func_symbols = expr.free_symbols
                # checking wether function is three state
                three_state = pin.getAttribute("three_state")
                if three_state is not None:
                    self._is_steering = True
                    expr = Not(lib_expr_parsing(three_state.getValue().replace('"', ''), local_dict))
                    self._observability[pin_name] = {
                        p: expr for p in set([str(s) for s in func_symbols])
                    }
                else:
                    self._observability[pin_name] = {
                        p: S.true for p in set([str(s) for s in func_symbols])
                    }
                pins_functions[pin_name] = expr

        # checking wether cell is a flip flop
        ff_grp = grp.getGroups("ff\\(.*")
        if len(ff_grp) >= 1:  # Attention, peut être ff et steering à la fois !!!! (reset...)
            ff_grp = ff_grp[0]
            # cell is ff
            self._is_ff = True
            # extracting outputs
            match = re.search(r"\((\w*),?(\w*).*\)", ff_grp.getGroupName())
            out = None
            inv_out = None
            if match:
                out, inv_out = match.groups()
            else:
                print("[ERROR] flip-flop groups is not correctly formatted.")
                raise SyntaxError
            # will only traver through inputs used in next_state parameter
            expr = lib_expr_parsing(ff_grp.getAttribute("next_state").getValue(), local_dict)
            ff_symbols = expr.free_symbols
            for output in [p_n for p_n, p_d in self._pins_direction.items() if p_d == "output"]:
                self._observability[output] = {pin: S.true for pin in set([str(s) for s in ff_symbols])}
            return

        # if not already steering, we need to check derivative of outputs relatives to each input.
        if not self._is_steering:
            pin_not_steering = [] # we got to list all pins not steering to find the one that steers
            input_pins = [pin for pin, direction in self._pins_direction.items() if direction == "input"]
            for output_name, func in pins_functions.items():
                for pin in input_pins:
                    symbol = symbols(pin)
                    f0 = func.subs(symbol, False)
                    f1 = func.subs(symbol, True)
                    derivative = simplify_logic(Xor(f0, f1))
                    if derivative == S.true or derivative == S.false:
                        return # TODO: is is really return and not continue ?
                    symbols_of_derivative = derivative.free_symbols
                    for p in input_pins:
                        if p not in [str(s).replace("~", "") for s in symbols_of_derivative] + [pin]:
                            pin_not_steering.append(pin)
                            self._observability[output_name][pin] = derivative
            if len(pin_not_steering) > 0:
                for p in [str(s) for s in input_pins]:
                    if p not in pin_not_steering:
                        self._is_steering = True
                        for output in self._observability.keys():
                            self._observability[output].pop(p)

    @property
    def isFlipflop(self):
        return self._is_ff

    @property
    def isSteering(self):
        return self._is_steering

    def print(self):
        print(f"# {self._master_cell}")
        print("# pins")
        for pin, direction in self._pins_direction.items():
            print(f"#   {pin}:{direction}")
        print(f"# is FF        : {self._is_ff}")
        print(f"# is isSteering: {self._is_steering}")
        print("# functions")
        for opin, inputs in self._observability.items():
            print(f"#   {opin}:")
            for ipin, func in inputs.items():
                print(f"#     {ipin}: {func}")
