cmake_minimum_required(VERSION 2.6)
project(openlux)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
add_definitions("-DVERSION_STR=\"${VERSION_STR}\"")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")

set(CMAKE_BUILD_TYPE "Debug")

find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(XF86VM REQUIRED xxf86vm)

add_subdirectory(src)
