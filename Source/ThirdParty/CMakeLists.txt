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
set_target_properties(asio PROPERTIES FOLDER ThirdParty)
target_compile_definitions(asio PRIVATE ASIO_SEPARATE_COMPILATION=1)
target_include_directories(asio PUBLIC ${OPENSSL_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include)
target_link_libraries(asio PUBLIC ${OPENSSL_LIBRARIES})

#-------------------------------------------------------------------------------------------
# fmt
#-------------------------------------------------------------------------------------------
set(FMT_PEDANTIC ON CACHE BOOL "" FORCE)
set(FMT_WERROR OFF CACHE BOOL "" FORCE)
set(FMT_DOC OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
set(FMT_TEST OFF CACHE BOOL "" FORCE)
set(FMT_FUZZ OFF CACHE BOOL "" FORCE)
set(FMT_CUDA_TEST OFF CACHE BOOL "" FORCE)
set(FMT_OS ON CACHE BOOL "" FORCE)
set(FMT_MODULE OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/fmt)
set_target_properties(fmt PROPERTIES FOLDER ThirdParty)
