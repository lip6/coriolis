from DTR import *
from decimal import Decimal

techno = Techno.readFromFile("./example.dtr.xml")

print "+-----------------------------+"
print "| technology:      "+techno.get)   +  "    |"
print "| units:           "+techno.getUnit()   +"      |"
print "| version:         "+techno.getVersion()+"      |"
print "+-----------------------------+\n\n"

print "transistorMinL                = %s"%techno.getValue("transistorMinL")
print "transistorMinW                = %s"%Decimal(techno.getValueAsString("transistorMinW"))
print "minWidth of metal1            = %s"%techno.getValue("minWidth", "metal1")
print "minSpacing of metal1          = %s"%techno.getValue("minWidth", "metal1")
print "minSpacing of active vs poly  = %s"%techno.getValue("minSpacing", "active", "poly")
print "minExtension active over poly = %s"%techno.getValue("minExtension", "poly", "active")
print "minArea of metal1             = %s"%techno.getValue("minArea", "metal1")

# an example of why it is important to use Decimal in python:
print techno.getValue("minArea", "metal1")*3-0.3 # returns 5.55111512313e-17
print Decimal(techno.getValueAsString("minArea", "metal1"))*3-Decimal('0.3') # returns 0.000
