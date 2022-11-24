import AGDS
lib = AGDS.Library("myTestLib")
lib.setUserUnits(0.001)
lib.setPhysUnits(1.0e-9)

active = AGDS.Rectangle( 6, 120, 290, 540, 690) # layer  6 corresponds to active
poly   = AGDS.Rectangle(17, 305, 150, 365, 830) # layer 17 corresponds to polysilicium

str = AGDS.Structure("Transistor")
str.addElement(active)
str.addElement(poly)

lib.addStructure(str)
lib.writeToFile("./transistor.agds")
