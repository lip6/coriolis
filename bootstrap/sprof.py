#!/usr/bin/env python

import sys
import os.path
import subprocess
import re


reSymbol   = re.compile(r'(?P<head>.*)(?P<symbol>_Z\w+)(?P<tail>.*)')

soPathName = sys.argv[1]
soFileName = os.path.basename( soPathName )
profName   = soFileName + '.profile'
if not os.path.isfile(profName):
  print '[ERROR] No profile datas <%s>.' % profName
  sys.exit( 1 )

sprof = subprocess.Popen ( ['sprof'
                           , soPathName
                           , profName ]
                         , stdout=subprocess.PIPE )

for line in sprof.stdout.readlines():
  m = reSymbol.match( line[:-1] )
  if m:
    cppfilt = subprocess.Popen ( ['c++filt'
                                 , '--no-verbose'
                                 , '--no-params'
                                 , m.group('symbol')]
                               , stdout=subprocess.PIPE )
    symbol  = cppfilt.stdout.readlines()[0][:-1]

    print m.group('head'), symbol
  else:
    print line[:-1]

sys.exit( 0 )
