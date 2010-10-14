from LIBERTY import *

library = Library.readFromFile("./testParse.lib")

if ( library ) :
    # print of the library
    library.pprint()
    # print of one attribute in particular
    print "| area of inv_x1= ", library.getCell("inv_x1").getAttribute("area").valueAsString()
    print "| timing intrinsic_rise of nq of inv_x1= ",  library.getCell("inv_x1").getPin("nq").getTiming("i").getAttribute("intrinsic_rise").valueAsString()
else :
    raise ( "library is NULL" )

