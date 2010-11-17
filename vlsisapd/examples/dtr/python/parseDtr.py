from DTR import *
from decimal import Decimal

techno = Techno.readFromFile("./example.dtr.xml")

print "+-----------------------------+"
print "| technology:      "+techno.getName().getString()+"    |"
print "| units:           "+techno.getUnit().getString()+"      |"
print "| version:         "+techno.getVersion().getString()+"      |"
print "+-----------------------------+\n\n"

print "transistorMinL                = %s"%techno.getValue(Name("transistorMinL"))
print "transistorMinW                = %s"%Decimal(techno.getValueAsString(Name("transistorMinW")))
print "minWidth of metal1            = %s"%techno.getValue(Name("minWidth"), Name("metal1"))
print "minSpacing of metal1          = %s"%techno.getValue(Name("minWidth"), Name("metal1"))
print "minSpacing of active vs poly  = %s"%techno.getValue(Name("minSpacing"), Name("active"), Name("poly"))
print "minExtension active over poly = %s"%techno.getValue(Name("minExtension"), Name("poly"), Name("active"))
print "minArea of metal1             = %s"%techno.getValue(Name("minArea"), Name("metal1"))

# an example of why it is important to use Decimal in python:
print techno.getValue(Name("minArea"), Name("metal1"))*3-0.3 # returns 5.55111512313e-17
print Decimal(techno.getValueAsString(Name("minArea"), Name("metal1")))*3-Decimal('0.3') # returns 0.000
