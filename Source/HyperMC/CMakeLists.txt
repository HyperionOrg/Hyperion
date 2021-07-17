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
        src/main.cpp
        src/Core/Logger.cpp
        src/Core/Player.cpp
        src/Network/Server.cpp)

set(HEADERS
        include/Core/Logger.hpp
        include/Core/Player.hpp
        include/Network/Server.hpp
        include/Utilities/Prerequisites.hpp)

add_executable(HyperMC ${SOURCES} ${HEADERS})
target_compile_features(HyperMC PRIVATE cxx_std_17)
target_include_directories(HyperMC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HyperMC PRIVATE Threads::Threads asio fmt)
target_compile_options(HyperMC PRIVATE ${STANDARD_FLAGS} ${WARNING_FLAGS})
