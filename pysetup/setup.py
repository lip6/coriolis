import os, sys
import setuptools

def find_coriolis_install(root=None):
    installs=[]
    if root is None:
        root=os.path.join(os.environ['HOME'],'coriolis-2.x')
    for r,d,f in os.walk(root):
        if "install" in d:
            installs.append(os.path.join(r,'install'))
    if not installs:
        print("Error: Coriolis build not found.")
        sys.exit(-1)
    if len(installs)>1:
        print("Error: Found several Coriolis builds. Work only with one.")
        sys.exit(-2)
    return installs[0]

VERSION = "2.4.0"

setup_path=os.path.dirname(os.path.realpath(__file__))
coriolis_install_path=find_coriolis_install()
coriolis_bin_path=os.path.join(coriolis_install_path,'bin')
for name in ["lib64","lib"]:
    coriolis_lib_path=os.path.join(coriolis_install_path,'lib64')
    if os.path.isdir(coriolis_lib_path):
        break
    coriolis_lib_path=None
if not coriolis_lib_path:
    print("Error: lib64 or lib directory not found in Coriolis build.")
    sys.exit(-3)
coriolis_python_path=None
for r,d,f in os.walk(coriolis_lib_path):
    if "coriolis" in d:
        coriolis_python_path=r
        break
if not coriolis_python_path:
    print("Error: 'coriolis' python module not found in Coriolis build.")
    sys.exit(-4)

#for name in (setup_path,coriolis_install_path,coriolis_bin_path,coriolis_lib_path,coriolis_python_path):
#    print(name)

def _add_preload_code(path,libs):
    with open(path,"rt") as f:
        code=f.read().strip()
    if not code.startswith("# preload"):
        with open(path,"wt") as f:
            f.write(
"""# preload shared libraries
import os, ctypes

__libs=[]
__lib_folder=os.path.join(os.path.dirname(os.path.realpath(__file__)),'libs')
for name in (
""")
            for lib in libs:
                lib=lib.strip()
                if not lib:
                    continue
                f.write(f'    "{lib}",\n')
            f.write(
"""
    ):
    __libs.append(ctypes.CDLL(os.path.join(__lib_folder,name)))
""")
            f.write("\n")
            f.write(code)
            f.write("\n")
    
#_add_preload_code(os.path.join(setup_path,"coriolis_cmds","__init__.py"),
#"""
#libdef.so
#libpyflute.so
#libcoloquinte.so
#libhurricane.so
#libutils.so
#libpycrlconst.so
#libconfiguration.so
#libpytypemanager.so
#libflute.so
#liblef.so
#libdefzlib.so
#libisobar.so
#libintervalTree.so
#libviewer.so
#liblefzlib.so
#libanalog.so
#libcrlcore.so
#libtutorial.so
#libequinox.so
#libpycrlcore.so
#liblibmanager.so
#libetesian.so
#libsolstice.so
#libunicorn.so
#libanabatic.so
#libkatana.so
#libpyanabatic.so
#libbora.so
#libpybora.so
#""".split())

import sys, ctypes

def _files(folder):
    files=[]
    for f in os.listdir(folder):
        path=os.path.join(folder,f)
        if os.path.isfile(path):
            files.append(path)
    return files

def _print_ordered_preload_libs():
    libs=_files(coriolis_lib_path)
    libs_ordered=[]
    __libs=[]
    while libs:
        name=libs.pop(0)
        if not name.endswith(".so"):
            continue
        try:
            lib=ctypes.CDLL(name)
            __libs.append(lib)
            libs_ordered.append(name)
        except OSError:
            libs.append(name)
    for path in libs_ordered:
        print(f'    "{os.path.split(path)[-1]}",')

#_print_ordered_preload_libs()

def _coriolis_packages():
    def _packages(prefix,folder):
        root=prefix+[folder]
        yield '.'.join(root)
        for f in os.listdir(os.path.join(coriolis_python_path,*root)):
            if f=="__pycache__":
                continue
            path=os.path.join(coriolis_python_path,*root,f)
            if os.path.isdir(path):
                yield from _packages(root,f)
    yield from _packages([],"coriolis")

with open(os.path.join(coriolis_python_path,"coriolis","__init__.py"),"wt") as f:
    f.write("from .cmds import *\n")

setuptools.setup(
   name = "coriolis",
   version = VERSION,
       description = 'Coriolis is a free database, placement tool and routing tool for VLSI design.',
       author = ', '.join(["Rémy ESCASSUT & Christian",
                           "Christophe ALEXANDRE",
                           "Sophie BELLOEIL",
                           "Damien DUPUIS",
                           "Jean-Paul CHAPUT"
                           ]),
       #author_email = 'martin@v.loewis.de',
       url = 'https://www-soc.lip6.fr/sesi-docs/coriolis2-docs/coriolis2/en/latex/users-guide/UsersGuide.pdf',
       long_description = '''
Coriolis provides several tools to perform the layout of VLSI circuits.
Its main components are the Hurricane database, the Etesian placer and
the Katana router, but other tools can use the Hurricane database and
the parsers provided.
''',
   install_requires=['platformdirs'],
   package_dir={"coriolis":os.path.join(coriolis_python_path,'coriolis'),
                "coriolis.libs":coriolis_lib_path,
                "coriolis.bin":coriolis_bin_path,
                "coriolis.cmds":os.path.join(setup_path,"coriolis_cmds"),
                },
   packages=list(_coriolis_packages())+["coriolis.libs", "coriolis.bin", "coriolis.cmds"],
   package_data={"":["*.so","*.so.*"],"coriolis.bin":['*']},
   entry_points = {
        'console_scripts': ['coriolis=coriolis.cmds.coriolis:run'],
   }
)
