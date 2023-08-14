#This is needed for poetry to recognise the top level module
import os
import sys
import subprocess

__version__ = "0.0.0"

#TODO not PEP302 complient -probably a big porting job
coriolis_package_dir = os.path.abspath(os.path.dirname(__file__))

os.environ["CORIOLIS_TOP"] = coriolis_package_dir
CORIOLIS_DATA = os.path.join(os.path.dirname(__file__), 'data')
CORIOLIS_BIN = os.path.join(CORIOLIS_DATA,"bin")

def _program(name, args):
    return subprocess.call([os.path.join(CORIOLIS_BIN, name)] + args, close_fds=False)

def blif2vst():
    raise SystemExit(_program("blif2vst.py", sys.argv[1:]))

def cx2y():
    raise SystemExit(_program("cx2y", sys.argv[1:]))

def cyclop():
    raise SystemExit(_program("cyclop", sys.argv[1:]))

def tutorial():
    raise SystemExit(_program("tutorial", sys.argv[1:]))

def unittests():
    raise SystemExit(_program("unittests", sys.argv[1:]))

def yosys_coriolis():
    raise SystemExit(_program("yosys.py", sys.argv[1:]))

def cgt():
    raise SystemExit(_program("cgt", sys.argv[1:]))
