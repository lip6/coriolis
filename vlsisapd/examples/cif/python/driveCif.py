import pyCIF
circuit = pyCIF.Circuit("Transistor", "micro", 0.001)
poly1 = pyCIF.Polygon(6)
poly1.addPoint(130, 290)
poly1.addPoint(540, 290)
poly1.addPoint(540, 690)
poly1.addPoint(130, 690)
circuit.addPolygon(poly1)
    
poly2 = pyCIF.Polygon(17)
poly2.addPoint(305, 150);
poly2.addPoint(365, 150);
poly2.addPoint(365, 830);
poly2.addPoint(305, 830);
circuit.addPolygon(poly2)

circuit.writeToFile("./transistor.cif")
