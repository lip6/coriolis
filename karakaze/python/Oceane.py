
import re


class Parameters ( object ):

    class Transistor ( object ):

        def __init__ ( self, aName ):
            name = aName
            L    = 0.0
            W    = 0.0
            M    = 0
            return


    def __init__ ( self ):
        self.transistors = { }
        self.indexToName = { }
        return


    def addTransistor ( self, name, index ):
        lname = name.lower()
        if self.transistors.has_key(lname):
          print 'Duplicated transistor "%s" (ignored).' % lname
        else:
          if self.indexToName.has_key(index):
            print 'Transistors "%s" and "%s" have the same index %d (ignored).' % \
                (self.indexToName[index],lname,index)
          self.transistors[ lname ] = Parameters.Transistor( lname )
          self.indexToName[ index ] = lname
          
        return self.transistors[ lname ]


    def getTransistor ( self, ref ):
        transistor = None
        if isinstance(ref,str):
          lname = ref.lower()
          if self.transistors.has_key(lname):
            transistor = self.transistors[lname]
          else:
            print 'No transistor named "%s".' % ref
        if isinstance(ref,int):
          if self.indexToName.has_key(ref):
            transistor = self.transistors[self.indexToName[ref]]
          else:
            print 'No transistor with index %d.' % ref
        return transistor

   #def getTransistorL ( self, ref ): return self.getTransistor(ref)[0]
   #def getTransistorW ( self, ref ): return self.getTransistor(ref)[1]
   #def getTransistorM ( self, ref ): return self.getTransistor(ref)[2]


    def read ( self, file ):
        reSpecTran = re.compile( r'^\* SPECIFICATIONS DE M(?P<index>\d+)\s+:\s+(?P<name>\w+) \*$' )
        reSpecL    = re.compile( r'L_(?P<index>\d+)\s+(?P<float>[0-9.e-]+)' )
        reSpecW    = re.compile( r'W_(?P<index>\d+)\s+(?P<float>[0-9.e-]+)' )
        reSpecM    = re.compile( r'M_(?P<index>\d+)\s+(?P<int>\d+)' )
        fd = open( file, 'r' )
        for line in fd.readlines():
          
          m = reSpecTran.match( line[:-1] )
          if m:
            i = int(m.group('index'))
           #print 'Found transistor %d:<%s>' % (i, m.group('name'))
            self.addTransistor( m.group('name'), i )
            continue

          m = reSpecL.match( line[:-1] )
          if m:
            i = int  (m.group('index'))
            L = float(m.group('float'))
           #print '  %d:L: %g' % (int(m.group('index')), L)
            self.getTransistor(i).L = L
            continue

          m = reSpecW.match( line[:-1] )
          if m:
            i = int  (m.group('index'))
            W = float(m.group('float'))
           #print '  %d:W: %g' % (int(m.group('index')), W)
            self.getTransistor(i).W = W
            continue

          m = reSpecM.match( line[:-1] )
          if m:
            i = int(m.group('index'))
            M = int(m.group('int'  ))
           #print '  %d:M: %d' % (int(m.group('index')), M)
            self.getTransistor(i).M = M
            self.getTransistor(i).W = M * self.getTransistor(i).W

        fd.close()
