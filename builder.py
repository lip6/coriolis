import io
import glob
import ninja
import os
import platform
import re
import subprocess
import sys

from distutils.version import LooseVersion
from distutils.dir_util import copy_tree, remove_tree
from distutils.sysconfig import get_python_inc
from distutils import sysconfig
from typing import Any, Dict

from setuptools.command.build_ext import build_ext
from setuptools.extension import Extension


class CMakeExtension(Extension):
    name: str  # exists, even though IDE doesn't find it

    def __init__(self, name: str, sourcedir: str="") -> None:
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)
        self.sourcedir_rel = sourcedir


class ExtensionBuilder(build_ext):
    def run(self) -> None:
        self.validate_cmake()
        super().run()

    def build_extension(self, ext: Extension) -> None:
        if isinstance(ext, CMakeExtension):
            self.build_cmake_extension(ext)
        else:
            super().build_extension(ext)

    def validate_cmake(self) -> None:
        cmake_extensions = [x for x in self.extensions if isinstance(x, CMakeExtension)]
        if len(cmake_extensions) > 0:
            try:
                out = subprocess.check_output(["cmake", "--version"])
            except OSError:
                raise RuntimeError(
                    "CMake must be installed to build the following extensions: "
                    + ", ".join(e.name for e in cmake_extensions)
                )
            if platform.system() == "Windows":
                cmake_version = LooseVersion(re.search(r"version\s*([\d.]+)", out.decode()).group(1))  # type: ignore
                if cmake_version < "3.1.0":
                    raise RuntimeError("CMake >= 3.1.0 is required on Windows")

    def build_cmake_extension(self, ext: CMakeExtension) -> None:
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = []

        ninja_executable_path = os.path.join(ninja.BIN_DIR, "ninja")
        cmake_args += ["-GNinja","-DCMAKE_MAKE_PROGRAM:FILEPATH=" + ninja_executable_path]

        cfg = "Debug" if self.debug else "Release"
        # cfg = 'Debug'
        build_args = ["--config", cfg, "-j", "4"]
        install_args = ["--config", cfg]

        cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg]
        if platform.system() == "Windows":
            if sys.maxsize > 2 ** 32:
                cmake_args += ["-A", "x64"]

        env = os.environ.copy()
        env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get("CXXFLAGS", ""), self.distribution.get_version())
        
        build_dir = os.path.join(self.build_temp, ext.sourcedir_rel)
        install_dir = os.path.join(extdir, 'coriolis')
        os.makedirs(build_dir,exist_ok=True)

        if "USE_CCACHE" in env:
            cmake_args += ["-DCMAKE_CXX_COMPILER_LAUNCHER=ccache"]

        cmake_args += [f"-DCMAKE_MODULE_PATH={os.path.abspath('bootstrap/cmake_modules')};"
                       f"{os.path.join(install_dir, 'share/cmake/Modules')}"]
        cmake_args += [f"-DCMAKE_INSTALL_PREFIX={install_dir}"]
        cmake_args += [f"-DPython_CORIOLISLIB={install_dir}"]
        cmake_args += [f"-DPython_CORIOLISARCH={install_dir}"]
        cmake_args += [f"-DSYS_CONF_DIR={install_dir}"]
        cmake_args += [f"-DCORIOLIS_TOP={install_dir}"]
        cmake_args += [f"-DCORIOLIS_USER_TOP={install_dir}"]
        cmake_args += [f"-DUSE_MANYLINUX=TRUE"]

        cmake_args += [f"-DPython_EXECUTABLE={sys.executable}"]

        cmake_args += ["-DPOETRY=1"]
        cmake_args += ["-DWITH_QT5=1"]
        cmake_args += ["-DBUILD_DOC=TRUE"]
        cmake_args += ["-DCMAKE_BUILD_RPATH_USE_ORIGIN=1"]
        cmake_args += ["-DCMAKE_SKIP_BUILD_RPATH=FALSE"]
        cmake_args += ["-DCMAKE_BUILD_WITH_INSTALL_RPATH=FALSE"]
        cmake_args += ["-DCMAKE_INSTALL_RPATH='$ORIGIN/lib:$ORIGIN'"]
        cmake_args += ["-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE"]


        print(f"Using cmake args: {cmake_args}")
        subprocess.check_call(["cmake", "--trace-redirect=build.cmake.trace", "--trace-expand",  ext.sourcedir] + cmake_args, cwd=build_dir, env=env)
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=build_dir)
        subprocess.check_call(["cmake", "--install", ".", "--prefix", install_dir] + install_args, cwd=build_dir)
        if os.path.exists(os.path.join(install_dir, "bin")):
            copy_tree(os.path.join(install_dir, "bin"), os.path.join(install_dir,"data/bin"))
            remove_tree(os.path.join(install_dir, "bin"))
            proc = subprocess.Popen(['file'] + glob.glob(os.path.join(install_dir,"data/bin/*")), stdout=subprocess.PIPE)
            for line in io.TextIOWrapper(proc.stdout, encoding="utf-8"):  
                if 'ELF' in line: #TODO support other OSs
                    f = line.split(':')[0]
                    print(f"fixing up {f}")
                    subprocess.check_call(["patchelf", "--set-rpath", '$ORIGIN/../../lib', f])

        # Remove docs
        if os.path.exists(os.path.join(install_dir, "share", "doc")):
            remove_tree(os.path.join(install_dir, "share", "doc"))

def build(setup_kwargs: Dict[str, Any]) -> None:
    cmake_modules = [
                     CMakeExtension("coloquinte", sourcedir="coloquinte"),
                     CMakeExtension("lefdef", sourcedir="lefdef"),
                     CMakeExtension("Hurricane", sourcedir="hurricane"),
                     CMakeExtension("crlcore", sourcedir="crlcore"),
                     CMakeExtension("flute", sourcedir="flute"),
                     CMakeExtension("etesian", sourcedir="etesian"),
                     CMakeExtension("anabatic", sourcedir="anabatic"),
                     CMakeExtension("katana", sourcedir="katana"),
                     CMakeExtension("equinox", sourcedir="equinox"),
                     CMakeExtension("solstice", sourcedir="solstice"),
                     CMakeExtension("oroshi", sourcedir="oroshi"),
                     CMakeExtension("bora", sourcedir="bora"),
                     CMakeExtension("karakaze", sourcedir="karakaze"),
                     #CMakeExtension("knik", sourcedir="knik"),
                     CMakeExtension("unicorn", sourcedir="unicorn"),
                     CMakeExtension("tutorial", sourcedir="tutorial"),
                     CMakeExtension("cumulus", sourcedir="cumulus"),
                     CMakeExtension("stratus1", sourcedir="stratus1"),
                     CMakeExtension("documentation", sourcedir="documentation"),
                     CMakeExtension("unittests", sourcedir="unittests")
                     ]
 
    ext_modules = cmake_modules
    setup_kwargs.update(
        {
            "ext_modules": ext_modules,
            "cmdclass": dict(build_ext=ExtensionBuilder),
            "zip_safe": False,
        }
    )
