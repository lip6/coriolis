import getopt

from coriolis.designflow.technos import setupSky130_c4m
from coriolis import CRL
from coriolis import Hurricane

setupSky130_c4m( '../../..', '../../../pdkmaster/C4M.Sky130' )

# Buffer's cell in the library
# TODO: get it in the library
AF = CRL.AllianceFramework.get()
BUF = AF.getCell('buf_x2', CRL.Catalog.State.Views)
Hurricane.UpdateSession.open()

# Arrange the cells's nets in a dictonary where:
# - keys are the number of cells drived by nets
# - values are lists of nets which have the key drived cells
# parameter: the cell to analyze
# return: the corresponding dictionary
def net_by_load(cell):
    #lnets = list(cell.getNets())
    #lnets.sort(key=lambda net: size(net.getPlugs()), reverse=True)
    #for net in lnets:
    #    print("net: %s (%d)" %(net.getName(), size(net.getPlugs())))
    sorted_dict = {}
    for net in cell.getNets():
        try:
            sorted_dict[size(net.getPlugs())].append(net)
        except KeyError:
            sorted_dict[size(net.getPlugs())] = [net]
    return sorted_dict

# TODO: move this function to a common place as it is usefull for lots of operations
# Return the number of elements (size) of a given collection 
def size(collection):
    total = 0
    for c in collection:
        total += 1
    return total

# TODO: see if this function is not already written
# Return if a net is a clock or not based on its name
def isClock(net):
    return net.getName() in ['ck', 'CK', 'clk', 'CLK']

# Amplify a net by:
# - adding a buffer (tech == 'buf')
# - or amplifying the previous cell (tech == 'amp')
# parameters:
# - net is the net to amplify
# - cell is the cell in which the net is defined (TODO: try to use MasterCell)
# - tech is the technic used (buffer or cell amplification)
# raise ValueError if the technic is not defined
def amplify_net(net, cell, tech):
    # find the source plug
    for p in net.getPlugs():
        if p.getMasterNet().getDirection() == Hurricane.Net.Direction.OUT:
            p_found = p
            break
    # modify the source plug net
    if tech == 'buf':
        # Add a buffer
        p_found.setNet(bufferize(net,cell))
    elif tech == 'amp':
        # Amplify the source cell
        amplify_source(p_found)
    else:
        raise ValueError(f'Unknown technic {tech}')

# Amplify the cell of a given plug
def amplify_source(plug):
    inst = plug.getInstance()
    newModel = find_bigger_cell(inst.getMasterCell())
    inst.setMasterCell(newModel)

# Return the biggest (in term of load) cell equivalent of a given cell
# TODO: try to access cells by technology file
# TODO: support the other load for cells
def find_bigger_cell(model):
    equivalence = {
        'a2_x2': ['a2_x4'],
        'a3_x2': ['a3_x4'],
        'a4_x2': ['a4_x4'],
        'an12_x1': ['an12_x4'],
        #'ao22_x2': ['ao22_x4'],
        'ao22_x2': ['a3_x2'], # POUR C4M sky essais
        'ao2o22_x2': ['ao2o22_x4'],
        'buf_x2': ['buf_x4', 'buf_x8'],
        'inv_x1': ['inv_x2', 'inv_x4', 'inv_x8'],
        'mx2_x2': ['mx2_x4'],
        'mx3_x2': ['mx3_x4'],
        'na2_x1': ['na2_x4'],
        'na3_x1': ['na3_x4'],
        'na4_x1': ['na4_x4'],
        'nao22_x1': ['nao22_x4'],
        'nao2o22_x1': ['nao2o22_x4'],
        'nmx2_x1': ['nmx2_x4'],
        #'nmx3_x1':
        'nor2_x0': ['nxr2_x1'], # POUR C4M sky essais
        'no2_x1': ['no2_x4'],
        'no3_x1': ['no3_x4'],
        'no4_x1': ['no4_x4'],
        'noa22_x1': ['noa22_x4'],
        'noa2a22_x1': ['noa2a22_x4'],
        'noa2a2a23_x1': ['noa2a2a23_x4'],
        'noa2a2a2a24_x1': ['noa2a2a2a24_x4'],
        'noa2ao222_x1': ['noa2ao222_x4'],
        'noa3ao322_x1': ['noa3ao322_x4'],
        'nts_x1': ['nts_x2'],
        'nxr2_x1': ['nxr2_x4'],
        'o2_x2': ['o2_x4'],
        'o3_x2': ['o3_x4'],
        'o4_x2': ['o4_x4'],
        'oa22_x2': ['oa22_x4'],
        'oa2a22_x2': ['oa2a22_x4'],
        'oa2a2a23_x2': ['oa2a2a23_x4'],
        'oa2a2a2a24_x2': ['oa2a2a2a24_x4'],
        'oa2ao222_x2': ['oa2ao222_x4'],
        'oa3ao322_x2': ['oa3ao322_x4'],
        'on12_x1': ['on12_x4'],
        #'one_x0': 
        #'powmid_x0':
        #'rowend_x0':
        #'sff1_x4':
        #'sff1r_x4':
        #'sff2_x4':
        #'tie_x0':
        'ts_x4': ['ts_x8'],
        'xr2_x1': ['xr2_x4']
        #'zero_x0':
    }
    return AF.getCell(equivalence[model.getName()][0], CRL.Catalog.State.Views)

# Add a buffer to a given net to amplify it
# parameters:
# - net is the net to amplify
# - cell is the cell in which the net is defined (TODO: try to use MasterCell)
def bufferize(net, cell):
    # create the buffer instance
    # TODO: try to get the cell from the net (masterCell ?)
    buf_i = Hurricane.Instance.create(cell, f'buf_{net.getName()}', BUF)
    # insert this instance to the net, i.e. cut the net into net_b (source to be created) and net
    buf_i.getPlug(BUF.getNet('q')).setNet(net)
    net_b = Hurricane.Net.create(cell, "%s_b" %(net.getName()))
    buf_i.getPlug(BUF.getNet('i')).setNet(net_b)
    return net_b

# Amplify a given cell using the corresponding technic
# Parameters:
# - cell is the cell to amplify
# - tech is the technic used (buffer or cell amplification)
# - threshold: if net has a load > threshold then bufferize
# - nb_max: bufferize all the nets with nb_max maximal thresholds
def amplify(cell, tech, threshold=0, nb_max=0):
    lnets = net_by_load(cell)
    max_keys = sorted(lnets.keys(), reverse=True)
    for i in max_keys[0:nb_max]:
        if i >= threshold:
            for net in lnets[i]:
                if not isClock(net):
                    amplify_net(net,cell,tech)

def usage():
    print("python utilities.py [option]")
    print("Utility functions to amplify nets from a netlist")
    print("-n (blif): the name of the blif netlist")
    print("-a (tech:th:nb): the amplification technique used (buf to bufferize the nets and amp to amplify the cells)")
    print("                 - th is the threshold corresponding to the load of a signal (in number of targetting cells)")
    print("                 - nb is the maximal number of thresholds to treat") 
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
            tech,th,nb = a.split(':')
            
    print(f'Amplify the {nb} nets of {fname} with a threshold of {th} with {tech}')
    amplify(cell,tech,int(th),int(nb))
    AF.saveCell(cell,CRL.Catalog.State.Logical)
