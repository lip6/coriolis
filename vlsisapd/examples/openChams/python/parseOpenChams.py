import sys

from OPENCHAMS import *

def printHBTree(node, indent):
    if node == None:
        return
    for i in range(indent):
        print " |",
    if isinstance(node, Bloc):
        print " bloc:", node.getName(), "-", node.getPosition()
        printHBTree(node.top  , indent+1)
        printHBTree(node.right, indent+1)
        return
    if isinstance(node, Group):
        print " group:", node.getName(), "-", node.getPosition(), "-", node.align, "-", node.isolated, "-", node.paired
        printHBTree(node.rootNode, indent+1)
        printHBTree(node.top     , indent+1)
        printHBTree(node.right   , indent+1)
        return

def printContents(circuit):
  print circuit.name
  # circuit parameters
  print " + parameters"
  for param in circuit.parameters.getValues():
    print " | |", param.key, ":", param.value
  for param in circuit.parameters.getEqValues():
    print " | |", param.key, ":", param.value
  # netlist
  print " + netlist"
  #  instances
  print " | + instances"
  for instance in circuit.netlist.getInstances():
    if isinstance(instance, Device): 
      print " | | +", instance.name, ":", instance.model, instance.order, instance.mosType, instance.sourceBulkConnected
    else:
      print " | | +", instance.name, ":", instance.model, instance.order
    print " | | | + connectors"
    for conn in instance.getConnectors():
      print " | | | |", conn.key, ":", conn.value.name
    if isinstance(instance, Device): 
      print " | | | + transistors"
      for tr in instance.getTransistors():
        print " | | | | name:", tr.name, "- gate:", tr.gate, "- source:", tr.source, "- drain:", tr.drain, "- bulk:", tr.bulk
  #  nets
  print " | + nets"
  schematicNet = False
  for net in circuit.netlist.getNets():
    print " | | +", net.name, ":", net.type, net.external
    print " | | | + connections"
    for conn in net.getConnections():
      print " | | | | %s.%s"%(conn.instanceName, conn.connectorName)
    if not net.hasNoPorts() or not net.hasNoWires():
      schematicNet = True
  # schematic
  if (circuit.schematic):
    print " + schematic"
    for instance in circuit.schematic.getInstances():
      print " | + instance name:", instance.key, "- x:", instance.value.x, "- y:", instance.value.y, "- orientation:", instance.value.orientation 
    if schematicNet:
      for net in circuit.netlist.getNets():
          if net.hasNoPorts() and net.hasNoWires():
              continue
          print " | + net name:", net.name
          for port in net.getPorts():
              print " | | + port type:", port.type, "- idx:", port.index, "- x:", port.x, "- y:", port.y, "- orientation:", port.orientation
          for wire in net.getWires():
              if isinstance(wire.startPoint, InstancePoint):
                  print " | | + wire <" + wire.startPoint.name.getString() + "," + wire.startPoint.plug.getString() +">"
              elif isinstance(wire.startPoint, PortPoint):
                  print " | | + wire <" + str(wire.startPoint.index) + ">"
              else:
                  print " - - UNKNOWN START POINT"
              for point in wire.getIntermediatePoints():
                  print " | |        <" + str(point.x) + "," + str(point.y) + ">"
              if isinstance(wire.endPoint, InstancePoint):
                  print " | |        <" + wire.endPoint.name.getString() + "," + wire.endPoint.plug.getString() +">"
              elif isinstance(wire.endPoint, PortPoint):
                  print " | |        <" + str(wire.endPoint.index) + ">"
              else:
                  print " - - UNKNOWN END POINT"
  # sizing
  if (circuit.sizing):
    print " + sizing"
    for op in circuit.sizing.getOperators():
      print " | + instance name:", op.key, "- operator:", op.value.name, "- simulModel:", op.value.simulModel
      for constraint in op.value.getConstraints():
        print " | | + param:", constraint.key, "- ref:", constraint.value.ref, "- refParam:", constraint.value.refParam, "- factor:", constraint.value.factor
    print " | + equations"
    for eq in circuit.sizing.getEquations():
      print " | |", eq.key, ":", eq.value
  # layout
  if (circuit.layout):
    print " + layout"
    for inst in circuit.layout.getInstances():
      print " | | instance name:", inst.key, "- style:", inst.value
    if circuit.layout.hbTreeRoot != None:
      print " | + hbtree"
      printHBTree(circuit.layout.hbTreeRoot, 2)

def usage():
    print "usage:", sys.argv[0], "[filename]"
    sys.exit(48)

def main():
  if len(sys.argv) == 1:
    filename = "./inverter.xml"
  elif len(sys.argv) == 2:
    filename = sys.argv[1]
  else:
    usage()

  circuit = Circuit.readFromFile(filename)
  printContents(circuit)


if __name__ == "__main__":
  main()

