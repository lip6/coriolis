import sys, getopt

import coriolis.technos.symbolic.cmos 
from coriolis.Hurricane import *
from coriolis.CRL import *

af = AllianceFramework.get()
UpdateSession.open()

# transform a given netlist to its corresponding adjacence matrix where:
#   - cell is the cell corresponding to the netlist
#   - tf is a boolean indicating the adjacence matrix's direction, if True (default)
#      + lines are the target (to)
#      + and columns the source (from)
#     the opposite if False
# return:
#   - the list of the nets with their index in the adjacence matrix
#   - the adjacence matrix
def cell2adj(cell, tf = True):
    dnets = {}
    mat =[]
    n = 0
    # Matrix initialization
    for n1 in cell.getNets():
        if n1.getType() == Net.Type.LOGICAL:
            mat.append([])
            dnets[n1.getName()] = [n,"Input"]
            n += 1
            for n2 in cell.getNets():
                if n2.getType() == Net.Type.LOGICAL:
                    mat[-1].append(0)
    # adjacence's matrix filling
    for ins in cell.getInstances():
        target = []
        source = []
        for plug in ins.getPlugs():
            net = plug.getMasterNet()
            if net.getType() == Net.Type.LOGICAL:
                if net.getDirection() == Net.Direction.IN:
                    source.append(dnets[plug.getNet().getName()][0])
                elif net.getDirection() == Net.Direction.OUT:
                    target.append(dnets[plug.getNet().getName()][0])
                    dnets[plug.getNet().getName()][1] = ins.getMasterCell().getName()
        for i in range(len(source)):
            for j in range(len(target)):
                if tf:
                    mat[target[j]][source[i]] = 1
                else:
                    mat[source[i]][target[j]] = 1
    #print(dnets)
    #for i in range(len(dnets)):
    #    print(mat[i])
    return (dnets,mat)

# get the adjassors' list of each net in the adjacence matrix mat:
#   - if the adjacence matrix is to_from (tf = True) it is a predecessors list
#   - if the adjacence matrix is from_to (tf = False) it is a successors list
# return a tuple with:
#   - the index of the net in the adjacence matrix
#   - the list of the corresponding adjassors
def adjassors(mat):
    res = []
    l = -1
    for line in mat:
        l += 1
        res.append((l,[]))
        for i in range(len(line)):
            if line[i] == 1:
                res[-1][1].append(i)
        if res[-1][1] == []:
            res.pop()
    #for i in range(len(res)):
    #    print(res[i])
    return res

# write the result in a json file where:
#   - fname is the name of the output file
#   - nets is the list of the nets and their index
#   - mat is the adjacence matrix
#   - adj is the list of adjassors
def write_json(fname, nets, mat, adj):
    fres = open(fname+'.json', 'wt')
    fres.write("{\n")
    fres.write('   "Nets" : ' + str(nets))
    #fres.write(',\n   "Type" : ' + str(ltypes))
    fres.write(',\n   "Mat"  : ' + str(mat))
    fres.write(',\n   "Adj" : ' + str(adj))
    fres.write('\n}')
    fres.close()

def usage():
    print("python blif2graph.py [option]")
    print("Gaphs' utilities on a netlist")
    print("-b (blif): the name of the blif netlist")
    print("-p (pred): list of predecessors of each net")
    print("-s (suc): list of sucessors of each net")
    print("-h (help): this message")

def main(argv):
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hb:ps", ["help", "blif=", "pred", "suc"])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    for o, a in opts:
        if o == '-b':
            cell = Blif.load(a)
            fname = a
        if o == '-p':
            tf = True
        if o == "-s":
            tf = False
        if o == "-h":
            usage()
    (nets,mat) = cell2adj(cell,tf)
    adj = adjassors(mat)
    write_json(fname,nets,mat,adj)

if __name__ == "__main__":
    main(sys.argv)
