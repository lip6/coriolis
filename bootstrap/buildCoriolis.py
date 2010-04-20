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
        self._svnMethod        = "svn+ssh://coriolis.lip6.fr"
        self._rootDir          = os.path.join ( os.environ["HOME"], "coriolis-2.x" )
        self._quiet            = False
        self._buildMode        = "Release"
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
        self._sourceDir  = os.path.join ( self._rootDir, "src" )
        self._osDir      = os.path.join ( self._rootDir
                                        , self._osType
                                        , "%s.%s" % (self._buildMode,self._libMode) )
        self._buildDir   = os.path.join ( self._osDir, "build" )
        self._installDir = os.path.join ( self._osDir, "install" )

        if self._enableShared == "ON": self._libMode = "Shared"
        else:                          self._libMode = "Static"
        return


    def _guessOs ( self ):
        self._osSLSoC5x_64     = re.compile (".*Linux.*el5.*x86_64.*")
        self._osSLSoC5x        = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
        self._osLinux_64       = re.compile (".*Linux.*x86_64.*")
        self._osLinux          = re.compile (".*Linux.*")
        self._osDarwin         = re.compile (".*Darwin.*")

        uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
        lines = uname.stdout.readlines()

        if   self._osSLSoC5x_64.match(lines[0]): self._osType = "Linux.SLSoC5x_64"
        elif self._osSLSoC5x   .match(lines[0]): self._osType = "Linux.SLSoC5x"
        elif self._osLinux_64  .match(lines[0]): self._osType = "Linux.x86_64"
        elif self._osLinux     .match(lines[0]): self._osType = "Linux.i386"
        elif self._osDarwin    .match(lines[0]): self._osType = "Darwin"
        else:
            uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
            self._osType = uname.stdout.readlines()[0][:-1]

            print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
            print "          (using: \"%s\")" % self._osType
        
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
       #cmakeModules  = os.path.join ( self._installDir, "share", "cmake_modules" )

        if not os.path.isdir(toolSourceDir):
            print "[ERROR] Missing tool source directory: \"%s\" (skipped)." % toolSourceDir
            return
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
                          , toolSourceDir ]
        self._execute ( command, "Second CMake failed" )

        if self._doBuild:
            print "Make arguments:", self._makeArguments
            sys.stdout.flush ()
            command  = ["make", "DESTDIR=%s" % self._installDir]
            command += self._makeArguments
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


    def _commandTemplate ( self, tools, projects, command ):
       # Set or guess the various projects TOP environment variables.
        for project in self._projects:
            topVariable     = "%s_TOP"      % project.getName().upper()
            topUserVariable = "%s_USER_TOP" % project.getName().upper()
            if not self._environment.has_key(topVariable):
                self._environment[ topVariable ] = self._installDir
           #if not self._environment.has_key(topUserVariable):
           #    self._environment[ topUserVariable ] = self._installDir
            if not self._quiet:
                print "Setting %s = \"%s\"." % (topVariable    ,self._environment[topVariable])
                if self._environment.has_key(topUserVariable):
                    print "Transmitting %s = \"%s\"." % (topUserVariable,self._environment[topUserVariable])

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


if __name__ == "__main__":

    io       = Project ( name      = "io"
                       , tools     =[ "io" ]
                       , repository="/users/outil/coriolis/svn"
                       )

    coriolis = Project ( name     = "coriolis"
                       , tools    =[ "hurricane"
                                   , "crlcore"
                                   , "knik"
                                   , "katabatic"
                                   , "kite"
                                   , "equinox"
                                   , "solstice"
                                   , "unicorn"
                                   , "ispd"
                                   ]
                       , repository="/users/outil/coriolis/svn"
                       )
    chams    = Project ( name     = "chams"
                       , tools    =[ "hurricaneAMS"
                                   , "amsCore"
                                   , "opSim"
                                   , "scribe"
                                   , "pharos"
                                   , "schematic"
                                   ]
                       , repository="/users/outil/chams/svn"
                       )

    parser = optparse.OptionParser ()  
  # Build relateds.
    parser.add_option ( "-q", "--quiet"      , action="store_true",                dest="quiet"    )
    parser.add_option ( "-r", "--release"    , action="store_true",                dest="release"  )
    parser.add_option ( "-d", "--debug"      , action="store_true",                dest="debug"    )
    parser.add_option ( "-s", "--static"     , action="store_true",                dest="static"   )
    parser.add_option (       "--doc"        , action="store_true",                dest="doc"      )
    parser.add_option ( "-D", "--check-db"   , action="store_true",                dest="checkDb"  )
    parser.add_option ( "-u", "--check-deter", action="store_true",                dest="checkDeterminism" )
    parser.add_option ( "-v", "--verbose"    , action="store_true",                dest="verboseMakefile" )
    parser.add_option (       "--root"       , action="store"     , type="string", dest="rootDir"  )
    parser.add_option (       "--no-build"   , action="store_true",                dest="noBuild"  )
    parser.add_option (       "--no-cache"   , action="store_true",                dest="noCache"  )
    parser.add_option (       "--svn-tag"    , action="store"     , type="string", dest="svnTag"   )
    parser.add_option (       "--svn-method" , action="store"     , type="string", dest="svnMethod")
    parser.add_option (       "--make"       , action="store"     , type="string", dest="makeArguments")
    parser.add_option (       "--project"    , action="append"    , type="string", dest="projects" )
    parser.add_option ( "-t", "--tool"       , action="append"    , type="string", dest="tools"    )
   # SVN repository relateds.
    parser.add_option ( "--svn-status"   , action="store_true", dest="svnStatus"   )
    parser.add_option ( "--svn-update"   , action="store_true", dest="svnUpdate"   )
    parser.add_option ( "--svn-checkout" , action="store_true", dest="svnCheckout" )
    ( options, args ) = parser.parse_args ()

    builder = ProjectBuilder ()
    builder.register ( io       )
    builder.register ( coriolis )
    builder.register ( chams    )

    if options.quiet:            builder.quiet             = True
    if options.release:          builder.buildMode         = "Release"
    if options.debug:            builder.buildMode         = "Debug"
    if options.static:           builder.enableShared      = "OFF"
    if options.doc:              builder.enableDoc         = "ON"
    if options.checkDb:          builder.enableDatabase    = "ON"
    if options.checkDeterminism: builder.enableDeterminism = "ON"
    if options.verboseMakefile:  builder.verboseMakefile   = "ON"
    if options.rootDir:          builder.rootDir           = options.rootDir
    if options.noBuild:          builder.doBuild           = False
    if options.noCache:          builder.noCache           = True
    if options.makeArguments:    builder.makeArguments     = options.makeArguments
    if options.svnMethod:        builder.svnMethod         = options.svnMethod
    if options.svnTag:           builder.svnTag            = options.svnTag

    if   options.svnStatus:   builder.svnStatus   ( tools=options.tools, projects=options.projects )
    elif options.svnUpdate:   builder.svnUpdate   ( tools=options.tools, projects=options.projects )
    elif options.svnCheckout: builder.svnCheckout ( tools=options.tools, projects=options.projects )
    else:                     builder.build       ( tools=options.tools, projects=options.projects )

    sys.exit ( 0 )
