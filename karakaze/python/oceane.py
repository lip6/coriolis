
import re


class Parameters ( object ):

    class Transistor ( object ):

        def __init__ ( self, aName ):
            self.name = aName
            self.L    = 0.0
            self.W    = 0.0
            self.M    = 0
            return

    class Capacitor ( object ):

        def __init__ ( self, aName ):
            self.name = aName
            self.C    = 0.0
            return

    class Resistor ( object ):

        def __init__ ( self, aName ):
            self.name = aName
            self.R    = 0.0
            return


    def __init__ ( self ):
        self.transistors = { }
        self.capacitors  = { }
        self.resistors   = { }
        self.indexToName = { }
        return


    def addTransistor ( self, name, index ):
        lname = name.lower()
        if lname in self.transistors:
          print( 'Duplicated transistor "%s" (ignored).' % lname )
        else:
          if index in self.indexToName:
            print( 'Transistors "%s" and "%s" have the same index %d (ignored).' % \
                (self.indexToName[index],lname,index))
          self.transistors[ lname ] = Parameters.Transistor( lname )
          self.indexToName[ index ] = lname
          
        return self.transistors[ lname ]


    def getTransistor ( self, ref ):
        transistor = None
        if isinstance(ref,str):
          lname = ref.lower()
          if lname in self.transistors:
            transistor = self.transistors[lname]
          else:
            print( 'No transistor named "%s".' % ref )
        if isinstance(ref,int):
          if ref in self.indexToName:
            transistor = self.transistors[self.indexToName[ref]]
          else:
            print( 'No transistor with index %d.' % ref )
        return transistor

   #def getTransistorL ( self, ref ): return self.getTransistor(ref)[0]
   #def getTransistorW ( self, ref ): return self.getTransistor(ref)[1]
   #def getTransistorM ( self, ref ): return self.getTransistor(ref)[2]


    def addCapacitor ( self, name, value ):
        lname = name.lower()
        if lname in self.capacitors:
          print( 'Duplicated capacitor "%s" (ignored).' % lname )
        else:
         #print( 'Add capacitor "%s"' % lname )
          self.capacitors[ lname ]   = Parameters.Capacitor( lname )
          self.capacitors[ lname ].C = value
          
        return self.capacitors[ lname ]


    def getCapacitor ( self, ref ):
        capacitor = None
        lname     = ref.lower()
        if lname in self.capacitors:
          capacitor = self.capacitors[lname]
        else:
          print( 'No capacitor named "%s".' % ref )
        return capacitor


    def addResistor ( self, name, value ):
        lname = name.lower()
        if lname in self.resistors:
          print( 'Duplicated resistor "%s" (ignored).' % lname )
        else:
          self.resistors[ lname ]   = Parameters.Resistor( lname )
          self.resistors[ lname ].R = value
          
        return self.resistors[ lname ]


    def getResistor ( self, ref ):
        resistor = None
        lname     = ref.lower()
        if lname in self.capactors:
          resistor = self.capactors[lname]
        else:
          print( 'No resistor named "%s".' % ref )
        return resistor


    def read ( self, file ):
        reSpecCapa  = re.compile( r'^(?P<name>C\w+)\s+(?P<value>.*)$' )
        reSpecResis = re.compile( r'^(?P<name>R\w+)\s+(?P<value>.*)$' )
        reSpecTran  = re.compile( r'^\* SPECIFICATIONS DE M(?P<index>\d+)\s+:\s+(?P<name>\w+) \*$' )
        reSpecL     = re.compile( r'L_(?P<index>\d+)\s+(?P<float>[0-9.e-]+)' )
        reSpecW     = re.compile( r'W_(?P<index>\d+)\s+(?P<float>[0-9.e-]+)' )
        reSpecM     = re.compile( r'M_(?P<index>\d+)\s+(?P<int>\d+)' )
        fd = open( file, 'r' )
        for line in fd.readlines():
          if line.startswith('REGIME_'): continue
          
          m = reSpecTran.match( line[:-1] )
          if m:
            i = int(m.group('index'))
           #print( 'Found transistor %d:<%s>' % (i, m.group('name')) )
            self.addTransistor( m.group('name'), i )
            continue

          m = reSpecL.match( line[:-1] )
          if m:
            i = int  (m.group('index'))
            L = float(m.group('float'))
           #print( '  %d:L: %g' % (int(m.group('index')), L) )
            self.getTransistor(i).L = L
            continue

          m = reSpecW.match( line[:-1] )
          if m:
            i = int  (m.group('index'))
            W = float(m.group('float'))
           #print( '  %d:W: %g' % (int(m.group('index')), W) )
            self.getTransistor(i).W = W
            continue

          m = reSpecM.match( line[:-1] )
          if m:
            i = int(m.group('index'))
            M = int(m.group('int'  ))
           #print( '  %d:M: %d' % (int(m.group('index')), M) )
            self.getTransistor(i).M = M
            self.getTransistor(i).W = M * self.getTransistor(i).W

          m = reSpecCapa.match( line[:-1] )
          if m:
            self.addCapacitor( m.group('name'), float(m.group('value')) )

          m = reSpecResis.match( line[:-1] )
          if m:
            self.addResistor( m.group('name'), float(m.group('value')) )

        fd.close()
