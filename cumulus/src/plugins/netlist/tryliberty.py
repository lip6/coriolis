from liberty.parser import parse_liberty
from liberty.types import *
from sympy import parse_expr

liberty_file = '/dsk/l1/misc/roselyne/coriolis/src/alliance-check-toolkit/cells/nsxlib/nsxlib.lib'
library = parse_liberty(open(liberty_file).read())

fdict = {}

# cell = select_cell(library, 'nmx2_x4')
# pins = cell.get_groups('pin')
# pinq = select_pin(cell, 'nq')
# f = pinq['function']
# expr = str(f).replace("!","~").replace("\"","")
# print(parse_expr(expr))

for cell_group in library.get_groups('cell'):
    print("\n" + cell_group.args[0])
    #print(cell_group.__repr__()

    for pin_group in cell_group.get_groups('pin'):
        print("   " + pin_group.args[0])
        pin = select_pin(cell_group, pin_group.args[0])
        #print(pin)
        #print(pin.__repr__())
        if pin['direction'] == 'output':
            f = str(parse_expr(str(pin['function']).replace("!","~").replace("\"","")))
            val = (cell_group.args[0],float(pin['capacitance']))
            print("   f: " + f)
            print("   cap: " + str(pin['capacitance']))
            try:
                fdict[f].append(val)
            except KeyError:
                fdict[f] = [val]

for (k,v) in fdict.items():
    print(k,v)
