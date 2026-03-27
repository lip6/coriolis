import getopt, sys

from coriolis.designflow.technos import setupSky130_nsx2
from coriolis.designflow.yosys import Yosys
from coriolis import CRL
from coriolis import Hurricane

from liberty.parser import parse_liberty
from liberty.types import *
from sympy import parse_expr, Id

setupSky130_nsx2( checkToolkit='../../..' )

LIB = '/dsk/l1/misc/roselyne/coriolis/src/alliance-check-toolkit/cells/nsxlib/nsxlib.lib'
#LIB = Yosys.liberty # doesn't work ???
print("LIB = " + str(LIB))
AF = CRL.AllianceFramework.get()
Hurricane.UpdateSession.open()

# Read liberty file to create a dictionary where:
# - key is the canonical logic function of the cell (thanks to sympy read_expr)
# - value is a list of dictionaries (cell's name,capa)
# This dictionary helps to choose the appropriate cell for amplification
# return: the created dictionary
# TODO: add decorator to cells?
def read_liberty():
    # TODO: read from setup
    liberty_file = LIB
    #liberty_file = Yosys.liberty
    print(liberty_file)
    library = parse_liberty(open(liberty_file).read())
    fdict = {}
    for cell_group in library.get_groups('cell'):
        #print("\n" + cell_group.args[0])
        #print(cell_group.__repr__()
        for pin_group in cell_group.get_groups('pin'):
            #print("   " + pin_group.args[0])
            pin = select_pin(cell_group, pin_group.args[0])
            #print(pin)
            #print(pin.__repr__())
            if pin['direction'] == 'output':
                f = parse_expr(str(pin['function']).replace("!","~").replace("\"",""))
                #print(f.args,f.func)
                val = (cell_group.args[0],float(pin['capacitance']))
                #print("   f: " + str(f))
                #print("   cap: " + str(pin['capacitance']))
                if (type(f) == sympy.core.symbol.Symbol) and (str(f) != 'IQ'): # buffer
                    f = 'buf'
                try:
                    fdict[str(f)].append(val)
                except KeyError:
                    fdict[str(f)] = [val]
    return fdict

# Find a cell given by its model's name in a given liberty librairy
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
# raise ValueError if the technic is not defined
def amplify_net(net, tech, lib):
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
        p_found.setNet(bufferize(net,lib))
    elif tech == 'amp':
        # Get the model of the instance of the found plug
        model = p_found.getInstance().getMasterCell().getName()
        # Find the cell in the library
        res = find_in_liberty(model,lib)
        new_cell = choose_cell(res)
        # Amplify the source cell 
        p_found.getInstance().setMasterCell(AF.getCell(new_cell, CRL.Catalog.State.Views))
    else:
        raise ValueError(f'Unknown technic {tech}')

# Choose the remplacing cell (biggest one)
# TODO: think about a searching technic
def choose_cell(dict_elem):
    # search the cell with the max capacitance
    dict_elem[1].sort(key=lambda capa: capa[1])
    return dict_elem[1][-1][0]

# Add a buffer to a given net to amplify it
# parameters:
# - net is the net to amplify
# - cell is the cell in which the net is defined
# - lib is the liberty library 
def bufferize(net, lib):
    # create the buffer instance
    # take a buffer cell in the liberty library
    # TODO: method to find a better one
    buf_m = AF.getCell(lib['buf'][0][0], CRL.Catalog.State.Views)
    buf_i = Hurricane.Instance.create(net.getCell(), f'buf_{net.getName()}', buf_m)
    for n in buf_m.getExternalNets():
        if (n.getDirection() == Hurricane.Net.Direction.IN) and not n.isSupply():
            buf_in = n.getName()
        elif n.getDirection() == Hurricane.Net.Direction.OUT:
            buf_out = n.getName()
    # insert this instance to the net, i.e. cut the net into net_b (source to be created) and net
    buf_i.getPlug(buf_m.getNet(buf_out)).setNet(net)
    net_b = Hurricane.Net.create(net.getCell(), "%s_b" %(net.getName()))
    buf_i.getPlug(buf_m.getNet(buf_in)).setNet(net_b)
    return net_b

# Amplify a given cell using the corresponding technic
# Parameters:
# - cell is the cell to amplify
# - tech is the technic used (buffer or cell amplification)
# - threshold: if net has a load (in number of target cells) > threshold then amplify
def amplify(cell, tech, threshold=0):
    lib = read_liberty()
    for net in cell.getNets():
        if not isClock(net):
            s = size(net.getPlugs())
            if s > threshold:
                print(net.getName() + " th: " + str(s))
                amplify_net(net,tech,lib)

def usage():
    print("python utilities.py [option]")
    print("Utility functions to amplify nets from a netlist")
    print("-n (blif): the name of the blif netlist")
    print("-a (tech:th): the amplification technique used (buf to bufferize the nets and amp to amplify the cells)")
    print("                 - th is the threshold corresponding to the load of a signal (in number of targetting cells)")
    print("-h (help): this message")
    
if __name__ == '__main__':
    tech = ""
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hn:a:", ["help", "blif=", "amp", "buf"])
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
            
    print(f'Amplify the nets of {fname} with a threshold of {th} with {tech}')
    amplify(cell,tech,int(th))
    AF.saveCell(cell,CRL.Catalog.State.Logical)
