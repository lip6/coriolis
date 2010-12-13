#!/usr/bin/env python

import sys
import re
import os
import os.path
import time
import socket
import subprocess
import optparse


class Project:

    def __init__ ( self, name, tools, repository ):
        self._name       = name
        self._tools      = tools
        self._repository = repository
        self._actives    = []
        return

    def getName       ( self ): return self._name
    def getTools      ( self ): return self._tools
    def getRepository ( self ): return self._repository
    def getActives    ( self ): return self._actives
    def hasTool       ( self, tool ): return tool in self._tools

    def desactivate ( self ):
        self._active = []
        return

    def activateAll ( self ):
        self._actives = self._tools
        return
    
    def activate ( self, tools ):
       # Build the ordered list.
        for tool in self._tools:
            if (tool in tools) and not (tool in self._actives):
                self._actives += [ tool ]
       # Find the tools not part of the project.
        rejecteds = []
        for tool in tools:
            if not (tool in self._tools) and (not tool in rejecteds):
                rejecteds += [ tool ]
        return rejecteds



class ProjectBuilder:

    def __init__ ( self ):
        self._projects         = []
        self._standalones      = []
        self._svnTag           = "x"
        self._svnMethod        = "svn+ssh://coriolis.soc.lip6.fr"
        self._rootDir          = os.path.join ( os.environ["HOME"], "coriolis-2.x" )
        self._quiet            = False
        self._buildMode        = "Release"
        self._rmBuild          = False
        self._doBuild          = True
        self._noCache          = False
        self._enableShared     = "ON"
        self._enableDoc        = "OFF"
        self._checkDatabase    = "OFF"
        self._checkDeterminism = "OFF"
        self._verboseMakefile  = "OFF"
        self._libMode          = "Shared"
        self._makeArguments    = []
        self._environment      = os.environ

        self._guessOs ()
        self._updateSecondary ()
        return


    def __setattr__ ( self, attribute, value ):
        if attribute[0] == "_":
            self.__dict__[attribute] = value
            return
        
        if   attribute == "svnTag":           self._svnTag           = value
        elif attribute == "svnMethod":        self._svnMethod        = value
        elif attribute == "rootDir":          self._rootDir          = os.path.expanduser(value)
        elif attribute == "quiet":            self._quiet            = value
        elif attribute == "buildMode":        self._buildMode        = value
        elif attribute == "rmBuild":          self._rmBuild          = value
        elif attribute == "doBuild":          self._doBuild          = value
        elif attribute == "noCache":          self._noCache          = value
        elif attribute == "enableShared":     self._enableShared     = value
        elif attribute == "enableDoc":        self._enableDoc        = value
        elif attribute == "enableShared":     self._enableShared     = value
        elif attribute == "checkDatabase":    self._checkDatabase    = value
        elif attribute == "checkDeterminism": self._checkDeterminism = value
        elif attribute == "verboseMakefile":  self._verboseMakefile  = value
        elif attribute == "makeArguments":    self._makeArguments    = value.split ()
        elif attribute == "libMode":          self._libMode          = value
        self._updateSecondary ()
        return


    def _updateSecondary ( self ):
        self._rpmbuildDir = os.path.join ( self._rootDir, "rpmbuild" )
        self._tmppathDir  = os.path.join ( self._rpmbuildDir, "tmp" )
        self._tarballDir  = os.path.join ( self._rootDir, "tarball" )
        self._archiveDir  = os.path.join ( self._tarballDir, "coriolis2-1.0.%s" % self._svnTag )
        self._sourceDir   = os.path.join ( self._rootDir, "src" )
        self._osDir       = os.path.join ( self._rootDir
                                        , self._osType
                                        , "%s.%s" % (self._buildMode,self._libMode) )
        self._buildDir    = os.path.join ( self._osDir, "build" )
        self._installDir  = os.path.join ( self._osDir, "install" )

        if self._enableShared == "ON": self._libMode = "Shared"
        else:                          self._libMode = "Static"

        self._specFileIn   = os.path.join ( self._sourceDir, "bootstrap", "coriolis2.spec.in" )
        self._specFile     = os.path.join ( self._sourceDir, "bootstrap", "coriolis2.spec" )
        self._sourceTarBz2 = "coriolis2-1.0.%s.tar.bz2"                  % self._svnTag
        self._binaryTarBz2 = "coriolis2-binary-1.0.%s-1.el5_soc.tar.bz2" % self._svnTag
        self._distribPatch = os.path.join ( self._sourceDir, "bootstrap", "coriolis2-for-distribution.patch" )
        return


    def _guessOs ( self ):
        self._libSuffix        = None
        self._osSLSoC5x_64     = re.compile (".*Linux.*el5.*x86_64.*")
        self._osSLSoC5x        = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
        self._osLinux_64       = re.compile (".*Linux.*x86_64.*")
        self._osLinux          = re.compile (".*Linux.*")
        self._osDarwin         = re.compile (".*Darwin.*")

        uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
        lines = uname.stdout.readlines()

        if   self._osSLSoC5x_64.match(lines[0]):
            self._osType    = "Linux.SLSoC5x_64"
            self._libSuffix = "64"
        elif self._osSLSoC5x   .match(lines[0]): self._osType = "Linux.SLSoC5x"
        elif self._osLinux_64  .match(lines[0]):
            self._osType    = "Linux.x86_64"
            self._libSuffix = "64"
        elif self._osLinux     .match(lines[0]): self._osType = "Linux.i386"
        elif self._osDarwin    .match(lines[0]): self._osType = "Darwin"
        else:
            uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
            self._osType = uname.stdout.readlines()[0][:-1]

            print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
            print "          (using: \"%s\")" % self._osType
        
        return


    def _guessSvnTag ( self, project ):
        revisionPattern = re.compile ( r"^Revision:\s*(?P<revision>\d+)" )
        projectSvnDir   = os.path.join ( self._svnMethod+project.getRepository() )

        command = [ "svn", "info", projectSvnDir ]
        svnInfo = subprocess.Popen ( command, stdout=subprocess.PIPE )

        for line in svnInfo.stdout.readlines():
            m = revisionPattern.match ( line )
            if m:
                self._svnTag = m.group("revision")
                print "Latest revision of project %s is %s." % (project.getName(),self._svnTag)
                self._updateSecondary ()
                return

        print "[WARNING] Cannot guess revision for project \"%s\"." % project.getName() 
        print "          (using: \"x\")"
        return


    def _doSpec ( self ):
        fdSpecFileIn = open ( self._specFileIn, "r" )
        fdSpecFile   = open ( self._specFile  , "w" )

        for line in fdSpecFileIn.readlines():
            stable       = False
            substituted0 = line

            while not stable:
                substituted1 = re.sub ( r"@svntag@"     , self._svnTag    , substituted0 )
                substituted1 = re.sub ( r"@coriolisTop@", "/opt/coriolis2", substituted1 )
                if substituted0 == substituted1: stable = True
                else: substituted0 = substituted1

            fdSpecFile.write ( substituted0 )

        fdSpecFileIn.close ()
        fdSpecFile.close ()
        return


    def _execute ( self, command, error ):
        sys.stdout.flush ()
        sys.stderr.flush ()
        child = subprocess.Popen ( command, env=self._environment, stdout=None )
        (pid,status) = os.waitpid ( child.pid, 0 )
        status >>= 8
        if status != 0:
            print "[ERROR] %s (status:%d)." % (error,status)
            sys.exit ( status )
        return


    def _build ( self, tool ):
        toolSourceDir = os.path.join ( self._sourceDir, tool )
        toolBuildDir  = os.path.join ( self._buildDir , tool )
       # Supplied directly in the CMakeLists.txt.
       #cmakeModules  = os.path.join ( self._installDir, "share", "cmake", "Modules" )

        if not os.path.isdir(toolSourceDir):
            print "[ERROR] Missing tool source directory: \"%s\" (skipped)." % toolSourceDir
            return

        if self._rmBuild:
            print "Removing tool build directory: \"%s\"." % toolBuildDir
            command = [ "/bin/rm", "-rf", toolBuildDir ]
            self._execute ( command, "Removing tool build directory" )
            
        if not os.path.isdir(toolBuildDir):
            print "Creating tool build directory: \"%s\"." % toolBuildDir
            os.makedirs ( toolBuildDir )
            os.chdir    ( toolBuildDir )

            command = ["cmake", "-D", "CMAKE_BUILD_TYPE:STRING=%s"  % self._buildMode
                              , "-D", "BUILD_SHARED_LIBS:STRING=%s" % self._enableShared
                             #, "-D", "CMAKE_MODULE_PATH:STRING=%s" % cmakeModules
                                    , toolSourceDir ]
            self._execute ( command, "First CMake failed" )

        os.chdir ( toolBuildDir )
        if self._noCache:
            cmakeCache = os.path.join(toolBuildDir,"CMakeCache.txt")
            if os.path.isfile ( cmakeCache ): os.unlink ( cmakeCache )

        command = ["cmake", "-D", "CMAKE_BUILD_TYPE:STRING=%s"       % self._buildMode
                          , "-D", "BUILD_SHARED_LIBS:STRING=%s"      % self._enableShared
                          , "-D", "BUILD_DOC:STRING=%s"              % self._enableDoc
                          , "-D", "CHECK_DATABASE:STRING=%s"         % self._checkDatabase
                          , "-D", "CHECK_DETERMINISM:STRING=%s"      % self._checkDeterminism
                          , "-D", "CMAKE_VERBOSE_MAKEFILE:STRING=%s" % self._verboseMakefile
                          , "-D", "CMAKE_INSTALL_PREFIX:STRING=%s"   % self._installDir
                          ]
        if self._libSuffix:
            command += [ "-D", "LIB_SUFFIX:STRING=%s" % self._libSuffix ]
        command += [ toolSourceDir ]
        self._execute ( command, "Second CMake failed" )

        if self._doBuild:
            command  = [ "make" ]
           #command += [ "DESTDIR=%s" % self._installDir ]
            if tool == "crlcore" and self._enableDoc == "ON":
                command += [ "dvi", "safepdf" ]
            command += self._makeArguments
            print "Make command:", command
            sys.stdout.flush ()
            self._execute ( command, "Build failed" )
        return


    def _svnStatus ( self, tool ):
        toolSourceDir = os.path.join ( self._sourceDir , tool )
        if not os.path.isdir(toolSourceDir):
            if not self._quiet:
                print "[ERROR] Missing tool source directory: \"%s\" (skipped)." % toolSourceDir
            return
        os.chdir ( toolSourceDir )

        print "Checking SVN status of tool: ", tool
        command = [ "svn", "status", "-u", "-q" ]
        self._execute ( command, "svn status -u -q" )
        print
        return


    def _svnUpdate ( self, tool ):
        toolSourceDir = os.path.join ( self._sourceDir , tool )
        if not os.path.isdir(toolSourceDir):
            if not self._quiet:
                print "[ERROR] Missing tool source directory: \"%s\" (skipped)." % toolSourceDir
            return
        os.chdir ( toolSourceDir )

        print "Doing a SVN update of tool: ", tool
        command = [ "svn", "update" ]
        self._execute ( command, "svn update" )
        print
        return


    def _svnCheckout ( self, tool ):
        project = self.getToolProject ( tool )
        if not project:
            print "[ERROR] Tool \"%s\" is not part of any project." % tool
            print "        Cannot guess the SVN repository."
            return
        if not project.getRepository ():
            print "[ERROR] Project \"%s\" isn't associated to a repository." % project.getName()
            return
        
        toolSvnTrunkDir = os.path.join ( self._svnMethod+project.getRepository(), tool, "trunk" )
        os.chdir ( self._sourceDir )

        print "Doing a SVN checkout of tool: ", tool
        command = [ "svn", "co", toolSvnTrunkDir, tool ]
        self._execute ( command, "svn checkout %s" % tool )
        print
        return


    def _svnExport ( self, tool ):
        project = self.getToolProject ( tool )
        if not project:
            print "[ERROR] Tool \"%s\" is not part of any project." % tool
            print "        Cannot guess the SVN repository."
            return
        if not project.getRepository ():
            print "[ERROR] Project \"%s\" isn't associated to a repository." % project.getName()
            return
        
        toolSvnTrunkDir = os.path.join ( self._svnMethod+project.getRepository(), tool, "trunk" )

        if not os.path.isdir ( self._archiveDir ):
            os.mkdir ( self._archiveDir )
        os.chdir ( self._archiveDir )

        toolExportDir = os.path.join ( self._archiveDir, tool )
        if os.path.isdir ( toolExportDir ):
            print "Removing tool export (tarball) directory: \"%s\"." % toolExportDir
            command = [ "/bin/rm", "-r", toolExportDir ]
            self._execute ( command, "Removing tool export (tarball) directory" )

        print "Doing a SVN export of tool: ", tool
        command = [ "svn", "export", toolSvnTrunkDir, toolExportDir ]
        if self._svnTag != "x":
            command += [ "--revision", self._svnTag ]
        self._execute ( command, "svn export %s" % toolExportDir )
        print
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


    def register ( self, project ):
        for registered in self._projects:
            if registered.getName() == project.getName():
                print "[ERROR] Project \"%s\" is already registered (ignored)."
                return
        self._projects += [ project ]
        return


    def _setEnvironment ( self, systemVariable, userVariable ):
        if not self._environment.has_key(systemVariable):
            self._environment[ systemVariable ] = self._installDir
       #if not self._environment.has_key(userVariable):
       #    self._environment[ userVariable ] = self._installDir
        if not self._quiet:
            print "Setting %s = \"%s\"." % (systemVariable    ,self._environment[systemVariable])
            if self._environment.has_key(userVariable):
                print "Transmitting %s = \"%s\"." % (userVariable,self._environment[userVariable])
        return


    def _commandTemplate ( self, tools, projects, command ):
       # Set or guess the various projects TOP environment variables.
        for project in self._projects:
            topVariable     = "%s_TOP"      % project.getName().upper()
            topUserVariable = "%s_USER_TOP" % project.getName().upper()
            self._setEnvironment ( topVariable, topUserVariable )

        if tools:
           # Checks if the requested tools are in the various projects.
            self._standalones = tools
            for project in self._projects:
                self._standalones = project.activate ( self._standalones ) 
            for tool in self._standalones:
                print "[WARNING] Tool \"%s\" is not part of any project." % tool

        if projects:
            for projectName in projects:
                project = self.getProject ( projectName )
                if not project:
                    print "[ERROR] No project of name \"%s\"." % projectName
                    sys.exit ( 1 )
                project.activateAll()

        if not tools and not projects:
            for project in self._projects:
                project.activateAll ()

        for project in self._projects:
            for tool in project.getActives():
                print "\nProcessing tool: \"%s\"." % tool
                getattr(self,command) ( tool )

        for tool in self._standalones:
            print "\nProcessing tool: \"%s\"." % tool
            getattr(self,command) ( tool )
        return


    def build ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_build" )
        return


    def svnStatus ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_svnStatus" )
        return


    def svnUpdate ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_svnUpdate" )
        return


    def svnCheckout ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_svnCheckout" )
        return


    def svnExport ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_svnExport" )
        return


    def tarball ( self, tools, projects ):
        if self._svnTag == "x":
            self._guessSvnTag ( self.getProject(projects[0]) )

        self._doSpec ()
        
        if os.path.isdir(self._tarballDir):
            print "Removing previous tarball directory: \"%s\"." % self._tarballDir
            command = [ "/bin/rm", "-r", self._tarballDir ]
            self._execute ( command, "Removing top export (tarball) directory" )
 
        print "Creating tarball directory: \"%s\"." % self._tarballDir
        os.makedirs ( self._tarballDir )
        self.svnExport ( tools, projects )

        removeds = [ os.path.join("vlsisapd","openChams")
                   , os.path.join("vlsisapd","dtr")
                   ]

       # Remove unpublisheds (yet) tools/files.
        for item in removeds:
            command = [ "/bin/rm", "-r", os.path.join(self._archiveDir,item) ]
            self._execute ( command, "rm of %s failed" % item)
 
        os.chdir ( self._archiveDir )
        command = [ "/usr/bin/patch", "--remove-empty-files"
                                    , "--no-backup-if-mismatch"
                                    , "-p0", "-i", self._distribPatch ]
        self._execute ( command, "patch for distribution command failed" )

        os.chdir ( self._tarballDir )
        command = [ "/bin/tar", "jcvf", self._sourceTarBz2, os.path.basename(self._archiveDir) ]
        self._execute ( command, "tar command failed" )
 
        print "Cleanup SVN export tarball archive directory: \"%s\"." % self._archiveDir
        command = [ "/bin/rm", "-r", self._archiveDir ]
        self._execute ( command, "Removing archive export (tarball) directory" )

        return


    def doRpm ( self, tools, projects ):
        self.tarball ( tools, projects )

        for rpmDir in [ "SOURCES", "SPECS", "BUILD", "tmp"
                      , "SRPMS", "RPMS/i386", "RPMS/i686", "RPMS/x86_64" ]:
            rpmFullDir = os.path.join ( self._rpmbuildDir, rpmDir )
            if not os.path.isdir(rpmFullDir):
                os.makedirs ( rpmFullDir )

        rpmSpecFile   = os.path.join ( self._rpmbuildDir, "SPECS"  , "coriolis2.spec" )
        rpmSourceFile = os.path.join ( self._rpmbuildDir, "SOURCES", self._sourceTarBz2 )
        sourceFile    = os.path.join ( self._tarballDir, self._sourceTarBz2 )

        if os.path.isfile ( rpmSpecFile ):
            os.unlink ( rpmSpecFile )
        os.symlink ( self._specFile  , rpmSpecFile   )

        if not os.path.islink ( rpmSourceFile ):
            os.symlink ( sourceFile, rpmSourceFile )

        os.chdir ( self._rpmbuildDir )

        command = [ "/usr/bin/rpmbuild"
                  , "--define", "_topdir                 %s" % self._rpmbuildDir
                  , "--define", "_tmppath                %s" % self._tmppathDir
                  , "--define", "_enable_debug_packages  0"
                  , "-ba", "--with", "binarytar", rpmSpecFile ]

        self._execute ( command, "Rebuild rpm packages" )

        return


if __name__ == "__main__":

    bootstrap = Project ( name      =  "bootstrap"
                        , tools     =[ "bootstrap" ]
                        , repository="/users/outil/coriolis/svn"
                        )

    vlsisapd  = Project ( name      =  "vlsisapd"
                        , tools     =[ "vlsisapd" ]
                        , repository="/users/outil/coriolis/svn"
                        )

    coriolis  = Project ( name     =  "coriolis"
                        , tools    =[ "hurricane"
                                    , "crlcore"
                                    , "nimbus"
                                    , "metis"
                                    , "mauka"
                                    , "knik"
                                    , "katabatic"
                                    , "kite"
                                    , "equinox"
                                    , "solstice"
                                    , "unicorn"
                                    , "ispd"
                                    , "cumulus"
                                    , "stratus1"
                                    ]
                        , repository="/users/outil/coriolis/svn"
                        )
    chams     = Project ( name     = "chams"
                        , tools    =[ "hurricaneAMS"
                                    , "amsCore"
                                    , "opSim"
                                    , "scribe"
                                    , "pharos"
                                    , "schematic"
                                    , "autoDTR"
                                    ]
                        , repository="/users/outil/chams/svn"
                        )

    parser = optparse.OptionParser ()  
  # Build relateds.
    parser.add_option ( "-q", "--quiet"      , action="store_true" ,                dest="quiet"    )
    parser.add_option ( "-r", "--release"    , action="store_true" ,                dest="release"  )
    parser.add_option ( "-d", "--debug"      , action="store_true" ,                dest="debug"    )
    parser.add_option ( "-s", "--static"     , action="store_true" ,                dest="static"   )
    parser.add_option (       "--doc"        , action="store_true" ,                dest="doc"      )
    parser.add_option ( "-D", "--check-db"   , action="store_true" ,                dest="checkDb"  )
    parser.add_option ( "-u", "--check-deter", action="store_true" ,                dest="checkDeterminism" )
    parser.add_option ( "-v", "--verbose"    , action="store_true" ,                dest="verboseMakefile" )
    parser.add_option (       "--root"       , action="store"      , type="string", dest="rootDir"  )
    parser.add_option (       "--no-build"   , action="store_true" ,                dest="noBuild"  )
    parser.add_option (       "--no-cache"   , action="store_true" ,                dest="noCache"  )
    parser.add_option (       "--rm-build"   , action="store_true" ,                dest="rmBuild"  )
    parser.add_option (       "--svn-tag"    , action="store"      , type="string", dest="svnTag"   )
    parser.add_option (       "--svn-method" , action="store"      , type="string", dest="svnMethod")
    parser.add_option (       "--make"       , action="store"      , type="string", dest="makeArguments")
    parser.add_option (       "--project"    , action="append"     , type="string", dest="projects" )
    parser.add_option ( "-t", "--tool"       , action="append"     , type="string", dest="tools"    )
   # SVN repository relateds.
    parser.add_option ( "--svn-status"   , action="store_true", dest="svnStatus"   )
    parser.add_option ( "--svn-update"   , action="store_true", dest="svnUpdate"   )
    parser.add_option ( "--svn-checkout" , action="store_true", dest="svnCheckout" )
   # Miscellaneous.
    parser.add_option ( "--tarball"      , action="store_true", dest="tarball" )
    parser.add_option ( "--do-rpm"       , action="store_true", dest="doRpm"   )
    ( options, args ) = parser.parse_args ()

    builder = ProjectBuilder ()
    builder.register ( bootstrap  )
    builder.register ( vlsisapd )
    builder.register ( coriolis )
    builder.register ( chams    )

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
    elif options.svnCheckout: builder.svnCheckout ( tools=options.tools, projects=options.projects )
    elif options.tarball:     builder.tarball     ( tools=options.tools, projects=options.projects )
    elif options.doRpm:       builder.doRpm       ( tools=options.tools, projects=options.projects )
    else:                     builder.build       ( tools=options.tools, projects=options.projects )

    sys.exit ( 0 )
