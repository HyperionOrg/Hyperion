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
# asio
#-------------------------------------------------------------------------------------------
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include/*.hpp)

add_library(asio STATIC ${SOURCES} ${HEADERS})
target_compile_definitions(asio PRIVATE ASIO_SEPARATE_COMPILATION=1)
target_include_directories(asio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include ${OPENSSL_INCLUDE_DIR})
target_link_libraries(asio PUBLIC ${OPENSSL_LIBRARIES})
