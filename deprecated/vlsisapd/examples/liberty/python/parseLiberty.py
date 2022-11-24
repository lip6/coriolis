from LIBERTY import *

library = Library.readFromFile("./testParse.lib")

if ( library ) :
    # print of the library
    print library
    # print of one attribute in particular of a cell
    print "Area of inv_x1 :", library.getCell("inv_x1").getAttribute("area").valueAsString()
    print "Timing intrinsic_rise of nq of inv_x1 :",  library.getCell("inv_x1").getPin("nq").getTiming("i").getAttribute("intrinsic_rise").valueAsString()
    # print of all the attributes of a cell
    print "Attributes of no2_x1 :"
    for attr in library.getCell("no2_x1").getAttributes() :
        print "  name=",  attr.key, \
              ", type=",  attr.value.typeToString(), \
              ", value=", attr.value.valueAsString()
    # print of all the timings of a pin
    print "Timing's attributes of pin nq of no2_x1 :"
    for timing in library.getCell("no2_x1").getPin("nq").getTimings() :
        print timing
else :
    raise ( "library is NULL" )

