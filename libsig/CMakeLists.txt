########################################################################
# Project setup
########################################################################
cmake_minimum_required (VERSION 2.8)
project(vs-simd CXX C)
enable_testing()
#select the release build type by default to get optimization flags
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release")
   message(STATUS "Build type not specified: defaulting to release.")
endif(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

########################################################################
# Compiler specific setup
########################################################################
#set(CMAKE_C_COMPILER "$ENV{LLVM}/bin/clang")
#set(CMAKE_CXX_COMPILER "$ENV{LLVM}/bin/clang++")

if(CMAKE_COMPILER_IS_GNUCXX AND NOT WIN32)
  #http://gcc.gnu.org/wiki/Visibility
  add_definitions(-fvisibility=hidden)
endif()

########################################################################
# Find vs-simd build dependencies
########################################################################
#find_package(CppUnit)
#find_package(FFTW3f)

########################################################################
# Setup the include and linker paths
########################################################################
include_directories(
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/lib
    ${CMAKE_BINARY_DIR}/include
    #${Boost_INCLUDE_DIRS}
    #${CPPUNIT_INCLUDE_DIRS}
    )

link_directories(
    #${Boost_LIBRARY_DIRS}
    #${CPPUNIT_LIBRARY_DIRS}
    )

set(CMAKE_CXX_FLAGS "-Wall -fno-exceptions -O3 -msse2 -fno-builtin" CACHE STRING "C++ common flags" FORCE)
#set(CMAKE_CXX_FLAGS_DEBUG "-O1 -g" CACHE STRING "C++ flags (debug) " FORCE)
#set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "C++ flags (release)" FORCE)
#set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG" CACHE STRING "C++ flags (release+size)" FORCE)
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG" CACHE STRING "C++ flags (release+debuginfo)" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt4 REQUIRED QtGui QtCore)
INCLUDE(${QT_USE_FILE})
include_directories( $ENV{QWT}/include ) 
include_directories( $ENV{VSPLOT} )

add_library( vs-simd
  TSC
  sig_eltwise
  sig_scalar 
  sig_complexd 
  sig_complex 
  sig_gui
  sig_random  
  sig_dft
  sig_blas
  sig_func  
  memalloc
  )
target_link_libraries(vs-simd $ENV{VSPLOT}/bin/libvsplot.a ${QT_LIBRARIES} $ENV{QWT}/lib/libqwt.so)

add_executable(t_AlignedArray ${PROJECT_SOURCE_DIR}/t_AlignedArray.cc)
target_link_libraries(t_AlignedArray vs-simd)
