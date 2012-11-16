#!/usr/bin/env python

import sys
import re
import os
import os.path
import datetime
import socket
import subprocess
import optparse


class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
        self._code   = code
        self._errors = [ 'Malformed call to ErrorMessage()'
                       , '%s' % str(arguments) ]

        text = None
        if len(arguments) == 1:
            if isinstance(arguments[0],Exception): text = str(arguments[0]).split('\n')
            else:
                self._errors = arguments[0]
        elif len(arguments) > 1:
            text = list(arguments)

        if text:
            self._errors = []
            while len(text[0]) == 0: del text[0]

            lstrip = 0
            if text[0].startswith('[ERROR]'): lstrip = 8 

            for line in text:
                if line[0:lstrip  ] == ' '*lstrip or \
                   line[0:lstrip-1] == '[ERROR]':
                    self._errors += [ line[lstrip:] ]
                else:
                    self._errors += [ line.lstrip() ]
        return

    def __str__ ( self ):
        if not isinstance(self._errors,list):
            return "[ERROR] %s" % self._errors

        formatted = "\n"
        for i in range(len(self._errors)):
            if i == 0: formatted += "[ERROR] %s" % self._errors[i]
            else:      formatted += "        %s" % self._errors[i]
            if i+1 < len(self._errors): formatted += "\n"
        return formatted

    def addMessage ( self, message ):
        if not isinstance(self._errors,list):
            self._errors = [ self._errors ]
        if isinstance(message,list):
            for line in message:
                self._errors += [ line ]
        else:
            self._errors += [ message ]
        return

    def terminate ( self ):
        print self
        sys.exit(self._code)

    def _getCode ( self ): return self._code

    code = property(_getCode)



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
        self._svnMethod        = None
        self._projectDir       = 'coriolis-2.x'
        self._rootDir          = os.path.join ( os.environ["HOME"], self._projectDir )
        self._packageName      = None
        self._packageVersion   = None
        self._packageExcludes  = []
        self._packageProject   = []

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
        elif attribute == "projectDir":       self._projectDir       = value
        elif attribute == "rootDir":          self._rootDir          = os.path.expanduser(value)
        elif attribute == "packageName":      self._packageName      = value
        elif attribute == "packageVersion":   self._packageVersion   = value
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
       #self._rootDir     = os.path.join ( os.environ["HOME"], self._projectDir )
        self._rpmbuildDir = os.path.join ( self._rootDir    , "rpmbuild" )
        self._debbuildDir = os.path.join ( self._rootDir    , "debbuild" )
        self._tmppathDir  = os.path.join ( self._rpmbuildDir, "tmp" )
        self._tarballDir  = os.path.join ( self._rootDir    , "tarball" )
        self._archiveDir  = os.path.join ( self._tarballDir , "%s-%s.%s" % (self._packageName
                                                                           ,self._packageVersion
                                                                           ,self._svnTag) )
        self._sourceDir   = os.path.join ( self._rootDir    , "src" )
        self._osDir       = os.path.join ( self._rootDir
                                        , self._osType
                                        , "%s.%s" % (self._buildMode,self._libMode) )
        self._buildDir    = os.path.join ( self._osDir, "build" )
        self._installDir  = os.path.join ( self._osDir, "install" )

        if self._enableShared == "ON": self._libMode = "Shared"
        else:                          self._libMode = "Static"

        self._specFileIn     = os.path.join ( self._sourceDir, "bootstrap", "%s.spec.in"%self._packageName )
        self._specFile       = os.path.join ( self._sourceDir, "bootstrap", "%s.spec"   %self._packageName )
        self._debianDir      = os.path.join ( self._sourceDir, "bootstrap", "debian" )
        self._debChangelogIn = os.path.join ( self._debianDir, "changelog.in" )
        self._debChangelog   = os.path.join ( self._debianDir, "changelog" )
        self._sourceTarBz2   = "%s-%s.%s.tar.bz2"                 % (self._packageName,self._packageVersion,self._svnTag)
        self._binaryTarBz2   = "%s-binary-%s.%s-1.slsoc6.tar.bz2" % (self._packageName,self._packageVersion,self._svnTag)
        self._distribPatch   = os.path.join ( self._sourceDir, "bootstrap", "%s-for-distribution.patch"%self._packageName )
        return


    def _guessOs ( self ):
        self._libSuffix         = None
        self._osSlsoc6x_64      = re.compile (".*Linux.*el6.*x86_64.*")
        self._osSlsoc6x         = re.compile (".*Linux.*el6.*")
        self._osSLSoC5x_64      = re.compile (".*Linux.*el5.*x86_64.*")
        self._osSLSoC5x         = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
        self._osLinux_64        = re.compile (".*Linux.*x86_64.*")
        self._osLinux           = re.compile (".*Linux.*")
        self._osFreeBSD8x_amd64 = re.compile (".*FreeBSD 8.*amd64.*")
        self._osFreeBSD8x_64    = re.compile (".*FreeBSD 8.*x86_64.*")
        self._osFreeBSD8x       = re.compile (".*FreeBSD 8.*")
        self._osDarwin          = re.compile (".*Darwin.*")

        uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
        lines = uname.stdout.readlines()

        if self._osSlsoc6x_64.match(lines[0]):
            self._osType    = "Linux.slsoc6x_64"
            self._libSuffix = "64"
        elif self._osSlsoc6x   .match(lines[0]): self._osType = "Linux.slsoc6x"
        elif self._osSLSoC5x_64.match(lines[0]):
            self._osType    = "Linux.SLSoC5x_64"
            self._libSuffix = "64"
        elif self._osSLSoC5x   .match(lines[0]): self._osType = "Linux.SLSoC5x"
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


    def _configure ( self, fileIn, file ):
        fdFileIn = open ( fileIn, "r" )
        fdFile   = open ( file  , "w" )

        for line in fdFileIn.readlines():
            stable       = False
            substituted0 = line

            while not stable:
                substituted1 = re.sub ( r"@svntag@"     , self._svnTag    , substituted0 )
                substituted1 = re.sub ( r"@coriolisTop@", "/opt/coriolis2", substituted1 )
                if substituted0 == substituted1: stable = True
                else: substituted0 = substituted1

            fdFile.write ( substituted0 )

        fdFileIn.close ()
        fdFile.close ()
        return


    def _doSpec ( self ):
        self._configure ( self._specFileIn, self._specFile )
        return


    def _doDebChangelog ( self ):
        self._configure ( self._debChangelogIn, self._debChangelog )
        return


    def _execute ( self, command, error ):
        sys.stdout.flush ()
        sys.stderr.flush ()
        child = subprocess.Popen ( command, env=self._environment, stdout=None )
        (pid,status) = os.waitpid ( child.pid, 0 )
        status >>= 8
        if status != 0:
            ErrorMessage( status, "%s (status:%d)."%(error,status) ).terminate()
        return


    def _enableTool ( self, tool ):
        return


    def _build ( self, tool ):
        toolSourceDir = os.path.join ( self._sourceDir, tool )
        toolBuildDir  = os.path.join ( self._buildDir , tool )
       # Supplied directly in the CMakeLists.txt.
       #cmakeModules  = os.path.join ( self._installDir, "share", "cmake", "Modules" )

        if not os.path.isdir(toolSourceDir):
            print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir )
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
            if self._enableDoc == "ON":
               #if tool == "crlcore" or tool == "stratus1":
                if tool == "stratus1":
                    command += [ "dvi", "safepdf", "html" ]
            command += self._makeArguments
            print "Make command:", command
            sys.stdout.flush ()
            self._execute ( command, "Build failed" )
        return


    def _svnStatus ( self, tool ):
        toolSourceDir = os.path.join ( self._sourceDir , tool )
        if not os.path.isdir(toolSourceDir):
            if not self._quiet:
                print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir )
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
                print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir)
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
            print ErrorMessage( 0, "Tool \"%s\" is not part of any project."%tool
                                 ,"Cannot guess the SVN repository." )
            return
        if not project.getRepository ():
            print ErrorMessage( 0, "Project \"%s\" isn't associated to a repository."%project.getName() )
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
            print ErrorMessage( 0, "Tool \"%s\" is not part of any project."%tool
                                 , "Cannot guess the SVN repository.")
            return
        if not project.getRepository ():
            print ErrorMessage( 0, "Project \"%s\" isn't associated to a repository."%project.getName() )
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
                print ErrorMessage( 0, "Project \"%s\" is already registered (ignored)." )
                return
        self._projects += [ project ]
        return


    def _setEnvironment ( self, systemVariable, userVariable ):
        if not self._environment.has_key(systemVariable) or self._environment[systemVariable] == "":
            if not self._environment.has_key(userVariable) or self._environment[userVariable] == "" :
                self._environment[ systemVariable ] = self._installDir
                print "[WARNING] Neither <%s> nor <%s> environment variables are sets." \
                      % (systemVariable,userVariable)
                print "          Setting <%s> to <%s>." % (systemVariable,self._installDir)
            else:
                self._environment[ systemVariable ] = self._environment[ userVariable ]
                
        if not self._quiet:
            print "Setting <%s> to <%s>." % (systemVariable,self._environment[systemVariable])
            if self._environment.has_key(userVariable):
                print "Transmitting <%s> as <%s>." % (userVariable,self._environment[userVariable])
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
                    ErrorMessage( 1, "No project of name \"%s\"."%projectName ).terminate()
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


    def enable ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_enableTool" )
        return


    def enabledTools ( self ):
        tools = []
        for project in self._projects:
            tools += project.getActives()
        return tools


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


    def svnTarball ( self, tools, projects ):
        if self._svnTag == "x":
            self._guessSvnTag ( self.getProject(projects[0]) )

        self._doSpec ()
        self._doDebChangelog ()
        
        if os.path.isdir(self._tarballDir):
            print "Removing previous tarball directory: \"%s\"." % self._tarballDir
            command = [ "/bin/rm", "-rf", self._tarballDir ]
            self._execute ( command, "Removing top export (tarball) directory" )
 
        print "Creating tarball directory: \"%s\"." % self._tarballDir
        os.makedirs ( self._tarballDir )
        self.svnExport ( tools, projects )

       # Remove unpublisheds (yet) tools/files.
        for item in self._packageExcludes:
            command = [ "/bin/rm", "-r", os.path.join(self._archiveDir,item) ]
            self._execute ( command, "rm of %s failed" % item)

       # Adds files neededs only for packaging purpose.
        command = [ "/bin/cp", "-r", os.path.join(self._sourceDir ,"bootstrap","Makefile.package")
                                   , os.path.join(self._archiveDir,"Makefile") ]
        self._execute ( command, "copy of %s failed" % "boostrap/Makefile.package")

        command = [ "/bin/cp", self._specFile, self._archiveDir ]
        self._execute ( command, "Copying RPM spec file" )

        command = [ "/bin/cp", "-r", self._debianDir, self._archiveDir ]
        self._execute ( command, "Copying Debian/Ubuntu package control files" )
 
        os.chdir ( self._archiveDir )
       #command = [ "/usr/bin/patch", "--remove-empty-files"
       #                            , "--no-backup-if-mismatch"
       #                            , "-p0", "-i", self._distribPatch ]
       #self._execute ( command, "patch for distribution command failed" )

        os.chdir ( self._tarballDir )
        command = [ "/bin/tar"
                  , "--exclude-backups"
                  , "--exclude-vcs"
                  , "-jcvf", self._sourceTarBz2, os.path.basename(self._archiveDir) ]
        self._execute ( command, "tar command failed" )
 
        print "Cleanup SVN export tarball archive directory: \"%s\"." % self._archiveDir
        command = [ "/bin/rm", "-rf", self._archiveDir ]
        self._execute ( command, "Removing archive export (tarball) directory" )

        return


    def userTarball ( self, tools, projects ):
        self.enable( tools, projects )

        userSourceTarBz2 = os.path.join ( self._tarballDir
                                        , datetime.date.today().strftime('%s-%s-%%Y%%m%%d.tar.bz2'%
                                                                         (self._packageName
                                                                         ,self._packageVersion)) )

        excludes = []
        for exclude in self._packageExcludes:
            excludes += [ '--exclude='+exclude ]

        os.chdir ( self._sourceDir )
        command = [ "/bin/tar"
                  , "--exclude-backups"
                  , "--exclude-vcs"
                  , "--transform=s,^,%s/src/,"%self._projectDir ] \
                + excludes                                        \
                + [ "-jcvf", userSourceTarBz2 ]                   \
                + self.enabledTools()
        self._execute ( command, "tar command failed" )
        
        return


    def doRpm ( self ):
        self.svnTarball ( [], self._packageProjects )

        for rpmDir in [ "SOURCES", "SPECS", "BUILD", "tmp"
                      , "SRPMS", "RPMS/i386", "RPMS/i686", "RPMS/x86_64" ]:
            rpmFullDir = os.path.join ( self._rpmbuildDir, rpmDir )
            if not os.path.isdir(rpmFullDir):
                os.makedirs ( rpmFullDir )

       #rpmSpecFile   = os.path.join ( self._rpmbuildDir, "SPECS"  , "coriolis2.spec" )
        rpmSourceFile = os.path.join ( self._rpmbuildDir, "SOURCES", self._sourceTarBz2 )
        sourceFile    = os.path.join ( self._tarballDir, self._sourceTarBz2 )

       #if os.path.isfile ( rpmSpecFile ):
       #    os.unlink ( rpmSpecFile )
       #os.symlink ( self._specFile, rpmSpecFile   )

        if not os.path.islink ( rpmSourceFile ):
            os.symlink ( sourceFile, rpmSourceFile )

        os.chdir ( self._rpmbuildDir )

        command = [ "/usr/bin/rpmbuild"
                  , "--define", "_topdir                 %s" % self._rpmbuildDir
                  , "--define", "_tmppath                %s" % self._tmppathDir
                  , "--define", "_enable_debug_packages  0"
                  , "-ta", "--with", "binarytar", rpmSourceFile ]

        self._execute ( command, "Rebuild rpm packages" )

        return


    def doDeb ( self ):
        self.svnTarball ( [], self._packageProjects )

        if not os.path.isdir(self._debbuildDir):
            os.makedirs ( self._debbuildDir )

        os.chdir ( self._debbuildDir )
        sourceFile  = os.path.join ( self._tarballDir , self._sourceTarBz2 )
        debOrigFile = os.path.join ( self._debbuildDir, "coriolis2_1.0.%s.orig.tar.bz2" % self._svnTag )
        if not os.path.islink(debOrigFile):
          os.link ( sourceFile, debOrigFile )

        command = [ "/bin/tar", "jxf", debOrigFile ]
        self._execute ( command, "Unpacking pristine sources" )

       #command = [ "/bin/cp", "-r", self._debianDir, "." ]
       #self._execute ( command, "Copying Debian/Ubuntu package control files" )

        packageDir = os.path.join ( self._debbuildDir, "coriolis2-1.0.%s" % self._svnTag )
        os.chdir ( packageDir )

        self._environment["CFLAGS"  ] = "-O2"
        self._environment["CXXFLAGS"] = "-O2"
        command = [ "/usr/bin/debuild", "-us", "-uc" ]
        self._execute ( command, "Rebuild Debian packages" )

        return


    def loadConfiguration ( self, confFile ):
        moduleGlobals = globals()

        print 'Reading configuration from:'
        print '  <%s>' % options.conf

        if not os.path.isfile(confFile):
            ErrorMessage( 1, 'Missing configuration file:', '<%s>'%confFile ).terminate()
        
        try:
            execfile( confFile, moduleGlobals )
        except Exception, e:
            ErrorMessage( 1, 'An exception occured while loading the configuration file:'
                           , '<%s>\n' % (confFile)
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
                           , '<%s>'%confFile ).terminate()

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


    def showConfiguration ( self ):
        print 'BuildCoriolis Configuration:'
        if self._svnMethod:
            print '  SVN Method: <%s>' % self._svnMethod
        else:
            print '  SVN Method not defined, will not be able to checkout/commit.'

        for project in self._projects:
            print '  project:%-15s repository:<%s>' % ( ('<%s>'%project.getName()), project.getRepository() )
            toolOrder = 1
            for tool in project.getTools():
                print '%s%02d:<%s>' % (' '*26,toolOrder,tool)
                toolOrder += 1
            print
        return


if __name__ == "__main__":

    try:
        scriptPath = os.path.abspath( os.path.dirname(sys.argv[0]) )
        print 'buildCoriolis.py is run from:'
        print '  <%s>' % scriptPath

        parser = optparse.OptionParser ()  
      # Build relateds.
        parser.add_option ( "-c", "--conf", type="string", dest="conf", default=os.path.join(scriptPath,'build.conf')
                          , help="Fichier de configuration." )
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
    
        builder = ProjectBuilder ()
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
    
        packagedProjects = [ "bootstrap", "vlsisapd", "coriolis" ]
    
        if   options.svnStatus:   builder.svnStatus   ( tools=options.tools, projects=options.projects )
        elif options.svnUpdate:   builder.svnUpdate   ( tools=options.tools, projects=options.projects )
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
