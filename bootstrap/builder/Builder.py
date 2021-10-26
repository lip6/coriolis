# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          T o o l c h a i n   B u i l d e r                      |
# |                                                                 |
# |  Author      :                   Jean-Paul Chaput               |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/Builder.py"                         |
# +-----------------------------------------------------------------+


import sys
import re
import os
import os.path
import datetime
import subprocess
from   .              import ErrorMessage
from   .Project       import Project
from   .Configuration import Configuration


class Builder:

    def __init__ ( self ):
        self._conf             = Configuration()
        self._quiet            = False
        self._rmBuild          = False
        self._doBuild          = True
        self._noCache          = False
        self._ninja            = False
        self._clang            = False
        self._noSystemBoost    = False
        self._macports         = False
        self._devtoolset       = 0
        self._llvmtoolset      = 0
        self._bfd              = "OFF"
        self._qt5              = False
        self._openmp           = False
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
        elif attribute == "clang":            self._clang            = value
        elif attribute == "macports":
            self._macports = value
            if value: self._noSystemBoost = True
        elif attribute == "devtoolset":
            self._devtoolset = value
            if value: self._noSystemBoost = True
        elif attribute == "llvmtoolset":
            self._llvmtoolset = value
        elif attribute == "bfd":              self._bfd              = value
        elif attribute == "qt5":              self._qt5              = value
        elif attribute == "openmp":           self._openmp           = value
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
        if not attribute in self.__dict__:
            raise ErrorMessage( 1, 'Builder has no attribute <%s>.'%attribute )
        return self.__dict__[attribute]

    def _guessGitHash ( self, project ):
        self.gitHash = 'x'
        os.chdir ( self.sourceDir+'/'+project.getName() )
        command = [ 'git', 'log', '--pretty=format:%h', '-n', '1']
        self.gitHash = subprocess.Popen ( command, stdout=subprocess.PIPE ).stdout.readlines()[0]
        return

    def _configure ( self, fileIn, fileOut ):
        fdFileIn  = open ( fileIn , "r" )
        fdFileOut = open ( fileOut, "w" )
        for line in fdFileIn.readlines():
            stable       = False
            substituted0 = line
            while not stable:
                substituted1 = re.sub ( r"@revdate@"    , self.revDate, substituted0 )
                substituted1 = re.sub ( r"@githash@"    , self.gitHash, substituted1 )
                substituted1 = re.sub ( r"@coriolisTop@", "/usr"      , substituted1 )
                if substituted0 == substituted1: stable = True
                else: substituted0 = substituted1
            fdFileOut.write ( substituted0 )
        fdFileIn.close ()
        fdFileOut.close ()
        return

    def _doSpec ( self ):
        self._configure ( self.specFileIn, self.specFile )
        return

    def _doDebChangelog ( self ):
        self._configure ( self.debChangelogIn, self.debChangelog )
        return

    def _execute ( self, command, error ):
        collections = []
        if self._devtoolset:
            collections.append( 'devtoolset-{}'.format(self._devtoolset) )
            print( 'Using devtoolset-{0} (scl enable devtoolset-{0} ...)'.format(self._devtoolset) )
        if self._llvmtoolset:
            collections.append( 'llvm-toolset-{}'.format(self._llvmtoolset) )
            print( 'Using llvm-toolset-{0} (scl enable llvm-toolset-{v} ...)'.format(self._llvmtoolset) )
        if collections:
            commandAsString = ''
            for i in range(len(command)):
                if i: commandAsString += ' '
                if ' ' in command[i]: commandAsString += '"'+command[i]+'"'
                else:                 commandAsString += command[i]
            command = [ 'scl', 'enable' ]
            command += collections
            command.append( commandAsString )
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
        toolSourceDir   = os.path.join ( self.sourceDir, tool.getToolDir() )
        toolBuildDir    = os.path.join ( self.buildDir , tool.name )
        cmakeInstallDir = os.path.join ( self.installDir, "share", "cmake", "Modules" )
       # Supplied directly in the CMakeLists.txt.
       #cmakeModules  = os.path.join ( self.installDir, "share", "cmake", "Modules" )
        if not os.path.isdir(toolSourceDir):
            print( ErrorMessage( 0, 'Missing tool source directory: "{}" (skipped).' \
                                    .format(toolSourceDir) ))
            return
        if self._rmBuild:
            print( 'Removing tool build directory: "{}".'.format(toolBuildDir) )
            command = [ "/bin/rm", "-rf", toolBuildDir ]
            self._execute ( command, "Removing tool build directory" )

        command = [ 'cmake' ]
        if self.libSuffix:      command += [ "-D", "LIB_SUFFIX:STRING=%s" % self.libSuffix ]
        if self._ninja:         command += [ "-G", "Ninja" ]
        if self._macports:      command += [ "-D", "WITH_MACPORTS:STRING=TRUE" ]
        if self._noSystemBoost: command += [ "-D", "Boost_NO_SYSTEM_PATHS:STRING=TRUE"
                                          #, "-D", "BOOST_INCLUDEDIR:STRING=/usr/include/boost169"
                                          #, "-D", "BOOST_LIBRARYDIR:STRING=/usr/lib64/boost169"
                                           ]
        if self._bfd:           command += [ "-D", "USE_LIBBFD:STRING=%s" % self._bfd ]
        if self._qt5:           command += [ "-D", "WITH_QT5:STRING=TRUE" ]
        if self._openmp:        command += [ "-D", "WITH_OPENMP:STRING=TRUE" ]
        command += [ "-D", "CMAKE_BUILD_TYPE:STRING=%s"     % self.buildMode
                  #, "-D", "BUILD_SHARED_LIBS:STRING=%s"    % self.enableShared
                   , "-D", "CMAKE_INSTALL_PREFIX:STRING=%s" % self.installDir
                   , "-D", "CMAKE_INSTALL_DIR:STRING=%s"    % cmakeInstallDir
                  #, "-D", "CMAKE_MODULE_PATH:STRING=%s"    % cmakeModules
                  #, "-D", "Boost_DEBUG:STRING=TRUE"
                   , toolSourceDir ]

        if not os.path.isdir(toolBuildDir):
            print( 'Creating tool build directory: "{}".'.format(toolBuildDir) )
            os.makedirs ( toolBuildDir )
            os.chdir    ( toolBuildDir )
            self._execute ( command, "First CMake failed" )
        os.chdir ( toolBuildDir )
        if self._noCache:
            cmakeCache = os.path.join(toolBuildDir,"CMakeCache.txt")
            if os.path.isfile ( cmakeCache ): os.unlink ( cmakeCache )

        command += [ "-D", "BUILD_DOC:STRING=%s"              % self._enableDoc
                   , "-D", "CMAKE_VERBOSE_MAKEFILE:STRING=%s" % self._verboseMakefile
                   , "-D", "CMAKE_INSTALL_PREFIX:STRING=%s"   % self.installDir
                   , "-D", "CMAKE_INSTALL_DIR:STRING=%s"      % cmakeInstallDir
                   ]
        if self.libSuffix:                 command += [ "-D", "LIB_SUFFIX:STRING=%s"        % self.libSuffix ]
        if self._checkDatabase    == 'ON': command += [ "-D", "CHECK_DATABASE:STRING=ON"    ]
        if self._checkDeterminism == 'ON': command += [ "-D", "CHECK_DETERMINISM:STRING=ON" ]
        command += [ toolSourceDir ]

        self._execute ( command, "Second CMake failed" )
        if self._doBuild:
            command  = [ "make" ]
            if self._ninja:
                command  = [ "ninja-build" ]
           #command += [ "DESTDIR=%s" % self.installDir ]
            command += self._makeArguments
            print( "Make/Ninja command:", command )
            sys.stdout.flush ()
            self._execute ( command, "Build failed" )
        return

    def gitArchive ( self, projectName ):
        rawArchive = self.tarballDir+'/'+projectName+'.tar'
        os.chdir ( self.sourceDir+'/'+projectName )
        command = [ 'git'
                  , 'archive'
                  , '--prefix=%s/' % projectName
                  , '--output=%s'  % rawArchive
                  , 'devel'
                  ]
        self._execute ( command, "git archive of project %s" % projectName )
        if not os.path.isdir ( self.archiveDir ):
            os.mkdir ( self.archiveDir )
        os.chdir ( self.archiveDir )

        command = [ 'tar', 'xf', rawArchive ]
        self._execute ( command, "unpacking raw archive %s" % rawArchive )

        command = [ 'rm', rawArchive ]
        self._execute ( command, "Removing raw archive %s" % rawArchive )

       # Adds files neededs only for packaging purpose.
        command = [ "/bin/ln", "-s", "./coriolis/bootstrap/Makefile.package"
                                   , self.archiveDir+"/Makefile" ]
        self._execute ( command, "link of %s failed" % "coriolis/boostrap/Makefile.package")

        command = [ "/bin/ln", "-s", "./coriolis/bootstrap/debian", self.archiveDir ]
        self._execute ( command, "Copying Debian/Ubuntu package control files" )

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
   #    os.chdir ( self.archiveDir )
   #    command = [ "/usr/bin/patch", "--remove-empty-files"
   #                                , "--no-backup-if-mismatch"
   #                                , "-p0", "-i", self.distribPatch ]
   #    self._execute ( command, "patch for distribution command failed" )
        absSourceTarBz2 = '%s/%s' % (self.tarballDir,self.sourceTarBz2)
        os.chdir ( self.tarballDir )
        command = [ 'tar', 'jcf', absSourceTarBz2, os.path.basename(self.archiveDir) ]
        self._execute ( command, "Creating composite archive %s" % absSourceTarBz2 )
        return

    def _setEnvironment ( self, systemVariable, userVariable ):
        if not systemVariable in self._environment or self._environment[systemVariable] == "":
            if not userVariable in self._environment or self._environment[userVariable] == "" :
                self._environment[ systemVariable ] = self.installDir
                print( '[WARNING] Neither "{0}" nor "{1}" environment variables are sets.' \
                       .format(systemVariable,userVariable) )
                print( '          Setting "{0}" to "{1}".'.format(systemVariable,self.installDir) )
            else:
                self._environment[ systemVariable ] = self._environment[ userVariable ]
        if not self._quiet:
            print( 'Setting "{0}" to "{1}".'.format(systemVariable,self._environment[systemVariable]) )
            if userVariable in self._environment:
                print( 'Transmitting "{0}" as "{1}".'.format(userVariable,self._environment[userVariable]) )
        return

    def _commandTemplate ( self, tools, projects, command ):
        if self._clang:
            self._environment[ 'CC'  ] = 'clang'
            self._environment[ 'CXX' ] = 'clang++'
        if self._devtoolset:
            self._environment[ 'BOOST_INCLUDEDIR' ] = '/opt/rh/devtoolset-%d/root/usr/include' % self._devtoolset
            self._environment[ 'BOOST_LIBRARYDIR' ] = '/opt/rh/devtoolset-%d/root/usr/lib'     % self._devtoolset
        if self._macports:
            self._environment[ 'BOOST_INCLUDEDIR' ] = '/opt/local/include'
            self._environment[ 'BOOST_LIBRARYDIR' ] = '/opt/local/lib'
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
                print( '[WARNING] Tool "{}" is not part of any project.'.format(tool) )
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
                print( '\nProcessing tool: "{}".'.format(tool.name) )
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

    def gitTarball ( self, tools, projects ):
        if self.gitHash == "x":
            self._guessGitHash ( self.getProject(projects[0]) )
        self._doSpec ()
   #    self._doDebChangelog ()
        if os.path.isdir(self.tarballDir):
            print( 'Removing previous tarball directory: "{}".'.format(self.tarballDir) )
            command = [ "/bin/rm", "-rf", self.tarballDir ]
            self._execute ( command, "Removing top export (tarball) directory" )
        print( 'Creating tarball directory: "{}".'.format(self.tarballDir) )
        os.makedirs ( self.tarballDir )
        self.gitArchive ( projects[0] )
        return

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
        self.gitTarball ( [], self.packageProjects )
        for rpmDir in [ "SOURCES", "SPECS", "BUILD", "tmp"
                      , "SRPMS", "RPMS/i386", "RPMS/i686", "RPMS/x86_64" ]:
            rpmFullDir = os.path.join ( self.rpmbuildDir, rpmDir )
            if not os.path.isdir(rpmFullDir):
                os.makedirs ( rpmFullDir )
            else:
                for entry in os.listdir(rpmFullDir):
                    path = os.path.join( rpmFullDir, entry )
                    if os.path.islink(path):
                        realpath = os.path.realpath( os.readlink(path) )
                        if not os.path.isfile(realpath):
                            print( 'Remove obsolete link: "{}".'.format(path) )
                            os.unlink( path )
        rpmSpecFile   = os.path.join ( self.rpmbuildDir, "SPECS"  , "coriolis2.spec" )
        rpmSourceFile = os.path.join ( self.rpmbuildDir, "SOURCES", self.sourceTarBz2 )
        sourceFile    = os.path.join ( self.tarballDir , self.sourceTarBz2 )

        if os.path.isfile ( rpmSpecFile ):
            os.unlink ( rpmSpecFile )
        os.symlink ( self.specFile, rpmSpecFile   )
        if not os.path.islink ( rpmSourceFile ):
            os.symlink ( sourceFile, rpmSourceFile )

        os.chdir ( self.rpmbuildDir )
        command = [ "/usr/bin/rpmbuild"
                  , "--define", "_topdir                 %s" % self.rpmbuildDir
                  , "--define", "_tmppath                %s" % self.tmppathDir
                 #, "--define", "_enable_debug_packages  0"
                  , "--with", "binarytar" ]
        if self._devtoolset:
            command += [ "--define", "scl devtoolset-%d"%self._devtoolset ]
        command += [ "-ba", "--clean", rpmSpecFile ]
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
