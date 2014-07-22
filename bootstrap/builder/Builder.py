# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                   Jean-Paul Chaput               |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/Builder.py"                         |
# +-----------------------------------------------------------------+


import sys
import re
import os
import os.path
import datetime
import subprocess
from   .             import ErrorMessage
from   Project       import Project
from   Configuration import Configuration


class Builder:

    def __init__ ( self ):
        self._conf             = Configuration()
        self._quiet            = False
        self._rmBuild          = False
        self._doBuild          = True
        self._noCache          = False
        self._ninja            = False
        self._devtoolset2      = False
        self._qt5              = False
        self._enableShared     = "ON"
        self._enableDoc        = "OFF"
        self._checkDatabase    = "OFF"
        self._checkDeterminism = "OFF"
        self._verboseMakefile  = "OFF"
        self._makeArguments    = []
        self._environment      = os.environ
        return


    def __setattr__ ( self, attribute, value ):
        if attribute[0] == "_":
            self.__dict__[attribute] = value
            return

        if attribute in self._conf.getAllIds(): setattr( self._conf, attribute, value )
        
        if   attribute == "quiet":            self._quiet            = value
        elif attribute == "rmBuild":          self._rmBuild          = value
        elif attribute == "doBuild":          self._doBuild          = value
        elif attribute == "noCache":          self._noCache          = value
        elif attribute == "ninja":            self._ninja            = value
        elif attribute == "devtoolset2":      self._devtoolset2      = value
        elif attribute == "qt5":              self._qt5              = value
        elif attribute == "enableDoc":        self._enableDoc        = value
        elif attribute == "enableShared":     self._enableShared     = value
        elif attribute == "checkDatabase":    self._checkDatabase    = value
        elif attribute == "checkDeterminism": self._checkDeterminism = value
        elif attribute == "verboseMakefile":  self._verboseMakefile  = value
        elif attribute == "makeArguments":    self._makeArguments    = value.split ()
        return


    def __getattr__ ( self, attribute ):
        if attribute[0] != "_":
            if attribute == 'conf': return self._conf
            if attribute in self._conf.getAllIds():
                return getattr( self._conf, attribute )

        if not self.__dict__.has_key(attribute):
            raise ErrorMessage( 1, 'Builder has no attribute <%s>.'%attribute )
        return self.__dict__[attribute]


   #def _guessGitHash ( self, project ):
   #    revisionPattern = re.compile ( r"^Revision:\s*(?P<revision>\d+)" )
   #    projectSvnDir   = os.path.join ( self.svnMethod+project.getRepository() )
   #
   #    os.chdir(  )
   #    command = [ 'git', 'log', '--pretty="%h"', '-n', '1']
   #    svnInfo = subprocess.Popen ( command, stdout=subprocess.PIPE )
   #
   #    for line in svnInfo.stdout.readlines():
   #        m = revisionPattern.match ( line )
   #        if m:
   #            self.gitHash = m.group("revision")
   #            print "Latest revision of project %s is %s." % (project.getName(),self.gitHash)
   #            return
   #
   #    print "[WARNING] Cannot guess revision for project \"%s\"." % project.getName() 
   #    print "          (using: \"x\")"
   #    return


    def _configure ( self, fileIn, file ):
        fdFileIn = open ( fileIn, "r" )
        fdFile   = open ( file  , "w" )

        for line in fdFileIn.readlines():
            stable       = False
            substituted0 = line

            while not stable:
                substituted1 = re.sub ( r"@svntag@"     , self.gitHash, substituted0 )
                substituted1 = re.sub ( r"@coriolisTop@", "/opt/coriolis2" , substituted1 )
                if substituted0 == substituted1: stable = True
                else: substituted0 = substituted1

            fdFile.write ( substituted0 )

        fdFileIn.close ()
        fdFile.close ()
        return


    def _doSpec ( self ):
        self._configure ( self.specFileIn, self.specFile )
        return


    def _doDebChangelog ( self ):
        self._configure ( self.debChangelogIn, self.debChangelog )
        return


    def _execute ( self, command, error ):
        if self._devtoolset2 == True:
            print 'Using devtoolset-2 (scl enable devtoolset-2 ...)'
            commandAsString = ''
            for i in range(len(command)):
                if i: commandAsString += ' '
                commandAsString += command[i]
            command = [ 'scl', 'enable', 'devtoolset-2', commandAsString ]

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
        toolSourceDir = os.path.join ( self.sourceDir, tool.getToolDir() )
        toolBuildDir  = os.path.join ( self.buildDir , tool.name )
       # Supplied directly in the CMakeLists.txt.
       #cmakeModules  = os.path.join ( self.installDir, "share", "cmake", "Modules" )

        if not os.path.isdir(toolSourceDir):
            print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir )
            return

        if self._rmBuild:
            print "Removing tool build directory: \"%s\"." % toolBuildDir
            command = [ "/bin/rm", "-rf", toolBuildDir ]
            self._execute ( command, "Removing tool build directory" )

        command = [ 'cmake' ]
        if self._ninja:       command += [ "-G", "Ninja" ]
        if self._devtoolset2: command += [ "-D", "Boost_NO_SYSTEM_PATHS:STRING=TRUE" ]
        if self._qt5:         command += [ "-D", "WITH_QT5:STRING=TRUE" ]

        command += [ "-D", "CMAKE_BUILD_TYPE:STRING=%s"  % self.buildMode
                   , "-D", "BUILD_SHARED_LIBS:STRING=%s" % self.enableShared
                  #, "-D", "CMAKE_MODULE_PATH:STRING=%s" % cmakeModules
                   , toolSourceDir ]
            
        if not os.path.isdir(toolBuildDir):
            print "Creating tool build directory: \"%s\"." % toolBuildDir
            os.makedirs ( toolBuildDir )
            os.chdir    ( toolBuildDir )
            self._execute ( command, "First CMake failed" )

        os.chdir ( toolBuildDir )
        if self._noCache:
            cmakeCache = os.path.join(toolBuildDir,"CMakeCache.txt")
            if os.path.isfile ( cmakeCache ): os.unlink ( cmakeCache )

        command += [ "-D", "BUILD_DOC:STRING=%s"              % self._enableDoc
                   , "-D", "CHECK_DATABASE:STRING=%s"         % self._checkDatabase
                   , "-D", "CHECK_DETERMINISM:STRING=%s"      % self._checkDeterminism
                   , "-D", "CMAKE_VERBOSE_MAKEFILE:STRING=%s" % self._verboseMakefile
                   , "-D", "CMAKE_INSTALL_PREFIX:STRING=%s"   % self.installDir
                   ]
        if self.libSuffix:
            command += [ "-D", "LIB_SUFFIX:STRING=%s" % self.libSuffix ]
        command += [ toolSourceDir ]

        self._execute ( command, "Second CMake failed" )

        if self._doBuild:
            command  = [ "make" ]
            if self._ninja:
                command  = [ "ninja-build" ]
           #command += [ "DESTDIR=%s" % self.installDir ]
            if self._enableDoc == "ON":
               #if tool.name == "crlcore" or tool.name == "stratus1":
                if tool.name == "stratus1":
                    command += [ "dvi", "safepdf", "html" ]
            command += self._makeArguments
            print "Make/Ninja command:", command
            sys.stdout.flush ()
            self._execute ( command, "Build failed" )
        return


   #def _svnStatus ( self, tool ):
   #    toolSourceDir = os.path.join ( self.sourceDir , tool )
   #    if not os.path.isdir(toolSourceDir):
   #        if not self._quiet:
   #            print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir )
   #        return
   #    os.chdir ( toolSourceDir )
   #
   #    print "Checking SVN status of tool: ", tool
   #    command = [ "svn", "status", "-u", "-q" ]
   #    self._execute ( command, "svn status -u -q" )
   #    print
   #    return
   #
   #
   #def _svnDiff ( self, tool ):
   #    toolSourceDir = os.path.join ( self.sourceDir , tool )
   #    if not os.path.isdir(toolSourceDir):
   #        if not self._quiet:
   #            print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir )
   #        return
   #    os.chdir ( toolSourceDir )
   #
   #    print "Doing a SVN diff of tool: ", tool
   #    command = [ "svn", "diff" ]
   #    if self.gitHash != "x":
   #        command += [ "--revision", self.gitHash ]
   #    self._execute ( command, "svn diff %s" % tool )
   #    print
   #    return
   #
   #
   #def _svnUpdate ( self, tool ):
   #    toolSourceDir = os.path.join ( self.sourceDir , tool )
   #    if not os.path.isdir(toolSourceDir):
   #        if not self._quiet:
   #            print ErrorMessage( 0, "Missing tool source directory: \"%s\" (skipped)."%toolSourceDir)
   #        return
   #    os.chdir ( toolSourceDir )
   #
   #    print "Doing a SVN update of tool: ", tool
   #    command = [ "svn", "update" ]
   #    self._execute ( command, "svn update" )
   #    print
   #    return
   #
   #
   #def _svnCheckout ( self, tool ):
   #    project = self._conf.getToolProject ( tool )
   #    if not project:
   #        print ErrorMessage( 0, "Tool \"%s\" is not part of any project."%tool
   #                             ,"Cannot guess the SVN repository." )
   #        return
   #    if not project.getRepository ():
   #        print ErrorMessage( 0, "Project \"%s\" isn't associated to a repository."%project.getName() )
   #        return
   #
   #    if not os.path.isdir(self.sourceDir):
   #        print ErrorMessage( 0, "Source directory <%s> doesn't exists. Creating."%self.sourceDir )
   #        os.makedirs( self.sourceDir )
   #    
   #    toolSvnTrunkDir = os.path.join ( self.svnMethod+project.getRepository(), tool, "trunk" )
   #    os.chdir ( self.sourceDir )
   #
   #    print "Doing a SVN checkout of tool: ", tool
   #    command = [ "svn", "co", toolSvnTrunkDir, tool ]
   #    if self.gitHash != "x":
   #        command += [ "--revision", self.gitHash ]
   #    self._execute ( command, "svn checkout %s" % tool )
   #    print
   #    return
   #
   #
   #def _svnExport ( self, tool ):
   #    project = self._conf.getToolProject ( tool )
   #    if not project:
   #        print ErrorMessage( 0, "Tool \"%s\" is not part of any project."%tool
   #                             , "Cannot guess the SVN repository.")
   #        return
   #    if not project.getRepository ():
   #        print ErrorMessage( 0, "Project \"%s\" isn't associated to a repository."%project.getName() )
   #        return
   #    
   #    toolSvnTrunkDir = os.path.join ( self.svnMethod+project.getRepository(), tool, "trunk" )
   #
   #    if not os.path.isdir ( self.archiveDir ):
   #        os.mkdir ( self.archiveDir )
   #    os.chdir ( self.archiveDir )
   #
   #    toolExportDir = os.path.join ( self.archiveDir, tool )
   #    if os.path.isdir ( toolExportDir ):
   #        print "Removing tool export (tarball) directory: \"%s\"." % toolExportDir
   #        command = [ "/bin/rm", "-r", toolExportDir ]
   #        self._execute ( command, "Removing tool export (tarball) directory" )
   #
   #    print "Doing a SVN export of tool: ", tool
   #    command = [ "svn", "export", toolSvnTrunkDir, toolExportDir ]
   #    if self.gitHash != "x":
   #        command += [ "--revision", self.gitHash ]
   #    self._execute ( command, "svn export %s" % toolExportDir )
   #    print
   #    return


    def _setEnvironment ( self, systemVariable, userVariable ):
        if not self._environment.has_key(systemVariable) or self._environment[systemVariable] == "":
            if not self._environment.has_key(userVariable) or self._environment[userVariable] == "" :
                self._environment[ systemVariable ] = self.installDir
                print "[WARNING] Neither <%s> nor <%s> environment variables are sets." \
                      % (systemVariable,userVariable)
                print "          Setting <%s> to <%s>." % (systemVariable,self.installDir)
            else:
                self._environment[ systemVariable ] = self._environment[ userVariable ]
                
        if not self._quiet:
            print "Setting <%s> to <%s>." % (systemVariable,self._environment[systemVariable])
            if self._environment.has_key(userVariable):
                print "Transmitting <%s> as <%s>." % (userVariable,self._environment[userVariable])
        return


    def _commandTemplate ( self, tools, projects, command ):
        if self._devtoolset2:
            self._environment[ 'BOOST_INCLUDEDIR' ] = '/opt/rh/devtoolset-2/root/usr/include'
            self._environment[ 'BOOST_LIBRARYDIR' ] = '/opt/rh/devtoolset-2/root/usr/lib'

       # Set or guess the various projects TOP environment variables.
        for project in self.projects:
            topVariable     = "%s_TOP"      % project.getName().upper()
            topUserVariable = "%s_USER_TOP" % project.getName().upper()
            self._setEnvironment ( topVariable, topUserVariable )

        if tools:
           # Checks if the requested tools are in the various projects.
            self.standalones = tools
            for project in self.projects:
                self.standalones = project.activate ( self.standalones ) 
            for tool in self.standalones:
                print "[WARNING] Tool \"%s\" is not part of any project." % tool

        if projects:
            for projectName in projects:
                project = self.getProject ( projectName )
                if not project:
                    ErrorMessage( 1, "No project of name \"%s\"."%projectName ).terminate()
                project.activateAll()

        if not tools and not projects:
            for project in self.projects:
                project.activateAll ()

        for project in self.projects:
            for tool in project.getActives():
                print "\nProcessing tool: \"%s\"." % tool.name
                getattr(self,command) ( tool )
        return


    def enable ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_enableTool" )
        return


    def enabledTools ( self ):
        tools = []
        for project in self.projects:
            tools += project.getActives()
        return tools


    def build ( self, tools, projects ):
        self._commandTemplate ( tools, projects, "_build" )
        return


   #def svnStatus ( self, tools, projects ):
   #    self._commandTemplate ( tools, projects, "_svnStatus" )
   #    return
   #
   #
   #def svnUpdate ( self, tools, projects ):
   #    self._commandTemplate ( tools, projects, "_svnUpdate" )
   #    return
   #
   #
   #def svnCheckout ( self, tools, projects ):
   #    self._commandTemplate ( tools, projects, "_svnCheckout" )
   #    return
   #
   #
   #def svnDiff ( self, tools, projects ):
   #    self._commandTemplate ( tools, projects, "_svnDiff" )
   #    return
   #
   #
   #def svnExport ( self, tools, projects ):
   #    self._commandTemplate ( tools, projects, "_svnExport" )
   #    return
   #
   #
   #def svnTarball ( self, tools, projects ):
   #    if self.gitHash == "x":
   #        self._guessGitHash ( self.getProject(projects[0]) )
   #
   #    self._doSpec ()
   #    self._doDebChangelog ()
   #    
   #    if os.path.isdir(self.tarballDir):
   #        print "Removing previous tarball directory: \"%s\"." % self.tarballDir
   #        command = [ "/bin/rm", "-rf", self.tarballDir ]
   #        self._execute ( command, "Removing top export (tarball) directory" )
   #
   #    print "Creating tarball directory: \"%s\"." % self.tarballDir
   #    os.makedirs ( self.tarballDir )
   #    self.svnExport ( tools, projects )
   #
   #   # Remove unpublisheds (yet) tools/files.
   #    for item in self.packageExcludes:
   #        command = [ "/bin/rm", "-rf", os.path.join(self.archiveDir,item) ]
   #        self._execute ( command, "rm of %s failed" % item)
   #
   #   # Adds files neededs only for packaging purpose.
   #    command = [ "/bin/cp", "-r", os.path.join(self.sourceDir ,"bootstrap","Makefile.package")
   #                               , os.path.join(self.archiveDir,"Makefile") ]
   #    self._execute ( command, "copy of %s failed" % "boostrap/Makefile.package")
   #
   #    command = [ "/bin/cp", self.specFile, self.archiveDir ]
   #    self._execute ( command, "Copying RPM spec file" )
   #
   #    command = [ "/bin/cp", "-r", self.debianDir, self.archiveDir ]
   #    self._execute ( command, "Copying Debian/Ubuntu package control files" )
   #
   #    os.chdir ( self.archiveDir )
   #   #command = [ "/usr/bin/patch", "--remove-empty-files"
   #   #                            , "--no-backup-if-mismatch"
   #   #                            , "-p0", "-i", self.distribPatch ]
   #   #self._execute ( command, "patch for distribution command failed" )
   #
   #    os.chdir ( self.tarballDir )
   #    command = [ "/bin/tar"
   #              , "--exclude-backups"
   #              , "--exclude-vcs"
   #              , "-jcvf", self.sourceTarBz2, os.path.basename(self.archiveDir) ]
   #    self._execute ( command, "tar command failed" )
   #
   #    print "Cleanup SVN export tarball archive directory: \"%s\"." % self.archiveDir
   #    command = [ "/bin/rm", "-rf", self.archiveDir ]
   #    self._execute ( command, "Removing archive export (tarball) directory" )
   #
   #    return


    def userTarball ( self, tools, projects ):
        self.enable( tools, projects )

        userSourceTarBz2 = os.path.join ( self.tarballDir
                                        , datetime.date.today().strftime('%s-%s-%%Y%%m%%d.tar.bz2'%
                                                                         (self.packageName
                                                                         ,self.packageVersion)) )

        excludes = []
        for exclude in self.packageExcludes:
            excludes += [ '--exclude='+exclude ]

        os.chdir ( self.sourceDir )
        command = [ "/bin/tar"
                  , "--exclude-backups"
                  , "--exclude-vcs"
                  , "--transform=s,^,%s/src/,"%self.projectDir ] \
                + excludes                                        \
                + [ "-jcvf", userSourceTarBz2 ]                   \
                + self.enabledTools()
        self._execute ( command, "tar command failed" )
        
        return


    def doRpm ( self ):
        self.svnTarball ( [], self.packageProjects )

        for rpmDir in [ "SOURCES", "SPECS", "BUILD", "tmp"
                      , "SRPMS", "RPMS/i386", "RPMS/i686", "RPMS/x86_64" ]:
            rpmFullDir = os.path.join ( self.rpmbuildDir, rpmDir )
            if not os.path.isdir(rpmFullDir):
                os.makedirs ( rpmFullDir )

       #rpmSpecFile   = os.path.join ( self.rpmbuildDir, "SPECS"  , "coriolis2.spec" )
        rpmSourceFile = os.path.join ( self.rpmbuildDir, "SOURCES", self.sourceTarBz2 )
        sourceFile    = os.path.join ( self.tarballDir, self.sourceTarBz2 )

       #if os.path.isfile ( rpmSpecFile ):
       #    os.unlink ( rpmSpecFile )
       #os.symlink ( self.specFile, rpmSpecFile   )

        if not os.path.islink ( rpmSourceFile ):
            os.symlink ( sourceFile, rpmSourceFile )

        os.chdir ( self.rpmbuildDir )

        command = [ "/usr/bin/rpmbuild"
                  , "--define", "_topdir                 %s" % self.rpmbuildDir
                  , "--define", "_tmppath                %s" % self.tmppathDir
                  , "--define", "_enable_debug_packages  0"
                  , "-ta", "--with", "binarytar", rpmSourceFile ]

        self._execute ( command, "Rebuild rpm packages" )

        return


    def doDeb ( self ):
        self.svnTarball ( [], self.packageProjects )

        if not os.path.isdir(self.debbuildDir):
            os.makedirs ( self.debbuildDir )

        os.chdir ( self.debbuildDir )
        sourceFile  = os.path.join ( self.tarballDir , self.sourceTarBz2 )
        debOrigFile = os.path.join ( self.debbuildDir, "coriolis2_1.0.%s.orig.tar.bz2" % self.gitHash )
        if not os.path.islink(debOrigFile):
          os.link ( sourceFile, debOrigFile )

        command = [ "/bin/tar", "jxf", debOrigFile ]
        self._execute ( command, "Unpacking pristine sources" )

       #command = [ "/bin/cp", "-r", self.debianDir, "." ]
       #self._execute ( command, "Copying Debian/Ubuntu package control files" )

        packageDir = os.path.join ( self.debbuildDir, "coriolis2-1.0.%s" % self.gitHash )
        os.chdir ( packageDir )

        self._environment["CFLAGS"  ] = "-O2"
        self._environment["CXXFLAGS"] = "-O2"
        command = [ "/usr/bin/debuild", "-us", "-uc" ]
        self._execute ( command, "Rebuild Debian packages" )

        return


    def getProject        ( self, name     ): return self._conf.getProject(name)
    def loadConfiguration ( self, confFile ): self._conf.load( confFile )
    def showConfiguration ( self ):           self._conf.show()


