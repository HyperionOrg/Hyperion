cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(HYPERION_NAME "Hyperion")
project(${HYPERION_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -g3 -O0 -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS}   -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS}  /std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX")
endif()

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/json")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/spdlog")
set_target_properties(spdlog PROPERTIES FOLDER "${CMAKE_SOURCE_DIR}/vendor/spdlog")

file(GLOB_RECURSE "HYPERION_SOURCE" "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.h")

foreach(item IN ITEMS ${HYPERION_SOURCE})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE "/" "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
endforeach()

message(STATUS "Searching for OpenSSL")
set(OPENSSL_USE_STATIC_LIBS True)
find_package(OpenSSL REQUIRED QUIET)

if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found.")
else()
    message(STATUS "Found OpenSSL")
endif()

add_executable("${HYPERION_NAME}" "${HYPERION_SOURCE}")

target_include_directories("${HYPERION_NAME}" PRIVATE
                            "${CMAKE_SOURCE_DIR}/src/"
                            "${CMAKE_SOURCE_DIR}/src/Hyperion"
                            "${CMAKE_SOURCE_DIR}/vendor/asio/asio/include"
                            "${CMAKE_SOURCE_DIR}/vendor/json/single_include"
                            "${CMAKE_SOURCE_DIR}/vendor/spdlog/include"
                            "${OPENSSL_INCLUDE_DIR}")

target_link_libraries("${HYPERION_NAME}" PRIVATE OpenSSL::SSL OpenSSL::Crypto spdlog::spdlog crypt32.lib)
