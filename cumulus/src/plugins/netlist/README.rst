# 1 Introduction
This tutorial introduces all the notions needed to manipulate netlist throw |Coriolis| with Python. It can be useful for optimization technics which need to automatically modify the netlist.

You can refer to the Hurricane+python tutorial for more advance concepts with the |Hurricane| database

# 2 Generalities
[[example.svg]]
![[example.svg]]
As shown with Figure |example.svg|, a design in Coriolis is refered as a *cell*. It is composed with *instances* (dot lines), *nets* (in blue) and *plugs* (black squares). Each instance refers to a *model* (`MasterCell`) which, in this example, are `OR2` and `AND2`.  A model is itself a cell with instances, nets and plugs. Each component of a design has a name which can be obtained by using the method `getName()`. Plugs, nets and instances are collections which can be obtained by the methods `getPlugs()`, `getNets()`, `getInstances()` respectively.

# 3 Using Coriolis
## 3.1 Setting up the environment
Coriolis can be used throw Python3. First of all, the adequate environment has to be setup with the following command:
`crlenv.py -- shell`
Where `shell` is the shell used by the user (bash, zsh...):
## 3.2 Importing the modules
To use Coriolis with Python, including the Coriolis's modules is mandatory:
`from coriolis import CRL`
`Hurricane` is the database on which Coriolis is build and `CRL` is the main part of Coriolis.

To make a design, we need to used a dedicated techno. The configuration of a design flow for a dedicated techno is done in the Python module `coriolis.designflow.technos` in which we need to import the techno. As example for SkyWater 130nm the import is:
`from coriolis.designflow.technos import setupSky130_c4m 
And after we use the `setupSky130_c4m` python function to run the configuration:
`setupSky130_c4m( '../../..', '../../../pdkmaster/C4M.Sky130' )`
### 3.3 Beginning the session
All the flow in Coriolis is running in a Coriolis session which has to be open:
`af = AllianceFramework.get()`
`UpdateSession.open()`

# 4 Netlist manipulation
First of all the netlist (in blif format) have to be loaded with the following method:
`cell = CRL.Blif.load("example")`

## 4.1 Navigation
To navigate through the different components of the netlist (`cell`), the different following instructions can be used:
- for nets: `for net in cell.getNets():`
- for instances: `for inst in cell.getInstances():`
- for plugs: `for plug in cell.getPlugs():`

## 4.2 Enter the hierarchy
Each instance can also be considered as a cell and follow the same navigation capacities. For that, it is necessary to refer to the model of is instance and access it by using the method `getMasterCell()` on the instance. For example, considering Figure |example.svg|, accessing to the nets and instances inside `inst1` can be done by the following code:
`cell1 = inst1.getMasterCell()`
`for net in cell1.getNets():`

## 4.3 Components' properties
Each component in the Hurricane database, is a class and has specific properties. The Hurricane's documentation gives all the details.

### 4.3.1 Net
Nets have `Direction` and `Type` which are both Hurricane's classes. A direction can be `IN`, `OUT`, `TRISTATE` etc. A type can be `LOGICAL`, `CLOCK` etc. Lot of method give access to the properties (`getName()`, `getDirection()`, etc.), test them (`isLogical()`, `isClock()`, etc.) or set them (`setType()`, `setExternal()`, etc.)

### 4.3.2 Instance
The main methods of this class concerning netlist manipulation are `getMasterCell()` and `getPlugs()`

### 4.3.3 Plug
The method `getInstance()` allows to get the instance of the plug and `getMasterNet()` to get the net connected to this plug.

## 4.4 Examples

WARNING Hurricane collections are not mutable, so change values outside iterator !!
=> the next sections have to be modify in consequence
### 4.4.1 Changing a cell
For example a cell from which one output drives more than `MAX` cells can be amplified, that mean change a `_x2` cell by a `_x4` one (i.e: `a2_x2` is changed by `a2_x4` which is amplified by 2):

    for net in cell.getNets():
        if size(net.getPlugs()) > MAX:
            for plug in net.getPlugs():
                if plug.getMasterNet().getDirection() == Net.Direction.OUT:
                    inst = plug.getInstance()
                    # replace _x1 or_x2 by _x4 in cell's name
                    cell_name = inst.getMasterCell().getName()[:-1]
                    cell_name += '4'
                    # get the corresponding cell (_x4)
                    new_cell = af.getCell(cell_name, Catalog.State.Views)
                    # change the cell
                    inst.setMasterCell(new_cell)

### 4.4.2 Adding a cell
For example, a buffer can be inserted to amplify a net which drives more than `MAX` cells:

```
for net in cell.getNets():
	if size(net.getPlugs()) > MAX:
		# get the buffer
		buf = af.getCell('buf_x2', Catalog.State.Views)
		# create its corresponding instance
		buf_i = Instance.create(cell, 'buf_%s' %(net.getName()), buf)
		# insert this instance to the net, i.e. cut the net into net and net_b (to be created)
		buf_i.getPlug(buf.getNet('i')).setNet(net)
		net_b = Net.create(cell, "%s_b" %(net.getName()))
		buf_i.getPlug(buf.getNet('q')).setNet(net_b)
```

# 5 A complete example
The following code is a concrete example of all the things described in the previous sections. It proposes 2 fonctions that can be applied to a given circuit:
`- amplify(nb)` that changes the cells that drive more than `nb` cells 
`- bufferize(nb)` that bufferizes the nets that drive more than `nb` cells




