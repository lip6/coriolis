# example of standalone use:
# python amplify.py -l gf180mcu -n arlet6502 -a amp:30 -o vlog

from coriolis import Hurricane, CRL

from liberty.parser import parse_liberty
from liberty.types import *
from sympy import parse_expr, Id
from coriolis.helpers.overlay import UpdateSession

# Read liberty file to create a dictionary where:
# - key is the canonical logic function of the cell (thanks to sympy read_expr)
# - value is a list of tuples (cell's name,capa)
# This dictionary helps to choose the appropriate cell for amplification
# return: the created dictionary
# TODO: add decorator to Hurricane library?
def read_liberty(liberty_file):
    #print("LIBERTY="+liberty_file)
    library = parse_liberty(open(liberty_file).read())
    fdict = {}
    for cell_group in library.get_groups('cell'):
        #print(cell_group.__repr__())
        for pin_group in cell_group.get_groups('pin'):
            #print("   " + pin_group.args[0])
            pin = select_pin(cell_group, pin_group.args[0])
            #print(pin)
            #print(pin.__repr__())
            if pin['direction'] == 'output':
                #print(pin['function'])
                # lower() because I is considered as Imaginary by sympy
                # and replace for coherence with sympy operators
                f = parse_expr(str(pin['function']).lower().replace("!","~").replace("\"",""))
                #print(f.args,f.func)
                #print(cell_group.args,pin)
                # replace to be coherent with Coriolis cells' naming
                #val = (cell_group.args[0].replace("__","_"),float(pin['max_capacitance']))
                val = (cell_group.args[0],float(pin['max_capacitance']))
                #print("   f: " + str(f))
                #print("   cap: " + str(pin['max_capacitance']))
                if (type(f) == sympy.core.symbol.Symbol) and (str(f) != 'IQ'): # buffer
                    f = 'buf'
                # threestate
                if pin['three_state']: f="ts"
                # latch
                if cell_group.get_groups('latch'): f="latch"
                # clock gating
                if cell_group['clock_gating_integrated_cell']: f="clkg"
                # flip-flop
                ff = cell_group.get_groups('ff')
                if ff:
                    f = "ff"
                    #print(ff.__repr__())
                    if cell_group.get_groups('test_cell'): f+='_test'
                    if ff[0]['clear']: f+='_r'
                    if ff[0]['preset']: f+='_s'
                try:
                    fdict[str(f)].append(val)
                except KeyError:
                    fdict[str(f)] = [val]
    # sort by capicitance values
    for v in fdict.values():
        v.sort(key=lambda gate: gate[1])
    return fdict

# Pretty display for the dictionary returned by read_liberty
def pretty_display(libdict):
    for k in libdict.keys():
        print(k,':')
        for e in libdict[k]:
            print("         ",e)

# Find a cell given by its model's name in a given liberty library
# return the corresponding (key,value), False if not found
def find_in_liberty(cell,lib):
    for (k,v) in lib.items():
        for l in v:
            if l[0] == cell:
                return (k,v)
    return False

# TODO: move this function to a common place as it is usefull for lots of operations
# Return the number of elements (size) of a given collection 
def size(collection):
    total = 0
    for c in collection:
        total += 1
    return total

# TODO: see if this function is not already written
# Yes it is but doesn't work
# Return if a net is a clock or not based on its name
def isClock(net):
    return net.getName() in ['ck', 'CK', 'clk', 'CLK']

# Amplify a net by:
# - adding a buffer (tech == 'buf')
# - or amplifying the previous cell (tech == 'amp')
# parameters:
# - net is the net to amplify
# - tech is the technic used (buffer or cell amplification)
# - lib is the liberty library
# - hlib is the Hurricane library
# raise ValueError if the technic is not defined
def amplify_net(net, tech, lib, hlib):
    # find the source plug
    p_found = False # keep False if the net is an input TODO: how to bufferize?
    for p in net.getPlugs():
        if p.getMasterNet().getDirection() == Hurricane.Net.Direction.OUT:
            p_found = p
            break
    # no output plug found mean net is an input of the circuit then exit
    # TODO: think how to do on inputs
    if not p_found:
        return
    # modify the source plug net
    if tech == 'buf':
        # Add a buffer
        p_found.setNet(bufferize(net,lib,hlib))
    elif tech == 'amp':
        # Get the model of the instance of the found plug
        model = p_found.getInstance().getMasterCell().getName()
        # Find the cell in the library
        res = find_in_liberty(model,lib)
        new_cell = choose_cell(res)
        # Amplify the source cell
        p_found.getInstance().setMasterCell(hlib.getCell(new_cell))
    else:
        raise ValueError(f'Unknown technic {tech}')

# Choose the remplacing cell (biggest one)
# TODO: think about a searching technic
def choose_cell(dict_elem):
    # search the cell with the max capacitance: the last one according to the sorting creation of the dictionary
    return dict_elem[1][-1][0]

# Add a buffer to a given net to amplify it
# parameters:
# - net is the net to amplify
# - cell is the cell in which the net is defined
# - lib is the liberty library
# - hlib is the Hurricane library
def bufferize(net, lib, hlib):
    # create the buffer instance
    # take a buffer cell in the liberty library
    # TODO: method to find a better one
    buf_m = hlib.getCell(lib['buf'][2][0])
    with UpdateSession():
        buf_i = Hurricane.Instance.create(net.getCell(), f'buf_{net.getName()}', buf_m)
    for n in buf_m.getExternalNets():
        if (n.getDirection() == Hurricane.Net.Direction.IN) and not n.isSupply():
            buf_in = n.getName()
        elif n.getDirection() == Hurricane.Net.Direction.OUT:
            buf_out = n.getName()
    # insert this instance to the net, i.e. cut the net into net_b (source to be created) and net
    buf_i.getPlug(buf_m.getNet(buf_out)).setNet(net)
    with UpdateSession():
        net_b = Hurricane.Net.create(net.getCell(), "%s_b" %(net.getName()))
    buf_i.getPlug(buf_m.getNet(buf_in)).setNet(net_b)
    return net_b

# Amplify a given cell using the corresponding technic
# Parameters:
# - lib is the liberty dictionary provided by read_liberty
# - cell is the cell to amplify
# - tech is the technic used (buffer or cell amplification)
# - threshold: if net has a load (in number of target cells) > threshold then amplify
def amplify(lib, hlib, cell, tech, threshold=0):
    for net in cell.getNets():
        if not isClock(net):
            s = size(net.getPlugs())
            if s > threshold:
                print(net.getName() + " th: " + str(s))
                amplify_net(net,tech,lib,hlib)
    

def usage():
    print("python utilities.py [option]")
    print("Utility functions to amplify nets from a netlist")
    print("-n (blif): the name of the blif netlist")
    print("-a (tech:th): the amplification technique used (buf to bufferize the nets and amp to amplify the cells)")
    print("                 - th is the threshold corresponding to the load of a signal (in number of targetting cells)")
    print("-o (output format): vlog or vst")
    print("-h (help): this message")

# main for standalone usage
if __name__ == '__main__':
    import getopt, sys
    
    tech = ""
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hn:a:o:l:", ["help", "blif=", "amp", "buf", "vlog", "vst", "lib="])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    for o, a in opts:
        if o == "-h":
            usage()
        if o == '-n':
            cell = CRL.Blif.load(a)
            fname = a
        if o == "-a":
            tech,th = a.split(':')
        if o == '-o':
            output = a
        if o == '-l':
            from coriolis.designflow.yosys    import Yosys
            from pathlib import Path
            import importlib
            mod = importlib.import_module("pdks."+a)
            
            mod.setup( useHV=True )
            liberty = Yosys._liberty
            libdict = read_liberty(liberty)
            # stem[0:-1] due to bug in Coriolis: mcu9t5v insteed mcu9t5v0
            hlib = Hurricane.DataBase.getDB().getRootLibrary().getLibrary(Path(liberty).stem[0:-1])
            
    print(f'Amplify the nets of {fname} with a threshold of {th} with {tech}')
    amplify(libdict,hlib,cell,tech,int(th))
    if output == 'vlog':
        CRL.Verilog.save(cell, True)
    elif output == 'vst':
        AF = CRL.AllianceFramework.get()
        AF.saveCell(cell,CRL.Catalog.State.Logical)
    else: raise ValueError("not implemented format " + output)
