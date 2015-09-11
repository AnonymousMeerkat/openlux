cmake_minimum_required(VERSION 2.6)


### Meta ###

project(openlux)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
add_definitions("-DVERSION_STR=\"${VERSION_STR}\"")


### Options ###

option(USE_IOS "Build for iOS if environment detected" ON)
option(USE_X11 "Build with X11 support" ON)


### Project variables ###

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

set(CMAKE_BUILD_TYPE "Debug")


### Check system ###

find_package(PkgConfig)
if (PKG_CONFIG_FOUND)

  # Check for X11
  find_package(X11)
  if (X11_FOUND)
    pkg_check_modules(XF86VM xxf86vm)

    if (XF86VM_FOUND)
      set(HAS_X11 on)
    endif (XF86VM_FOUND)
  endif(X11_FOUND)

endif (PKG_CONFIG_FOUND)

# Hacky way to find if we're building for iOS (requires a toolchain with SDK_ROOT
#   pointing to the root directory of the iOS SDK)
if (EXISTS "${SDK_ROOT}/System/Library/PrivateFrameworks/IOMobileFramebuffer.framework")
  set(HAS_IOS on)

  set(CMAKE_OSX_SYSROOT ${SDK_ROOT})
  include_directories(${SDK_ROOT}/usr/include)

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F${SDK_ROOT}/System/Library/PrivateFrameworks -framework IOKit -framework IOMobileFramebuffer")
endif ()

message("X11: ${HAS_X11}")
message("iOS: ${HAS_IOS}")

if (HAS_IOS AND USE_IOS)
  message("Building for iOS")
  set(BUILD_IOS on)
  add_definitions("-DOL_USE_IOS")
else ()
  if (HAS_X11 AND USE_X11)
    message("Building with X11 support")
    set(BUILD_X11 on)
    add_definitions("-DOL_USE_X11")
  endif ()
endif ()


add_subdirectory(src)
