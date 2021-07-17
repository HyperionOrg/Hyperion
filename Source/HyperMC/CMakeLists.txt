#-------------------------------------------------------------------------------------------
# Copyright (c) 2020-2021, SkillerRaptor <skillerraptor@protonmail.com>
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------
# CMake Info
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------------------
# HyperMC
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/main.cpp)

add_executable(HyperMC ${SOURCES})
target_include_directories(HyperMC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HyperMC PRIVATE asio ${CMAKE_THREAD_LIBS_INIT})
