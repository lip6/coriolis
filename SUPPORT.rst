
=================
Help and tooltips
=================


Compilation errors
==================

This section is here to help you with the build process, and help you fix dependency issues or workaround compilation errors if it happens on your distribution.


RapidJson
---------

Some versions of RapidJson do not build with newer compilers, while others build only with newer ones.
The version provided in README.rst should work for older compilers. Otherwise, consider pulling a newer version.

Qt5
---

To use Qt5, you should add the --qt5 option to the ccb.py line in Makefile.
On some distributions, you may need to replace the lines in bootstrap/cmake_modules/FindBootstrap.cmake from ::
    find_package(Qt5Core         REQUIRED)
to ::
    find_package(Qt5 COMPONENTS REQUIRED Core)

