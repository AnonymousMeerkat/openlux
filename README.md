openlux
=======

Adjusts screen color temperature, similar to f.lux

## Prerequisites ##

Required software:

 * CMake (>=2.6, compilation only)
 * GCC or Clang (compilation only)
 * One of these:
   * X11, with support for the Xf86VM extension (header files needed for compilation)
   * iOS >= 4

## Installation ##

If you wish to compile for iOS, check the wiki for instructions.

Compilation:

    mkdir build
    cd build

    cmake ..
    make

The generated program will be located in `build/src/openlux`. If you wish to install:

    sudo make install
