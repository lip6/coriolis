import sys

class BuildConfigurationFileReader:
  def __init__(self):
    src_dir=""
    self.sources = ""
    self.include_dirs = ""
    self.library_dirs = ""
  def load(self):
    f = open("PyExtensionBuild.conf")
    try:
      for line in f:
        if (line == "\n"):
          continue
        args = line.split("=", 1)
        if (len(args) > 0):
          if (len(args) != 2):
            print "Error in configuration file"
            print args
            sys.exit(55)
          if (args[0]=="src_dir"):
            self.src_dir = args[1].strip("\n")
          if (args[0]=="sources"):
            self.sources = args[1].strip("\n").split(";")
            self.sources = map(lambda s : self.src_dir + "/" + s, self.sources)
          elif (args[0]=="include_dirs"):
            self.include_dirs=[args[1].strip("\n")]
            print self.include_dirs
          elif (args[0]=="library_dirs"):
            self.library_dirs=args[1].strip("\n").split(":")
    finally:
      f.close()
    if (len(self.sources)==0 or len(self.include_dirs)==0 or len(self.library_dirs)==0):
      print "Error in configuration file"
      print self.sources
      print self.include_dirs
      print self.library_dirs
      sys.exit(55)

class PyBuilder:
  def __init__(self, sources, library_dirs, include_dirs):
    self.sources = sources
    self.libraryDirs = library_dirs
    self.libraries = ['isobar', 'hurricane']
    self.includeDirs = include_dirs
  def build(self, build=False):
    from distutils.core import setup, Extension
    hurricane = Extension('Hurricane',
      include_dirs=self.includeDirs,
      libraries=self.libraries,
      library_dirs=self.libraryDirs,
      sources=self.sources,
      define_macros=[('__PYTHON_MODULE__', '1')])

    build = ('build' in sys.argv)

    if (build):
      setup(name='Hurricane',
        version='2.0',
        description = "Python Module for Hurricane",
        author='Coriolis Team',
        ext_modules=[hurricane])

if (__name__=='__main__'):
  r = BuildConfigurationFileReader()
  r.load()
  b = PyBuilder(r.sources, r.library_dirs, r.include_dirs)
  build = ('build' in sys.argv)
  b.build(build)
