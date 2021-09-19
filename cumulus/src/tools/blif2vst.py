#!/usr/bin/env python3

import sys
import traceback
import os.path
import optparse
try:
    import helpers
    from   helpers    import showPythonTrace
    from   helpers.io import ErrorMessage, catch
    helpers.loadUserSettings()
    import Cfg
    import Hurricane
    from   Hurricane  import DbU, UpdateSession, Breakpoint, Transformation, \
                             Cell, Instance
    import Viewer
    import CRL
    import plugins.rsave
except Exception, e:
    catch( e )
    sys.exit(2)


framework = CRL.AllianceFramework.get()


def renameNMigen( occurrence ):
    masterCell = occurrence.getEntity().getMasterCell()
    origName   = masterCell.getName()
    replName   = origName.replace( '$$', '_unm' )
    if not masterCell.isTerminalNetlist() and not replName.startswith('cmpt_'):
        replName = 'cmpt_' + replName
    for letter in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
        replName = replName.replace(letter, letter.lower())
    if origName != replName:
        print( '  - "{}" => "{}"'.format(origName, replName) )
        masterCell.setName( replName )


def renameNMigenUniquify ( topCell ):
    for occurrence in topCell.getTerminalNetlistInstanceOccurrences():
        renameNMigen(occurrence)
    for occurrence in topCell.getNonTerminalNetlistInstanceOccurrences():
        renameNMigen(occurrence)
    return


if __name__ == '__main__':

    parser = optparse.OptionParser()
    parser.add_option( '-c', '--cell'                  , type='string'      , dest='cellName'            , help='The name of the BLIF to convert, without extension.')
    parser.add_option( '-v', '--verbose'               , action='store_true', dest='verbose'             , help='First level of verbosity.')
    parser.add_option( '-V', '--very-verbose'          , action='store_true', dest='veryVerbose'         , help='Second level of verbosity.')
    parser.add_option(       '--vst-use-concat'        , action='store_true', dest='vstUseConcat'        , help='The VST driver will use "&" (concat) in PORT MAP.')
    parser.add_option(       '--vst-no-lowercase'      , action='store_true', dest='vstNoLowerCase'      , help='The VST will keep the case of all identifiers.')
    parser.add_option(       '--vst-uniquify-uppercase', action='store_true', dest='vstUniquifyUpperCase', help='The uppercase identifiers will be uniquified.')
    parser.add_option(       '--vst-no-linkage'        , action='store_true', dest='vstNoLinkage'        , help='Undefined direction will be set to "in" instead of "linkage".')
    (options, args) = parser.parse_args()
    
    views = CRL.Catalog.State.Logical
    if options.verbose:
        Cfg.getParamBool('misc.verboseLevel1').setBool(True)
    if options.veryVerbose:
        Cfg.getParamBool('misc.verboseLevel1').setBool(True)
        Cfg.getParamBool('misc.verboseLevel2').setBool(True)
    if options.vstUseConcat:         views |= CRL.Catalog.State.VstUseConcat
    if options.vstNoLowerCase:       views |= CRL.Catalog.State.VstNoLowerCase
    if options.vstUniquifyUpperCase: views |= CRL.Catalog.State.VstUniquifyUpperCase
    if options.vstNoLinkage:         views |= CRL.Catalog.State.VstNoLinkage
      
    cell = CRL.Blif.load( options.cellName )
    if cell.getName() == 'top':
        print( '  o  Renaming RTLIL anonymous top cell "top" into "{}".'.format(options.cellName) )
        cell.setName( options.cellName )
    renameNMigenUniquify( cell )
    CRL.restoreNetsDirection( cell, Cell.Flags_TerminalNetlist )
  
    kw          = {}
    kw['views'] = views
    kw['cell' ] = cell
  
    plugins.rsave.scriptMain( **kw )
  
    sys.exit( 0 )
