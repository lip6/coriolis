import sys

from OPENCHAMS import *

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
      print " | | +", instance.name, ":", instance.model, instance.mosType, instance.sourceBulkConnected
    else:
      print " | | +", instance.name, ":", instance.model
    print " | | | + connectors"
    for conn in instance.getConnectors():
      print " | | | |", conn.key, ":", conn.value.name
    if isinstance(instance, Device): 
      print " | | | + transistors"
      for tr in instance.getTransistors():
        print " | | | | name:", tr.name, "- gate:", tr.gate, "- source:", tr.source, "- drain:", tr.drain, "- bulk:", tr.bulk
  #  nets
  print " | + nets"
  for net in circuit.netlist.getNets():
    print " | | +", net.name, ":", net.type, net.external
    print " | | | + connections"
    for conn in net.getConnections():
      print " | | | | %s.%s"%(conn.instanceName, conn.connectorName)
  # schematic
  if (circuit.schematic):
    print " + schematic - zoom:", circuit.schematic.zoom
    for instance in circuit.schematic.getInstances():
      print " | name:", instance.key, "- x:", instance.value.x, "- y:", instance.value.y, "- symmetry:", instance.value.symmetry 
  # sizing
  if (circuit.sizing):
    print " + sizing"
    for op in circuit.sizing.getOperators():
      print " | + instance name:", op.key, "- operator:", op.value.name, "- simulModel:", op.value.simulModel, "- callOrder:", op.value.callOrder
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

