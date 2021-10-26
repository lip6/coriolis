
# This file is part of the Coriolis Project.
# Copyright (C) Laboratoire LIP6 - Departement ASIM
# Universite Pierre et Marie Curie
#
# Main contributors :
#        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
#        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
#        Hugo Clement                   <Hugo.Clement@lip6.fr>
#        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
#        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
#        Christian Masson           <Christian.Masson@lip6.fr>
#        Marek Sroka                     <Marek.Sroka@lip6.fr>
# 
# The  Coriolis Project  is  free software;  you  can redistribute  it
# and/or modify it under the  terms of the GNU General Public License
# as published by  the Free Software Foundation; either  version 2 of
# the License, or (at your option) any later version.
# 
# The  Coriolis Project is  distributed in  the hope  that it  will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
# GNU General Public License for more details.
# 
# You should have  received a copy of the  GNU General Public License
# along with the Coriolis Project;  if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA
#
# License-Tag
# Authors-Tag
# ===================================================================
#
# x-----------------------------------------------------------------x 
# |                                                                 |
# |                   C O R I O L I S                               |
# |        S t r a t u s   -  Netlists/Layouts Description          |
# |                                                                 |
# |  Author      : Roselyne AVOT-CHOTIN - Sophie BELLOEIL           |
# |  E-mail      :           Sophie.Belloeil@asim.lip6.fr           |
# | =============================================================== |
# |  Py Module   :       "./st_slansky.py"                          |
# | *************************************************************** |
# |  U p d a t e s                                                  |
# |                                                                 |
# x-----------------------------------------------------------------x



from stratus import *

class Sub ( Model ) :
    
   def __init__(self, name, param):
      Model.__init__(self, name, param)
      if 'nbit' in param:
         self.nbit  = param['nbit']
         self.nbit0 = self.nbit
         self.nbit1 = self.nbit
      else:
         self.nbit0 = param['nbit0']
         self.nbit1 = param['nbit1']
         self.nbit  = max(self.nbit0,self.nbit1)

      if 'signed' not in param : self._signed = True
      else                     : self._signed = param['signed']

      if 'extended' not in param : self._extended = False
      else                     : self._extended = param['extended']
    
      if (self.nbit < 2): raise Exception ( "\n[Stratus ERROR] slansky : input arity should be greater than 1.\n" )

   def Interface(self):
      self.i0 = SignalIn("i0", self.nbit0)
      self.i1 = SignalIn("i1", self.nbit1)
      if self._extended : self.o = SignalOut("o", self.nbit+1)
      else : self.o = SignalOut("o", self.nbit)

      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")

   def Netlist(self):
      # extand the inputs to have the same width
      if (self.nbit0 < self.nbit) : 
        i0e = Signal("i0e", self.nbit)
        if self._signed : i0e <= self.i0.Extend(self.nbit,'signed')
        else : i0e <= self.i0.Extend(self.nbit,'zero')
      else:
        i0e = self.i0
      if (self.nbit1 < self.nbit) : 
        i1e = Signal("i1e", self.nbit)
        if self._signed : i1e <= self.i1.Extend(self.nbit,'signed')
        else : i1e <= self.i1.Extend(self.nbit,'zero')
      else:
        i1e = self.i1

      Generate ( "Slansky", "slansky_cin_%d" % ( self.nbit ), param={'nbit':self.nbit,'cin':True, 'extended':self._extended} )
      Inst ( "slansky_cin_%d" % ( self.nbit )
           , map = { 'i0' : i0e
                   , 'i1' : ~i1e
                   , 'cin' : One(1)
                   , 'o'  : self.o
                   , 'vdd' : self.vdd
                   , 'vss' : self.vss
                   }
           )

   def GetParam ( cls ):

      return {'nbit' : 'integer', 'nbit0' : 'integer', 'nbit1' : 'integer'}

   GetParam = classmethod ( GetParam )


class Slansky ( Model ) :

   def __init__(self, name, param):
      Model.__init__(self, name, param)
      if 'nbit' in param:
         self.nbit  = param['nbit']
         self.nbit0 = self.nbit
         self.nbit1 = self.nbit
      else:
         self.nbit0 = param['nbit0']
         self.nbit1 = param['nbit1']
         self.nbit  = max(self.nbit0,self.nbit1)

      if 'signed' not in param : self._signed = True
      else                     : self._signed = param['signed']

      if 'extended' not in param : self._extended = False
      else                     : self._extended = param['extended']
    
      if 'cin' not in param  : self._cin = False
      else                   : self._cin = param['cin']

      if (self.nbit < 2): raise Exception ( "\n[Stratus ERROR] slansky : input arity should be greater than 1.\n" )

   def Interface(self):
      self.i0 = SignalIn("i0", self.nbit0)
      self.i1 = SignalIn("i1", self.nbit1)
      if self._cin : self.cin = SignalIn("cin", 1)
      if self._extended : self.o = SignalOut("o", self.nbit+1)
      else : self.o = SignalOut("o", self.nbit)

      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")

   def Netlist(self):
     # extand the inputs to have the same width
     if (self.nbit0 < self.nbit) : 
       i0e = Signal("i0e", self.nbit)
       if self._signed : i0e <= self.i0.Extend(self.nbit,'signed')
       else : i0e <= self.i0.Extend(self.nbit,'zero')
     else:
       i0e = self.i0
     if (self.nbit1 < self.nbit) : 
       i1e = Signal("i1e", self.nbit)
       if self._signed : i1e <= self.i1.Extend(self.nbit,'signed')
       else : i1e <= self.i1.Extend(self.nbit,'zero')
     else:
       i1e = self.i1
       
     # pigi preprocessing
     [p,g] = pigi(i0e,i1e)
    
     # Slansky algorithm
     [pp,gg] = pg_tree(p,g,'slansky')
     
     # postprocessing :
     if self._cin : self.o[0] <= p[0] ^ self.cin
     else         : self.o[0] <= p[0].Buffer()
     
     for i in range(1,self.nbit):
       if self._cin : self.o[i] <= p[i] ^ (gg[i-1] | (pp[i-1] & self.cin))
       else         : self.o[i] <= p[i] ^ gg[i-1]

     if self._extended:
       if self._signed:
         ovft = Signal("ovft",1)
         if self._cin:
           ovft <= (gg[self.nbit-1] | (pp[self.nbit-1] & self.cin)) ^ (gg[self.nbit-2] | (pp[self.nbit-2] & self.cin))
           # pb inout asimut remplacement de o[nbit-1] par sa valeur
           # self.o[self.nbit] <= (self.o[self.nbit-1] & ~ovf) | (cout & ovf)
           self.o[self.nbit] <= ((p[self.nbit-1] ^ (gg[self.nbit-2] | (pp[self.nbit-2] & self.cin))) & ~ovft) | (gg[self.nbit-1] & ovft)
         else:
           ovft <= gg[self.nbit-1] ^ gg[self.nbit-2]
           # pb inout asimut remplacement de o[nbit-1] par sa valeur
           # self.o[self.nbit] <= (self.o[self.nbit-1] & ~ovf) | (cout & ovf)
           self.o[self.nbit] <= ((p[self.nbit-1] ^ gg[self.nbit-2]) & ~ovft) | (gg[self.nbit-1] & ovft)
       else:
         if self._cin:
           self.o[self.nbit] <= gg[self.nbit-1] | (pp[self.nbit-1] & self.cin)
         else:
           self.o[self.nbit] <= gg[self.nbit-1].Buffer()

   def GetModelName ( cls, param ) :
     name = 'slansky'

     if 'nbit' in param:
       name += '_%d' % (param['nbit'])
     else:
       if param['nbit0'] != param['nbit1'] : name += '_%dx%d' % (param['nbit0'], param['nbit1'])
       else                                : name += '_%d'    % (param['nbit0'])

     if ('cin' in param) and (param['cin']) : name += '_cin'
     
     return name
   GetModelName = classmethod ( GetModelName )

   def GetParam ( cls ):

      return {'nbit' : 'integer', 'nbit0' : 'integer', 'nbit1' : 'integer', 'cin' : 'boolean'}

   GetParam = classmethod ( GetParam )

####################################################################################################
nb_pigi = 0
nb_pg_tree = 0

def pigi(a,b):
   global nb_pigi
   nb_pigi += 1
   pi = Signal("pi%d_a" %nb_pigi,a._arity)
   gi = Signal("gi%d_a" %nb_pigi,a._arity)
   Generate ( "st_slansky.PiGiBus", "pigibus%d" % a._arity, param = {'nbit' : a._arity} )
   Inst("pigibus%d" % a._arity,
      map = {
         'a' : a,
         'b' : b,
         'pi' : pi,
         'gi' : gi,
         'vdd' : a._st_cell._st_vdds[0],
         'vss' : a._st_cell._st_vsss[0]
      },
   )
   return [pi,gi]

class PiGiBus(Model):
   def __init__(self, name, param):
      Model.__init__(self, name, param)
      self.nbit = param['nbit']
      if self.nbit < 1:
         raise Exception ( 'PiGiBus : input arity should be greater than 0' )

   def Interface(self):
      self.a = SignalIn("a",self.nbit)
      self.b = SignalIn("b",self.nbit)
      self.pi = SignalOut("pi",self.nbit)
      self.gi = SignalOut("gi",self.nbit)

      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")

   def Netlist(self):
      for i in range (self.nbit):
         Generate ( "st_slansky.PiGi", "pigi" )
         Inst("pigi",
            map = {
               'a'  : self.a[i],
               'b'  : self.b[i],
               'pi' : self.pi[i],
               'gi' : self.gi[i],
               'vdd': self.vdd,
               'vss': self.vss
            }
         )

   def GetParam ( cls ):

      return {'nbit' : 'integer'}

   GetParam = classmethod ( GetParam )

         
class PiGi(Model):
   def Interface(self):
      self.a = SignalIn("a",1)
      self.b = SignalIn("b",1)
      self.pi = SignalOut("pi",1)
      self.gi = SignalOut("gi",1)
      
      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")

   def Netlist(self):
      self.pi <= self.a ^ self.b
      self.gi <= self.a & self.b

########################
def pg_tree(pi,gi,algo):
   global nb_pg_tree
   nb_pg_tree += 1
   p = Signal("p%d_a" %nb_pg_tree,pi._arity)
   g = Signal("g%d_a" %nb_pg_tree,pi._arity)
   Generate ( "st_slansky.PGTree", "pgtree%d%s" % ( pi._arity, algo ), param = {'nbit' : pi._arity, 'algo' : algo} )
   Inst("pgtree%d%s" % ( pi._arity, algo ),
      map = {
         'pi' : pi,
         'gi' : gi,
         'p' : p,
         'g' : g,
         'vdd' : pi._st_cell._st_vdds[0],
         'vss' : pi._st_cell._st_vsss[0]
      }
   )
   return [p,g]

class PGTree(Model):

   def __init__(self, name, param):
      Model.__init__(self, name, param)
      self.nbit = param['nbit']
      if self.nbit < 1:
         raise Exception ( 'input arity should be greater than 0' )
      self.algo = param['algo']
      if self.algo != 'slansky':
         raise Exception ( '%s algorithm is not yet implemented' %self.algo )
        
   def Interface(self):
      self.pi = SignalIn("pi",self.nbit)
      self.gi = SignalIn("gi",self.nbit)
      self.p = SignalOut("p",self.nbit)
      self.g = SignalOut("g",self.nbit)

      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")

   def Netlist(self):
      nb_stages = log2(self.nbit)
      matrix = prefixMatrix(self.algo,self.nbit)
      p = []
      g = []
      p.append(self.pi)
      g.append(self.gi)
      # group propagation
      Generate ( "st_slansky.PG", "pg" )
      for i in range(0, nb_stages):
         p.append(Signal("p%d" %(i+1), self.nbit))
         g.append(Signal("g%d" %(i+1), self.nbit))
         for j in range(0, self.nbit):
            if (j % 2**i) == 0:
               k = j-1
            if matrix[i][j] == 1:
               Inst("pg",
                  map = {
                     'p1'  : p[i][j],
                     'g1'  : g[i][j],
                     'p2'  : p[i][k],
                     'g2'  : g[i][k],
                     'p'   : p[i+1][j],
                     'g'   : g[i+1][j],
                     'vdd' : self.vdd,
                     'vss' : self.vss
                  }
               )
            else:
               p[i+1][j] <= p[i][j].Buffer()
               g[i+1][j] <= g[i][j].Buffer()
      self.p <= p[nb_stages].Buffer()
      self.g <= g[nb_stages].Buffer()

   def GetParam ( cls ):

      return {'nbit' : 'integer'}

   GetParam = classmethod ( GetParam )


class PG(Model):
   def Interface(self):
      self.p1 = SignalIn("p1",1)
      self.g1 = SignalIn("g1",1)
      self.p2 = SignalIn("p2",1)
      self.g2 = SignalIn("g2",1)
      self.p = SignalOut("p",1)
      self.g = SignalOut("g",1)

      self.vdd = VddIn("vdd")
      self.vss = VssIn("vss")
           
   def Netlist(self):
       self.p <= self.p1 & self.p2
       self.g <= self.g1 | (self.g2 & self.p1)

def prefixMatrix(algo,size):
   if algo == 'slansky':
      nb_stages = log2(size)
      matrix = []
      for i in range(0, nb_stages):
         matrix.append([])
         connection = True
         j = -1
         while (j < size-1):
            connection = not connection
            k = -1
            while((k < 2**i-1) and (j < size-1)):
               j = j + 1
               k = k + 1
               if connection :
                  matrix[i].append(1)
               else:
                  matrix[i].append(0)
      return matrix
   else:
      raise Exception ( '%s algorithm is not yet implemented' %algo )
