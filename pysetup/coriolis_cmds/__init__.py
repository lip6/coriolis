# preload shared libraries
import os, ctypes

__libs=[]
__lib_folder=os.path.join(os.path.dirname(os.path.realpath(__file__)),'..','libs')
for name in (
    "libdef.so",
    "libpyflute.so",
    "libcoloquinte.so",
    "libhurricane.so",
    "libutils.so",
    "libpycrlconst.so",
    "libconfiguration.so",
    "libpytypemanager.so",
    "libflute.so",
    "liblef.so",
    "libdefzlib.so",
    "libisobar.so",
    "libintervalTree.so",
    "libviewer.so",
    "liblefzlib.so",
    "libanalog.so",
    "libcrlcore.so",
    "libtutorial.so",
    "libequinox.so",
    "libpycrlcore.so",
    "liblibmanager.so",
    "libetesian.so",
    "libsolstice.so",
    "libunicorn.so",
    "libanabatic.so",
    "libkatana.so",
    "libpyanabatic.so",
    "libbora.so",
    "libpybora.so",

    ):
    __libs.append(ctypes.CDLL(os.path.join(__lib_folder,name)))
