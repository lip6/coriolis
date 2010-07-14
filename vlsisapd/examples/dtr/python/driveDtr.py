from DTR import *

techno = Techno(Name("myTech"), Name("micro"))

techno.addRule (Name("transistorMinL"), 0.1 , Name("ref1"))
techno.addRule (Name("transistorMinW"), 0.2 , Name("ref2"))
techno.addRule (Name("minWidth")      , 0.15, Name("ref3"), Name("metal1"))
techno.addRule (Name("minSpacing")    , 0.2 , Name("ref4"), Name("metal1"))
techno.addRule (Name("minSpacing")    , 0.1 , Name("ref5"), Name("active"), Name("poly"))
techno.addARule(Name("minExtension")  , 0.2 , Name("ref6"), Name("poly"), Name("active"))

rule = techno.addRule(Name("minArea"), 0.1, Name("ref7"), Name("metal1"))
rule.setType(Name("area"))

techno.writeToFile("./out.dtr.xml")
