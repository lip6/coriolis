from LIBERTY import *

library = Library("test")

# Attributes
library.addAttribute("default_inout_pin_cap",  Attribute.Type.Double, "0.011")
library.addAttribute("default_wire_load_mode", Attribute.Type.String, "enclosed")
library.addAttribute("time_unit",              Attribute.Type.Unit,   "1", "ns")
library.addAttribute("capacitive_load_unit",   Attribute.Type.Unit,   "1", "pf")

# WireLoads
library.addWireLoad("medium")
wireLoad = library.getWireLoad("medium")
wireLoad.addAttribute("slope",         Attribute.Type.Double, "200")
wireLoad.addAttribute("fanout_length", Attribute.Type.Pair,   "1", "200")

# WireLoadSelection
library.addWireLoadSelection("medium")
wireLoadSelection = library.getWireLoadSelection()
wireLoadSelection.addWireLoadArea(0, 500, "small")

# Cells
library.addCell("inv")
cell = library.getCell("inv")
cell.addAttribute("area",           Attribute.Type.Double, "1")
cell.addAttribute("cell_footprint", Attribute.Type.String, "inv")

# Pins
cell.addPin("e")
pin = cell.getPin("e")
pin.addAttribute("direction",   Attribute.Type.String, "input")
pin.addAttribute("capacitance", Attribute.Type.Double, "0.008")

cell.addPin("s")
pin = cell.getPin("s")
pin.addAttribute("direction", Attribute.Type.String, "output")
pin.addAttribute("function",  Attribute.Type.String, "i'")

# Timing
pin.addTiming()
timing = pin.getTimings()[-1]
timing.addAttribute("timing_sense",   Attribute.Type.String, "negative_unate")
timing.addAttribute("related_pin",    Attribute.Type.String, "e")
timing.addAttribute("intrinsic_rise", Attribute.Type.Double, "0.101")

# Write
library.writeToFile("testDrive.lib")
