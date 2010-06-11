import pyAGDS
lib = pyAGDS.Library("myTestLib")
lib.setUserUnits(0.001)
lib.setPhysUnits(1.0e-9)

active = pyAGDS.Rectangle( 6, 120, 290, 540, 690) # layer  6 corresponds to active
poly   = pyAGDS.Rectangle(17, 305, 150, 365, 830) # layer 17 corresponds to polysilicium

str = pyAGDS.Structure("Transistor")
str.addElement(active)
str.addElement(poly)

lib.addStructure(str)
lib.writeToFile("./transistor.agds")
