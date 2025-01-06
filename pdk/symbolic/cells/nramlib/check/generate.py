#!/usr/bin/env python
# -*- coding: utf-8 -*-

try:
  import sys
  import traceback
  import os.path
  import optparse
  import Cfg
  import CRL
  import stratus
except ImportError, e:
  serror = str(e)
  if serror.startswith('No module named'):
    module = serror.split()[-1]
    print '[ERROR] The <%s> python module or symbol cannot be loaded.' % module
    print '        Please check the integrity of the <coriolis> package.'
  if str(e).find('cannot open shared object file'):
    library = serror.split(':')[0]
    print '[ERROR] The <%s> shared library cannot be loaded.' % library
    print '        Under RHEL 6, you must be under devtoolset-2.'
    print '        (scl enable devtoolset-2 bash)'
  sys.exit(1)
except Exception, e:
  print '[ERROR] A strange exception occurred while loading the basic Coriolis/Python'
  print '        modules. Something may be wrong at Python/C API level.\n'
  print '        %s' % e
  sys.exit(2)


framework = CRL.AllianceFramework.get()


if __name__ == '__main__':
  parser = optparse.OptionParser()
  parser.add_option( '-b', '--bits' , type='int'   ,                      dest='bits'       , help='Number of bits of each word (2 <= bits <= 64).')
  parser.add_option( '-w', '--words', type='int'   ,                      dest='words'      , help='Number of words in the RAM (32 <= words <= 256).')
  parser.add_option( '-m', '--model', type='string',                      dest='model'      , help='The name of the model to generate.')
  parser.add_option( '-v', '--verbose'             , action='store_true', dest='verbose'    , help='First level of verbosity.')
  parser.add_option( '-V', '--very-verbose'        , action='store_true', dest='veryVerbose', help='Second level of verbosity.')
  (options, args) = parser.parse_args()

  modelName = None
  if options.verbose:     Cfg.getParamBool('misc.verboseLevel1').setBool(True)
  if options.veryVerbose: Cfg.getParamBool('misc.verboseLevel2').setBool(True)
  if options.model:       modelName = options.model

  print framework.getEnvironment().getPrint()

  cell = stratus.buildModel( 'dpgen_RAM'
                           , stratus.DoNetlist|stratus.DoLayout
                           , className ='DpgenRam'
                           , modelName = modelName
                           , parameters={ 'nbit'     : options.bits
                                        , 'nword'    : options.words
                                        , 'logical'  : True
                                        , 'physical' : True
                                        } )

  sys.exit( 0 )
