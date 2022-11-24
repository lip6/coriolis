import os
import platform
import re
import subprocess
import sys
import sysconfig
from distutils.version import LooseVersion
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
                raise RuntimeErrorPython_CORIOLISDIR(
                    "CMake must be installed to build the following extensions: "
                    + ", ".join(e.name for e in cmake_extensions)
                )
            if platform.system() == "Windows":
                cmake_version = LooseVersion(re.search(r"version\s*([\d.]+)", out.decode()).group(1))  # type: ignore
                if cmake_version < "3.1.0":
                    raise RuntimeError("CMake >= 3.1.0 is required on Windows")

    def build_cmake_extension(self, ext: CMakeExtension) -> None:
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ["-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=" + extdir, "-DPYTHON_EXECUTABLE=" + sys.executable]

        cfg = "Debug" if self.debug else "Release"
        # cfg = 'Debug'
        build_args = ["--config", cfg]
        install_args = ["--config", cfg]

        if platform.system() == "Windows":
            cmake_args += ["-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)]
            if sys.maxsize > 2 ** 32:
                cmake_args += ["-A", "x64"]
            build_args += ["--", "/m"]
        else:
            cmake_args += ["-DCMAKE_BUILD_TYPE=" + cfg]
            build_args += ["--", "-j4", "VERBOSE=1"]
        cmake_args += ["-DPYTHON_INCLUDE_DIR={}".format(sysconfig.get_path("include"))]

        env = os.environ.copy()
        env["CXXFLAGS"] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get("CXXFLAGS", ""), self.distribution.get_version())

        build_dir = os.path.join(self.build_temp, ext.sourcedir_rel)
        install_dir = os.path.join(self.build_temp, "install")
        os.makedirs(build_dir,exist_ok=True)
        os.makedirs(os.path.join(install_dir, cfg), exist_ok=True)

        cmake_args += [f"-DCMAKE_MODULE_PATH={os.path.abspath('bootstrap/cmake_modules')};"
                       f"{os.path.join(install_dir, 'share/cmake/Modules')}"]
        cmake_args += [f"-DCMAKE_INSTALL_PREFIX={install_dir}"]
        cmake_args += [f"-DPython_CORIOLISLIB={extdir}"]
        cmake_args += [f"-DPython_CORIOLISARCH={extdir}"]
    

        subprocess.check_call(["cmake", "--debug-find", "--trace-redirect=build.cmake.trace", "--trace-expand",  ext.sourcedir] + cmake_args, cwd=build_dir, env=env)
        subprocess.check_call(["cmake", "--build", "."] + build_args, cwd=build_dir)
        subprocess.check_call(["cmake", "--install", ".", "--prefix", install_dir] + install_args, cwd=build_dir)


def build(setup_kwargs: Dict[str, Any]) -> None:
    cmake_modules = [CMakeExtension("coriolis.hurricane", sourcedir="coriolis/hurricane"),
                     CMakeExtension("coriolis.crlcore", sourcedir="coriolis/crlcore")]
    ext_modules = cmake_modules
    setup_kwargs.update(
        {
            "ext_modules": ext_modules,
            "cmdclass": dict(build_ext=ExtensionBuilder),
            "zip_safe": False,
        }
    )
