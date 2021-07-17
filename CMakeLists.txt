#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------
# CMake Info
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(HyperMC)

#-------------------------------------------------------------------------------------------
# Global Flags
#-------------------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------------------------------------------------------------------------------------------
# Properties
#-------------------------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------------------
# Variables
#-------------------------------------------------------------------------------------------
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(BUILD_TYPE "Debug")
else ()
    set(BUILD_TYPE "Release")
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(COMPILER "Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(COMPILER "GNU")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(COMPILER "MSVC")
endif ()

#-------------------------------------------------------------------------------------------
# Flags
#-------------------------------------------------------------------------------------------
if ("${COMPILER}" STREQUAL "MSVC")
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP8")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_DEPRECATE /D_SCL_SECURE_NO_WARNINGS")
endif ()

if ("${BUILD_TYPE}" STREQUAL "Debug")
    if ("${COMPILER}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -D_GLIBCXX_DEBUG")
    elseif ("${COMPILER}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    elseif ("${COMPILER}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /Zi")
    endif ()
elseif ("${BUILD_TYPE}" STREQUAL "Release")
    if ("${COMPILER}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -m64 -mavx2 -mfma -ffast-math")
    elseif ("${COMPILER}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -m64 -mavx2 -mfma -ffast-math")
    elseif ("${COMPILER}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /arch:AVX2 /fp:fast")
    endif ()
endif ()

#-------------------------------------------------------------------------------------------
# Source
#-------------------------------------------------------------------------------------------
add_subdirectory(Source)
