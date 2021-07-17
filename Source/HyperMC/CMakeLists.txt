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
        src/Network/Connection.cpp
        src/Network/Server.cpp
        src/Packets/Packet.cpp
        src/Packets/PacketsIn/PacketInHandshake.cpp
        src/Utilities/VarInt.cpp
        src/Utilities/VarLong.cpp)

set(HEADERS
        include/Core/Logger.hpp
        include/Core/Player.hpp
        include/Network/Connection.hpp
        include/Network/Server.hpp
        include/Packets/Packet.hpp
        include/Packets/PacketsIn/PacketInHandshake.hpp
        include/Utilities/Prerequisites.hpp
        include/Utilities/VarInt.hpp
        include/Utilities/VarLong.hpp)

add_executable(HyperMC ${SOURCES} ${HEADERS})
target_compile_features(HyperMC PRIVATE cxx_std_17)
target_include_directories(HyperMC PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HyperMC PRIVATE Threads::Threads asio fmt)
target_compile_options(HyperMC PRIVATE ${STANDARD_FLAGS} ${WARNING_FLAGS})
