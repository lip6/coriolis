#!/usr/bin/env python

import sys
import os
import os.path
import optparse
import re


cppPattern      = re.compile( r".*\.(h|cpp)$" )
refactorPattern = re.compile( r"^\s*(?P<orig>[^\s]*)\s*-->\s*(?P<replace>[^\s]*)\s*$" )


class Refactor:

    def __init__ ( self ):
        self._substitutions = []
        return

    def _addRefactor ( self, original, replacement ):
        self._substitutions.append( ( re.compile(original), replacement ) )
        return

    def doLineRefactor ( self, line ):
        pline = line
        for (original,replacement) in self._substitutions:
          pline = original.sub( replacement, pline )
        return pline

    def doFileRefactor ( self, file ):
        oldFile = file + '.noRefactor'
        if not os.path.isfile(oldFile):
          os.rename( file, oldFile )

        fdold = open( oldFile, 'r' )
        fdnew = open( file   , 'w' )

        while True:
          oldline = fdold.readline()
          if oldline == '': break

          newline = self.doLineRefactor( oldline )
          fdnew.write( newline )

        fdnew.close()
        fdold.close()
        return

    def loadPatterns ( self, file ):
        fd = open( file, "r" )
        if fd:
          while True:
            line = fd.readline()
            if line    == ""  : break
            if line[0] == '#' : continue
            if line[0] == '\n': continue
          
            m = refactorPattern.match( line )
            if m:
              print "o:\"%s\" r:\"%s\"" % ( m.group("orig"), m.group("replace") )
              self._addRefactor( m.group("orig"), m.group("replace") )
          fd.close ()
        return 
    

if __name__ == '__main__' :

    parser = optparse.OptionParser ()
    parser.add_option( '-t', '--tree'    , action='store' , type='string', dest='tree'     )
    parser.add_option( '-p', '--patterns', action='store' , type='string', dest='patterns' )
    parser.add_option( '-f', '--file'    , action='append', type='string', dest='files'    )

    (options, args) = parser.parse_args()

    if not options.patterns and not options.files: sys.exit( 1 )

    refactor = Refactor()
    refactor.loadPatterns( options.patterns )

    rfiles = []
    if options.patterns:
      if options.tree:
        for (root,dirs,tfiles) in os.walk(options.tree):
          if  'CVS' in dirs: dirs.remove(  'CVS' )
          if '.svn' in dirs: dirs.remove( '.svn' )
          for file in tfiles:
            if cppPattern.match( file ):
              rfiles.append( os.path.join(root,file) )

    allFiles = []
    if options.files: allFiles += options.files
    allFiles += rfiles

    for file in allFiles:
      print file
      refactor.doFileRefactor( file )

    sys.exit( 0 )
