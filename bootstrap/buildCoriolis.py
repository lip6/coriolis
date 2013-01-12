#!/usr/bin/env python

import sys
import os.path
import optparse
from   builder         import ErrorMessage
from   builder.Builder import Builder


if __name__ == "__main__":

    try:
        scriptPath = os.path.abspath( os.path.dirname(sys.argv[0]) )
        print 'buildCoriolis.py is run from:'
        print '  <%s>' % scriptPath

        parser = optparse.OptionParser ()  
      # Build relateds.
        parser.add_option ( "-c", "--conf", type="string", dest="conf" ,                                    help="Fichier de configuration." )
        parser.add_option (       "--show-conf"  , action="store_true" ,                dest="showConf"   , help="Display the Project/Tools configuration, then exit." )
        parser.add_option ( "-q", "--quiet"      , action="store_true" ,                dest="quiet"      , help="Do not print all the informative messages." )
        parser.add_option ( "-r", "--release"    , action="store_true" ,                dest="release"    , help="Build a <Release> aka optimized version." )
        parser.add_option ( "-d", "--debug"      , action="store_true" ,                dest="debug"      , help="Build a <Debug> aka (-g) version." )
        parser.add_option ( "-s", "--static"     , action="store_true" ,                dest="static"     , help="Try to link statically, as much as possible." )
        parser.add_option (       "--doc"        , action="store_true" ,                dest="doc"        , help="Enable the documentation building (uses with -j1)." )
        parser.add_option ( "-v", "--verbose"    , action="store_true" ,                dest="verboseMakefile" , help="Tells CMake to print all compilation commands." )
        parser.add_option (       "--root"       , action="store"      , type="string", dest="rootDir"    , help="The root directory (default: <~/coriolis-2.x/>)." )
        parser.add_option (       "--no-build"   , action="store_true" ,                dest="noBuild"    , help="Do *not* build anything (by default: build)." )
        parser.add_option (       "--no-cache"   , action="store_true" ,                dest="noCache"    , help="Remove previous CMake cache before building." )
        parser.add_option (       "--rm-build"   , action="store_true" ,                dest="rmBuild"    , help="Remove previous build directoty before building." )
        parser.add_option (       "--make"       , action="store"      , type="string", dest="makeArguments", help="Arguments to pass to make (ex: \"-j4 install\")." )
        parser.add_option (       "--project"    , action="append"     , type="string", dest="projects"   , help="The name of a project to build (may appears any number of time)." )
        parser.add_option ( "-t", "--tool"       , action="append"     , type="string", dest="tools"      , help="The name of a tool to build (may appears any number of time)." )
       # SVN repository relateds.
        parser.add_option ( "--svn-tag"          , action="store"      , type="string", dest="svnTag"     , help="Explicitly select a SVN tag (for SVN related commands)." )
        parser.add_option ( "--svn-method"       , action="store"      , type="string", dest="svnMethod"  , help="Allows to sets the svn checkout method (svn+ssh://coriolis.soc.lip6.fr)." )
        parser.add_option ( "--svn-status"       , action="store_true"                , dest="svnStatus"  , help="Check status against the SVN repository." )
        parser.add_option ( "--svn-diff"         , action="store_true"                , dest="svnDiff"    , help="Perform a diff against the SVN repository." )
        parser.add_option ( "--svn-update"       , action="store_true"                , dest="svnUpdate"  , help="Update to the latest SVN version *or* the one given by svn-tag." )
        parser.add_option ( "--svn-checkout"     , action="store_true"                , dest="svnCheckout", help="Checkout the latest SVN version *or* the one given by svn-tag." )
       # Miscellaneous.                                                
        parser.add_option ( "--user-tarball"     , action="store_true"                , dest="userTarball", help="Regenerate a tarball from checked out sources (in <root>/tarball/." )
        parser.add_option ( "--tarball"          , action="store_true"                , dest="tarball"    , help="Regenerate a tarball (in <root>/tarball/." )
        parser.add_option ( "--rpm"              , action="store_true"                , dest="doRpm"      , help="Regenerate RPM packages." )
        parser.add_option ( "--deb"              , action="store_true"                , dest="doDeb"      , help="Regenerate Debian/Ubuntu packages." )
       # Katabatic/Kite specific options.
        parser.add_option ( "-D", "--check-db"   , action="store_true"                , dest="checkDb"    , help="Enable Katabatic/Kite data-base checking (*very* slow)." )
        parser.add_option ( "-u", "--check-deter", action="store_true"                , dest="checkDeterminism", help="Enable Katabatic/Kite determinism checking (*very* slow)." )
        ( options, args ) = parser.parse_args ()
    
        builder = Builder ()
        builder.loadConfiguration ( options.conf )

        if options.showConf:
            builder.showConfiguration ()
            sys.exit(0)
    
        if options.quiet:            builder.quiet             = True
        if options.release:          builder.buildMode         = "Release"
        if options.debug:            builder.buildMode         = "Debug"
        if options.static:           builder.enableShared      = "OFF"
        if options.doc:              builder.enableDoc         = "ON"
        if options.checkDb:          builder.checkDatabase     = "ON"
        if options.checkDeterminism: builder.enableDeterminism = "ON"
        if options.verboseMakefile:  builder.verboseMakefile   = "ON"
        if options.rootDir:          builder.rootDir           = options.rootDir
        if options.noBuild:          builder.doBuild           = False
        if options.noCache:          builder.noCache           = True
        if options.rmBuild:          builder.rmBuild           = True
        if options.makeArguments:    builder.makeArguments     = options.makeArguments
        if options.svnMethod:        builder.svnMethod         = options.svnMethod
        if options.svnTag:           builder.svnTag            = options.svnTag
    
        if   options.svnStatus:   builder.svnStatus   ( tools=options.tools, projects=options.projects )
        elif options.svnUpdate:   builder.svnUpdate   ( tools=options.tools, projects=options.projects )
        elif options.svnDiff:     builder.svnDiff     ( tools=options.tools, projects=options.projects )
        elif options.svnCheckout: builder.svnCheckout ( tools=options.tools, projects=options.projects )
        elif options.userTarball: builder.userTarball ( tools=options.tools, projects=options.projects )
        elif options.tarball:     builder.svnTarball  ( tools=options.tools, projects=options.projects )
        elif options.doRpm:       builder.doRpm       ()
        elif options.doDeb:       builder.doDeb       ()
        else:                     builder.build       ( tools=options.tools, projects=options.projects )
    except ErrorMessage, e:
        print e
        sys.exit(e.code)
   #except Exception, e:
   #    print e
   #    sys.exit(1)

    sys.exit ( 0 )
