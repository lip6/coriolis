
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2008-2015, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                   Jean-Paul Chaput               |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/Configuration.py"                   |
# +-----------------------------------------------------------------+


import sys
import re
import os
import os.path
import datetime
import subprocess
from   .       import ErrorMessage
from   Project import Project


class Configuration ( object ):

    PrimaryNames = \
        [ 'confFile'   , 'projects', 'standalones'
        , 'gitHash'    , 'gitMethod', 'revDate'
        , 'projectDir' , 'rootDir'
        , 'packageName', 'packageVersion', 'packageExcludes', 'packageProjects'
        , 'osType'     , 'libSuffix', 'buildMode', 'libMode', 'enableShared'
        ]
    SecondaryNames = \
        [ 'rpmbuildDir' , 'debbuildDir'   , 'tmppathDir'  , 'tarballDir'
        , 'archiveDir'  , 'sourceDir'     , 'osDir'       , 'buildDir'
        , 'installDir'  , 'bootstrapDir'  , 'specFileIn'  , 'specFile'
        , 'debianDir'   , 'debChangelogIn', 'debChangelog', 'sourceTarBz2'
        , 'binaryTarBz2', 'distribPatch'
        ]

    def __init__ ( self ):
        self._confFile         = None
        self._projects         = []
        self._standalones      = []
        self._revDate          = datetime.date.today().strftime('%Y%m%d')
        self._gitHash          = "x"
        self._gitMethod        = None
        self._projectDir       = 'coriolis-2.x'
        self._rootDir          = os.path.join ( os.environ["HOME"], self._projectDir )
        self._packageName      = None
        self._packageVersion   = None
        self._packageExcludes  = []
        self._packageProject   = []
        self._enableShared     = 'ON'
        self._buildMode        = 'Release'

       # Secondary (derived) variables.
       # Setup by _updateSecondary().
        self._guessOs()
        self._updateSecondary()
        return


    def __setattr__ ( self, attribute, value ):
        if attribute in Configuration.SecondaryNames:
            print ErrorMessage( 1, 'Attempt to write in read-only attribute <%s> in Configuration.'%attribute )
            return

        if attribute[0] == '_':
            self.__dict__[attribute] = value
            return

        if   attribute == 'rootDir': value = os.path.expanduser(value)
        elif attribute == 'enableShared' and value != 'ON': value = 'OFF'

        self.__dict__['_'+attribute] = value
        self._updateSecondary()
        return


    def __getattr__ ( self, attribute ):
        if attribute[0] != '_': attribute = '_'+attribute
        if not self.__dict__.has_key(attribute):
            raise ErrorMessage( 1, 'Configuration has no attribute <%s>.'%attribute )
        return self.__dict__[attribute]


    def _updateSecondary ( self ):
        if self._enableShared == "ON": self._libMode = "Shared"
        else:                          self._libMode = "Static"

       #self._rootDir      = os.path.join ( os.environ["HOME"], self._projectDir )
        self._rpmbuildDir  = os.path.join ( self._rootDir    , "rpmbuild" )
        self._debbuildDir  = os.path.join ( self._rootDir    , "debbuild" )
        self._tmppathDir   = os.path.join ( self._rpmbuildDir, "tmp" )
        self._tarballDir   = os.path.join ( self._rootDir    , "tarball" )
        self._archiveDir   = os.path.join ( self._tarballDir , "%s-%s.%sgit%s" % (self._packageName
                                                                                 ,self._packageVersion
                                                                                 ,self._revDate
                                                                                 ,self._gitHash) )
        self._sourceDir    = os.path.join ( self._rootDir    , "src" )
        self._bootstrapDir = os.path.join ( self._sourceDir  , "coriolis/bootstrap" )
        self._osDir        = os.path.join ( self._rootDir
                                          , self._osType
                                          , "%s.%s" % (self._buildMode,self._libMode) )
        self._buildDir     = os.path.join ( self._osDir, "build" )
        self._installDir   = os.path.join ( self._osDir, "install" )

        self._specFileIn     = os.path.join ( self._bootstrapDir, "%s.spec.in"%self._packageName )
        self._specFile       = os.path.join ( self._bootstrapDir, "%s.spec"   %self._packageName )
        self._debianDir      = os.path.join ( self._bootstrapDir, "debian" )
        self._debChangelogIn = os.path.join ( self._debianDir   , "changelog.in" )
        self._debChangelog   = os.path.join ( self._debianDir   , "changelog" )
        self._sourceTarBz2   = "%s-%s.%sgit%s.tar.bz2"            % (self._packageName
                                                                    ,self._packageVersion
                                                                    ,self._revDate
                                                                    ,self._gitHash)
        self._binaryTarBz2   = "%s-binary-%s.%sgit%s-1.el7.tar.bz2" % (self._packageName
                                                                      ,self._packageVersion
                                                                      ,self._revDate
                                                                      ,self._gitHash)
        self._distribPatch   = os.path.join ( self._sourceDir, "bootstrap", "%s-for-distribution.patch"%self._packageName )
        return


    def _guessOs ( self ):
        self._libSuffix         = None
        self._osSlsoc7x_64      = re.compile (".*Linux.*(el7|slsoc7).*x86_64.*")
        self._osSlsoc6x_64      = re.compile (".*Linux.*(el6|slsoc6).*x86_64.*")
        self._osSlsoc6x         = re.compile (".*Linux.*(el6|slsoc6).*")
        self._osSLSoC5x_64      = re.compile (".*Linux.*el5.*x86_64.*")
        self._osSLSoC5x         = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
        self._osFedora_64       = re.compile (".*Linux.*fc.*x86_64.*")
        self._osFedora          = re.compile (".*Linux.*fc.*")
        self._osLinux_64        = re.compile (".*Linux.*x86_64.*")
        self._osLinux           = re.compile (".*Linux.*")
        self._osFreeBSD8x_amd64 = re.compile (".*FreeBSD 8.*amd64.*")
        self._osFreeBSD8x_64    = re.compile (".*FreeBSD 8.*x86_64.*")
        self._osFreeBSD8x       = re.compile (".*FreeBSD 8.*")
        self._osDarwin          = re.compile (".*Darwin.*")
        self._osCygwinW7_64     = re.compile (".*CYGWIN_NT-6\.1.*x86_64.*")
        self._osCygwinW7        = re.compile (".*CYGWIN_NT-6\.1.*i686.*")
        self._osCygwinW8_64     = re.compile (".*CYGWIN_NT-6\.[2-3].*x86_64.*")
        self._osCygwinW8        = re.compile (".*CYGWIN_NT-6\.[2-3].*i686.*")

        uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
        lines = uname.stdout.readlines()

        if self._osSlsoc7x_64.match(lines[0]):
            self._osType    = "Linux.el7_64"
            self._libSuffix = "64"
        elif self._osSlsoc6x_64.match(lines[0]):
            self._osType    = "Linux.slsoc6x_64"
            self._libSuffix = "64"
        elif self._osSlsoc6x   .match(lines[0]): self._osType = "Linux.slsoc6x"
        elif self._osSLSoC5x_64.match(lines[0]):
            self._osType    = "Linux.SLSoC5x_64"
            self._libSuffix = "64"
        elif self._osSLSoC5x   .match(lines[0]): self._osType = "Linux.SLSoC5x"
        elif self._osFedora_64 .match(lines[0]):
            self._osType    = "Linux.fc_64"
            self._libSuffix = "64"
        elif self._osFedora    .match(lines[0]): self._osType = "Linux.fc"
        elif self._osLinux_64  .match(lines[0]):
            self._osType    = "Linux.x86_64"
            self._libSuffix = "64"
        elif self._osLinux     .match(lines[0]): self._osType = "Linux.i386"
        elif self._osDarwin    .match(lines[0]): self._osType = "Darwin"
        elif self._osFreeBSD8x_amd64.match(lines[0]):
            self._osType    = "FreeBSD.8x.amd64"
            self._libSuffix = "64"
        elif self._osFreeBSD8x_64.match(lines[0]):
            self._osType    = "FreeBSD.8x.x86_64"
            self._libSuffix = "64"
        elif self._osFreeBSD8x .match(lines[0]): self._osType = "FreeBSD.8x.i386"
        elif self._osCygwinW7_64.match(lines[0]):
            self._osType = "Cygwin.W7_64"
            self._libSuffix = "64"
        elif self._osCygwinW7.match(lines[0]): self._osType = "Cygwin.W7"
        elif self._osCygwinW8_64.match(lines[0]):
            self._osType = "Cygwin.W8_64"
            self._libSuffix = "64"
        elif self._osCygwinW8.match(lines[0]): self._osType = "Cygwin.W8"
        else:
            uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
            self._osType = uname.stdout.readlines()[0][:-1]

            print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
            print "          (using: \"%s\")" % self._osType
        
        return


    def getPrimaryIds   ( self ): return Configuration.PrimaryNames
    def getSecondaryIds ( self ): return Configuration.SecondaryNames
    def getAllIds       ( self ): return Configuration.PrimaryNames + Configuration.SecondaryNames


    def register ( self, project ):
        for registered in self._projects:
            if registered.getName() == project.getName():
                print ErrorMessage( 0, "Project \"%s\" is already registered (ignored)." )
                return
        self._projects += [ project ]
        return


    def getProject ( self, name ):
        for project in self._projects:
            if project.getName() == name:
                return project
        return None


    def getToolProject ( self, name ):
        for project in self._projects:
            if project.hasTool(name):
                return project
        return None


    def load ( self, confFile ):
        moduleGlobals = globals()

        if not confFile:
            print 'Making an educated guess to locate the configuration file:'
            locations = [ os.path.abspath(os.path.dirname(sys.argv[0]))
                        , os.environ['HOME']+'/coriolis-2.x/src/coriolis/bootstrap'
                        , os.environ['HOME']+'/coriolis/src/coriolis/bootstrap'
                        , '/users/outil/coriolis/coriolis-2.x/src/coriolis/bootstrap'
                        , self._rootDir+'/src/coriolis/bootstrap'
                        ]

            for location in locations:
                self._confFile = location + '/build.conf'
                print '  <%s>' % self._confFile

                if os.path.isfile(self._confFile): break
            if not self._confFile:
                ErrorMessage( 1, 'Cannot locate any configuration file.' ).terminate()
        else:
            print 'Using user-supplied configuration file:'
            print '  <%s>' % confFile

            self._confFile = confFile
            if not os.path.isfile(self._confFile):
                ErrorMessage( 1, 'Missing configuration file:', '<%s>'%self._confFile ).terminate()

        print 'Reading configuration from:'
        print '  <%s>' % self._confFile
        
        try:
            execfile( self._confFile, moduleGlobals )
        except Exception, e:
            ErrorMessage( 1, 'An exception occured while loading the configuration file:'
                           , '<%s>\n' % (self._confFile)
                           , 'You should check for simple python errors in this file.'
                           , 'Error was:'
                           , '%s\n' % e ).terminate()
        
        if moduleGlobals.has_key('projects'):
            entryNb = 0
            for entry in moduleGlobals['projects']:
                entryNb += 1
                if not entry.has_key('name'):
                    raise ErrorMessage( 1, 'Missing project name in project entry #%d.' % entryNb )
                if not entry.has_key('tools'):
                    raise ErrorMessage( 1, 'Missing tools list in project entry #%d (<%s>).' \
                                           % (entryNb,entry['name']) )
                if not isinstance(entry['tools'],list):
                    raise ErrorMessage( 1, 'Tools item of project entry #%d (<%s>) is not a list.' \
                                           % (entryNb,entry['name']) )
                if not entry.has_key('repository'):
                    raise ErrorMessage( 1, 'Missing project repository in project entry #%d.' \
                                           % entryNb )

                self.register( Project(entry['name'],entry['tools'],entry['repository']) )
        else:
            ErrorMessage( 1, 'Configuration file is missing the \'project\' symbol.'
                           , '<%s>'%self._confFile ).terminate()

        if moduleGlobals.has_key('projectdir'):
            self.projectDir = moduleGlobals['projectdir']

        if moduleGlobals.has_key('svnconfig'):
            svnconfig = moduleGlobals['svnconfig']
            if svnconfig.has_key('method'): self._svnMethod = svnconfig['method']

        if moduleGlobals.has_key('package'):
            package = moduleGlobals['package']
            if package.has_key('name'    ): self.packageName    = package['name']
            if package.has_key('version' ): self.packageVersion = package['version']
            if package.has_key('excludes'):
                if not isinstance(package['excludes'],list):
                    raise ErrorMessage( 1, 'Excludes of package configuration is not a list.')
                self._packageExcludes = package['excludes']
            if package.has_key('projects'):
                if not isinstance(package['projects'],list):
                    raise ErrorMessage( 1, 'Projects to package is not a list.')
                self._packageProjects = package['projects']
        return


    def show ( self ):
        print 'CCB Configuration:'
        if self._gitMethod:
            print '  Git Method: <%s>' % self._gitMethod
        else:
            print '  Git Method not defined, will not be able to push/pull.'

        for project in self._projects:
            print '  project:%-15s repository:<%s>' % ( ('<%s>'%project.getName()), project.getRepository() )
            toolOrder = 1
            for tool in project.getTools():
                print '%s%02d:<%s>' % (' '*26,toolOrder,tool)
                toolOrder += 1
            print
        return
