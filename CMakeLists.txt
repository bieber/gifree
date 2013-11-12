cmake_minimum_required (VERSION 2.6.2)
project (gifree)

set (GIFREE_VERSION_MAJOR 0)
set (GIFREE_VERSION_MINOR 1)
configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/gifree_cmake_settings.h.in"
  "${PROJECT_BINARY_DIR}/gifree_cmake_settings.h"
)
include_directories("${PROJECT_BINARY_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

add_subdirectory(src)
FILE(GLOB_RECURSE SOURCES src/*.c)
FILE(GLOB_RECURSE HEADERS src/*.h)

set (INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

include_directories(${INCLUDE_DIRS})
add_executable(gifree ${SOURCES})
target_link_libraries(gifree ${GTK3_LIBRARIES})
